struct DisjointUnionSet {
    vector<int> fa;

    void init(int n) {
        fa.resize(n+1);
        iota(fa.begin(), fa.end(), 0);
    }

    int find_root(int x) {
        return x==fa[x]?x:fa[x]=find_root(fa[x]);
    }

    bool same(int x,int y) {
        return find_root(x)==find_root(y);
    }

    bool merge(int x,int y) {
        x=find_root(x);
        y=find_root(y);
        if(x==y) return false;
        fa[y]=x;
        return true;
    }

    DisjointUnionSet() = default;
    DisjointUnionSet(int n) { init(n); }
} dsu;