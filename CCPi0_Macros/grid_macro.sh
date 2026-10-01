#!/bin/bash


# ===========================================================================================================
#  Setup env variables, cmt config, source setup.sh packages
#  (Originally implemented by Ben's 'grid_ccpi_macro.sh')
# ===========================================================================================================

# The -n option [to export]causes the export property to be removed from each name
# CONDOR_DIR_INPUT is the area where `-f` jobsub arguments (i.e. our tarball) are dropped

echo
echo " ================== Set HOME = TOPDIR = CONDOR_DIR_INPUT ================== "
export -n HOME 
export -n TOPDIR
export -n MINERVA_PREFIX

export HOME=${CONDOR_DIR_INPUT}
export TOPDIR=${CONDOR_DIR_INPUT}
export MINERVA_PREFIX=${TOPDIR}/opt

echo " CONDOR_DIR_INPUT: " ${CONDOR_DIR_INPUT}
echo " HOME:             " ${HOME}
echo " TOPDIR:           " ${TOPDIR}
echo " MINERVA_PREFIX:   " ${MINERVA_PREFIX}
echo " EXPERIMENT:       " ${EXPERIMENT}

echo
echo
echo " ================== cd to HOME a.k.a. TOPDIR a.k.a. CONDOR_DIR_INPUT ================== "
cd $HOME
echo " PWD: " $PWD

echo
echo
echo " ================== pwd, ls -a ================== "
echo " PWD: " $PWD
ls -a

echo
echo
echo " ================== Untarring ================== "
tar xvzf ${TARFILE} -C ./ > /dev/null

echo
echo
echo " ================== ls -a ================== "
echo " PWD: " $PWD
ls -a

echo
echo
echo " ================== source TOPDIR/opt/bin/setup.sh ================== "
source opt/bin/setup.sh

echo
echo
echo " ================== source TOPDIR/opt/bin/setupROOT6OnGPVMs.sh ================== "
source opt/bin/setupROOT6OnGPVMs.sh

echo
echo
echo " ================== echo PLOTUTILSROOT ================== "
echo $PLOTUTILSROOT

echo
echo
echo " ================== cd to CCPi0_Macros directory ================== "
cd ${TOPDIR}/CCPi0_Macros
echo " PWD: " $PWD

echo
echo
echo " ================== Remove any pre-existing ROOT files that got passed to the grid ================== "
ls -a
rm *.root

echo
echo
echo " ================== Clear out pre-existing '.so', '.d', '.o', and '.pcm' files ================== "
source clean.sh
ls -a



# ===========================================================================================================
#  Tell ROOT (via .rootrc) that every time ROOT is open/run, it should first
#  run rootlogon_grid.C, which is located in the cd.
# 
#  rootlogon_grid.C contains PlotUtils setup code.
# ===========================================================================================================

echo
echo
echo " ================== Create ./.rootrc ================== "
echo " PWD: " $PWD
echo Rint.Logon: ./rootlogon_grid.C > ./.rootrc

echo
echo
echo " ================== ls -a ================== "
echo " PWD: " $PWD
ls -a

echo
echo
echo " ================== cat .rootrc ================== "
cat .rootrc

echo
echo
echo " ================== cat .rootlogon_grid.C ================== "
cat rootlogon_grid.C

# To make sure we're using the right rootlogon.C 
echo
echo
echo " ================== gEnv->Print() ================== "
echo "gEnv->Print(); gSystem->Exit(0);" | root -b -l | grep Logon



# ===========================================================================================================
#  Ship it
#  The "++" in loadLibs.C might be redundant with removing the .so's, but just to be safe
# ===========================================================================================================

echo
echo
echo " ================== Print MACRO ================== "
echo " PWD: " $PWD
echo " MACRO: " $MACRO

echo
echo
echo " ================== root.exe -b -q loadLibs.C++ MACRO ================== "
root.exe -b -q -l loadLibs.C++ ${MACRO}

echo
echo
echo " ================== ls *.root ================== "
echo " PWD: " $PWD
ls *.root

echo
echo
echo " ================== Move *.root to CONDOR_DIR_OUT ================== "
mv *.root $CONDOR_DIR_OUT

echo
echo
echo " ================== ls CONDOR_DIR_OUT ================== "
echo " PWD: " $PWD
ls $CONDOR_DIR_OUT

echo
echo
echo "Done!!"
echo
