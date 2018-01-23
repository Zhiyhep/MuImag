//Action initialization of muon imaging simulation

#include "MuActionInitialization.hh"
#include "MuPrimaryGeneratorAction.hh"
#include "MuRunAction.hh"
#include "MuEventAction.hh"
#include "MuSteppingAction.hh"
#include "MuDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuActionInitialization::MuActionInitialization
                            (G4String gdml, MuDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{
   fGdml = gdml;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuActionInitialization::~MuActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuActionInitialization::BuildForMaster() const
{
  SetUserAction(new MuRunAction(fGdml, fDetConstruction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuActionInitialization::Build() const
{
  SetUserAction(new MuPrimaryGeneratorAction);

  MuRunAction *runAction = new MuRunAction(fGdml, fDetConstruction);
  SetUserAction(runAction);

  MuEventAction *eventAction = new MuEventAction(fDetConstruction, runAction);
  SetUserAction(eventAction);
  SetUserAction(new MuSteppingAction(fDetConstruction, eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
