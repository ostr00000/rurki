#ifndef najmniejszyElement_h
#define najmniejszyElement_h

#include "pozycja.h"
#include <iostream>

class NajmniejszyElement{
    int indeksyGlobalne[4];
    int id;
public:
    static int idGlobalne;
    NajmniejszyElement();
    void set(int indexGlobalny,Pozycja poz);
    int get(Pozycja poz);
    int get(int);
    void wypisz();
};
#endif

