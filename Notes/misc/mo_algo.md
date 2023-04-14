# 莫队

## 普通莫队

将询问离线，并按照左端点块号为第一关键字，右端点为第二关键字进行排序。

设区间长度 $n$，询问次数 $q$，块长 $b$，块数 $\frac{n}{b}$，那么左指针移动的次数最多为 $bq$，右指针移动次数最多为 $\frac{n}{b}n$，令 $bq=n\frac{n}{b}$ 时最优，即 $b=\sqrt \frac{n^2}{q}$。

时间复杂度 $\mathcal{O}(n \sqrt{q} + q \log q)$。

```cpp
namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=320;
    using Query=tuple<int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_]=x;
            const auto &[L,R,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return getid(l)&1?r<R:r>R;
        });

        int l=1,r=0,res=0;
        
        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };
        
        for(const auto &[L,R,id]:query) {
            while(l>L) add(--l);
            while(r<R) add(++r);
            while(l<L) del(l++);
            while(r>R) del(r--);
            // TODO ans[id]=res;
        }
    }
}
```

### trick

可以用奇偶化排序优化莫队的指针移动次数，即左端点所在块为奇数时右端点排升序，反之按降序排序。这样指针在处理奇数块时大概率滚到较右边，然后处理偶数块时刚好能从右侧一直滚回左侧。这个优化较为玄学，有一定概率能优化常数（也可能变慢）。不能用于回滚莫队。

```cpp
return getid(l)&1?r<R:r>R;
```

关于 $l,r$ 指针移动顺序，应先拓展区间，然后收缩区间，防止维护的区间长度变为负数。

## 回滚莫队

也叫只增莫队，用来对付只可加不可减的信息维护，比如 $max$。

算法流程：

- 如果当前询问的左端点块号与之前一次不同，设当前块右端点为 $rbd$，暴力将 $l$ 拉到 $rbd+1$，将 $r$ 拉到 $rbd$，将维护的信息重置到初始状态。注意，因为已经排好序，左端点必定会在 $rbd+1$ 左侧，但 $r$ 则不一定。
- 如果当前询问仅在一个块之内
  - 备份答案（实际上，此时答案必定为初始状态）
  - 暴力统计区间信息，并计算答案
  - 暴力删除区间信息，回滚答案（需要保证答案恢复到初始状态）
- 如果询问跨块
  - 首先滚右端点
  - 备份答案，然后滚左端点，计算答案
  - 将左端点回滚至 $rbd+1$，回滚答案。

复杂度同普通莫队，但常数更大， $\mathcal{O}(n \sqrt{q} + q \log q)$。

```cpp
namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=320;
    using Query=tuple<int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve(int n) {
        auto getid=[](int x) {
            return x/block;
        };
        auto getr=[&](int id) {
            return min(id*block+block-1,n);
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_]=x;
            const auto &[L,R,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return r<R;
        });

        int l=1,r=0,lastid=-1,rbd=0;

        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };

        auto reset=[&]() {
            while(r<rbd) add(++r);
            while(r>rbd) del(r--);
            while(l<=rbd) del(l++);
            // res=bak=0;
        };
        
        for(const auto &[L,R,id]:query) {
            if(getid(L)!=lastid) {
                lastid=getid(L);
                rbd=getr(lastid);
                reset();
            }

            if(getid(L)==getid(R)) {
                // bak=res;
                for(int i=L;i<=R;i++) add(i);
                // ans[id]=res;
                for(int i=L;i<=R;i++) del(i);
                // res=bak;
            }
            else {
                while(r<R) add(++r);
                // bak=res;
                while(l>L) add(--l);
                // ans[id]=res;
                while(l<=rbd) del(l++);
                // res=bak;
            }
        }
    }
}
```

## 带修莫队

通过增加一维时间维，就可以让莫队处理带修改的问题。

将时间设定为进行修改的次数，初始时间设为0，即0次修改。然后将所有询问按照左端点块号为第一关键字，右端点块号为第二关键字，时间为第三关键字排序。

算法流程和普通莫队类似：

- 首先滚左右端点
- 再对齐时间：
  - 若当前修改不在询问区间内，则仅需修改
  - 否则，修改的同时更新维护的区间信息

设区间长度为 $n$，询问数为 $q$，修改数为 $m$，块长为 $b$，块数为 $\frac{n}{b}$。左右指针移动次数均为 $bq$，时间指针移动次数为 $(\frac{n}{b})^2 m$，当块长取 $\frac{n^\frac{2}{3}m^\frac{1}{3}}{q^\frac{1}{3}}$ 时得到最优复杂度 $O(n^\frac{2}{3} q^\frac{2}{3} m^\frac{1}{3} + q \log q)$。若令 $n,q,m$ 同阶，则 $b=n^\frac{2}{3}$，时间复杂度 $O(n^\frac{5}{3} + q \log q)$。

```cpp
namespace mo {
    constexpr int N=1e5+10,Q=1e5+10,block=2155;
    using Query=tuple<int,int,int,int>;
    vector<Query> query;
    int ans[Q];
    
    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,t,_]=x;
            const auto &[L,R,T,__]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            if(getid(r)!=getid(R)) return getid(r)<getid(R);
            return t<T;
        });

        int l=1,r=0,tm=0;
        
        auto add=[&](int idx) {
            
        };

        auto del=[&](int idx) {
            
        };

        auto modify=[&](int t) {
            // int idx=pos[t];
            // if(idx>=l&&idx<=r) del(idx);
            // swap(w[idx],nw[t]);
            // if(idx>=l&&idx<=r) add(idx);
        };

        auto rollback=[&](int t) {
            modify(t);
        };
        
        for(const auto &[L,R,T,id]:query) {
            while(l>L) add(--l);
            while(r<R) add(++r);
            while(l<L) del(l++);
            while(r>R) del(r--);
            while(tm<T) modify(++tm);
            while(tm>T) rollback(tm--);
            // TODO ans[id]=res;
        }
    }
}
```

### trick

在进行修改后，可以将修改值置成反效果，这样在撤销修改时就可以复用修改的代码。例如，如果是单点赋值的修改，我们可以swap数组的值和修改的值，之后`rollback`直接调用`modify`即可。

```cpp
auto modify=[&](int t) {
    int idx=pos[t];
    if(idx>=l&&idx<=r) del(idx);
    swap(w[idx],nw[t]);
    if(idx>=l&&idx<=r) add(idx);
};

auto rollback=[&](int t) {
    modify(t);
};
```

## 树上莫队

通过 $\text{dfs}$ 序将一棵树拍平成一个序列，我们就可以用莫队来解决树上路径询问的问题，序列长度为节点数x2。

用 $first,last$ 表示 $u$ 在 $\text{dfs}$ 序出现的前后两个位置。

设 $first_u < first_v$，则 $u,v$ 间路径可以转化为：

- 若 $lca(u,v)=u$，那么询问区间为 $[first_u,first_v]$。
- 反之询问区间为 $[last_u,first_v]$，并加上父节点 $lca(u,v)$。

用 $odd$ 表示节点出现次数的奇偶性，若一个点出现在树上路径上，则 $odd=1$，反之 $odd=0$。

之后使用普通莫队的做法即可，块长取 $b=2\sqrt \frac{n^2}{q}$ 最优。

复杂度 $\mathcal{O}(n \sqrt{q} + q \log q)$，由于 $n$ 翻倍且需要求 $lca$，常数较大。

```cpp
namespace mo {
    constexpr int N=5e4+10,Q=1e5+10,block=320;
    using Query=tuple<int,int,int,int>;
    vector<Query> query;
    vector<int> adj[N];
    int uid[N<<1],first[N],last[N],idx;
    bool odd[N];
    int ans[Q];
    
    namespace hpd {
        int dep[N],sz[N],top[N],p[N],hch[N];
    
        void dfs1(int u,int fa,int d) {
            dep[u]=d,p[u]=fa,sz[u]=1;
            for(int v:adj[u]) {
                if(v==fa) continue;
                dfs1(v,u,d+1);
                sz[u]+=sz[v];
                if(sz[hch[u]]<sz[v]) hch[u]=v;
            }
        }
    
        void dfs2(int u,int t) {
            top[u]=t;
            if(!hch[u]) return;
            dfs2(hch[u],t);
            for(int v:adj[u])
                if(v!=p[u]&&v!=hch[u]) dfs2(v,v);
        }
    
        int lca(int x,int y) {
            while(top[x]!=top[y]) {
                if(dep[top[x]]<dep[top[y]]) swap(x,y);
                x=p[top[x]];
            }
            if(dep[x]<dep[y]) swap(x,y);
            return y;
        }
    
        void init() {
            dfs1(1,-1,1); dfs2(1,1);
        }

        void clear(int n) {
            fill(hch, hch+n+1, 0);
        }
    }

    void dfs(int u,int fa) {
        uid[++idx]=u;
        first[u]=idx;
        for(int v:adj[u]) if(v!=fa) dfs(v,u);
        uid[++idx]=u;
        last[u]=idx;
    }

    void add_query(int u,int v,int id) {
        if(first[u]>first[v]) swap(u,v);
        int p=hpd::lca(u,v);
        int l,r,pidx;
        if(u==p) l=first[u],r=first[v],pidx=0;
        else l=last[u],r=first[v],pidx=first[p];
        query.emplace_back(l,r,pidx,id);
    }

    void solve() {
        auto getid=[](int x) {
            return x/block;
        };

        sort(query.begin(),query.end(),[&](const Query &x,const Query &y) {
            const auto &[l,r,_,__]=x;
            const auto &[L,R,___,____]=y;
            if(getid(l)!=getid(L)) return getid(l)<getid(L);
            return getid(l)&1?r<R:r>R;
        });

        auto _add=[&](int u) {
            
        };

        auto _del=[&](int u) {
            
        };

        auto extend=[&](int idx) {
            int u=uid[idx];
            if(odd[u]^=1) _add(u);
            else _del(u);
        };

        int l=1,r=0;
        for(const auto &[L,R,pidx,id]:query) {
            while(l>L) extend(--l);
            while(r<R) extend(++r);
            while(l<L) extend(l++);
            while(r>R) extend(r--);
            if(pidx) extend(pidx);
            // TODO ans[id]=res;
            if(pidx) extend(pidx);
        }
    }

    void init() {
        hpd::init();
        dfs(1,-1);
    }

    void clear(int n) {
        idx=0;
        query.clear();
        fill(odd, odd+n+1, 0);
        for(int i=0;i<=n;i++) adj[i].clear();
        hpd::clear(n);
    }
}
```