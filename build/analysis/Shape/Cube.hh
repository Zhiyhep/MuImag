// Class for defining cubes in space
// Writer: Zhi Yu
// Date: 2018/2/23
// Members: -> fXDim, fYDim, fZDim: dimensions of the cube.
//          -> Contain(): If the voxel's center is in the cube then return true, 
//                        else return false.
//
#ifndef Cube_H
#define Cube_H

#include "Volume.hh"
#include "TMath.h"
class Cube:public Volume
{
    public:
        Cube(double x,double y, double z,double a, double b, double c):
        Volume(x,y,z){
            fXDim = a;
            fYDim = b;
            fZDim = c;
        }
        virtual ~Cube();
        virtual bool Contain(double x,double y,double z){
            return (TMath::Abs(x-fX) <= fXDim/2) && (TMath::Abs(y-fY) <= fYDim/2) && (TMath::Abs(z-fZ) <= fZDim/2);
        }
        double fXDim, fYDim, fZDim;
 
};
#endif