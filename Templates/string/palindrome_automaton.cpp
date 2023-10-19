struct PalindromeAutomaton {
    constexpr static int A=26;
    constexpr static char B='a';
    struct Node {
        int len,link;
        int cnt;
        int ch[A];
    };
    vector<Node> node;
    string str;
    int last;

    int new_node(int len) {
        node.emplace_back(len);
        return node.size()-1;
    }
    
    void clear() {
        node.clear();
        last=0;
        str="!";
        new_node(0);
        new_node(-1);
        node[0].link=1;
    }

    int getfail(int x) {
        while(str.end()[-node[x].len-2]!=str.back()) x=node[x].link;
        return x;
    }

    void extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
        }
        last=node[pre].ch[c];
        node[last].cnt++;
    }

    void build(const string &s) { for(auto x:s) extend(x); }
    int size() { return node.size(); }

    PalindromeAutomaton(int sz=0) { str.reserve(sz),node.reserve(sz),clear(); }
};