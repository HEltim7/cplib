LL dp(int n) {
    LL res=0,pre=0;
    vector<pair<LL,LL>> q(1);
    for(int i=1,idx=0;i<=n;i++) {
        
        // assert(idx<q.size());
        auto [x,y]=q[idx];
        LL k=0.0;
        LL b=y-k*x;
        while(idx+1<q.size()) {
            auto [x,y]=q[idx+1];
            if(y-k*x<=b) {
                b=y-k*x;
                idx++;
            }
            else break;
        }
        
        res=0.0;
        x=0.0;
        y=0.0;
    
        while(q.size()>=2) {
            auto [xl,yl]=q[q.size()-2];
            auto [xr,yr]=q[q.size()-1];
            if((y-yr)*(x-xl)<=(y-yl)*(x-xr)) q.pop_back();
            else break;
        }
        q.emplace_back(x,y);
    }
    return res;
}