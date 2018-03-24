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

#include "MuPrimaryGeneratorAction.hh"
#include "MuDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
//#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
TRandom3 my_rand(0);
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPrimaryGeneratorAction::MuPrimaryGeneratorAction(MuDetectorConstruction *det)
 : G4VUserPrimaryGeneratorAction(), fDetectorConstruction(det), fParticleGun(nullptr), fPreParticleTime(0)
{
 // using gps
 fParticleGun = new G4ParticleGun();
 G4double X_Width = fDetectorConstruction->fHalf_Size_X/10;// [cm]
 G4double Y_Width = fDetectorConstruction->fHalf_Size_Y/10;// [cm]
 fTau = 60./(X_Width*Y_Width);// [s] 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MuPrimaryGeneratorAction::~MuPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  SetParticleDefinition();
  SetParticlePosition();
  SetParticleDirection();
  SetParticleEnergy();
  SetParticleTime();
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MuPrimaryGeneratorAction::SetParticleDefinition()
{
    auto mu_plus = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
    auto mu_minus = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    double r = G4UniformRand();
    double fraction = 1;//fraction of mu+
    if(r <= fraction){
         fParticleGun->SetParticleDefinition(mu_plus);
    }
    else{
         fParticleGun->SetParticleDefinition(mu_minus);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void  MuPrimaryGeneratorAction::SetParticlePosition()
{
  fGun_Pos_X = (fDetectorConstruction->fHalf_Size_X)*(2*G4UniformRand()-1)*mm;
  fGun_Pos_Y = (fDetectorConstruction->fHalf_Size_Y)*(2*G4UniformRand()-1)*mm;
  fGun_Pos_Z = ((fDetectorConstruction->fHalf_Size_Z)+1)*mm;      
  fParticleGun->SetParticlePosition(G4ThreeVector(fGun_Pos_X, fGun_Pos_Y, fGun_Pos_Z));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MuPrimaryGeneratorAction::SetParticleDirection()
{
  // f(\theta) = A*sin(\theta)*cos^2(\theta) (0 <= \theta <= pi/2)
  G4double rTheta = TMath::ACos(TMath::Power(G4UniformRand(),1./3.));
  fGun_Dir_Z = -TMath::Cos(rTheta);
  // f(\phi) = 1 (0 <= \phi 2*pi)
  G4double rPhi = G4UniformRand()*2*(TMath::Pi());
  fGun_Dir_X = TMath::Sin(rTheta)*TMath::Cos(rPhi);
  fGun_Dir_Y = TMath::Sin(rTheta)*TMath::Sin(rPhi);
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(fGun_Dir_X,fGun_Dir_Y,fGun_Dir_Z));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MuPrimaryGeneratorAction::SetParticleEnergy()
{
  G4double Theta = TMath::ACos(-fGun_Dir_Z);
  G4double max, E;
  E = 0.1+2.6;
  max = 1.8e-6*TMath::Power(E,-2.7)*(90*TMath::Power(0.78,1.7)/(90-E*fGun_Dir_Z)+0.36*0.635*442*TMath::Power(0.52,1.7)/(442-E*fGun_Dir_Z))/(6.2*E); 
  G4double energy;
  while(true){
    G4double randxerg, randyerg;
    randxerg = G4UniformRand();
    randyerg = G4UniformRand();
    G4double Ex = 0.1+(1000-0.1)*randxerg+2.6;
    G4double Ey = 1.8e-6*TMath::Power(Ex,-2.7)*(90*TMath::Power(0.78,1.7)/(90-Ex*fGun_Dir_Z)+0.36*0.635*442*TMath::Power(0.52,1.7)/(442-Ex*fGun_Dir_Z))/(6.2*Ex)*TMath::Sin(Theta);
    if(randyerg <= Ey/max){
       energy = Ex;
       break;
    }
  }
  fParticleGun->SetParticleEnergy(energy*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MuPrimaryGeneratorAction::SetParticleTime()
{
  fParticleGun->SetParticleTime(fPreParticleTime);   
  fPreParticleTime += my_rand.Exp(fTau);
}
