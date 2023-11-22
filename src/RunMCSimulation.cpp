// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Script to run the Monte Carlo simulation

#include "../include/Event.h"
#include "TFile.h"

void RunMCSimulation(int A = 208, double SigmaNN = 6, double r = 5, int Nevents = 1000, NucleusRadiusProfile radiusProfile = NucleusRadiusProfile::WOOD_SAXON, CollisionsProfile profile = CollisionsProfile::GAUSSIAN)
{
    gRandom->SetSeed(42);
    TGraph *gNpart = new TGraph(Nevents);
    TGraph *gNcoll = new TGraph(Nevents);
    TGraph *gXY = new TGraph(Nevents);
    
    Event *event = new Event(A, profile, SigmaNN, r, radiusProfile);
    event->Collision();
    gNpart->SetPoint(1, event->GetDistance(), event->GetNpart());
    gNcoll->SetPoint(1, event->GetDistance(), event->GetNcoll());
    gXY->SetPoint(1, event->GetNucleus2()->GetX(), event->GetNucleus2()->GetY());
    event->Draw();
    delete event;

    for (int i = 0; i < Nevents-1; i++)
    {
        if (i%100 == 0)
            std::cout << "Event " << i << "\r" << std::endl;
        Event *event = new Event(A, profile, SigmaNN, r, radiusProfile);
        event->Collision();
        gNpart->SetPoint(i, event->GetDistance(), event->GetNpart());
        gNcoll->SetPoint(i, event->GetDistance(), event->GetNcoll());
        gXY->SetPoint(i, event->GetNucleus2()->GetX(), event->GetNucleus2()->GetY());
        delete event;
    }

    gNcoll->SetMarkerStyle(kFullCircle);
    gNcoll->SetMarkerSize(.5);
    gNcoll->SetTitle("N_{coll} vs b");
    gNcoll->GetXaxis()->SetTitle("b [fm]");
    gNcoll->GetYaxis()->SetTitle("N_{coll}");
    gNcoll->SetDrawOption("P");

    gNpart->SetMarkerStyle(kFullCircle);
    gNpart->SetMarkerSize(.5);
    gNpart->SetTitle("N_{part} vs b");
    gNpart->GetXaxis()->SetTitle("b [fm]");
    gNpart->GetYaxis()->SetTitle("N_{part}");
    gNpart->SetDrawOption("P");
    
    gXY->SetMarkerStyle(kFullCircle);
    gXY->SetMarkerSize(.5);
    gXY->SetTitle("N_{part} vs b");
    gXY->GetXaxis()->SetTitle("b [fm]");
    gXY->GetYaxis()->SetTitle("N_{part}");
    gXY->SetDrawOption("P");

    TFile* outfile = new TFile("output.root", "RECREATE");
    gNpart->Write("gNpart");
    gNcoll->Write("gNcoll");
    gXY->Write("gXY");
    outfile->Close();
}