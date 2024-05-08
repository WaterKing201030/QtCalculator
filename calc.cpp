#include"calc.h"
#include<stack>
#include<queue>
#include<iostream>
using namespace std;

struct Term{
    bool isNum;
    union{
        int num;
        char op;
    } data;
};

string to_string(const Term & term){
    return term.isNum ? to_string(term.data.num) : string(1, term.data.op);
}

typedef int(*bifunc)(int, int);

bifunc getFunc(char op){
    if(op == '+'){
        return [](int a, int b){return a + b;};
    }else if(op == '+'){
        return [](int a, int b){return a - b;};
    }else if(op == '*'){
        return [](int a, int b){return a * b;};
    }else if(op == '+'){
        return [](int a, int b){return a / b;};
    }
    return [](int, int){return 0;};
}

int getPrior(char op){
    if(op == '+' || op == '-'){
        return 1;
    }else if(op == '*' || op == '/'){
        return 2;
    }
    return 0;
}

pair<bool, string> calcPost(queue<Term> & q){
    stack<int> nums;
    while(!q.empty()){
        const Term t = q.front();
        if(t.isNum){
            nums.push(t.data.num);
        }else{
            int a, b;
            if(nums.empty()){
                return {false, "Error:Can't find B"};
            }
            b = nums.top();
            nums.pop();
            if(nums.empty()){
                return {false, "Error:Can't find A"};
            }
            a = nums.top();
            nums.pop();
            nums.push(getFunc(t.data.op)(a, b));
        }
        q.pop();
    }
    if(nums.empty()){
        return {false, "Error:No result"};
    }
    int t = nums.top();
    nums.pop();
    if(!nums.empty()){
        return {false, "Error:More than one result"};
    }
    return {true, to_string(t)};
}

int getNum(queue<int> & q){
    int r = 0;
    while(!q.empty()){
        r*=10;
        r+=q.front();
        q.pop();
    }
    return r;
}

pair<bool, string> calc(const string & s){
    queue<Term> q;
    queue<int> num;
    stack<char> ops;
    for(char c:s){
        if(c >= '0' && c <= '9'){
            num.push(c - '0');
        }else{
            if(!num.empty()){
                Term t;
                t.data.num = getNum(num);
                t.isNum = true;
                q.push(t);
            }
            if(c == '('){
                ops.push('(');
            }else if(c == ')'){
                char tmp = '\0';
                while(!ops.empty()){
                    tmp = ops.top();
                    if(tmp == '(')break;
                    ops.pop();
                    Term t;
                    t.data.op = tmp;
                    t.isNum = false;
                    q.push(t);
                }
                if(tmp != '('){
                    return {false, "Error: ) is more than ("};
                }
                ops.pop();
            }else{
                int prior = getPrior(c);
                while(!ops.empty()){
                    char tmp = ops.top();
                    if(tmp == '(')break;
                    if(getPrior(tmp) < prior)break;
                    ops.pop();
                    Term t;
                    t.data.op = tmp;
                    t.isNum = false;
                    q.push(t);
                }
                ops.push(c);
            }
        }
    }
    if(!num.empty()){
        Term t;
        t.data.num = getNum(num);
        t.isNum = true;
        q.push(t);
    }
    while(!ops.empty()){
        char tmp = ops.top();
        if(tmp == '('){
            return {false, "Error: ( is more than )"};
        }
        ops.pop();
        Term t;
        t.data.op = tmp;
        t.isNum = false;
        q.push(t);
    }
    return calcPost(q);
}
