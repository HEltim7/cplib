string minimize(const string &s) {
    int i=0,j=1,k=0,n=s.size();
    while(i<n&&j<n&&k<n) {
        if(s[(i+k)%n]==s[(j+k)%n]) k++;
        else {
            if(s[(i+k)%n]>s[(j+k)%n]) i=i+k+1;
            else j=j+k+1;
            if(i==j) i++;
            k=0;
        }
    }
    i=min(i,j);
    string res;
    for(int j=0;j<n;j++) res.push_back(s[(i+j)%n]);
    return res;
}