#include <iostream>
#include <cstdlib>

#include <TCanvas.h>
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

using namespace Garfield;

// Impulse response of the PASA
double transfer(double t) {
  const double tau = 160;
  const double fC_to_mV = 12.7;
  return fC_to_mV * exp(4) * pow((t / tau), 4) * exp(-4 * t / tau);  
}

int main(int argc, char * argv[]) {

  TApplication app("app", &argc, argv);
  
  // Switch between IROC and OROC.
  const bool iroc = false;

  // Switch gating on or off.
  const bool gating = true;

  // Voltage settings [V]
  const double vSens = iroc ? 1460. : 1570.;
  const double vGate = -70.;
  const double deltav = 90.; // for closed gate mode
 
  // y-axis gap between rows of wires
  const double gap = iroc ? 0.2 : 0.3;
 
  // y coordinates of the wires
  const double ys = gap;            // anode wires
  const double yc = 2. * gap;       // cathode
  const double yg = 2. * gap + 0.3; // gate
 
  // y coordinate and voltage of the HV plane (drift field)
  const double yHV = 249.7;
  const double vHV = -100000;
 
  // Periodicity (wire spacing) [cm]
  const double period = 0.25;
  const int nRep = 10;
 
  const double dc = period;
  const double dg = period / 2.;
 
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
  // Set the composition.
  gas->SetComposition("ne", 85.72, "co2", 9.52, "n2", 4.76);

  // Read the electron transport coefficients from a .gas file.
  gas->LoadGasFile("Ne_90_CO2_10_N2_5_with_mg.gas");
  // Read the ion mobility table.
  const std::string garfpath = std::getenv("GARFIELD_HOME");
  gas->LoadIonMobility(garfpath + "/Data/IonMobility_Ne+_Ne.txt");

  // Build the geometry.
  GeometrySimple* geo = new GeometrySimple();
  SolidBox* box = new SolidBox(0., yHV / 2, 0,
                               (nRep) * period, yHV / 2, 1);

  // Add the solid to the geometry, together with the medium inside.
  geo->AddSolid(box, gas);

  // Setup the electric field.
  ComponentAnalyticField* comp = new ComponentAnalyticField();
  comp->SetPeriodicityX(nRep * period);
  // Add the sense wires.
  for (int i = 0; i < nRep + 10; ++i) {
    const double xs = (i - 2.) * period;
    comp->AddWire(xs, ys, dSens, vSens, "s", 100., 50., 19.3, 1);
  }
  // Add the cathode wires.
  for (int i = 0; i < nRep + 5; ++i) {
    const double xc = dc * (i + 0.5);
    comp->AddWire(xc, yc, dCath, 0., "c", 100., 50., 19.3, 1);
  }
  // Add the gate wires.
  if (gating) {
    for (int i = 0; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5);
      comp->AddWire(xg, yg, dGate, vGate + deltav, "g+", 100., 50., 19.3, 1);
    }
    for (int i = 1; i < nRep * 2; i += 2) {
      const double xg = dg * (i + 0.5);
      comp->AddWire(xg, yg, dGate, vGate - deltav, "g-", 100., 50., 19.3, 1);
    }
  } else {
    for (int i = 0; i < nRep * 7; ++i) {
      const double xg = dg * (i + 0.5);
      // Set trap radius to 1 to avoid stopping the drift line 
      // prematurely at the gating grid.
      comp->AddWire(xg, yg, dGate, vGate, "g", 100., 50., 19.3, 1);  
    }
  }
  // Add the planes.
  comp->AddPlaneY(0., 0., "pad_plane");
  comp->AddPlaneY(yHV, vHV, "HV");

  // Request weighting-field calculation for the pad plane. 
  comp->AddReadout("pad_plane");
 
  // Set the magnetic field [T].
  comp->SetMagneticField(0, 0.5, 0);
  comp->SetGeometry(geo);

  // Make a sensor (gating closed).
  Sensor* sensor = new Sensor();
  sensor->AddComponent(comp);
  sensor->AddElectrode(comp, "pad_plane");
  // Change the time window for less/better resolution in time 
  // (effect on convolution can be important).
  sensor->SetTimeWindow(0.,1,50000); 

  // Plot isopotential contours
  ViewField* fView = new ViewField;
  fView->SetSensor(sensor);
  fView->SetArea(-nRep * period / 2., 0.,
                  nRep * period / 2., 2);
  fView->SetVoltageRange(-400., 1000.);
  fView->PlotContour();

  // Plot the cell layout.
  TCanvas* cCell = new TCanvas();
  ViewCell* cellView = new ViewCell();
  cellView->SetCanvas(cCell);
  cellView->SetArea(-3 * period / 2., -0.1, -10000., 
                     3 * period, 2., +10000.);
  cellView->SetComponent(comp); 
  cellView->Plot2d();

  // Calculate ion drift lines using the RKF method.
  DriftLineRKF* driftline = new DriftLineRKF();
  // driftline->EnableDebugging();
  // driftline->EnableVerbose();
  driftline->SetSensor(sensor);

  // Plot the drift line.
  // Comment this out when calculating many drift lines.
  ViewDrift* driftView = new ViewDrift();
  driftView->SetCanvas(cCell);
  driftline->EnablePlotting(driftView);
 
  // const int gain = 10000;
  const int gain = 10;
  double r = 0.003;
  double dummy = 0.;
  int status = 0;
  double endpoint = gap;
  // Count the number of ions that drift to 
  // plane, cathode, gate or drift volume, respectively.
  int plane = 0, cathode = 0, gate = 0, escape = 0; 
  for (int i = 0; i < gain; i++) {
    double angle = RndmGaussian(0, 1.4);
    driftline->DriftIon(r * sin(angle), gap + r * cos(angle), 0, 0);
    driftline->GetEndPoint(dummy,endpoint,dummy,dummy,status);
    if (endpoint < gap / 2) {
      // Drifted to plane.
      ++plane;
    } else if (endpoint > 1.5 * gap && endpoint < 2.5 * gap) {
      // Drifted to cathode wires.
      ++cathode;
    } else if (endpoint > 2.5 * gap && endpoint <2 * gap + 0.3 * 1.5) {
      // Drifted to gating wires.
      ++gate;
    } else {
      // Escaped to the drift volume.
      ++escape;
    }
  }

  // Plot the drift lines on top of the cell layout.
  driftView->Plot(true, false); 

  // Plot the induced current.
  ViewSignal* vs1 = new ViewSignal();
  vs1->SetSensor(sensor);
  vs1->PlotSignal("pad_plane");
  // Convolute with the transfer function and plot.
  ViewSignal* vs2 = new ViewSignal();
  sensor->SetTransferFunction(transfer);
  sensor->ConvoluteSignal();
  vs2->SetSensor(sensor);
  vs2->PlotSignal("pad_plane");
  app.Run(kTRUE);

}

