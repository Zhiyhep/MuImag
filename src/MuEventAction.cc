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
//Event action for muon imaging simulation

#include "MuEventAction.hh"
#include "MuRunAction.hh"
#include "MuAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>
#include <iostream>
#include "G4SystemOfUnits.hh"
#include "MuDetectorConstruction.hh"
#include <vector>
#include "TVector3.h"
#include "GarfieldG4FastSimulationModel.hh"
#include "MuonEvent.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuEventAction::MuEventAction(const MuDetectorConstruction* detectorConstruction, MuRunAction *aRun)
 : G4UserEventAction(), fDetConstruction(detectorConstruction),fRunAction(aRun)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuEventAction::~MuEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuEventAction::BeginOfEventAction(const G4Event* event)
{  
  cout << "Begin of Event " << event->GetEventID() << endl;
  fDetConstruction->fGarfieldG4FastSimulationModel->muevent->Clean();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuEventAction::EndOfEventAction(const G4Event* event)
{
  auto eventID = event->GetEventID();
  cout << "End of Event " << eventID << endl;
  
  fRunAction->tree->Fill();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
