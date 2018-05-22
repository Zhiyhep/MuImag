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
//Detector construction for muon imaging simulation

#include "MuDetectorConstruction.hh"
#include "GarfieldG4FastSimulationModel.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Region.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* MuDetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuDetectorConstruction::MuDetectorConstruction(G4String gdml)
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true)
{
  fGdml = gdml;
  fParser = new G4GDMLParser();
  fParser->Read(fGdml, false);
  fHalf_Size_X = fParser->GetConstant("Plate_Size_X")/2*CLHEP::mm;
  fHalf_Size_Y = fParser->GetConstant("Plate_Size_Y")/2*CLHEP::mm;
  fHalf_Size_Z = (fParser->GetConstant("ObjectAreaWidth")*0.5+fParser->GetConstant("Detector_Size_Z")*2+fParser->GetConstant("Detector_Offset"))*CLHEP::mm;
  fDet_Half_Z = (fParser->GetConstant("Chamber_Size_Z")/2-fParser->GetConstant("Plate_Size_Z"))*CLHEP::mm;
  G4LogicalVolume* gasVolume = fParser->GetVolume("gasVolume");
  G4Region* regionGarfield = new G4Region("RegionGarfield");
  regionGarfield->AddRootLogicalVolume(gasVolume);
  fGarfieldG4FastSimulationModel = new GarfieldG4FastSimulationModel("GarfieldG4FastSimulationModel", regionGarfield, this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuDetectorConstruction::~MuDetectorConstruction()
{
 delete fParser; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* MuDetectorConstruction::Construct()
{
  G4cout << "Detector Construction" << G4endl;
  G4VPhysicalVolume* worldPhy = fParser->GetWorldVolume();
  return worldPhy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MuDetectorConstruction::ConstructSDandField()
{
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

