template<typename T=int,T init=T()> struct Fenwick2D {
    using F=function<void(T&,const T&)>;
    F add,sub;
    vector<vector<T>> tr;

    int lowbit(int x) { return x&(-x); }
    void resize(int r,int c) { tr.resize(r+2,vector<T>(c+2,init)); }

    void modify(int r,int c,T val) {
        if(++r<=0||++c<=0) return;
        for(int i=r;i<tr.size();i+=lowbit(i))
            for(int j=c;j<tr[i].size();j+=lowbit(j))
                add(tr[i][j],val);
    }

    void reset(int r,int c) {
        if(++r<=0||++c<=0) return;
        for(int i=r;i<tr.size();i+=lowbit(i))
            for(int j=c;j<tr[i].size();j+=lowbit(j))
                tr[i][j]=init;
    }

    T query(int r,int c) {
        if(++r<0||++c<0) return init;
        T res=init;
        for(int i=r;i;i-=lowbit(i))
            for(int j=c;j;j-=lowbit(j))
                add(res,tr[i][j]);
        return res;
    }

    T matrix_query(int r,int c,int x,int y) {
        T res=query(x,y);
        sub(res,query(x,c-1));
        sub(res,query(r-1,y));
        add(res,query(r-1,c-1));
        return res;
    }
    
    explicit Fenwick2D(
        int r,int c,
        F add=[](T &x,const T &y) { x+=y; },
        F sub=[](T &x,const T &y) { x-=y; })
        : add(add),sub(sub) {
        resize(r,c);
    }
};