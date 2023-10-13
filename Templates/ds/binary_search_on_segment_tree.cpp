// 区间二分
template<class F> int find_first(int u,int l,int r,int x,int y,F f) {
    if(l>y||r<x||l>=x&&r<=y&&!f(info[u])) return -1;
    if(l==r) return l;
    int mid=(l+r)/2;
    int res=find_first(lch[u],l,mid,x,y,f);
    if(res==-1) res=find_first(rch[u],mid+1,r,x,y,f);
    return res;
}
template<class F> int find_first(int u,int l,int r,F f) {
    return find_first(root[u],rng_l,rng_r,l,r,f);
}

template<class F> int find_last(int u,int l,int r,int x,int y,F f) {
    if(l>y||r<x||l>=x&&r<=y&&!f(info[u])) return -1;
    if(l==r) return l;
    int mid=(l+r)/2;
    int res=find_last(rch[u],mid+1,r,x,y,f);
    if(res==-1) res=find_last(lch[u],l,mid,x,y,f);
    return res;
}
template<class F> int find_last(int u,int l,int r,F f) {
    return find_last(root[u],rng_l,rng_r,l,r,f);
}

// 区间累加二分
template<class F>
int find_first(int u,int l,int r,int x,int y,F f,Info &suf) {
    if(l==r&&!f(info[u]+suf)) return -1;
    if(l>=x&&r<=y&&f(info[u]+suf)) return suf=info[u]+suf,l;
    pushdn(u);
    int mid=(l+r)/2;
    if(mid>=x&&mid<y) {
        int res=find_first(rch,mid+1,r,x,y,f,suf);
        if(res==mid+1) {
            int t=find_first(lch,l,mid,x,y,f,suf);
            if(t!=-1) res=t;
        }
        return res;
    }
    else if(mid>=x) return find_first(lch,l,mid,x,y,f,suf);
    return find_first(rch,mid+1,r,x,y,f,suf);
}
template<class F> int find_first(int l,int r,F f,Info suf={}) {
    l=max(l,rng_l),r=min(r,rng_r);
    return l>r?-1:find_first(1,rng_l,rng_r,l,r,f,suf);
}

template<class F>
int find_last(int u,int l,int r,int x,int y,F f,Info &pre) {
    if(l==r&&!f(pre+info[u])) return -1;
    if(l>=x&&r<=y&&f(pre+info[u])) return pre=pre+info[u],r;
    pushdn(u);
    int mid=(l+r)/2;
    if(mid>=x&&mid<y) {
        int res=find_last(lch,l,mid,x,y,f,pre);
        if(res==mid) {
            int t=find_last(rch,mid+1,r,x,y,f,pre);
            if(t!=-1) res=t;
        }
        return res;
    }
    else if(mid>=x) return find_last(lch,l,mid,x,y,f,pre);
    return find_last(rch,mid+1,r,x,y,f,pre);
}
template<class F> int find_last(int l,int r,F f,Info pre={}) {
    l=max(l,rng_l),r=min(r,rng_r);
    return l>r?-1:find_last(1,rng_l,rng_r,l,r,f,pre);
}

// 可持久化线段树上二分
constexpr static int nil=numeric_limits<int>::min();
template<class F> int find_first(int v,int u,int l,int r,Info p,F f) {
    if(!u) return nil;
    if(l==r) return l;
    int m=l+(r-l)/2;
    Info t=p+(info[lch[u]]-info[lch[v]]);
    if(f(t)) return find_first(lch[v],lch[u],l,m,p,f);
    return find_first(rch[v],rch[u],m+1,r,t,f);
}
template<class F> int find_first(int lrt,int rrt,F f) {
    return f(info[rrt]-info[lrt])?find_first(lrt,rrt,L,R,{},f):nil;
}