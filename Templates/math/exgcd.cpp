template<typename T> T exgcd(T a,T b,T &x,T &y) {
    if(!b) { x=1,y=0; return a; }
    T res=exgcd(b, a%b, x, y),t=x;
    x=y,y=t-(a/b)*y;
    return res;
}