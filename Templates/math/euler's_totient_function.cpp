constexpr int M=1e6+10;
int prime[M],euler[M],idx;
bool isnp[M];

void get_prime(int n=M-1) {
    isnp[1]=euler[1]=1;
    for(int i=2;i<=n;i++) {
        if(!isnp[i]) prime[++idx]=i,euler[i]=i-1;
        for(int j=1;prime[j]<=n/i;j++) {
            isnp[prime[j]*i]=true;
            if(i%prime[j]==0) {
                euler[i*prime[j]]=euler[i]*prime[j];
                break;
            }
            else euler[i*prime[j]]=euler[i]*(prime[j]-1);
        }
    }
}