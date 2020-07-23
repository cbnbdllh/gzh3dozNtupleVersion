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
//
/// \file optical/OpNovice2/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
//#include "EventAction.hh"
//#include "HistoManager.hh"
#include "TrackInformation.hh"
#include "Run.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4ProcessManager.hh"
#include "RunData.hh"

#include "G4SystemOfUnits.hh"
//int fPhotonCounter1=0;
//int fPhotonCounter2=0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::SteppingAction()
: G4UserSteppingAction(),
  fVerbose(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  static G4ParticleDefinition* opticalphoton = 
              G4OpticalPhoton::OpticalPhotonDefinition();
//  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  Run* run = static_cast<Run*>(
               G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  // bu line yeni
  auto runData = static_cast<RunData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  G4Track* track = step->GetTrack();
  G4StepPoint* endPoint   = step->GetPostStepPoint();
  G4StepPoint* startPoint = step->GetPreStepPoint();

  G4String particleName = track->GetDynamicParticle()->
                                 GetParticleDefinition()->GetParticleName();

    G4String volume = step->GetTrack()->GetVolume()->GetName();
    G4ThreeVector pos =step->GetTrack()->GetPosition();
    /////////***********position information
    if ( volume == "Mylar"&&
         // step->GetTrack()->GetParentID() == 0 &&
         step->GetTrack()->GetDynamicParticle()->GetMomentumDirection().z() < 0
            ){
        if(step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
              G4cout<<"posx=  "<<pos.x() /cm<<"  posy=  "<<pos.y() /cm<<"  posz=  "<<pos.z() /cm<<G4endl;
            runData->detectorData1(pos.x() /cm, pos.y() /cm);}
        runData->detectorData2(pos.z() /cm);
    }

  TrackInformation* trackInfo = 
                        (TrackInformation*)(track->GetUserInformation());
   // const G4double hc = 1239.7 / eV * nm;
  if (particleName == "opticalphoton") {
    const G4VProcess* pds = endPoint->GetProcessDefinedStep();
    if (pds->GetProcessName() == "OpAbsorption") {
      run->AddOpAbsorption(); 
      if (trackInfo->GetIsFirstScintillatorX()) {
        run->AddOpAbsorptionPrior();
      }
    } 
    else if (pds->GetProcessName() == "OpRayleigh") {
      run->AddRayleigh();
    }

    // optical process has endpt on bdry, 
    if (endPoint->GetStepStatus() == fGeomBoundary) {

      const G4DynamicParticle* theParticle = track->GetDynamicParticle();

      G4ThreeVector oldMomentumDir = theParticle->GetMomentumDirection();

      G4ThreeVector m0 = startPoint->GetMomentumDirection();
      G4ThreeVector m1 = endPoint->GetMomentumDirection();

      G4OpBoundaryProcessStatus theStatus = Undefined;

      G4ProcessManager* OpManager = 
        G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
      G4int MAXofPostStepLoops = 
        OpManager->GetPostStepProcessVector()->entries();
      G4ProcessVector* postStepDoItVector = 
        OpManager->GetPostStepProcessVector(typeDoIt);


      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM1"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease1") {
	  // Number of Photons
	  G4int fPhotonCounter=0;
	  fPhotonCounter++;
	  G4double fEkin1 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(14, fEkin1);
	  runData->Add(0,fEkin1,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM2"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease2") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin2 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(15, fEkin2);
        runData->Add(1,fEkin2,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM3"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease3") {
	  // Number of Photons
	  G4int fPhotonCounter=0;
	  fPhotonCounter++;
	  G4double fEkin3 = startPoint->GetKineticEnergy() / eV;
	 // analysisMan->FillH1(16, fEkin3);
            runData->Add(2,fEkin3,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM4"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease4") {
	  // Number of Photons
	  G4int fPhotonCounter=0;
	  fPhotonCounter++;
	  G4double fEkin4 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(17, fEkin4);
        runData->Add(3,fEkin4,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM5"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease5") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin5 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(18, fEkin5);
            runData->Add(4,fEkin5,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM6"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease6") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin6 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(19, fEkin6);
        runData->Add(5,fEkin6,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM7"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease7") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin7 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(20, fEkin7);
            runData->Add(6,fEkin7,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM8"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease8") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin8 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(21, fEkin8);
        runData->Add(7,fEkin8,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM9"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease9") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin9 = startPoint->GetKineticEnergy() / eV;
	 // analysisMan->FillH1(22, fEkin9);
            runData->Add(8,fEkin9,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM10"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease10") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin10 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(23, fEkin10);
        runData->Add(9,fEkin10,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM11"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease11") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin11 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(24, fEkin11);
            runData->Add(10,fEkin11,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM12"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease12") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin12 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(25, fEkin12);
        runData->Add(11,fEkin12,fPhotonCounter);

    }
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM13"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease13") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin13 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(26, fEkin13);
            runData->Add(12,fEkin13,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM14"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease14") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin14 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(27, fEkin14);
        runData->Add(13,fEkin14,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM15"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease15") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin15 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(28, fEkin15);
            runData->Add(14,fEkin15,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM16"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease16") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin16 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(29, fEkin16);
        runData->Add(15,fEkin16,fPhotonCounter);
	}
      	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM17"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease17") {
	  // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
	  G4double fEkin17 = startPoint->GetKineticEnergy() / eV;
	//  analysisMan->FillH1(30, fEkin17);
            runData->Add(16,fEkin17,fPhotonCounter);
	}
	if (endPoint->GetPhysicalVolume()->GetName() == "SiPM18"
	    && startPoint->GetPhysicalVolume()->GetName()
	       == "Grease18") {
	  // Number of Photons
        G4int fPhotonCounter=0;
        fPhotonCounter++;
	  G4double fEkin18 = startPoint->GetKineticEnergy() / eV;
	 // analysisMan->FillH1(31, fEkin18);
        runData->Add(17,fEkin18,fPhotonCounter);
	}
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM19"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease19") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin19 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(32, fEkin19);
              	  runData->Add(18,fEkin19,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM20"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease20") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin20 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(33, fEkin20);
                  runData->Add(19,fEkin20,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM21"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease21") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin21 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(34, fEkin21);
                  runData->Add(20,fEkin21,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM22"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease22") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin22 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(35, fEkin22);
                 runData->Add(21,fEkin22,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM23"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease23") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin23 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(36, fEkin23);
                    runData->Add(22,fEkin23,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM24"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease24") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin24 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(37, fEkin24);
                    runData->Add(23,fEkin24,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM25"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease25") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin25 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(38, fEkin25);
                runData->Add(24,fEkin25,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM26"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease26") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin26 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(39, fEkin26);
                 runData->Add(25,fEkin26,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM27"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease27") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin27 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(40, fEkin27);
                      runData->Add(26,fEkin27,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM28"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease28") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin28 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(41, fEkin28);
                 runData->Add(27,fEkin28,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM29"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease29") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin29 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(42, fEkin29);
                runData->Add(28,fEkin29,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM30"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease30") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin30 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(43, fEkin30);
               runData->Add(29,fEkin30,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM31"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease31") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin31 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(44, fEkin31);
               runData->Add(30,fEkin31,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM32"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease32") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin32 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(45, fEkin32);
                 runData->Add(31,fEkin32,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM33"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease33") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin33 = startPoint->GetKineticEnergy() / eV;
         //   analysisMan->FillH1(46, fEkin33);
                   runData->Add(32,fEkin33,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM34"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease34") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin34 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(47, fEkin34);
            runData->Add(33,fEkin34,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM35"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease35") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin35 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(48, fEkin35);
            runData->Add(34,fEkin35,fPhotonCounter);

        }
        if (endPoint->GetPhysicalVolume()->GetName() == "SiPM36"
            && startPoint->GetPhysicalVolume()->GetName()
               == "Grease36") {
            // Number of Photons
            G4int fPhotonCounter=0;
            fPhotonCounter++;
            G4double fEkin36 = startPoint->GetKineticEnergy() / eV;
        //    analysisMan->FillH1(49, fEkin36);
                    runData->Add(35,fEkin36,fPhotonCounter);

        }


      if (trackInfo->GetIsFirstScintillatorX()) {
        G4ThreeVector momdir = endPoint->GetMomentumDirection();
     //   G4double px1 = momdir.x();
    //    G4double py1 = momdir.y();
     //   G4double pz1 = momdir.z();


        trackInfo->SetIsFirstScintillatorX(false);
        run->AddTotalSurface(); 

        for (G4int i=0; i<MAXofPostStepLoops; ++i) {
          G4VProcess* currentProcess = (*postStepDoItVector)[i];

          G4OpBoundaryProcess* opProc = 
            dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
          if (opProc) {
            theStatus = opProc->GetStatus();
          //  analysisMan->FillH1(3, theStatus);
            if (theStatus == Transmission) {
              run->AddTransmission();
            }
            else if (theStatus == FresnelRefraction) {
              run->AddFresnelRefraction(); 
         //     analysisMan->FillH1(10, px1);
           //   analysisMan->FillH1(11, py1);
             // analysisMan->FillH1(12, pz1);
            }
            else if (theStatus == FresnelReflection) { 
              run->AddFresnelReflection(); 
            }
            else if (theStatus == TotalInternalReflection) { 
              run->AddTotalInternalReflection();
            }
            else if (theStatus == LambertianReflection) {
              run->AddLambertianReflection();
            }
            else if (theStatus == LobeReflection) {
              run->AddLobeReflection();
            }
            else if (theStatus == SpikeReflection) {
              run->AddSpikeReflection();
            }
            else if (theStatus == BackScattering) {
              run->AddBackScattering();
            }
            else if (theStatus == Absorption) {
              run->AddAbsorption();
            }
            else if (theStatus == Detection) {
              run->AddDetection();
            }
            else if (theStatus == NotAtBoundary) {
              run->AddNotAtBoundary();
            }
            else if (theStatus == SameMaterial) {
              run->AddSameMaterial();
            }
            else if (theStatus == StepTooSmall) {
              run->AddStepTooSmall();
            }
            else if (theStatus == NoRINDEX) {
              run->AddNoRINDEX();
            }
            else if (theStatus == PolishedLumirrorAirReflection) {
              run->AddPolishedLumirrorAirReflection();
            }
            else if (theStatus == PolishedLumirrorGlueReflection) {
              run->AddPolishedLumirrorGlueReflection();
            }
            else if (theStatus == PolishedAirReflection) {
              run->AddPolishedAirReflection();
            }
            else if (theStatus == PolishedTeflonAirReflection) {
              run->AddPolishedTeflonAirReflection();
            }
            else if (theStatus == PolishedTiOAirReflection) {
              run->AddPolishedTiOAirReflection();
            }
            else if (theStatus == PolishedTyvekAirReflection) {
              run->AddPolishedTyvekAirReflection();
            }
            else if (theStatus == PolishedVM2000AirReflection) {
              run->AddPolishedVM2000AirReflection();
            }
            else if (theStatus == PolishedVM2000GlueReflection) {
              run->AddPolishedVM2000AirReflection();
            }
            else if (theStatus == EtchedLumirrorAirReflection) {
              run->AddEtchedLumirrorAirReflection();
            }
            else if (theStatus == EtchedLumirrorGlueReflection) {
              run->AddEtchedLumirrorGlueReflection();
            }
            else if (theStatus == EtchedAirReflection) {
              run->AddEtchedAirReflection();
            }
            else if (theStatus == EtchedTeflonAirReflection) {
              run->AddEtchedTeflonAirReflection();
            }
            else if (theStatus == EtchedTiOAirReflection) {
              run->AddEtchedTiOAirReflection();
            }
            else if (theStatus == EtchedTyvekAirReflection) {
              run->AddEtchedTyvekAirReflection();
            }
            else if (theStatus == EtchedVM2000AirReflection) {
              run->AddEtchedVM2000AirReflection();
            }
            else if (theStatus == EtchedVM2000GlueReflection) {
              run->AddEtchedVM2000AirReflection();
            }
            else if (theStatus == GroundLumirrorAirReflection) {
              run->AddGroundLumirrorAirReflection();
            }
            else if (theStatus == GroundLumirrorGlueReflection) {
              run->AddGroundLumirrorGlueReflection();
            }
            else if (theStatus == GroundAirReflection) {
              run->AddGroundAirReflection();
            }
            else if (theStatus == GroundTeflonAirReflection) {
              run->AddGroundTeflonAirReflection();
            }
            else if (theStatus == GroundTiOAirReflection) {
              run->AddGroundTiOAirReflection();
            }
            else if (theStatus == GroundTyvekAirReflection) {
              run->AddGroundTyvekAirReflection();
            }
            else if (theStatus == GroundVM2000AirReflection) {
              run->AddGroundVM2000AirReflection();
            }
            else if (theStatus == GroundVM2000GlueReflection) {
              run->AddGroundVM2000AirReflection();
            }
            else if (theStatus == Dichroic) {
              run->AddDichroic();
            }
            
            else {
              G4cout << "theStatus: " << theStatus 
                     << " was none of the above." << G4endl;
            }

          }
        }
      }
    }
  }

  else { // particle != opticalphoton
    // print how many Cerenkov and scint photons produced this step
    // this demonstrates use of GetNumPhotons()
    auto proc_man = track->GetDynamicParticle()->GetParticleDefinition()
                         ->GetProcessManager();
    G4int n_proc = proc_man->GetPostStepProcessVector()->entries();
    G4ProcessVector* proc_vec = proc_man->GetPostStepProcessVector(typeDoIt);

    G4int n_scint = 0;
    G4int n_cer   = 0;
    for (G4int i = 0; i < n_proc; ++i) {
      if ((*proc_vec)[i]->GetProcessName().compare("Cerenkov") == 0) {
        auto cer = (G4Cerenkov*)(*proc_vec)[i];
        n_cer = cer->GetNumPhotons();
      }
      else if ((*proc_vec)[i]->GetProcessName().compare("Scintillation") == 0) {
        auto scint = (G4Scintillation*)(*proc_vec)[i];
        n_scint = scint->GetNumPhotons();
      }
    }
    if (fVerbose > 0) {
      if (n_cer > 0 || n_scint > 0) {
        G4cout << "In this step, " << n_cer
               << " Cerenkov and " << n_scint
               << " scintillation photons were produced." << G4endl;
      }
    }

    // loop over secondaries, create statistics
    const std::vector<const G4Track*>* secondaries =
                                step->GetSecondaryInCurrentStep();

    for (auto sec : *secondaries) {
      if (sec->GetDynamicParticle()->GetParticleDefinition() == opticalphoton){
        if (sec->GetCreatorProcess()->GetProcessName().compare("Cerenkov")==0){
          G4double en = sec->GetKineticEnergy();
          run->AddCerenkovEnergy(en);
          run->AddCerenkov();
        //  G4AnalysisManager::Instance()->FillH1(1, en/eV);
        }
        else if (sec->GetCreatorProcess()
                    ->GetProcessName().compare("Scintillation") == 0) {
          G4double en = sec->GetKineticEnergy();
          run->AddScintillationEnergy(en);
          run->AddScintillation();
        //  G4AnalysisManager::Instance()->FillH1(2, en/eV);

       //   G4double time = sec->GetGlobalTime();
       //   analysisMan->FillH1(13, time/ns);
        }
      }
    }
  } 

  return;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
