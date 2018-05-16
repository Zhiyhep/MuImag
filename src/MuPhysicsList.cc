/*
 * MuonPhysicsList.cc
 *
 *  Created on: May 27, 2017
 *      Author: newdriver
 */

#include "MuPhysicsList.hh"
#include "MuDetectorConstruction.hh"

#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LossTableManager.hh"
#include "G4ProductionCutsTable.hh"
#include "G4EmConfigurator.hh"
#include "G4EmParameters.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

MuPhysicsList::MuPhysicsList() : G4VModularPhysicsList()
{
  G4EmParameters::Instance()->SetVerbose(0);

  SetDefaultCutValue(0.1*mm);

  fDecayPhysicsList = new G4DecayPhysics();

  fEmPhysicsList = new G4EmStandardPhysics(0);

  SetVerboseLevel(1);
}

MuPhysicsList::~MuPhysicsList()
{
  delete fDecayPhysicsList;
  delete fEmPhysicsList;
}

void MuPhysicsList::ConstructParticle()
{
  fDecayPhysicsList->ConstructParticle();
}

void MuPhysicsList::ConstructProcess()
{
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  fDecayPhysicsList->ConstructProcess();
  G4EmParameters::Instance()->AddPAIModel("all","world","pai");
}

void MuPhysicsList::SetCuts()
{
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(25*eV, 1e20);
  if(verboseLevel > 0 ){ DumpCutValuesTable();}
}
