template<typename I> struct PersistentBinaryTrie {
    constexpr static int H=sizeof(I)*8-1;
    struct Node {
        int ch[2];
        int cnt;
    };
    vector<Node> tr;
    vector<int> root;
    
    int ver() { return root.size()-1; }

    int new_root() {
        root.push_back({});
        return ver();
    }

    int new_node() {
        tr.push_back({});
        return tr.size()-1;
    }

    void insert(int &rt,int v,int val) {
        int u=rt=new_node();
        tr[u]=tr[v];
        for(int i=H;i>=0;i--) {
            bool x=val>>i&1;
            u=tr[u].ch[x]=new_node();
            v=tr[v].ch[x];
            tr[u]=tr[v];
            tr[u].cnt++;
        }
    }
    int insert(int val) {
        new_root();
        insert(root[ver()],root[ver()-1],val);
        return ver();
    }

    I xor_max(int u,int val) {
        u=root[u];
        I res{};
        for(int i=H;i>=0;i--) {
            bool x=val>>i&1^1;
            if(tr[tr[u].ch[x]].cnt) {
                res|=1<<i;
                u=tr[u].ch[x];
            }
            else u=tr[u].ch[x^1];
        }
        return res;
    }

    I range_xor_max(int u,int v,int val) {
        u=root[u],v=root[v];
        I res{};
        for(int i=H;i>=0;i--) {
            bool x=val>>i&1^1;
            if(tr[tr[u].ch[x]].cnt-tr[tr[v].ch[x]].cnt) {
                res|=1<<i;
                u=tr[u].ch[x];
                v=tr[v].ch[x];
            }
            else u=tr[u].ch[x^1],v=tr[v].ch[x^1];
        }
        return res;
    }

    void clear() {
        tr.clear();
        new_root();
        new_node();
    }

    explicit PersistentBinaryTrie(int size=0) {
        tr.reserve(size*(H+1));
        clear();
    }
};