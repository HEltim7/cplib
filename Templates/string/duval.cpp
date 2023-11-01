vector<string> duval(const string &s) {
    vector<string> res;
    for(int i=0;i<s.size();) {
        int j=i,k=i+1;
        while(k<s.size()&&s[j]<=s[k]) {
            if(s[j]<s[k]) j=i;
            else j++;
            k++;
        }
        while(i<=j) {
            res.emplace_back(s.substr(i,k-j));
            i+=k-j;
        }
    }
    return res;
}