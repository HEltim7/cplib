namespace chrt {
    void init(int u) {

    }

    void link(int p,int u) {

    }

    void cut(int p,int u) {

    }

    void dfs1(int u,int fa) {
        init(u);
        for(int v:adj[u]) {
            if(v!=fa) {
                dfs1(v,u);
                link(u,v);
            }
        }
    }

    void dfs2(int u,int fa) {
        // todo update ans
        for(int v:adj[u]) {
            if(v!=fa) {
                cut(u,v),link(v,u);
                dfs2(v,u);
                cut(v,u),link(u,v);
            }
        }
    }

    void solve(int rt) {
        dfs1(rt, -1);
        dfs2(rt, -1);
    }
}