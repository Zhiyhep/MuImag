// Ordinary least square functor
// Writer: Zhi Yu
// Date: 2018/3/18
// 

#include "TString.h"
#include "TFile.h"
#include "TMath.h"
#include "TParameter.h"
#include "TTree.h"
#include <vector>
#include <iostream>

class OLS_FCN
{
  public:
     OLS_FCN(TString file="PoCA_Result.root", TString info="ML_Info", TString poca="PoCA");
    virtual ~OLS_FCN(){};
    double operator()(const double *);
    double* Init();
    int GetNVariable() const;
  private:
    TFile* f;// file name
    TTree* t_ml;// ml info tree name
    TTree* t_poca;// poca tree name
    //tempal variables
    int fNVoxel;
    int* fIndex;
    double* fLength;
    double fScatt_Ang_X2;
    double fScatt_Ang_Y2;
    int poca_index;
    double fs;
    double* init_value;
  public:
    //meta data
    int NBin_X, NBin_Y, NBin_Z;
};

OLS_FCN::OLS_FCN(TString file, TString info, TString poca)
{
 fIndex = new int[200];
 fLength = new double[200];
 f = new TFile(file,"readonly");
 t_ml = (TTree*)f->Get(info);
 t_poca = (TTree*)f->Get(poca);
 TBranch *br1 = t_ml->GetBranch("fNVoxel");
 TBranch *br2 = t_ml->GetBranch("fIndex");
 TBranch *br3 = t_ml->GetBranch("fLength");
 TBranch *br4 = t_ml->GetBranch("fScatt_Ang_X2");
 TBranch *br5 = t_ml->GetBranch("fScatt_Ang_Y2");
 br1->SetAddress(&fNVoxel);
 br2->SetAddress(fIndex);
 br3->SetAddress(fLength);
 br4->SetAddress(&fScatt_Ang_X2);
 br5->SetAddress(&fScatt_Ang_Y2); 
 TBranch* br6 = t_poca->GetBranch("fIndex");
 TBranch* br7 = t_poca->GetBranch("fScatt_Density");
 br6->SetAddress(&poca_index);
 br7->SetAddress(&fs);

 TParameter<int> *x_par, *y_par, *z_par;
 x_par = (TParameter<int>*)f->Get("NBin_X");
 y_par = (TParameter<int>*)f->Get("NBin_Y");
 z_par = (TParameter<int>*)f->Get("NBin_Z");
 NBin_X = x_par->GetVal();
 NBin_Y = y_par->GetVal();
 NBin_Z = z_par->GetVal();
}

double OLS_FCN::operator()(const double* lambda)
{
 double result = 0;
 int nentry = t_ml->GetEntries();
 for(int i =0; i < nentry; i++)
 {
   t_ml->GetEntry(i);
   double temp = 0;
   for(int j=0; j < fNVoxel; j++)
   {
     int index = fIndex[j];
     temp+=fLength[j]*lambda[index];
    // std::cout << i<<": Length, lambda: " << fLength[j] << "," << lambda[index] << std::endl;
   }
   //std::cout<<"temp: "<<temp <<std::endl;
   double error = temp-(fScatt_Ang_X2+fScatt_Ang_Y2)/2;
   result+=error*error;
 }
 //  std::cout << result << std::endl;
 return result;
}

double* OLS_FCN::Init()
{
  int nvariable = t_poca->GetEntries();
  init_value = new double[nvariable];
  for(int i = 0; i < nvariable; i++)
  {
    t_poca->GetEntry(i);
    init_value[poca_index] = fs;
  }
  return init_value;
}

int OLS_FCN::GetNVariable() const 
{
  return t_poca->GetEntries();
}

