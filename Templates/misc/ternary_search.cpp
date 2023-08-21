template<typename I,class F>
I ternary_search_min(I l,I r,F f) {
    while(l<r) {
        I lmid=l+(r-l)/2;
        I rmid=lmid+1;
        if(f(lmid)<f(rmid)) r=rmid-1;
        else l=lmid+1;
    }
    return l;
};

template<typename I,class F>
I ternary_search_max(I l,I r,F f) {
    while(l<r) {
        I lmid=l+(r-l)/2;
        I rmid=lmid+1;
        if(f(lmid)<f(rmid)) l=lmid+1;
        else r=rmid-1;
    }
    return l;
};