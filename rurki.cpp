#include <iostream>
#include <cstdlib>

using namespace std;

class MacierzPodstawowa{
    static constexpr double wartosci[4][4]={
        {2/3,-1/6,-1/3,-1/6},
        {-1/6,2/3,-1/6,-1/3},
        {-1/3,-1/6,2/3,-1/6},
        {-1/6,-1/3,-1/6,2/3}};
public:
    double get(int wspX,int wspY){
        return wartosci[wspY][wspX];
    }
};

enum Pozycja{
    lewoGora=0,
    prawoGora=1,
    lewoDol=2,
    prawoDol=3
};

class NajmniejszyElement{
    int indeksyGlobalne[4];

public:
    NajmniejszyElement(){
        for(int i=0;i<4;i++){
            indeksyGlobalne[i]=-1;
        }
    }
    void set(int indexGlobalny,Pozycja poz){
        indeksyGlobalne[poz]=indexGlobalny;
    }
    int get(Pozycja poz){
        return indeksyGlobalne[poz];
    }
    void wypisz(){
        for(int i=0;i<4;i++){
            cout<<indeksyGlobalne[i]<<" ";
            if(1==i)cout<<endl;
        }
    }
};

class Fragment{
    int podzial;
    NajmniejszyElement** elem;


public:
    Fragment(int podzial):
        podzial(podzial){
        elem=new NajmniejszyElement* [podzial];
        for(int i=0;i<podzial;i++){
            elem[i]=new NajmniejszyElement[podzial];
        }
    }

    ~Fragment(){
        for(int i=0;i<podzial;i++){
            delete [] elem[i];
        }
        delete [] elem;
    }

    void zainicjuj(Pozycja fragm){
        int dlRzedu=podzial*2+1;
        if(lewoGora==fragm){
            for(int j=0;j<podzial;j++){
                for(int i=0;i<podzial;i++){
                    elem[i][j].set(i+1+dlRzedu*j,lewoGora);
                    elem[i][j].set(i+2+dlRzedu*j,prawoGora);
                    elem[i][j].set(i+1+dlRzedu*(j+1),lewoDol);
                    elem[i][j].set(i+2+dlRzedu*(j+1),prawoDol);
                }

            }
        }else if(prawoGora==fragm){
            for(int j=0;j<podzial;j++){
                for(int i=0;i<podzial;i++){
                    elem[i][j].set(podzial+i+1+dlRzedu*j,lewoGora);
                    elem[i][j].set(podzial+i+2+dlRzedu*j,prawoGora);
                    elem[i][j].set(podzial+i+1+dlRzedu*(j+1),lewoDol);
                    elem[i][j].set(podzial+i+2+dlRzedu*(j+1),prawoDol);
                }
            }
        }else throw exception();

    }
    void wypisz(){
         for(int j=0;j<podzial;j++){
            for(int i=0;i<podzial;i++){
                cout<<"element na pozycji ("<<i<<","<<j<<")"<<endl;
                elem[j][i].wypisz();
                cout<<endl;
            }
         }
    }
};

class Powierzchnia{
    Fragment *a,*b,*c;
    int podzial;
public:

    Powierzchnia(int podzial):
        podzial(podzial){
            a=new Fragment(podzial);
            a->zainicjuj(lewoGora);
    }
    void wypisz(){
        cout<<"fragment a"<<endl;
        a->wypisz();
    }
};

int main(int argc,char** argv){
    int liczbaPodzialow;
    if(argc<2){
        cin>>liczbaPodzialow;
    }else{
        string s=argv[1];
        liczbaPodzialow=atoi(s.c_str());
    }
    cout<<"liczbaPodzialow "<<liczbaPodzialow<<endl;
    Powierzchnia *p=new Powierzchnia(liczbaPodzialow);
    p->wypisz();


    return 0;
}
