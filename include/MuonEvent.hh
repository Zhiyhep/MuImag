#ifndef MuonEvent_H
#define MuonEvent_H

#include <vector>
#include "Cluster.hh"
class MuonEvent {
public:
  MuonEvent();
  MuonEvent(const MuonEvent&);
  virtual ~MuonEvent();
  void Clean();

  std::vector<Cluster> fClusters; // Cluster details
  unsigned int fClusterNum; // Number of Clusters
};
#endif
