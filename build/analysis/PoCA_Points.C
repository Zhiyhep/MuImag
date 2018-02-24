// The simplest muon imaging algorithm 
// Writer: Zhi Yu
// Date: 2018/2/20
// Flow: -> Read reconstructed result from file
//       -> Select events that have been detected by all detectors
//       -> Create PointSet and fill it with PoCA points
//       -> Plot PointSet
//
#include "Scatter_Pt.h"
#include "MultiView.C"
#include <iostream>
#include "DetectorLayout.hh"
void Plot_PoCA(TString geometry = "Geometry/Muon_Setup.root",
               TString datafile = "../rawdata.root")
{
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
    TEvePointSet* ps = new TEvePointSet();
    ps->SetMarkerColor(2);
    ps->SetMarkerSize(1);
    ps->SetMarkerStyle(4);

    TFile *f = new TFile(datafile,"readonly");
    TTree *t = (TTree*)f->Get("Reconstruct");
    TVector3  *pos1 = new TVector3();
    TVector3  *pos2 = new TVector3();
    TVector3  *pos3 = new TVector3();
    TVector3  *pos4 = new TVector3();
    int event_no = 0;

    TBranch *br1 = t->GetBranch("Pos1");
    TBranch *br2 = t->GetBranch("Pos2");
    TBranch *br3 = t->GetBranch("Pos3");
    TBranch *br4 = t->GetBranch("Pos4");
    TBranch *br5 = t->GetBranch("Event_No");
    br1->SetAddress(&pos1);
    br2->SetAddress(&pos2);
    br3->SetAddress(&pos3);
    br4->SetAddress(&pos4);
    br5->SetAddress(&event_no);
    
    int nentry = t->GetEntries();
    for(int i = 0; i < nentry; i++){
        t->GetEntry(i);
        TVector3 Incident_Dir = (*pos2-*pos1).Unit();
        Double_t Incident_Ang_X = TMath::ATan(-Incident_Dir.x()/Incident_Dir.z()); // Incident angle [rad]
        Double_t Incident_Ang_Y = TMath::ATan(-Incident_Dir.y()/Incident_Dir.z()); // Incident angle [rad]
        TVector3 Exit_Dir = (*pos4-*pos3).Unit();
        Double_t Exit_Ang_X = TMath::ATan(-Exit_Dir.x()/Exit_Dir.z()); // Exit angle [rad]
        Double_t Exit_Ang_Y = TMath::ATan(-Exit_Dir.y()/Exit_Dir.z()); // Exit angle [rad]
        double Scatt_Ang_X = Exit_Ang_X - Incident_Ang_X;
        double Scatt_Ang_Y = Exit_Ang_Y - Incident_Ang_Y;
        double S = (TMath::Power(Scatt_Ang_X,2)+TMath::Power(Scatt_Ang_Y,2))/2*1e6;
        TVector3 Pt = Scatter_Pt(*pos1,Incident_Dir,*pos3,Exit_Dir);
        if(TMath::Abs(Pt.z())<ObjectAreaWidth/20 && S > ObjectAreaWidth/20){
            ps->SetNextPoint(Pt.x(),Pt.y(),Pt.z());
            std::cout << event_no <<": S = " << S << std::endl;
        }
    }
    gEve->AddElement(ps);
    
    // MultiView
    MultiView* gMultiView = new MultiView;
    gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
    gMultiView->SetDepth(-10);
    gMultiView->ImportGeomRPhi(det);
    gMultiView->ImportEventRPhi(ps);
    gMultiView->ImportGeomRhoZ(det);
    gMultiView->ImportEventRhoZ(ps);
    gMultiView->SetDepth(0);
    
    gEve->GetViewers()->SwitchColorSet();
    gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);
    gEve->Redraw3D(kTRUE);
    f->Close();
}

void PoCA_Points(){
    Plot_PoCA("Geometry/Muon_Setup.root", "../rawdata.root");
}
