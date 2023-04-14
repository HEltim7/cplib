constexpr int HASHCNT=2;
array<int,HASHCNT> mod;
template<int size,typename I=int,typename L=long long,const array<I,size> &p=mod> struct Hashint {
    array<I,size> v;
    I _pow(int i,L b) const {
        L res=1,a=v[i];
        while(b) { if(b&1) res=res*a%p[i]; b>>=1; a=a*a%p[i]; }
        return res;
    }
    I _inv(int i) const { return _pow(i,p[i]-2); }
    Hashint pow(L b) {
        Hashint res;
        for(int i=0;i<size;i++) res[i]=_pow(i,b);
        return res;
    }

    Hashint &operator+=(const Hashint &x) { for(int i=0;i<size;i++) v[i]+=x[i],v[i]-=v[i]>=p[i]?p[i]:0; return *this; }
    Hashint &operator-=(const Hashint &x) { for(int i=0;i<size;i++) v[i]-=x[i],v[i]+=v[i]<0?p[i]:0; return *this; }
    Hashint &operator*=(const Hashint &x) { for(int i=0;i<size;i++) v[i]=L(1)*v[i]*x[i]%p[i]; return *this; }
    Hashint &operator/=(const Hashint &x) { for(int i=0;i<size;i++) v[i]=L(1)*v[i]*x._inv(i)%p[i]; return *this; }

    friend Hashint operator+(Hashint l,const Hashint &r) { return l+=r; }
    friend Hashint operator-(Hashint l,const Hashint &r) { return l-=r; }
    friend Hashint operator*(Hashint l,const Hashint &r) { return l*=r; }
    friend Hashint operator/(Hashint l,const Hashint &r) { return l/=r; }
    
    Hashint operator++(int) { auto res=*this; *this+=1; return res; }
    Hashint operator--(int) { auto res=*this; *this-=1; return res; }
    Hashint operator-  () { return *this*-1; }
    Hashint &operator++() { return *this+=1; }
    Hashint &operator--() { return *this-=1; }

    bool operator< (const Hashint &x) const { return v< x.v; }
    bool operator> (const Hashint &x) const { return v> x.v; }
    bool operator<=(const Hashint &x) const { return v<=x.v; }
    bool operator>=(const Hashint &x) const { return v>=x.v; }
    bool operator==(const Hashint &x) const { return v==x.v; }
    bool operator!=(const Hashint &x) const { return v!=x.v; }

    auto &operator[](int i) { return v[i]; }
    auto &operator[](int i) const { return v[i]; }

    Hashint(L x=0) { for(int i=0;i<size;i++) v[i]=(x%p[i]+p[i])%p[i]; }
}; using Hint=Hashint<HASHCNT>;