#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc,char** argv){
    int liczbaPodzialow;
    if(argc<2){
        cin>>liczbaPodzialow;
    }else{
        string s=argv[1];
        liczbaPodzialow=atoi(s.c_str());
    }
    cout<<liczbaPodzialow;

    return 0;
}
