//#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "OLS_FCN.hh"
#include "Likelihood_FCN.hh"
#include <iostream>
#include <string>
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TStyle.h"
#include "TList.h"
#include "TCanvas.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TStopwatch.h"
#include "DetectorLayout.hh"

double my_transfer_function(const double *x, const double*)
{
  if(*x < 3.) return 0;
  if(*x < 26.) return 0.1;
  if(*x < 78.) return 0.3;
  return 0.6;
}
int MLEM()
{
  TStopwatch timer;
  timer.Start(1);
  double learnrate = 0.002;
  double gamma = 0.9; // Momentum coefficient for Newton method
  int niteration = 20; // Number of iteration
  double target_gradient_length = 100; // Iteration ends when gradient length is below this value.
  // the minimum and maximum limitation of parameters
  double leftvalue = 0.1;
  double rightvalue = 100;
  //ROOT::Math::Minimizer* minim = ROOT::Math::Factory::CreateMinimizer("Minuit2","Migrad");
  //minim->SetMaxFunctionCalls(2000000);
  //minim->SetMaxIterations(2000000);
  //minim->SetTolerance(1);
  //minim->SetPrintLevel(4);
  Likelihood_FCN lf = Likelihood_FCN("PoCA_Result.root","Ray_Info","Voxel_Info","PoCA");
  int nvariable = lf.GetNVariable();
  //ROOT::Math::Functor f(lf, nvariable);
  double *lambda = lf.Init();
  double *gradient = 0;
  //minim->SetFunction(f);
  double *step = new double[nvariable];// step length of each iteration
  for(int i = 0; i < nvariable; i++) step[i] = 0;
  for(int n=0; n< niteration; n++)
  {
    std::cout << "iteration #" << n+1 <<": "<< std::endl;
    gradient = lf.Derivative(lambda);
    for(int i=0;i<nvariable;i++)
    {
      //const string name = "x" + std::to_string(i);
      //minim->SetLowerLimitedVariable(i,name,init_value[i],0.1,2.3);
      step[i] = gamma*step[i] + learnrate*gradient[i];
      lambda[i] -= step[i];
      if(lambda[i] < leftvalue) lambda[i] = leftvalue;
      else if(lambda[i] > rightvalue) lambda[i] = rightvalue;
    }
    std::cout << "Likelihood value: "<< lf(lambda) << std::endl;
    if(gradient[nvariable] < target_gradient_length) break;
  }
  std::cout << "End Iteration          " << std::endl;
  //minim->Minimize();
  //const double *xs = minim->X();
  int NBin_X, NBin_Y, NBin_Z;
  NBin_X = lf.NBin_X;
  NBin_Y = lf.NBin_Y;
  NBin_Z = lf.NBin_Z;
  double HalfX = Plate_Size_X/20; 
  double HalfY = Plate_Size_Y/20; 
  double HalfZ = ObjectAreaWidth/20;
  TH3F *h3 = new TH3F("Muon Image","MLS Image",NBin_X, -HalfX, HalfX, NBin_Y,
                     -HalfY, HalfY, NBin_Z, -HalfZ, HalfZ);
  for(int i=0; i < nvariable; i++)
  {
    int nz = i/(NBin_X*NBin_Y);
    int ny = (i-nz*NBin_X*NBin_Y)/NBin_X;
    int nx = i-nz*NBin_X*NBin_Y-ny*NBin_X;
    //std::cout << nx <<", "<< ny << ", "<<nz<<", "<<lambda[i]<<std::endl;
    h3->SetBinContent(nx+1, ny+1, nz+1, lambda[i]);
  } 
  // Plot
  gStyle->SetCanvasPreferGL(1);
  TCanvas *c = new TCanvas("glc","MuImag",0,0,HalfX*40,HalfZ*40);
  TList *flist = h3->GetListOfFunctions();
  if(flist)
  {
    TF1 *tf = new TF1("TransferFunction", my_transfer_function);
    flist->Add(tf);
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
  h3->Draw("glcol");
  timer.Stop();
  std::cout <<"Likelihood value: " << lf(lambda) << std::endl;
  std::cout <<"Time spent: " << timer.RealTime()<< std::endl;
  
  return 0;  
}
#ifndef __CINT__
int main()
{
MLEM();
return 0;
}
#endif
