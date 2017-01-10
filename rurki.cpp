#include <iostream>
#include <cstdlib>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;


double polowa(double a,double b){
    return (a+b)/2;
}

double fun_g(double x,double y){
    return 0;
}
#define dwie3 (2.0/3)
#define minus1_6 (-1.0/6)
#define minus1_3 (-1.0/3)
const double wartosci[4][4]={
    {dwie3,minus1_6,minus1_3,minus1_6},
    {minus1_6,dwie3,minus1_6,minus1_3},
    {minus1_3,minus1_6,dwie3,minus1_6},
    {minus1_6,minus1_3,minus1_6,dwie3}};

class Plansza{
    struct Punkt{
        double x,y;
        int index;
        void set(int x2,int y2,int index2){
            x=x2;
            y=y2;
            index=index2;
        }
    };
    int dlugosc,podzial;
    Punkt **tablica;
    int max_index;
    double przyrost;
public:
    Plansza(int podzial):dlugosc(podzial*2+1),podzial(podzial),przyrost(1.0/podzial){
        tablica=new Punkt*[dlugosc];
        int index=0;
        for(int i=0;i<dlugosc;i++){
            tablica[i]=new Punkt[dlugosc];
            for(int j=0;j<dlugosc;j++){
                tablica[i][j].set(-1+j*przyrost,1-i*przyrost,index++);
            }
        }
        max_index=--index;
    }
    void wypisz(){
        for(int i=0;i<dlugosc;i++){
            for(int j=0;j<dlugosc;j++){
                cout<<"Punkt ("<<i<<","<<j<<") x=";
                cout.width(3);
                cout<<tablica[i][j].x<<", y=";
                cout.width(3);
                cout<<tablica[i][j].y<<", id=";
                cout.width(2);
                cout<<tablica[i][j].index<<" ";
            }
            cout<<endl;
        }
    }
    int getMaxIndex(){
        return max_index;
    }
    int wyznaczIndex(int i,int j,int wewnetrzny){
        switch(wewnetrzny){
        case 0:
            return tablica[i][j-1].index;
        case 1:
            return tablica[i][j].index;
        case 2:
            return tablica[i-1][j].index;
        case 3:
            return tablica[i-1][j-1].index;
        }
    }

    double wyznaczL(int i,int j){
        static double polowaPrzyrost=przyrost/2;

        double ret=0;

        //Punkt lewoGora=tablica[i-1][j-1];
        Punkt prawoGora=tablica[i-1][j];
        Punkt lewoDol=tablica[i][j-1];
        Punkt prawoDol=tablica[i][j];

        double wspMiedzyY=polowa(prawoGora.y,prawoDol.y);
        double wspMiedzyX=polowa(lewoDol.x,prawoDol.x);

        ///lewo
        ret+=fun_g(lewoDol.x,wspMiedzyY);

        ///prawo
        ret+=fun_g(prawoDol.x,wspMiedzyY);

        ///gora
        ret+=fun_g(wspMiedzyX,prawoGora.y);

        ///dol
        ret+=fun_g(wspMiedzyX,prawoDol.y);

        return ret*polowaPrzyrost;
    }

    void wypelnijMacierz(MatrixXd &m,VectorXd &v){
        int index=0;
        ///petle po fragmentach powierzchni
        for(int i=1;i<dlugosc;i++){
            for(int j=1;j<dlugosc;j++){
//ustawic zerowanie wiersza i jedynke na przekatnej
                ///jesli nieuzywana cwiartka
                if(i>=podzial and j<=podzial){
                    v(index++)=0;
                }else{
                    v(index++)=wyznaczL(i,j);
                }

                ///petle po funkcjach fi (albo psi)
                for(int a=0;a<4;a++){
                    int wsp_X=wyznaczIndex(i,j,a);
                    for(int b=0;b<4;b++){
                        int wsp_Y=wyznaczIndex(i,j,b);
                        double wartosc=wartosci[a][b];
                        m(wsp_X,wsp_Y)=wartosc;
                    }
                }
            }
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

    Plansza plansza(liczbaPodzialow);
    plansza.wypisz();

    VectorXd prawaStrona(plansza.getMaxIndex());
    MatrixXd matrix=MatrixXd::Zero( 1+plansza.getMaxIndex(), 1+plansza.getMaxIndex() );

    plansza.wypelnijMacierz(matrix,prawaStrona);

    cout<<matrix<<endl;
    //cout << MatrixXd::Zero(2,3) << endl;

    return 0;
}
