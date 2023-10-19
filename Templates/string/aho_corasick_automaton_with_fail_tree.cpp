struct AhoCorasickAutomaton {
    constexpr static int A=26;
    constexpr static char B='a';
    struct Node {
        int link,cnt;
        int ch[A];
        vector<int> adj;
    };

    vector<Node> tr;
    vector<int> id,ed;
    int sz=0,idx=0;

    int insert(string &s) {
        int root=0;
        for(auto x:s) {
            int c=x-B;
            if(!tr[root].ch[c]) 
                tr[root].ch[c]=new_node();
            root=tr[root].ch[c];
        }
        tr[root].cnt++;
        return root;
    }

    void build() {
        queue<int> q;
        for(int i=0;i<A;i++) 
            if(tr[0].ch[i]) {
                q.push(tr[0].ch[i]);
                tr[0].adj.push_back(tr[0].ch[i]);

            }
        while(q.size()) {
            auto root=q.front();
            q.pop();
            for(int i=0;i<A;i++) {
                int &cur=tr[root].ch[i];
                int pre=tr[tr[root].link].ch[i];
                if(!cur) cur=pre;
                else {
                    // tr[cur].cnt+=tr[pre].cnt;
                    tr[cur].link=pre;
                    tr[pre].adj.push_back(cur);
                    q.push(cur);
                }
            }
        }

        id.resize(size());
        ed.resize(size());
        relabel(0);
    }

    void relabel(int u) {
        id[u]=++idx;
        for(int v:tr[u].adj) relabel(v);
        ed[u]=idx;
    }

    int size() { return tr.size(); }
    int new_node() { tr.emplace_back();return ++sz; }
    void clear() { tr.clear();tr.resize(1);sz=idx=0; }

    AhoCorasickAutomaton(int sz=0) { tr.reserve(sz+1);tr.emplace_back(); }
};