struct MergeSplitSegmentTree {

    #define lch tr[u].ch[0]
    #define rch tr[u].ch[1]
    constexpr static int MAX_SIZE=1e7+10;
    
    struct Node {
        int ch[2];
        int cnt;
    } tr[MAX_SIZE];
    int idx;

    int new_node() {
        // assert(idx<MAX_SIZE);
        return ++idx;
    }

    void pushup(int u) {
        if(lch&&rch) ;
        else if(lch) ;
        else if(rch) ;
    }

    // remember to pushdn laze tag
    void pushdn(int u) {
        if(lch) ;
        if(rch) ;

    }
    
    void merge(int &u,int v) {
        if(!u&&!v) return;
        if(!u||!v) u=u|v;
        else {
            pushdn(u);pushdn(v);
            merge(lch,tr[v].ch[0]);
            merge(rch,tr[v].ch[1]);
            pushup(u);
        }
    }

    // k][k+1
    void split(int &u,int &v,int l,int r,int k) {
        if(!u||k>=r) return;
        if(k<l) swap(u,v);
        else {
            v=new_node();
            int mid=l+r>>1;
            if(k<=mid) swap(rch,tr[v].ch[1]);
            pushdn(u);
            if(k<mid) split(lch, tr[v].ch[0], l, mid, k);
            else split(rch, tr[v].ch[1], mid+1, r, k);
            pushup(u),pushup(v);
        }
    }

    int kth(int u,int l,int r,int k) {
        if(tr[u].cnt<k) return -1;
        if(l==r) return l;
        int mid=l+r>>1;
        pushdn(u);
        if(tr[lch].cnt>=k) return kth(lch, l, mid, k);
        return kth(rch, mid+1, r, k-tr[lch].cnt);
    }

    void build(int &u,int l,int r,int p) {
        u=new_node();
        if(l==r) ;
        else {
            int mid=l+r>>1;
            if(p<=mid) build(lch,l,mid,p);
            else build(rch,mid+1,r,p);
            pushup(u);
        }
    }

    #undef lch
    #undef rch

} sgt;