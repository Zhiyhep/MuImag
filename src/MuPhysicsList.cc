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
// $Id: GarfieldPhysicsList.cc 999997 2015-12-11 14:47:43Z dpfeiffe $
//
/// \file GarfieldPhysicsList.cc
/// \brief Implementation of the GarfieldPhysicsList class

#include "MuPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "QGSP_BERT_HP.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4PAIPhotModel.hh"
#include "G4PAIModel.hh"
#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPhysicsList::MuPhysicsList() :
		G4VModularPhysicsList() {
	G4int verb = 0;
	SetVerboseLevel(verb);
	defaultCutValue = 1 * CLHEP::mm;
	QGSP_BERT_HP *physicsList = new QGSP_BERT_HP;
	for (G4int i = 0;; ++i) {
		G4VPhysicsConstructor* elem =
				const_cast<G4VPhysicsConstructor*>(physicsList->GetPhysics(i));
		if (elem == NULL)
			break;
		G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
		RegisterPhysics(elem);
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuPhysicsList::~MuPhysicsList() {

}

void MuPhysicsList::AddParameterisation() {

	G4FastSimulationManagerProcess* fastSimProcess_garfield =
			new G4FastSimulationManagerProcess("G4FSMP_garfield");

	auto theParticleIterator = GetParticleIterator();
	theParticleIterator->reset();
	while ((*theParticleIterator)()) {

		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4EmConfigurator* config = G4LossTableManager::Instance()->EmConfigurator();
		G4LossTableManager::Instance()->SetVerbose(1);

                pmanager->AddDiscreteProcess(fastSimProcess_garfield);
		G4PAIModel* pai = new G4PAIModel(particle, "G4PAIModel");
		if (particle->GetParticleName() == "e-" || particle->GetParticleName() == "e+") {
		    config->SetExtraEmModel(particle->GetParticleName(), "eIoni", pai, "RegionGarfield", 0 * MeV, 1000. * MeV, pai);
		    } else if (particle->GetParticleName() == "mu-" || particle->GetParticleName() == "mu+") {
		    config->SetExtraEmModel(particle->GetParticleName(), "muIoni", pai, "RegionGarfield", 0 * MeV, 1e6 * MeV, pai);
		    } else if (particle->GetParticleName() == "proton" 
			       || particle->GetParticleName() == "pi+" || particle->GetParticleName() == "pi-") {
		    config->SetExtraEmModel(particle->GetParticleName(), "hIoni", pai, "RegionGarfield", 0 * MeV, 100. * MeV, pai);
                    } else if (particle->GetParticleName() == "alpha"
			       || particle->GetParticleName() == "He3"
			       || particle->GetParticleName() == "GenericIon") {
		    config->SetExtraEmModel(particle->GetParticleName(), "ionIoni", pai, "RegionGarfield", 0. * MeV,
							1000. * MeV, pai);
		    }
	} 

}


void MuPhysicsList::SetCuts() {
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100. * eV,
			100. * TeV);

	SetCutsWithDefault();

	G4Region *region = G4RegionStore::GetInstance()->GetRegion(
			"RegionGarfield");
	G4ProductionCuts * cuts = new G4ProductionCuts();
	cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("gamma"));
	cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("e-"));
	cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("e+"));
	if (region) {
		region->SetProductionCuts(cuts);
	}

	DumpCutValuesTable();
}

void MuPhysicsList::ConstructParticle() {
	G4VModularPhysicsList::ConstructParticle();
}

void MuPhysicsList::ConstructProcess() {
	G4VModularPhysicsList::ConstructProcess();
	AddParameterisation();
}

