namespace cdq {
    constexpr int N=1e5+10; // ***
    struct Point {
        
        bool operator<(const Point &p) const {
            
        }
    } p[N],tmp[N],bak[N];

    void solve(const int L,const int R) {
        if(L==R) return;
        int mid=L+R>>1;
        solve(L,mid),solve(mid+1,R);

        int i=L,j=mid+1,idx=L;
        while(j<=R) {
            while(i<=mid&&true) {
                // TODO 双指针更新i
                tmp[idx++]=p[i++];
            }
            // TODO 更新答案
            tmp[idx++]=p[j++];
        }
        for(int k=L;k<i;k++) ; // TODO reset状态
        while(i<=mid) tmp[idx++]=p[i++];
        for(int i=L;i<=R;i++) p[i]=tmp[i];
    }

} using cdq::p,cdq::bak;