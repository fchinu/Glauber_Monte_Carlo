// Creator:  Fabrizio Chinu
// Date: 22/11/2023
// Last modify: 22/11/2023
// Description: Event class header

#ifndef EVENT_H
#define EVENT_H

#include "Nucleus.h"

enum class CollisionsProfile {UNIFORM, GAUSSIAN};

/**
 * @brief The Event class represents a nuclear collision event.
 */
class Event {
public:
    /**
     * @brief Constructs an Event object with the specified parameters.
     * @param A The atomic number of the nucleus.
     * @param profile The collisions profile.
     * @param SigmaNN The nucleon-nucleon cross section.
     * @param r The impact parameter.
     * @param radiusProfile The nucleus radius profile.
     */
    Event(int A, CollisionsProfile profile, double SigmaNN=6, double r = 10, NucleusRadiusProfile radiusProfile = NucleusRadiusProfile::UNIFORM);

    /**
     * @brief Destructor for the Event class.
     */
    ~Event();

    /**
     * @brief Returns the first nucleus involved in the event.
     * @return A pointer to the first nucleus.
     */
    Nucleus *GetNucleus1() {return fNucleus1;}

    /**
     * @brief Returns the second nucleus involved in the event.
     * @return A pointer to the second nucleus.
     */
    Nucleus *GetNucleus2() {return fNucleus2;}

    /**
     * @brief Getter for the 2 nuclei graphs.
     * @param gNucleus1 The graph of the first nucleus.
     * @param gNucleus2 The graph of the second nucleus.
     */
    void GetEventGraphs(TGraph *gNucleus1, TGraph *gNucleus2)
    {
        gNucleus1 = fNucleus1->GetGraph();
        gNucleus2 = fNucleus2->GetGraph();
    }

    /**
     * @brief Sets the first nucleus involved in the event.
     * @param nucleus1 A pointer to the first nucleus.
     */
    void SetNucleus1(Nucleus *nucleus1) {fNucleus1 = nucleus1;}

    /**
     * @brief Sets the second nucleus involved in the event.
     * @param nucleus2 A pointer to the second nucleus.
     */
    void SetNucleus2(Nucleus *nucleus2) {fNucleus2 = nucleus2;}

    /**
     * @brief Returns the distance between the two nuclei.
     * @return The distance between the two nuclei.
     */
    double GetDistance();

    /**
     * @brief Performs the collision between the two nuclei.
     */
    void Collision();

    /**
     * @brief Returns the number of participants in the collision.
     * @return The number of participants.
     */
    int GetNpart();

    /**
     * @brief Returns the number of binary collisions in the collision.
     * @return The number of binary collisions.
     */
    int GetNcoll();

    /**
     * @brief Prints the details of the event.
     */
    void Print(int verbose = 0);

    /**
     * @brief Draws a visualization of the event.
     */
    void Draw();

    /**
     * @brief Draws a visualization of the event in the selected canvas.
     */
    void DrawHere(TCanvas *c);

private:
    int fA; // The atomic number of the nucleus.
    CollisionsProfile fCollisionsProfile; // The collisions profile.
    NucleusRadiusProfile fRadiusProfile; // The nucleus radius profile.
    double fR, fSigmaNN; // The impact parameter and nucleon-nucleon cross section.

    Nucleus *fNucleus1; // Pointer to the first nucleus.
    Nucleus *fNucleus2; // Pointer to the second nucleus.

    void (Event::*GenerateNuclei)() = &Event::GenerateNucleiUniform; // Pointer to the function for generating nuclei.
    void GenerateNucleiUniform(); // Generates nuclei with uniform profile.
    void GenerateNucleiGaussian(); // Generates nuclei with Gaussian profile.

    /**
     * @brief Sets the profile functions of the nucleus radius based on the collisions profile.
     */
    void SetProfileFunctions();
};


#endif