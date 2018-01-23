//Stepping action of muon imaging simulation

#ifndef MuSteppingAction_h
#define MuSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <vector>

class MuDetectorConstruction;
class MuEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in B4aEventAction.

class MuSteppingAction : public G4UserSteppingAction
{
public:
  MuSteppingAction(const MuDetectorConstruction* detectorConstruction,
                    MuEventAction* eventAction);
  virtual ~MuSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const MuDetectorConstruction* fDetConstruction;
  MuEventAction*  fEventAction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
