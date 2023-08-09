template<class S=string> struct KMPAutomaton {
    using C=typename S::value_type;
    vector<int> link;
    S s;
    
    void extend(C c) {
        s.push_back(c);
        if(s.size()==1) {
            link.emplace_back(-1);
            return;
        }
        int i=s.size()-1,j=link[i-1];
        while(j!=-1&&s[i]!=s[j+1]) j=link[j];
        if(s[i]==s[j+1]) j++;
        link.emplace_back(j);
    }

    void append(const S &s) {
        for(C c:s) extend(c);
    }

    int count(const S &t) {
        if(t.size()<s.size()) return 0;
        int res=0;
        for(int i=0,j=-1;i<t.size();i++) {
            while(j!=-1&&t[i]!=s[j+1]) j=link[j];
            if(t[i]==s[j+1]) j++;
            if(j+1==s.size()) {
                res++;
                j=link[j];
            }
        }
        return res;
    }

    vector<int> match(const S &t) {
        if(t.size()<s.size()) return {};
        vector<int> res;
        for(int i=0,j=-1;i<t.size();i++) {
            while(j!=-1&&t[i]!=s[j+1]) j=link[j];
            if(t[i]==s[j+1]) j++;
            if(j+1==s.size()) {
                res.emplace_back(i);
                j=link[j];
            }
        }
        return res;
    }

    void clear() {
        s.clear();
        link.clear();
    }

    KMPAutomaton(const S &s=S{}) { append(s); }
};