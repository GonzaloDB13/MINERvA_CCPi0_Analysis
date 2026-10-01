// ==============================================================================
//  Miscelaneous utility functions:
//  * Write POT number to a ROOT file
//  * Manipulate vectors of variables
//  ...more stuff can be added here
// ==============================================================================

#ifndef common_functions_h
#define common_functions_h

#include <algorithm>
#include <string>

#include "MacroUtil.h"
#include "Constants.h"
#include "PlotUtils/MnvH1D.h"

#include "TFile.h"
#include "TString.h"

#ifndef __CINT__
#include "Variable.h"
#endif  // __CINT__



// Forward declare Variable class
class Variable;



// ==============================================================================
//  Write POT to a file
// ==============================================================================

// Either MC POT or data POT on a file
// ===================================

void WritePOT(TFile& fout,
              const bool is_mc,
              const double pot)
{
    fout.Write(0, TObject::kOverwrite);
    fout.cd();
    
    // Create POT histogram
    const char* name = is_mc ? "mc_POT" : "data_POT";
    PlotUtils::MnvH1D* h_pot = new PlotUtils::MnvH1D(name, name, 1, 0., 1.);
    
    // Fill and write
    h_pot -> Fill(0.5, pot);
    h_pot -> Write();
}


// Both MC POT and data POT on a file
// ==================================

void WritePOT(TFile& fout,
              const double mc_pot,
              const double data_pot)
{
    fout.Write(0, TObject::kOverwrite);
    fout.cd();
    
    // Create POT histograms
    PlotUtils::MnvH1D* h_mc_pot   = new PlotUtils::MnvH1D("mc_POT",   "mc_POT",   1, 0., 1.);
    PlotUtils::MnvH1D* h_data_pot = new PlotUtils::MnvH1D("data_POT", "data_POT", 1, 0., 1.);
    
    // Fill and write
    h_mc_pot   -> Fill(0.5, mc_pot);
    h_data_pot -> Fill(0.5, data_pot);
    
    h_mc_pot   -> Write();
    h_data_pot -> Write();
}




// ==============================================================================
//  Get POT from input file
// ==============================================================================

// Either MC POT or data POT on a file
// ===================================

double GetPOT(TFile& fin,
              const bool is_mc)
{
    // Get POT branch name
    std::string pot_branch = is_mc ? "mc_POT" : "data_POT";
    
    // Get POT from input file
    PlotUtils::MnvH1D* h_pot = (PlotUtils::MnvH1D*)fin.Get(Form("%s", pot_branch.c_str()));
    double pot = h_pot->GetBinContent(1);
    
    // Return POT
    return pot;
}





// ==============================================================================
//  Load POT from input files and set MacroUtil
// ==============================================================================

// MC-only or data-only input file
// ===============================

void SetMacroUtilPOT(TFile& fin,
                     CCPi0::MacroUtil& util,
                     const bool is_mc)
{
    // Get POT branch name
    std::string pot_branch = is_mc ? "mc_POT" : "data_POT";
    
    // Get POT from input file
    PlotUtils::MnvH1D* h_pot = (PlotUtils::MnvH1D*)fin.Get(Form("%s", pot_branch.c_str()));
    double pot = h_pot->GetBinContent(1);
    
    // Set POT for MacroUtil
    if ( is_mc ) {
        util.m_mc_pot    = pot;
        util.m_data_pot  = -1.0;
        util.m_pot_scale = -1.0;
    }
    else {
        util.m_mc_pot    = -1.0;
        util.m_data_pot  = pot;
        util.m_pot_scale = -1.0;
    }
}


// MC-only and data-only input files, each by separate
// ===================================================

void SetMacroUtilPOT(TFile& mc_fin,
                     TFile& data_fin,
                     CCPi0::MacroUtil& util)
{
    // Get MC and data POT from input files
    PlotUtils::MnvH1D* h_mc_pot   = (PlotUtils::MnvH1D*)mc_fin.Get("mc_POT");
    PlotUtils::MnvH1D* h_data_pot = (PlotUtils::MnvH1D*)data_fin.Get("data_POT");
    
    double mc_pot   = h_mc_pot->GetBinContent(1);
    double data_pot = h_data_pot->GetBinContent(1);
    
    // Set POT for MacroUtil
    if ( data_pot > 0.0 ) {
        util.m_mc_pot    = mc_pot;
        util.m_data_pot  = data_pot;
        util.m_pot_scale = (util.m_data_pot)/(util.m_mc_pot);
    }
    else {
        std::cout << " ERROR: DATA POT IS ZERO!!! " << std::endl;
        util.m_mc_pot    = -1.0;
        util.m_data_pot  = -1.0;
        util.m_pot_scale = -1.0;
    }
}


// MC and data in one input file
// =============================

void SetMacroUtilPOT(TFile& fin,
                     CCPi0::MacroUtil& util)
{
    // Get MC and data POT from input files
    PlotUtils::MnvH1D* h_mc_pot   = (PlotUtils::MnvH1D*)fin.Get("mc_POT");
    PlotUtils::MnvH1D* h_data_pot = (PlotUtils::MnvH1D*)fin.Get("data_POT");
    
    double mc_pot   = h_mc_pot->GetBinContent(1);
    double data_pot = h_data_pot->GetBinContent(1);
    
    // Set POT for MacroUtil
    if ( data_pot > 0.0 ) {
        util.m_mc_pot    = mc_pot;
        util.m_data_pot  = data_pot;
        util.m_pot_scale = (util.m_data_pot)/(util.m_mc_pot);
    }
    else {
        std::cout << " ERROR: DATA POT IS ZERO!!! " << std::endl;
        util.m_mc_pot    = -1.0;
        util.m_data_pot  = -1.0;
        util.m_pot_scale = -1.0;
    }
}





// ==============================================================================
//  Check if vector of variables contain a certain variable
// ==============================================================================

bool HasVar(std::vector<Variable*> variables,
            std::string name)
{
#ifndef __CINT__  // CINT doesn't like lambdas
    auto it = find_if(variables.begin(), variables.end(),
                      [&name](Variable* v) {return v->Name() == name;});
    
    if ( it != variables.end() )
        return true;
    else
        return false;
#endif  // __CINT__
}





// ==============================================================================
//  Get a certain variable from a vector of variables
// ==============================================================================

Variable* GetVar(std::vector<Variable*> variables,
                 std::string name)
{
#ifndef __CINT__  // CINT doesn't like lambdas
    auto it = find_if(variables.begin(), variables.end(),
                      [&name](Variable* v) {return v->Name() == name;});
    
    if ( it != variables.end() )
        return *it;
    else {
        std::cout << name << " ERROR IN GetVar(): " << name << " VARIABLE NOT FOUND!!!" << std::endl;
        return nullptr;
    }
#endif  // __CINT__
}


#endif  // common_functions_h