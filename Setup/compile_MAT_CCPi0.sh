#!/bin/bash

## =================== ##
##  Compile MAT-CCPi0  ##
## =================== ##

# This is another rough macro to compile all MAT-Git packages after any change.
# Need to be run every time anything in these packages changes, not only after updating from the repositories, but also if I change them on my own.


# Go to 'CCPi0_Macros' directory
cd $CCPI0MACROS


# Compile MAT, MAT-MINERvA and UnfoldUtils
echo
echo
echo " -------------------------------------------- "
echo "  Compiling MAT, MAT-MINERvA and UnfoldUtils  "
echo " -------------------------------------------- "
echo
cd ../opt/build
make install


# Compile GENIEXSecExtract
echo
echo
echo " ---------------------------- "
echo "  Compiling GENIEXSecExtract  "
echo " ---------------------------- "
echo
cd ../buildGENIEXSecExtract
make install


# Go to 'CCPi0_Macros' directory again
cd $CCPI0MACROS
echo
echo " ----------------------- "
echo "  Compilation complete!  "
echo " ----------------------- "
echo