/*
 * GarfieldModel.h
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */

#ifndef GARFIELDG4FASTSIMULATIONMODEL_H_
#define GARFIELDG4FASTSIMULATIONMODEL_H_

#include "G4VFastSimulationModel.hh"

#include "Sensor.hh"
#include "AvalancheMC.hh"
#include "AvalancheMicroscopic.hh"
#include "ComponentAnalyticField.hh"
#include "ComponentConstant.hh"
#include "TrackHeed.hh"
#include "TrackSimple.hh"
#include "MediumMagboltz.hh"
#include "GeometryRoot.hh"
#include "GeometrySimple.hh"
#include "SolidBox.hh"

class G4VPhysicalVolume;
class MuDetectorConstruction;
class MuonEvent;

class GarfieldG4FastSimulationModel : public G4VFastSimulationModel
{
public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
	GarfieldG4FastSimulationModel (G4String, G4Region*, MuDetectorConstruction*);
  ~GarfieldG4FastSimulationModel ();

  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack &);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
  MuonEvent* muevent;

private:
  TGeoManager* fGeoManager;
  MuDetectorConstruction* fMuDetectorConstruction;
  Garfield::MediumMagboltz* fMediumMagboltz;
  Garfield::Sensor* fSensor;
  Garfield::AvalancheMC* fDrift;
  Garfield::AvalancheMicroscopic* fAvalanche;
  Garfield::TrackHeed* fTrackHeed;
  Garfield::GeometryRoot* fGeometryRoot;
  Garfield::GeometrySimple* fGeometrySimple;
  Garfield::ComponentAnalyticField* fComponentAnalyticField;
  Garfield::ComponentConstant* fComponentConstant;
  Garfield::SolidBox* fBox;
  double fDepositEnergy;
  int fElectronNum;
  G4String fVolName;
};


#endif /* GARFIELDMODEL_H_ */
