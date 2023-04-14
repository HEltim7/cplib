constexpr int N=1e5+10;
Mint faet[N],infaet[N];

void init() {
    faet[1]=1,faet[0]=1;
    infaet[1]=1,infaet[0]=1;
    for(int i=2;i<N;i++){
        faet[i]=faet[i-1]*i;
        infaet[i]=infaet[i-1]/i;
    }
}

Mint cmb(int a,int b) {
    if(a<0||b<0||a<b) return 0;
    return faet[a]*infaet[a-b]*infaet[b];
}