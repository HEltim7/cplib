template<int N,int M,typename T=int> struct SparseTable2D {
    using F=function<T(T,T)>;
    T st[max(__lg(N),__lg(M))+1][N][M];
    F _merge;

    T merge(T a,T b,T c,T d) {
        return _merge(_merge(a,b),_merge(c,d));
    }

    void build(int n,int m,function<T(int,int)> f) {
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                st[0][i][j]=f(i,j);

        for(int b=1,t=1<<b;t<=min(n,m);b++,t<<=1)
            for(int i=1,j=i+t/2,k=i+t-1;k<=n;i++,j++,k++)
                for(int x=1,y=x+t/2,z=x+t-1;z<=m;x++,y++,z++)
                    st[b][i][x]=merge(
                        st[b-1][i][x],st[b-1][i][y],
                        st[b-1][j][x],st[b-1][j][y]
                    );
    }

    T query(int i,int x,int len) {
        int b=__lg(len);
        int j=i+len-1-(1<<b)+1;
        int y=x+len-1-(1<<b)+1;
        return merge(st[b][i][x],st[b][i][y],st[b][j][x],st[b][j][y]);
    }

    SparseTable2D(F merge=[](T x,T y){ return max(x,y); }):_merge(merge) {}
};