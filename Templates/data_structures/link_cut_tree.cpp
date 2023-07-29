template<class Info,class Tag,int MAX_SIZE,
         bool CHECK_LINK = 0,bool CHECK_CUT = 0,bool ASSERT = 0>
struct LinkCutTree {
    #define lch tr[u].ch[0]
    #define rch tr[u].ch[1]
    #define wch(u) (tr[tr[u].p].ch[1]==u)

    struct Node {
        int ch[2],p;
        bool rev;
        Info info;
        Tag tag;
        void update(const Tag &x) {
            info.update(x);
            tag.update(x);
        }
    };
    array<Node, MAX_SIZE> tr;
    array<int, MAX_SIZE> stk;

    bool is_root(int u) {
        return tr[tr[u].p].ch[0]!=u&&tr[tr[u].p].ch[1]!=u;
    }

    void pushup(int u) {
        tr[u].info.pushup(tr[lch].info,tr[rch].info);
    }

    void pushrev(int u) {
        tr[u].info.reverse();
        swap(lch,rch);
        tr[u].rev^=1;
    }

    void pushdn(int u) {
        if(tr[u].rev) {
            if(lch) pushrev(lch);
            if(rch) pushrev(rch);
            tr[u].rev=0;
        }
        if(lch) tr[lch].update(tr[u].tag);
        if(rch) tr[rch].update(tr[u].tag);
        tr[u].tag.clear();
    }

    void rotate(int x) {
        int y=tr[x].p,z=tr[y].p,k=wch(x);
        if(!is_root(y)) tr[z].ch[wch(y)]=x;
        tr[y].ch[k]=tr[x].ch[!k],tr[tr[y].ch[k]].p=y;
        tr[x].ch[!k]=y,tr[y].p=x,tr[x].p=z;
        pushup(y),pushup(x);
    }

    void splay(int u) {
        int top=0,fa=u;
        stk[++top]=fa;
        while(!is_root(fa)) stk[++top]=fa=tr[fa].p;
        while(top) pushdn(stk[top--]);
        for(;!is_root(u);rotate(u))
            if(!is_root(fa=tr[u].p)) rotate(wch(u)==wch(fa)?fa:u);
    }

    int access(int u) {
        int v=0;
        for(;u;v=u,u=tr[u].p)
            splay(u),rch=v,pushup(u);
        return v;
    }

    void make_root(int u) {
        access(u);
        splay(u);
        pushrev(u);
    }

    int split(int u,int v) {
        make_root(u);
        access(v);
        splay(v);
        return v;
    }

    int find_root(int u) {
        access(u);
        splay(u);
        while(lch) pushdn(u),u=lch;
        splay(u);
        return u;
    }

    bool same(int u,int v) {
        make_root(u);
        return find_root(v)==u;
    }

    bool link(int u,int v) {
        make_root(u);
        if(CHECK_LINK&&find_root(v)==u)
            return assert(!ASSERT),0;
        tr[u].p=v;
        return 1;
    }

    bool cut(int u,int v) {
        make_root(u);
        if(CHECK_CUT&&!(find_root(v)==u&&rch==v&&!tr[v].ch[0]))
            return assert(!ASSERT),0;
        else access(v),splay(u);
        rch=tr[v].p=0;
        pushup(u);
        return 1;
    }

    int lca(int u,int v) {
        access(u);
        return access(v);
    }

    int lca(int rt,int u,int v) {
        make_root(rt);
        return lca(u,v);
    }

    void modify(int u,const Tag &x) {
        if(!is_root(u)) splay(u);
        tr[u].update(x);
    }

    Info &info(int u) {
        return tr[u].info;
    }

    #undef lch
    #undef rch
    #undef wch
};

struct Tag {

    void update(const Tag &x) {
        
    }

    void clear() {
        
    }
};

struct Info {

    //* lch+parent+rch
    void pushup(const Info &l,const Info &r) {
        
    }

    void update(const Tag &x) {

    }

    void reverse() {}
};

LinkCutTree<Info,Tag,N> lct;