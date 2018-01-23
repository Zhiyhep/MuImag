// Event action for muon imaging simulation

#ifndef MuEventAction_h
#define MuEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()
class MuRunAction;
class MuDetectorConstruction;

class MuEventAction : public G4UserEventAction
{
  public:
    MuEventAction(const MuDetectorConstruction*, MuRunAction*);
    virtual ~MuEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    std::vector<G4double> fEdep;
    std::vector<G4double> fTime;
    std::vector<G4double> fPosX;
    std::vector<G4double> fPosY; 
    std::vector<G4double> fPosZ;
    G4int fNhits;
  private:
    const MuDetectorConstruction* fDetConstruction;
    MuRunAction* fRunAction;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
