struct HashArray {
    constexpr static int base=131;
    vector<Hint> arr,pw;

    void push_back(int x) {
        arr.push_back(arr.back()*base+x);
        pw.push_back(pw.back()*base);
    }

    void append(string &s) { for(auto x:s) push_back(x); }
    void append(vector<int> &s) { for(auto x:s) push_back(x); }

    Hint query(int l,int r) {
        return arr[r]-arr[l-1]*pw[r-l+1];
    }

    void clear() { arr.clear(),pw.clear();arr.push_back(0),pw.push_back(1); }
    
    HashArray() { clear(); };
    HashArray(int sz) {
        clear();
        arr.reserve(sz),pw.reserve(sz);
    };
};