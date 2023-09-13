# 树状数组

## 板子

```cpp
template<typename T=int,T init=T()> struct Fenwick {
    using F=function<void(T&,const T&)>;
    F add,sub;
    vector<T> tr;

    int lowbit(int x) { return x&(-x); }
    void resize(int n) { tr.resize(n+2,init); }

    void modify(int pos,T val) {
        if(++pos<=0) return;
        while(pos<tr.size()) add(tr[pos],val),pos+=lowbit(pos);
    }

    void reset(int pos) {
        if(++pos<=0) return;
        while(pos<tr.size()) tr[pos]=init,pos+=lowbit(pos);
    }

    T query(int pos) {
        if(++pos<0) return init;
        T res=init;
        while(pos) add(res,tr[pos]),pos-=lowbit(pos);
        return res;
    }
    
    T range_query(int l,int r) {
        T res=query(r);
        sub(res,query(l-1));
        return res;
    }

    explicit Fenwick(
        int n,
        F add=[](T &x,const T &y) { x+=y; },
        F sub=[](T &x,const T &y) { x-=y; })
        : add(add),sub(sub) {
        resize(n);
    }
};
```

二维树状数组

```cpp
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
```

## 区间加区间和

修改 `l,r`

```cpp
Fenwick<LL> dif(n),idif(n);

dif.modify(l, x);
dif.modify(r+1, -x);
idif.modify(l, 1LL*l*x);
idif.modify(r+1, -1LL*(r+1)*x);
```

查询 `l,r`

```cpp
auto get=[&](int x) {
    return dif.query(x)*(x+1)-idif.query(x);
};
cout<<get(r)-get(l-1)<<endl;
```

## 矩阵加矩阵和

修改 `a,b,c,d`

```cpp
Fenwick2D<LL> dif(n,m),idif(n,m),jdif(n,m),ijdif(n,m);

auto modify=[&](int r,int c,LL x) {
    dif.modify(r, c, x);
    idif.modify(r, c, x*r);
    jdif.modify(r, c, x*c);
    ijdif.modify(r, c, x*r*c);
};

modify(a, b, x);
modify(a, d+1, -x);
modify(c+1, b, -x);
modify(c+1, d+1, x);
```

查询 `a,b,c,d`

```cpp
auto get=[&](int r,int c) {
    return 
        (1LL*r*c+r+c+1)*dif.query(r, c)-
        (r+1)*jdif.query(r, c)-
        (c+1)*idif.query(r, c)+
        ijdif.query(r, c);
};

cout<<get(c, d)-get(c, b-1)-get(a-1, d)+get(a-1, b-1)<<endl;
```