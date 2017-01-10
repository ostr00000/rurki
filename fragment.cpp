
#include "fragment.h"
using namespace std;

Fragment::Fragment(int podzial,Pozycja fragm):
    podzial(podzial),
    fragm(fragm){
    elem=new NajmniejszyElement* [podzial];
    for(int i=0;i<podzial;i++){
        elem[i]=new NajmniejszyElement[podzial];

    }
    Fragment::zainicjuj();
}

Fragment::~Fragment(){
    for(int i=0;i<podzial;i++){
        delete [] elem[i];
    }
    delete [] elem;
}

int Fragment::getMaxIndex(){
    return elem[podzial-1][podzial-1].get(prawoDol);
}

void Fragment::wypisz(){
     for(int j=0;j<podzial;j++){
        for(int i=0;i<podzial;i++){
            cout<<"element na pozycji ("<<i<<","<<j<<")"<<endl;
            elem[j][i].wypisz();
            cout<<endl;
        }
     }
}
void Fragment::wypiszRzad(int n){
    if(n!=podzial){
        for(int i=0;i<podzial;i++){
            cout.width(2);
            cout<<elem[i][n].get(lewoGora)<<" ";
        }
        if(prawoGora==fragm or prawoDol==fragm){
            cout.width(2);
            cout<<elem[podzial-1][n].get(prawoGora);
        }
    }else{
        n--;
        for(int i=0;i<podzial;i++){
            cout.width(2);
            cout<<elem[i][n].get(lewoDol)<<" ";
        }
        if(prawoGora==fragm or prawoDol==fragm){
            cout.width(2);
            cout<<elem[podzial-1][n].get(prawoDol);
        }
    }
}

void Fragment::zainicjuj(){
    if(lewoGora==fragm){
        int dlRzedu=podzial*2+1;
        for(int j=0;j<podzial;j++){
            for(int i=0;i<podzial;i++){
                elem[i][j].set(i+1+dlRzedu*j,lewoGora);
                elem[i][j].set(i+2+dlRzedu*j,prawoGora);
                elem[i][j].set(i+1+dlRzedu*(j+1),lewoDol);
                elem[i][j].set(i+2+dlRzedu*(j+1),prawoDol);
            }

        }
    }else if(prawoGora==fragm){
        int dlRzedu=podzial*2+1;
        for(int j=0;j<podzial;j++){
            for(int i=0;i<podzial;i++){
                elem[i][j].set(podzial+i+1+dlRzedu*j,lewoGora);
                elem[i][j].set(podzial+i+2+dlRzedu*j,prawoGora);
                elem[i][j].set(podzial+i+1+dlRzedu*(j+1),lewoDol);
                elem[i][j].set(podzial+i+2+dlRzedu*(j+1),prawoDol);
            }
        }
    }else if(prawoDol){
        int przesuniecieStartowe=(2*podzial+1)*podzial+podzial;
        for(int j=0;j<podzial;j++){
            for(int i=0;i<podzial;i++){
                elem[i][j].set(przesuniecieStartowe+i+1+(podzial+1)*j,lewoGora);
                elem[i][j].set(przesuniecieStartowe+i+2+(podzial+1)*j,prawoGora);
                elem[i][j].set(przesuniecieStartowe+i+1+(podzial+1)*(j+1),lewoDol);
                elem[i][j].set(przesuniecieStartowe+i+2+(podzial+1)*(j+1),prawoDol);
            }
        }
    }else throw exception();

}

