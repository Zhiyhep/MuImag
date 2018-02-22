# MuImag

This project aims at simulating cosmic muons' transporting processes through detectors and matters and utilises the
simulation results to image the objects placed within the object area.

## Simulation
The simulation is based on Geant4. Material and geometry of detectors and objects are defined in GDML files. Primary particles are generated through macro files (e.g. Cosmic_Muon.mac) using gps commands. Physics processes and particles are user-defined. 
Complete information of each muon step within detectors, including position, global time, deposit energy, volume name, total number of hits, ?process names, are stored in user-defined class (PrimaryHit) and saved in root files for persistency.

## Analysis
The reconstruct, imaging and analysis programs are in directory /build/analysis. 

The reconstruct program utilizes weight algorithm to calculate muon positions at each detector. The reconstructed result is stored in the same root file as simulation result, with a different key name. Other processes, e.g. diffusion, amplification, electric noise should be able to take effect according to user's interest.

The imaging program is able to be voxelized with arbitrary number of bins in each dimension. It utilizes the reconstruct result to estimate the scatter density (sensitive to object's atomic number) of each voxel and thus generates a 3D image of objects.

The analysis programs serve as a toolkit for inspecting events and analysis simulation results, including:
* Event_Viewer: visulizes the detector and object layout and each event.
* ? Error_Function: calculate the error rate (or serves as loss function?) of imaging.
* ? Distribution: plot distributions of important quantities such as scatter angle, displacement.

<<<<<<< HEAD
<<<<<<< HEAD
![](/build/analysis/pics/Eve.png)
=======
![](https://zhiyhep.github.com/MuImag1.1/build/analysis/pics/Eve.png)
>>>>>>> eb3834e3fdc6cfed73c457c0dddd43f9d6ab59ef
=======
![](/build/analysis/pics/Eve.png)
>>>>>>> 2ff1f29298b48d1046849f0682e5eeb596ddeeb9
