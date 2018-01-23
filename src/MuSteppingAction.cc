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
  const G4String detName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  if(detName == "gasVolume" && (particleName == "mu+" || particleName == "mu-")){
  	G4double edep = step->GetTotalEnergyDeposit();
  	G4double gtime = track->GetGlobalTime();
  	G4ThreeVector pos = track->GetPosition();
  	fEventAction->fEdep.push_back(edep);
  	fEventAction->fTime.push_back(gtime);
  	fEventAction->fPosX.push_back(pos.x());
  	fEventAction->fPosY.push_back(pos.y());
  	fEventAction->fPosZ.push_back(pos.z());
  	fEventAction->fNhits++;
   }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
