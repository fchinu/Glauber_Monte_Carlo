// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Nucleus class implementation

#ifndef NUCLEUS_H
#define NUCLEUS_H

#include <vector>
#include "Nucleon.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TF1.h"
#include "TRandom3.h"

// enumeration for the nucleus radius profile
enum NucleusRadiusProfile {UNIFORM, WOOD_SAXON};    //TODO: add more profiles

class Nucleus
/**
 * @brief Class representing a nucleus.
 */
{
public:
    /**
     * @brief Constructor for the Nucleus class.
     * @param A The number of nucleons in the nucleus.
     * @param x The x-coordinate of the nucleus.
     * @param y The y-coordinate of the nucleus.
     * @param profile The profile of the nucleus radius.
     */
    Nucleus(int A, double x, double y, NucleusRadiusProfile profile);

    /**
     * @brief Destructor for the Nucleus class.
     */
    ~Nucleus();

    /**
     * @brief Get the number of nucleons in the nucleus.
     * @return The number of nucleons.
     */
    int GetNucleons() { return fA; }

    /**
     * @brief Get the average radius of the nucleus.
     * @return The average radius.
     */
    double GetAverageRadius() { return fAvRadius; }

    /**
     * @brief Get the x-coordinate of the nucleus.
     * @return The x-coordinate.
     */
    double GetX() { return fX; }

    /**
     * @brief Get the y-coordinate of the nucleus.
     * @return The y-coordinate.
     */
    double GetY() { return fY; }

    /**
     * @brief Get the nucleon at the specified index.
     * @param i The index of the nucleon.
     * @return The nucleon at the specified index.
     */
    Nucleon GetNucleon(int i) { return fNucleons[i]; }

    /**
     * @brief Set the number of nucleons in the nucleus.
     * @param nucleons The number of nucleons.
     */
    void SetA(int nucleons);

    /**
     * @brief Set the x-coordinate of the nucleus.
     * @param x The x-coordinate.
     */
    void SetX(double x) { fX = x; }

    /**
     * @brief Set the y-coordinate of the nucleus.
     * @param y The y-coordinate.
     */
    void SetY(double y) { fY = y; }

    /**
     * @brief Print information about the nucleus.
     * @param verbose The verbosity level of the printout.
     */
    void Print(int verbose = 0);

    /**
     * @brief Draw the nucleus.
     */
    void Draw();

private:
    int fA;                         ///< The number of nucleons in the nucleus.
    double fX;                      ///< The x-coordinate of the nucleus.
    double fY;                      ///< The y-coordinate of the nucleus.
    double fAvRadius;               ///< The average radius of the nucleus.
    double fThickness = 0;          ///< The thickness of the nucleus.
    NucleusRadiusProfile fProfile;  ///< The profile of the nucleus radius.
    std::vector<Nucleon> fNucleons; ///< The nucleons in the nucleus.
    TF1* fWoodSaxon;                ///< The Wood-Saxon function.


    double (Nucleus::*GetRadiusRndm)() = &Nucleus::GetWoodSaxonRadius;
    void (Nucleus::*ComputeAverageRadius)() = &Nucleus::ComputeAverageRadiusWoodSaxon;

    /**
     * @brief Set the profile functions of the nucleus radius.
     */
    void SetProfileFunctions()
    {
        switch (fProfile)
        {
            case UNIFORM:
                this->GetRadiusRndm = &Nucleus::GetUniformRadius;
                this->ComputeAverageRadius = &Nucleus::ComputeAverageRadiusUniform;
                break;
            case WOOD_SAXON:
                this->GetRadiusRndm = &Nucleus::GetWoodSaxonRadius;
                this->ComputeAverageRadius = &Nucleus::ComputeAverageRadiusWoodSaxon;
                break;
        }
    }

    /**
     * @brief Compute the average radius of the nucleus from uniform sphere distribution.
     * @return The average radius.
     */
    void ComputeAverageRadiusUniform()
    {   
        fAvRadius = 1.21 * TMath::Power(fA, 1.0/3.0);
    }

    /**
     * @brief Compute the average radius of the nucleus from Wood-Saxon distribution.
     * @return The average radius.
     */
    void ComputeAverageRadiusWoodSaxon()
    {
        fAvRadius = 1.12 * TMath::Power(fA, 1.0/3.0) - 0.86 * TMath::Power(fA, -1.0/3.0);
        fThickness = 0.54;
        fWoodSaxon = new TF1("fWoodSaxon", "[0]/(1+exp((x-[1])/[2]))", 0, 50);
        fWoodSaxon->SetParameters(0.16, fAvRadius, fThickness);
        fWoodSaxon->Draw();
    }

    /**
     * @brief Get a random radius according to the Wood-Saxon distribution.
     * @return The random radius.
     */
    double GetWoodSaxonRadius()
    {
        return fWoodSaxon->GetRandom();
    }

    /**
     * @brief Generate a random radius according to the uniform distribution.
     * @return The random radius.
     */
    double GetUniformRadius()
    {
        return (gRandom->Rndm()*fAvRadius);
    }

    /**
     * @brief Generate the nucleons in the nucleus.
     */
    void GenerateNucleons()
    {
        for (int i = 0; i < fA; i++)
        {
            double r = (this->*GetRadiusRndm)();
            //double z;
            gRandom->Circle(fX, fY, r);
            fNucleons.push_back(Nucleon(fX, fY));
        }
    }
};

#endif