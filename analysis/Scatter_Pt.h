// Calculate the mid point of the common verticle line of two lines
// Writer: Zhi Yu
// Date: 2018/1/27
//

#ifndef Scatter_Pt_H
#define Scatter_Pt_H 
#include "TVector3.h"
inline Double_t GetCos(const TVector3 M, const TVector3 N){return (M.Unit())*(N.Unit());}
TVector3 Scatter_Pt(const TVector3 P1, const TVector3 V1, const TVector3 P2, const TVector3 V2)
{
  Double_t Cos_AP1P2 = GetCos(V1,(P2-P1));
  Double_t Cos_BP2P1 = GetCos(V2,(P2-P1));
  Double_t Cos_Lines = GetCos(-V1,V2);
  Double_t a = (P2-P1).Mag();
  Double_t m = (Cos_AP1P2+Cos_BP2P1*Cos_Lines)/(1-Cos_Lines*Cos_Lines)*a;
  Double_t n = (Cos_BP2P1+Cos_AP1P2*Cos_Lines)/(1-Cos_Lines*Cos_Lines)*a;
  TVector3 A = P1+m*V1.Unit();
  TVector3 B = P2-n*V2.Unit();
  //cout<<"A: ("<<A.X()<<", "<<A.Y()<<", "<<A.Z()<<")"<<endl;
  //cout<<"B: ("<<B.X()<<", "<<B.Y()<<", "<<B.Z()<<")"<<endl;
  return (A+B)*0.5;
}
#endif
