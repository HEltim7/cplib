vector<int> toporder;
void toposort(string &s) {
    auto &q=toporder;
    static int cid=0;
    static vector<int> col,ind,vec;
    col.resize(size()),ind.resize(size());
    vec.clear(),q.clear();
    cid++;

    int u=0;
    for(char x:s) {
        int c=x-B;
        u=edp[u].ch[c];
        for(int p=u;p&&col[p]!=cid;p=edp[p].link) {
            col[p]=cid;
            vec.emplace_back(p);
            ind[edp[p].link]++;
        }
    }

    for(int u:vec) if(!ind[u]) q.emplace_back(u);
    for(int u:q) {
        int p=edp[u].link;
        if(p&&!--ind[p]) q.emplace_back(p);
    }
}