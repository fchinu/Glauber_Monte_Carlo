// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Event class header

#ifndef EVENT_H
#define EVENT_H

#include "Nucleus.h"

enum class CollisionsProfile {UNIFORM, GAUSSIAN};

class Event
{
public:
    Event(int A, CollisionsProfile profile, double r = 10, NucleusRadiusProfile radiusProfile = NucleusRadiusProfile::WOOD_SAXON);

    Nucleus *GetNucleus1() {return fNucleus1;}
    Nucleus *GetNucleus2() {return fNucleus2;}

    void SetNucleus1(Nucleus *nucleus1) {fNucleus1 = nucleus1;}
    void SetNucleus2(Nucleus *nucleus2) {fNucleus2 = nucleus2;}

    void Print() {std::cout << "Event: \nNucleus 1: "; fNucleus1->Print(); std::cout << "\nNucleus 2: ";fNucleus2->Print();}
    void Draw();

private:
    int fA;
    CollisionsProfile fCollisionsProfile;
    NucleusRadiusProfile fRadiusProfile;
    double fR;

    Nucleus *fNucleus1;
    Nucleus *fNucleus2;

    void (Event::*GenerateNuclei)() = &Event::GenerateNucleiUniform;
    void GenerateNucleiUniform();
    void GenerateNucleiGaussian();

    /**
     * @brief Set the profile functions of the nucleus radius.
     */
    void SetProfileFunctions()
    {
        switch (fCollisionsProfile)
        {
            case CollisionsProfile::UNIFORM:
                this->GenerateNuclei = &Event::GenerateNucleiUniform;
                break;
            case CollisionsProfile::GAUSSIAN:
                this->GenerateNuclei = &Event::GenerateNucleiGaussian;
                break;
        }
    }
};


#endif