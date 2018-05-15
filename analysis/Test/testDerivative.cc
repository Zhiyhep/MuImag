#include "Math/Functor.h"
#include "OLS_FCN.hh"
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
#include "Math/RichardsonDerivator.h"
#include "DetectorLayout.hh"

int testDerivative()
{
  
  OLS_FCN lf = OLS_FCN("PoCA_Result.root","ML_Info","PoCA");
  int nvariable = lf.GetNVariable();
  ROOT::Math::Functor f(lf, nvariable);
  double *init_value = lf.Init();
  ROOT::Math::RichardsonDerivator rd;
  rd.SetFunction(f);
  lf(init_value);
  //minim->SetFunction(f);
  for(int i=0;i<nvariable;i++)
  {
    const string name = "x" + std::to_string(i);
  }

  return 0;  
}
