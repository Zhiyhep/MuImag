// Class for saving G4 hits information
// Writer: Zhi Yu
// Date: 2018/1/26

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
    fDetName.clear();
}

Double_t PrimaryHits::GetDepositEnergyInVolume(TString volName){
    Double_t dep = 0;
    for(Int_t i = 0; i < fNhits; i++){
        if(volName == fDetName[i]) dep += fEdep[i];
    }
    return dep;
}

Int_t PrimaryHits::GetNhitsInVolume(TString volName){
    Int_t nhit = 0;
    for(Int_t i = 0; i < fNhits; i++){
        if(volName == fDetName[i]) nhit++;
    }
    return nhit;
}

TVector3 PrimaryHits::GetMeanPositionInVolume(TString volName){
    Double_t x = 0, y = 0, z = 0;
    for(Int_t i = 0; i < fNhits; i++){
        if(volName == fDetName[i]){
            x += fEdep[i]*fPosX[i];
            y += fEdep[i]*fPosY[i];
            z += fEdep[i]*fPosZ[i];
        }
    }
    x = x/GetDepositEnergyInVolume(volName);
    y = y/GetDepositEnergyInVolume(volName);
    z = z/GetDepositEnergyInVolume(volName);
    TVector3 pos(x, y, z);
    return pos;
}