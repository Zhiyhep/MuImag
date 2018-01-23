//Run action of muon imaging simulation

#ifndef MuRunAction_h
#define MuRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"

class G4Run;
class MuDetectorConstruction;
class PrimaryHits;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// H1D histograms are created in BeginOfRunAction() for the following 
/// physics quantities:
/// - Edep in absorber
/// - Edep in gap
/// - Track length in absorber
/// - Track length in gap
/// The same values are also saved in the ntuple.
/// The histograms and ntuple are saved in the output file in a format
/// accoring to a selected technology in B4Analysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.
///

class MuRunAction : public G4UserRunAction
{
  public:
    MuRunAction(G4String name, const MuDetectorConstruction*);
    virtual ~MuRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    TFile *file;
    TTree *tree;
    PrimaryHits* phit;
  private:
    const MuDetectorConstruction* fDetConstruction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

