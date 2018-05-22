#include "MuonEvent.hh"
#include <vector>

MuonEvent::MuonEvent() : fClusterNum(0){}

MuonEvent::MuonEvent(const MuonEvent& muevent)
{
  fClusters.assign(muevent.fClusters.begin(), muevent.fClusters.end());
  fClusterNum = muevent.fClusterNum;
}
MuonEvent::~MuonEvent(){ fClusterNum = 0; Clean();}

void MuonEvent::Clean() {
  fClusters.clear();
}

