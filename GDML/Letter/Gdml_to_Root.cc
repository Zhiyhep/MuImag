// Convert gdml file to root file
// Writer: Zhi Yu
// Date: 2018/1/29
//
#include <TGeoManager.h>
void Gdml_to_Root(TString gdml){
    TGeoManager *geo = new TGeoManager();
    geo->Import(gdml);
    geo->Export(gdml.ReplaceAll(".gdml",".root"));
}
