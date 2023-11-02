namespace sgt {
    #define lch (tr[u].lc)
    #define rch (tr[u].rc)

    struct Node {
        int lc=0,rc=0;
		int cnt=0;
    };
    constexpr int M=N*(__lg(N)+1)*2;
    vector<Node> tr(M);
    int idx,L,R;

    int new_node() {
        assert(++idx<M);
        tr[idx]={};
        return idx;
    }

    void pushup(int u) {
        tr[u].cnt=tr[lch].cnt+tr[rch].cnt;
    }

    void update(int u) {
        if(!u) return;

    }

    void pushdn(int u) {

    }

    void merge(int &u,int v,int l,int r) {
        if(!u||!v) u=u|v;
        else if(l==r) {
            tr[u].cnt+=tr[v].cnt;
        }
        else {
            pushdn(u),pushdn(v);
            int mid=(l+r)/2;
            merge(tr[u].lc, tr[v].lc, l, mid);
            merge(tr[u].rc, tr[v].rc, mid+1, r);
            pushup(u),pushup(v);
        }
    }
	void merge(int &u,int v) { merge(u,v,L,R); }

    pair<int,int> split(int u,int l,int r,int p) {
        if(r<p) return {u,0};
        if(l>=p) return {0,u};
        pushdn(u);
        int v=new_node();
        int mid=(l+r)/2;
        auto [a,b]=split(lch, l, mid, p);
        auto [c,d]=split(rch, mid+1, r, p);
        tr[u].lc=a,tr[u].rc=c;
        tr[v].lc=b,tr[v].rc=d;
        pushup(u),pushup(v);
        return {u,v};
    }
	pair<int,int> split(int u,int p) { return split(u,L,R,p); }

    int extract(int &u,int l,int r,int x,int y) {
        auto [a,b]=split(u, l, r, x);
        auto [c,d]=split(b, l, r, y+1);
        merge(a, d, l, r);
        return u=a,c;
    }
	int extract(int &u,int l,int r) { return extract(u,L,R,l,r); }

    int query(int u,int l,int r,int x,int y) {
        if(!u||l>y||r<x) return {};
        if(l>=x&&r<=y) {
			return tr[u].cnt;
		}
        pushdn(u);
        int mid=(l+r)/2;
        return query(lch, l, mid, x, y)+query(rch, mid+1, r, x, y);
    }
	int query(int u,int l,int r) { return query(u,L,R,l,r); }

    void modify(int &u,int l,int r,int p,int v) {
        if(!u) u=new_node();
        if(l==r) {
            tr[u].cnt+=v;
        }
        else {
            pushdn(u);
            int mid=(l+r)/2;
            if(p<=mid) modify(lch, l, mid, p, v);
            else modify(rch, mid+1, r, p, v);
            pushup(u);
        }
    }
	void modify(int &u,int p,int v) { modify(u,L,R,p,v); }

	void init(int l,int r) { idx=0,L=l,R=r; }

    #undef lch
    #undef rch
}