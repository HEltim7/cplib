namespace dsu {
    constexpr int N=1e5+10; // ***
    int id[N],ed[N],ori[N],sz[N],hch[N],idx;
    vector<int> adj[N];

    void init(int u,int fa) {
        sz[u]=1;
        id[u]=++idx;
        ori[id[u]]=u;
        for(int v:adj[u]) {
            if(v!=fa) {
                init(v,u);
                if(sz[v]>sz[hch[u]]) hch[u]=v;
                sz[u]+=sz[v];
            }
        }
        ed[u]=idx;
    }

    void solve(int u,int fa,bool keep) {
        auto add=[](int id) {
            int x=ori[id];
            
        };

        auto del=[](int id) {
            int x=ori[id];
            
        };
        
        for(int v:adj[u]) if(v!=fa&&v!=hch[u]) solve(v,u,0);
        if(hch[u]) solve(hch[u],u,1);
        for(int v:adj[u]) 
            if(v!=fa&&v!=hch[u]) 
                for(int i=id[v];i<=ed[v];i++) add(i);
        add(id[u]);
        
        // TODO update ans

        if(!keep) for(int i=id[u];i<=ed[u];i++) del(i);
    }

    void clear(int n=N-1) {
        idx=0;
        fill(hch,hch+n+1,0);
        for(int i=0;i<=n;i++) adj[i].clear();
    }
}