template<class Info> struct DynamicSegmentTree {
    int L,R,rt;
    vector<int> lch,rch;
    vector<Info> info;

    int new_node() {
        lch.emplace_back();
        rch.emplace_back();
        info.emplace_back();
        return info.size()-1;
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
    Info query(int l,int r) { return query(1,L,R,l,r); }

    int modify(int u,int l,int r,int p,const Info &v) {
        if(!u) info[u=new_node()].init(l,r);
        if(l==r) info[u].update(v);
        else {
            int m=l+(r-l)/2;
            if(p<=m) lch[u]=modify(lch[u],l,m,p,v);
            else rch[u]=modify(rch[u],m+1,r,p,v);
            pushup(u);
        }
        return u;
    }
    void modify(int p,const Info &v) { rt=modify(rt,L,R,p,v); }

    DynamicSegmentTree(int l,int r,int sz=0):L(l),R(r),rt(0) {
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

    void update(const Info &v) {

    }
};