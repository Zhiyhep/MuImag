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
