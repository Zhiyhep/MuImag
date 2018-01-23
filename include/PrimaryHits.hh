#ifndef PrimaryHits_H
#define PrimaryHits_H

#include "TVector3.h"
#include <vector>

class PrimaryHits{
    public:
        PrimaryHits();
        virtual ~PrimaryHits();          
        std::vector<Double_t> fEdep;
        std::vector<Double_t> fTime;
        std::vector<Double_t> fPosX;
        std::vector<Double_t> fPosY;
        std::vector<Double_t> fPosZ;
        Int_t fNhits;
        TVector3 fParticleGunPosition;
        TVector3 fParticleGunDirection;
        Double_t fPrimaryEnergy;
        void Clean();

};
#endif
