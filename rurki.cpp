#include <iostream>
#include <cstdlib>
#include <Eigen/Dense>

#include "powierzchnia.h"
#include "NajmniejszyElement.h"
#include "fragment.h"
#include "pozycja.h"

using namespace Eigen;
using namespace std;

int NajmniejszyElement::idGlobalne=0;

const double wartosci[4][4]={
    {2/3,-1/6,-1/3,-1/6},
    {-1/6,2/3,-1/6,-1/3},
    {-1/3,-1/6,2/3,-1/6},
    {-1/6,-1/3,-1/6,2/3}};

void wypelnijMacierz(MatrixXd &matrix,Powierzchnia &pow){

    int x,y;
    for(int indexElem=1,maks=pow.getMaxIndex();indexElem<maks;indexElem++){
        NajmniejszyElement *e=pow.getElem(indexElem);
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                double wartosc=wartosci[i][j];
                matrix(e->get(i),e->get(j))=wartosc;
            }
        }
    }

}

int main(int argc,char** argv){
    int liczbaPodzialow;
    if(argc<2){
        cin>>liczbaPodzialow;
    }else{
        string s=argv[1];
        liczbaPodzialow=atoi(s.c_str());
    }
    cout<<"liczbaPodzialow "<<liczbaPodzialow<<endl;
    Powierzchnia pow(liczbaPodzialow);
    //p->wypisz();
    pow.wypiszPlansze();

    MatrixXd matrix=MatrixXd::Zero(pow.getMaxIndex(),pow.getMaxIndex());
    wypelnijMacierz(matrix,pow);
    cout<<matrix<<endl;
    //cout << MatrixXd::Zero(2,3) << endl;

    return 0;
}
