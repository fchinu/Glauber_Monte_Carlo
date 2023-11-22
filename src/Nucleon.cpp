// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleon class implementation

#include "../include/Nucleon.h"

Nucleon::Nucleon():
fX(0),
fY(0),
fIsWounded(false),
fNcoll(0)
{}

Nucleon::Nucleon(double x, double y):
fX(x),
fY(y),
fIsWounded(false),
fNcoll(0)
{}

void Nucleon::Collide(Nucleon nucleon, double sigmaNN)
{
    double b = TMath::Sqrt(TMath::Power(fX - nucleon.GetX(), 2) + pow(fY - nucleon.GetY(), 2));
    if (b < TMath::Sqrt(sigmaNN/TMath::Pi()))
    {
        fIsWounded = true;
        fNcoll++;
    }
}