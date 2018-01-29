#include <TFile.h>
#include <TTree.h>
#include <TEveManager.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TEveGeoNode.h>
#include <TGeoManager.h>
#include "../../include/PrimaryHits.hh"
#include <iostream>
#include <TEvePointSet.h>
#include <TEveRGBAPalette.h>
#include <TColor.h>
#include <TNamed.h>

void Start_Viewer(TString gdml, TString datafile, Int_t event_no){
    TEveManager::Create();
    TFile* geom = TFile::Open(gdml);
    TFile* data = TFile::Open(datafile);
    TTree* t = (TTree*) data->Get("G4Event");
    PrimaryHits *phit = new PrimaryHits;
    TBranch *event = t->GetBranch("event");
    event->SetAddress(&phit);
    event->GetEntry(event_no);
    if(!geom) return;
    gGeoManager = gEve->GetGeometry(gdml);
    TGeoNode* node = gGeoManager->GetCurrentNode();
    TEveGeoTopNode* det = new TEveGeoTopNode(gGeoManager, node);

    // Hit Points
    TEvePointSet* hit_pos = new TEvePointSet();
    hit_pos->SetOwnIds(kTRUE);
    for(Int_t i = 0; i < phit->fNhits; i++){
        hit_pos->SetNextPoint(phit->fPosX[i],phit->fPosY[i],phit->fPosZ[i]);
        hit_pos->SetPointId(new TNamed(Form("Hit %d",i),""));
    }

    hit_pos->SetMarkerColor(3);
    gEve->AddGlobalElement(det);
    gEve->Redraw3D(kTRUE);
    
}

void Event_Viewer(){
    Start_Viewer("Muon_Setup.root");
}