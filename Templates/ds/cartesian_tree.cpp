template<typename T=int> struct CartesianTree {
	vector<int> lch,rch,stk;
	vector<T> val;
	int root,idx;

	void extend(int x) {
		idx++;
		lch.emplace_back(0);
		rch.emplace_back(0);
		val.emplace_back(x);
		
		while(stk.size()&&val[stk.back()]>x) {
			lch[idx]=stk.back();
			stk.pop_back();
		}
		if(stk.size()) rch[stk.back()]=idx;
		else root=idx;
		stk.emplace_back(idx);
	}

	void clear() {
		root=idx=0;
		lch.assign(1,{});
		rch.assign(1,{});
		val.assign(1,{});
		stk.clear();
	}

	CartesianTree(int sz=0) {
		lch.reserve(sz+1);
		rch.reserve(sz+1);
		val.reserve(sz+1);
		stk.reserve(sz+1);
		clear();
	}
};