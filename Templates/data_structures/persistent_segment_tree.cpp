struct Info {

    void init(int l,int r) {
        if(l!=r) return;
        
    }
    void init(int l) { init(l,l); }

    friend Info operator+(const Info &l,const Info &r) {
        Info res;
        
        return res;
    }
};

template<class Info,int node_size,int root_size,int rng_l,int rng_r>
struct PersistentSegmentTree {
    #define lch(u) (tr[u].ch[0])
    #define rch(u) (tr[u].ch[1])

    struct Node {
        Info info;
        int ch[2];
        void init(int l,int r) {
            info.init(l,r);
        }
    };

    array<int, root_size> root;
    array<Node, node_size> tr;
    int idx,ver;

    int new_node() {
        assert(idx<node_size);
        return ++idx;
    }

    void pushup(int u) {
        tr[u].info=tr[lch(u)].info+tr[rch(u)].info;
    }

    Info query(int u,const int ql,const int qr,int l=rng_l,int r=rng_r) {
        if(l>=ql&&r<=qr) { return tr[u].info; }
        else {
            int mid=(l+r)/2;
            if(mid>=ql&&mid<qr) 
                return query(lch(u),ql,qr,l,mid)+query(rch(u),ql,qr,mid+1,r);
            else if(mid>=ql) return query(lch(u),ql,qr,l,mid);
            return query(rch(u),ql,qr,mid+1,r);
        }
    }

    void modify(int &u,int v,const int p,const Info &val,int l=rng_l,int r=rng_r) {
        u=new_node();
        tr[u]=tr[v];
        if(l==r) tr[u].info=val;
        else {
            int mid=(l+r)/2;
            if(p<=mid) modify(lch(u),lch(v),p,val,l,mid);
            else modify(rch(u),rch(v),p,val,mid+1,r);
            pushup(u);
        }
    }

    void append(int p,const Info &val) {
        assert(ver<root_size);
        ver++;
        modify(root[ver],root[ver-1],p,val);
    }

    void build(int &u,int l,int r) {
        u=new_node();
        tr[u].init(l,r);
        if(l!=r) {
            int mid=l+r>>1;
            build(lch(u),l,mid);
            build(rch(u),mid+1,r);
            pushup(u);
        }
    }
    void build() { build(root[0],rng_l,rng_r); }

    #undef lch
    #undef rch
};
PersistentSegmentTree<Info, N*__lg(N)*4, N*2, 1, N> sgt;