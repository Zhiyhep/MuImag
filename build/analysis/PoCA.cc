// The simplest muon imaging algorithm 
// Writer: Zhi Yu
// Date: 2018/1/27
// Flow: -> Read reconstructed result from file
//       -> Select events that have been detected by all detectors
//       -> Initialize scatter density matrix and track number matrix
//       -> For each event, find the voxels it transverses
//       -> For each event, renew the scatter density of corresponding voxels
//       -> Renew the density matrix and track number matrix
//       -> Save results to PoCA_Result.dat
//       -> Plot the density matrix
//

#include "TFile.h"
#include "TROOT.h"
#include "TVector3.h"
#include "TTree.h"
#include "../../include/PrimaryHits.hh"
#include "Scatter_Pt.h"
#include "TMath.h"
#include "TH3.h"
#include "TF1.h"
#include "TStyle.h"
#include "TList.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "DetectorLayout.hh"
Double_t my_transfer_function(const Double_t *px, const Double_t*){
    const Double_t x = *px;
    return x/300;
}
void Imag(TString filename, const Double_t (&ObjArea_halfsize)[3], const Int_t (&NBins)[3]){
    TFile *f = new TFile(filename,"readonly");
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

    Int_t nentry = t->GetEntries();
    Double_t E_THR = 0.; // Lowest deposit energy to be detected
    
    const Int_t NBin_X = NBins[0], NBin_Y = NBins[1], NBin_Z = NBins[2];
    const Double_t HalfX = ObjArea_halfsize[0], HalfY = ObjArea_halfsize[1], HalfZ = ObjArea_halfsize[2];
    const Double_t dX = 2*HalfX/NBin_X, dY = 2*HalfY/NBin_Y, dZ = 2*HalfZ/NBin_Z;
    Int_t Ntrack[NBin_X][NBin_Y][NBin_Z];
    Double_t Scatt_Density[NBin_X][NBin_Y][NBin_Z];
    for(Int_t i = 0; i < NBin_X; i++){
        for(Int_t j = 0; j < NBin_Y; j++){
            for(Int_t k = 0; k < NBin_Z; k++){
                Ntrack[i][j][k] = 0;
                Scatt_Density[i][j][k] = 0.;
            }
        }
    }
    for(Int_t i = 0; i < nentry; i++){
        t->GetEntry(i);
        TVector3 Incident_Dir = (*pos2-*pos1).Unit();
        Double_t Incident_Ang = TMath::ACos(-Incident_Dir.z());
        Double_t Incident_Ang_X = TMath::ATan(-Incident_Dir.x()/Incident_Dir.z()); // Incident angle [rad]
        Double_t Incident_Ang_Y = TMath::ATan(-Incident_Dir.y()/Incident_Dir.z()); // Incident angle [rad]
        TVector3 Exit_Dir = (*pos4-*pos3).Unit();
        Double_t Exit_Ang = TMath::ACos(-Exit_Dir.z());
        Double_t Exit_Ang_X = TMath::ATan(-Exit_Dir.x()/Exit_Dir.z()); // Exit angle [rad]
        Double_t Exit_Ang_Y = TMath::ATan(-Exit_Dir.y()/Exit_Dir.z()); // Exit angle [rad]
        TVector3 Pt = Scatter_Pt(*pos1,Incident_Dir,*pos3,Exit_Dir);
        std::cout<<i<<std::endl;
        std::cout<<"event number: "<<event_no<<std::endl;
        //std::cout<<"Pt: ("<<Pt.x()<<", "<<Pt.y()<<", "<<Pt.z()<<")"<<std::endl;
        if(TMath::Abs(Pt.x())<=HalfX && TMath::Abs(Pt.y())<=HalfY && TMath::Abs(Pt.z())<=HalfZ){
            //Calculate scatter density [mrad^2/cm]
            Double_t S = (TMath::Power(Exit_Ang_X-Incident_Ang_X,2)+TMath::Power(Exit_Ang_Y-Incident_Ang_Y,2))/dZ/2*TMath::Cos((Incident_Ang+Exit_Ang)/2)*1e6;
            // Index of scatter point
            Int_t Pt_nx = TMath::Floor((Pt.x()+HalfX)/dX);
            Int_t Pt_ny = TMath::Floor((Pt.y()+HalfY)/dY);
            Int_t Pt_nz = TMath::Floor((Pt.z()+HalfZ)/dZ);
            Ntrack[Pt_nx][Pt_ny][Pt_nz]++;
            Scatt_Density[Pt_nx][Pt_ny][Pt_nz]+=S;
            for(Int_t nz = 0; nz < Pt_nz; nz++){
                // The line function: P = *pos3-L*Exit_Dir
                Double_t L = ((*pos3).z() - ((nz+0.5)*dZ-HalfZ))/Exit_Dir.z();
                // Interac point of exit ray and layer nz  
                TVector3 P = *pos3 - L*Exit_Dir;
                // Index of P
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
               // std::cout<<P_nx<<", "<<P_ny<<std::endl;
                Ntrack[P_nx][P_ny][nz]++;
            } 
            for(Int_t nz = Pt_nz+1; nz < NBin_Z; nz++){
                // The line function: P = *pos2+L*Incident_Dir
                Double_t L = (((nz+0.5)*dZ-HalfZ)-(*pos2).z())/Incident_Dir.z();
                // Interac point of exit ray and layer nz  
                TVector3 P = *pos2 + L*Incident_Dir;
                // Index of P
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
                //std::cout<<P_nx<<", "<<P_ny<<std::endl;
                Ntrack[P_nx][P_ny][nz]++;
            } 
        }
        else{
            TVector3 Mid_Dir = (Incident_Dir+Exit_Dir)*0.5;
            Mid_Dir = Mid_Dir.Unit();
            for(int nz = 0;nz < NBin_Z;nz++){
                // The line function: P = *pos2+L*Incident_Dir
                Double_t L = (((nz+0.5)*dZ-HalfZ)-(*pos2).z())/Incident_Dir.z();  
                TVector3 P = *pos2 + L*Incident_Dir;
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
                Ntrack[P_nx][P_ny][nz]++;     
            }
            std::cout<<"Incident_Ang_X: "<<Incident_Ang_X*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Incident_Ang_Y: "<<Incident_Ang_Y*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Exit_Ang_X: "<<Exit_Ang_X*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Exit_Ang_Y: "<<Exit_Ang_Y*180/TMath::Pi()<<" degree"<<std::endl;
        }

    }
    f->Close();

    struct PoCA_Result{
        //coordinates of voxel
        double fx;
        double fy;
        double fz;
        //reconstructed scatter density
        double fs;
        //number of tracks that passes the voxel
        int ntrack;
    };
    PoCA_Result result;
    //output file
    TFile *fout = new TFile("PoCA_Result.root","recreate");
    TTree *tout = new TTree("PoCA","Result of PoCA algorithm");
    tout->Branch("Scatt_density",&result.fx,"fx/D:fy/D:fz/D:scatt_density/D:ntrack/I");

    // Renew scatter density matrix
    TH3F *h3 = new TH3F("Muon Image","Muon Image",NBin_X,-HalfX, HalfX,NBin_Y,-HalfY,HalfY
               ,NBin_Z,-HalfZ,HalfZ);
    for(Int_t nx = 0; nx < NBin_X; nx++){
        for(Int_t ny = 0; ny < NBin_Y; ny++){
            for(Int_t nz = 0; nz < NBin_Z; nz++){
                if(Ntrack[nx][ny][nz] == 0) Scatt_Density[nx][ny][nz] = 0;
                else{
                      Scatt_Density[nx][ny][nz] = Scatt_Density[nx][ny][nz]/Ntrack[nx][ny][nz];
                      h3->SetBinContent(nx+1,ny+1,nz+1,Scatt_Density[nx][ny][nz]);
                }
                result.fx = nx*dX-HalfX+dX/2;
                result.fy = ny*dY-HalfY+dY/2;
                result.fz = nz*dZ-HalfZ+dZ/2;
                result.fs = Scatt_Density[nx][ny][nz];
                result.ntrack = Ntrack[nx][ny][nz];
                tout->Fill();
            }
        }
    }
    tout->Print();
    tout->Write();
    // Plot scatter density
    gStyle->SetCanvasPreferGL(1);
    TCanvas *c = new TCanvas("glc","MuImag",0,0,800,500);
    TList *lf = h3->GetListOfFunctions();
    if(lf){
        TF1 *tf = new TF1("TransferFunction",my_transfer_function);
        lf->Add(tf);
    }
    gStyle->SetPalette(1);
    h3->Draw("glcolz");
    std::cout <<"done" << std::endl;
}

void PoCA(){
    const double ObjectArea_halfsize[3] = {Plate_Size_X/20,Plate_Size_Y/20,ObjectAreaWidth/20};
    const Int_t NBins[3] = {20,20,20};
    Imag("../rawdata.root",ObjectArea_halfsize,NBins);
}
