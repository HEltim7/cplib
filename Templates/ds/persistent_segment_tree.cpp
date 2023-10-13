template<class Info> struct PersistentSegmentTree {
    int L,R;
    vector<int> lch,rch;
    vector<Info> info;

    int new_node() {
        lch.emplace_back();
        rch.emplace_back();
        info.emplace_back();
        return info.size()-1;
    }

    void clone(int v,int u) {
        info[u]=info[v];
        lch[u]=lch[v];
        rch[u]=rch[v];
    }

    void pushup(int u) {
        info[u]=info[lch[u]]+info[rch[u]];
    }

    Info query(int u,int l,int r,int x,int y) {
        if(!u||l>y||r<x) return {};
        if(l>=x&&r<=y) return info[u];
        int m=l+(r-l)/2;
        return query(lch[u],l,m,x,y)+query(rch[u],m+1,r,x,y);
    }
    Info query(int rt,int l,int r) { return query(rt,L,R,l,r); }

    Info query(int v,int u,int l,int r,int x,int y) {
        if(!u||l>y||r<x) return {};
        if(l>=x&&r<=y) return info[u]-info[v];
        int m=l+(r-l)/2;
        return query(lch[v],lch[u],l,m,x,y)+
            query(rch[v],rch[u],m+1,r,x,y);
    }
    Info query(int lrt,int rrt,int l,int r) {
        return query(lrt,rrt,L,R,l,r);
    }

    int modify(int v,int l,int r,int p,const Info &val) {
        int u=new_node();
        if(v) clone(v,u);
        else info[u].init(l,r);
        if(l==r) info[u].update(val);
        else {
            int m=l+(r-l)/2;
            if(p<=m) lch[u]=modify(lch[v],l,m,p,val);
            else rch[u]=modify(rch[v],m+1,r,p,val);
            pushup(u);
        }
        return u;
    }
    int modify(int rt,int p,const Info &val) {
        return modify(rt,L,R,p,val);
    }

    PersistentSegmentTree(int l,int r,int sz=0):L(l),R(r) {
        lch.reserve(sz),rch.reserve(sz),info.reserve(sz);
        new_node();
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

    // l-r,not r-l
    friend Info operator-(const Info &l,const Info &r) {
        Info res;

        return res;
    }

    void update(const Info &v) {

    }
};