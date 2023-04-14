# 树上启发式合并

[OI wiki](https://oi-wiki.org/graph/dsu-on-tree/)

[CF Tutorial](https://codeforces.com/blog/entry/44351)

dsu on tree 是一种树上的离线算法，可以用来解决形如“多个询问，每次询问一个子树中满足xx性质的节点有多少个”这种问题。

dsu on tree 本质是一种启发式算法，因此被叫做“树上启发式合并”，其过程看起来暴力无比但又可以证明其复杂度是对的。

## 例题引入

[树上数颜色](https://www.luogu.com.cn/problem/U41492)

给定一棵树，每个节点有一种颜色，每次询问一棵子树中不同的颜色数量，允许离线。

### 算法

这题可以使用诸如树套树或者树上莫队来实现，但是使用dsu on tree可以得到一个简单又快速的解法。

尝试预处理出整棵树的答案，如果暴力预处理，那么复杂度显然是 $O(n^2)$ 的，不可接受，但是我们可以采用一个更聪明的方法：

首先预处理出 $dfn$ 序和重链。

我们用 $cnt[i]$ 表示颜色 $i$ 出现的次数，$ans[u]$ 表示 $u$ 点的答案。

设函数 `solve(v,keep=0)` 表示解决 $v$ 子树问题，同时不保留其对 $cnt$ 影响。`solve(v,keep=1)`则表示保留其对 $cnt$ 的影响，即将这棵子树中的颜色计入 $cnt$。

假设当前考虑的点为 $u$，那么我们算法的流程为：

1. 对于 $u$ 所有的轻儿子，调用`solve(v,keep=0)`。
2. 对于 $u$ 的重儿子，调用`solve(v,keep=1)`。
3. 对于 $u$ 所有的轻儿子，利用 $dfn$ 序遍历子树节点，加入其对 $cnt$ 的影响。
4. 将 $u$ 点的颜色加入 $cnt$ 得到 $ans[u]$。
5. 若`solve(u)`传入的`keep=0`，利用 $dfn$ 序暴力删除整棵 $u$ 子树对 $cnt$ 的影响。

#### 正确性

参考算法流程，实际上我们是在暴力处理一颗子树中所有的颜色，并且保证计算一颗子树时，没有其他树外节点对 $cnt$ 造成干扰（因此步骤1和2不能对调），所以正确性是可以保证的。

#### 复杂度

我们的做法可以起来似乎和 $O(n^2)$ 的暴力没什么区别，但为什么复杂度是对的呢？

首先根据重链剖分的结论，根节点到树上任意节点的路径上，其轻/重链切换至多 $O(\log n)$ 次，因为每次轻重链切换子树大小都会至少减少一半。

分步考虑每个点被遍历的次数，对于步骤1、2、4，不难得到其对每个点遍历次数的贡献为 $O(1)$。而对于步骤3，每次当 $v$ 为 $u$ 的轻儿子时，$v$ 子树所有点的贡献+1，即一个点因步骤3增加的遍历次数，等价于其到根的路径上有多少这样的 $v$ 是 $u$ 的轻儿子（即发生轻/重链切换），也就是 $O(\log n)$ 的量级。步骤5所造成的贡献和步骤3一样，也是 $O(\log n)$。

因此每个点都不会被遍历超过 $O(\log n)$ 次，所以总复杂度就是 $O(n\log n)$。

---

至此我们就可以很容易地解决这个问题了。

```cpp
    void init(int u,int fa) {
        sz[u]=1;
        id[u]=++idx;
        ori[id[u]]=u;
        for(int v:adj[u]) {
            if(v!=fa) {
                init(v,u);
                if(sz[v]>sz[hch[u]]) hch[u]=v;
                sz[u]+=sz[v];
            }
        }
        ed[u]=idx;
    }

    void solve(int u,int fa,bool keep) {
        auto add=[](int id) {
            int x=ori[id];
            if(++cnt[c[x]]==1) dif++;
        };

        auto del=[](int id) {
            int x=ori[id];
            if(--cnt[c[x]]==0) dif--;
        };
        
        for(int v:adj[u]) if(v!=fa&&v!=hch[u]) solve(v,u,0);
        if(hch[u]) solve(hch[u],u,1);
        for(int v:adj[u]) 
            if(v!=fa&&v!=hch[u]) 
                for(int i=id[v];i<=ed[v];i++) add(i);
        add(id[u]);
        ans[u]=dif;
        if(!keep) for(int i=id[u];i<=ed[u];i++) del(i);
    }
```

## 应用

### CF 600E Lomsat gelral

> [原题链接](https://codeforces.com/contest/600/problem/E)

#### 题意

给定一棵树，每个节点有一个颜色，我们称一种颜色主导了一棵子树，当且仅当这颗子树中其他所有颜色的数量不比它多。因此一棵树中可能存在多种主导色。求每棵子树的主导色之和。

#### 思路

这题和树上数颜色区别不大，微调代码即可。额外维护一个数组 $tot[i]$ 表示出现次数为 $i$ 的颜色之和，用 $mx$ 表示当且最大颜色数量：

```cpp
auto add=[&](int c) {
    c=idcolor[c];
    tot[cnt[c]]-=c;
    cnt[c]++;
    tot[cnt[c]]+=c;
    mx=max(mx,cnt[c]);
};

auto del=[&](int c) {
    c=idcolor[c];
    tot[cnt[c]]-=c;
    cnt[c]--;
    tot[cnt[c]]+=c;
    if(tot[mx]==0) mx--;
};
```

时间复杂度 $O(n\log n)$。

### CF 570D Tree Requests

> [原题连接](https://codeforces.com/contest/570/problem/D)

#### 题意

给定一棵树，每个点有一个字符，每次询问一棵子树 $u$ 中，是否可以利用所有深度为 $h$ 的点的字符来构成一个回文串（包括空串）。

#### 思路

修改 $cnt$ 数组的定义，$cnt[i][j]$ 表示字符 $i$ 在深度 $j$ 中出现的次数。再用 $tot[i]$ 表示深度为 $i$ 的节点个数，用 $odd[i]$ 表示深度 $i$ 中，出现奇数次的不同字符数。

将询问离线到点上，对于每个询问，根据 $tot[h]$ 的奇偶性分类讨论即可。

```cpp
void solve(int u,bool keep) {
    auto add=[](int x) {
        if((++cnt[s[x]-'a'+1][dep[x]])&1) odd[dep[x]]++;
        else odd[dep[x]]--;
        tot[dep[x]]++;
    };
    auto del=[](int x) {
        if((--cnt[s[x]-'a'+1][dep[x]])&1) odd[dep[x]]++;
        else odd[dep[x]]--;
        tot[dep[x]]--;
    };
    
    for(int v:adj[u]) if(v!=hch[u]) solve(v, 0);
    if(hch[u]) solve(hch[u], 1);
    for(int v:adj[u])
        if(v!=hch[u])
            for(int i=id[v];i<=ed[v];i++) add(i);
    add(id[u]);
    
    for(auto [d,i]:q[u]) {
        if(tot[d]&1) ans[i]=odd[d]==1;
        else ans[i]=odd[d]==0;
    }
    
    if(!keep) for(int i=id[u];i<=ed[u];i++) del(i);
}
```

时间复杂度 $O(n\log n)$。

### 牛客 挑战赛67C 点分治

> [原题连接](https://ac.nowcoder.com/acm/contest/51727/C),[题解](https://ac.nowcoder.com/discuss/1136833)

标程是dsu on tree。~~不懂怎么dsu，直接上点分治~~

