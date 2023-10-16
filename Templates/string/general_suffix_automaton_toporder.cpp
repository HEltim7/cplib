vector<int> toporder;
void toposort(string &s) {
    static int cid=0;
    static vector<int> col,vec;
    vector<int> cnt(s.size()+1);
    col.resize(size());
    vec.clear();
    cid++;

    int u=0;
    for(char x:s) {
        int c=x-B;
        u=edp[u].ch[c];
        for(int p=u;p&&col[p]!=cid;p=edp[p].link) {
            col[p]=cid;
            vec.emplace_back(p);
            cnt[edp[p].len]++;
        }
    }

    toporder.resize(vec.size());
    partial_sum(cnt.rbegin(),cnt.rend(),cnt.rbegin());
    for(int u:vec) toporder[--cnt[edp[u].len]]=u;
}