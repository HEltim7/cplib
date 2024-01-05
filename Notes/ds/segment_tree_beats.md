# Segment Tree Beats

吉如一老师提出的数据结构，所以也叫吉司机线段树。

> [OI Wiki 区间最值操作 & 区间历史最值](https://oi-wiki.org/ds/seg-beats/)

一般的线段树可以维护区间取 $\max$ 区间查 $\max$ ，但是如果要在取 $\max$ 时维护一些额外信息，例如区间和，那么普通的线段树就显得有些力不从心了。

## 维护区间最值区间和

引入一个例题：

[2015 HDU 多校-2 Gorgeous Sequence](https://vjudge.net/problem/HDU-5306)

> 维护一个序列 [a] ：
> 
> `0 l r t` $\forall l\le i\le r,\ a_i=\min(a_i,t)$。
> 
> `1 l r` 输出区间 [[l,r]] 中的最大值。
> 
> `2 l r` 输出区间和。

在线段树中维护以下信息：

- `mx1` 区间最大值
- `mx2` 区间次大值
- `cnt` 最大值出现次数
- `sum` 区间和

同时维护以下懒标记：

- `mn` 将区间对 `mn` 取 $\min$

**处理信息合并**

这部分和普通线段树保持一致。

```cpp
friend Info operator+(const Info &l,const Info &r) {
    Info res;
    res.mx1=max(l.mx1,r.mx1);
    res.sum=l.sum+r.sum;
    if(l.mx1==r.mx1) {
        res.mx2=max(l.mx2,r.mx2);
        res.cnt=l.cnt+r.cnt;
    }
    else if(l.mx1>r.mx1) {
        res.mx2=max(l.mx2,r.mx1);
        res.cnt=l.cnt;
    }
    else {
        res.mx2=max(l.mx1,r.mx2);
        res.cnt=r.cnt;
    }
    return res;
}
```

**处理区间修改**

在执行区间取 $\min$ 的操作时，分类讨论更新信息：

- 若 $mx_1 \le t$ ，那么什么都不用干
- 若 $mx_2 < t \le mx_1$ ，那么当前的仅有最大值被更新
  - $sum = sum - (mx_1 - t) * cnt$
  - $mx_1 = t$
  - $mx_2,cnt$ 保持不变
  - 更新完后对当前节点打上懒标记返回
- 若 $t \le mx_2$
  - 那么可能有多个不同的值被更新，递归修改然后 `pushup`。
  - 为什么 $t = mx_2$ 仅有最大值被更新也被归为这一类？这是为了避免更新完后 $mx_1 = mx_2$。

在 $t \le mx_2$ 中递归修改的一步便能体现吉司机线段树的核心思想。

考虑分析这部分操作的复杂度。

记 $d$ 为整个序列不同的数的个数。每次一个线段树节点执行向下递归的操作后，至少会将 $d$ 减少1

# 问题集

[【模板】线段树 3](https://www.luogu.com.cn/problem/P6242)

[2023 牛客多校-1 C](https://ac.nowcoder.com/acm/contest/57355/C)

[2020 ICPC 南京 J. Just Another Game of Stones](https://codeforces.com/gym/102992/problem/J)

