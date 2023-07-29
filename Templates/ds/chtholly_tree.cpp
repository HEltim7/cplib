struct ChthollyTree {
    struct Node {
        int l,r,v;
        Node(int L,int R,int V) : l(L),r(R),v(V) {}
        bool operator< (const Node &x) const {
            return l<x.l;
        }
    };
    set<Node> st;

    auto split(int pos){
        auto it=st.lower_bound(Node(pos,pos,0));
        if(it!=st.end()&&it->l==pos) return it;
        it=prev(it);
        auto [l,r,v]=*it;
        st.erase(it);
        st.insert(Node(l,pos-1,v));
        return st.insert(Node(pos,r,v)).first;
    }

    void assign(int l,int r,int v){
        auto end=split(r+1),begin=split(l);
        st.erase(begin,end);
        st.insert(Node(l,r,v));
    }
} odt;