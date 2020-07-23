/// \file RunData.cc
/// \brief Implementation of the RunData class

#include "RunData.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunData::RunData()
        : G4Run(),
          fVolumeNames{ { "SiPM1","SiPM2",
                                "SiPM3","sipm4","sipm5","sipm6","sipm7","sipm8",
                                "sipm9","sipm10","sipm11","sipm12",
                                "sipm13","sipm14","sipm15","sipm16",
                                "sipm17","sipm18","sipm19","sipm20",
                                "sipm21","sipm22","sipm23","sipm24",
                                "sipm25","sipm26","sipm27","sipm28",
                                "sipm29","sipm30","sipm31","sipm32",
                                "sipm33","sipm34","sipm35","sipm36"} }
{
    for ( auto& edep : fEkin ) {
        edep = 0.;
    }
    for ( auto& trackLength : fWavelength ) {
        trackLength = 0.;
    }
    fposx=0;
    fposy=0;
    fposz=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunData::~RunData()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunData::FillPerEvent()
{
    G4int counter = 0;

    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // accumulate statistic
    // in the order od the histograms, ntuple columns declarations

  for ( auto ekin : fEkin ) {

    analysisManager->FillNtupleDColumn(counter++, ekin);
  }
  for ( auto trackLength : fWavelength ) {
    analysisManager->FillNtupleDColumn(counter++, trackLength);
  }
    analysisManager->FillNtupleDColumn(counter++,fposx);
    analysisManager->FillNtupleDColumn(counter++,fposy);
    analysisManager->FillNtupleDColumn(counter++,fposz);
 // G4cout<<"posx= "<<fposx<< "posy= "<<fposy<<"posz ="<<fposz<<G4endl;
  analysisManager->AddNtupleRow();
  Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunData::Reset()
{
    for ( auto& edep : fEkin ) {
        edep = 0.;
    }
    for ( auto& trackLength : fWavelength ) {
        trackLength = 0.;
    }
    fposx=0;
    fposy=0;
    fposz=0;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
