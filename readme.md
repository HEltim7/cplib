# cplib

HEltim7的算法竞赛文档库，包括算法模板、笔记和各种工具。

## 目录结构

- Notes 笔记~~和胡扯~~
- Print 模板打印，存放Notes汇总导出的pdf
- Templates 模板代码
- Tools 工具
  - `debug.hpp` 简易debug工具
  - `snippet.cpp` 用来将Templates中的代码转化为vscode snippets
  - `pyjudge.py` 简易对拍/评测脚本

算法模板目录：

> 这个目录并不是完整的Templates内容~~而且我大概率会忘记更新~~。

- data_structures 数据结构
  - [树状数组](./Templates/data_structures/fenwick_tree.cpp)
  - 线段树
    - [线段树](./Templates/data_structures/segment_tree.cpp)
    - [线段树，仅单点修改](./Templates/data_structures/segment_tree_without_lazy_tag.cpp)
    - [势能线段树](./Templates/data_structures/potential_segment_tree.cpp)
    - [可持久化线段树](./Templates/data_structures/persistent_segment_tree.cpp)
    - [线段树合并](./Templates/data_structures/merge_split_segment_tree.cpp)
  - 并查集
    - [并查集](./Templates/data_structures/disjoint_union_set.cpp)
    - [可撤销并查集](./Templates/data_structures/undoable_disjoint_union_set.cpp)
  - [树链剖分](./Templates/data_structures/heavy_path_decomposition.cpp)
  - [字典树](./Templates/data_structures/trie.cpp)
  - [稀疏表](./Templates/data_structures/sparse_table.cpp)
  - [Lick-Cut Tree](./Templates/data_structures/link_cut_tree.cpp)
  - [珂朵莉树](./Templates/data_structures/chtholly_tree.cpp)
  - [虚树](./Templates/data_structures/virtual_tree.cpp)
- dp 动态规划
  - [斜率优化](./Templates/dp/convex_hull_trick.cpp)
- graph 图论
  - [树重心](./Templates/graph/centroid.cpp)
  - 最近公共祖先
    - [倍增](./Templates/graph/lca_binary_lifting.cpp)
    - [树剖](./Templates/graph/lca_heavy_light_decomposition.cpp)
  - [强连通分量](./Templates/graph/tarjan.cpp)
  - [最大流](./Templates/graph/maximum_flow.cpp)
  - [费用流](./Templates/graph/minimum_cost_maximum_flow.cpp)
- math 数学
  - [Modint](./Templates/math/modint.cpp)
  - [组合数](./Templates/math/combinatorial_number.cpp)
  - [欧拉筛](./Templates/math/euler's_sieve.cpp)
  - [欧拉函数](./Templates/math/euler's_totient_function.cpp)
  - [因数分解 O(sqrt(n))](./Templates/math/factorization.cpp)
  - [质因数分解 O(n)+O(log(n))](./Templates/math/prime_factorization.cpp)
  - [拓展欧几里得](./Templates/math/exgcd.cpp)
  - [数论分块](./Templates/math/number_theory_sqrt_decomposition.cpp)
  - [矩阵](./Templates/math/matrix.cpp)
- string 字符串
  - [kmp](./Templates/string/kmp.cpp)
  - [AC自动机](./Templates/string/aho_corasick_automaton.cpp)
  - [后缀数组](./Templates/string/suffix_array.cpp)
  - 后缀自动机
    - [SAM](./Templates/string/suffix_automaton.cpp)
    - [SAM 拓扑序](./Templates/string/suffix_automaton_toporder.cpp)
    - [SAM 维护子串出现次数](./Templates/string/suffix_automaton_count.cpp)
    - [SAM parent树上倍增](./Templates/string/suffix_automaton_lca.cpp)
    - [SAM 线段树合并维护endpos](./Templates/string/suffix_automaton_endpos_maintaining.cpp)
  - 广义后缀自动机
    - [GSAM 在线构造](./Templates/string/general_suffix_automaton.cpp)
    - [GSAM 离线构造](./Templates/string/general_suffix_automaton_offline.cpp)
  - 回文自动机
    - [PAM](./Templates/string/palindrome_automaton.cpp)
    - [双端PAM](./Templates/string/double_end_palidrome_automaton.cpp)
- misc 杂项
  - 分治
    - [cdq分治](./Templates/misc/cdq's_divide_and_conquer.cpp)
    - [点分治](./Templates/misc/centroid_decomposition.cpp)
  - 莫队
    - [普通莫队](./Templates/misc/mo's_algorithm.cpp)
    - [回滚莫队](./Templates/misc/mo's_algorithm_rollback.cpp)
    - [带修莫队](./Templates/misc/mo's_algorithm_modifiable.cpp)
    - [树上莫队](./Templates/misc/mo's_algorithm_on_tree.cpp)
  - [dsu on tree](./Templates/misc/dsu_on_tree.cpp)
  - [离散化](./Templates/misc/discrete.cpp)
  - 哈希
    - [Hashint](./Templates/misc/hashint.cpp)
    - [字符串哈希](./Templates/misc/hash_array.cpp)
    - [树哈希](./Templates/misc/hash_of_tree.cpp)
  - 卡常
    - [快读](./Templates/misc/fast_input.cpp)
    - [快读快写](./Templates/misc/fast_io.cpp)