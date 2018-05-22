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
//Detector construction of muon imaging simulation

#ifndef MuDetectorConstruction_h
#define MuDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4GDMLParser.hh"
#include "G4String.hh"
#include "G4UserLimits.hh"

class G4VPhysicalVolume;
class G4VLogicalVolume;
class G4GlobalMagFieldMessenger;
class GarfieldG4FastSimulationModel;

class MuDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MuDetectorConstruction(G4String gdml);
    virtual ~MuDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    // Half size of object Area
    G4double fHalf_Size_X; // [mm]
    G4double fHalf_Size_Y;
    G4double fHalf_Size_Z;

    // Half size of detector
    G4double fDet_Half_Z;

    GarfieldG4FastSimulationModel* fGarfieldG4FastSimulationModel;

  private:
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
    G4String fGdml;
    G4GDMLParser* fParser; 
    G4UserLimits* fStepLimit;
    G4bool fCheckOverlaps;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

