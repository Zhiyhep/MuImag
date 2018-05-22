#ifndef MuPhysicsList_h
#define MuPhysicsList_h 1


#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class MuPhysicsList: public G4VModularPhysicsList {
public:
	MuPhysicsList();
	virtual ~MuPhysicsList();
	virtual void SetCuts();
	virtual void ConstructParticle();
	virtual void ConstructProcess();
protected:
	// these methods Construct physics processes and register them
	void AddParameterisation();

};


#endif /* MuPhysicsList_h */
