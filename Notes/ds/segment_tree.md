# 线段树

## 区间套区间问题

对于类似询问一个区间 $[l,r]$ 其中 $l \in [L,R],r \in [x,y]$ 这样的区间套区间问题，通常可以用离线询问+时间线差分+线段树维护历史前缀和解决。

经典问题：[2021 南京 E 派蒙线段树](https://codeforces.com/gym/103470/problem/E)

> 题目大意：
> 维护一个数组，每次修改将区间中的每个数都加上x，然后询问版本[x,y]中，区间[l,r]的平方和。

题解参考：[github](https://github.com/HEltim7/cpcode/blob/master/Archive/ICPC/2021%E5%8D%97%E4%BA%AC/E.md)

以及一道相似的题 [LuoTianyi and the Function](https://codeforces.com/contest/1824/problem/D)

> 题目大意
> 给定一个序列，定义 $g(i,j)$ 为最大的一个 $x$ 满足由区间 $[i,j]$ 构成的集合等于由区间 $[x,j]$ 构成的集合。
> 询问 ...

维护每个数上一次出现的位置，发现每次仅会将一个区间修改为同一个值（并且一定更大），因此也可以转化为和上一题同样的做法。

# 线段树合并/分裂

## 板子

基于可重集的维护。

```cpp
namespace sgt {
    #define lch (tr[u].lc)
    #define rch (tr[u].rc)

    struct Node {
        int lc=0,rc=0;
		int cnt=0;
    };
    constexpr int M=N*(__lg(N)+1)*2;
    vector<Node> tr(M);
    int idx,rng_l,rng_r;

    int new_node() {
        assert(++idx<M);
        tr[idx]={};
        return idx;
    }

    void pushup(int u) {
        tr[u].cnt=tr[lch].cnt+tr[rch].cnt;
    }

    void update(int u) {
        if(!u) return;

    }

    void pushdn(int u) {

    }

    void merge(int &u,int v,int l,int r) {
        if(!u||!v) u=u|v;
        else if(l==r) {
            tr[u].cnt+=tr[v].cnt;
        }
        else {
            pushdn(u);
            int mid=(l+r)/2;
            merge(tr[u].lc, tr[v].lc, l, mid);
            merge(tr[u].rc, tr[v].rc, mid+1, r);
            pushup(u),pushup(v);
        }
    }
	void merge(int &u,int v) { merge(u,v,rng_l,rng_r); }

    pair<int,int> split(int u,int l,int r,int p) {
        if(r<p) return {u,0};
        if(l>=p) return {0,u};
        pushdn(u);
        int v=new_node();
        int mid=(l+r)/2;
        auto [a,b]=split(lch, l, mid, p);
        auto [c,d]=split(rch, mid+1, r, p);
        tr[u].lc=a,tr[u].rc=c;
        tr[v].lc=b,tr[v].rc=d;
        pushup(u),pushup(v);
        return {u,v};
    }
	pair<int,int> split(int u,int p) { return split(u,rng_l,rng_r,p); }

    int extract(int &u,int l,int r,int x,int y) {
        auto [a,b]=split(u, l, r, x);
        auto [c,d]=split(b, l, r, y+1);
        merge(a, d, l, r);
        return u=a,c;
    }
	int extract(int &u,int l,int r) { return extract(u,rng_l,rng_r,l,r); }

    int query(int u,int l,int r,int x,int y) {
        if(!u||l>y||r<x) return {};
        if(l>=x&&r<=y) {
			return tr[u].cnt;
		}
        pushdn(u);
        int mid=(l+r)/2;
        return query(lch, l, mid, x, y)+query(rch, mid+1, r, x, y);
    }
	int query(int u,int l,int r) { return query(u,rng_l,rng_r,l,r); }

    void modify(int &u,int l,int r,int p,int v) {
        if(!u) u=new_node();
        if(l==r) {
            tr[u].cnt+=v;
        }
        else {
            pushdn(u);
            int mid=(l+r)/2;
            if(p<=mid) modify(lch, l, mid, p, v);
            else modify(rch, mid+1, r, p, v);
            pushup(u);
        }
    }
	void modify(int &u,int p,int v) { modify(u,rng_l,rng_r,p,v); }

	void init(int l,int r) { idx=0,rng_l=l,rng_r=r; }

    #undef lch
    #undef rch
}
```

线段树合并除了合并和分裂外，剩下的和动态开点线段树保持一致，因此也需要考虑线段树共有的问题：

- 信息+信息
- 信息初始化与信息+空信息
- 信息+懒标记
- 懒标记+懒标记
- 懒标记清空

线段树合并更需要考虑是如何在合并/分裂时维护信息的变化，这部分的问题就比一般的线段树灵活很多。

合并子树需要考虑的：

- 是否需要合并叶节点，合并叶子时的信息维护
- 合并时是否需要新建节点来保留子树结构

合并需要保留子树结构最常见于树上线段树合并，此时需要每次都新建节点，需要的空间也会翻倍。

```cpp
int merge(int x,int y) {
    if(!x||!y) return x|y;
    int u=new_node();
    lch=merge(tr[x].lc,tr[y].lc);
    rch=merge(tr[x].rc,tr[y].rc);
    tr[u].cnt=tr[lch].cnt+tr[rch].cnt;
    return u;
}
```

考虑合并的复杂度，由于一棵树中的一个节点至多被合并一次，即一个节点的贡献至多为1，所以无论以何种顺序合并，复杂度都等于总节点数，为 $\mathcal{O}(n \log n)$。分裂为合并的逆过程，复杂度和合并保持一致。

## 技巧

### 按X维分裂维护Y维信息的线段树

考虑现在有一颗权值线段树，现在要把线段树按照区间的 $p$ 位置分裂。

为了简化问题，不妨设原数组是一个排列。

显然，我们是不能按照区间直接分裂一颗权值线段树的，因此我们需要在线段树上保存值在区间的最左位置 $minp$ 和最右位置 $maxp$。

那么分裂当前节点 $u$ 时：

- 如果 $[minp,maxp]$ 仅在 $p$ 的一侧，那么不需要分裂。
- 否则递归地分裂左右子树，假设左子树分裂为了 $a,b$ 两棵树，右子树分裂为了 $c,d$ 两棵树，$u$ 分裂为 $u,v$，那么 $a,c$ 归到 $u$，$b,d$ 归到 $v$。

```cpp
pair<int,int> split(int u,int k) {
    if(tr[u].minp>=k) return {0,u};
    if(tr[u].maxp<k) return {u,0};
    pushdn(u);
    int v=new_node();
    auto [a,b]=split(lch, k);
    auto [c,d]=split(rch, k);
    tr[u].lc=a,tr[u].rc=c;
    tr[v].lc=b,tr[v].rc=d;
    pushup(u),pushup(v);
    return {u,v};
}
```

同样的，我们可以按照权值分裂区间线段树。

如果原数组不是一个排列，那么和上述做法的区别是需要分裂叶子节点，而排列递归到叶子一定不需要分裂。

分裂叶子节点需要计算新的 $maxp,minp$，可以在 `std::set<pair<int,int>>` 上二分解决。不过这样总归还是比较麻烦，所以尽可能转化为排列来做。

复杂度与一般的分裂一致，为 $\mathcal{O}(n \log n)$。

## 问题集

[ix35的题单](https://www.luogu.com.cn/training/2971) 收录了一些题。

### [【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)

模板题，直接使用线段树合并/分裂即可。

需要额外处理的部分是求第 $k$ 大。

```cpp
int kth(int u,int l,int r,int k) {
    if(tr[u].cnt<k) return -1;
    if(l==r) return l;
    pushdn(u);
    int mid=(l+r)/2;
    if(tr[lch].cnt>=k) return kth(lch, l, mid, k);
    return kth(rch, mid+1, r, k-tr[lch].cnt);
}
```

> [评测记录](https://www.luogu.com.cn/record/124013165)

### [这场ucup](https://qoj.ac/contest/1207) 的F。

线段树合并加速dp。

### [Peaks](https://www.luogu.com.cn/problem/P4197)

#### 题意

给定 $n$ 个带权点 $m$ 条带权边的图，$q$ 次询问，每次询问从 $v$ 开始，只经过权值不超过 $x$ 能到达的点中第 $k$ 大的权值是多少。

$n \le 10^5,m,q \le 5 \times 10^5$。

#### 思路

首先离线询问，然后按照路径的难度加边。使用权值线段树维护连通块中的 $h$，使用并查集维护连通性和线段树的根节点。加边时同时进行并查集的合并与线段树的合并。询问第 $k$ 大在权值线段树上二分完成。

本题的做法还有很多，使用平衡树代替线段树、克鲁斯卡尔重构树等。

复杂度为 $\mathcal{O}(n \log n + m \log m + q \log q)$。

#### 实现

> [评测记录](https://www.luogu.com.cn/record/123881114)

```cpp
void merge(int &u,int v,int l,int r) {
    if(!u||!v) u=u|v;
    // 注意处理叶节点的合并，因为可能有重复的h
    else if(l==r) tr[u].cnt+=tr[v].cnt;
    else {
        int mid=(l+r)/2;
        merge(tr[u].lc, tr[v].lc, l, mid);
        merge(tr[u].rc, tr[v].rc, mid+1, r);
        pushup(u);
    }
}

int kth(int u,int l,int r,int k) {
    if(tr[u].cnt<k) return -1;
    if(l==r) return l;
    int mid=(l+r)/2;
    if(tr[lch].cnt>=k) return kth(lch, l, mid, k);
    return kth(rch, mid+1, r, k-tr[lch].cnt);
}
```

### [ucup-2 Stage 1: Qingdao G. Couleur](https://contest.ucup.ac/contest/1339/problem/7108)

用分裂权值线段树的技巧解决。参考jiangly的 [题解](https://www.luogu.com.cn/blog/jiangly/the-2nd-universal-cup-stage-1-qingdao) 和 [实现](https://contest.ucup.ac/submission/161108)。
