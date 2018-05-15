#include <iostream>
#include <TROOT.h>
#include <TApplication.h>

#include "ViewField.hh"
#include "ViewCell.hh"
#include "ComponentAnalyticField.hh"
#include "MediumMagboltz.hh"
#include "SolidBox.hh"
#include "GeometrySimple.hh"
#include "Sensor.hh"
#include "ViewDrift.hh"
#include "FundamentalConstants.hh"
#include "DriftLineRKF.hh"
#include "ViewMedium.hh"
#include "ViewSignal.hh"
#include "Random.hh"
#include "TrackHeed.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  TApplication app("app", &argc, argv);
  
  // Switch between IROC and OROC.
  const bool iroc = false;
  // Switch gating on or off.
  bool gating = false;

  // Voltage settings [V].
  const double vSens = iroc ? 1460. : 1570.;
  const double vGate = -70.;
  const double deltav = 90.; // for closed gate mode
 
  // y-axis gap between rows of wires [cm]
  const double gap = iroc ? 0.2 : 0.3;
 
  // y coordinates of the wires [cm]
  const double ys = gap;            // anode wires
  const double yc = 2. * gap;       // cathode
  const double yg = 2. * gap + 0.3; // gate
 
  // HV plane (drift field)
  const double yHV = 249.7;
  const double vHV = -100000;
 
  // Periodicity (wire spacing)
  const double period = 0.25;
  const int nRep = 10;
  
  const double dc = period;
  const double dg = period / 2;
 
  // Wire diameters [cm]
  const double dSens = 0.0020;
  const double dCath = 0.0075;
  const double dGate = 0.0075;
 
  // Setup the gas.
  MediumMagboltz* gas = new MediumMagboltz();
  // Set the temperature [K] and pressure [Torr].
  const double pressure = 750.;
  const double temperature = 293.15;
  gas->SetTemperature(temperature);
  gas->SetPressure(pressure);
  // Set the gas mixture.
  gas->SetComposition("ne", 85.72, "co2", 9.52, "n2", 4.76);
  // Read the electron transport coefficients from a .gas file.
  gas->LoadGasFile("Ne_90_CO2_10_N2_5_with_mg.gas");
  // Read the ion mobility table from file.
  const std::string garfpath = std::getenv("GARFIELD_HOME");
  gas->LoadIonMobility(garfpath + "/Data/IonMobility_Ne+_Ne.txt");

  // Build the geometry.
  GeometrySimple* geo = new GeometrySimple();
  SolidBox* box = new SolidBox(0., yHV/2, 0,
                               (nRep) * period, yHV/2, 1);

  // Add the solid to the geometry, together with the medium inside.
  geo->AddSolid(box, gas);

  // Setup the electric field for the electrons (gating open).
  ComponentAnalyticField* comp_e = new ComponentAnalyticField();
  comp_e->SetPeriodicityX(nRep * period);
  for (int i = 0; i < nRep; ++i) {
   comp_e->AddWire((i - 2.) * period, ys, dSens, vSens, "s");
  }
  for (int i = 0; i < nRep+5; ++i) {
    comp_e->AddWire(dc * (i + 0.5), yc, dCath, 0., "c");
  }
  if (gating) {
    for (int i = 0; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5);
      comp_e->AddWire(xg, yg, dGate, vGate+deltav, "g+");
    }
    for (int i = 1; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5);
      comp_e->AddWire(xg, yg, dGate, vGate-deltav, "g-");
    }
  } else {
    for (int i = 0; i < nRep * 7; ++i) {
      const double xg = dg * (i + 0.5);
      comp_e->AddWire(xg, yg, dGate, vGate, "g", 100., 50., 19.3, 1);
    }
  }
  // Add the planes.
  comp_e->AddPlaneY(0., 0., "pad_plane");
  comp_e->AddPlaneY(yHV, vHV, "HV");
 
  // Set the magnetic field [T].
  comp_e->SetMagneticField(0, 0.5, 0);
  comp_e->SetGeometry(geo);

  // Create a canvas that will also be used for drift lines plotting.
  TCanvas* myCanvas = new TCanvas();
  ViewCell* cellView = new ViewCell();
  cellView->SetComponent(comp_e);
  cellView->SetArea(-0.5, 0, -2, 0.5, 5 * gap, 2);
  cellView->SetCanvas(myCanvas);
  cellView->Plot2d();
  myCanvas->Update();

  const int nWires = comp_e->GetNumberOfWires();
  for (int i = 0; i < nWires; ++i) {
    double xw, yw, dw, vw;
    std::string lbl;
    double lw, qw;
    int nw;
    comp_e->GetWire(i, xw, yw, dw, vw, lbl, lw, qw, nw);
    std::cout << i << "   " << lbl << "   " << xw << "   " << yw << "   " << nw << std::endl;
  }

  // Make a sensor for the electrons (gating open).
  Sensor* sensor_e = new Sensor();
  sensor_e->AddComponent(comp_e);

  // Plot isopotential contours
  ViewField* fView = new ViewField;
  fView->SetSensor(sensor_e);
  fView->SetArea(-nRep * period / 2., 0.,
                  nRep * period / 2., 2);
  fView->SetVoltageRange(-400., 1000.);
  fView->PlotContour();
 
  ViewDrift* vd = new ViewDrift();
  vd->SetCanvas(myCanvas);
  vd->SetArea(-0.5, 0, -2, 0.5, 5 * gap, 2);

  DriftLineRKF* driftline_e = new DriftLineRKF();
  driftline_e->SetSensor(sensor_e);
  driftline_e->EnablePlotting(vd);

  // Use a separate sensor for the ions (gating switched on).
  gating = true;
  // Setup the electric field
  ComponentAnalyticField* comp_i = new ComponentAnalyticField();
  comp_i->SetPeriodicityX(nRep * period);
  for (int i = 0; i < nRep+10; ++i) {
   comp_i->AddWire((i - 2.) * period, ys, dSens, vSens, "s");
  }
  for (int i = 0; i < nRep +5; ++i) {
    comp_i->AddWire(dc * (i + 0.5), yc, dCath, 0., "c");
  }
  if (gating) {
    for (int i = 0; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5); 
      comp_i->AddWire(xg, yg, dGate, vGate + deltav, "g+");
    }
    for (int i = 1; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5); 
      comp_i->AddWire(xg, yg, dGate, vGate - deltav, "g-");
    }
  } else {
    for (int i = 0; i < nRep * 7; ++i) {
      const double xg = dg * (i + 0.5); 
      comp_i->AddWire(xg, yg, dGate, vGate, "g", 100., 50., 19.3, 1);
    }
  }
  comp_i->AddPlaneY(0., 0., "pad_plane");
  comp_i->AddPlaneY(yHV, vHV, "HV");
 
  // Set the magnetic field [T].
  comp_i->SetMagneticField(0,0.5,0);
  comp_i->SetGeometry(geo);

  // Make a sensor for ions (gating closed).
  Sensor* sensor_i = new Sensor();
  sensor_i->AddComponent(comp_i);
  sensor_i->AddElectrode(comp_i, "pad_plane");

  // Polya parameter for gain distribution (for Ne)
  const double theta = 0.4;
  const double avg_gain = 5;

  // Gain per avalanche follows a Polya distribution
  double gain = RndmPolya(theta) * avg_gain; 

  double r = 0.01;

  DriftLineRKF* driftline_i = new DriftLineRKF();
  driftline_i->SetSensor(sensor_i);
  driftline_i->EnablePlotting(vd);
 
  // Set up the charged particle track.
  TrackHeed* track = new TrackHeed();
  track->SetSensor(sensor_e);
  track->SetParticle("pi");
  // Set the momentum [eV / c].
  const double momentum = 1.e9;
  track->SetMomentum(momentum);
  // Set the starting point and direction of the track.
  double x0 = (-nRep + 1) * period;
  double y0 = 1., z0 = 0., t0 = 0.;
  double dx0 = 1., dy0 = 0., dz0 = 0.;
  // Simulate a track.
  track->NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

  // Retrieve the clusters. 
  double xcl, ycl, zcl, tcl, ecl, extra;
  int ncl = 0;
  while (track->GetCluster(xcl, ycl, zcl, tcl, ncl, ecl, extra)) {
    // Retrieve the electrons of the cluster.
    for (int i = 0; i < ncl; ++i) {
      double x, y, z, t, e, dx, dy, dz;
      track->GetElectron(i, x, y, z, t, e, dx, dy, dz);
      // Smear the coordinates to account for the drift up to the ROC.
      const double dT = 0.0198;
      double ycheat = std::max(y, 1.2);
      driftline_e->DriftElectron(RndmGaussian(x, dT * sqrt(ycheat - 1.2)), 
                                 1.2, 
                                 RndmGaussian(z, dT * sqrt(ycheat - 1.2)), 0);  
      double xendpoint = 0, yendpoint = gap;
      double dummy = 0.;
      int status = 0;
      driftline_e->GetEndPoint(xendpoint, yendpoint, dummy, dummy, status);
      if (yendpoint > 0.5 * gap && yendpoint < 1.5 * gap) {
        // Electron drifted to a sensing wire
        for (int j = 0; j < gain; ++j) {
          const double angle = RndmGaussian(0, 1.4);
          driftline_i->DriftIon(xendpoint + r * sin(angle), 
                                gap + r * cos(angle), 0, 0);
        }
      }
    }
  }
  vd->Plot(true, false);
 
  app.Run(kTRUE);

}
