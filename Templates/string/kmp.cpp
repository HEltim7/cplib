void kmp_build(int n) {
    for(int i=2,j=0;i<=n;i++) {
        while(j&&t[i]!=t[j+1]) j=link[j];
        if(t[i]==t[j+1]) j++;
        link[i]=t[i+1]==t[j+1]?link[j]:j; 
    }
}