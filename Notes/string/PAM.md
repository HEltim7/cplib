# 回文自动机 $Palindrome Automaton$

回文自动机是解决字符串回文子串问题的有力工具。

```cpp
struct PalindromeAutomaton {
    const static int A=26;
    const static char B='a';
    struct Node {
        int len,link;
        int cnt;
        int ch[A];
    };
    vector<Node> node;
    string str;
    int last;

    int new_node(int len) {
        node.push_back({len});
        return node.size()-1;
    }
    
    void clear() {
        node.clear();
        last=0;
        str="!";
        new_node(0);
        new_node(-1);
        node[0].link=1;
    }

    int getfail(int x) {
        while(str[str.size()-node[x].len-2]!=str.back()) x=node[x].link;
        return x;
    }

    void extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
        }
        last=node[pre].ch[c];
        node[last].cnt++;
    }

    void build(string &s) { for(auto x:s) extend(x); }
    int size() { return node.size(); }

    PalindromeAutomaton() { clear(); }
    PalindromeAutomaton(int sz) { str.reserve(sz),node.reserve(sz),clear(); }
} pam;
```

## Luogu P5496 【模板】回文自动机（PAM）

### 思路

以i结尾的回文串数量等价于对应节点跳$link$的次数，即$PAM$ $fail$树节点的深度。因此我们对于每个节点，维护一个深度值$dep$即可。

时间复杂度$\mathcal{O}(|S|)$。

### 实现

```cpp
constexpr int N=5e5+10;
int dep[N];

struct PalindromeAutomaton {
    ...

    int extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
            dep[cur]=dep[node[cur].link]+1;
        }
        last=node[pre].ch[c];
        node[last].cnt++;
        return dep[last];
    }

    ...
} pam(N);

void solve() {
    string s;
    cin>>s;
    int lastans=0;
    for(auto x:s) {
        char cur=(x-97+lastans)%26+97;
        lastans=pam.extend(cur);
        cout<<lastans<<' ';
    }
}
```

## Luogu P4287 [SHOI2011]双倍回文 

### 思路

在原串上建出$PAM$之后，判定一个串是否为双倍回文串，等价于能否在$PAM$上跳$link$找到一个一半长度的节点。

考虑维护一个数组$cnt$，$cnt_x$表示长度为$x$的串出现过几次。对于一个节点$u$，我们只需要维护从$u$到根这条链的$cnt$就能快速判断当前串是否为双倍回文串。在$fail$树上$dfs$，搜到一个点时`cnt[len]++`，回溯时`cnt[len]--`即可。

时间复杂度$\mathcal{O}(|S|)$。

### 实现

```cpp
constexpr int N=5e5+10;
vector<int> adj[N];
int cnt[N];

struct PalindromeAutomaton {
    ...

    void build(string &s) {
        for(auto x:s) extend(x);
        for(int i=2;i<size();i++)
            adj[node[i].link].push_back(i);
    }
    
    ...
} pam(N);

int ans;
void dfs(int u) {
    int len=pam.node[u].len;
    if(len%4==0) ans=max(ans,cnt[len/2]?len:0);
    cnt[len]++;
    for(int v:adj[u]) dfs(v);
    cnt[len]--;
}

void solve() {
    string s;
    cin>>s>>s;
    pam.build(s);
    dfs(0);
    cout<<ans;
}
```

## Luogu P4555 [国家集训队]最长双回文串 

### 思路

维护两个数组，$pre$与$suf$，$pre_i,suf_i$分别表示$i$位置向前/向后看的最长回文串。那么我们只需要枚举$i$，对$pre_i+suf_{i+1}$取$max$即可。

考虑如何计算这两个信息，$pre$数组很简单，在原串上建出$PAM$，记录每次$extend$的$len$值即可。那么如何计算$suf$呢？发现在原串上很难计算这个信息，于是我们可以将原串反转之后，在反串上再做一遍求$pre$的操作即可。

时间复杂度$\mathcal{O}(|S|)$。

### 实现

```cpp
constexpr int N=1e5+10;
int pre[N],suf[N];

struct PalindromeAutomaton {
    ...

    int extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
        }
        last=node[pre].ch[c];
        node[last].cnt++;
        return node[last].len;
    }

    ...
} pam(N);

void solve() {
    string s;
    cin>>s;
    int n=s.size();
    for(int i=0;i<n;i++) pre[i+1]=pam.extend(s[i]);
    reverse(s.begin(),s.end());
    pam.clear();
    for(int i=0;i<n;i++) suf[n-i]=pam.extend(s[i]);

    int ans=0;
    for(int i=1;i<=n;i++) if(pre[i]&&suf[i+1]) ans=max(ans,pre[i]+suf[i+1]);
    cout<<ans;
}
```

## Luogu P5555 秩序魔咒 

### 思路

要求同时出现在两个串中的回文串，首先第一步肯定是建出这两个串的$PAM$。根据回文树的性质，同时出现在两个串中的回文串必定也在两颗回文树中出现，所以我们对两颗回文树求交集，重叠节点所代表的串即为在两个串中都出现的串，统计一下即可。

时间复杂度$\mathcal{O}(n+m)$。

### 实现

```cpp
constexpr int N=3e5+10;

using PAM=struct PalindromeAutomaton {
    ...
};

PAM pam[2]={PAM(N),PAM(N)};
auto &L=pam[0].node;
auto &R=pam[1].node;
int cnt[N];

void dfs(int l,int r) {
    cnt[L[l].len]++;
    for(int i=0;i<PAM::A;i++)
        if(L[l].ch[i]&&R[r].ch[i])
            dfs(L[l].ch[i],R[r].ch[i]);
}

void solve() {
    string s,t;
    cin>>s>>t>>s>>t;
    pam[0].build(s);
    pam[1].build(t);
    dfs(0,0);
    dfs(1,1);

    int n=min(s.size(),t.size());
    for(int i=n;i>=1;i--) {
        if(cnt[i]) {
            cout<<i<<' '<<cnt[i];
            return;
        }
    }
}
```

## Luogu P1659 [国家集训队]拉拉队排练 

### 思路

直接对原串建$PAM$，统计每个长度的奇回文串出现的次数即可。可以使用拓扑排序来解决这个问题，因为在建立$PAM$时是按照拓扑序构建的，所以我们只需要倒着枚举即可。

时间复杂度$\mathcal{O}(n)$。

### 实现

~~偷懒掏了modint板子~~

```cpp
using LL=long long;
constexpr int N=1e6+10;
LL cnt[N];

template<typename I,typename L,I mod> struct Modint {
    I v;
    I pow(L b) const {
        L res=1,a=v;
        while(b) { if(b&1) res=res*a%mod; b>>=1; a=a*a%mod; }
        return res;
    }
    I inv() const { return pow(mod-2); }

    Modint &operator+=(const Modint &x) { v+=x.v; v-=v>=mod?mod:0; return *this; }
    Modint &operator-=(const Modint &x) { v-=x.v; v+=v<0?mod:0; return *this; }
    Modint &operator*=(const Modint &x) { v=L(1)*v*x.v%mod; return *this; }
    Modint &operator/=(const Modint &x) { v=L(1)*v*x.inv()%mod; return *this; }

    friend Modint operator+(Modint l,const Modint &r) { return l+=r; }
    friend Modint operator-(Modint l,const Modint &r) { return l-=r; }
    friend Modint operator*(Modint l,const Modint &r) { return l*=r; }
    friend Modint operator/(Modint l,const Modint &r) { return l/=r; }

    Modint operator++(int) { auto res=*this; *this+=1; return res; }
    Modint operator--(int) { auto res=*this; *this-=1; return res; }
    Modint operator-  () { return *this*-1; }
    Modint &operator++() { return *this+=1; }
    Modint &operator--() { return *this-=1; }

    bool operator< (const Modint&x) { return v< x.v; }
    bool operator> (const Modint&x) { return v> x.v; }
    bool operator<=(const Modint&x) { return v<=x.v; }
    bool operator>=(const Modint&x) { return v>=x.v; }
    bool operator==(const Modint&x) { return v==x.v; }
    bool operator!=(const Modint&x) { return v!=x.v; }

    friend istream &operator>>(istream &is,Modint &x) { return is>>x.v; }
    friend ostream &operator<<(ostream &os,const Modint &x) { return os<<x.v; }

    Modint(L x=0): v((x%=mod)<0?x+mod:x) {}
    static_assert(0ULL+mod+mod-2<1ULL<<(sizeof(I)*8-1), "Modint overflow");
    static_assert(1ULL*(mod-1)*(mod-1)<1ULL<<(sizeof(L)*8-1), "Modint overflow");
    
}; using Mint=Modint<int,long long,19930726>;

struct PalindromeAutomaton {
    ...

    void count() {
        for(int i=size()-1;i>=2;i--) {
            node[node[i].link].cnt+=node[i].cnt;
            if(node[i].len&1) cnt[node[i].len]+=node[i].cnt;
        }
    }

    ...
} pam(N);

void solve() {
    int n;
    LL k;
    string s;
    cin>>n>>k>>s;
    pam.build(s);
    pam.count();
    
    Mint ans=1;
    for(int i=n;i>=1&&k;i--) {
        LL t=min(k,cnt[i]);
        k-=t;
        ans*=Mint(i).pow(t);
    }
    if(k) cout<<-1;
    else cout<<ans;
}
```

## Luogu P5685 [JSOI2013]快乐的 JYY 

### 思路

与秩序魔咒那道题非常相似，也是求同时出现在两个串中的回文串，我们如法炮制，先在两个串上建好$PAM$，然后统计一下每个串出现的次数，求交集的时候累加每个串出现次数的乘积即可。

时间复杂度$\mathcal{O}(|A|+|B|)$。

### 实现

```cpp
using LL=long long;
constexpr int N=5e4+10;

using PAM=struct PalindromeAutomaton {
    ...

    const static char B='A';

    ...
};

PAM pam[]={PAM(N),PAM(N)};
auto &L=pam[0].node;
auto &R=pam[1].node;

LL dfs(int l,int r) {
    LL res=l>=2?1LL*L[l].cnt*R[r].cnt:0;
    for(int i=0;i<PAM::A;i++)
        if(L[l].ch[i]&&R[r].ch[i])
            res+=dfs(L[l].ch[i],R[r].ch[i]);
    return res;
}

void solve() {
    string s,t;
    cin>>s>>t;
    pam[0].build(s);
    pam[1].build(t);
    pam[0].count();
    pam[1].count();
    cout<<dfs(0,0)+dfs(1,1);
}
```

## CF 17E Palisection

### 思路

求相交的回文串，发现似乎并不好做，即使使用数据结构暴力维护时间复杂度也不优秀。于是我们考虑反着做，先统计所有的回文串对数，然后再减去不相交的回文串即可。

所以问题转化为，如何快速的求出不相交的的回文串对数。我们可以参考[最长双回文串](#p4555-国家集训队最长双回文串)的做法，计算出$pre$与$suf$数组，然后对$pre$求一遍前缀和（或者对$suf$求后缀和），累加乘积即可。

时间复杂度$\mathcal{O}(n)$，空间复杂度$\mathcal{O}(n|\Sigma|)$。

### 实现

这道题只给了`128MB`内存，而$PAM$又十分消耗内存，大概需要`256MB`，所以是会被卡空间的（这道题年代过于久远，那时候还没有$PAM$，~~被t宝卡了半天内存qwq~~）。可以使用`vector`来代替转移边数组，代价每次都需要遍历，算是用时间换空间。

时间复杂度$\mathcal{O}(n|\Sigma|)$，空间复杂度$\mathcal{O}(n)$，实际跑起来空间还是相当逼仄。

另外需要注意题目给的模数不是质数。

```cpp
using LL=long long;
constexpr int N=2e6+10,mod=51123987;

template<typename I,typename L,I mod> struct Modint {
    ...
}; using Mint=Modint<int,long long,mod>;

Mint pre[N],suf[N];
int dep[N];

struct PalindromeAutomaton {
    ...

    struct Node {
        int len,link;
        vector<pair<char,int>> ch;
        int get(char x) {
            for(auto [a,b]:ch) if(a==x) return b;
            return 0;
        }
    };

    int extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].get(c)) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].get(c);
            node[pre].ch.emplace_back(c,cur);
            dep[cur]=dep[node[cur].link]+1;
        }
        last=node[pre].get(c);
        return dep[last];
    }

    ...
} pam(N);

void solve() {
    int n;
    string s;
    cin>>n>>s;
    
    __int128_t tot=0;
    for(int i=0;i<n;i++) pre[i+1]=tot+=pam.extend(s[i]);
    Mint ans=tot*(tot-1)/2%mod;

    pam.clear();
    reverse(s.begin(),s.end());
    for(int i=0;i<n;i++) suf[n-i]=pam.extend(s[i]);

    for(int i=1;i<n;i++) ans-=pre[i]*suf[i+1];
    cout<<ans;
}
```

## Luogu P4762 [CERC2014]Virus synthesis 

### 思路

这道题就比前面的~~水~~题难了。不妨考虑最后的串$S$是如何得来的，其必然是一个回文串（可以为空串）首尾加上若干字符（可以为0）得到。不妨设$ans_i$为构造出$i$号回文串的最少步数，那么我们只需要枚举每个回文串，然后加上相差的长度即可。



考虑如何求得$ans_i$，我们发现这个问题等价于求串$S$的问题，构造$i$回文串必然是一个回文（可以是空串）首尾加上若干字符（可以为0）得到。所以长回文串的最小步数必然由短回文串的最小步数推出，这启发我们使用$dp$来解决这个问题。不妨将$ans_i$记为$dp_i$。

观察操作2，我们可以发现通过这种操作得到的回文串长度为偶数，我们还可以得到一个结论：**构造一个偶回文串，最后一步为操作2肯定不会比最优解差。**

不严谨的证明：不妨设回文串$X=cYYc,|Y| \ge 0$。最后一步为操作1的最小步数：$step1=\min\{dp(cYY)+1,dp(YYc)+1\}$。最后一步为操作二的最小步数：$step2=\min\{dp(cY)+1,dp(Yc)+1\}$。而又因为$dp(cY) \le dp(cYY),dp(Yc) \le dp(YYc)$，所以$step2 \le step1$。

---

考虑枚举决策得到状态转移方程，借助回文自动机分析两种操作，假设当前研究的两个偶回文串为$s,t(|s| < |t|)$。

- 操作1：如果$s$存在到$t$的一个转移，那么在执行$s$的最后一步倍增操作前，我们可以先将$s$拓展一个字符变成一半的$t$，然后再倍增，这样我们只需要比$s$多花1步就能得到$t$。即$dp_t=dp_s+1$。
- 操作2：如果$t$能通过跳后缀链接找到$s$并且$|s| \le \frac{|t|}{2}$，那么我们先将$s$补到$t$长度的一半，然后倍增。即$dp_t=dp_s+\frac{|t|-|s|}{2}+1$。

观察两个状态转移，只有情况2下$s$长度为奇数也可以转移，而因为奇回文串只能通过偶串进行操作1得到，所以通过奇数长的$s$转移就等价于通过某个偶串转移。所以实际上我们**只需要考虑长度为偶数的回文串**。

所以我们只需要计算出每个回文串最长的一个长度不超过其一半的回文串，然后从偶根开始跑$bfs$即可。

时间复杂度$\mathcal{O}(|S|)$。

### 实现

```cpp
constexpr int N=1e5+10;
int dp[N],pre[N],mp[1<<7];
vector<int> adj[N],stk;

struct PalindromeAutomaton {
    ...
    
    const static int A=4;

    void cal_pre(int u,int idx) {
        while(idx+1<stk.size()&&node[stk[idx+1]].len<=node[u].len/2) idx++;
        if(u) pre[u]=stk[idx];
        stk.push_back(u);
        for(int v:adj[u]) cal_pre(v, idx);
        stk.pop_back();
    }

    int bfs() {
        int ans=N,len=str.length()-1;
        queue<int> q;
        q.push(0);
        while(q.size()) {
            int u=q.front();
            q.pop();
            if(node[u].len==2) dp[u]=2;
            dp[u]=min(dp[u],dp[pre[u]]+node[u].len/2-node[pre[u]].len+1);
            ans=min(ans,len-node[u].len+dp[u]);
            
            for(int j=0;j<A;j++) {
                int v=node[u].ch[j];
                if(v) {
                    dp[v]=min(dp[v],dp[u]+1);
                    q.push(v);
                }
            }
        }
        return ans;
    }

    void build(string &s) {
        for(auto x:s) extend(x);
        for(int i=0;i<size();i++) adj[i].clear();
        for(int i=2;i<size();i++) {
            dp[i]=node[i].len;
            int &t=node[i].link;
            if(node[t].len&1) t=node[t].link;
            adj[t].push_back(i);
        }
        cal_pre(0, 0);
    }

    ...
} pam(N);

void solve() {
    string s;
    cin>>s;
    for(auto &x:s) x=mp[x];
    pam.clear();
    pam.build(s);
    cout<<pam.bfs()<<endl;
}

int main() {
    mp['A']='a',mp['G']='b',mp['T']='c',mp['C']='d';
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int t;
    cin>>t;
    while(t--) solve();
    return 0;
}
```

**下面的介绍的题目均衍生自的一个经典问题：[最小回文划分](https://oi-wiki.org/string/pam/#%E6%9C%80%E5%B0%8F%E5%9B%9E%E6%96%87%E5%88%92%E5%88%86)**

## SPOJ IITKWPCE - Let us play with strings

最小回文划分模板题。

```cpp
constexpr int N=2e3+10;
int dp[N],g[N];

struct PalindromeAutomaton {
    ...

    struct Node {
        int len,link;
        int ch[A];
        int dif,slink;
    };

    void extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            int lk=node[getfail(node[pre].link)].ch[c];
            node[cur].link=lk;
            node[pre].ch[c]=cur;
            node[cur].dif=node[cur].len-node[lk].len;
            node[cur].slink=node[cur].dif==node[lk].dif?node[lk].slink:lk;
        }
        last=node[pre].ch[c];
        
        int i=str.size()-1;
        for(int cur=last;cur>=2;cur=node[cur].slink) {
            auto &t=node[cur];
            g[cur]=dp[i-(node[t.slink].len+t.dif)];
            if(t.dif==node[t.link].dif) g[cur]=min(g[cur],g[t.link]);
            dp[i]=min(dp[i],g[cur]+1);
        }
    }

    ...
} pam;

void solve() {
    string s;
    cin>>s;
    int n=s.size();
    for(int i=1;i<=n;i++) dp[i]=1e9;
    pam.clear();
    pam.build(s);
    cout<<dp[s.length()]<<endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(nullptr);
    int t;
    cin>>t;
    while(t--) solve();
    return 0;
}
```

## CF 932G Palindrome Partition 

### 思路

构造字符串：

$$
s'=s_1 s_n s_2 s_{n-1} ... s_{\frac{n}{2}} s_{\frac{n}{2}+1},n=|s|
$$

问题转化为求$s'$的最小偶回文划分，OIwiki已经讲的很细致了。

时间复杂度$\mathcal{O(|s| \log |s|)}$。

### 实现

```cpp
constexpr int N=1e6+10;
Mint dp[N],g[N];

struct PalindromeAutomaton {
    ...

    void extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            int lk=node[getfail(node[pre].link)].ch[c];
            node[cur].link=lk;
            node[pre].ch[c]=cur;
            node[cur].dif=node[cur].len-node[lk].len;
            node[cur].slink=node[cur].dif==node[lk].dif?node[lk].slink:lk;
        }
        last=node[pre].ch[c];
        
        int i=str.size()-1;
        for(int cur=last;cur>=2;cur=node[cur].slink) {
            auto &t=node[cur];
            g[cur]=dp[i-(node[t.slink].len+t.dif)];
            if(t.dif==node[t.link].dif) g[cur]+=g[t.link];
            if(i%2==0) dp[i]+=g[cur];
        }
    }

    ...
} pam;

void solve() {
    string s,t;
    cin>>s;
    if(s.size()&1) {
        cout<<0<<endl;
        return;
    }
    for(int i=0,j=s.size()-1;i<j;i++,j--)
        t.push_back(s[i]),t.push_back(s[j]);

    dp[0]=1;
    pam.build(t);
    cout<<dp[s.length()];
}
```

## CF 906E Reverses

### 思路

与[CF932G](#cf-932g-palindrome-partition)类似，我们需要构造一个串$z$：

$$
z=s_1 t_1 s_2 t_2 ... s_n t_n
$$

不难发现问题转化为求$z$的最小偶回文划分。特别的，我们需要令长度为2的划分代价为0，因为这等价于我们不做翻转操作的情况。

样例的$z$串划分方案：

$$
acbbca|xx|xx|xx|dfeefd
$$

在$dp$转移时，记录一下前驱状态最后倒着输出即可。

时间复杂度$\mathcal{O(|s| \log |s|)}$。

### 实现

```cpp
constexpr int N=1e6+10,INF=1e9;
int dp[N],g[N],predp[N],preg[N];

struct PalindromeAutomaton {
    ...

    void extend(char x) {
        str.push_back(x);
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            int lk=node[getfail(node[pre].link)].ch[c];
            node[cur].link=lk;
            node[pre].ch[c]=cur;
            node[cur].dif=node[cur].len-node[lk].len;
            node[cur].slink=node[cur].dif==node[lk].dif?node[lk].slink:lk;
        }
        last=node[pre].ch[c];
        
        int i=str.size()-1;
        for(int cur=last;cur>=2;cur=node[cur].slink) {
            auto &t=node[cur];
            int len=node[t.slink].len+t.dif;
            g[cur]=dp[i-len];
            preg[cur]=i-len;
            if(t.dif==node[t.link].dif) {
                if(g[t.link]<g[cur]) {
                    g[cur]=g[t.link];
                    preg[cur]=preg[t.link];
                }
            }
            if(i%2==0) {
                if(g[cur]+1<dp[i]) {
                    dp[i]=g[cur]+1;
                    predp[i]=preg[cur];
                }
                if(str[i]==str[i-1]&&dp[i-2]<dp[i]) {
                    dp[i]=dp[i-2];
                    predp[i]=i-2;
                }
            }
        }
    }

    ...
} pam(N);

void solve() {
    string x,y,z;
    cin>>x>>y;
    int n=x.size();
    for(int i=0;i<n;i++) z.push_back(x[i]),z.push_back(y[i]);
    n=z.size();
    
    for(int i=1;i<=n;i++) dp[i]=INF;
    pam.build(z);
    if(dp[z.length()]==INF) {
        cout<<-1<<endl;
        return;
    }
    cout<<dp[z.length()]<<endl;

    int cur=n;
    while(cur) {
        int last=predp[cur];
        if(cur-last>2) cout<<last/2+1<<' '<<cur/2<<endl;
        cur=last;
    }
}
```

## HDU 5421 Victor and String

### 思路

回文自动机的拓展：双端回文自动机。顾名思义，是支持双向插入字符的回文自动机。

由于回文串良好的性质，使得我们只需要对单端$PAM$稍作修改即可得到双端$PAM$：

1. 首先我们需要将$last$变成两个，分别用来表示左右端的最长回文串在$PAM$上的状态。
2. 然后模仿普通$PAM$得到左侧的$getfail()$函数。
3. 特别的，当某次新增的回文串恰好是整个串时，我们需要同时更新两个$last$来保证正确性，并且不难证明只有这种情况需要同时更新。
4. 最后把`string`改成`deque`即可，另外需要去掉特殊符号，否则匹配字符串时会出错。

时间复杂度依然为$\mathcal{O(n)}$，具体的步骤可以参考代码。

### 实现

```cpp
constexpr int N=1e5+10;
LL ans=0;

struct PalindromeAutomaton {
    const static int A=26;
    const static char B='a';
    struct Node {
        int len,link;
        int cnt;
        int ch[A];
    };
    vector<Node> node;
    deque<char> str;
    int last_l,last_r;

    int new_node(int len) {
        node.push_back({len});
        return node.size()-1;
    }
    
    void clear() {
        node.clear();
        last_l=last_r=0;
        str.clear();
        new_node(0);
        new_node(-1);
        node[0].link=1;
    }

    template<typename T> void extend(char x,int &last,T getfail) {
        int c=x-B;
        int pre=getfail(last);
        if(!node[pre].ch[c]) {
            int cur=new_node(node[pre].len+2);
            node[cur].link=node[getfail(node[pre].link)].ch[c];
            node[pre].ch[c]=cur;
            node[cur].cnt=node[node[cur].link].cnt+1;
        }
        last=node[pre].ch[c];
        if(node[last].len==str.size()) last_l=last_r=last;
        ans+=node[last].cnt;
    }

    void extend_l(char x) {
        str.push_front(x);
        extend(x, last_l, [&](int x) {
            int n=int(str.size())-1;
            while(node[x].len+1>n||str[node[x].len+1]!=str[0]) x=node[x].link;
            return x;
        });
    }

    void extend_r(char x) {
        str.push_back(x);
        extend(x, last_r, [&](int x) {
            int n=int(str.size())-1;
            while(n-node[x].len-1<0||str[n-node[x].len-1]!=str[n]) x=node[x].link;
            return x;
        });
    }

    int size() { return node.size(); }

    PalindromeAutomaton() { clear(); }
    PalindromeAutomaton(int sz) { node.reserve(sz),clear(); }
} pam(N);

void solve() {
    int n;
    while(cin>>n) {
        ans=0;
        pam.clear();
        while(n--) {
            int op;
            char in;
            cin>>op;
            if(op==1) cin>>in,pam.extend_l(in);
            else if(op==2) cin>>in,pam.extend_r(in);
            else if(op==3) cout<<pam.size()-2<<endl;
            else cout<<ans<<endl;
        }
    }
}
```