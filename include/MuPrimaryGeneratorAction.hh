//Primary generator action for muon imaging simulation

#ifndef MuPrimaryGeneratorAction_h
#define MuPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4GeneralParticleSource;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class MuPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  MuPrimaryGeneratorAction();    
  virtual ~MuPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4GeneralParticleSource* fParticleGun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
