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
//Stepping action of muon imaging simulation

#include "MuSteppingAction.hh"
#include "MuEventAction.hh"
#include "MuDetectorConstruction.hh"
#include "MuAnalysis.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuSteppingAction::MuSteppingAction(
                      const MuDetectorConstruction* detectorConstruction,
                      MuEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuSteppingAction::~MuSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track *track = step->GetTrack();
  const G4String particleName = track->GetParticleDefinition()->GetParticleName();
  const G4String logName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  const G4String physName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
  if(logName == "gasVolume" && (particleName == "mu+" || particleName == "mu-")){
        
  	G4double edep = step->GetTotalEnergyDeposit();
  	G4double gtime = track->GetGlobalTime(); // Just fly time here
  	G4ThreeVector pos = track->GetPosition();
  	fEventAction->fEdep.push_back(edep/keV);
  	fEventAction->fTime.push_back(gtime/s);
  	fEventAction->fPosX.push_back(pos.x()/cm);
  	fEventAction->fPosY.push_back(pos.y()/cm);
  	fEventAction->fPosZ.push_back(pos.z()/cm);
        fEventAction->fDetName.push_back(physName);
  	fEventAction->fNhits++;
   }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
