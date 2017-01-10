#include "powierzchnia.h"

using namespace std;

Powierzchnia::Powierzchnia(int podzial):
    podzial(podzial),
    a(podzial,lewoGora),
    b(podzial,prawoGora),
    c(podzial,prawoDol){
}

int Powierzchnia::getMaxIndex(){
    return c.getMaxIndex();
}

void Powierzchnia::addMap(int key,NajmniejszyElement *val){
    map.insert({key,val});
}

NajmniejszyElement *Powierzchnia::getElem(int key){
    return map[key];
}

void Powierzchnia::wypisz(){
    cout<<"fragment a"<<endl;
    a.wypisz();
    cout<<"fragment b"<<endl;
    b.wypisz();
    cout<<"fragment c"<<endl;
    c.wypisz();
    cout<<endl;
}

void Powierzchnia::wypiszPlansze(){
    cout<<endl;
    for(int i=0;i<=podzial;i++){
        a.wypiszRzad(i);
        b.wypiszRzad(i);
        cout<<endl;
    }
    for(int i=1;i<=podzial;i++){
        for(int j=0;j<podzial;j++){
            cout<<"   ";
        }
        c.wypiszRzad(i);
        cout<<endl;
    }
    cout<<endl;
}

