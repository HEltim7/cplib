void build(const S &s) {
    this->s=s;
    link.resize(s.size());
    link[0]=-1;
    for(int i=1,j=-1;i<s.size();i++) {
        while(j!=-1&&s[i]!=s[j+1]) j=link[j];
        if(s[i]==s[j+1]) j++;
        link[i]=(i+1<s.size()&&j!=-1&&s[i+1]==s[j+1])?link[j]:j;
    }
}