template<class F>
int find_first(int u,int l,int r,int x,int y,F check,Info &suf) {
    if(l==r&&!check(info[u]+suf)) return -1;
    if(l>=x&&r<=y&&check(info[u]+suf)) return suf=info[u]+suf,l;
    pushdn(u);
    int mid=(l+r)/2;
    if(mid>=x&&mid<y) {
        int res=find_first(rch,mid+1,r,x,y,check,suf);
        if(res==mid+1) {
            int t=find_first(lch,l,mid,x,y,check,suf);
            if(t!=-1) res=t;
        }
        return res;
    }
    else if(mid>=x) return find_first(lch,l,mid,x,y,check,suf);
    return find_first(rch,mid+1,r,x,y,check,suf);
}
template<class F> int find_first(int l,int r,F check,Info suf={}) {
    l=max(l,rng_l),r=min(r,rng_r);
    return l>r?-1:find_first(1,rng_l,rng_r,l,r,check,suf);
}

template<class F>
int find_last(int u,int l,int r,int x,int y,F check,Info &pre) {
    if(l==r&&!check(pre+info[u])) return -1;
    if(l>=x&&r<=y&&check(pre+info[u])) return pre=pre+info[u],r;
    pushdn(u);
    int mid=(l+r)/2;
    if(mid>=x&&mid<y) {
        int res=find_last(lch,l,mid,x,y,check,pre);
        if(res==mid) {
            int t=find_last(rch,mid+1,r,x,y,check,pre);
            if(t!=-1) res=t;
        }
        return res;
    }
    else if(mid>=x) return find_last(lch,l,mid,x,y,check,pre);
    return find_last(rch,mid+1,r,x,y,check,pre);
}
template<class F> int find_last(int l,int r,F check,Info pre={}) {
    l=max(l,rng_l),r=min(r,rng_r);
    return l>r?-1:find_last(1,rng_l,rng_r,l,r,check,pre);
}