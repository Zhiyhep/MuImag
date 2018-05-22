#ifndef MuEventAction_H
#define MuEventAction_H 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class MuRunAction;
class MuDetectorConstruction;

class MuEventAction : public G4UserEventAction
{
public:
  MuEventAction(const MuDetectorConstruction*, MuRunAction*);
  virtual ~MuEventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  const MuDetectorConstruction* fDetConstruction;
  MuRunAction* fRunAction;
};

#endif
