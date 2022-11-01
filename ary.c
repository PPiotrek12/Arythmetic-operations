#include "ary.h"
#include "math.h"

//---------------------------Funkcje pomocnicze--------------------------------

//Max22(NAN, x) = x;
double Max2(double a, double b) {
   if(isnan(a) == 0 && (a > b || isnan(b) == 1)) return a;
   else return b;
}
double Max4(double a, double b, double c, double d) {
   return Max2(Max2(a, b), Max2(c, d));
}
//Min2(NAN, x) = x;
double Min2(double a, double b) {
   if(isnan(a) == 0 && (a < b || isnan(b) == 1)) return a;
   else return b;
}
double Min4(double a, double b, double c, double d) {
   return Min2(Min2(a, b), Min2(c, d));
}

bool zero(double x) { return fabs(x) < 1e-10; }
bool rowne(double x, double y){ return fabs(x-y)<1e-10; }

//-------------------------------Konstruktory----------------------------------

wartosc wartosc_od_do(double x, double y) {
   wartosc wynik = {.pocz = x, .kon = y};
   return wynik;
}

wartosc wartosc_dokladna(double x) { return wartosc_od_do(x, x); }

wartosc wartosc_dokladnosc(double x, double p) {
   if(x > 0 || zero(x))
      return wartosc_od_do(x - x * p / (double)100, x + x * p / (double)100);
   else
      return wartosc_od_do(x + x * p / (double)100, x - x * p / (double)100);
}

//-------------------------------Selektory-------------------------------------

bool in_wartosc(wartosc w, double x) {
   if(isnan(w.pocz)) return 0;
   if(w.pocz > w.kon)
      return ((x < w.kon || rowne(x, w.kon)) || (x > w.pocz || rowne(x, w.pocz)));
   else
      return ((x > w.pocz || rowne(x, w.pocz)) && (x < w.kon || rowne(x, w.kon)));
}

double min_wartosc(wartosc w) {
   if(isnan(w.pocz)) return NAN;
   double wynik = -HUGE_VAL;
   if(w.pocz < w.kon || rowne(w.pocz, w.kon)) wynik = w.pocz;
   return wynik;
}

double max_wartosc(wartosc w) {
   if(isnan(w.pocz)) return NAN;
   double wynik = HUGE_VAL;
   if(w.pocz < w.kon || rowne(w.pocz, w.kon)) wynik = w.kon;
   return wynik;
}

double sr_wartosc(wartosc w) {
   if(isnan(w.pocz) || w.pocz > w.kon) return NAN;
   return (w.pocz + w.kon) / (double)2;
}

//--------------------------Modyfikatory: plus i minus-------------------------

wartosc plus(wartosc a, wartosc b) {
   if(isnan(a.pocz) || isnan(b.pocz)) {
      wartosc wynik = {.pocz = NAN, .kon = NAN};
      return wynik;
   }
   if(a.pocz > a.kon && b.pocz > b.kon) {
      wartosc wynik = {.pocz = -HUGE_VAL, .kon = HUGE_VAL};
      return wynik;
   }
   wartosc wynik = {.pocz = a.pocz + b.pocz, .kon = a.kon + b.kon};

   /*Jakis przedzial byl postaci (-inf, b] <suma> [a, inf), a po operacji jest
   [a, b]. Powinien wiec byc (-inf, inf). */
   if((a.pocz > a.kon || b.pocz > b.kon) &&
            (wynik.pocz < wynik.kon || rowne(wynik.pocz, wynik.kon)))
      wynik.pocz = -HUGE_VAL, wynik.kon = HUGE_VAL;
   return wynik;
}

wartosc minus(wartosc a, wartosc b) {
   if(isnan(a.pocz) || isnan(b.pocz)) {
      wartosc wynik = {.pocz = NAN, .kon = NAN};
      return wynik;
   }
   wartosc b2 = {.pocz = -b.kon, .kon = -b.pocz};
   return plus(a, b2);
}

//------------------------Funkcje pomocnicze do mnozenia-----------------------


wartosc razy_oba_antyprzedzialy(wartosc a, wartosc b) {
   wartosc wynik;
   if(a.pocz < 0 || zero(a.pocz) || b.pocz < 0 || zero(b.pocz)
            || a.kon > 0 || zero(a.kon) || b.kon > 0 || zero(b.kon))
      wynik.pocz = -HUGE_VAL, wynik.kon = HUGE_VAL;
   else {
      wynik.pocz = Min2(a.pocz * b.pocz, a.kon * b.kon);
      wynik.kon = Max2(a.pocz * b.kon, a.kon * b.pocz);
   }
   return wynik;
}

wartosc razy_jeden_antyprzedzial(wartosc a, wartosc b) {
   wartosc wynik;
   if(b.pocz < 0 && b.kon < 0) {
      wynik.pocz = Min2(a.kon * b.pocz, a.kon * b.kon);
      wynik.kon = Max2(a.pocz * b.pocz, a.pocz * b.kon);
   }
   else if(b.pocz > 0 && b.kon > 0) {
      wynik.pocz = Min2(a.pocz * b.pocz, a.pocz * b.kon);
      wynik.kon = Max2(a.kon * b.pocz, a.kon * b.kon);
   }
   if(((b.pocz < 0 || zero(b.pocz)) && (b.kon > 0 || zero(b.kon)))
            || (wynik.pocz < wynik.kon || rowne(wynik.pocz, wynik.kon)))
      wynik.pocz = -HUGE_VAL, wynik.kon = HUGE_VAL;
   return wynik;
}

wartosc razy_oba_przedzialy(wartosc a, wartosc b) {
   wartosc wynik = {
   .pocz = Min4(a.pocz * b.pocz, a.pocz * b.kon,
                     a.kon * b.pocz, a.kon * b.kon),
   .kon = Max4(a.pocz * b.pocz, a.pocz * b.kon,
                     a.kon * b.pocz, a.kon * b.kon) };
   return wynik;
}

//------------------------Modyfikatory: razy i podzielic-----------------------

wartosc razy(wartosc a, wartosc b) {
   if(isnan(a.pocz) || isnan(b.pocz)) { //Brak liczby
      wartosc wynikNAN = {.pocz = NAN, .kon = NAN};
      return wynikNAN;
   }
   if((zero(a.pocz) && zero(a.kon)) //Mnozenie przez 0.
            || (zero(b.pocz) && zero(b.kon))) {
      wartosc wyn = {.pocz = 0, .kon = 0};
      return wyn;
   }

   /* Przypadek, gdy obie wartosci sa postaci [-inf, kon] <suma> [pocz, inf] */
   if(a.pocz > a.kon && b.pocz > b.kon)
      return razy_oba_antyprzedzialy(a, b);

   if((a.pocz < a.kon || rowne(a.pocz, a.kon)) && b.pocz > b.kon) {
      wartosc pom = a;
      a = b;
      b = pom;
   }
   /* Przypadek gdy jedna wartosc jest postaci [-inf, kon] <suma> [pocz, inf] */
   if(a.pocz > a.kon && (b.pocz < b.kon || rowne(b.pocz, b.kon)))
      return razy_jeden_antyprzedzial(a, b);

   /* Przypadek, gdy oba przedzialy sa postaci [pocz, kon] */
   //if(a.pocz <= a.kon && b.pocz <= b.kon)
   return razy_oba_przedzialy(a, b);
}

/* Funkcja pomocnicza do podzielic - odraca przedzial, tzn.
   przyjmuje za argument przedzial [a, b] i zwraca 1/[a,b]. */
wartosc zamien_na_odwrotnosc(wartosc a) {
   wartosc wynik;
   if(isnan(a.pocz)) return a;

   if(a.pocz <= -HUGE_VAL && a.kon >= HUGE_VAL) return a;

   /* Jeden z koncow przedzialow jest zerem. Zwracamy odpowiednio:
   [1/b, inf) lub (-inf, 1/a]. */
   if(zero(a.pocz) && a.kon > 0)
      wynik.pocz = (double)1 / a.kon, wynik.kon = HUGE_VAL;
   else if(a.pocz < 0  && zero(a.kon))
      wynik.pocz = -HUGE_VAL, wynik.kon = (double)1 / a.pocz;

   /* Przedzial jest zerem. Nie istnieje odwrotnosc */
   else if(zero(a.pocz) && zero(a.kon))
      wynik.pocz = wynik.kon = NAN;
   else
      wynik.pocz = (double)1 / a.kon, wynik.kon = (double)1 / a.pocz;
   return wynik;
}

wartosc podzielic(wartosc a, wartosc b) {
   return razy(a, zamien_na_odwrotnosc(b));
}
