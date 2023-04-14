constexpr int M=1e6+10;
int prime[M],idx;
bool isnp[M];

void get_prime(int n=M-1) {
    isnp[1]=1;
    for(int i=2;i<=n;i++) {
        if(!isnp[i]) prime[++idx]=i;
        for(int j=1;prime[j]<=n/i;j++) {
            isnp[prime[j]*i]=true;
            if(i%prime[j]==0) break;
        }
    }
}