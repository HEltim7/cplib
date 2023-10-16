using Hash=pair<int,int>;
constexpr int p1=998244353,p2=int(1e9)+7;
Hash operator*(Hash x,Hash y) {
    return Hash(1LL*x.first*y.first%p1,1LL*x.second*y.second%p2);
}
Hash operator+(Hash x,Hash y) {
    return Hash((x.first+y.first)%p1,(x.second+y.second)%p2);
}
Hash operator-(Hash x,Hash y) {
    return Hash((x.first-y.first+p1)%p1,(x.second-y.second+p2)%p2);
}

struct HashArray {
    constexpr static Hash base{114514,1919810};
    vector<Hash> hsh,pw;

    void push_back(int x) {
        hsh.push_back(hsh.back()*base+Hash(x,x));
        pw.push_back(pw.back()*base);
    }

    template<class S> void append(const S &s) {
        for(auto x:s) push_back(x);
    }

    Hash query(int l,int r) {
        // if(l>r) return {};
        return hsh[r+1]-hsh[l]*pw[r-l+1];
    }

    void clear() {
        hsh.clear(),pw.clear();
        hsh.emplace_back(),pw.emplace_back(1,1);
    }
    
    HashArray(int sz=0) {
        hsh.reserve(sz),pw.reserve(sz);
        clear();
    }
};