#include "Cluster.hh"

Cluster::Cluster() : fClusterId(0), fElectronNum(0), fTransEnergy(0), 
	             fPosX(0), fPosY(0), fPosZ(0), fTime(0), fVolName(""){}

Cluster::~Cluster(){}

void Cluster::Clean()
{
  fClusterId = 0;
  fElectronNum = 0;
  fTransEnergy = 0;
  fPosX = 0;
  fPosY = 0;
  fPosZ = 0;
  fTime = 0;
  fVolName ="";
  PrimaryElectron.clear();
  DriftedElectron.clear();
}

