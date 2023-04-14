template<typename T=int,T init=T()> struct Fenwick {
    int size=0;
    vector<T> tr;

    int lowbit(int x) { return x&(-x); }

    void update(T &aim,const T &val) { aim+=val; }

    void add(int pos,T val) {
        while(pos<=size) update(tr[pos],val),pos+=lowbit(pos);
    }

    T query(int pos) {
        T res=init;
        while(pos) update(res,tr[pos]),pos-=lowbit(pos);
        return res;
    }

    void resize(int n) {
        size=n;
        tr.resize(size+1,init);
    }

    Fenwick()=default;
    Fenwick(int n):size(n) { tr.resize(size+1,init); }
};