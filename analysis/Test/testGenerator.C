// Test muon generator
// Writer: Zhi Yu
// Data: 2018/3/5
// Flow: -> Open and read data file
//       -> Extract particle gun position and direction
//       -> Plot the result
//
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TString.h"
#include "TMath.h"
#include "TH2.h"
#include "../../../include/PrimaryHits.hh"

R__LOAD_LIBRARY(../../../lib/libPrimaryHits.so)
void testGenerator(TString filename="../../rawdata.root")
{
    TFile *f = new TFile(filename,"readonly");
    TTree *t = (TTree*)f->Get("G4Event");
    PrimaryHits *phit = new PrimaryHits;
    TBranch *event = t->GetBranch("event");
    event->SetAddress(&phit);
    int nentry = t->GetEntries();
    double theta = 0, E = 0;
    TCanvas* c = new TCanvas();
    c->Divide(2,2);
    c->SetLogx();
    TH2F* h2 = new TH2F("h2","Muon generator",100,t->GetMinimum("fPrimaryEnergy"),t->GetMaximum("fPrimaryEnergy"),100,0,TMath::Pi()/2);
    for(int nevent = 0; nevent < nentry; nevent++)
    {
        event->GetEntry(nevent);
        theta = TMath::ACos(-(phit->fParticleGunDirection).z());
        E = phit->fPrimaryEnergy;
        h2->Fill(E,theta);
    }
    c->cd(1);
    c->GetPad(1)->SetLogx();
    c->GetPad(1)->SetLogy();
    h2->ProjectionX()->DrawClone();
    c->cd(2);
    h2->ProjectionY()->DrawClone();
    c->cd(3);
    c->GetPad(3)->SetLogx();
    c->GetPad(3)->SetLogy();
    h2->Draw();
}