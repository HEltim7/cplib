struct PalindromeAutomaton {
    constexpr static int A=26;
    constexpr static char B='a';
    struct Node {
        int len,link;
        int cnt;
        int ch[A];
    };
    vector<Node> node;
    deque<char> str;
    int last_l,last_r;

    int new_node(int len) {
        node.emplace_back(len);
        return node.size()-1;
    }
    
    void clear() {
        node.clear();
        last_l=last_r=0;
        str.clear();
        new_node(0);
        new_node(-1);
        node[0].link=1;
    }

    template<class F> void extend(char x,int &last,F getfail) {
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
        }
        last=node[pre].ch[c];
        if(node[last].len==str.size()) last_l=last_r=last;
        node[last].cnt++;
    }

    void extend_l(char x) {
        str.push_front(x);
        extend(x, last_l, [&](int x) {
            int n=int(str.size())-1;
            while(node[x].len+1>n||str[node[x].len+1]!=str[0]) x=node[x].link;
            return x;
        });
    }

    void extend_r(char x) {
        str.push_back(x);
        extend(x, last_r, [&](int x) {
            int n=int(str.size())-1;
            while(n-node[x].len-1<0||str[n-node[x].len-1]!=str[n]) x=node[x].link;
            return x;
        });
    }

    int size() { return node.size(); }

    PalindromeAutomaton(int sz=0) { node.reserve(sz),clear(); }
};