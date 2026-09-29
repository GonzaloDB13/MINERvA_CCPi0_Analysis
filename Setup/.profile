#
# Add any personal extra databases here:
#
#UPS_EXTRA_DIR=$HOME/p/upsdb; export UPS_EXTRA_DIR
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# get ups environment, and then setup the login product
#
#if [  -f "/afs/fnal.gov/ups/etc/setups.sh" ]
#then 
#    . "/afs/fnal.gov/ups/etc/setups.sh"
#    if ups exist login
#    then
#        setup login
#    fi
#fi
#
# make sure our .shrc gets run...
#
ENV=$HOME/.shrc
export ENV 
if [ "`basename $SHELL`" != ksh -a -r $ENV ]
then
    . $ENV
fi
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# The default umask setting is 022, disabling writes except by
# owner.  Uncomment the following line to allow group writes.
# umask 002

# set prompt
#PS1="<`hostname`> "; export PS1

PROMPT_DIRTRIM=5
PS1="\u@\h: [ \w ] \n > "; export PS1

#PS1="\[\e[1;32m\]<`hostname`>\[\e[m\] "; export PS1
#PS1="\[\e[1;32m\]<\u@`hostname`>\[\e[m\]\n "; export PS1

#PROMPT_DIRTRIM=5
#PS1="\[\e[1;32m\]\u@\h:\[\e[m\] \[\e[48;5;237m\] \w \[\e[m\] \n > "; export PS1

#PS1="\[\e[1;32m\]<`hostname`>\[\e[m\]\n \[\e[1;34m\]"; export PS1
#trap "printf \\e[0m" DEBUG

# set default printer, etc.
# FLPQUE=fcc2w_ps;	export FLPQUE;
# FLPHOST=fnprt;		export FLPHOST;

# set timezone, esp if you don't want central time
# export TZ;		TZ=CST6CDT



## ========================================================================================= ##
##                                                                                           ##
##  **************** CUSTOM FUNCTIONS ****************                                       ##
##                                                                                           ##
## ========================================================================================= ##

## ============================= ##
##  Main commands and variables  ##
## ============================= ##

# Persistent directory
export PNFS=/pnfs/minerva/persistent/users/gonzalo

# Scratch directory
export SCRATCH=/pnfs/minerva/scratch/users/gonzalo

# Bluearc directory
export BLUEARC=/minerva/data/users/gonzalo

# ------------------------------------------------------

# ls with options
#   -a : Show hidden files
#   -l : Show list with permissions
#   -t : Show list sorted by date
#   --color : Self explanatory
alias ls="ls -lta --color=auto"

# Emacs with no-window system
#alias emacs="emacs -nw"

# ROOT without welcome screen
alias root="root -l"

# When no files were added since last compilation
#alias makequick="make QUICK=1"

# Number of lines of .bash_history
HISTSIZE=50000

# ----------------------------------------------------

# Show list of directories and sizes up to a certain depth
function Show()
{
  du -h --max-depth=$1.0 | sort -hr
}

# ----------------------------------------------------

# Setup to activate screen
export PATH=$PATH:$HOME/bin

# Enable visual mode in screen if needed
function Display()
{
  export DISPLAY="localhost:$1.0"
}

# ----------------------------------------------------

## ========================== ##
##  Setup CCPi0 environments  ##
## ========================== ##

# Set MAT-CCPi0
function MAT_CCPi0 {
  cd
  source set_MAT_CCPi0.sh
}


# Update MAT-CCPi0 from git repositories
function Update_MAT_CCPi0 {
  cd
  source update_MAT_CCPi0.sh
}


# Compile MAT-CCPi0 after changes
function Compile_MAT_CCPi0 {
  cd
  source compile_MAT_CCPi0.sh
}


# Set MINERvA CCPi0 in NX v22r1p1
function Minerva_CCPi0 {
  cd
  source set_Minerva_CCPi0.sh
}


# Set MINERvA CCPi0 in NX v22r1p1 for submission only
function Minerva_CCPi0_Submit {
  cd
  source set_Minerva_CCPi0_submit.sh
}


## =========================== ##
##  Set xrootd authentication  ##
## =========================== ##

function Xrootd {
  voms-proxy-destroy
  kx509
  voms-proxy-init -rfc --voms=fermilab:/fermilab/minerva/Role=Analysis --noregen -valid 24:0
  echo
}


## ================ ##
##  Samweb options  ##
## ================ ##

# Setup Samweb in MAT
function Setup_Samweb {
  cd
  source setup_samweb.sh
  cd $TOPDIR
}


# Samweb options
function Samweb_Options {
  cd
  source samweb_options.sh
  cd $TOPDIR
}


## ================================== ##
##  Setup generic version of MINERvA  ##
## ================================== ##

# Should be followed by a software version, i.e. < Set_Minerva v21r1p1 >
function Set_Minerva() {
  VERSION=$1

  if [ "$VERSION" = "v21r1p1" ]; then
    source /cvmfs/minerva.opensciencegrid.org/minerva/software_releases/$VERSION/setup.sh

  elif [ "$VERSION" = "v22r1p1" ]; then
    source /grid/fermiapp/minerva/software_releases/$VERSION/setup.sh

  else
    source /cvmfs/minerva.opensciencegrid.org/minerva/software_releases/$VERSION/setup.sh
  fi
  
  cd $User_release_area
}


## ===================== ##
##  Submit jobs to grid  ##
## ===================== ##

# Long submissions of playlists or entire runs
function Submit_Jobs() {
  OPTION=$1

  cd
  source submit_jobs.sh $1
  cd $TOPDIR
}


## ================================ ##
##  Submit test jobs interactively  ##
## ================================ ##

# Just to be run in interactive mode
function Submit_Test() {
  OPTION=$1

  cd
  source submit_test.sh $1
  cd $TOPDIR
}


## ================================= ##
##  Submit merging of files to grid  ##
## ================================= ##

function Submit_Merge() {
  OPTION=$1

  cd
  source submit_merge.sh $1
  cd $TOPDIR
}


## ====================================== ##
##  Submit audit of merged files to grid  ##
## ====================================== ##

function Submit_Audit() {
  OPTION=$1

  cd
  source submit_audit.sh $1
  cd $TOPDIR
}