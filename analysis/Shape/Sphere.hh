// Class for defining spheres in space
// Writer: Zhi Yu
// Date: 2018/2/23
// Members: -> fR: radius of the sphere.
//          -> Contain(): If the voxel's center is in the sphere then return true, 
//                        else return false.
//
#ifndef Sphere_H
#define Sphere_H

#include "Volume.hh"
#include "TMath.h"
class Sphere: Volume
{
    public:
        Sphere(double x,double y,double z,double r):Volume(x,y,z){
            fR = r;
        }
        virtual ~Sphere();
        virtual bool Contain(double x, double y, double z){
            double R2 = TMath::Power((x-fX),2)+TMath::Power((y-fY),2)+TMath::Power((z-fZ),2);
            return R2 <= fR*fR;
        }
        double fR;
};
#endif