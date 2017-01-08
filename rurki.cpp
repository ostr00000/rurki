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
    Pozycja fragm;
public:
    Fragment(int podzial,Pozycja fragm):
        podzial(podzial),
        fragm(fragm){
        elem=new NajmniejszyElement* [podzial];
        for(int i=0;i<podzial;i++){
            elem[i]=new NajmniejszyElement[podzial];
        }
        zainicjuj();
    }

    ~Fragment(){
        for(int i=0;i<podzial;i++){
            delete [] elem[i];
        }
        delete [] elem;
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
    void wypiszRzad(int n){
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
private:
    void zainicjuj(){
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
};

class Powierzchnia{
    Fragment *a,*b,*c;
    int podzial;
public:

    Powierzchnia(int podzial):
        podzial(podzial){
            a=new Fragment(podzial,lewoGora);
            b=new Fragment(podzial,prawoGora);
            c=new Fragment(podzial,prawoDol);
    }
    void wypisz(){
        cout<<"fragment a"<<endl;
        a->wypisz();
        cout<<"fragment b"<<endl;
        b->wypisz();
        cout<<"fragment c"<<endl;
        c->wypisz();
        cout<<endl;
    }
    void wypiszPlansze(){
        cout<<endl;
        for(int i=0;i<=podzial;i++){
            a->wypiszRzad(i);
            b->wypiszRzad(i);
            cout<<endl;
        }
        for(int i=1;i<=podzial;i++){
            for(int j=0;j<podzial;j++){
                cout<<"   ";
            }
            c->wypiszRzad(i);
            cout<<endl;
        }
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
    //p->wypisz();
    p->wypiszPlansze();


    return 0;
}
