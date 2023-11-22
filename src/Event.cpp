// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Event class implementation

#include "../include/Event.h"

Event::Event(int A, CollisionsProfile profile, double SigmaNN, double r, NucleusRadiusProfile radiusProfile):
fA(A),
fCollisionsProfile(profile),
fSigmaNN(SigmaNN),
fR(r),
fRadiusProfile(radiusProfile)
{
    SetProfileFunctions();
    (this->*GenerateNuclei)();
}

/**
 * @brief Generate the nuclei with uniform distribution.
 */
void Event::GenerateNucleiUniform()
{
    fNucleus1 = new Nucleus(fA, 0, 0, fRadiusProfile);
    fNucleus2 = new Nucleus(fA, gRandom->Rndm()*fR - fR/2, gRandom->Rndm()*fR - fR/2, fRadiusProfile);
}

/**
 * @brief Generate the nuclei with gaussian distribution.
 */
void Event::GenerateNucleiGaussian()
{
    fNucleus1 = new Nucleus(fA, 0, 0, NucleusRadiusProfile::WOOD_SAXON);
    fNucleus2 = new Nucleus(fA, gRandom->Gaus(0, fR), gRandom->Gaus(0, fR), NucleusRadiusProfile::WOOD_SAXON);
}

/**
 * @brief Draw the event.
 */
void Event::Draw()
{
    TCanvas *c = new TCanvas("c", "c", 800, 800);
    c->DrawFrame(-20,-20,20,20);

    TGraph* gNucleus1 = fNucleus1->GetGraph();
    TGraph* gNucleus2 = fNucleus2->GetGraph();
    gNucleus1->SetMarkerStyle(kFullCircle);
    gNucleus2->SetMarkerStyle(kFullCircle); 
    gNucleus1->SetMarkerColor(kRed);
    gNucleus2->SetMarkerColor(kBlue);
    gNucleus1->SetMarkerSize(1.);
    gNucleus2->SetMarkerSize(1.);
    gNucleus1->Draw("P,same");
    gNucleus2->Draw("P,same");

    c->SaveAs("event.png");
}

void Event::Print(int verbose)
{
    std::cout << "Event: \nA: " << fA << "\nSigmaNN: " << fSigmaNN << "\nParticipants: " << GetNpart() << "\nBinary collisions: " << GetNcoll() << std::endl;
    if (verbose)
    {
        std::cout << "Nucleus 1: \n";
        fNucleus1->Print();
        std::cout << "Nucleus 2: \n";
        fNucleus2->Print();
    }
}

/**
 * @brief Simulate the collision between the two nuclei.
 */
void Event::Collision()
{
    fNucleus2->Collide(*fNucleus1, fSigmaNN);
    fNucleus1->Collide(*fNucleus2, fSigmaNN);
}

int Event::GetNpart()
{
    return fNucleus1->GetNpart() + fNucleus2->GetNpart();
}

int Event::GetNcoll()
{
    int Ncoll = 0;
    Ncoll += fNucleus1->GetNcoll();
    Ncoll += fNucleus2->GetNcoll();
    return Ncoll;
}

void Event::SetProfileFunctions()
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