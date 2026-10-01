#include "TString.h"
#include "TSystem.h"


void loadClasses()
{
    TString path(TString::Format("%s/CCPi0_Macros/includes/", gSystem->Getenv("TOPDIR")));
    //std::cout << " \tPath is:" << path << std::endl;
    
    TString oldpath = gSystem->GetIncludePath();
    oldpath += " -I";
    oldpath += path;
    
    //std::cout << " \tOldPath is:" << oldpath << std::endl;
    gSystem -> SetIncludePath(oldpath);
    
    gSystem -> CompileMacro("CVUniverse.cxx",       "kF");
    gSystem -> CompileMacro("Cuts.cxx",             "k");
    gSystem -> CompileMacro("StackedHistogram.cxx", "k");
    gSystem -> CompileMacro("Histograms.cxx",       "k");
    gSystem -> CompileMacro("Variable.cxx",         "k");
    gSystem -> CompileMacro("Histograms2D.cxx",     "k");
    gSystem -> CompileMacro("Variable2D.cxx",       "k");
    gSystem -> CompileMacro("MacroUtil.cxx",        "kF");
    gSystem -> CompileMacro("CCPi0Event.cxx",       "kF");
    gSystem -> CompileMacro("PlasticFitter.cxx",    "k");
    gSystem -> CompileMacro("PhysicsFitter.cxx",    "k");
}