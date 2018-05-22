// Class for saving Garfield cluster information
// Writer: Zhi Yu
// Date: 2018/5/21
// Data Member: ->
//

#ifndef Cluster_H
#define Cluster_H
#include <string>
#include <vector>

typedef struct
{
  unsigned int electron_id;
  double xe, ye, ze;
  double te; 
  double dx, dy, dz;
  double ee;
}Electron;

class Cluster {
public:
  Cluster();
  virtual ~Cluster(); 
  void Clean();
  unsigned int fClusterId;
  unsigned int fElectronNum;
  double fTransEnergy;
  double fPosX, fPosY, fPosZ;
  double fTime;
  std::vector<Electron> PrimaryElectron; // Electrons induced by ionisation
  std::vector<Electron> DriftedElectron; // Electrons after drift process 
  std::string fVolName;
};
#endif

