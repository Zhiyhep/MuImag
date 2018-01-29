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
// Run action of muon imaging simulation

#include "MuRunAction.hh"
#include "MuAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "MuDetectorConstruction.hh"
#ifdef __CINT__
#else
#include "PrimaryHits.hh"
#endif

class PrimaryHits;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuRunAction::MuRunAction(G4String name, const MuDetectorConstruction* detectorConstruction)
 : G4UserRunAction(),
   fDetConstruction(detectorConstruction)
{ 
   file = new TFile(name, "recreate");
   tree = new TTree("G4Event","Primary Events");

   G4RunManager::GetRunManager()->SetPrintProgress(1);
   phit = new PrimaryHits;
   tree->Branch("event", "PrimaryHits", &phit); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuRunAction::~MuRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  G4cout << "Begin of Run Action" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4cout << "End of Run Action" << G4endl;
  phit->Clean();
  file->Write("",TObject::kOverwrite);
//  file->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
