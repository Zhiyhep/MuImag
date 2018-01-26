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
// Event action for muon imaging simulation

#ifndef MuEventAction_h
#define MuEventAction_h 1

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

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    std::vector<G4double> fEdep;
    std::vector<G4double> fTime;
    std::vector<G4double> fPosX;
    std::vector<G4double> fPosY; 
    std::vector<G4double> fPosZ;
    std::vector<G4String> fDetName;
    G4int fNhits;
  private:
    const MuDetectorConstruction* fDetConstruction;
    MuRunAction* fRunAction;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
