#include<bits/stdc++.h>
using namespace std;
class pda_trans_from{
    public:
        string cur_state{};
        char input,now_top;
        pda_trans_from(){}
        pda_trans_from(string state,char in_,char top_);
        bool operator < (const pda_trans_from &x) const{
            return (cur_state+input+now_top)<(x.cur_state+x.input+x.now_top);
        }
};
class pda_trans_to{
    public:
        string new_state{},new_top{};
        pda_trans_to(){}
        pda_trans_to(string state,string top_);
};
class PDA{
    public:
        set<string> states{},final_states{};
        string cur_state{},start_state{};
        set<char> input_set{},stack_set{};
        stack<char> my_stack{};
        char start_stack;
        map<pda_trans_from,pda_trans_to> trans{};
        PDA(char *file_name);
        bool transfer(string input_string);
};