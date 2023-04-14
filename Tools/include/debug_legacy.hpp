#define __CONCAT_IMPL(args1,args2) args1 ## args2
#define CONCAT(args1,args2) __CONCAT_IMPL(args1,args2)

#define GET_ARGS_ELEM(IDX, ...) GET_ARGS_ELEM_ ## IDX(__VA_ARGS__)
#define GET_ARGS_ELEM_1(a1, ...) a1
#define GET_ARGS_ELEM_2(a1, a2, ...) a2
#define GET_ARGS_ELEM_3(a1, a2, a3, ...) a3
#define GET_ARGS_ELEM_4(a1, a2, a3, a4, ...) a4
#define GET_ARGS_ELEM_5(a1, a2, a3, a4, a5, ...) a5
#define GET_ARGS_ELEM_6(a1, a2, a3, a4, a5, a6, ...) a6
#define GET_ARGS_ELEM_7(a1, a2, a3, a4, a5, a6, a7, ...) a7
#define GET_ARGS_ELEM_8(a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define GET_ARGS_ELEM_9(a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9

#define GET_ARGS_ELEM_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10
#define GET_ARGS_ELEM_11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#define GET_ARGS_ELEM_12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, ...) a12
#define GET_ARGS_ELEM_13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, ...) a13
#define GET_ARGS_ELEM_14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14
#define GET_ARGS_ELEM_15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, ...) a15
#define GET_ARGS_ELEM_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, ...) a16

#define GET_ARGS_SIZE(...) GET_ARGS_ELEM(16 __VA_OPT__(,__VA_ARGS__) ,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define __pre std::cout << "[debug] " 
#define __cat(x) << #x << " = " << x << " | "
#define __cat_end(x) << #x << " = " << x << '\n' <<std::flush 

#define cat(...) CONCAT(__debug, GET_ARGS_SIZE(__VA_ARGS__))(__VA_ARGS__)
#define __debug0()
#define __debug1(a) __pre __cat_end(a)
#define __debug2(a,b) __pre __cat(a) __cat_end(b)
#define __debug3(a,b,c) __pre __cat(a) __cat(b) __cat_end(c)
#define __debug4(a,b,c,d) __pre __cat(a) __cat(b) __cat(c) __cat_end(d)
#define __debug5(a,b,c,d,e) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat_end(e)
#define __debug6(a,b,c,d,e,f) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat_end(f)
#define __debug7(a,b,c,d,e,f,g) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat_end(g)
#define __debug8(a,b,c,d,e,f,g,h) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat_end(h)
#define __debug9(a,b,c,d,e,f,g,h,i) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat_end(i)

#define __debug10(a,b,c,d,e,f,g,h,i,j) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat_end(j)
#define __debug11(a,b,c,d,e,f,g,h,i,j,k) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat(j) __cat_end(k)
#define __debug12(a,b,c,d,e,f,g,h,i,j,k,l) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat(j) __cat(k) __cat_end(l)
#define __debug13(a,b,c,d,e,f,g,h,i,j,k,l,m) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat(j) __cat(k) __cat(l) __cat_end(m)
#define __debug14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat(j) __cat(k) __cat(l) __cat(m) __cat_end(n)
#define __debug15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) __pre __cat(a) __cat(b) __cat(c) __cat(d) __cat(e) __cat(f) __cat(g) __cat(h) __cat(i) __cat(j) __cat(k) __cat(l) __cat(m) __cat(n) __cat_end(o)

#define cat_arr(v,...) __cat_arr(v,#v __VA_OPT__(,__VA_ARGS__));

#include<vector>
#include<iostream>
using namespace std;

template <typename T> inline void 
    __cat_arr(vector<T> &v,string name,int l=0,int r=-1){
        std::cout<<"[debug] "<<name;
        if(r==-1) r=v.size()-1;
        else cout<<'['<<l<<"->"<<r<<']';
        cout<<" = [";

        int add=1;
        if(l>r) add=-1;
        for(int i=l;i!=r+add;i+=add) std::cout<<v[i]<<",]"[i==r];
        cout<<'\n'<<std::flush;
    }

template <typename T,typename U> inline void
    __cat_arr(vector<pair<T,U>> &v,string name,int l=0,int r=-1){
        std::cout<<"[debug] "<<name;
        if(r==-1) r=v.size()-1;
        else cout<<'['<<l<<"->"<<r<<']';
        cout<<" = ";

        int add=1;
        if(l>r) add=-1;
        for(int i=l;i!=r+add;i+=add) std::cout<<'['<<v[i].first<<' '<<v[i].second<<']';
        cout<<'\n'<<std::flush;
    }

template <typename T> inline void
    __cat_arr(vector<vector<T>> &v,string name,int l=0,int r=-1){
        std::cout<<"[debug] "<<name;
        if(r==-1) r=v.size()-1;
        else cout<<'['<<l<<"->"<<r<<']';
        cout<<" = "<<'\n'<<std::flush;

        int add=1;
        if(l>r) add=-1;
        for(int i=l;i!=r+add;i+=add) {
            cout<<name<<'['<<i<<']'<<"=>";
            cat_arr(v[i]);
        }
        cout<<'\n'<<std::flush;
    }

template <typename T> inline void
    __cat_arr(T arr[],string name,int l=0,int r=-1){
        std::cout<<"[debug] "<<name;
        cout<<'['<<l<<"->"<<r<<']';
        if(r==-1) return;
        cout<<" = [";

        int add=1;
        if(l>r) add=-1;
        for(int i=l;i!=r+add;i+=add) cout<<arr[i]<<",]"[i==r];
        cout<<'\n'<<std::flush;
    }