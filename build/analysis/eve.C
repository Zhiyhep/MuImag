#include "../../include/PrimaryHits.hh"
R__LOAD_LIBRARY(../../src/libPrimaryHits.so)
void eve(TString geometry = "Muon_Setup.root",datafile = "../rawdata.root"){
  gSystem->Load("libPhysics");
  gSystem->Load("../../src/libPrimaryHits");
  gSystem->IgnoreSignal(kSigSegmentationViolation,true);
  gEve = TEveManager::Create();
  gGeoManager = gEve->GetGeometry(geometry);
  TGeoNode* node = gGeoManager->GetCurrentNode();
  TEveGeoTopNode* det = new TEveGeoTopNode(gGeoManager,node);
  gEve->AddGlobalElement(det);
  TEveRGBAPalette* pal = new TEveRGBAPalette(0,130);
  TEveFrameBox* frm = new TEveFrameBox();
  frm->SetAABoxCenterHalfSize(0, 0, 0, 100, 100, 100);
  frm->SetFrameColor(kCyan);
  frm->SetBackColorRGBA(120,120,120,120);
  frm->SetDrawBack(kTRUE);
  PrimaryHits *phit = new PrimaryHits;
  TFile *f = TFile::Open(datafile,"readonly");
  TTree *t = (TTree*)f->Get("G4Event");
  TBranch *event = t->GetBranch("event");
  event->SetAddress(&phit);
  gEve->Redraw3D(kTRUE);
  
}
