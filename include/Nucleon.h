// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleon class header

#ifndef NUCLEON_H
#define NUCLEON_H

#include <iostream>

class Nucleon
{
public:
    Nucleon();
    Nucleon(double x, double y);

    double GetX() {return fX;}
    double GetY() {return fY;}

    void SetX(double x) {fX = x;}
    void SetY(double y) {fY = y;}

    void Print() {std::cout << "Nucleon: \nx: " << fX << "\ny: " << fY << std::endl;}

private:
    double fX;
    double fY;
};




#endif