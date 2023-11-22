// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleon class header

#ifndef NUCLEON_H
#define NUCLEON_H

#include <iostream>
#include "TMath.h"

class Nucleon
{
public:
    Nucleon();
    Nucleon(double x, double y);

    double GetX() {return fX;}
    double GetY() {return fY;}
    bool IsWounded() {return fIsWounded;}
    int GetNcoll() {return fNcoll;}

    void Collide(Nucleon nucleon, double sigmaNN);

    void SetX(double x) {fX = x;}
    void SetY(double y) {fY = y;}

    void Print() {std::cout << "Nucleon: \nx: " << fX << "\ny: " << fY << std::endl;}

private:
    double fX;
    double fY;
    bool fIsWounded;
    int fNcoll;
};




#endif