//把要处理的代码放在同目录下的in.txt里,结果会放在out.json里面
#include <fstream>
#include <iostream>
using namespace std;

int main() {
    constexpr bool interactive=false;

    ifstream fin("in.txt",ios::in);
    ofstream fout("out.json",ios::out);
    string in;
    char _='"';
    string tab="    ";

    if(interactive) {
        cout<<"snippet name"<<endl<<"> ";
        getline(cin,in);
    }

    fout<<_<<in<<_<<": {"<<endl;

    if(interactive) {
        cout<<"preifx"<<endl<<"> ";
        getline(cin,in);
    }

    fout<<tab<<_<<"prefix"<<_<<": "<<_<<in<<_<<","<<endl;

    fout<<tab<<_<<"body"<<_<<": ["<<endl;
    while(getline(fin,in)){
        fout<<tab<<tab<<_;
        for(auto x:in){
            if(x=='\\'||x==_) fout<<'\\';
            fout<<x;
        }
        fout<<_<<","<<endl;
    }
    fout<<tab<<"],"<<endl;

    in.clear();
    if(interactive) {
        cout<<"description"<<endl<<">";
        getline(cin,in);
    }

    fout<<tab<<_<<"description"<<_<<": "<<_<<in<<_<<","<<endl;
    fout<<"},"<<endl;
    return 0;
}