template<int N,typename T=int> struct SparseTable {
    using F=function<T(T,T)>;
    T st[__lg(N)+1][N];
    F merge;

    void build(int n,function<T(int)> f) {
        for(int i=1;i<=n;i++) st[0][i]=f(i);
        for(int k=1,t=1<<k;t<=n;k++,t<<=1)
            for(int i=1,j=i+t-1,mid=i+t/2;j<=n;i++,j++,mid++)
                st[k][i]=merge(st[k-1][i],st[k-1][mid]);
    }

    T query(int l,int r) {
        if(r<l) return 0;
        int k=__lg(r-l+1);
        return merge(st[k][l],st[k][r-(1<<k)+1]);
    }

    SparseTable(F merge=[](T x,T y){ return max(x,y); }) : merge(merge) {}
};