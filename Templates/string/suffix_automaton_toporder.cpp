vector<int> toporder;
void toposort() {
    vector<int> cnt(size());
    toporder.resize(size()-1);
    for(int i=1;i<size();i++) cnt[edp[i].len]++;
    partial_sum(cnt.rbegin(),cnt.rend(),cnt.rbegin());
    for(int i=1;i<size();i++) toporder[--cnt[edp[i].len]]=i;
}