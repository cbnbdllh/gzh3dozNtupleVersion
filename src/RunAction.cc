//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file optical/OpNovice2/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"

#include "RunAction.hh"
#include "RunData.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"

#include "Run.hh"
#include "G4Run.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction* prim)
 : G4UserRunAction(),
   fTimer(nullptr),
   fRun(nullptr),
   fHistoManager(nullptr),
   fPrimary(prim)
{
  fTimer = new G4Timer;

  auto  analysisManager = G4AnalysisManager::Instance();

  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  analysisManager->CreateNtuple("test", "analysis for per event");
    analysisManager->CreateNtupleDColumn("Ekin1");
    analysisManager->CreateNtupleDColumn("Ekin2");
     analysisManager->CreateNtupleDColumn("Ekin3");
     analysisManager->CreateNtupleDColumn("Ekin4");
     analysisManager->CreateNtupleDColumn("Ekin5");
     analysisManager->CreateNtupleDColumn("Ekin6");
     analysisManager->CreateNtupleDColumn("Ekin7");
     analysisManager->CreateNtupleDColumn("Ekin8");
     analysisManager->CreateNtupleDColumn("Ekin9");
     analysisManager->CreateNtupleDColumn("Ekin10");
     analysisManager->CreateNtupleDColumn("Ekin11");
     analysisManager->CreateNtupleDColumn("Ekin12");
     analysisManager->CreateNtupleDColumn("Ekin13");
     analysisManager->CreateNtupleDColumn("Ekin14");
     analysisManager->CreateNtupleDColumn("Ekin15");
     analysisManager->CreateNtupleDColumn("Ekin16");
     analysisManager->CreateNtupleDColumn("Ekin17");
     analysisManager->CreateNtupleDColumn("Ekin18");
     analysisManager->CreateNtupleDColumn("Ekin19");
     analysisManager->CreateNtupleDColumn("Ekin20");
     analysisManager->CreateNtupleDColumn("Ekin21");
     analysisManager->CreateNtupleDColumn("Ekin22");
     analysisManager->CreateNtupleDColumn("Ekin23");
     analysisManager->CreateNtupleDColumn("Ekin24");
     analysisManager->CreateNtupleDColumn("Ekin25");
     analysisManager->CreateNtupleDColumn("Ekin26");
     analysisManager->CreateNtupleDColumn("Ekin27");
     analysisManager->CreateNtupleDColumn("Ekin28");
     analysisManager->CreateNtupleDColumn("Ekin29");
     analysisManager->CreateNtupleDColumn("Ekin30");
     analysisManager->CreateNtupleDColumn("Ekin31");
     analysisManager->CreateNtupleDColumn("Ekin32");
     analysisManager->CreateNtupleDColumn("Ekin33");
     analysisManager->CreateNtupleDColumn("Ekin34");
     analysisManager->CreateNtupleDColumn("Ekin35");
     analysisManager->CreateNtupleDColumn("Ekin36");
     analysisManager->CreateNtupleDColumn("count1");
     analysisManager->CreateNtupleDColumn("count2");
     analysisManager->CreateNtupleDColumn("count3");
     analysisManager->CreateNtupleDColumn("count4");
     analysisManager->CreateNtupleDColumn("count5");
     analysisManager->CreateNtupleDColumn("count6");
     analysisManager->CreateNtupleDColumn("count7");
     analysisManager->CreateNtupleDColumn("count8");
     analysisManager->CreateNtupleDColumn("count9");
     analysisManager->CreateNtupleDColumn("count10");
     analysisManager->CreateNtupleDColumn("count11");
     analysisManager->CreateNtupleDColumn("count12");
     analysisManager->CreateNtupleDColumn("count13");
     analysisManager->CreateNtupleDColumn("count14");
     analysisManager->CreateNtupleDColumn("count15");
     analysisManager->CreateNtupleDColumn("count16");
     analysisManager->CreateNtupleDColumn("count17");
     analysisManager->CreateNtupleDColumn("count18");
     analysisManager->CreateNtupleDColumn("count19");
     analysisManager->CreateNtupleDColumn("count20");
     analysisManager->CreateNtupleDColumn("count21");
     analysisManager->CreateNtupleDColumn("count22");
     analysisManager->CreateNtupleDColumn("count23");
     analysisManager->CreateNtupleDColumn("count24");
     analysisManager->CreateNtupleDColumn("count25");
     analysisManager->CreateNtupleDColumn("count26");
     analysisManager->CreateNtupleDColumn("count27");
     analysisManager->CreateNtupleDColumn("count28");
     analysisManager->CreateNtupleDColumn("count29");
     analysisManager->CreateNtupleDColumn("count30");
     analysisManager->CreateNtupleDColumn("count31");
     analysisManager->CreateNtupleDColumn("count32");
     analysisManager->CreateNtupleDColumn("count33");
     analysisManager->CreateNtupleDColumn("count34");
     analysisManager->CreateNtupleDColumn("count35");
     analysisManager->CreateNtupleDColumn("count36");
    analysisManager->CreateNtupleDColumn("MylarxAxes");
    analysisManager->CreateNtupleDColumn("MylaryAxes");
    analysisManager->CreateNtupleDColumn("MylarzAxes");
  analysisManager->FinishNtuple();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
  delete G4AnalysisManager::Instance();
}

G4Run* RunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (fPrimary) {
    G4ParticleDefinition* particle = 
      fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

    auto analysisManager = G4AnalysisManager::Instance();
    G4String fileName = "gzhnew";
    analysisManager->OpenFile(fileName);

    fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

  if (isMaster) fRun->EndOfRun();

  // save histograms
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
