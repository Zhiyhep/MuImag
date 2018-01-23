/*
 * MuonPhysicsList.hh
 *
 *  Created on: May 27, 2017
 *      Author: newdriver
 *      Those part is the same as Prof. Wen's simulation
 */

#ifndef SOURCE_DIRECTORY__SRC_MUONPHYSICSLIST_HH_
#define SOURCE_DIRECTORY__SRC_MUONPHYSICSLIST_HH_

#include "G4VUserPhysicsList.hh"
#include "globals.hh"


class MuPhysicsList :public G4VUserPhysicsList {
public:
	MuPhysicsList();
	virtual ~MuPhysicsList();

protected:
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();

protected:
  // these methods Construct particles
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();

protected:
// these methods Construct physics processes and register them
  void ConstructGeneral();
  void ConstructEM();
  void AddStepMax();

  G4int 	verboseLevel;
  G4String 	namePhysics;
  G4ParticleTable * 	theParticleTable;
  //G4ParticleTable::G4PTblDicIterator * 	theParticleIterator;
};

#endif /* SOURCE_DIRECTORY__SRC_MUONPHYSICSLIST_HH_ */
