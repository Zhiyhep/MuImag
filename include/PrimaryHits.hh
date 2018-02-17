// Class for saving G4 hits information
// Writer: Zhi Yu
// Date: 2018/1/26
// Data Member: -> fEdep: deposit energy (keV) of each step
//              -> fTime: global fly time of each time
//              -> fPosX, fPosY, fPosZ: position of each step
//              -> fDetName: detector name of each step
//              -> fNhits: total number of hits
//              -> fParticleGunPosition: primary position
//              -> fParticleGunDirection: primary direction
//              -> fPrimaryEnergy: primary energy
 
#ifndef PrimaryHits_H
#define PrimaryHits_H

#include "TVector3.h"
#include <vector>

class PrimaryHits{
    public:
        PrimaryHits();
        virtual ~PrimaryHits();     
    // Member Data     
        std::vector<Double_t> fEdep;
        std::vector<Double_t> fTime;
        std::vector<Double_t> fPosX;
        std::vector<Double_t> fPosY;
        std::vector<Double_t> fPosZ;
        std::vector<TString> fDetName;
        Int_t fNhits;
        TVector3 fParticleGunPosition;
        TVector3 fParticleGunDirection;
        Double_t fPrimaryEnergy;

    // Member Function
        // Get deposit energy in certain volume
        Double_t GetDepositEnergyInVolume(TString volName);
        // Get number of hits in certain volume
        Int_t GetNhitsInVolume(TString volName);
        // Get mean position in certain volume, using deposit energy as weights
        TVector3 GetMeanPositionInVolume(TString volName);
        // Check if deposit energy is greater than threshold energy
        Bool_t IsDetectedInVolume(TString volName, Double_t E_THR);
        void Clean();

};
#endif
