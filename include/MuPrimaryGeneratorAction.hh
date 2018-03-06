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
//Primary generator action for muon imaging simulation

#ifndef MuPrimaryGeneratorAction_h
#define MuPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4GeneralParticleSource;
class MuDetectorConstruction;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class MuPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  MuPrimaryGeneratorAction(MuDetectorConstruction* det);    
  virtual ~MuPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  
  // set methods
  void SetRandomFlag(G4bool value);
  void SetParticleDefinition();
  void SetParticlePosition();
  void SetParticleDirection();
  void SetParticleEnergy();
  void SetParticleTime();
  G4double fGun_Pos_X, fGun_Pos_Y, fGun_Pos_Z; //[mm]
  G4double fGun_Dir_X, fGun_Dir_Y, fGun_Dir_Z;

private:
  MuDetectorConstruction * fDetectorConstruction;
  G4ParticleGun* fParticleGun;
  G4double fTau; // Average interval between successive muons, [s]
  G4double fPreParticleTime;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
