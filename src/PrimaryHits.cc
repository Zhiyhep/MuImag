#include "PrimaryHits.hh"
#include <vector>

PrimaryHits::PrimaryHits():fNhits(0),fPrimaryEnergy(0){ }
PrimaryHits::~PrimaryHits(){ }
void PrimaryHits::Clean(){
    fPosX.clear();
    fPosY.clear();
    fPosZ.clear();
    fEdep.clear();
    fTime.clear();
}

