// final version 7.2 by A.Coban - 21.07.2020
//
/// \file optical/OpNovice2/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetectorMessenger(nullptr)
{
 fExpHall_x = fExpHall_y = fExpHall_z = 30*cm;
  fScintillator_xy    = 100.0*mm; 
  fScintillator_z    = 5.0*mm;

  fScintillator = nullptr;

  fScintillatorMPT    = new G4MaterialPropertiesTable();
  fWorldMPT   = new G4MaterialPropertiesTable();
  fSurfaceMPT = new G4MaterialPropertiesTable();

  fSurface = new G4OpticalSurface("Surface");
  fSurface->SetType(dielectric_dielectric);
  fSurface->SetFinish(ground);
  fSurface->SetModel(unified);
  fSurface->SetMaterialPropertiesTable(fSurfaceMPT);

  fScintillator_LV  = nullptr;
  fWorld_LV = nullptr;

  fScintillatorMaterial  = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  fScintillatorMaterial->SetMaterialPropertiesTable(fScintillatorMPT);
  fScintillatorMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);

//-------Materials------
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;
G4double density;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------
/*
  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(6);
  
  density = 1.060*g/cm3;

  fSilicone = fNistMan->
          ConstructNewMaterial("Silicone", elements, natoms, density);

  elements.clear();
  natoms.clear();*/
  G4Element* H = new G4Element("H", "H", 1., 1.01*g/mole);
  G4Element* C = new G4Element("C", "C", 6., 12.01*g/mole);
G4Material*    fGreaseMaterial= new G4Material("Silicone", density= 1.032*g/cm3, 2);
    fGreaseMaterial->AddElement(C,9);
    fGreaseMaterial->AddElement(H,10);


  //
  // ------------ Generate & Add Material Properties Table ------------
  //

  G4double photonEnergy[] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  G4double refractiveIndex[] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

  assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);




  G4double absClad[] =
  {20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};

  assert(sizeof(absClad) == sizeof(photonEnergy));

  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------

   G4double refractiveIndexSilicone[] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

   assert(sizeof(refractiveIndexSilicone) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->
           AddProperty("RINDEX",photonEnergy,refractiveIndexSilicone,nEntries);
  mptSilicone->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fGreaseMaterial->SetMaterialPropertiesTable(mptSilicone);





/////////////////////////////////////////////////////////////////////////////////////////////////////7/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------------------------------------------
  //  BC408
  //--------------------------------------------------

  density = 1.032*g/cm3;
    fBC408= new G4Material("BC408", density= 1.032*g/cm3, 2);
    fBC408->AddElement(C,9);
    fBC408->AddElement(H,10);

G4double EnergyBC408[] = 
{3.299*eV, 3.201*eV, 3.130*eV, 3.089*eV, 3.069*eV, 3.056*eV, 3.036*eV, 3.023*eV, 3.016*eV, 3.004*eV, 2.991*eV, 2.972*eV, 2.947*eV, 2.923*eV, 2.905*eV, 2.887*eV, 2.863*eV, 2.852*eV, 2.829*eV, 2.818*eV, 2.818*eV, 2.795*eV, 2.768*eV, 2.736*eV, 2.699*eV, 2.658*eV, 2.638*eV, 2.599*eV, 2.562*eV, 2.511*eV, 2.455*eV, 2.421*eV, 2.384*eV};
const G4int nEntriesBC408 = sizeof(EnergyBC408)/sizeof(G4double);


G4double refractiveIndexBC408[] =
{ 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58};
assert(sizeof(refractiveIndexBC408) == sizeof(EnergyBC408));


G4double absBC408[] =
{210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm,210*cm};
assert(sizeof(absBC408) == sizeof(EnergyBC408));


G4double scintillationFastBC408[] =
{0.057, 0.097, 0.163, 0.260, 0.326, 0.396, 0.449, 0.520, 0.564, 0.648, 0.714, 0.793, 0.877, 0.943, 0.978, 0.996, 0.965, 0.930, 0.833, 0.753, 0.705, 0.599, 0.537, 0.476, 0.388, 0.282, 0.242, 0.181, 0.141, 0.088, 0.048, 0.035, 0.022}; 
assert(sizeof(scintillationFastBC408) == sizeof(EnergyBC408));

// Add entries into properties table
G4MaterialPropertiesTable* BC408 = new G4MaterialPropertiesTable();
BC408->AddProperty("RINDEX",EnergyBC408,refractiveIndexBC408,nEntriesBC408);
BC408->AddProperty("ABSLENGTH",EnergyBC408,absBC408,nEntriesBC408);
BC408->AddProperty("FASTCOMPONENT",EnergyBC408, scintillationFastBC408,nEntriesBC408);
//BC408->AddProperty("SLOWCOMPONENT",EnergyBC408, scintillationSlowBC408,nEntriesBC408);
BC408->AddConstProperty("FASTTIMECONSTANT", 0.9*ns);
BC408->AddConstProperty("SLOWTIMECONSTANT", 2.1*ns);
BC408->AddConstProperty("SCINTILLATIONYIELD",10240./MeV);
BC408->AddConstProperty("YIELDRATIO",1.0);
BC408->AddConstProperty("RESOLUTIONSCALE",1.0);

fBC408->SetMaterialPropertiesTable(BC408);
// Set the Birks Constant for the BC408 scintillator
fBC408->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


G4NistManager* man = G4NistManager::Instance();
G4Material *fMylar = man->FindOrBuildMaterial("G4_MYLAR");
G4Material *fSiPMMaterial = man->FindOrBuildMaterial("G4_Si");







  // ---------------------- Volumes ------------------------
  // The experimental Hall
  G4Box* world_box = new G4Box("World", fExpHall_x, fExpHall_y, fExpHall_z);

  fWorld_LV
    = new G4LogicalVolume(world_box, fWorldMaterial, "World", 0, 0, 0);

  G4VPhysicalVolume* world_PV
    = new G4PVPlacement(0, G4ThreeVector(), fWorld_LV, "World", 0, false, 0);
//-----------------------------------------------------------

//----------------------Mylar---------------------------


G4double mylarX = 100.03 * mm;
G4double mylarY = 100.03 * mm;
G4double	mylarThickness = 5.03 * mm;

	G4Box* solidMylar = new G4Box("Mylar",
			mylarX / 2, mylarY / 2,
			mylarThickness / 2);

	G4LogicalVolume* logicMylar=	 new G4LogicalVolume(solidMylar, //its solid
						             fMylar,    //its material
							     "Mylar");           //its name

	G4VPhysicalVolume *fFoil = new G4PVPlacement(0,                             //no rotation
				G4ThreeVector(),                  //at (0,0,0)
				logicMylar,                      //its logical volume
				"Mylar",                           //its name
				fWorld_LV,                                //its mother  volume
				false,                            //no boolean operation
				0);                               //copy number

//----------------------------------------------------

	///////////////////////
	//// Holes ///////////
	/////////////////////

	////////////////HOLE SIZE////////////////////////
	G4double holeSizeX = 3.25 * mm;
	G4double holeSizeY = 5 * mm;
	G4double holeSizeZ = 4.5 * mm;
        G4double gapBetweenHoles = 1.75 * mm;
	G4double gapOfBeginning = 6.65 * mm;
	/////////////////////////////////////////////////
//----------------- Scintillator --------------------------
G4Box* solidHole = new G4Box("hole",                            //its name
			holeSizeX / 2, holeSizeY / 2, holeSizeZ / 2);     //its size
	G4Box* solidScintillator = new G4Box("Scintillator",
			fScintillator_xy / 2, fScintillator_xy / 2,
			fScintillator_z / 2);

	G4SubtractionSolid* subtract1 = new G4SubtractionSolid("subt1",
			solidScintillator, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));

	G4SubtractionSolid* subtract2 = new G4SubtractionSolid("subt2", subtract1,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ gapBetweenHoles + holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract3 = new G4SubtractionSolid("subt3", subtract2,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 2 * gapBetweenHoles + 2 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract4 = new G4SubtractionSolid("subt4", subtract3,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 3 * gapBetweenHoles + 3 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract5 = new G4SubtractionSolid("subt5", subtract4,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 4 * gapBetweenHoles + 4 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract6 = new G4SubtractionSolid("subt6", subtract5,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 5 * gapBetweenHoles + 5 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract7 = new G4SubtractionSolid("subt7", subtract6,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 6 * gapBetweenHoles + 6 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract8 = new G4SubtractionSolid("subt8", subtract7,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 7 * gapBetweenHoles + 7 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract9 = new G4SubtractionSolid("subt9", subtract8,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 8 * gapBetweenHoles + 8 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract10 = new G4SubtractionSolid("subt10", subtract9,
			solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 9 * gapBetweenHoles + 9 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract11 = new G4SubtractionSolid("subt11",
			subtract10, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 10 * gapBetweenHoles + 10 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract12 = new G4SubtractionSolid("subt12",
			subtract11, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 11 * gapBetweenHoles + 11 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract13 = new G4SubtractionSolid("subt13",
			subtract12, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 12 * gapBetweenHoles + 12 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract14 = new G4SubtractionSolid("subt14",
			subtract13, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 13 * gapBetweenHoles + 13 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract15 = new G4SubtractionSolid("subt15",
			subtract14, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 14 * gapBetweenHoles + 14 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract16 = new G4SubtractionSolid("subt16",
			subtract15, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 15 * gapBetweenHoles + 15 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract17 = new G4SubtractionSolid("subt17",
			subtract16, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 16 * gapBetweenHoles + 16 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));
	G4SubtractionSolid* subtract18 = new G4SubtractionSolid("subt18",
			subtract17, solidHole, 0,
			G4ThreeVector(
					-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
							+ 17 * gapBetweenHoles + 17 * holeSizeX,
					-fScintillator_xy / 2 + holeSizeY / 2, 0));

////////////////////////y-axes holes/////////////////
		G4Box* solidHole2 = new G4Box("hole2",                            //its name
				holeSizeY / 2, holeSizeX / 2, holeSizeZ / 2);     //its size
////////////////////////end - y-axes holes/////////////////


	G4SubtractionSolid* subtract19 = new G4SubtractionSolid("subt19",
				subtract18, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning, 0));

	G4SubtractionSolid* subtract20 = new G4SubtractionSolid("subt20",
				subtract19, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ gapBetweenHoles + holeSizeX, 0));

	G4SubtractionSolid* subtract21 = new G4SubtractionSolid("subt21",
				subtract20, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 2*gapBetweenHoles + 2*holeSizeX, 0));

	G4SubtractionSolid* subtract22 = new G4SubtractionSolid("subt22",
				subtract21, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 3*gapBetweenHoles + 3*holeSizeX, 0));

	G4SubtractionSolid* subtract23 = new G4SubtractionSolid("subt23",
				subtract22, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 4*gapBetweenHoles + 4*holeSizeX, 0));

	G4SubtractionSolid* subtract24 = new G4SubtractionSolid("subt24",
				subtract23, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 5*gapBetweenHoles + 5*holeSizeX, 0));

	G4SubtractionSolid* subtract25 = new G4SubtractionSolid("subt25",
				subtract24, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 6*gapBetweenHoles + 6*holeSizeX, 0));

	G4SubtractionSolid* subtract26 = new G4SubtractionSolid("subt26",
				subtract25, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 7*gapBetweenHoles + 7*holeSizeX, 0));

	G4SubtractionSolid* subtract27 = new G4SubtractionSolid("subt27",
				subtract26, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 8*gapBetweenHoles + 8*holeSizeX, 0));

	G4SubtractionSolid* subtract28 = new G4SubtractionSolid("subt28",
				subtract27, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 9*gapBetweenHoles + 9*holeSizeX, 0));

	G4SubtractionSolid* subtract29 = new G4SubtractionSolid("subt29",
				subtract28, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 10*gapBetweenHoles + 10*holeSizeX, 0));

	G4SubtractionSolid* subtract30 = new G4SubtractionSolid("subt30",
				subtract29, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 11*gapBetweenHoles + 11*holeSizeX, 0));

	G4SubtractionSolid* subtract31 = new G4SubtractionSolid("subt31",
				subtract30, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 12*gapBetweenHoles + 12*holeSizeX, 0));

	G4SubtractionSolid* subtract32 = new G4SubtractionSolid("subt32",
				subtract31, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 13*gapBetweenHoles + 13*holeSizeX, 0));

	G4SubtractionSolid* subtract33 = new G4SubtractionSolid("subt33",
				subtract32, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 14*gapBetweenHoles + 14*holeSizeX, 0));

	G4SubtractionSolid* subtract34 = new G4SubtractionSolid("subt34",
				subtract33, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 15*gapBetweenHoles + 15*holeSizeX, 0));

	G4SubtractionSolid* subtract35 = new G4SubtractionSolid("subt35",
				subtract34, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 16*gapBetweenHoles + 16*holeSizeX, 0));

	G4SubtractionSolid* subtract36 = new G4SubtractionSolid("subt36",
				subtract35, solidHole2, 0,
				G4ThreeVector(
						fScintillator_xy / 2 - holeSizeY / 2 ,
						-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
						+ 17*gapBetweenHoles + 17*holeSizeX, 0));



	fScintillator_LV = new G4LogicalVolume(subtract36, fBC408,
			"Scintillator");

	fScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
			fScintillator_LV, "Scintillator", logicMylar, false, 0);


//--------------end Holes and Scint------------------------------------

	//
	// Optical Grease

	G4double innerRadiusOfTheTube = 0. * mm;
	G4double outerRadiusOfTheTube = 0.6 * mm;
	G4double hightOfTheTube = 0.5 * mm;
	G4double hightOfTheTube2 = 5 * um;
	G4double startAngleOfTheTube = 0. * deg;
	G4double spanningAngleOfTheTube = 360. * deg;

	///////////////////// Grease Geometry//////////////////////////////
	G4Tubs* SolidGrease = new G4Tubs("Grease", innerRadiusOfTheTube,
			outerRadiusOfTheTube, hightOfTheTube2 / 2, startAngleOfTheTube,
			spanningAngleOfTheTube);

	fLogicGrease = new G4LogicalVolume(SolidGrease, fGreaseMaterial, "Grease");
	///////////////////// end - Grease Geometry//////////////////////////////

	///////////////////// SiPM Geometry//////////////////////////////
	G4Tubs* SolidSiPM = new G4Tubs("SiPM", innerRadiusOfTheTube, outerRadiusOfTheTube,
				hightOfTheTube / 2, startAngleOfTheTube, spanningAngleOfTheTube);

		fLogicSiPM = new G4LogicalVolume(SolidSiPM, fSiPMMaterial, "SiPM");
	///////////////////// end - SiPM Geometry//////////////////////////////

		////////////// 18 volume placement in Scintilator /////////////
	G4int nOfSolids = 18;
	for (G4int k = 1; k <= nOfSolids; k++) {

		G4String volNameGrease;
		G4String volNameSiPM;
		switch (k) {
		case 1:
			volNameGrease = "Grease1";
			volNameSiPM = "SiPM1";
			break;
		case 2:
			volNameGrease = "Grease2";
			volNameSiPM = "SiPM2";
			break;
		case 3:
			volNameGrease = "Grease3";
			volNameSiPM = "SiPM3";
			break;
		case 4:
			volNameGrease = "Grease4";
			volNameSiPM = "SiPM4";
			break;
		case 5:
			volNameGrease = "Grease5";
			volNameSiPM = "SiPM5";
			break;
		case 6:
			volNameGrease = "Grease6";
			volNameSiPM = "SiPM6";
			break;
		case 7:
			volNameGrease = "Grease7";
			volNameSiPM = "SiPM7";
			break;
		case 8:
			volNameGrease = "Grease8";
			volNameSiPM = "SiPM8";
			break;
		case 9:
			volNameGrease = "Grease9";
			volNameSiPM = "SiPM9";
			break;
		case 10:
			volNameGrease = "Grease10";
			volNameSiPM = "SiPM10";
			break;
		case 11:
			volNameGrease = "Grease11";
			volNameSiPM = "SiPM11";
			break;
		case 12:
			volNameGrease = "Grease12";
			volNameSiPM = "SiPM12";
			break;
		case 13:
			volNameGrease = "Grease13";
			volNameSiPM = "SiPM13";
			break;
		case 14:
			volNameGrease = "Grease14";
			volNameSiPM = "SiPM14";
			break;
		case 15:
			volNameGrease = "Grease15";
			volNameSiPM = "SiPM15";
			break;
		case 16:
			volNameGrease = "Grease16";
			volNameSiPM = "SiPM16";
			break;
		case 17:
			volNameGrease = "Grease17";
			volNameSiPM = "SiPM17";
			break;
		case 18:
			volNameGrease = "Grease18";
			volNameSiPM = "SiPM18";
			break;

		}
	////////////////////// Grease Placement///////////////////////
		G4RotationMatrix rotm2 = G4RotationMatrix();
				rotm2.rotateX(90 * deg);
				G4Transform3D transform2 = G4Transform3D(rotm2,
						G4ThreeVector(
								-fScintillator_xy / 2 + holeSizeX / 2
										+ gapOfBeginning + (k - 1) * gapBetweenHoles
										+ (k - 1) * holeSizeX,
								-fScintillator_xy / 2 + holeSizeY
										- hightOfTheTube2 / 2, 0));

		fgreasePV = new G4PVPlacement(transform2, fLogicGrease, volNameGrease, logicMylar, false,
				0);
		////////////////////// SiPM Placement///////////////////////
	G4ThreeVector position = G4ThreeVector(
			-fScintillator_xy / 2 + holeSizeX / 2
											+ gapOfBeginning + (k - 1) * gapBetweenHoles
											+ (k - 1) * holeSizeX,
			-fScintillator_xy / 2 + holeSizeY - hightOfTheTube2
					- hightOfTheTube / 2, 0);

	G4RotationMatrix rotm = G4RotationMatrix();
	rotm.rotateX(90 * deg);
	G4Transform3D transform = G4Transform3D(rotm, position);

	 new G4PVPlacement(transform,   //rotated
			fLogicSiPM,               //its logical volume
			volNameSiPM,                  //its name
			logicMylar,             //its mother
			false,                   //no boulean operat
			0);                      //copy number
}

//////////////////////
	/////19-36

	G4int nOfSolidsL = 18;
	for (G4int l = 1; l <= nOfSolidsL; l++) {

		G4String volNameGrease;
		G4String volNameSiPM;
		switch (l) {
		case 1:
			volNameGrease = "Grease19";
			volNameSiPM = "SiPM19";
			break;
		case 2:
			volNameGrease = "Grease20";
			volNameSiPM = "SiPM20";
			break;
		case 3:
			volNameGrease = "Grease21";
			volNameSiPM = "SiPM21";
			break;
		case 4:
			volNameGrease = "Grease22";
			volNameSiPM = "SiPM22";
			break;
		case 5:
			volNameGrease = "Grease23";
			volNameSiPM = "SiPM23";
			break;
		case 6:
			volNameGrease = "Grease24";
			volNameSiPM = "SiPM24";
			break;
		case 7:
			volNameGrease = "Grease25";
			volNameSiPM = "SiPM25";
			break;
		case 8:
			volNameGrease = "Grease26";
			volNameSiPM = "SiPM26";
			break;
		case 9:
			volNameGrease = "Grease27";
			volNameSiPM = "SiPM27";
			break;
		case 10:
			volNameGrease = "Grease28";
			volNameSiPM = "SiPM28";
			break;
		case 11:
			volNameGrease = "Grease29";
			volNameSiPM = "SiPM29";
			break;
		case 12:
			volNameGrease = "Grease30";
			volNameSiPM = "SiPM30";
			break;
		case 13:
			volNameGrease = "Grease31";
			volNameSiPM = "SiPM31";
			break;
		case 14:
			volNameGrease = "Grease32";
			volNameSiPM = "SiPM32";
			break;
		case 15:
			volNameGrease = "Grease33";
			volNameSiPM = "SiPM33";
			break;
		case 16:
			volNameGrease = "Grease34";
			volNameSiPM = "SiPM34";
			break;
		case 17:
			volNameGrease = "Grease35";
			volNameSiPM = "SiPM35";
			break;
		case 18:
			volNameGrease = "Grease36";
			volNameSiPM = "SiPM36";
			break;

		}

////////////////////// Grease Placement///////////////////////
		G4RotationMatrix rotm3 = G4RotationMatrix();
				rotm3.rotateY(90 * deg);
				G4Transform3D transform3 = G4Transform3D(rotm3,
						G4ThreeVector(fScintillator_xy / 2 - holeSizeY + hightOfTheTube2 / 2,
										-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
										+ (l-1)*gapBetweenHoles + (l-1)*holeSizeX, 0));

		new G4PVPlacement(transform3, fLogicGrease, volNameGrease, logicMylar, false,
				0);
		////////////////////// SiPM Placement///////////////////////


	G4RotationMatrix rotm4 = G4RotationMatrix();
	rotm4.rotateY(90 * deg);
	G4Transform3D transform4 = G4Transform3D(rotm4, G4ThreeVector(fScintillator_xy / 2 - holeSizeY + hightOfTheTube2 
	  +  hightOfTheTube / 2,
			-fScintillator_xy / 2 + holeSizeX / 2 + gapOfBeginning
			+ (l-1)*gapBetweenHoles + (l-1)*holeSizeX, 0));

	 new G4PVPlacement(transform4,   //rotated
			fLogicSiPM,               //its logical volume
			volNameSiPM,                  //its name
			logicMylar,             //its mother
			false,                   //no boulean operat
			0);                      //copy number
	}


  // ------------- Surface --------------

  G4LogicalBorderSurface* surface =
          new G4LogicalBorderSurface("Surface",
                                 fScintillator, fFoil, fSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
        (surface->GetSurface(fScintillator,fFoil)->GetSurfaceProperty());
  G4cout << "******  opticalSurface->DumpInfo:" << G4endl;
  if (opticalSurface) { opticalSurface->DumpInfo(); }
  G4cout << "******  end of opticalSurface->DumpInfo" << G4endl;

  return world_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetSurfaceSigmaAlpha(G4double v) {
  fSurface->SetSigmaAlpha(v);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Surface sigma alpha set to: " << fSurface->GetSigmaAlpha()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetSurfacePolish(G4double v) {
  fSurface->SetPolish(v);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Surface polish set to: " << fSurface->GetPolish()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddScintillatorMPV(const char* c,
                                     G4MaterialPropertyVector* mpv) {
  fScintillatorMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the box is now: " << G4endl;
  fScintillatorMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPV(const char* c,
                                       G4MaterialPropertyVector* mpv) {
  fWorldMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddSurfaceMPV(const char* c,
                                         G4MaterialPropertyVector* mpv) {
  fSurfaceMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the surface is now: " << G4endl;
  fSurfaceMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddScintillatorMPC(const char* c, G4double v) {
  fScintillatorMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the box is now: " << G4endl;
  fScintillatorMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPC(const char* c, G4double v) {
  fWorldMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddSurfaceMPC(const char* c, G4double v) {
  fSurfaceMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the surface is now: " << G4endl;
  fSurfaceMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetWorldMaterial(const G4String& mat) {
  G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  if (pmat && fWorldMaterial != pmat) {
    fWorldMaterial = pmat;
    if (fWorld_LV) {
      fWorld_LV->SetMaterial(fWorldMaterial);
      fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);
    }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4cout << "World material set to " << fWorldMaterial->GetName()
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetScintillatorMaterial(const G4String& mat) {
  G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  if (pmat && fScintillatorMaterial != pmat) {
    fScintillatorMaterial = pmat;
    if (fScintillator_LV) {
      fScintillator_LV->SetMaterial(fScintillatorMaterial);
      fScintillatorMaterial->SetMaterialPropertiesTable(fScintillatorMPT);
      fScintillatorMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4cout << "Scintillator material set to " << fScintillatorMaterial->GetName()
           << G4endl;
  }
}
