// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleon class implementation

#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include <string>

void Run(TString myopt="")
{
    gSystem->AddIncludePath((string("-I ")+gSystem->GetWorkingDirectory()+"/build").c_str());
    TString opt;
    if (myopt.Contains("force"))
        opt="kfg-";          //k mantiene il .so, f forza la compilazione (come mettere il ++), g serve se vogliamo usare un debugger
    else
        opt="kg-";
    if (myopt.Contains("clean"))
        gSystem->Exec("./clean.sh");
    gSystem->CompileMacro("src/Nucleon.cpp",opt.Data(),"","build");
    gSystem->CompileMacro("src/Nucleus.cpp",opt.Data(),"","build");
    gSystem->CompileMacro("src/Event.cpp",opt.Data(),"","build");
    gSystem->CompileMacro("src/RunMCSimulation.cpp",opt.Data(),"","build");

    gROOT->ProcessLine("RunMCSimulation(208, 6, 5, 1000, NucleusRadiusProfile::WOOD_SAXON, CollisionsProfile::GAUSSIAN)");
    //gROOT->ProcessLine("a.Collision()");
    //gROOT->ProcessLine("a.Print()");
    //gROOT->ProcessLine("a.Draw()");
}