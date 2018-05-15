// Class for defining columns in space
// Writer: Zhi Yu
// Date: 2018/2/23
// Members: -> fR: radius of the cross-section.
//          -> fH: height of the column
//          -> Contain(): If the voxel's center is in the column then return true, 
//                        else return false.
//
#ifndef Column_H
#define Column_H
#include "Volume.hh"
#include "TMath.h"

class Column: public Volume
{
    public:
        Column(double x,double y,double z,double r,double h):Volume(x,y,z)
        {
            fR = r;
            fH = h;
        }
        virtual ~Column();
        virtual bool Contain(double x, double y, double z){
            double R2 = TMath::Power((x-fX),2)+TMath::Power((y-fY),2);
            return (R2 <= fR*fR) && (TMath::Abs(z-fZ) <= fH/2);
        }
        double fR, fH;
};
#endif
