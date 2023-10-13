template<class Info,int size> struct SegmentTree {
    #define lch ((u)<<1)
    #define rch ((u)<<1|1)

    int L,R;
    constexpr static int node_size=4<<__lg(size)|1;
    array<Info, node_size> info;
    array<int, size+1> leaf;

    void pushup(int u) {
        info[u]=info[lch]+info[rch];
    }

    Info query(int u,int l,int r,int x,int y) {
        if(l>y||r<x) return {};
        if(l>=x&&r<=y) return info[u];
        int m=(l+r)/2;
        return query(lch,l,m,x,y)+query(rch,m+1,r,x,y);
    }
    Info query(int l,int r) { return query(1,L,R,l,r); }

    void modify(int p,const Info &v) {
        int u=leaf[p];
        info[u].update(v);
        while(u>>=1) pushup(u);
    }

    void build(int u,int l,int r) {
        info[u].init(l,r);
        if(l!=r) {
            int m=(l+r)/2;
            build(lch,l,m);
            build(rch,m+1,r);
            pushup(u);
        }
        else leaf[l]=u;
    }
    void build(int l=1,int r=size) { build(1,L=l,R=r); }

    #undef lch
    #undef rch
};

struct Info {

    void init(int l,int r) {
        if(l!=r) return;

    }

    friend Info operator+(const Info &l,const Info &r) {
        Info res;

        return res;
    }

    void update(const Info &v) {

    }
};

SegmentTree<Info, N> sgt;