#ifndef _ARY_H_
#define _ARY_H_

#include "stdbool.h"

/*
- Zbiory liczb rzeczywistych bedziemy reprezentowali jako przedzialy domkniete
postaci [.pocz, .kon].
- Mozliwe jest, azeby podczas dzielenia przedzialow [a, b] i [c, d] powstaly
dwa przedzialy, jednak zawsze beda one postaci (-inf, e] <suma> [f, +inf).
Taki przedzial reprezentowany bedzie jako [.pocz, .kon], gdzie .pocz > .kon.
- Dopuszczalne sa rowniez notacje postaci (-HUGE_VAL, a] lub [a, HUGE_VAL).
*/
typedef struct wartosc {
   double pocz, kon;
} wartosc;

//-------------------------------Konstruktory----------------------------------

wartosc wartosc_dokladnosc(double x, double p);
wartosc wartosc_od_do(double x, double y);
wartosc wartosc_dokladna(double x);

//-------------------------------Selektory-------------------------------------

bool in_wartosc(wartosc w, double x);
double min_wartosc(wartosc w);
double max_wartosc(wartosc w);
double sr_wartosc(wartosc w);

//---------------------------------Modyfikatory--------------------------------

wartosc plus(wartosc a, wartosc b);
wartosc minus(wartosc a, wartosc b);
wartosc razy(wartosc a, wartosc b);
wartosc podzielic(wartosc a, wartosc b);

#endif
