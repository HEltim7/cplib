vector<int> toporder;
void toposort() {
    auto &q=toporder;
    q.clear();
    q.reserve(size());
    vector<int> ind(size());
    for(int i=1;i<size();i++) ind[edp[i].link]++;
    for(int i=1;i<size();i++) if(!ind[i]) q.push_back(i);
    for(int i=0;i<q.size();i++) {
        int u=q[i];
        int p=edp[u].link;
        if(p&&!--ind[p]) q.push_back(p);
    }
}