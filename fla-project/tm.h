#include<bits/stdc++.h>
#define TAPE_LEN_MAX 200000
#define INIT_TAPE_POS 100000
using namespace std;
class tm_trans_from{
    public:
    string cur_state,cur_tape;
    tm_trans_from(){}
    tm_trans_from(string state,string in_);
    bool operator < (const tm_trans_from &x) const{
        return (cur_state+cur_tape)<(x.cur_state+x.cur_tape);
    }
};
class tm_trans_to{
    public:
    string new_state,new_tape,mov;
    tm_trans_to(){}
    tm_trans_to(string state,string out_,string mov_);
};
class TM{
    public:
    set<string> states,final_states;
    string cur_state,start_state;
    set<char> input_set,tape_set;
    int tape_pos[100],tape_cnt;
    char *tape[100];
    map<tm_trans_from,tm_trans_to> trans;
    TM(char *file_name);
    string transfer(string input_string,int verbose);
    void print_state(int step);
};