#include "Likelihood_FCN.hh"
#include <iostream>
void test(int j)
{
  Likelihood_FCN lf;
  double* v = lf.Init();
  int n = lf.GetNVariable();
  double* f = new double[n];
  TGraph* gr = new TGraph();
  for(int i = 0; i<100;i++)
  {
    v[j] = i;
    f[i] = lf(v)/1e6;
    std::cout << i <<": " << f[i] << std::endl;
    gr->SetPoint(i,v[j],f[i]);
  }
  gr->Draw();
}
