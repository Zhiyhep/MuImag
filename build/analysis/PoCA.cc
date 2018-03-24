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
#include "TH2.h"
#include "TStyle.h"
#include "TList.h"
#include "TCanvas.h"
#include "TParameter.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>

#include "DetectorLayout.hh"

Double_t my_transfer_function(const Double_t *x, const Double_t*)
{
  if(*x < 3) return 0;
  if(*x < 26.) return 0.1;
  if(*x < 78.) return 0.3; 
  return 0.6;
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
    Int_t*** Ntrack = new Int_t**[NBin_X];
    Double_t*** Scatt_Density = new Double_t**[NBin_X];
    for(Int_t i = 0; i < NBin_X;i++){
      Ntrack[i] = new Int_t*[NBin_Y];
      Scatt_Density[i] = new Double_t*[NBin_Y];
    }
    for(Int_t i = 0; i < NBin_X; i++){
       for(Int_t j = 0; j < NBin_Y; j++){
         Ntrack[i][j] = new Int_t[NBin_Z];
         Scatt_Density[i][j] = new Double_t[NBin_Z];
       }
    }
    for(Int_t i = 0; i < NBin_X; i++){
        for(Int_t j = 0; j < NBin_Y; j++){
            for(Int_t k = 0; k < NBin_Z; k++){
                Ntrack[i][j][k] = 0;
                Scatt_Density[i][j][k] = 0.;
            }
        }
    }
    struct PoCA_Result{
        //coordinates of voxel
        double fx;
        double fy;
        double fz;
        //reconstructed scatter density
        double fs;
        //number of tracks that passes the voxel
        int ntrack;
        //index of the voxel
        int index;
    };
    // Infomation of each ray
    struct Ray_Info{
        Int_t NVoxel; // Number of voxels the ray passes
        Int_t Index[200]; // The index of the transversed voxels
        Double_t Length[200]; // The pass length of the transversed voxels
        double Scatt_Ang_X2; // Square of scatter angle in x direction
        double Scatt_Ang_Y2; // Square of scatter angle in y direction
    };
    // Information of each voxel
    struct Voxel_Info{
        Int_t Index; // Index of the voxel
        Int_t NRay; // Number of rays that pass the voxel
        Int_t Event_no[20000]; // The index of the rays that pass the voxel
        double Length[20000]; // The pass length of the rays in the voxel
    };
    PoCA_Result result;
    Ray_Info rinfo;
    std::map<int, std::vector<int>> vox_index;
    std::map<int, std::vector<double>> vox_length;
    Voxel_Info v_info;
    rinfo.NVoxel = NBin_Z;
    //output file
    TFile *fout = new TFile("PoCA_Result.root","recreate");
    TTree *tout = new TTree("PoCA","Result of PoCA algorithm");
    TTree *t_ray = new TTree("Ray_Info","Ray info for maximum likelihood"); 
    TTree *t_voxel = new TTree("Voxel_Info","Voxel info for maximum likelihood");
    tout->Branch("fx",&result.fx,"fx/D");
    tout->Branch("fy",&result.fy,"fy/D");
    tout->Branch("fz",&result.fz,"fz/D");
    tout->Branch("fScatt_Density",&result.fs,"fScatt_Density/D");
    tout->Branch("fNtrack",&result.ntrack,"fNtrack/I");
    tout->Branch("fIndex",&result.index,"fIndex/I");
    t_ray->Branch("fNVoxel",&rinfo.NVoxel,"fNVoxel/I");
    t_ray->Branch("fIndex",rinfo.Index,"fIndex[fNVoxel]/I");
    t_ray->Branch("fLength",rinfo.Length,"fLength[fNVoxel]/D");
    t_ray->Branch("fScatt_Ang_X2",&rinfo.Scatt_Ang_X2,"fScatt_Ang_X2/D");
    t_ray->Branch("fScatt_Ang_Y2",&rinfo.Scatt_Ang_Y2,"fScatt_Ang_Y2/D");
    t_voxel->Branch("fIndex",&v_info.Index,"fIndex/I");
    t_voxel->Branch("fNRay",&v_info.NRay,"fNRay/I");
    t_voxel->Branch("fEventNo",&v_info.Event_no,"fEventNo[fNRay]/I");
    t_voxel->Branch("fLength",&v_info.Length,"fLength[fNRay]/D");
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
        rinfo.Scatt_Ang_X2 = TMath::Power(Exit_Ang_X-Incident_Ang_X,2)*1e6;
        rinfo.Scatt_Ang_Y2 = TMath::Power(Exit_Ang_Y-Incident_Ang_Y,2)*1e6;
        //std::cout<<"Pt: ("<<Pt.x()<<", "<<Pt.y()<<", "<<Pt.z()<<")"<<std::endl;
        if(TMath::Abs(Pt.x())<=HalfX && TMath::Abs(Pt.y())<=HalfY && TMath::Abs(Pt.z())<=HalfZ){
            //Calculate scatter density [mrad^2/cm]
            Double_t S = (rinfo.Scatt_Ang_X2+rinfo.Scatt_Ang_Y2)/dZ/2*TMath::Cos((Incident_Ang+Exit_Ang)/2);
            // Index of scatter point
            Int_t Pt_nx = TMath::Floor((Pt.x()+HalfX)/dX);
            Int_t Pt_ny = TMath::Floor((Pt.y()+HalfY)/dY);
            Int_t Pt_nz = TMath::Floor((Pt.z()+HalfZ)/dZ);
            Ntrack[Pt_nx][Pt_ny][Pt_nz]++;
            Scatt_Density[Pt_nx][Pt_ny][Pt_nz]+=S;
            
            for(Int_t nz = 0; nz < Pt_nz; nz++){
                // The line function: P = *pos3-L*Exit_Dir
                Double_t L = ((*pos3).z() - ((nz+0.5)*dZ-HalfZ))/Exit_Dir.z();
                rinfo.Length[nz] = -dZ/Exit_Dir.z();
                // Interac point of exit ray and layer nz  
                TVector3 P = *pos3 - L*Exit_Dir;
                // Index of P
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
               // std::cout<<P_nx<<", "<<P_ny<<std::endl;
                int vindex = P_nx+NBin_X*P_ny+NBin_X*NBin_Y*nz;
                rinfo.Index[nz] = vindex;
                vox_index[vindex].push_back(i);
                vox_length[vindex].push_back(rinfo.Length[nz]);
                Ntrack[P_nx][P_ny][nz]++;
            } 
            rinfo.Length[Pt_nz]  = dZ/TMath::Cos((Incident_Ang+Exit_Ang)/2);
            int vindex = Pt_nx+NBin_X*Pt_ny+NBin_X*NBin_Y*Pt_nz;
            rinfo.Index[Pt_nz] = vindex;
            vox_index[vindex].push_back(i);
            vox_length[vindex].push_back(rinfo.Length[Pt_nz]);
            for(Int_t nz = Pt_nz+1; nz < NBin_Z; nz++){
                // The line function: P = *pos2+L*Incident_Dir
                Double_t L = (((nz+0.5)*dZ-HalfZ)-(*pos2).z())/Incident_Dir.z();
                rinfo.Length[nz] = -dZ/Incident_Dir.z();
                // Interac point of exit ray and layer nz  
                TVector3 P = *pos2 + L*Incident_Dir;
                // Index of P
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
                //std::cout<<P_nx<<", "<<P_ny<<std::endl;
                int vindex = P_nx+NBin_X*P_ny+NBin_X*NBin_Y*nz;
                rinfo.Index[nz] = vindex;
                vox_index[vindex].push_back(i);
                vox_length[vindex].push_back(rinfo.Length[nz]);
                Ntrack[P_nx][P_ny][nz]++;
            } 
        }
        else{
            TVector3 Mid_Dir = (Incident_Dir+Exit_Dir)*0.5;
            Mid_Dir = Mid_Dir.Unit();
            for(int nz = 0;nz < NBin_Z;nz++){
                // The line function: P = *pos2+L*Incident_Dir
                Double_t L = (((nz+0.5)*dZ-HalfZ)-(*pos2).z())/Incident_Dir.z(); 
                rinfo.Length[nz] = dZ/TMath::Cos((Incident_Ang+Exit_Ang)/2); 
                TVector3 P = *pos2 + L*Incident_Dir;
                Int_t P_nx = TMath::Floor((P.x()+HalfX)/dX);
                Int_t P_ny = TMath::Floor((P.y()+HalfY)/dY);
                if(P_nx < 0) P_nx = 0;
                else if(P_nx >= NBin_X) P_nx = NBin_X-1;
                if(P_ny < 0) P_ny = 0;
                else if(P_ny >= NBin_Y) P_ny = NBin_Y-1;
                std::cout <<"nx, ny: "<< P.x() <<", "<<P.y()<<std::endl;
                int vindex = P_nx+NBin_X*P_ny+NBin_X*NBin_Y*nz;
                rinfo.Index[nz] = vindex;
                vox_index[vindex].push_back(i);
                vox_length[vindex].push_back(rinfo.Length[nz]);
                Ntrack[P_nx][P_ny][nz]++;     
            }
            std::cout<<"Incident_Ang_X: "<<Incident_Ang_X*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Incident_Ang_Y: "<<Incident_Ang_Y*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Exit_Ang_X: "<<Exit_Ang_X*180/TMath::Pi()<<" degree"<<std::endl;
            std::cout<<"Exit_Ang_Y: "<<Exit_Ang_Y*180/TMath::Pi()<<" degree"<<std::endl;
        }
        for(int i = 0; i < NBin_Z;i++) std::cout << rinfo.Length[i] << std::endl;
        t_ray->Fill();
    }
    f->Close();

    // Renew scatter density matrix
    TH3F *h3 = new TH3F("Muon Image","PoCA Image",NBin_X,-HalfX, HalfX,NBin_Y,-HalfY,HalfY
               ,NBin_Z,-HalfZ,HalfZ);
    /*
    TH2F *hyz = new TH3F("hyz","Muon Image (y-z)",NBin_Y,-HalfY,HalfY,NBin_Z,-HalfZ,HalfZ);
    TH2F *hxz = new TH3F("hyz","Muon Image (x-z)",NBin_X,-HalfX,HalfX,NBin_Z,-HalfZ,HalfZ);
    TH2F *hxy = new TH3F("hxy","Muon Image (x-y)",NBin_X,-HalfX,HalfX,NBin_Y,-HalfY,HalfY);
    */
    for(Int_t nx = 0; nx < NBin_X; nx++){
        for(Int_t ny = 0; ny < NBin_Y; ny++){
            for(Int_t nz = 0; nz < NBin_Z; nz++){
                if(Ntrack[nx][ny][nz] == 0) Scatt_Density[nx][ny][nz] = 2.3; // Air Scatter Density 
                else{
                      Scatt_Density[nx][ny][nz] = Scatt_Density[nx][ny][nz]/Ntrack[nx][ny][nz];
                      if(Scatt_Density[nx][ny][nz] > 200) Scatt_Density[nx][ny][nz] = 200;
                      if(Scatt_Density[nx][ny][nz] < 2.3) Scatt_Density[nx][ny][nz] = 2.3;
                }
                h3->SetBinContent(nx+1,ny+1,nz+1,Scatt_Density[nx][ny][nz]);
                result.fx = nx*dX-HalfX+dX/2;
                result.fy = ny*dY-HalfY+dY/2;
                result.fz = nz*dZ-HalfZ+dZ/2;
                result.fs = Scatt_Density[nx][ny][nz];
                result.ntrack = Ntrack[nx][ny][nz];
                result.index = nx+NBin_X*ny+NBin_X*NBin_Y*nz;
                tout->Fill();
            }
        }
    }
    int voxel_num = NBin_X*NBin_Y*NBin_Z;
    for(int i=0; i < voxel_num; i++)
    {
      v_info.Index = i;
      v_info.NRay = vox_index[i].size();
      for(int j=0; j < v_info.NRay; j++) 
      {
        v_info.Event_no[j] = vox_index[i][j];
        v_info.Length[j] = vox_length[i][j];
      }
      t_voxel->Fill();
    }
    tout->Print();
    t_ray->Print();
    t_voxel->Print();
    tout->Write();
    t_ray->Write();
    t_voxel->Write();
    // Plot scatter density
    gStyle->SetCanvasPreferGL(1);
    TCanvas *c = new TCanvas("glc","MuImag",0,0,HalfX*40,HalfZ*40);
    TList *lf = h3->GetListOfFunctions();
    if(lf){
        TF1 *tf = new TF1("TransferFunction",my_transfer_function);
        lf->Add(tf);
    }
    gStyle->SetPalette(1);
    c->Divide(2,2);
    c->cd(1);
    TH2F *h_zx = (TH2F*) h3->Project3D("zx");
    h_zx->SetStats(0);
    h_zx->Draw();
    c->cd(2);
    h3->Project3D("zy")->SetStats(0);
    h3->Project3D("zy")->Draw();
    c->cd(3);
    h3->Project3D("yx")->SetStats(0);
    h3->Project3D("yx")->Draw();
    c->cd(4);
    h3->Draw("glcolz");
    std::cout <<"done" << std::endl;
    for(Int_t i = 0; i < NBin_X; i++){
       for(Int_t j = 0; j < NBin_Y; j++){
          delete[] Ntrack[i][j];
          delete[] Scatt_Density[i][j];
       }
    }
    for(Int_t i = 0;i<NBin_X;i++){
      delete[] Ntrack[i];
      delete[] Scatt_Density[i];
    }
    delete[] Ntrack;
    delete[] Scatt_Density;
    
    // Save useful parameters
    TParameter<int> NBin_X_par("NBin_X",NBin_X);
    TParameter<int> NBin_Y_par("NBin_Y",NBin_Y);
    TParameter<int> NBin_Z_par("NBin_Z",NBin_Z);
    fout->WriteTObject(&NBin_X_par,NBin_X_par.GetName());
    fout->WriteTObject(&NBin_Y_par,NBin_Y_par.GetName());
    fout->WriteTObject(&NBin_Z_par,NBin_Z_par.GetName());
}

void PoCA(){
    const double ObjectArea_halfsize[3] = {Plate_Size_X/20,Plate_Size_Y/20,ObjectAreaWidth/20};
    const Int_t NBins[3] = {20,20,20};
    Imag("../rawdata.root",ObjectArea_halfsize,NBins);
}
 
int main()
{
  PoCA();
  return 0;
}
