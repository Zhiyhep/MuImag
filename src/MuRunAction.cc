// Run action of muon imaging simulation

#include "MuRunAction.hh"
#include "MuAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "MuDetectorConstruction.hh"
#ifdef __CINT__
#else
#include "PrimaryHits.hh"
#endif

class PrimaryHits;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuRunAction::MuRunAction(G4String name, const MuDetectorConstruction* detectorConstruction)
 : G4UserRunAction(),
   fDetConstruction(detectorConstruction)
{ 
   file = new TFile(name, "recreate");
   tree = new TTree("G4Event","Primary Events");

   G4RunManager::GetRunManager()->SetPrintProgress(1);
   phit = new PrimaryHits;
   tree->Branch("event", "PrimaryHits", &phit); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuRunAction::~MuRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  G4cout << "Begin of Run Action" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4cout << "End of Run Action" << G4endl;
  phit->Clean();
  file->Write();
//  file->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
