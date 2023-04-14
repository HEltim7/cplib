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

