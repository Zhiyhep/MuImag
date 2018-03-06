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
//Event action for muon imaging simulation

#include "MuEventAction.hh"
#include "MuRunAction.hh"
#include "MuAnalysis.hh"
#include "MuSteppingAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include "G4SystemOfUnits.hh"
#include "MuDetectorConstruction.hh"
#include <vector>
#include "PrimaryHits.hh"
#include "TVector3.h"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuEventAction::MuEventAction(const MuDetectorConstruction* detectorConstruction, MuRunAction *aRun)
 : G4UserEventAction(), fDetConstruction(detectorConstruction),fRunAction(aRun)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuEventAction::~MuEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuEventAction::BeginOfEventAction(const G4Event* event)
{  
  cout << "Begin of Event " << event->GetEventID() << endl;
  fEdep.clear();
  fTime.clear();
  fPosX.clear();
  fPosY.clear();
  fPosZ.clear();
  fDetName.clear();
  fNhits = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuEventAction::EndOfEventAction(const G4Event* event)
{
  auto eventID = event->GetEventID();
  cout << "End of Event " << eventID << endl;
  // Accumulate statistics
  G4PrimaryVertex* gpVertex = event->GetPrimaryVertex();
  G4PrimaryParticle* gpParticle = gpVertex->GetPrimary();
  G4ThreeVector ParticleGunDirection = gpParticle->GetMomentumDirection();
  ParticleGunDirection = ParticleGunDirection.unit();
  G4double PrimaryEnergy = gpParticle->GetKineticEnergy();
  G4ThreeVector ParticleGunPosition = gpVertex->GetPosition();
  G4double t0 = gpVertex->GetT0(); // Time point of primary particle generation
  if(fNhits > 0){  
  	fRunAction->phit->fParticleGunPosition.SetXYZ(ParticleGunPosition.x()/cm,ParticleGunPosition.y()/cm,ParticleGunPosition.z()/cm);
  	fRunAction->phit->fParticleGunDirection.SetXYZ(ParticleGunDirection.x(),ParticleGunDirection.y(),ParticleGunDirection.z());
  	fRunAction->phit->fPrimaryEnergy = PrimaryEnergy/GeV;
  	fRunAction->phit->fNhits = fNhits;
  	fRunAction->phit->fEdep.assign(fEdep.begin(), fEdep.end());
  	fRunAction->phit->fTime.assign(fTime.begin(), fTime.end());
  	fRunAction->phit->fPosX.assign(fPosX.begin(), fPosX.end());
  	fRunAction->phit->fPosY.assign(fPosY.begin(), fPosY.end());
  	fRunAction->phit->fPosZ.assign(fPosZ.begin(), fPosZ.end());
    fRunAction->phit->fDetName.assign(fDetName.begin(),fDetName.end());
  	fRunAction->tree->Fill();
   }
  // Print per event (modulo n)
  //
  G4int printModulo = 10;//G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    G4cout << "ParticleGunPosition: "<<G4BestUnit(ParticleGunPosition.x(),"Length")<<", "<<G4BestUnit(ParticleGunPosition.y(),"Length")<<", "<<G4BestUnit(ParticleGunPosition.z(),"Length")<<") "<<G4endl;
    G4cout << "ParticleGunDirection: "<<ParticleGunDirection.x()<<", "<<ParticleGunDirection.y()<<", "<<ParticleGunDirection.z()<<") "<<G4endl;
    G4cout <<"Primary Particle Energy: "<<G4BestUnit(PrimaryEnergy,"Energy")<<G4endl;   
    G4cout <<"Number of Hits: "<< fNhits << G4endl;
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
