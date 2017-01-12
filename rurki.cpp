#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <Eigen/Dense>
#include <Eigen/LU>

#define comp 0 ///z wypisywaniem czy bez (1 tylko dla malych podzialow)

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

double fun_przyklad(double x,double y){
    return pow(x*x,1./3);
}

double fun_g(double x,double y){
    //static const double epsilon=1e-10;
    //return 33;
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
    int dlugosc;///wielkosc tablicy punktow
    int podzial;///na ile czesci jest dzielony jeden fragment
    Punkt **tablica;
    int max_index;///kazdy punkt ma przypisany index globalny, max_index=dlugosc*dlugos-1
    double przyrost;///odleglosc miedzy sasiednimi punktami
    double polowaPrzyrost;

public:
    Plansza(int podzial):
        dlugosc(podzial*2+1),
        podzial(podzial),
        przyrost(1.0/podzial),
        polowaPrzyrost(przyrost/2){
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

    ///Wypelnia macierz B i vektor prawej strony rownania L
    void wypelnijMacierz(MatrixXd &m,VectorXd &v){
        int index=0;

        ///petle po fragmentach powierzchni
        for(int i=0;i<dlugosc;i++){
            for(int j=0;j<dlugosc;j++){
                v(index++)=wypelnijPrawaStrone(i,j);

                ///dalej sa sprawdzanie tylko kwadraty
                if(i==0 or j==0)continue;

                ///petle po funkcjach fi (albo psi)
                for(int a=0;a<4;a++){
                    int wsp_X=wyznaczIndex(i,j,a);
                    for(int b=0;b<4;b++){
                        int wsp_Y=wyznaczIndex(i,j,b);
                        double wartosc=wartosci[a][b];
                        m(wsp_X,wsp_Y)+=wartosc;
                    }
                }
            }
        }

        ///zeruje punkty na brzegu (i nie tylko - te dalej tez), a na przekatna daje 1
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

    ///zapisuje macierz do pliku w formacie potrzebnym do odczytania przez gnuplot
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

    ///wypisuje na ekran - do testow
    void wypisz(){
        cout<<"Plansza: "<<endl;
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
        cout<<endl<<endl;
    }

    int getMaxIndex(){
        return max_index;
    }

private:
    /// i!=0,j!=0 - wspolrzedne punktu w tablicy, wewnetrzny - w ktore miejsce ma odwzorowac
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
        default:
            throw exception();
        }
    }
    ///zwraca wartosc dla punktow "obok" podanego punktu
    double gora(Punkt punkt){
        return fun_g(punkt.x,punkt.y+polowaPrzyrost);
    }
    double dol(Punkt punkt){
        return fun_g(punkt.x,punkt.y-polowaPrzyrost);
    }
    double lewo(Punkt punkt){
        return fun_g(punkt.x-polowaPrzyrost,punkt.y);
    }
    double prawo(Punkt punkt){
        return fun_g(punkt.x+polowaPrzyrost,punkt.y);
    }

    ///zwraca wartosc w macierzy po prawej stronie rownania dla punktu o indeksach lokalnych i,j
    double wypelnijPrawaStrone(int i,int j){
        double ret=0;///gdy nie jest na brzegu ma wartosc 0
        Punkt punkt=tablica[i][j];
        /*if(i>=podzial and j<=podzial){
            return 0;
        }else if(i==0 and j==0){
            ret+=dol(punkt);
            ret+=prawo(punkt);
        }else if(i==0 and j==dlugosc-1){
            ret+=lewo(punkt);
            ret+=dol(punkt);
        }else if(i==dlugosc-1 and j==dlugosc-1){
            ret+=lewo(punkt);
            ret+=gora(punkt);
        }else if(i==0 or i==dlugosc-1){
            ret+=lewo(punkt);
            ret+=prawo(punkt);
        }else if(j==0 or j==dlugosc-1){
            ret+=gora(punkt);
            ret+=dol(punkt);
        }else ret=0;
        */
        ///punkty na brzegu dirichleta lub nieistotne
        if(i>=podzial and j<=podzial){
            return 0;
        }

        ///sprawdzam gore (w lewym kacie, w prawym kacie, w srodku)
        if(i==0){
            if(j==0){
                ret+=dol(punkt);
                ret+=prawo(punkt);
            }else if(j==dlugosc-1){
                ret+=lewo(punkt);
                ret+=dol(punkt);
            }else{
                ret+=lewo(punkt);
                ret+=prawo(punkt);
            }
        }else

        ///sprawdzam dol (w lewym kacie, w prawym kacie, w srodku)
        if(i==dlugosc-1){
            if(j==0){
                ret+=gora(punkt);
                ret+=prawo(punkt);
            }else if(j==dlugosc-1){
                ret+=lewo(punkt);
                ret+=gora(punkt);
            }else{
                ret+=lewo(punkt);
                ret+=prawo(punkt);
            }
        }else
        ///sprawdzam czy sa na prawym albo lewym brzegu
        if(j==0 or j==dlugosc-1){
            ret+=gora(punkt);
            ret+=dol(punkt);
        }

        ///zgodnie ze wzorem = (g(A)+g(B))*1/2*|krawedz|
        return ret*polowaPrzyrost;
    }
};

int main(int argc,char** argv){
    int liczbaPodzialow;
    if(argc<2){
        cin>>liczbaPodzialow;
    }else{
        liczbaPodzialow=atoi(argv[1]);
    }

    #if comp==1
    cout<<"liczbaPodzialow "<<liczbaPodzialow<<endl<<endl;
    #endif // comp

    Plansza plansza(liczbaPodzialow);

    #if comp==1
    plansza.wypisz();
    #endif // comp

    VectorXd prawaStrona(1+plansza.getMaxIndex());
    MatrixXd matrix=MatrixXd::Zero( 1+plansza.getMaxIndex(), 1+plansza.getMaxIndex() );

    plansza.wypelnijMacierz(matrix,prawaStrona);

    #if comp==1
    cout<<"macierz B:"<<endl<<matrix<<endl<<endl;
    cout<<"macierz L:"<<endl<<prawaStrona<<endl<<endl;
    #endif // comp

    MatrixXd wynik=matrix.inverse()*prawaStrona;

    #if comp==1
    cout<<"macierz odwrotna do macierzy B:"<<endl<<matrix.inverse()<<endl<<endl;
    cout<<"obliczona macierz:"<<endl<<wynik<<endl<<endl;
    #endif // comp

    plansza.zapisz(wynik);

    char* polecenie="gnuplot -p -e \"splot 'dane.txt' with pm3d\"";
    system(polecenie);

    return 0;
}
