// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Event class implementation

#include "../include/Event.h"

Event::Event(int A, CollisionsProfile profile, double r, NucleusRadiusProfile radiusProfile):
fA(A),
fCollisionsProfile(profile),
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