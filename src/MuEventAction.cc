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
  if(fNhits > 0){  
  	fRunAction->phit->fParticleGunPosition.SetXYZ(ParticleGunPosition.x(),ParticleGunPosition.y(),ParticleGunPosition.z());
  	fRunAction->phit->fParticleGunDirection.SetXYZ(ParticleGunDirection.x(),ParticleGunDirection.y(),ParticleGunDirection.z());
  	fRunAction->phit->fPrimaryEnergy = PrimaryEnergy;
  	fRunAction->phit->fNhits = fNhits;
  	fRunAction->phit->fEdep.assign(fEdep.begin(), fEdep.end());
  	fRunAction->phit->fTime.assign(fTime.begin(), fTime.end());
  	fRunAction->phit->fPosX.assign(fPosX.begin(), fPosX.end());
  	fRunAction->phit->fPosY.assign(fPosY.begin(), fPosY.end());
  	fRunAction->phit->fPosZ.assign(fPosZ.begin(), fPosZ.end());
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
