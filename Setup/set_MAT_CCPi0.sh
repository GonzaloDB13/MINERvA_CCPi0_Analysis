#!/bin/bash

## ================= ##
##  Setup MAT-CCPi0  ##
## ================= ##

# Set experiment
export EXPERIMENT=minerva


# Set MAT-CCPi0 as top directory
export TOPDIR=/exp/minerva/app/users/gonzalo/cmtuser/MAT_CCPi0
cd $TOPDIR


# Source MAT, MAT-MINERvA and UnfoldUtils
echo
echo " ------------------------------------------ "
echo "  Setting MAT, MAT-MINERvA and UnfoldUtils  "
echo " ------------------------------------------ "
source opt/bin/setup.sh
echo " Done ! "


# Source GENIEXSecExtract
echo
echo " -------------------------- "
echo "  Setting GENIEXSecExtract  "
echo " -------------------------- "
source opt/buildGENIEXSecExtract/setup_GENIEXSecExtract.sh
echo " Done! "


# Source ROOT 6
echo
echo " ---------------- "
echo "  Setting ROOT 6  "
echo " ---------------- "
source opt/bin/setupROOT6OnGPVMs.sh
echo " Done! "


# Source MINERvA products
echo
echo " -------------------------- "
echo "  Setting MINERvA products  "
echo " -------------------------- "
source /cvmfs/minerva.opensciencegrid.org/minerva/setup/setup_minerva_products.sh
echo " Done! "


# Go to 'CCPi0_Macros' directory
export CCPI0MACROS=$TOPDIR/CCPi0_Macros
cd $CCPI0MACROS

echo
echo " ------------------------------------- "
echo "  Welcome to 'CCPi0_Macros' directory  "
echo " ------------------------------------- "
echo