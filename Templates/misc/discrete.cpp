sort(num.begin(),num.end());
num.erase(unique(num.begin(),num.end()),num.end());
for(int i=1;i<=n;i++) 
    arr[i]=lower_bound(num.begin(),num.end(),arr[i])-num.begin()+1;
