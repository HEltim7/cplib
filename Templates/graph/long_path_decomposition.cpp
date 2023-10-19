namespace lpd {
    constexpr int N=1e5+10;
    int id[N],ed[N],ori[N],len[N],hch[N],top[N],dfn;
    vector<int> adj[N];

    void dfs1(int u,int fa) {
        for(int v:adj[u]) {
            if(v!=fa) {
                dfs1(v,u);
                if(len[hch[u]]<len[v]) hch[u]=v;
            }
        }
        len[u]=len[hch[u]]+1;
    }

    void dfs2(int u,int fa,int t) {
        top[u]=t;
        ori[id[u]=++dfn]=u;
        ed[u]=id[u]+len[u]-1;
        if(hch[u]) dfs2(hch[u],u,t);
        for(int v:adj[u]) if(v!=fa&&v!=hch[u]) dfs2(v,u,v);
    }

    void init() { dfs1(1,0),dfs2(1,0,1); }
    
    void solve(int u,int fa) {
        if(hch[u]) solve(hch[u],u);
        // todo 合并u点信息
        for(int v:adj[u]) {
            if(v!=fa&&v!=hch[u]) {
                solve(v,u);
                // todo 合并轻儿子链信息
                for(int i=1,j=0;j<len[v];i++,j++) {
                    // info(u,i) <- info(v,j)
                }
            }
        }
    }
}