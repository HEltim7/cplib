namespace cd {
    constexpr int N=1e5+10,L=__lg(N)+2;
    int sz[N],centroid[2],belong[L][N],lyr[N],p[N];
    vector<int> adj[N];
    bool del[N];

    void get_centroid(int u,int fa,int tot) {
        int maxx=0;
        sz[u]=1;
        for(int v:adj[u]) {
            if(v!=fa&&!del[v]) {
                get_centroid(v,u,tot);
                sz[u]+=sz[v];
                maxx=max(maxx,sz[v]);
            }
        }
        maxx=max(maxx,tot-sz[u]);
        if(maxx<=tot/2) centroid[centroid[0]!=0]=u;
    }

    // todo 统计从分治重心子节点开始的信息
    void dfs_subtr(int lay,int rt,int u,int fa) {
        belong[lay][u]=rt;
        for(int v:adj[u]) {
            if(v!=fa&&!del[v]) {
                dfs_subtr(lay, rt, v, u);
            }
        }
    }

    // todo 统计从分治重心开始的信息
    void dfs_rt(int lay,int rt,int u,int fa) {

        for(int v:adj[u]) {
            if(v!=fa&&!del[v]) {
                dfs_rt(lay, rt, v, u);
            }
        }
    }

    int build(int lay,int _u,int tot) {
        centroid[0]=centroid[1]=0;
        get_centroid(_u, -1, tot);
        int u=centroid[0];
        get_centroid(u, -1, tot);
        lyr[u]=lay;
        del[u]=1;

        for(int v:adj[u]) if(!del[v]) dfs_subtr(lay, v, v, u);

        // todo 统计从u开始的信息

        for(int v:adj[u]) if(!del[v]) p[build(lay+1, v, sz[v])]=u;
        return u;
    }

    void modify(int u,int s) {
        int lay=lyr[u];
        // todo 修改u信息

        if(u!=s) {
            int v=belong[lay][s];
            // todo 修改s所属的子树v信息
        }
        if(p[u]) modify(p[u], s);
    }

    LL query(int u,int s) {
        LL res=0;
        int lay=lyr[u];
        // todo 合并u信息到答案

        if(u!=s) {
            int v=belong[lay][s];
            // todo 容斥减去s所属的子树v信息
        }
        if(p[u]) res+=query(p[u], s);
        return res;
    }

    void clear(int n) {
        fill(del,del+1+n,0);
        for(int i=0;i<=n;i++) adj[i].clear();
    }
}