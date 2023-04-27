template<typename T,int R,int C=R> struct Matrix {
    array<array<T,C>,R> v;
    
    template<int Rr,int Cr> constexpr Matrix<T,R,Cr> 
    operator*(const Matrix<T,Rr,Cr> &r) const {
        static_assert(C==Rr,"");
        array<array<T,Cr>,R> ans;
        for(int i=0;i<R;i++) {
            for(int k=0;k<C;k++) {
                // if(v[i][k]==0) continue;
                for(int j=0;j<Cr;j++) {
                    ans[i][j]+=v[i][k]*r[k][j];
                }
            }
        }
        return ans;
    }

    constexpr Matrix operator+(const Matrix &r) const {
        array<array<T,C>,R> ans;
        for(int i=0;i<R;i++)
            for(int j=0;j<C;j++)
                ans[i][j]=v[i][j]+r[i][j];
        return ans;
    }

    constexpr Matrix operator-(const Matrix &r) const {
        array<array<T,C>,R> ans;
        for(int i=0;i<R;i++)
            for(int j=0;j<C;j++)
                ans[i][j]=v[i][j]-r[i][j];
        return ans;
    }

    constexpr Matrix &operator*=(const Matrix<T,C,C> &r) { return *this=*this*r; }
    constexpr Matrix &operator+=(const Matrix &r) { return *this=*this+r; }
    constexpr Matrix &operator-=(const Matrix &r) { return *this=*this-r; }

    constexpr Matrix pow(long long k) const {
        Matrix res(1),x=*this;
        while(k) { if(k&1) res*=x; k>>=1; x*=x; }
        return res;
    }

    constexpr auto &operator[](int idx) { return v[idx]; }
    constexpr auto &operator[](int idx) const { return v[idx]; }

    constexpr void clear() { v={}; }
    constexpr void unit(T x=1) {
        static_assert(R==C,"");
        clear(); for(int i=0;i<R;i++) v[i][i]=x;
    }

    constexpr Matrix() { clear(); }
    constexpr Matrix(T x) { unit(x); }
    constexpr Matrix(const array<array<T,C>,R> &x): v(x) {}
};