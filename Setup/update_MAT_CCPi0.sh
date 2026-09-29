#!/bin/bash

## ================== ##
##  Update MAT-CCPi0  ##
## ================== ##

# This is basically a rough macro to pull the latest versions of the 'main' branches of Git-MAT packages.

# Go to directory previous to 'CCPi0_Macros'
cd $CCPI0MACROS


# Update MAT
echo
echo " -------------- "
echo "  Updating MAT  "
echo " -------------- "
echo
cd ../MAT
git checkout main
git commit -a
git pull origin main


# Update MAT-MINERvA
echo
echo " ---------------------- "
echo "  Updating MAT-MINERvA  "
echo " ---------------------- "
echo
cd ../MAT-MINERvA
git checkout main
git commit -a
git pull origin main


# Update UnfoldUtils
echo
echo " ---------------------- "
echo "  Updating UnfoldUtils  "
echo " ---------------------- "
echo
cd ../UnfoldUtils
git checkout main
git commit -a
git pull origin main


# Update GENIEXSecExtract
echo
echo " --------------------------- "
echo "  Updating GENIEXSecExtract  "
echo " --------------------------- "
echo
cd ../GENIEXSecExtract
git checkout main
git commit -a
git pull origin main


# Go to 'CCPi0_Macros' directory
cd $CCPI0MACROS
echo
echo " ------------------ "
echo "  Update complete!  "
echo " ------------------ "
echo