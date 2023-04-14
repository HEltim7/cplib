namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=320;
    using Query=tuple<int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_]=x;
            const auto &[L,R,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return getid(l)&1?r<R:r>R;
        });

        int l=1,r=0,res=0;
        
        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };
        
        for(const auto &[L,R,id]:query) {
            while(l>L) add(--l);
            while(r<R) add(++r);
            while(l<L) del(l++);
            while(r>R) del(r--);
            // TODO ans[id]=res;
        }
    }
}