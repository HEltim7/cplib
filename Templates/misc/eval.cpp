namespace eval {
    // 调度场算法 O(n) 输入中缀表达式，输出后缀表达式
    // 默认每个词(数、运算符、函数)仅为1char长 
    // 注意，-可以同时理解为减法和取反时会导致歧义
    string shunting_yard(string &s) {
        // 是否为基础值
        auto is_num=[](char x) {
            return isdigit(x);
        };

        // 是否为运算符
        auto is_oper=[](char x) {
            const static string s="+-*/";
            return find(s.begin(),s.end(),x)!=s.end();
        };

        // 是否为函数
        auto is_func=[](char x) {
            const static string s="";
            return find(s.begin(),s.end(),x)!=s.end();
        };

        // 是否左结合
        auto left_assoc=[](char x) {
            const static string s="+-*/";
            return find(s.begin(),s.end(),x)!=s.end();
        };

        // 运算优先级 优先级越高，值越小
        auto prio=[](char x) {
            if(x=='*'||x=='/') return 1;
            if(x=='+'||x=='-') return 2;
            return 3;
        };

        string res,stk;
        for(auto x:s) {
            if(is_num(x)) res.push_back(x);
            else if(is_func(x)) stk.push_back(x);
            else if(is_oper(x)) {
                while(stk.size()&&is_oper(stk.back())) {
                    char y=stk.back();
                    if (
                        prio(y)<prio(x)||
                        left_assoc(x)&&prio(x)==prio(y)
                    ) stk.pop_back(),res.push_back(y);
                    else break;
                }
                stk.push_back(x);
            }
            else if(x==',') {
                while(stk.back()!='(')
                    res.push_back(stk.back()),stk.pop_back();
            }
            else if(x=='(') stk.push_back(x);
            else if(x==')') {
                while(stk.size()&&stk.back()!='(')
                    res.push_back(stk.back()),stk.pop_back();
                assert(stk.size());
                stk.pop_back();
                if(stk.size()&&is_func(stk.back()))
                    res.push_back(stk.back()),stk.pop_back();
            }
            else assert(0);
        }

        while(stk.size()) {
            assert(stk.back()!='(');
            res.push_back(stk.back());
            stk.pop_back();
        }
        return res;
    }

    // 计算中缀表达式
    LL cal(string s) {
        s=shunting_yard(s);
        vector<LL> stk;
        for(char x:s) {
            if(isdigit(x)) stk.emplace_back(x-'0');
            else {
                LL b=stk.end()[-1];
                LL a=stk.end()[-2];
                stk.pop_back();
                if(x=='*') stk.back()=a*b;
                else if(x=='/') stk.back()=a/b;
                else if(x=='+') stk.back()=a+b;
                else if(x=='-') stk.back()=a-b;
                else assert(0);
            }
        }
        return stk.back();
    }
}