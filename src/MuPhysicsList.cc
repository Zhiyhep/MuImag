/*
 * MuonPhysicsList.cc
 *
 *  Created on: May 27, 2017
 *      Author: newdriver
 */

#include "MuPhysicsList.hh"

#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ParticleTable.hh"

MuPhysicsList::MuPhysicsList() :G4VUserPhysicsList()
{
	// TODO Auto-generated constructor stub
	defaultCutValue = 1.*cm;
	SetVerboseLevel(1);
}

MuPhysicsList::~MuPhysicsList() {
	// TODO Auto-generated destructor stub
}


void MuPhysicsList::ConstructParticle()
{
	// In this method, static member functions should be called
	  // for all particles which you want to use.
	  // This ensures that objects of these particle types will be
	  // created in the program.
	  G4IonConstructor pIonConstructor;
	  pIonConstructor.ConstructParticle();

	  ConstructBosons();
	  ConstructLeptons();
	  ConstructMesons();
	  ConstructBaryons();

}


void MuPhysicsList::ConstructBosons(){
	  // pseudo-particles
	  G4Geantino::GeantinoDefinition();
	  G4ChargedGeantino::ChargedGeantinoDefinition();

	  // gamma
	  G4Gamma::GammaDefinition();
}

void MuPhysicsList::ConstructLeptons() {
	  // leptons
	  //  e+/-
	  G4Electron::ElectronDefinition();
	  G4Positron::PositronDefinition();
	  // mu+/-
	  G4MuonPlus::MuonPlusDefinition();
	  G4MuonMinus::MuonMinusDefinition();
	  // nu_e
	  G4NeutrinoE::NeutrinoEDefinition();
	  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
	  // nu_mu
	  G4NeutrinoMu::NeutrinoMuDefinition();
	  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void MuPhysicsList::ConstructMesons(){
	  //  mesons
	  //    light mesons
	  G4PionPlus::PionPlusDefinition();
	  G4PionMinus::PionMinusDefinition();
	  G4PionZero::PionZeroDefinition();
	  G4Eta::EtaDefinition();
	  G4EtaPrime::EtaPrimeDefinition();
	  G4KaonPlus::KaonPlusDefinition();
	  G4KaonMinus::KaonMinusDefinition();
	  G4KaonZero::KaonZeroDefinition();
	  G4AntiKaonZero::AntiKaonZeroDefinition();
	  G4KaonZeroLong::KaonZeroLongDefinition();
	  G4KaonZeroShort::KaonZeroShortDefinition();

}

void MuPhysicsList::ConstructBaryons(){
	  //  barions
	  G4Proton::ProtonDefinition();
	  G4AntiProton::AntiProtonDefinition();

	  G4Neutron::NeutronDefinition();
	  G4AntiNeutron::AntiNeutronDefinition();
}

void MuPhysicsList::ConstructProcess(){
	  AddTransportation();
	  ConstructEM();
	  ConstructGeneral();
	  AddStepMax();
}

//++++++++++++++++++++++++++++++++++++++++++

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

void MuPhysicsList::ConstructEM(){
        //theParticleIterator = GetParticleIterator();
	theParticleIterator->reset();
        G4cout<<"ConstructEM"<<G4endl;
	  while( (*theParticleIterator)() ){
	    G4ParticleDefinition* particle = theParticleIterator->value();
	    G4ProcessManager* pmanager = particle->GetProcessManager();
	    G4String particleName = particle->GetParticleName();
            /*
	    if (particleName == "gamma") {
	      // gamma
	      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
	      pmanager->AddDiscreteProcess(new G4ComptonScattering);
	      pmanager->AddDiscreteProcess(new G4GammaConversion);

	    } else if (particleName == "e-") {
	      //electron
	      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
	      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);

	    } else if (particleName == "e+") {
	      //positron
	      pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
	      pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	      pmanager->AddProcess(new G4eplusAnnihilation,    0,-1, 4);

	    } */if( particleName == "mu+" ||particleName == "mu-") {
	      //muon
	      pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
	      pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
	      pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);

	    } /*else if( particleName == "proton" ||
	               particleName == "pi-" ||
	               particleName == "pi+"    ) {
	      //proton
	      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
	      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
	      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);

	    }*/ /*else if( particleName == "alpha" ||
		       particleName == "He3" )     {
	      //alpha
	      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);

	    } else if( particleName == "GenericIon" ) {
	      //Ions
	      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);

	    }*/ else if ((!particle->IsShortLived()) &&
		       (particle->GetPDGCharge() != 0.0) &&
		       (particle->GetParticleName() != "chargedgeantino")) {
	      //all others charged particles except geantino
	      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
	      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
	    }
	  }
}

#include "G4Decay.hh"

void MuPhysicsList::ConstructGeneral(){
	  // Add Decay Process
	  G4Decay* theDecayProcess = new G4Decay();
	  theParticleIterator->reset();
	  while( (*theParticleIterator)() ){
	    G4ParticleDefinition* particle = theParticleIterator->value();
	    G4ProcessManager* pmanager = particle->GetProcessManager();
	    if (theDecayProcess->IsApplicable(*particle)) {
	      pmanager ->AddProcess(theDecayProcess);
	      // set ordering for PostStepDoIt and AtRestDoIt
	      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
	      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
	    }
	  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void MuPhysicsList::AddStepMax() {

	  // Step limitation seen as a process
	  G4StepLimiter* stepLimiter = new G4StepLimiter();
	  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();

	  theParticleIterator->reset();
	  while ((*theParticleIterator)()){
	      G4ParticleDefinition* particle = theParticleIterator->value();
	      G4ProcessManager* pmanager = particle->GetProcessManager();

	      if (particle->GetPDGCharge() != 0.0)
	        {
		  pmanager ->AddDiscreteProcess(stepLimiter);
		  ////pmanager ->AddDiscreteProcess(userCuts);
	        }
	  }
}


void MuPhysicsList::SetCuts() {
	  //G4VUserPhysicsList::SetCutsWithDefault method sets
	  //the default cut value for all particle types
	  //
	  SetCutsWithDefault();

	  if (verboseLevel>0) DumpCutValuesTable();

}
