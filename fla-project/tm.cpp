#include"tm.h"
tm_trans_from::tm_trans_from(string state,string in_){
    cur_state=state,cur_tape=in_;
}
tm_trans_to::tm_trans_to(string state,string out_,string mov_){
    new_state=state,new_tape=out_,mov=mov_;
}
bool cmp(string a,string b){
    if(a.size()!=b.size()) return 0;
    for(int i=0;i<a.size();i++){
        if(b[i]=='*'){
            if(a[i]=='_') return 0;
            else continue;
        }
        else if(a[i]==b[i]) continue;
        else return 0;
    }
    return 1;
}
string TM::transfer(string input_string,int verbose){
    for(int i=0;i<tape_cnt;i++){
        tape[i]=(char*)malloc(TAPE_LEN_MAX*sizeof(char));
        memset(tape[i],'_',TAPE_LEN_MAX*sizeof(char));
        tape_pos[i]=INIT_TAPE_POS;
    }
    for(int i=0;i<input_string.size();i++){
        tape[0][i+INIT_TAPE_POS]=input_string[i];
    }
    cur_state=start_state;
    int step=0;
    while(1){
        if(verbose) print_state(step);
        string cur;
        for(int i=0;i<tape_cnt;i++) cur+=tape[i][tape_pos[i]];
        int flag=0;
        for(auto it=trans.begin();it!=trans.end();it++){
            if(it->first.cur_state==cur_state&&cmp(cur,it->first.cur_tape)){
                string new_state=it->second.new_state,new_tape=it->second.new_tape,mov=it->second.mov;
                cur_state=new_state;
                for(int i=0;i<new_tape.size();i++){
                    if(new_tape[i]=='*') continue;
                    else tape[i][tape_pos[i]]=new_tape[i];
                }
                for(int i=0;i<mov.size();i++){
                    if(mov[i]=='l') tape_pos[i]--;
                    else if(mov[i]=='r') tape_pos[i]++;
                }
                flag=1;
                break;
            }
        }
        if(flag==0) break;
        step++;
    }
    int leftmost=-1,rightmost=-1;
    for(int i=TAPE_LEN_MAX-1;i>0;i--){
        if(tape[0][i]!='_'){
            rightmost=i;
            break;
        }
    }
    for(int i=0;i<TAPE_LEN_MAX;i++){
        if(tape[0][i]!='_'){
            leftmost=i;
            break;
        }
    }
    string res;
    if(leftmost==-1&&rightmost==-1) res="";
    else for(int i=leftmost;i<=rightmost;i++) res+=tape[0][i];
    return res;
}
static string trim(string str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}
static bool brace(string str) {
    int leftBraceCount = 0;
    int rightBraceCount = 0;
    int leftpos=0,rightpos=0;
    for (int i=0;i<str.size();i++) {
        if (str[i] == '{') {
            if (leftBraceCount == 1) return false;
            leftBraceCount++;
            leftpos=i;
        } else if (str[i] == '}') {
            if (rightBraceCount == 1 || leftBraceCount == 0)  return false;
            rightBraceCount++;
            rightpos=i;
        }
    }
    if(leftBraceCount!=1||rightBraceCount!=1) return false;
    char last;
    int cnt=0;
    for(int i=0;i<leftpos;i++){
        if(str[i]!=' ') last=str[i],cnt++;
    }
    if(cnt!=3||last!='=') return false;
    for(int i=rightpos+1;i<str.size();i++){
        if(str[i]!=' ') return false;
    }
    return true;
}
TM::TM(char *file_name){
    ifstream file(file_name);
    if(!file){
        cerr<<"syntax error"<<endl;
        exit(1);
    }
    string line;
    int has_q=0,has_s=0,has_g=0,has_q0=0,has_b=0,has_f=0,has_n=0;
    while (getline(file, line)) {
        if(line.size()==0) continue;
        size_t semicolonPos = line.find(';');
        if (semicolonPos != std::string::npos) {//ignoring ;
            line = line.substr(0, semicolonPos);
        }
        if(line.size()==0) continue;
        else if(line.rfind(";",0)==0) continue;
        else if (line.rfind("#Q", 0) == 0) {//#Q={... , ...} states
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            if(!brace(line)){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            regex re("\\{([^}]*)\\}");
            smatch match;
            if (regex_search(line, match, re)) {
                string content = match[1];
                vector<string> items;
                string item;
                istringstream iss(content);
                while (getline(iss, item, ',')) items.push_back(item);
                for (const auto& i : items){
                    string ii=trim(i);
                    if(ii.find(' ')!=string::npos){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                    states.insert(ii);
                }
                has_q=1;
            }
        }
        else if (line.rfind("#S", 0) == 0) {//#S={... , ...} input symbols
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            if(!brace(line)){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            regex re("\\{([^}]*)\\}");
            smatch match;
            if (regex_search(line, match, re)) {
                string content = match[1];
                vector<string> items;
                string item;
                istringstream iss(content);
                while (getline(iss, item, ',')) items.push_back(item);
                for (const auto& i : items) input_set.insert(trim(i)[0]);
                has_s=1;
            }
        }
        else if (line.rfind("#G", 0) == 0) {//#G={... , ...} tape symbols
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            if(!brace(line)){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            regex re("\\{([^}]*)\\}");
            smatch match;
            if (regex_search(line, match, re)) {
                string content = match[1];
                vector<string> items;
                string item;
                istringstream iss(content);
                while (getline(iss, item, ',')) items.push_back(item);
                for (const auto& i : items) tape_set.insert(trim(i)[0]);
                has_g=1;
            }
        }
        else if (line.rfind("#F", 0) == 0) {//#F={... , ...} final states
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            if(!brace(line)){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            regex re("\\{([^}]*)\\}");
            smatch match;
            if (regex_search(line, match, re)) {
                string content = match[1];
                vector<string> items;
                string item;
                istringstream iss(content);
                while (getline(iss, item, ',')) items.push_back(item);
                for (const auto& i : items) final_states.insert(trim(i));
                has_f=1;
            }
        }
        else if (line.rfind("#q0", 0) == 0) {//#q0= start state
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            size_t pos = line.find('=');
            int cnt=0;
            for(int j=0;j<pos;j++){
                if(line[j]!=' ') cnt++;
            }
            if(cnt!=3){
                cerr<<"syntax error"<<endl;
                exit(0);
            }
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                size_t start = value.find_first_not_of(" \t");
                size_t end = value.find_last_not_of(" \t");
                value = value.substr(start, end - start + 1);
                start_state=trim(value);
                if(states.find(start_state)==states.end()){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                has_q0=1;
            }
        }
        else if (line.rfind("#N", 0) == 0) {//#N= tape cnt
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            size_t pos = line.find('=');
            int cnt=0;
            for(int j=0;j<pos;j++){
                if(line[j]!=' ') cnt++;
            }
            if(cnt!=2){
                cerr<<"syntax error"<<endl;
                exit(0);
            }
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                size_t start = value.find_first_not_of(" \t");
                size_t end = value.find_last_not_of(" \t");
                value = value.substr(start, end - start + 1);
                string tmp=trim(value);
                for(int j=0;j<tmp.size();j++){
                    if(tmp[j]>='0'&&tmp[j]<='9') continue;
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                tape_cnt=stoi(tmp);
                has_n=1;
            }
        }
        else if (line.rfind("#B", 0) == 0){//#B= blank symbol
            if(line.find('=')==string::npos){
                cerr<<"syntax error"<<endl;
                exit(1);
            }
            size_t pos = line.find('=');
            int cnt=0;
            for(int j=0;j<pos;j++){
                if(line[j]!=' ') cnt++;
            }
            if(cnt!=2){
                cerr<<"syntax error"<<endl;
                exit(0);
            }
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                size_t start = value.find_first_not_of(" \t");
                size_t end = value.find_last_not_of(" \t");
                value = value.substr(start, end - start + 1);
                if(trim(value)!="_"){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                has_b=1;
            }
        }
        else{
            istringstream iss(line);
            vector<string> parts;
            string part;
            while (iss >> part) parts.push_back(part);
            if (parts.size() == 5){
                if(states.find(parts[0])==states.end()){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                if(parts[1].size()!=tape_cnt||parts[2].size()!=tape_cnt||parts[3].size()!=tape_cnt){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                for(int j=0;j<tape_cnt;j++){
                    if(parts[1][j]!='*'&&tape_set.find(parts[1][j])==tape_set.end()){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                    if(parts[2][j]!='*'&&tape_set.find(parts[2][j])==tape_set.end()){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                    if(parts[3][j]!='l'&&parts[3][j]!='r'&&parts[3][j]!='*'){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                }
                if(states.find(parts[4])==states.end()){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                tm_trans_from from(parts[0],parts[1]);
                tm_trans_to to(parts[4],parts[2],parts[3]);
                trans[from]=to;
            }
            else{
                cerr<<"syntax error"<<endl;
                exit(1);
            }
        }
    }
    if(has_q==0||has_s==0||has_g==0||has_q0==0||has_b==0||has_f==0||has_n==0){
        cerr<<"syntax error"<<endl;
        exit(1);
    }
    for(auto it=final_states.begin();it!=final_states.end();it++){
        if(states.find(*it)==states.end()){
            cerr<<"syntax error"<<endl;
            exit(1);
        }
    }
    for(auto it=input_set.begin();it!=input_set.end();it++){
        if(tape_set.find(*it)==tape_set.end()){
            cerr<<"syntax error"<<endl;
            exit(1);
        }
    }
    if(tape_set.find('_')==tape_set.end()){
        cerr<<"syntax error"<<endl;
        exit(1);
    }
    file.close();
}
void TM::print_state(int step){
    cout<<"Step   : "<<step<<endl;
    cout<<"State  : "<<cur_state<<endl;
    for(int i=0;i<tape_cnt;i++){
        int leftmost=-1,rightmost=-1;
        for(int j=0;j<TAPE_LEN_MAX;j++){
            if(tape[i][j]!='_'){
                leftmost=j;
                break;
            }
        }
        for(int j=TAPE_LEN_MAX-1;j>=0;j--){
            if(tape[i][j]!='_'){
                rightmost=j;
                break;
            }
        }
        int lpos,rpos;
        if(leftmost==-1&&rightmost==-1) lpos=tape_pos[i],rpos=tape_pos[i];
        else if(tape_pos[i]<leftmost) lpos=tape_pos[i],rpos=rightmost;
        else if(tape_pos[i]>rightmost) lpos=leftmost,rpos=tape_pos[i];
        else lpos=leftmost,rpos=rightmost;
        if(i<=9){
            cout<<"Index"<<i<<" : ";
            for(int j=lpos;j<=rpos;j++){
                if(j-INIT_TAPE_POS>=0) cout<<(j-INIT_TAPE_POS)<<" ";
                else if(j-INIT_TAPE_POS<0) cout<<(INIT_TAPE_POS-j)<<" ";
            }
            cout<<endl;
            cout<<"Tape"<<i<<"  : ";
            for(int j=lpos;j<=rpos;j++){
                if(j-INIT_TAPE_POS>=-9&&j-INIT_TAPE_POS<=9) cout<<tape[i][j]<<" ";
                else cout<<tape[i][j]<<"  ";
            }
            cout<<endl;
            cout<<"Head"<<i<<"  : ";
            for(int j=lpos;j<=rpos;j++){
                if(j==tape_pos[i]) cout<<"^ ";
                else cout<<"  ";
                if(j-INIT_TAPE_POS>9||j-INIT_TAPE_POS<-9) cout<<" ";
            }
            cout<<endl;
        }
        else{
            cout<<"Index"<<i<<": ";
            for(int j=lpos;j<=rpos;j++){
                if(j-INIT_TAPE_POS>=0) cout<<(j-INIT_TAPE_POS)<<" ";
                else if(j-INIT_TAPE_POS<0) cout<<(INIT_TAPE_POS-j)<<" ";
            }
            cout<<endl;
            cout<<"Tape"<<i<<" : ";
            for(int j=lpos;j<=rpos;j++){
                if(j-INIT_TAPE_POS>=-9&&j-INIT_TAPE_POS<=9) cout<<tape[i][j]<<" ";
                else cout<<tape[i][j]<<"  ";
            }
            cout<<endl;
            cout<<"Head"<<i<<" : ";
            for(int j=lpos;j<=rpos;j++){
                if(j==tape_pos[i]) cout<<"^ ";
                else cout<<"  ";
                if(j-INIT_TAPE_POS>9||j-INIT_TAPE_POS<-9) cout<<" ";
            }
            cout<<endl;
        }
    }
    cout<<"---------------------------------------------"<<endl;
}