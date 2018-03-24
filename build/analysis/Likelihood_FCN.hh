// Likelihood functor
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

class Likelihood_FCN
{
  public:
    Likelihood_FCN(TString file="PoCA_Result.root", TString rinfo="Ray_Info", TString vinfo="Voxel_Info", TString poca="PoCA");
    virtual ~Likelihood_FCN();
    double operator()(const double *) const;
    double* Init();
    int GetNVariable();
    double* Derivative(const double*) const;
  private:
    TFile* f;// file name
    TTree* t_ray;// ray info tree name
    TTree* t_voxel; // voxel info tree name
    TTree* t_poca;// poca tree name
    //tempal variables
    // ray info variables
    int fNVoxel;
    int* fIndex;
    double* fLength;
    double fScatt_Ang_X2;
    double fScatt_Ang_Y2;
    // voxel info variables
    int voxel_index;
    int ray_no;
    int* event_no;
    double* vox_length;
    // poca result variables
    int poca_index;
    double fs;
    double* init_value;
    double* derivative;
  public:
    //meta data
    int NBin_X, NBin_Y, NBin_Z;
    int fNVariable;
};

Likelihood_FCN::Likelihood_FCN(TString file, TString rinfo, TString vinfo, TString poca)
{
 fIndex = new int[200];
 fLength = new double[200];
 event_no = new int[20000];
 vox_length = new double[20000];
 f = new TFile(file,"readonly");
 t_ray = (TTree*)f->Get(rinfo);
 t_voxel = (TTree*)f->Get(vinfo);
 t_poca = (TTree*)f->Get(poca);
 TBranch *br1 = t_ray->GetBranch("fNVoxel");
 TBranch *br2 = t_ray->GetBranch("fIndex");
 TBranch *br3 = t_ray->GetBranch("fLength");
 TBranch *br4 = t_ray->GetBranch("fScatt_Ang_X2");
 TBranch *br5 = t_ray->GetBranch("fScatt_Ang_Y2");
 br1->SetAddress(&fNVoxel);
 br2->SetAddress(fIndex);
 br3->SetAddress(fLength);
 br4->SetAddress(&fScatt_Ang_X2);
 br5->SetAddress(&fScatt_Ang_Y2); 
 TBranch* br6 = t_voxel->GetBranch("fIndex");
 TBranch* br7 = t_voxel->GetBranch("fNRay");
 TBranch* br8 = t_voxel->GetBranch("fEventNo");
 TBranch* br9 = t_voxel->GetBranch("fLength");
 br6->SetAddress(&voxel_index);
 br7->SetAddress(&ray_no);
 br8->SetAddress(event_no);
 br9->SetAddress(vox_length);
 TBranch* br10 = t_poca->GetBranch("fIndex");
 TBranch* br11 = t_poca->GetBranch("fScatt_Density");
 br10->SetAddress(&poca_index);
 br11->SetAddress(&fs);
 
 fNVariable = t_poca->GetEntries();
 init_value = new double[fNVariable];
 derivative = new double[fNVariable];
 
 TParameter<int> *x_par, *y_par, *z_par;
 x_par = (TParameter<int>*)f->Get("NBin_X");
 y_par = (TParameter<int>*)f->Get("NBin_Y");
 z_par = (TParameter<int>*)f->Get("NBin_Z");
 NBin_X = x_par->GetVal();
 NBin_Y = y_par->GetVal();
 NBin_Z = z_par->GetVal();
}

Likelihood_FCN::~Likelihood_FCN()
{
  delete[] fIndex;
  delete[] fLength;
  delete[] event_no;
  delete[] vox_length;
  delete[] init_value;
  delete[] derivative;
}
double Likelihood_FCN::operator()(const double* lambda) const
{
 double result = 0;
 int nentry = t_ray->GetEntries();
 for(int i =0; i < nentry; i++)
 {
   t_ray->GetEntry(i);
   double temp = 0;
   for(int j=0; j < fNVoxel; j++)
   {
     int index = fIndex[j];
     temp+=fLength[j]*lambda[index];
    // std::cout << i<<": Length, lambda: " << fLength[j] << "," << lambda[index] << std::endl;
   }
   //std::cout<<"temp: "<<temp <<std::endl;
   result+=2*TMath::Log(temp)+(fScatt_Ang_X2+fScatt_Ang_Y2)/temp;
 }
 //std::cout << result << std::endl;
 return result;
}

double* Likelihood_FCN::Init()
{
  for(int i = 0; i < fNVariable; i++)
  {
    t_poca->GetEntry(i);
    init_value[poca_index] = fs;
  }
  return init_value;
}

int Likelihood_FCN::GetNVariable()  
{
  return fNVariable;
}

double* Likelihood_FCN::Derivative(const double* x) const
{
  double length = 0;
  std::cout << "Derivative..." << std::endl;
  for(int i = 0; i < fNVariable; i++)
  {
    t_voxel->GetEntry(i);
    double d = 0;
    for(int j = 0; j < ray_no; j++)
    {
      int event = event_no[j];
      t_ray->GetEntry(event);
      double temp = 0;
      for(int k = 0; k < fNVoxel; k++)
      {
        int index = fIndex[k];
        temp += fLength[k]*x[index];
      }
      d+=2*vox_length[j]/temp-(fScatt_Ang_X2+fScatt_Ang_Y2)*vox_length[j]/temp/temp;
    }
    //std::cout << "d: " << d <<" "<<i<<"/"<<fNVariable<<std::endl;
    derivative[i] = d;
    length+=d*d;
    std::cout << "parameter: "<< i+1 <<"/"<<fNVariable<<"\r"<<flush;
  }
  std::cout << std::endl;
  length = TMath::Sqrt(length);
  /*
  for(int i = 0; i < fNVariable; i++)
  {
   derivative[i] = derivative[i]/length;
   //std::cout<<derivative[i]<<std::endl;
  }
  */
  derivative[fNVariable] = length;
  std::cout << "Gradient Norm: "<< length << std::endl;

  return derivative;
}
