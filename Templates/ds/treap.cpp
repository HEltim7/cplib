template<class Node> struct Treap {
    #define lch (tr[u].ch[0])
    #define rch (tr[u].ch[1])
    using I=const Node&;
    vector<Node> tr;
    int root;

    int new_node(I x) {
        tr.emplace_back(x);
        return tr.size()-1;
    }

    void pushup(int u) {
        tr[u].pushup(tr[lch], tr[rch]);
    }

    void pushdn(int u) {
        tr[lch].update(tr[u]);
        tr[rch].update(tr[u]);
        tr[u].clear_tag();
    }

    pair<int,int> split_by_key(int u,I key,bool le=true) {
        if(!u) return {};
        pushdn(u);
        if(tr[u]<key||le&&!(key<tr[u])) {
            auto [l,r]=split_by_key(rch, key, le);
            rch=l;
            pushup(u);
            return {u,r};
        }
        else {
            auto [l,r]=split_by_key(lch, key, le);
            lch=r;
            pushup(u);
            return {l,u};
        }
    }
    tuple<int,int,int> extract_by_key(int u,I lkey,I rkey) {
        auto [t,r]=split_by_key(u, rkey);
        auto [l,m]=split_by_key(t, lkey, false);
        return {l,m,r};
    }

    pair<int,int> split_by_rank(int u,int rk) {
        if(!u) return {};
        pushdn(u);
        if(tr[lch].sz+1<=rk) {
            auto [l,r]=split_by_rank(rch, rk-tr[lch].sz-1);
            rch=l;
            pushup(u);
            return {u,r};
        }
        else {
            auto [l,r]=split_by_rank(lch, rk);
            lch=r;
            pushup(u);
            return {l,u};
        }
    }
    tuple<int,int,int> extract_by_rank(int u,int lrk,int rrk) {
        auto [t,r]=split_by_rank(u, rrk);
        auto [l,m]=split_by_rank(t, lrk-1);
        return {l,m,r};
    }

    int merge(int u,int v) {
        if(!u||!v) return u|v;
        pushdn(u);pushdn(v);
        if(tr[u].prio<tr[v].prio) {
            tr[u].ch[1]=merge(tr[u].ch[1], v);
            pushup(u);
            return u;
        }
        else {
            tr[v].ch[0]=merge(u, tr[v].ch[0]);
            pushup(v);
            return v;
        }
    }
    int merge(int x,int y,int z) {
        return merge(merge(x,y),z);
    }

    int find(int u,I key) {
        if(!u||!(tr[u]<key)&&!(key<tr[u])) return u;
        pushdn(u);
        if(key<tr[u]) return find(lch,key);
        return find(rch,key);
    }

    void insert(I key) {
        int u=new_node(key);
        auto [l,r]=split_by_key(root, key);
        root=merge(l,u,r);
    }

    void erase(I key) {
        if(find(root,key)) {
            auto [l,t]=split_by_key(root, key, false);
            auto [m,r]=split_by_rank(t, 1);
            root=merge(l,r);
        }
    }

    template<class F> int build(int l,int r,F f) {
        if(l>r) return 0;
        int m=(l+r)/2;
        int u=new_node(f(m));
        lch=build(l,m-1,f);
        rch=build(m+1,r,f);
        pushup(u);
        return u;
    }

    int count_less(I key,bool le=false) {
        auto [l,r]=split_by_key(root, key, le);
        int cnt=tr[l].sz;
        root=merge(l,r);
        return cnt;
    }

    int count_range(I lkey,I rkey) {
        return count_less(rkey,true)-count_less(lkey);
    }

    int rank(I key) {
        return count_less(key)+1;
    }

    int kth(int rk) {
        auto [l,m,r]=extract_by_rank(root, rk, rk);
        root=merge(l,m,r);
        return m;
    }

    int prev(I key) { return kth(count_less(key)); }
    int next(I key) { return kth(count_less(key,true)+1); }
    int prev_equal(I key) { return kth(count_less(key,true)); }
    int next_equal(I key) { return kth(count_less(key)+1); }

    bool empty() { return root==0; }

    void clear() {
        root=0;
        tr.clear();
        new_node({});
        tr[0].set_null();
    }

    Node &operator[](int id) { return tr[id]; }
    I operator[](int id) const { return tr[id]; }

    Treap(int sz=0) { tr.reserve(sz),clear(); }

    #undef lch
    #undef rch
};

auto rnd=mt19937(random_device()());
struct Node {
    int ch[2],prio,sz;

    Node() {
        ch[0]=ch[1]=0;
        prio=rnd();
        sz=1;
    }

    void set_null() {
        sz=0;
    }

    // lch + par + rch
    void pushup(const Node &l,const Node &r) {
        sz=l.sz+1+r.sz;
    }

    // update info and lazy tag
    void update(const Node &p) {

    }

    // clear lazy tag
    void clear_tag() {

    }

    bool operator<(const Node &r) const {
        
    }
};