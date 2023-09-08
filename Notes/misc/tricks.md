# 技巧汇总

## 图论

### 树上点对间距离转根点间距离

在根为 $r$ 的有根树上求 $u,v$ 点之间的距离，可以转化为：

$$
\begin{equation}
\begin{split}
    d(u,v) &= d(u,lca(u,v))+d(v,lca(u,v)) \\
    &=d(u,r)-d(lca(u,v),r)+d(v,r)-d(lca(u,v),r) \\
    &=d(u,r)+d(v,r)-2d(lca(u,v),r)
\end{split}
\end{equation}
$$

于是求点对间距离可以转化为等价的两个子问题：

- 给定 $u$ 点，求 $d(u,r)$
- 给定 $u,v$，求 $lca(u,v)$

## 杂项

### 异或性质&结论

**性质1** 

如果 $x<y<z$ 那么 $min(x \oplus y, y \oplus z) < x \oplus z$。

假设 $x,y,z$ 前 $k+1$ 的高位都相同，第 $k$ 位不同，那么可以推出 $x$ 的第 $k$ 位为 $0$，$z$ 的第 $k$ 位为 $1$，所以 $x \oplus z \ge 2^k$。

- 若 $y$ 的第 $k$ 位为 $0$ 那么 $x \oplus y < 2^{k} < x \oplus z$
- 若 $y$ 的第 $k$ 位为 $1$ 那么 $y \oplus z < 2^{k} < x \oplus z$

例题：[ABC 308 G - Minimum Xor Pair Query](https://atcoder.jp/contests/abc308/tasks/abc308_g)，[题解](https://atcoder.jp/contests/abc308/editorial/6716)

**性质2**

给定 $x,z$ 满足 $x \oplus y < z$ 的 $y$ 最多有 $O(\log X)$ 段连续的区间。

例题 [UCUP Ōokayama N. XOR Reachable](https://qoj.ac/contest/1207/problem/6330)，[题解](https://qoj.ac/download.php?type=attachments&id=1207&r=1)