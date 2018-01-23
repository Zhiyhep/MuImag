//Action initialization for muon imaging simulation

#ifndef MuActionInitialization_h
#define MuActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4String.hh"
class MuDetectorConstruction;
class MuRunAction;

/// Action initialization class.
///

class MuActionInitialization : public G4VUserActionInitialization
{
  public:
    MuActionInitialization(G4String gdml, MuDetectorConstruction*);
    virtual ~MuActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    MuDetectorConstruction* fDetConstruction;
    G4String fGdml;
};

#endif

    
