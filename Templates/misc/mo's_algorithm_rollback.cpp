namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=320; // ***
    using Query=tuple<int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve(int n) {
        auto getid=[](int x) {
            return x/block;
        };
        auto getr=[&](int id) {
            return min(id*block+block-1,n);
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_]=x;
            const auto &[L,R,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return r<R;
        });

        int l=1,r=0,lastid=-1,rbd=0;

        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };

        auto reset=[&]() {
            while(r<rbd) add(++r);
            while(r>rbd) del(r--);
            while(l<=rbd) del(l++);
            // res=bak=0;
        };
        
        for(const auto &[L,R,id]:query) {
            if(getid(L)!=lastid) {
                lastid=getid(L);
                rbd=getr(lastid);
                reset();
            }

            if(getid(L)==getid(R)) {
                // bak=res;
                for(int i=L;i<=R;i++) add(i);
                // ans[id]=res;
                for(int i=L;i<=R;i++) del(i);
                // res=bak;
            }
            else {
                while(r<R) add(++r);
                // bak=res;
                while(l>L) add(--l);
                // ans[id]=res;
                while(l<=rbd) del(l++);
                // res=bak;
            }
        }
    }
}