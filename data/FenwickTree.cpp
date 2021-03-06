template<typename T,std::size_t N> 
struct Fenwick {
    std::array<T,N> tr;

    int lowbit(int x) { return x&(-x); }

    void update(T &aim,T &val) {
        aim+=val;
    }

    void add(std::size_t pos,T val) {
        while(pos<=N) update(tr[pos],val),pos+=lowbit(pos);
    }

    T query(std::size_t pos) {
        T res=0;
        while(pos) update(res,tr[pos]),pos-=lowbit(pos);
        return res;
    }
};