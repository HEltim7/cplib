namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=2155;
    using Query=tuple<int,int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,t,_]=x;
            const auto &[L,R,T,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            if(getid(r)!=getid(R)) return getid(r)<getid(R);
            return t<T;
        });

        int l=1,r=0,tm=0;
        
        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };

        auto modify=[&](int t) {
            // int idx=pos[t];
            // if(idx>=l&&idx<=r) del(idx);
            // swap(w[idx],nw[t]);
            // if(idx>=l&&idx<=r) add(idx);
        };

        auto rollback=[&](int t) {
            modify(t);
        };
        
        for(const auto &[L,R,T,id]:query) {
            while(l>L) add(--l);
            while(r<R) add(++r);
            while(l<L) del(l++);
            while(r>R) del(r--);
            while(tm<T) modify(++tm);
            while(tm>T) rollback(tm--);
            // TODO ans[id]=res;
        }
    }
}