# 数论分块

数论分块也叫整除分块，利用在整除运算下值分布为连续的块这个性质，将其打包加速运算。

## 引理1

$$
\forall a,b,c \in \Z, \lfloor \frac{a}{bc} \rfloor = \lfloor \frac{\lfloor \frac{a}{b} \rfloor}{c} \rfloor
$$

证明

$$
\begin{split}
    \frac{a}{b} = \lfloor \frac{a}{b} \rfloor + k (k\in [0,1)) \\
    \lfloor \frac{a}{bc} \rfloor = \lfloor (\lfloor \frac{a}{b} \rfloor + k) \frac{1}{c} \rfloor = \lfloor \frac{\lfloor \frac{a}{b} \rfloor}{c} + \frac{k}{c} \rfloor
\end{split}
$$

观察 $\lfloor \frac{\lfloor \frac{a}{b} \rfloor}{c} + \frac{k}{c} \rfloor$ 这个式子我们是否可以去掉$\frac{k}{c}$这部分，只有在$\frac{\lfloor \frac{a}{b} \rfloor}{c}$离整数只差一点然后加上$\frac{k}{c}$之后能补上去时才可以+1，但是因为$k \in [0,1)$，即$\frac{k}{c} < \frac{1}{c}$，所以无论如何都满足，因此我们可以直接去掉这部分。

因此

$$
\lfloor \frac{a}{bc} \rfloor = \lfloor (\lfloor \frac{a}{b} \rfloor + k) \frac{1}{c} \rfloor = \lfloor \frac{\lfloor \frac{a}{b} \rfloor}{c} \rfloor
$$

## 引理2

$$
\forall n \in \mathbb{N}_{+},  \left|\left\{ \lfloor \frac{n}{d} \rfloor \mid d \in \mathbb{N}_{+},d\leq n \right\}\right| \leq \lfloor 2\sqrt{n} \rfloor
$$

证明

- 当$d \le \lfloor \sqrt{n} \rfloor$ 时，因为$d$只有$\lfloor \sqrt{n} \rfloor$种取值，所以$\lfloor \frac{n}{d} \rfloor$也只有至多$\lfloor \sqrt{n} \rfloor$种取值
- 当$d > \lfloor \sqrt{n} \rfloor$，即$d \ge \lceil \sqrt{n} \rceil$，此时$\lfloor \frac{n}{d} \rfloor \le \lfloor \sqrt{n} \rfloor$，所以此时$\lfloor \frac{n}{d} \rfloor$也只有至多$\lfloor \sqrt{n} \rfloor$种取值

## 数论分块

### 向下取整

对于给定的$n,i$使下式成立的最大的$j=\lfloor \frac{n}{\lfloor \frac{n}{i} \rfloor} \rfloor$

$$
\left\lfloor\dfrac ni\right\rfloor=\left\lfloor\dfrac nj\right\rfloor
$$

这点可以感性地理解。

### 向上取整

向上取整的数论分块可以转化为向下取整：$\lceil \frac{n}{i} \rceil = \lfloor \frac{n-1}{i} \rfloor + 1$

证明：

如果$i|n$，那么$\lceil \frac{n}{i} \rceil = \lfloor \frac{n}{i} \rfloor = \lfloor \frac{n-1}{i} \rfloor + 1$

反之，$\lceil \frac{n}{i} \rceil = \lfloor \frac{n}{i} \rfloor + 1 = \lfloor \frac{n-1}{i} \rfloor + 1$

所以令下式成立的最大的$j=\lfloor \frac{n-1}{\lfloor \frac{n-1}{i} \rfloor} \rfloor$，需要特判$n=1$的情况。

$$
\left\lceil \frac{n}{i} \right\rceil = \left\lceil\dfrac nj\right\rceil
$$

## 应用

对于形如$\sum_{i=1}^nf(i)g(\left\lfloor\dfrac ni\right\rfloor)$的式子计算，我们可以先算出$f(x)$的前缀和，然后每次按照$[i,\lfloor \frac{n}{\lfloor \frac{n}{i} \rfloor} \rfloor]$为一块打包运算。