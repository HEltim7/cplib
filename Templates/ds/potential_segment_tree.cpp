struct Info {
    bool final;

    void init(int l,int r) {
        if(l!=r) return;

    }

    friend Info operator+(const Info &l,const Info &r) {
        Info res;

        return res;
    }

    void operator--(int) {
        
    }

    explicit operator bool() const { return final; }
};

template<class Info,int size> struct SegmentTree {
    #define lch (u<<1)
    #define rch (u<<1|1)

    struct Node {
        int l,r;
        Info info;
        void init(int l,int r) {
            this->l=l;
            this->r=r;
            info.init(l, r);
        }
    };

    array<Node, 1<<__lg(size)<<2|1> tr;

    void pushup(int u) {
        tr[u].info=tr[lch].info+tr[rch].info;
    }

    Info query(int u,int l,int r) {
        if(tr[u].l>=l&&tr[u].r<=r) { return tr[u].info; }
        else {
            int mid=(tr[u].l+tr[u].r)/2;
            if(mid>=l&&mid<r) return query(lch,l,r)+query(rch,l,r);
            else if(mid>=l) return query(lch,l,r);
            return query(rch,l,r);
        }
    }
    Info query(int l,int r) { return query(1,l,r); }

    void release(int u,int l,int r) {
        if(tr[u].info) return;
        else if(tr[u].l==tr[u].r) tr[u].info--;
        else {
            int mid=(tr[u].l+tr[u].r)/2;
            if(l<=mid) release(lch,l,r);
            if(r>mid) release(rch,l,r);
            pushup(u);
        }
    }
    void release(int l,int r) { release(1,l,r); }

    void build(int u,int l,int r) {
        tr[u].init(l,r);
        if(l!=r) {
            int mid=(l+r)/2;
            build(lch,l,mid);
            build(rch,mid+1,r);
            pushup(u);
        }
    }
    void build(int l=1,int r=size) { build(1,l,r); }

    #undef lch
    #undef rch
};
SegmentTree<Info, N> sgt;