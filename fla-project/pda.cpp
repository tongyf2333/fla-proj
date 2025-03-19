#include"pda.h"
pda_trans_from::pda_trans_from(string state,char in_,char top_){
    cur_state=state,input=in_,now_top=top_;
}
pda_trans_to::pda_trans_to(string state,string top_){
    new_state=state,new_top=top_;
}
bool PDA::transfer(string input_string){
    cur_state=start_state;
    while(!my_stack.empty()) my_stack.pop();
    my_stack.push(start_stack);
    int cnt=0;
    for(int i=0;i<input_string.size();i++){
        char now=input_string[i],top_;
        if(my_stack.empty()) top_='_';
        else top_=my_stack.top();
        pda_trans_from cur(cur_state,now,top_);
        //cout<<i<<" "<<cur_state<<" "<<now<<" "<<top_<<endl;
        if(trans.find(cur)!=trans.end()){//non empty transfer
            //cout<<i<<" "<<cur_state<<" "<<now<<" "<<top_<<" "<<trans[cur].new_state<<" "<<trans[cur].new_top<<endl;
            if(top_!='_') my_stack.pop();
            if(trans[cur].new_top=="_"){}
            else{
                for(int j=trans[cur].new_top.size()-1;j>=0;j--){
                    //cout<<"insert:"<<trans[cur].new_top[j]<<endl;
                    my_stack.push(trans[cur].new_top[j]);
                }
            }
            cur_state=trans[cur].new_state;
            cnt++;
        }
        else{//empty transfer
            cur.input='_';
            if(trans.find(cur)!=trans.end()){
                my_stack.pop();
                for(int j=trans[cur].new_top.size()-1;j>=0;j--)
                    my_stack.push(trans[cur].new_top[j]);
                cur_state=trans[cur].new_state;
                i--;
            }
            else break;
        }
    }
    while(1){//rest empty transfer
        char top_;
        if(my_stack.empty()) top_='_';
        else top_=my_stack.top();
        pda_trans_from cur=pda_trans_from(cur_state,'_',top_);
        if(trans.find(cur)!=trans.end()){
            if(top_!='_') my_stack.pop();
            if(trans[cur].new_top=="_"){}
            else{
                for(int j=trans[cur].new_top.size()-1;j>=0;j--)
                    my_stack.push(trans[cur].new_top[j]);
            }
            cur_state=trans[cur].new_state;
        }
        else break;
        if(final_states.find(cur_state)!=final_states.end()) return 1;
    }
    if(cnt!=input_string.size()) return 0;
    if(final_states.find(cur_state)!=final_states.end()) return 1;
    return 0;
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
PDA::PDA(char *file_name){
    ifstream file(file_name);
    if(!file){
        cerr<<"syntax error"<<endl;
        exit(1);
    }
    string line;
    int has_q=0,has_s=0,has_g=0,has_q0=0,has_z0=0,has_f=0;
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
        else if (line.rfind("#G", 0) == 0) {//#G={... , ...} stack symbols
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
                for (const auto& i : items) stack_set.insert(trim(i)[0]);
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
        else if (line.rfind("#z0", 0) == 0) {//#z0= start stack
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
                start_stack=trim(value)[0];
                if(stack_set.find(start_stack)==stack_set.end()){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                has_z0=1;
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
                if(input_set.find(parts[1][0])==input_set.end()){
                    if(parts[1][0]!='_'){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                }
                if(stack_set.find(parts[2][0])==stack_set.end()){
                    if(parts[2][0]!='_'){
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                }
                if(states.find(parts[3])==states.end()){
                    cerr<<"syntax error"<<endl;
                    exit(1);
                }
                for(int j=0;j<parts[4].size();j++){
                    if(stack_set.find(parts[4][j])==stack_set.end()){
                        if(parts[4][j]=='_') continue;
                        cerr<<"syntax error"<<endl;
                        exit(1);
                    }
                }
                pda_trans_from from(parts[0],parts[1][0],parts[2][0]);
                pda_trans_to to(parts[3],parts[4]);
                trans[from]=to;
            }
            else{
                cerr<<"syntax error"<<endl;
                exit(1);
            }
        }
    }
    if(has_q==0||has_s==0||has_g==0||has_q0==0||has_z0==0||has_f==0){
        cerr<<"syntax error"<<endl;
        exit(1);
    }
    for(auto it=final_states.begin();it!=final_states.end();it++){
        if(states.find(*it)==states.end()){
            cerr<<"syntax error"<<endl;
            exit(1);
        }
    }
    file.close();
}