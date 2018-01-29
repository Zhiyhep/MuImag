#include "PrimaryHits.hh"
#include <vector>

PrimaryHits::PrimaryHits():fNhits(0),fPrimaryEnergy(0){}

PrimaryHits::~PrimaryHits(){}

void PrimaryHits::Clean(){
    fPosX.clear();
    fPosY.clear();
    fPosZ.clear();
    fEdep.clear();
    fTime.clear();
}

// Get deposit energy in certain volume
Double_t PrimaryHits::GetDepositEnergyInVolume(TString volName){
    Double_t edep = 0;
    for(Int_t i=0; i < fNhits; i++){
       if(fDetName[i] == volName) edep+=fEdep[i];
    }
    return edep;
}

// Get number of hits in certain volume
Int_t PrimaryHits::GetNhitsInVolume(TString volName){
    Int_t Nhits = 0;
    for(Int_t i = 0; i < fNhits;i++){
        if(fDetName[i] == volName) Nhits++;
    }
    return Nhits;
}

// Get mean position in certain volume, using deposit energy as weights
TVector3 PrimaryHits::GetMeanPositionInVolume(TString volName){
    Int_t x = 0, y = 0, z = 0;
    for(Int_t i = 0; i < fNhits; i++){
        if(fDetName[i] == volName){
            x+=fEdep[i]*fPosX[i];
            y+=fEdep[i]*fPosY[i];
            z+=fEdep[i]*fPosZ[i];
        }
    }
    // If deposit energy is zero, then x = y = z = 0
    x = x/GetDepositEnergyInVolume(volName);
    y = y/GetDepositEnergyInVolume(volName);
    z = z/GetDepositEnergyInVolume(volName);
    return TVector3(x,y,z);
}

// Check if deposit energy is greater than threshold energy
Bool_t PrimaryHits::IsDetectedInVolume(TString volName, Double_t E_THR=0.){
    return (GetDepositEnergyInVolume(volName) > E_THR);
}

