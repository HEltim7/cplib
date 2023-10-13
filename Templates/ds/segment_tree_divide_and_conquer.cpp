namespace sd {
    #define lch (u<<1)
    #define rch (u<<1|1)
    using T=int;
    vector<vector<T>> seg;
    int L,R;

    void add(int u,int x,int y,int l,int r,T val) {
        if(x>r||y<l) return;
        if(x<=l&&y>=r) seg[u].emplace_back(val);
        else {
            int mid=(l+r)/2;
            add(lch,x,y,l,mid,val);
            add(rch,x,y,mid+1,r,val);
        }
    }
    void add(int x,int y,T val) {
        add(1,x,y,L,R,val);
    }

    void solve(int u,int l,int r) {
        // apply
        for(auto x:seg[u]) {
            
        }

        // update ans
        if(l==r) ;
        else {
            int mid=(l+r)/2;
            solve(lch,l,mid);
            solve(rch,mid+1,r);
        }

        // undo
    }
    void solve() {
        solve(1,L,R);
    }

    void init(int l,int r) {
        L=l,R=r;
        seg.clear();
        seg.resize(4<<__lg(r-l+1)|1);
    }

    #undef lch
    #undef rch
}