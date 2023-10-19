struct GeneralSuffixAutomaton {
    constexpr static int A=26;
    constexpr static char B='a';
    using Arr=array<int, A>;
    struct Endpos {
        int link,len;
        Arr ch;
    };
    vector<Endpos> edp;

    int new_edp() { edp.emplace_back(); return edp.size()-1; }

    int split(int p,int c,int len) {
        int q=edp[p].ch[c];
        if(edp[q].len==len) return q;
        else {
            int clone=new_edp();
            edp[clone]=edp[q];
            edp[clone].len=len;
            edp[q].link=clone;
            for(;p!=-1&&edp[p].ch[c]==q;p=edp[p].link)
                edp[p].ch[c]=clone;
            return clone;
        }
    }

    void extend(int p,int c) {
        int cur=edp[p].ch[c];
        edp[cur].len=edp[p].len+1;
        for(;p!=-1&&(edp[p].ch[c]==cur||!edp[p].ch[c]);p=edp[p].link)
            edp[p].ch[c]=cur;
        if(p!=-1) edp[cur].link=split(p, c, edp[p].len+1);
    }

    void insert(string &s) {
        int t=0,c=0;
        for(auto x:s) {
            c=x-B;
            if(!edp[t].ch[c]) edp[t].ch[c]=new_edp();
            t=edp[t].ch[c];
        }
    }

    void build() {
        queue<int> q;
        q.push(0);
        while(q.size()) {
            int p=q.front();
            q.pop();
            for(int c=0;c<A;c++) if(edp[p].ch[c]) 
                extend(p, c),q.push(edp[p].ch[c]);
        }
    }

    int size() { return edp.size(); }
    void clear() { edp.clear(),edp.emplace_back(-1); }
    
    GeneralSuffixAutomaton(int sz=0) { edp.reserve(sz),clear(); }
};