//

/// \file RunData.hh
/// \brief Definition of the RunData class

#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "globals.hh"

#include <array>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


const G4int kDim = 36;

///  Run data class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers.
///
/// In order to reduce the number of data members a 2-dimensions array
/// is introduced for each quantity:
/// - fEdep[], fTrackLength[].
///
/// The data are collected step by step in SteppingAction, and
/// the accumulated values are filled in histograms and entuple
/// event by event in B4EventAction.

class RunData : public G4Run
{
public:
    RunData();
    virtual ~RunData();

    void detectorData1(G4double dx, G4double dy);
    void detectorData2(G4double dz);

    void Add(G4int id, G4double de, G4double dl);
    void FillPerEvent();

    void Reset();


private:
    std::array<G4String, kDim>  fVolumeNames;
    std::array<G4double, kDim>  fEkin;
    std::array<G4double, kDim>  fWavelength;
    G4double fposx;
    G4double fposy;
    G4double fposz;


};

// inline functions



inline void RunData::Add(G4int id, G4double de, G4double dl) {
    fEkin[id] = de;
    fWavelength[id] = dl;
}
inline  void RunData::detectorData1(G4double dx, G4double dy) {
    fposx = dx;
    fposy = dy;
}
inline  void RunData::detectorData2(G4double dz) {
    fposz = dz;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

