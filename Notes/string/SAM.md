# 后缀自动机

后缀自动机(Suffix Automaton, SAM)是处理字符串问题非常强大的工具。

几乎所有涉及到字符串子串的问题，都可以用后缀自动机解决。此外，后缀自动机还可以拓展到Trie上，解决多字符串的问题（广义后缀自动机）。

<!-- ## 基本概念 -->

## 应用



## 问题集

### [NSUBSTR - Substrings](https://ac.nowcoder.com/acm/contest/37092/B)

模板题，建出SAM后，每个等价类对 `edp[edp[u].link].len+1` 到 `edp[u].len` 的区间进行值为 `edp[u].cnt` 区间修改，使用线段树维护即可。

### [Typewriter](https://ac.nowcoder.com/acm/contest/37092/D)

SAM优化dp。设 $maxlen[i]$ 表示满足 $s[1,x]$ 含有 $s[x+1,i]$ 的最大 $s[x+1,i]$ 长度。设 $left[i]$ 表示等价类 $i$ 在串中出现的最左位置，使用dp可以在线性时间内计算 $left$。$maxlen[i]$ 是可二分的，每次check时，定位子串在SAM上的位置，然后检查 $left$ 即可。计算 $maxlen$ 的复杂度便是 $n \log^2{n}$。

求出 $maxlen$ 之后，直接dp转移即可，转移过程是贪心的，每次复制一段必定是复制越长越好。总的时间复杂度为 $n \log^2{n}$。
