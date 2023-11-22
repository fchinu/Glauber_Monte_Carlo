// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleus class implementation

#include "../include/Nucleus.h"

Nucleus::Nucleus(int A, double x, double y, NucleusRadiusProfile profile):
fA(A),
fX(x),
fY(y),
fProfile(profile)
{
    SetProfileFunctions();
    (this->*ComputeAverageRadius)();
    GenerateNucleons();
    FillGraph();
}

Nucleus::~Nucleus()
{
    fNucleons.clear();
}

void Nucleus::Print(int verbose)
{
    std::cout << "Nucleus: \nA: " << fA << "\nX: " << fX << "\nY: " << fY << "\nAverage radius: " << fAvRadius << std::endl;
    if (verbose)
    {
        for (int i=0; i<fA; i++)
        {
            std::cout << "Nucleon " << i << ": \n";
            fNucleons[i].Print();
        }
    }
}

void Nucleus::Draw()
{
    TCanvas* canvas = new TCanvas("canvas", "Nucleus Scatter Plot", 800, 600);
    canvas->DrawFrame(-20, -20, 20, 20);

    fGraph->Draw("P,same");

    canvas->Update();
    canvas->SaveAs("nucleus_scatter_plot.png");

    delete canvas;
}

void Nucleus::Collide(Nucleus nucleus, double sigmaNN)
{
    for (int i=0; i<fA; i++)
    {
        for (int j=0; j<nucleus.GetNucleons(); j++)
        {
            fNucleons[i].Collide(nucleus.GetNucleon(j), sigmaNN);
        }
    }
}

int Nucleus::GetNpart()
{
    int Npart = 0;
    for (int i=0; i<fA; i++)
        if (fNucleons[i].IsWounded())
            Npart++;

    return Npart;
}

int Nucleus::GetNcoll()
{
    int Ncoll = 0;
    for (int i=0; i<fA; i++)
        Ncoll += fNucleons[i].GetNcoll();

    return Ncoll;
}