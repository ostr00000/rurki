#ifndef powierzchnia_h
#define powierzchnia_h


#include "NajmniejszyElement.h"
#include <unordered_map>
#include "pozycja.h"
#include "fragment.h"
#include <iostream>

class Powierzchnia{
    Fragment a,b,c;
    std::unordered_map<int,NajmniejszyElement*> map;
    int podzial;
public:
    Powierzchnia(int podzial);
    int getMaxIndex();
    void addMap(int key,NajmniejszyElement *val);
    NajmniejszyElement *getElem(int key);
    void wypisz();
    void wypiszPlansze();
};
#endif
