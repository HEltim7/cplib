struct PersistentSegmentTree {

    #define lch tr[u].ch[0]
    #define rch tr[u].ch[1]
    constexpr static int MAX_SIZE=N*20*2;

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
        tr[u].cnt=tr[lch].cnt+tr[rch].cnt;
    }

    void modify(int &u,int v,int l,int r,int p) {
        u=new_node();
        tr[u]=tr[v];
        if(l==r) tr[u].cnt++;
        else {
            int mid=l+r>>1;
            if(p<=mid) modify(lch, tr[v].ch[0], l, mid, p);
            else modify(rch, tr[v].ch[1], mid+1, r, p);
            pushup(u);
        }
    }

    int kth(int u,int v,int l,int r,int k) {
        if(l==r) return l;
        int mid=l+r>>1;
        int lcnt=tr[lch].cnt-tr[tr[v].ch[0]].cnt;
        if(lcnt>=k) return kth(lch, tr[v].ch[0], l, mid, k);
        return kth(rch, tr[v].ch[1], mid+1, r, k-lcnt);
    }

    void build(int &u,int l,int r) {
        u=new_node();
        tr[u]={l,r};
        if(l!=r) {
            int mid=l+r>>1;
            build(lch,l,mid);
            build(rch,mid+1,r);
        }
    }

    #undef lch
    #undef rch

} sgt;