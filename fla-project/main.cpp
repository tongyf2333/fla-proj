#include "pda.h"
#include "tm.h"
using namespace std;
int main(int argc, char* argv[]){
    char *filename,*inputstring;
    int verbose=0;
    if(argc<=1){
        cerr<<"too few arguments"<<endl;
        exit(1);
    }
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"--verbose")==0||strcmp(argv[i],"-v")==0){
            verbose=1;
            break;
        }
    }
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"--help")==0||strcmp(argv[i],"-h")==0){
            cerr<<"usage:   fla [-h|--help] <pda> <input>"<<endl;
            cerr<<"         fla [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
            exit(1);
        }
    }
    int cnt=0;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--verbose")==0||strcmp(argv[i],"-v")==0) continue;
        if(cnt==0) cnt++,filename=argv[i];
        else if(cnt==1) cnt++,inputstring=argv[i];
        else{
            cerr<<"too many arguments"<<endl;
            exit(1);
        }
    }
    if(cnt!=2){
        cerr<<"too few arguments"<<endl;
        exit(1);
    }
    int len=strlen(filename);
    if(filename[len-4]=='.'&&filename[len-3]=='p'&&filename[len-2]=='d'&&filename[len-1]=='a'){
        PDA pda(filename);
        string input=inputstring;
        for(int i=0;i<input.size();i++){
            if(pda.input_set.find(input[i])==pda.input_set.end()){
                cerr<<"illegal input"<<endl;
                exit(1);
            }
        }
        bool res=pda.transfer(input);
        if(res) cout<<"true"<<endl;
        else cout<<"false"<<endl;
    }
    if(filename[len-3]=='.'&&filename[len-2]=='t'&&filename[len-1]=='m'){
        TM tm(filename);
        string input=inputstring;
        if(verbose){
            cout<<"Input: "<<input<<endl;
            for(int i=0;i<input.size();i++){
                if(tm.input_set.find(input[i])==tm.input_set.end()){
                    cerr<<"==================== ERR ===================="<<endl;
                    cerr<<"error: '"<<input[i]<<"' was not declared in the set of input symbols"<<endl;
                    cerr<<"Input: "<<input<<endl;
                    cerr<<"       ";
                    for(int j=0;j<i;j++) cerr<<" ";
                    cerr<<"^"<<endl;
                    cerr<<"==================== END ===================="<<endl;
                    exit(1);
                }
            }
            cout<<"==================== RUN ===================="<<endl;
        }
        else{
            for(int i=0;i<input.size();i++){
                if(tm.input_set.find(input[i])==tm.input_set.end()){
                    cerr<<"illegal input"<<endl;
                    exit(1);
                }
            }
        }
        string ans=tm.transfer(input,verbose);
        if(verbose){
            cout<<"Result: "<<ans<<endl;
            cout<<"==================== END ===================="<<endl;
        }
        else cout<<ans<<endl;
    }
    return 0;
}