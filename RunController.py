from ROOT import gInterpreter, gSystem, gRandom, gStyle, gROOT
from ROOT import TGraph, TFile, TCanvas, TLegend
from ROOT import kFullCircle, kWhite, kRed, kGreen, kBlack, kAzure, kTeal
gSystem.AddIncludePath(str("-I ")+gSystem.GetWorkingDirectory()+"/src")
gInterpreter.ProcessLine('#include "/home/fabrizio/Documents/Lectures/Fisica_nucleare/Glauber_Monte_Carlo/include/Event.h"')
gSystem.Load('/home/fabrizio/Documents/Lectures/Fisica_nucleare/Glauber_Monte_Carlo/build/Event_cpp.so')

from ROOT import Event, CollisionsProfile, NucleusRadiusProfile

def SetStyle(graypalette) :
  
  gStyle.Reset("Plain")
  gStyle.SetOptTitle(0)
  gStyle.SetOptStat(0)
  if(graypalette):
    gStyle.SetPalette(8,0)
  else:
    gStyle.SetPalette(1)
  gStyle.SetCanvasColor(10)
  gStyle.SetCanvasBorderMode(0)
  gStyle.SetFrameLineWidth(1)
  gStyle.SetFrameFillColor(kWhite)
  gStyle.SetPadColor(10)
  gStyle.SetPadTickX(1)
  gStyle.SetPadTickY(1)
  gStyle.SetPadBottomMargin(0.14)
  gStyle.SetPadLeftMargin(0.15)
  gStyle.SetPadRightMargin(0.035)
  gStyle.SetPadTopMargin(0.035)
  gStyle.SetHistLineWidth(1)
  gStyle.SetHistLineColor(kRed)
  gStyle.SetFuncWidth(2)
  gStyle.SetFuncColor(kGreen)
  gStyle.SetLineWidth(2)
  gStyle.SetLabelSize(0.045,"xyz")
  gStyle.SetLabelOffset(0.01,"y")
  gStyle.SetLabelOffset(0.01,"x")
  gStyle.SetLabelColor(kBlack,"xyz")
  gStyle.SetTitleSize(0.05,"xyz")
  gStyle.SetTitleOffset(1.4,"y")
  gStyle.SetTitleOffset(1.2,"x")
  gStyle.SetTitleFillColor(kWhite)
  gStyle.SetTextSizePixels(26)
  gStyle.SetTextFont(42)
  #  gStyle.SetTickLength(0.04,"X")  gStyle.SetTickLength(0.04,"Y") 

  gStyle.SetLegendBorderSize(0)
  gStyle.SetLegendFillColor(kWhite)
  #  gStyle.SetFillColor(kWhite)
  gStyle.SetLegendFont(42)


def RunMCSimulation(A=208, SigmaNN=6, r=5, Nevents=1000, radiusProfile=NucleusRadiusProfile.WOOD_SAXON, profile=CollisionsProfile.GAUSSIAN, outfile="out/data/output.root"):
    SetStyle(False)
    gRandom.SetSeed(42)
    gNpart = TGraph(Nevents)
    gNcoll = TGraph(Nevents)
    gXY = TGraph(Nevents)
    
    outfile = TFile(outfile, "RECREATE")
    outfile.mkdir("EventDisplay")
    outfile.cd("EventDisplay")

    for i in range(Nevents-1):
        event = Event(A, profile, SigmaNN, r, radiusProfile)
        event.Collision()
        gNpart.SetPoint(i, event.GetDistance(), event.GetNpart())
        gNcoll.SetPoint(i, event.GetDistance(), event.GetNcoll())
        gXY.SetPoint(i, event.GetNucleus2().GetX(), event.GetNucleus2().GetY())
        if i % 100 == 0:
            print("Event", i)
            canvas = TCanvas(f"c{i//100}", f"c{i//100}", 800, 800)
            canvas.DrawFrame(-20, -20, 20, 20,"Nuclei collision;x [fm];y [fm]")
            event.DrawHere(canvas)
            canvas.SetGrid()
            canvas.Update()
            canvas.Draw()
            canvas.Write()
            del canvas
        del event

    outfile.cd()

    gNcoll.SetMarkerStyle(kFullCircle)
    gNcoll.SetMarkerSize(.5)
    gNcoll.SetTitle("N_{coll} vs b")
    gNcoll.GetXaxis().SetTitle("b [fm]")
    gNcoll.GetYaxis().SetTitle("N_{coll}")
    gNcoll.SetDrawOption("P")

    gNpart.SetMarkerStyle(kFullCircle)
    gNpart.SetMarkerSize(.5)
    gNpart.SetTitle("N_{part} vs b")
    gNpart.GetXaxis().SetTitle("b [fm]")
    gNpart.GetYaxis().SetTitle("N_{part}")
    gNpart.SetDrawOption("P")
    
    gXY.SetMarkerStyle(kFullCircle)
    gXY.SetMarkerSize(.5)
    gXY.SetTitle("N_{part} vs b")
    gXY.GetXaxis().SetTitle("b [fm]")
    gXY.GetYaxis().SetTitle("N_{part}")
    gXY.SetDrawOption("P")

    
    gNpart.Write("gNpart")
    gNcoll.Write("gNcoll")
    gXY.Write("gXY")
    outfile.Close()

    return gNpart, gNcoll, gXY

if __name__ == "__main__":
    gROOT.SetBatch(True)
    sigmas = [2.1,3.3,4.2,6.0]
    colours = [kRed, kAzure +3, kTeal-7, kBlack]
    gNparts = []
    gNcolls = []
    for sigma in sigmas:
        gNpart, gNcoll, _ = RunMCSimulation(SigmaNN=sigma, Nevents=1000, outfile=f"out/data/output_sigmaNN_{sigma}.root")
        gNparts.append(gNpart)
        gNcolls.append(gNcoll)

    outfile = TFile("out/data/Comparison.root", "RECREATE")
    canvasNpart = TCanvas("canvasNpart", "canvasNpart", 800, 800)
    canvasNpart.DrawFrame(0, 0, 20, 500, "Nuclei collision;b [fm];N_{part}")
    canvasNcoll = TCanvas("canvasNcoll", "canvasNcoll", 800, 800)
    canvasNcoll.DrawFrame(0, 0, 20, 2000, "Nuclei collision;b [fm];N_{coll}")
    TLegendNpart = TLegend(0.6, 0.7, 0.9, 0.9)
    TLegendNcoll = TLegend(0.6, 0.7, 0.9, 0.9)
    for gNpart, gNcoll, colour, sigma in zip(gNparts, gNcolls, colours, sigmas):
        canvasNpart.cd()
        gNpart.SetLineColor(colour)
        gNpart.SetMarkerColor(colour)
        gNpart.Draw("P")
        TLegendNpart.AddEntry(gNpart, f"#sigma_{{inel}} = {sigma*10} mb", "l")
        canvasNcoll.cd()
        gNcoll.SetLineColor(colour)
        gNcoll.SetMarkerColor(colour)
        gNcoll.Draw("P")
        TLegendNcoll.AddEntry(gNcoll, f"#sigma_{{inel}} = {sigma*10} mb", "l")

    canvasNpart.cd()
    TLegendNpart.Draw()
    canvasNpart.SetGrid()
    canvasNpart.Update()
    canvasNpart.Draw()
    canvasNpart.Write()
    canvasNpart.SaveAs("out/figures/Npart.png")

    canvasNcoll.cd()
    TLegendNcoll.Draw()
    canvasNcoll.SetGrid()
    canvasNcoll.Update()
    canvasNcoll.Draw()
    canvasNcoll.Write()
    canvasNcoll.SaveAs("out/figures/Ncoll.png")
