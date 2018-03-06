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
  SetUserAction(new MuPrimaryGeneratorAction(fDetConstruction));

  MuRunAction *runAction = new MuRunAction(fGdml, fDetConstruction);
  SetUserAction(runAction);

  MuEventAction *eventAction = new MuEventAction(fDetConstruction, runAction);
  SetUserAction(eventAction);
  SetUserAction(new MuSteppingAction(fDetConstruction, eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
