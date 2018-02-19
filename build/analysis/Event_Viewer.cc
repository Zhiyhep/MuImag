#include <TFile.h>
#include <TTree.h>
#include <TEveManager.h>
#include <TEveManager.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TEveGeoNode.h>
#include <TGeoManager.h>
#include <iostream>
#include <TEveBoxSet.h>
#include <TEveRGBAPalette.h>
#include <TColor.h>
#include <TNamed.h>
#include <TEveBoxSet.h>
#include <TEvePointSet.h>
#include <TEveStraightLineSet.h>
#include <TVector3.h>
#include "../../include/PrimaryHits.hh"
#include "Scatter_Pt.h"
#include "MultiView.C"
#include <iostream>
using namespace std;

MultiView* gMultiView = 0;
void Event_Viewer(int event_no = 0, TString geometry = "Geometry/Muon_Setup.root",
                  TString datafile = "../rawdata.root")
{
    R__LOAD_LIBRARY(../../lib/libPrimaryHits.so)
    gSystem->IgnoreSignal(kSigSegmentationViolation,true);
    TEveManager::Create();
    gGeoManager = gEve->GetGeometry(geometry);
    TGeoVolume* detVolume = gGeoManager->GetVolume("DetectorBoxVolume");
    TGeoVolume* gasVolume = gGeoManager->GetVolume("gasVolume");
    TGeoMaterial* kapton = gGeoManager->GetMaterial("Kapton");
    TGeoMaterial* Ar_CO2 = gGeoManager->GetMaterial("Ar_CO2");
    kapton->SetTransparency(90);
    Ar_CO2->SetTransparency(90);
    detVolume->SetLineColor(kBlue);
    gasVolume->SetLineColor(kGreen);
    TGeoNode* node = gGeoManager->GetCurrentNode();
    TEveGeoTopNode* det = new TEveGeoTopNode(gGeoManager,node);
    det->SetEditMainTransparency(true);
    gEve->AddGlobalElement(det);

    /*
    TEveFrameBox* frm = new TEveFrameBox();
    frm->SetAABoxCenterHalfSize(0, 0, 0, 100, 100, 100);
    frm->SetFrameColor(kCyan);
    frm->SetBackColorRGBA(120,120,120,120);
    frm->SetDrawBack(kTRUE);
    */
    TEveRGBAPalette* pal = new TEveRGBAPalette(0,130);
    TEveBoxSet* q = new TEveBoxSet("BoxSet");
    q->SetPalette(pal);
    //q->SetFrame(frm);
    q->Reset(TEveBoxSet::kBT_AABox, kFALSE, 64);
    q->RefitPlex();
    gSystem->AddIncludePath("-I../../include");
    gSystem->Load("libPhysics");
    TFile *f = new TFile(datafile,"readonly");
    TTree *t = (TTree *)f->Get("G4Event");
    TBranch *event = t->GetBranch("event");
    PrimaryHits* phit = new PrimaryHits;
    event->SetAddress(&phit);
    event->GetEntry(event_no);
    for(int i = 0; i<phit->fNhits;i++){
        q->AddBox(phit->fPosX[i]-1,phit->fPosY[i]-1,phit->fPosZ[i]-0.1,2,2,0.2);
        q->DigitValue(phit->fEdep[i]*20);
        cout<<phit->fPosX[i]<<", "<<phit->fPosY[i]<<", "<<phit->fPosZ[i]<<endl;
    }
    gEve->AddElement(q);
    
    TVector3 p1 = phit->GetMeanPositionInVolume("gas");
    TVector3 p2 = phit->GetMeanPositionInVolume("gas2");
    TVector3 p3 = phit->GetMeanPositionInVolume("gas3");
    TVector3 p4 = phit->GetMeanPositionInVolume("gas4");
    TVector3 pt = Scatter_Pt(p1,p2-p1,p3,p4-p3);
    cout<<"Mean Position"<<endl;
    cout<<p1.x()<<", "<<p1.y()<<", "<<p1.z()<<endl;
    cout<<p2.x()<<", "<<p2.y()<<", "<<p2.z()<<endl;
    cout<<p3.x()<<", "<<p3.y()<<", "<<p3.z()<<endl;
    cout<<p4.x()<<", "<<p4.y()<<", "<<p4.z()<<endl;
    double k1 = (pt.z()-p2.z())/(p2.z()-p1.z());
    double k2 = (pt.z()-p3.z())/(p3.z()-p4.z());
    TEveStraightLineSet* ls = new TEveStraightLineSet();
    ls->AddLine(p1.x(),p1.y(),p1.z(),
               p2.x()+k1*(p2.x()-p1.x()),p2.y()+k1*(p2.y()-p1.y()),pt.z()); 
    ls->AddLine(p4.x(),p4.y(),p4.z(),
               p3.x()+k2*(p3.x()-p4.x()),p3.y()+k2*(p3.y()-p4.y()),pt.z()); 
    gEve->AddElement(ls);
    TEvePointSet* ps = new TEvePointSet();
    ps->SetNextPoint(p1.x(),p1.y(),p1.z());
    ps->SetNextPoint(p2.x(),p2.y(),p2.z());
    ps->SetNextPoint(p3.x(),p3.y(),p3.z());
    ps->SetNextPoint(p4.x(),p4.y(),p4.z());
    ps->SetNextPoint(pt.x(),pt.y(),pt.z());
    ps->SetMarkerColor(5);
    ps->SetMarkerSize(1.5);
    ps->SetMarkerStyle(3);
    gEve->AddElement(ps);
    //Multi View
    gMultiView = new MultiView;
    gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
    gMultiView->SetDepth(-10);
    gMultiView->ImportGeomRPhi(det);
    gMultiView->ImportEventRPhi(q);
    gMultiView->ImportEventRPhi(ps);
    gMultiView->ImportEventRPhi(ls);
    gMultiView->ImportGeomRhoZ(det);
    gMultiView->ImportEventRhoZ(q);
    gMultiView->ImportEventRhoZ(ps);
    gMultiView->ImportEventRhoZ(ls);
    gMultiView->SetDepth(0);
    
    gEve->GetViewers()->SwitchColorSet();
    gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);
    gEve->Redraw3D(kTRUE);
    f->Close();
}
