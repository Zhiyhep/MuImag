// Base class for defining volumes 
// Writer: Zhi Yu
// Date: 2018/2/23
// Members: -> fX,fY,fZ: coordinates of the volume's center
//          -> Contain(x,y,z): Test if the volume contains a certain voxel,
//                             The voxel's position is (x,y,z). 
//                         
#ifndef Volume_H
#define Volume_H

class Volume
{
    public:
        Volume(): fX(0),fY(0),fZ(0){};
        Volume(double x, double y, double z){
            fX = x;
            fY = y;
            fZ = z;
        };
        virtual ~Volume();
        virtual bool Contain() = 0;
        double fX, fY, fZ;
};

#endif