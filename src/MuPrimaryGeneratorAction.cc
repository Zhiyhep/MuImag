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
//Primary generator action for muon imaging simulation

#include "MuPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPrimaryGeneratorAction::MuPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{
  //using particle gun 
 /* G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  auto particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun->SetParticleEnergy(3.*GeV);
*/
 // using gps
 fParticleGun = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPrimaryGeneratorAction::~MuPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore
  //
  /*
  G4double worldZHalfLength = 0.;
  G4double detectorXHalfLength = 0.;
  G4double detectorYHalfLength = 0.;
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  //auto targetLV = G4LogicalVolumeStore::GetInstance()->GetVolume("TargetLV");
  auto detectorLV = G4LogicalVolumeStore::GetInstance()->GetVolume("DetectorLV");
  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  //G4Box* targetBox = nullptr;
  G4Box* detectorBox = nullptr;
  if (  worldLV && detectorLV) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    detectorBox = dynamic_cast<G4Box*>(detectorLV->GetSolid());
  }

  if ( worldBox && detectorBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();
    detectorXHalfLength = detectorBox->GetXHalfLength();
    detectorYHalfLength = detectorBox->GetYHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("B4PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  } 
  
  // Set gun position
  G4double GunX = detectorXHalfLength*(2*G4UniformRand()-1);
  G4double GunY = detectorYHalfLength*(2*G4UniformRand()-1);
  
  while(GunX*GunX+GunY*GunY>targetRadius*targetRadius){
        GunX = targetRadius*(2*G4UniformRand()-1);
        GunY = targetRadius*(2*G4UniformRand()-1);
  }
   
  fParticleGun
    ->SetParticlePosition(G4ThreeVector(GunX, GunY, worldZHalfLength));*/
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

