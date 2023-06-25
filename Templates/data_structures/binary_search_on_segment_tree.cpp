template<class F>
int find_first(int u,int l,int r,int x,int y,F check) {
    if(l>y||r<x||l>=x&&r<=y&&!check(info[u])) return -1;
    if(l==r) return l;
    pushdn(u);
    int mid=(l+r)/2;
    int res=find_first(lch,l,mid,x,y,check);
    if(res==-1) res=find_first(rch,mid+1,r,x,y,check);
    return res;
}
template<class F> int find_first(int l,int r,F check) {
    return find_first(1,rng_l,rng_r,l,r,check);
}

template<class F>
int find_last(int u,int l,int r,int x,int y,F check) {
    if(l>y||r<x||l>=x&&r<=y&&!check(info[u])) return -1;
    if(l==r) return l;
    pushdn(u);
    int mid=(l+r)/2;
    int res=find_last(rch,mid+1,r,x,y,check);
    if(res==-1) res=find_last(lch,l,mid,x,y,check);
    return res;
}
template<class F> int find_last(int l,int r,F check) {
    return find_last(1,rng_l,rng_r,l,r,check);
}