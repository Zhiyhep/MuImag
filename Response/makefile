OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CFLAGS = -W -Wall -Wextra -Wno-long-long \
	`root-config --cflags` \
	-O3 -fno-common -c \
	-I$(INCDIR) -I$(HEEDDIR)

# Debug flags
CFLAGS += -g

LDFLAGS = -L$(LIBDIR) -lGarfield 
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm
LDFLAGS += -L$(MuImag_Dir)/lib -lPrimaryHits
#LDFLAGS += -g

ion_signal:	ion_signal.C
	$(CXX) $(CFLAGS) ion_signal.C
	$(CXX) -o ion_signal ion_signal.o $(LDFLAGS)
	rm ion_signal.o
full_tpc_sim:	full_tpc_sim.C
	$(CXX) $(CFLAGS) full_tpc_sim.C
	$(CXX) -o full_tpc_sim full_tpc_sim.o $(LDFLAGS)
	rm full_tpc_sim.o

