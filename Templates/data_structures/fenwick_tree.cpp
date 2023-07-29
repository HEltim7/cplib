template<typename T=int,T init=T()> struct Fenwick {
    using F=function<void(T&,const T&)>;
    F add;
    vector<T> tr;

    int lowbit(int x) { return x&(-x); }
    void resize(int n) { tr.resize(n+2,init); }

    void modify(int pos,T val) {
        if(++pos<=0) return;
        while(pos<tr.size()) add(tr[pos],val),pos+=lowbit(pos);
    }

    void reset(int pos) {
        if(++pos<=0) return;
        while(pos<tr.size()) tr[pos]=init,pos+=lowbit(pos);
    }

    T query(int pos) {
        if(++pos<0) return {};
        T res=init;
        while(pos) add(res,tr[pos]),pos-=lowbit(pos);
        return res;
    }
    
    explicit Fenwick(
        int n,F add=[](T &x,const T &y) { x += y; })
        : add(add) {
        resize(n);
    }
};