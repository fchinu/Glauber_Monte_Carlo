// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleus class implementation

#include "../include/Nucleus.h"
#include "TEllipse.h"
#include "TH1D.h"

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
            fNucleons[i].Collide(nucleus.GetNucleon(j), sigmaNN);
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

void Nucleus::DrawCollision(int i, Nucleus nucleus, double sigmaNN)
{
    TCanvas* canvas = new TCanvas("canvas", "Nucleus Scatter Plot", 800, 600);
    canvas->DrawFrame(-20, -20, 20, 20);

    TGraph* gNucleon = new TGraph(1);
    gNucleon->SetPoint(0, fNucleons[i].GetX(), fNucleons[i].GetY());
    gNucleon->SetMarkerStyle(kFullCircle);
    gNucleon->SetMarkerSize(1.);
    gNucleon->SetMarkerColor(kRed);

    TGraph* gNucleus = new TGraph(nucleus.GetNucleons());
    Nucleon myNucleon = fNucleons[i];
    myNucleon.Print();
    int counter = 0;
    for (int j=0; j<nucleus.GetNucleons(); j++)
    {
        Nucleon nucleon = nucleus.GetNucleon(j);
        double b = TMath::Sqrt(TMath::Power(myNucleon.GetX() - nucleon.GetX(), 2) + TMath::Power(myNucleon.GetY() - nucleon.GetY(), 2));
        if (b < TMath::Sqrt(sigmaNN/TMath::Pi()))
        {
            counter++;
            std::cout<<"COUNTER: "<<counter<<std::endl;
            gNucleus->SetPoint(counter, nucleon.GetX(), nucleon.GetY());
            std::cout<<"x: "<<nucleon.GetX()<<" y: "<<nucleon.GetY()<<std::endl;
            std::cout << "Nucleon " << j << ": \n";
            nucleon.Print();
            std::cout << "b: " << b << std::endl;
            std::cout << "max b: " << TMath::Sqrt(sigmaNN/TMath::Pi()) << std::endl;
        }
    }

    gNucleus->SetMarkerStyle(kFullCircle);
    gNucleus->SetMarkerSize(1.);
    gNucleus->SetMarkerColor(kBlue);

    TEllipse el1(myNucleon.GetX(),myNucleon.GetY(),TMath::Sqrt(sigmaNN/TMath::Pi()),TMath::Sqrt(sigmaNN/TMath::Pi()));
    el1.Draw("same");
    gNucleon->Draw("P,same");
    gNucleus->Draw("P,same");


    canvas->Update();
    canvas->SaveAs("nucleus_scatter_plot.png");
    canvas->SaveAs("nucleus_scatter_plot.root");
    std::cin.get();
}