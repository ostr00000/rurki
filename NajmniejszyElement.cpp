#include "NajmniejszyElement.h"
using namespace std;

NajmniejszyElement::NajmniejszyElement():id(NajmniejszyElement::idGlobalne++){
    for(int i=0;i<4;i++){
        indeksyGlobalne[i]=-1;
    }
}

void NajmniejszyElement::set(int indexGlobalny,Pozycja poz){
    indeksyGlobalne[poz]=indexGlobalny;
}

int NajmniejszyElement::get(Pozycja poz){
    return indeksyGlobalne[poz];
}

int NajmniejszyElement::get(int poz){
    return indeksyGlobalne[poz];
}

void NajmniejszyElement::wypisz(){
    for(int i=0;i<4;i++){
        cout<<indeksyGlobalne[i]<<" ";
        if(1==i)cout<<endl;
    }
}


