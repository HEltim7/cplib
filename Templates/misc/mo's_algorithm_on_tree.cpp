namespace mo {
    constexpr int N=5e4+10,Q=1e5+10,block=320; // ***
    using Query=tuple<int,int,int,int>;
    vector<Query> query;
    vector<int> adj[N];
    int uid[N<<1],first[N],last[N],idx;
    bool odd[N];
    int ans[Q];
    
    namespace hpd {
        int dep[N],sz[N],top[N],p[N],hch[N];
    
        void dfs1(int u,int fa,int d) {
            dep[u]=d,p[u]=fa,sz[u]=1;
            for(int v:adj[u]) {
                if(v==fa) continue;
                dfs1(v,u,d+1);
                sz[u]+=sz[v];
                if(sz[hch[u]]<sz[v]) hch[u]=v;
            }
        }
    
        void dfs2(int u,int t) {
            top[u]=t;
            if(!hch[u]) return;
            dfs2(hch[u],t);
            for(int v:adj[u])
                if(v!=p[u]&&v!=hch[u]) dfs2(v,v);
        }
    
        int lca(int x,int y) {
            while(top[x]!=top[y]) {
                if(dep[top[x]]<dep[top[y]]) swap(x,y);
                x=p[top[x]];
            }
            if(dep[x]<dep[y]) swap(x,y);
            return y;
        }
    
        void init() {
            dfs1(1,-1,1); dfs2(1,1);
        }

        void clear(int n) {
            fill(hch, hch+n+1, 0);
        }
    }

    void dfs(int u,int fa) {
        uid[++idx]=u;
        first[u]=idx;
        for(int v:adj[u]) if(v!=fa) dfs(v,u);
        uid[++idx]=u;
        last[u]=idx;
    }

    void add_query(int u,int v,int id) {
        if(first[u]>first[v]) swap(u,v);
        int p=hpd::lca(u,v);
        int l,r,pidx;
        if(u==p) l=first[u],r=first[v],pidx=0;
        else l=last[u],r=first[v],pidx=first[p];
        query.emplace_back(l,r,pidx,id);
    }

    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_,__]=x;
            const auto &[L,R,___,____]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return getid(l)&1?r<R:r>R;
        });

        auto _add=[&](int u) {
            
        };

        auto _del=[&](int u) {
            
        };

        auto extend=[&](int idx) {
            int u=uid[idx];
            if(odd[u]^=1) _add(u);
            else _del(u);
        };

        int l=1,r=0;
        for(const auto &[L,R,pidx,id]:query) {
            while(l>L) extend(--l);
            while(r<R) extend(++r);
            while(l<L) extend(l++);
            while(r>R) extend(r--);
            if(pidx) extend(pidx);
            // TODO ans[id]=res;
            if(pidx) extend(pidx);
        }
    }

    void init() {
        hpd::init();
        dfs(1,-1);
    }

    void clear(int n) {
        idx=0;
        query.clear();
        fill(odd, odd+n+1, 0);
        for(int i=0;i<=n;i++) adj[i].clear();
        hpd::clear(n);
    }
}