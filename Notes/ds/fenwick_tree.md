# 树状数组

# 应用

## 区间加单点和

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

## 静态区间数颜色

## 树状数组上二分