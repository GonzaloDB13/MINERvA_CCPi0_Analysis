#ifndef TunePlastic_C
#define TunePlastic_C

#include <iostream>
#include <vector>

#include "../includes/CVUniverse.h"
#include "../includes/MacroUtil.h"
#include "../includes/CCPi0Event.h"
#include "../includes/PlasticFitter.h"
#include "../includes/TruthMatching.h"
#include "../includes/Constants.h"
#include "../includes/Binning.h"
#include "../includes/GetVariables.h"
#include "../includes/common_functions.h"
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#endif  // __CINT__

#include "TF1.h"
#include "TFile.h"





// ==============================================================================
//  GET PLASTIC FUNCTIONAL FORM WEIGHT
// ==============================================================================

/* This function returns a HistWrapper with the functional form of the
 * plastic weight in bins of the variable that is being tuned.
 * 
 * Inputs are:
 * -> MacroUtil to load its error bands and loop over them.
 * -> Variable to extract its MC and data histograms,
 *    which are used to perform the fit.
 * -> Plastic weight HistWrapper not filled yet.
 * -> MC model used.
 * -> String with info of the physics region where to perform the tuning.
 * -> String with info of the plastic sideband to focus on.
 * -> Text file to print the scale functions. */

void GetMCPlasticWeight(CCPi0::MacroUtil util,
                        Variable* var,
                        CVHW& hw_Weight,
                        const EnumModels& type_model,
                        std::string phys_region,
                        std::string plas_sideband,
                        std::ofstream& text_file)
{
    // Set decimal precision
    text_file << std::setprecision(3) << std::fixed;
    
    
    // Configure physics region
    text_file << std::endl;
    text_file << " ============================================================================================================= " << std::endl;
    text_file << " ============================================================================================================= " << std::endl;
    text_file << std::endl;
    
    if ( phys_region == "SigReg" )            text_file << "  SIGNAL REGION "               << std::endl;
    else if ( phys_region == "PionBlobSB" )   text_file << "  PION-LIKE SHOWER SIDEBAND "   << std::endl;
    else if ( phys_region == "ProtonBlobSB" ) text_file << "  PROTON-LIKE SHOWER SIDEBAND " << std::endl;
    else if ( phys_region == "HighWSB" )      text_file << "  HIGH-W SIDEBAND "             << std::endl;
    text_file << std::endl;
    
    if ( plas_sideband == "PlasUpSB" ) {
        text_file << "  Plastic upstream sideband "       << std::endl;
        text_file << "  \tTuning true plastic upstream... " << std::endl;
    }
    else if ( plas_sideband == "PlasBetwSB" ) {
        text_file << "  Plastic between sideband "         << std::endl;
        text_file << "  \tTuning true plastic between... " << std::endl;
    }
    else if ( plas_sideband == "PlasDownSB" ) {
        text_file << "  Plastic downstream sideband "         << std::endl;
        text_file << "  \tTuning true plastic downstream... " << std::endl;
    }
    
    text_file << std::endl;
    text_file << std::endl;
    
    
    // Loop over error bands
    // =====================
        
    for ( auto error_band : util.m_error_bands )
    {
        std::string error_band_name = error_band.first;
        std::vector<CVUniverse*> universes = error_band.second;
        
        text_file << std::endl;
        text_file << " Error band: " << error_band_name << std::endl;
        text_file << " ==========  " << std::endl;
        text_file << std::endl;
        
        
        // Loop over universes
        // ===================
        
        for ( auto universe : universes )
        {
            // Construct fitter
            PlasticFitter fitter = PlasticFitter(*universe, var->m_hists, type_model, phys_region, plas_sideband);
            
            // Perform fit
            fitter.Fit();
            
            
            // Fill histogram of weights
            int Nbins = hw_Weight.univHist(universe)->GetNbinsX();
            for ( int bin = 1; bin <= Nbins; ++bin ) {
                double bin_center  = hw_Weight.univHist(universe)->GetBinCenter(bin);
                double bin_content = fitter.m_function->Eval(bin_center);
                
                if ( bin_content < 0.0 ) bin_content = 0.0;
                hw_Weight.univHist(universe) -> SetBinContent(bin, bin_content);
            }
            
            
            // Print information of fit function
            std::string universe_name = universe->ShortName();
            
            double par_a = fitter.m_function->GetParameter(0);
            double par_m = fitter.m_function->GetParameter(1);
            
            double par_err_a = fitter.m_function->GetParError(0);
            double par_err_m = fitter.m_function->GetParError(1);
            
            text_file << " \tUniverse: " << universe_name << std::endl;
            text_file << " \t\tConstant 'a': " << par_a << " +/- " << par_err_a << std::endl;
            text_file << " \t\tSlope 'm':    " << par_m << " +/- " << par_err_m << std::endl;
            text_file << std::endl;
            
        }  // End of loop over universes
        
    }  // End of loop over error bands
    
    text_file << std::endl;
    text_file << " ============================================================================================================= " << std::endl;
    text_file << " ============================================================================================================= " << std::endl;
    text_file << std::endl;
}





// ==============================================================================
//  SCALE PLASTIC BACKGROUND
// ==============================================================================

/* This function scales true plastic HistWrappers of each variable by using
 * MC plastic weight HistWrappers that cointain the functional form of the
 * plastic weight accordingly binned.
 * 
 * The scaling is done by three steps:
 * -> First, subtract the true plastic HistWrapper from the total MC HistWrapper.
 * -> Second, scale true plastic HW multiplying it by the MC plastic weight HW.
 * -> Third, add the scaled true plastic HW to the total MC HW.
 * 
 * Inputs are:
 * -> Variable to extract its MC histograms and modify them.
 * -> String with info of the physics region where to perform the tuning.
 * -> String with info of the true plastic sideband. */

void ScalePlasticBackgr(Variable* var,
                        CVHW hw_Weight,
                        std::string phys_region,
                        std::string true_plastic)
{
    // Signal region
    // =============
    
    if ( phys_region == "SigReg" )
    {
        // Get entries from total MC
        double Nentries_RecoPb_In_SigReg     = var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> GetEntries();
        double Nentries_RecoFe_In_SigReg     = var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> GetEntries();
        double Nentries_PlasUp_In_SigReg     = var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> GetEntries();
        double Nentries_PlasBetwSB_In_SigReg = var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> GetEntries();
        double Nentries_RecoDownSB_In_SigReg = var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> GetEntries();
        
        
        // True plastic upstream
        if ( true_plastic == "TruePlasUp" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_RecoPb_TruePlasUp     = var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist->GetEntries();
            double Nentries_RecoFe_TruePlasUp     = var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist->GetEntries();
            double Nentries_PlasUpSB_TruePlasUp   = var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasUp = var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasUp = var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist,     -1);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist,     -1);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist     -> Multiply(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist,     hw_Weight.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist     -> Multiply(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist,     hw_Weight.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist     -> SetEntries(Nentries_RecoPb_TruePlasUp);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist     -> SetEntries(Nentries_RecoFe_TruePlasUp);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasUp);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasUp);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist -> SetEntries(Nentries_PlasDownSB_TruePlasUp);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist);
        }
        
        
        // True plastic between
        else if ( true_plastic == "TruePlasBetw" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_RecoPb_TruePlasBetw     = var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist->GetEntries();
            double Nentries_RecoFe_TruePlasBetw     = var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasUpSB_TruePlasBetw   = var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasBetw = var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasBetw = var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist,     -1);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist,     -1);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist     -> Multiply(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist,     hw_Weight.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist     -> Multiply(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist,     hw_Weight.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist     -> SetEntries(Nentries_RecoPb_TruePlasBetw);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist     -> SetEntries(Nentries_RecoFe_TruePlasBetw);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasBetw);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasBetw);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist -> SetEntries(Nentries_PlasDownSB_TruePlasBetw);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist);
        }
        
        
        // True plastic downstream
        else if ( true_plastic == "TruePlasDown" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_RecoPb_TruePlasDown     = var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist->GetEntries();
            double Nentries_RecoFe_TruePlasDown     = var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist->GetEntries();
            double Nentries_PlasUpSB_TruePlasDown   = var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasDown = var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasDown = var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist,     -1);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist,     -1);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist     -> Multiply(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist,     hw_Weight.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist     -> Multiply(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist,     hw_Weight.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist     -> SetEntries(Nentries_RecoPb_TruePlasDown);
            var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist     -> SetEntries(Nentries_RecoFe_TruePlasDown);
            var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasDown);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasDown);
            var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist -> SetEntries(Nentries_PlasDownSB_TruePlasDown);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.hist);
            var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> Add(var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.hist);
            var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist);
            var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist);
            var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist);
        }
        
        
        // Set entries from total MC
        var->m_hists.m_mc_RecoPb_In_SigReg.hist     -> SetEntries(Nentries_RecoPb_In_SigReg);
        var->m_hists.m_mc_RecoFe_In_SigReg.hist     -> SetEntries(Nentries_RecoFe_In_SigReg);
        var->m_hists.m_mc_PlasUpSB_In_SigReg.hist   -> SetEntries(Nentries_PlasUp_In_SigReg);
        var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist -> SetEntries(Nentries_PlasBetwSB_In_SigReg);
        var->m_hists.m_mc_PlasDownSB_In_SigReg.hist -> SetEntries(Nentries_RecoDownSB_In_SigReg);
    }
    
    
    
    // Pion sideband
    // =============
    
    else if ( phys_region == "PionBlobSB" )
    {
        // Get entries from total MC
        double Nentries_PlasUp_In_PionBlobSB     = var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> GetEntries();
        double Nentries_PlasBetwSB_In_PionBlobSB = var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> GetEntries();
        double Nentries_RecoDownSB_In_PionBlobSB = var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> GetEntries();
        
        
        // True plastic upstream
        if ( true_plastic == "TruePlasUp" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasUp   = var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasUp = var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasUp = var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasUp);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasUp);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist -> SetEntries(Nentries_PlasDownSB_TruePlasUp);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist);
        }
        
        
        // True plastic between
        else if ( true_plastic == "TruePlasBetw" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasBetw   = var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasBetw = var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasBetw = var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasBetw);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasBetw);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasDownSB_TruePlasBetw);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist);
        }
        
        
        // True plastic downstream
        else if ( true_plastic == "TruePlasDown" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasDown   = var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasDown = var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasDown = var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist,   hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasDown);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasDown);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist -> SetEntries(Nentries_PlasDownSB_TruePlasDown);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist);
        }
        
        
        // Set entries from total MC
        var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist   -> SetEntries(Nentries_PlasUp_In_PionBlobSB);
        var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist -> SetEntries(Nentries_PlasBetwSB_In_PionBlobSB);
        var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist -> SetEntries(Nentries_RecoDownSB_In_PionBlobSB);
    }
    
    
    
    // Proton sideband
    // ===============
    
    else if ( phys_region == "ProtonBlobSB" )
    {
        // Get entries from total MC
        double Nentries_PlasUp_In_ProtonBlobSB     = var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> GetEntries();
        double Nentries_PlasBetwSB_In_ProtonBlobSB = var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> GetEntries();
        double Nentries_RecoDownSB_In_ProtonBlobSB = var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> GetEntries();
        
        
        // True plastic upstream
        if ( true_plastic == "TruePlasUp" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasUp   = var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasUp = var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasUp = var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasUp);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasUp);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist -> SetEntries(Nentries_PlasDownSB_TruePlasUp);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist);
        }
        
        
        // True plastic between
        else if ( true_plastic == "TruePlasBetw" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasBetw   = var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasBetw = var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasBetw = var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasBetw);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasBetw);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasDownSB_TruePlasBetw);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist);
        }
        
        
        // True plastic downstream
        else if ( true_plastic == "TruePlasDown" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasDown   = var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasDown = var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasDown = var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasDown);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasDown);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist -> SetEntries(Nentries_PlasDownSB_TruePlasDown);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist);
        }
        
        
        // Set entries from total MC
        var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist   -> SetEntries(Nentries_PlasUp_In_ProtonBlobSB);
        var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist -> SetEntries(Nentries_PlasBetwSB_In_ProtonBlobSB);
        var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist -> SetEntries(Nentries_RecoDownSB_In_ProtonBlobSB);
    }
    
    
    
    // High-W sideband
    // ===============
    
    else if ( phys_region == "HighWSB" )
    {
        // Get entries from total MC
        double Nentries_PlasUp_In_HighWSB     = var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> GetEntries();
        double Nentries_PlasBetwSB_In_HighWSB = var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> GetEntries();
        double Nentries_RecoDownSB_In_HighWSB = var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> GetEntries();
        
        
        // True plastic upstream
        if ( true_plastic == "TruePlasUp" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasUp   = var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasUp = var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasUp = var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasUp);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasUp);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist -> SetEntries(Nentries_PlasDownSB_TruePlasUp);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist);
        }
        
        
        // True plastic between
        else if ( true_plastic == "TruePlasBetw" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasBetw   = var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasBetw = var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasBetw = var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasBetw);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasBetw);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist -> SetEntries(Nentries_PlasDownSB_TruePlasBetw);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist);
        }
        
        
        // True plastic downstream
        else if ( true_plastic == "TruePlasDown" )
        {
            // Get entries fron non-tuned plastic
            double Nentries_PlasUpSB_TruePlasDown   = var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasBetwSB_TruePlasDown = var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist->GetEntries();
            double Nentries_PlasDownSB_TruePlasDown = var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist->GetEntries();
            
            // Subtract non-tuned plastic from total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist,   -1);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist, -1);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist, -1);
            
            // Scale plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist   -> Multiply(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist, hw_Weight.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist -> Multiply(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist, hw_Weight.hist);
            
            // Set entries for tuned plastic
            var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist   -> SetEntries(Nentries_PlasUpSB_TruePlasDown);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist -> SetEntries(Nentries_PlasBetwSB_TruePlasDown);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist -> SetEntries(Nentries_PlasDownSB_TruePlasDown);
            
            // Add tuned plastic to total MC
            var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> Add(var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist);
            var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> Add(var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist);
        }
        
        
        // Set entries from total MC
        var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist   -> SetEntries(Nentries_PlasUp_In_HighWSB);
        var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist -> SetEntries(Nentries_PlasBetwSB_In_HighWSB);
        var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist -> SetEntries(Nentries_RecoDownSB_In_HighWSB);
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void TunePlastic(std::string option_date,
                 std::string option_model,
                 std::string option_material,
                 bool do_systematics = true)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file 
    const std::string option_systematics = do_systematics ? "WithSyst" : "NoSyst";
    const std::string option_date_mc     = option_date + "_" + option_model;
    const std::string option_date_data   = option_date;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // MC before tuning
    std::string mc_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/%s/BeforeTuning", option_date_mc.c_str(),
                                                                                                                       option_material.c_str());
    
    // Data
    std::string data_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/data/%s/%s", option_date_data.c_str(),
                                                                                                          option_material.c_str());
    
    
    // Define input files
    // ==================
    
    // MC distributions before tuning
    TFile mc_fin(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fin_topdir.c_str(),
                                                                                                option_model.c_str(),
                                                                                                option_systematics.c_str(),
                                                                                                option_material.c_str()), "READ");
    
    // Data distributions
    TFile data_fin(Form("%s/Data_PlasticTuning_AllPlaylists_%s.root", data_topdir.c_str(),
                                                                      option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Top directory
    // =============
    
    // MC after tuning
    std::string mc_fout_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/%s/AfterTuning", option_date_mc.c_str(),
                                                                                                                       option_material.c_str());
    
    
    // Create output file
    // ==================
    
    // MC distrbutions after tuning AND plastic scale functions
    TFile mc_fout(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fout_topdir.c_str(),
                                                                                                option_model.c_str(),
                                                                                                option_systematics.c_str(),
                                                                                                option_material.c_str()), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  MacroUtil
    // =========================================
    
    // Playlist string
    // (Use only to load MC chain and access systematics)
    const std::string plist_string = "minervame1A";
    
    
    // Set playlists MC and data input
    // (Similarly, only to load MC and access systematics)
    const std::string mc_file_list   = GetPlaylistFile(true,  plist_string, "test");
    const std::string data_file_list = GetPlaylistFile(false, plist_string, "test");
    
    
    // Set MacroUtil
    // (TRUTH option set as 'false')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, false, do_systematics, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    SetMacroUtilPOT(mc_fin, data_fin, util);
    
    std::cout << std::endl;
    std::cout << " Tuning plastic background... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << mc_pot   << std::endl;
    std::cout << " \tData POT: " << data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Write POT to output file (MC-only output)
    WritePOT(mc_fout, true, mc_pot);
    
    
    // Text file with scale function info
    std::string text_scale_topdir = Form("/minerva/data/users/gonzalo/MAT/PlasticTuning/ScaleFunctions/%s/%s", option_date_mc.c_str(),
                                                                                                               option_material.c_str());
    
    std::ofstream text_scale(Form("%s/PlasticScaleFunctions_%s.txt", text_scale_topdir.c_str(),
                                                                     option_material.c_str()));
    
    
    
    // =========================================
    //  Prepare histograms before tuning
    // =========================================
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Initialize MC plastic weight histograms
    for ( auto var : variables ) {
        var -> InitMCWeights_PlasBackgr_In_SigReg(util.m_error_bands);
        var -> InitMCWeights_PlasBackgr_In_PhysSB(util.m_error_bands);
    }
    
    
    // Load histograms before tuning
    for ( auto var : variables )
    {
        // Plastic background in signal region
        var -> LoadMCHists_PlasSB_In_SigReg(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_SigReg(data_fin);
        
        // Plastic background in physics sidebands
        var -> LoadMCHists_PlasSB_In_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_PhysSB(data_fin);
    }
    
    
    
    // =========================================
    //  Tune background
    // =========================================
    
    for ( auto var : variables )
    {
        // Fill MC plastic weights
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_SigReg_TruePlasUp,   type_model, "SigReg", "PlasUpSB",   text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_SigReg_TruePlasBetw, type_model, "SigReg", "PlasBetwSB", text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_SigReg_TruePlasDown, type_model, "SigReg", "PlasDownSB", text_scale);
        
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasUp,   type_model, "PionBlobSB", "PlasUpSB",   text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasBetw, type_model, "PionBlobSB", "PlasBetwSB", text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasDown, type_model, "PionBlobSB", "PlasDownSB", text_scale);
        
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasUp,   type_model, "ProtonBlobSB", "PlasUpSB",   text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasBetw, type_model, "ProtonBlobSB", "PlasBetwSB", text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasDown, type_model, "ProtonBlobSB", "PlasDownSB", text_scale);
        
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_HighWSB_TruePlasUp,   type_model, "HighWSB", "PlasUpSB",   text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_HighWSB_TruePlasBetw, type_model, "HighWSB", "PlasBetwSB", text_scale);
        GetMCPlasticWeight(util, var, var->m_hists.m_mc_Weight_HighWSB_TruePlasDown, type_model, "HighWSB", "PlasDownSB", text_scale);
        
        
        // Sync MC histograms
        var -> SyncMCHists_PlasSB_In_SigReg();
        var -> SyncMCHists_PlasSB_In_PhysSB();
        
        var -> SyncMCWeights_PlasBackgr_In_SigReg();
        var -> SyncMCWeights_PlasBackgr_In_PhysSB();
        
        
        // Scale true plastic
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_SigReg_TruePlasUp,   "SigReg", "TruePlasUp");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_SigReg_TruePlasBetw, "SigReg", "TruePlasBetw");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_SigReg_TruePlasDown, "SigReg", "TruePlasDown");
        
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasUp,   "PionBlobSB", "TruePlasUp");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasBetw, "PionBlobSB", "TruePlasBetw");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasDown, "PionBlobSB", "TruePlasDown");
        
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasUp,   "ProtonBlobSB", "TruePlasUp");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasBetw, "ProtonBlobSB", "TruePlasBetw");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasDown, "ProtonBlobSB", "TruePlasDown");
        
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_HighWSB_TruePlasUp,   "HighWSB", "TruePlasUp");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_HighWSB_TruePlasBetw, "HighWSB", "TruePlasBetw");
        ScalePlasticBackgr(var, var->m_hists.m_mc_Weight_HighWSB_TruePlasDown, "HighWSB", "TruePlasDown");
    }
    
    
    
    // =========================================
    //  Sync and write histograms
    // =========================================
    
    for ( auto var : variables )
    {
        // Sync MC histograms again
        var -> SyncMCHists_PlasSB_In_SigReg();
        var -> SyncMCHists_PlasSB_In_PhysSB();
        
        var -> SyncMCWeights_PlasBackgr_In_SigReg();
        var -> SyncMCWeights_PlasBackgr_In_PhysSB();
        
        
        // Write tuned histograms and plastic weights
        var -> WriteMCHists_PlasSB_In_SigReg(mc_fout);
        var -> WriteMCHists_PlasSB_In_PhysSB(mc_fout);
        
        var -> WriteMCWeights_PlasBackgr_In_SigReg(mc_fout);
        var -> WriteMCWeights_PlasBackgr_In_PhysSB(mc_fout);
    }
    
    
    // Close scale function text file
    text_scale.close();
    
    
    // Close ROOT files
    mc_fin.Close();
    data_fin.Close();
    
    mc_fout.Close();
}


#endif  // TunePlastic_C