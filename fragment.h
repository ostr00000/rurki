#ifndef fragment_h
#define fragment_h

#include "NajmniejszyElement.h"
#include <iostream>

class Fragment{
    int podzial;
    NajmniejszyElement** elem;
    Pozycja fragm;
public:
    Fragment(int podzial,Pozycja fragm);
    ~Fragment();
    int getMaxIndex();
    void wypisz();
    void wypiszRzad(int n);
private:
    void zainicjuj();
};

#endif
