#include <iostream>
#include "GarfieldG4FastSimulationModel.hh"
#include "MuDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"

#include "MuonEvent.hh"

#include "G4SystemOfUnits.hh"

#include "MuonEvent.hh"

GarfieldG4FastSimulationModel::GarfieldG4FastSimulationModel(G4String modelName, G4Region* envelope, MuDetectorConstruction* det) :
	G4VFastSimulationModel(modelName, envelope), fMuDetectorConstruction(det), fDepositEnergy(0), fElectronNum(0) {
  fVolName = "";
  muevent = new MuonEvent();
  fMediumMagboltz = new Garfield::MediumMagboltz();

  fMediumMagboltz->SetComposition("ar", 70., "co2", 30.);
  fMediumMagboltz->SetTemperature(293.15);
  fMediumMagboltz->SetPressure(760.);
  fMediumMagboltz->EnableDebugging();
  fMediumMagboltz->Initialise(true);
  fMediumMagboltz->DisableDebugging();
  const double rPenning = 0.57;
  const double lambdaPenning = 0;
  fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  //fMediumMagboltz->LoadIonMobility("IonMobility_Ar+_Ar.txt");
  fMediumMagboltz->LoadGasFile("ar_70_co2_30_1000mbar.gas");

  fSensor = new Garfield::Sensor();
  fDrift = new Garfield::AvalancheMC();
  fAvalanche = new Garfield::AvalancheMicroscopic();
  fComponentConstant = new Garfield::ComponentConstant();

  fGeometrySimple = new Garfield::GeometrySimple();
  fBox = new Garfield::SolidBox(0, 0, 0, fMuDetectorConstruction->fHalf_Size_X/CLHEP::cm, fMuDetectorConstruction->fHalf_Size_Y/CLHEP::cm, fMuDetectorConstruction->fDet_Half_Z/CLHEP::cm);
  fGeometrySimple->AddSolid(fBox, fMediumMagboltz);
  fComponentConstant->SetElectricField(0, 0, 1000);
  fComponentConstant->SetGeometry(fGeometrySimple);
  fSensor->AddComponent(fComponentConstant);
  fSensor->SetArea(-fMuDetectorConstruction->fHalf_Size_X/CLHEP::cm, -fMuDetectorConstruction->fHalf_Size_Y/CLHEP::cm, -fMuDetectorConstruction->fDet_Half_Z/CLHEP::cm, fMuDetectorConstruction->fHalf_Size_X/CLHEP::cm, fMuDetectorConstruction->fHalf_Size_Y/CLHEP::cm, fMuDetectorConstruction->fDet_Half_Z/CLHEP::cm);

  fDrift->SetSensor(fSensor);
  fDrift->SetDistanceSteps(fMuDetectorConstruction->fDet_Half_Z/CLHEP::cm*2);
  fAvalanche->SetSensor(fSensor);

  fTrackHeed = new Garfield::TrackHeed();
  fTrackHeed->SetSensor(fSensor);
  fTrackHeed->EnableElectricField();

  fTrackHeed->EnableDeltaElectronTransport();
  std::cout << "Construct GarfieldG4FastSimulationModel ... " << std::endl;
}

GarfieldG4FastSimulationModel::~GarfieldG4FastSimulationModel(){
  delete fGeoManager;
  delete fMuDetectorConstruction;
  delete fMediumMagboltz;
  delete fSensor;
  delete fDrift;
  delete fAvalanche;
  delete fTrackHeed;
  delete fGeometryRoot;
  delete fGeometrySimple;
  delete fComponentAnalyticField;
  delete fComponentConstant;
  delete fBox;
  delete muevent;

  std::cout << "Destruct GarfieldG4FastSimulationModel ... " << std::endl;
}

G4bool GarfieldG4FastSimulationModel::IsApplicable(const G4ParticleDefinition& particleType){
  G4String particleName = particleType.GetParticleName();
  return (particleName == "mu+") ||(particleName == "mu-");
}

G4bool GarfieldG4FastSimulationModel::ModelTrigger(const G4FastTrack& fastTrack){
  G4TouchableHandle theTouchable = fastTrack.GetPrimaryTrack()->GetTouchableHandle();
  G4String name = theTouchable->GetVolume()->GetName();
  if(name != fVolName){
    fVolName = name;
    std::cout << "FastSimulationModel Triggered" << std::endl;
    return true;
  }
  return false;
}

void GarfieldG4FastSimulationModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {
  fDepositEnergy = 0;
  fElectronNum = 0;

  muevent->Clean();
  Cluster aCluster;
  Electron primaryElectron;
  Electron driftedElectron;
  G4TouchableHandle theTouchable = fastTrack.GetPrimaryTrack()->GetTouchableHandle();
  G4String name = theTouchable->GetVolume()->GetName();

  G4ThreeVector pdirection = fastTrack.GetPrimaryTrack()->GetMomentum().unit();
  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();

  G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();

  G4ThreeVector localPosition = fastTrack.GetPrimaryTrackLocalPosition();

  double ekin_MeV = fastTrack.GetPrimaryTrack()->GetKineticEnergy() / MeV;
  G4double globalTime = fastTrack.GetPrimaryTrack()->GetGlobalTime();
  G4String particleName = fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

  //fastStep.KillPrimaryTrack();
  fastStep.SetPrimaryTrackPathLength(0.0);

  double xc = 0., yc = 0., zc = 0., tc = 0.;
  int nc = 0;
  double ec = 0.;
  double extra = 0.;
  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Incident Muon: " << particleName << std::endl;
  std::cout << "Gas Volume: " << name << std::endl;
  std::cout << "Position: " << worldPosition.x()/CLHEP::cm <<" cm, "<<worldPosition.y()/CLHEP::cm <<" cm, "<< worldPosition.z()/CLHEP::cm <<" cm"<<std::endl;
  std::cout << "Time: " << globalTime <<" s "<< std::endl;
  std::cout << "Energy: " << ekin_MeV/1000 << " GeV" << std::endl;
  std::cout << "Direction: "<<localdir.x()<<", "<<localdir.y()<<", "<<localdir.z()<<std::endl;
  std::cout <<"-----------------------------------------------"<< std::endl;
  fTrackHeed->SetParticle(particleName);
  fTrackHeed->SetKineticEnergy(ekin_MeV*1e6);
  fTrackHeed->NewTrack(localPosition.x()/CLHEP::cm, localPosition.y()/CLHEP::cm, localPosition.z()/CLHEP::cm, globalTime, localdir.x(), localdir.y(), localdir.z());
  int clusternum = 0;
  while(fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra)){
   aCluster.Clean();
   clusternum++;
   fDepositEnergy += ec;
   /*
   std::cout << "\n" << std::endl;
   std::cout << "****************************************" << std::endl;
   std::cout << "\n" << std::endl;
   std::cout << "Cluster" << clusternum << ": "<< std::endl;
   */
   aCluster.fClusterId = clusternum;
   aCluster.fElectronNum = nc;
   aCluster.fTransEnergy = ec;
   aCluster.fPosX = xc;
   aCluster.fPosY = yc;
   aCluster.fPosZ = zc;
   aCluster.fTime = tc;
   aCluster.fVolName = name.data();
   for(int i = 0; i < nc; ++i){
      fTrackHeed->GetElectron(i, primaryElectron.xe, primaryElectron.ye, primaryElectron.ze, primaryElectron.te, primaryElectron.ee, primaryElectron.dx, primaryElectron.dy, primaryElectron.dz);
      primaryElectron.electron_id = i;
      fElectronNum++;
      /*
      std::cout << "Primary Electron " << fElectronNum << ": " << std::endl;
      std::cout << "x: " << xe << ", y: " << ye << ", z: " << ze <<std::endl;
      std::cout << "t: " << te << ", e: " << ee << std::endl;
      std::cout << "dx: " << dxe << ", dy: " << dye << ", dz: " << dze << std::endl;
      std::cout << "----------------------------------------------" << std::endl;
      */
      fDrift->DriftElectron(primaryElectron.xe, primaryElectron.ye, primaryElectron.ze, primaryElectron.te);
      int nDrift = fDrift->GetNumberOfDriftLinePoints();
      fDrift->GetDriftLinePoint(nDrift-1, driftedElectron.xe, driftedElectron.ye, driftedElectron.ze, driftedElectron.te);
      driftedElectron.electron_id = i;
      driftedElectron.dx = 0;
      driftedElectron.dy = 0;
      driftedElectron.dz = 0;
      driftedElectron.ee = 0;

      aCluster.PrimaryElectron.push_back(primaryElectron);
      aCluster.DriftedElectron.push_back(driftedElectron);
      /*
      std::cout << "Drifted Electron " << fElectronNum << ": " << std::endl;
      std::cout << "x: " << xe1 << ", y: " << ye1 << ", z: " << ze1 << ", t: " << te1 <<std::endl;
              std::cout << "----------------------------------------------" << std::endl;
      */
      //fAvalanche->AvalancheElectron(xe2, ye2, ze2, te2, 0.1, 0,0, 0);
   } 
   muevent->fClusters.push_back(aCluster);
  }
  muevent->fClusterNum = clusternum;
  fastStep.SetTotalEnergyDeposited(fDepositEnergy);

  std::cout << "Physical Volume: " << fVolName << std::endl;
  std::cout << "Deposit Energy: " << fDepositEnergy << std::endl;
  std::cout << "Number of Electrons: " << fElectronNum++ << std::endl;
}
