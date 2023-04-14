template<typename cap,typename cost,int vertex,int edge> struct Flow {
    constexpr static int N=vertex,M=edge,INF=cap(1)<<(8*sizeof(cap)-2);
    int S=0,T=N-1,idx;
    int ne[M],e[M];
    int h[N],q[N],pre[N];
    cap f[M],mf[N];
    cost d[N],w[M];
    bool inq[N];

    void add_edge(int a,int b,cap c,cost d) {
        e[idx]=b,f[idx]=c,w[idx]=d,ne[idx]=h[a],h[a]=idx++;
        e[idx]=a,f[idx]=0,w[idx]=-d,ne[idx]=h[b],h[b]=idx++;
    }

    bool spfa() {
        memset(d,0x3f,sizeof d);
        memset(mf,0,sizeof mf);
        int hh=0,tt=1;
        q[0]=S,d[S]=0,mf[S]=INF;
        while(hh!=tt) {
            int u=q[hh++];
            if(hh==N) hh=0;
            inq[u]=0;

            for(int i=h[u];~i;i=ne[i]) {
                int v=e[i];
                if(f[i]&&d[v]>d[u]+w[i]) {
                    d[v]=d[u]+w[i];
                    pre[v]=i;
                    mf[v]=min(mf[u],f[i]);
                    if(!inq[v]){
                        q[tt++]=v;
                        if(tt==N) tt=0;
                        inq[v]=1;
                    }
                }
            }
        }
        return mf[T]>0;
    }

    pair<cap,cost> maxflow() {
        cap flow=0; cost val=0;
        while(spfa()) {
            flow+=mf[T],val+=mf[T]*d[T];
            for(int i=T;i!=S;i=e[pre[i]^1]) {
                f[pre[i]]-=mf[T];
                f[pre[i]^1]+=mf[T];
            }
        }
        return {flow,val};
    }

    void init() {
        idx=0;
        memset(h, -1, sizeof h);
    }

    Flow() { init(); }
};