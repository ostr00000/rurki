#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <Eigen/Dense>
#include <Eigen/LU>

#define comp 1

using namespace Eigen;
using namespace std;

#define dwie3 (2.0/3)
#define minus1_6 (-1.0/6)
#define minus1_3 (-1.0/3)
const double wartosci[4][4]={
    {dwie3,minus1_6,minus1_3,minus1_6},
    {minus1_6,dwie3,minus1_6,minus1_3},
    {minus1_3,minus1_6,dwie3,minus1_6},
    {minus1_6,minus1_3,minus1_6,dwie3}};


double polowa(double a,double b){
    return (a+b)/2;
}


double fun_przyklad(double x,double y){
    return pow(x*x,1./3);

}

double fun_g(double x,double y){
    ///sprawdzenie czy (x,y) lezy na brzegu dirichleta
    static const double epsilon=1e-10;
    /*if((abs(x)<epsilon and y<0)or (abs(y)<epsilon and x<0) or (abs(x)<epsilon and abs(y)<epsilon)){
        return 0;
    }*/
    //return x*y;
    return fun_przyklad(x,y);
}


class Plansza{
    struct Punkt{
        double x,y;
        int index;
        void set(double x2,double y2,int index2){
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
            double y=1-i*przyrost;
            for(int j=0;j<dlugosc;j++){
                tablica[i][j].set(-1+j*przyrost,y,index++);
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

        Punkt punkt=tablica[i][j];
        double ret=0;

        ///gora
        if((i!=0 and i!=dlugosc-1) or j==dlugosc-1){
            ret+=fun_g(punkt.x,punkt.y+polowaPrzyrost);
        }
        ///dol
        if((i!=dlugosc-1 and i!=0) or j==0 or j==dlugosc-1 ){
            ret+=fun_g(punkt.x,punkt.y-polowaPrzyrost);
        }
        ///lewo
        if((j!=0 and j!=dlugosc-1) or i==0 or i==dlugosc-1){
            ret+=fun_g(punkt.x-polowaPrzyrost,punkt.y);
        }
        ///prawo
        if((j!=dlugosc-1 and j!=0) or i==0){
            ret+=fun_g(punkt.x+polowaPrzyrost,punkt.y);
        }

        return ret*polowaPrzyrost;
    }

    void wypelnijMacierz(MatrixXd &m,VectorXd &v){
        int index=0;
        ///petle po fragmentach powierzchni
        for(int i=0;i<dlugosc;i++){
            for(int j=0;j<dlugosc;j++){
                //if(i>1 or j>1)continue; //test

                ///jesli nieuzywana cwiartka
                if((i>=podzial and j<=podzial) or (i!=0 and j!=0 and i!=dlugosc-1 and j!=dlugosc-1)){
                    v(index++)=0;
                }else{
                    v(index++)=wyznaczL(i,j);
                }

                ///dalej sa sprawdzanie tylko kwadraty
                if(i==0 or j==0)continue;

                ///petle po funkcjach fi (albo psi)
                for(int a=0;a<4;a++){
                    int wsp_X=wyznaczIndex(i,j,a);
                    for(int b=0;b<4;b++){
                        int wsp_Y=wyznaczIndex(i,j,b);
                        double wartosc=wartosci[a][b];
                        m(wsp_X,wsp_Y)=+wartosc;
                    }
                }
            }
        }
        ///zeruje punkty na brzegu (i nie tylko - te dalej tez)
        for(int i=podzial;i<dlugosc;i++){
            for(int j=0;j<=podzial;j++){
                index=tablica[i][j].index;
                for(int a=0;a<=getMaxIndex();a++){
                    m(index,a)=0;
                }
                m(index,index)=1;
            }
        }
    }

    void zapisz(MatrixXd &matrix){
        fstream plik;
        plik.open("dane.txt",ios::out);
        int index=0;
        for(int i=0;i<dlugosc;i++){
            for(int j=0;j<dlugosc;j++){
                plik<<tablica[i][j].x<<" "
                    <<tablica[i][j].y<<" "
                    <<matrix(index++,0)<<endl;
            }
            plik<<endl;
        }
        plik.close();
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
    #if comp==1
    cout<<"liczbaPodzialow "<<liczbaPodzialow<<endl;
    #endif // comp

    Plansza plansza(liczbaPodzialow);

    #if comp==1
    plansza.wypisz();
    #endif // comp

    VectorXd prawaStrona(1+plansza.getMaxIndex());
    MatrixXd matrix=MatrixXd::Zero( 1+plansza.getMaxIndex(), 1+plansza.getMaxIndex() );

    plansza.wypelnijMacierz(matrix,prawaStrona);

    #if comp==1
    cout<<matrix<<endl<<endl;
    cout<<prawaStrona<<endl<<endl;
    #endif // comp

    MatrixXd wynik=matrix.inverse()*prawaStrona;

    #if comp==1
    cout<<matrix.inverse()<<endl<<endl;
    cout<<wynik<<endl<<endl;
    #endif // comp
    plansza.zapisz(wynik);

    char* polecenie="gnuplot -p -e \"splot 'dane.txt' with pm3d";
    system(polecenie);

    return 0;
}
