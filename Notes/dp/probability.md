# 概率&期望dp

概率dp就是用dp来解决概率与数学期望问题。

## DP求期望

概率dp与一般的dp区别不大，而期望dp通常会倒过来处理。

### 问题引入

[恶意竞争](https://ac.nowcoder.com/acm/contest/28263/B)

设 $dp_{i,j}$ 为已经找到了 $i,j$ 种bug，找齐bug还需要的期望时间。

根据定义 $dp_{n,s}=0$，此时已经找齐了所有bug，所以还需要0天。

考虑一个状态 $dp_{i,j}$ 的转移：

- 找到的bug没有新属性： $dp_{i,j} \leftarrow \frac{i}{n} \frac{j}{s} (dp_{i,j}+1)$
- 找到的bug有一个新属性： $dp_{i,j} \leftarrow \frac{n-i}{n} \frac{j}{s} (dp_{i+1,j}+1)$
- 找到的bug有另一个新属性： $dp_{i,j} \leftarrow \frac{i}{n} \frac{s-j}{s} (dp_{i,j+1}+1)$
- 找到的bug有两个全新的属性： $dp_{i,j} \leftarrow \frac{n-i}{n} \frac{s-j}{s} (dp_{i+1,j+1}+1)$

整理一下方程：

$$
dp_{i,j}=\frac{i}{n} \frac{j}{s} dp_{i,j}+\frac{n-i}{n} \frac{j}{s} dp_{i+1,j}+\frac{i}{n} \frac{s-j}{s} dp_{i,j+1}+\frac{n-i}{n} \frac{s-j}{s} dp_{i+1,j+1}+1
$$

发现左右式均存在 $dp_{i,j}$ 项，移项后得到：

$$
dp_{i,j}=\frac {\frac{n-i}{n} \frac{j}{s} dp_{i+1,j}+\frac{i}{n} \frac{s-j}{s} dp_{i,j+1}+\frac{n-i}{n} \frac{s-j}{s} dp_{i+1,j+1}+1}{1-\frac{i}{n} \frac{j}{s}}
$$

然后从 $dp_{n,s}$ 开始倒着dp，$dp_{0,0}$（没有找到任何bug，找齐bug还需要的期望时间） 便是答案。

```cpp
void solve() {
    cout<<fixed<<setprecision(12);
    int n,s;
    cin>>n>>s;
    dp[n][s]=0;
    for(int i=n;i>=0;i--) {
        for(int j=s-(i==n);j>=0;j--) {
            LD pn=LD(i)/n,_pn=1-pn;
            LD ps=LD(j)/s,_ps=1-ps;
            dp[i][j]=(
                pn*ps+
                _pn*ps*(dp[i+1][j]+1)+
                pn*_ps*(dp[i][j+1]+1)+
                _pn*_ps*(dp[i+1][j+1]+1)
            )/(1-pn*ps);
        }
    }
    cout<<dp[0][0];
}
```

## 循环转移/后效性

概率dp比较麻烦的是可能出现循环转移，比较暴力的办法是使用高斯消元，但是高斯消元时间复杂度高且难写，如果问题不是那么的毒瘤，我们可以使用更高效的办法处理循环转移。

### 问题引入

[筛子游戏](https://ac.nowcoder.com/acm/contest/28263/D)

首先我们可以在 $\mathcal{O}(k_1k_2k_3)$ 的时间内预处理 $p_i$，$p_i$ 表示投骰子中了 $i$ 点的概率。特别的，我们用 $p_0$ 表示投到了 $(a,b,c)$ 的概率。

设 $dp_i$ 表示当前点数为 $i$ ，积累到目标点数所需的期望步数。

根据定义 $dp_{i>n}=0$。

考虑一个状态 $dp_i$ 的转移：

- 投到了非 $(a,b,c)$ 的点数，那么转移很简单 $dp_i \leftarrow \sum_{j=3}^{k_1+k_2+k_3} p_j dp_{i+j}+1$
- 否则一夜回到解放前 $dp_i \leftarrow p_0 dp_0+1$

整理一下方程

$$
dp_i=\sum_{j=3}^{k_1+k_2+k_3} p_j dp_{i+j}+p_0 dp_0+1
$$

发现每一项都要从 $dp_0$ 转移，然后 $dp_0$ 又是未知的。

~~怎么办？高斯消元！~~ 有更简单的处理方法，如果 $dp_0$ 已知，那么我们直接嗯转移就能计算出所有的dp值了，也就是说这些dp值均可以写成关于 $dp_0$ 的一个多项式，不妨令 $dp_i=x_idp_0+y_i$。（由于每次运算都是乘和加，所以必然可以写成一个一元一次函数。）

根据定义 $x_{i>n}=0,y_{i>n}=0$。

然后上面的方程就可以写成：

$$
\begin{split}
    dp_i &= \sum_{j=3}^{k_1+k_2+k_3} p_j (x_{i+j} dp_0 +y_{i+j})+p_0 dp_0+1 \\
    &= (\sum_{j=3}^{k_1+k_2+k_3} p_j x_{i+j}+p_0)dp_0+ \sum_{j=3}^{k_1+k_2+k_3} p_j y_{i+j}+1
\end{split}
$$

由于 $dp_i=x_idp_0+y_i$，所以我们可以得到 $x_i,y_i$ 的转移方程：

$$
x_i=\sum_{j=3}^{k_1+k_2+k_3} p_j x_{i+j}+p_0
$$

$$
y_i=\sum_{j=3}^{k_1+k_2+k_3} p_j y_{i+j}+1
$$

发现 $x$ 仅从 $x$ 转移，$y$ 也仅从 $y$ 转移，所以我们就可以分别计算 $x,y$ 了！解出 $x,y$ 后就可以算出答案 $dp_0=x_0 dp_0+y_0$ 即 $dp_0=\frac{y_0}{1-x_0}$。

```cpp
void solve() {
    int n,k1,k2,k3,a,b,c;
    cin>>n>>k1>>k2>>k3>>a>>b>>c;
    for(int i=1;i<=k1;i++)
        for(int j=1;j<=k2;j++)
            for(int k=1;k<=k3;k++)
                if(i==a&&j==b&&k==c) p[0]=LD(1)/(k1*k2*k3);
                else p[i+j+k]+=LD(1)/(k1*k2*k3);

    for(int i=n;i>=0;i--) {
        x[i]=p[0];
        y[i]=1;
        for(int j=3;j<=k1+k2+k3;j++) {
            x[i]+=x[i+j]*p[j];
            y[i]+=y[i+j]*p[j];
        }
    }
    
    LD ans=y[0]/(1-x[0]);
    cout<<fixed<<setprecision(12)<<ans;
}
```

[食堂](https://ac.nowcoder.com/acm/contest/28263/E)

这题也存在循环转移，而且更加复杂，但是处理的方式还是类似的。

设 $dp_{i_j}$ 为队列长度为 $i$ 且当前排在 $j$ 位置时的概率。

考虑转移：

- 排在队头：$dp_{i,1} = p_1 dp_{i,1}+p_2dp_{i,i}+p_4$
- 排在前 $k$ 位：$dp_{i,j} = p_1 dp_{i,j}+p_2dp_{i,j-1}+p_3dp_{i-1,j-1}+p_4$
- 排在后面：$dp_{i,j} = p_1+p_2dp_{i,j-1}+p_3dp_{i-1,j-1}$

我们按照队列长度从小到大遍历，那么 $dp_{i-1}$ 是已知的。

我们发现 $dp_{i,1}$ 会由 $dp_{i,i}$ 转移过来，而计算 $dp_{i,j}$ 又需要已知 $dp_{i,j-1}$，也就是说转移形成了一个环（准确来说是简单环）。

设 $p_x=\frac{p_2}{1-p1}$，我们把每一项都写成关于 $dp_{i,i}$ 的形式，然后不断代入 $dp_{i,i}$，可以得到：

$$
dp_{i,i}=p_x^idp_{i,i}+\sum_{j=1}^ip_x^{i-j}\frac{p_3dp_{i-1,j-1}}{1-p_1}+\sum_{j=1}^{\min \{k,i\}}p_x^{i-j}\frac{p_4}{1-p_1}
$$

式子仅包含 $dp_{i,i}$ 一个未知量，解出 $dp_{i,i}$ 之后再计算其他dp值即可。

上一题的做法是有局限性的，在这题中将dp值拆成 $x,y$ 并不能很好地解决循环问题。

```cpp
void solve() {
    int n,m,k;
    LD p1,p2,p3,p4;
    cin>>n>>m>>k>>p1>>p2>>p3>>p4;
    LD _p1=1-p1,px=p2/_p1;
    pw[0]=1;
    for(int i=1;i<=n;i++) pw[i]=pw[i-1]*px;

    for(int i=1;i<=n;i++) {
        for(int j=1;j<=i;j++)
            dp[i][i]+=pw[i-j]*(p3*dp[i-1][j-1])/_p1;
        for(int j=1;j<=min(k,i);j++)
            dp[i][i]+=pw[i-j]*p4/_p1;
        dp[i][i]=dp[i][i]/(1-pw[i]);

        dp[i][1]=px*dp[i][i]+p4/_p1;
        for(int j=2;j<=min(i,k);j++)
            dp[i][j]=px*dp[i][j-1]+(p3*dp[i-1][j-1]+p4)/_p1;
        for(int j=k+1;j<i;j++)
            dp[i][j]=px*dp[i][j-1]+(p3*dp[i-1][j-1])/_p1;
    }

    cout<<fixed<<setprecision(5)<<dp[n][m]<<endl;
}
```

[迷宫游戏](https://ac.nowcoder.com/acm/contest/28265/C)

设 $dp_i$ 为 $i$ 点走出迷宫的期望步数，$p_k,p_e,p_f,p_c$ 分别为走传送门、走出迷宫、走到父节点、走到指定子节点的概率，$f_i$ 为 $i$ 的父节点，为了方便，如果没有歧义，记 $f=f_i$。

考虑一个状态 $dp_i$ 的转移：

- 若 $i$ 为根（$i=1$）：$dp_i=\sum_{v \in i}p_c(dp_v+1)$
- 若 $i$ 为叶节点： $dp_i=p_kdp_1+p_f(dp_f+1)$
- 若 $i$ 既不是根也不是叶：$dp_i=p_kdp_1+p_f(dp_f+1)+\sum_{v \in i} p_c(dp_v+1)$

实际上可以合并成两种情况：是叶/不是叶。

设 $dp_i=x_idp_1+y_idp_f+z_i$。

**若 $i$ 为叶节点**

$$
\begin{split}
    dp_i &= p_kdp_1+p_f(dp_f+1) \\
    &= p_kdp_1+p_fdp_f+p_f
\end{split}
$$

根据定义

$$
x_i=p_k,y_i=p_f,z_i=p_f
$$

**若 $i$ 不是叶节点**

$$
\begin{split}
    dp_i &= p_kdp_1+p_f(dp_f+1)+\sum_{v \in i} p_c(dp_v+1) \\
    &= p_kdp_1+p_fdp_{f_i}+p_f+\sum_{v \in i} p_c(x_vdp_1+y_vdp_{f_v}+z_v+1) \\
    &= p_kdp_1+p_fdp_f+p_f+\sum_{v \in i} p_c(x_vdp_1+y_vdp_i+z_v+1) \\
    &= p_kdp_1+\sum_{v \in i} p_cx_vdp_1+p_fdp_f+\sum_{v \in i} p_cy_vdp_i+\sum_{v \in i} p_c(z_v+1)+p_f \\
    &= \frac{(p_k+\sum_{v \in i} p_cx_v)dp_1+p_fdp_f+\sum_{v \in i} p_c(z_v+1)+p_f}{1-\sum_{v \in i} p_cy_v}
\end{split}
$$

记 $t=1-\sum_{v \in i} p_cy_v$，根据定义

$$
\begin{split}
x_i &= \frac{p_k+\sum_{v \in i} p_cx_v}{t} \\
y_i &= \frac{p_f}{t} \\
z_i &= \frac{p_f+\sum_{v \in i} p_c(z_v+1)}{t}
\end{split}
$$

这样处理后，叶节点的 $x,y,z$ 均已知，即我们构造出了dp的起点。之后使用树形dp计算出 $x_1,y_1,z_1$，答案就是 $dp_1=x_1dp_1+y_1dp_f+z_1=\frac{z_1}{1-x_1}$。

```cpp
void dfs(int u,int fa) {
    LD t=0;
    for(int v:adj[u]) if(v!=fa) dfs(v,u);
    for(int v:adj[u]) {
        if(v!=fa) {
            t+=y[v];
            x[u]+=x[v];
            z[u]+=z[v]+1;
        }
    }
    t=1-t*pc[u];
    x[u]=(pk[u]+pc[u]*x[u])/t;
    y[u]=(pf[u])/t;
    z[u]=(pf[u]+pc[u]*z[u])/t;
}

bool check(int u,int fa) {
    if(pe[u]>0) return 1;
    bool res=0;
    for(int v:adj[u])
        if(v!=fa&&pc[v]>0) 
            res|=check(v,u);
    return res;
}

void solve() {
    int n;
    cin>>n;
    for(int i=1;i<n;i++) {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for(int i=1;i<=n;i++) {
        cin>>pk[i]>>pe[i];
        pk[i]/=100;
        pe[i]/=100;
        pc[i]=(1-pk[i]-pe[i])/adj[i].size();
        if(i>1) pf[i]=pc[i];
    }

    if(check(1,0)) {
        dfs(1,0);
        LD ans=z[1]/(1-x[1]);
        cout<<fixed<<setprecision(15)<<ans;
    }
    else cout<<"impossible";
}
```