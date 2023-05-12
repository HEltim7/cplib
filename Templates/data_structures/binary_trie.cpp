template<typename I> struct BinaryTrie {
    constexpr static int H=sizeof(I)*8-1;
    struct Node {
        int ch[2];
        int cnt;
    };
    vector<Node> tr;

    int new_node() {
        tr.push_back({});
        return tr.size()-1;
    }

    void insert(int v) {
        for(int i=H,u=0;i>=0;i--) {
            bool x=v>>i&1;
            if(!tr[u].ch[x]) tr[u].ch[x]=new_node();
            u=tr[u].ch[x];
            tr[u].cnt++;
        }
    }

    void erase(int v) {
        for(int i=H,u=0;i>=0;i--) {
            bool x=v>>i&1;
            u=tr[u].ch[x];
            tr[u].cnt--;
        }
    }

    I xor_max(int v) {
        I res{};
        for(int i=H,u=0;i>=0;i--) {
            bool x=v>>i&1^1;
            if(tr[tr[u].ch[x]].cnt) {
                res|=1<<i;
                u=tr[u].ch[x];
            }
            else u=tr[u].ch[x^1];
        }
        return res;
    }

    void clear() {
        tr.clear();
        new_node();
    }

    explicit BinaryTrie(int size=0) {
        tr.reserve(size*(H+1));
        clear();
    }
};