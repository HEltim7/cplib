template<int N,int M,typename T=int> struct SparseTable2D {
    T st[__lg(N)+1][__lg(M)+1][N][M];
    using F=function<T(T,T)>;
    F merge;

    void build(int n,int m,function<T(int,int)> f) {
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                st[0][0][i][j]=f(i,j);

        for(int a=0,s=1<<a;s<=n;a++,s<<=1) {
            for(int i=1,j=1+s/2,k=s;k<=n&&a;i++,j++,k++)
                for(int x=1;x<=m;x++)
                    st[a][0][i][x]=merge(
                        st[a-1][0][i][x],st[a-1][0][j][x]
                    );

            for(int b=1,t=1<<b;t<=m;b++,t<<=1)
                for(int i=1;i+s-1<=n;i++)
                    for(int x=1,y=1+t/2,z=t;z<=m;x++,y++,z++)
                        st[a][b][i][x]=merge(
                            st[a][b-1][i][x],st[a][b-1][i][y]
                        );
        }
    }

    T query(int i,int x,int k,int z) {
        if(i>k||x>z) return {};
        int a=__lg(k-i+1);
        int b=__lg(z-x+1);
        int j=k-(1<<a)+1;
        int y=z-(1<<b)+1;
        return merge(
            merge(st[a][b][i][x],st[a][b][i][y]),
            merge(st[a][b][j][x],st[a][b][j][y])
        );
    }

    SparseTable2D(F merge=[](T x,T y){ return max(x,y); }):merge(merge) {}
};