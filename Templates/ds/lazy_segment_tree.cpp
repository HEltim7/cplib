template<class Info,class Tag,int size> struct LazySegmentTree {
    #define lch ((u)<<1)
    #define rch ((u)<<1|1)

    int L,R;
    constexpr static int node_size=4<<__lg(size)|1;
    array<Tag, node_size> tag;
    array<Info, node_size> info;
    array<bool, node_size> clean;

    void pushup(int u) {
        info[u]=info[lch]+info[rch];
    }

    void update(int u, const Tag &t) {
        info[u].update(t);
        tag[u].update(t);
        clean[u]=0;
    }

    void pushdn(int u) {
        if(clean[u]) return;
        update(lch, tag[u]);
        update(rch, tag[u]);
        clean[u]=1;
        tag[u].clear();
    }

    Info query(int u,int l,int r,int x,int y) {
        if(l>y||r<x) return {};
        if(l>=x&&r<=y) return info[u];
        pushdn(u);
        int m=(l+r)/2;
        return query(lch,l,m,x,y)+query(rch,m+1,r,x,y);
    }
    Info query(int l,int r) { return query(1,L,R,l,r); }

    void modify(int u,int l,int r,int x,int y,const Tag &t) {
        if(l>y||r<x) return;
        if(l>=x&&r<=y) update(u, t);
        else {
            pushdn(u);
            int m=(l+r)/2;
            if(m>=x) modify(lch,l,m,x,y,t);
            if(m<y) modify(rch,m+1,r,x,y,t);
            pushup(u);
        }
    }
    void modify(int l,int r,const Tag &t) { modify(1,L,R,l,r,t); }

    void build(int u,int l,int r) {
        clean[u]=1;
        info[u].init(l,r);
        tag[u].clear();
        if(l!=r) {
            int m=(l+r)/2;
            build(lch,l,m);
            build(rch,m+1,r);
            pushup(u);
        }
    }
    void build(int l=1,int r=size) { build(1,L=l,R=r); }

    #undef lch
    #undef rch
};

struct Tag {

    void clear() {

    }

    void update(const Tag &t) {

    }
};

struct Info {

    void init(int l,int r) {
        if(l!=r) return;

    }

    friend Info operator+(const Info &l,const Info &r) {
        Info res;

        return res;
    }

    void update(const Tag &t) {

    }
};

LazySegmentTree<Info, Tag, N> sgt;