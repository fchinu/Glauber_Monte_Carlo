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
    TGraph* graph = new TGraph(fNucleons.size());

    for (std::vector<Nucleon>::size_type i = 0; i < fNucleons.size(); i++)
        graph->SetPoint(i, fNucleons[i].GetX(), fNucleons[i].GetY());

    graph->SetTitle("Nucleus Scatter Plot");
    graph->GetXaxis()->SetTitle("X");
    graph->GetYaxis()->SetTitle("Y");
    graph->SetMarkerStyle(kFullCircle);
    graph->SetMarkerSize(1);

    graph->Draw("P,same");

    canvas->Update();
    canvas->SaveAs("nucleus_scatter_plot.png");

    delete graph;
    delete canvas;
}