#ifndef TunePhysics_C
#define TunePhysics_C

#include <iostream>
#include <vector>

#include "../includes/CVUniverse.h"
#include "../includes/MacroUtil.h"
#include "../includes/CCPi0Event.h"
#include "../includes/PhysicsFitter.h"
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
//  GET PHYSICS FUNCTIONAL FORM WEIGHTS
// ==============================================================================

/* This function returns different HistWrapper according to the number of
 * backgrounds to be fitted (currently, signal + 3 physics backgrounds).
 * 
 * Each HW contains thefunctional form of its corresponding physics weight
 * in bins of the variable that is being tuned.
 * 
 * Inputs are:
 * -> MacroUtil to load its error bands and loop over them.
 * -> Variable to extract its MC and data histograms,
 *    which are used to perform the fit.
 * -> All of the physics weight HistWrappers that need to be filled.
 * -> MC model used.
 * -> Material to be analyzed.
 * -> Fit type to use (linear, linear alternative, bilinear or bilinear alternative).
 * -> Text file to print the scale functions. */

void GetMCPhysicsWeight(CCPi0::MacroUtil util,
                        Variable* var,
                        CVHW& hw_Weight_Signal,
                        CVHW& hw_Weight_Pi0HighW,
                        CVHW& hw_Weight_QElike,
                        CVHW& hw_Weight_PionProd,
                        const EnumModels& type_model,
                        std::string option_material,
                        std::string option_fit_method,
                        std::string option_fit_function,
                        int n_iterations,
                        TFile& fout,
                        std::ofstream& text_file)
{
    // Sanity check since I only use "Combined" fit and not "Iterative anymore"
    if ( option_fit_method != "Combined" || n_iterations != 1 ) {
        std::cout << " PHYSICS FITTER MUST USE 'Combined' FIT METHOD THAT'S SHOULD BE SET BY DEFAULT " << std::endl;
        std::cout << " IF YOU ARE SEEING THIS, CHECK THE CODE!!! " << std::endl;
        exit(1);
    }
    
    
    // Set decimal precision
    text_file << std::setprecision(3) << std::fixed;
    
    
    // Create histograms with info of fit parameters
    // =============================================
    
    // Define lower and upper limit of histograms depending on the meaning of the parameters
    double lower_limit_a = 0.0;
    double upper_limit_a = 3.0;
    
    double lower_limit_m = -1.0;
    double upper_limit_m = 1.0;
    
    double lower_limit_b = -5.0;
    double upper_limit_b = 5.0;
    
    double lower_limit_T, upper_limit_T;
    if ( option_fit_function == "LinearAlt" ) {
        lower_limit_T = 0.0;
        upper_limit_T = 2.0;
    }
    else if ( option_fit_function == "Bilinear" ) {
        lower_limit_T = 0.0;
        upper_limit_T = 0.325;
    }
    
    
    // Make histograms
    int Nbins = 1000;
    
    TH1D* h_Par_a_Signal[n_iterations];
    TH1D* h_Par_m_Signal[n_iterations];
    TH1D* h_Par_b_Signal[n_iterations];
    TH1D* h_Par_T_Signal[n_iterations];
    
    TH1D* h_Par_a_Pi0HighW[n_iterations];
    TH1D* h_Par_m_Pi0HighW[n_iterations];
    TH1D* h_Par_b_Pi0HighW[n_iterations];
    TH1D* h_Par_T_Pi0HighW[n_iterations];
    
    TH1D* h_Par_a_QElike[n_iterations];
    TH1D* h_Par_m_QElike[n_iterations];
    TH1D* h_Par_b_QElike[n_iterations];
    TH1D* h_Par_T_QElike[n_iterations];
    
    TH1D* h_Par_a_PionProd[n_iterations];
    TH1D* h_Par_m_PionProd[n_iterations];
    TH1D* h_Par_b_PionProd[n_iterations];
    TH1D* h_Par_T_PionProd[n_iterations];
    
    for ( int iter = 1; iter <= n_iterations; ++iter ) {
        h_Par_a_Signal[iter-1] = new TH1D("Par_a_Signal", "", Nbins, lower_limit_a, upper_limit_a);
        h_Par_m_Signal[iter-1] = new TH1D("Par_m_Signal", "", Nbins, lower_limit_m, upper_limit_m);
        h_Par_b_Signal[iter-1] = new TH1D("Par_b_Signal", "", Nbins, lower_limit_b, upper_limit_b);
        h_Par_T_Signal[iter-1] = new TH1D("Par_T_Signal", "", Nbins, lower_limit_T, upper_limit_T);
        
        h_Par_a_Pi0HighW[iter-1] = new TH1D("Par_a_Pi0HighW", "", Nbins, lower_limit_a, upper_limit_a);
        h_Par_m_Pi0HighW[iter-1] = new TH1D("Par_m_Pi0HighW", "", Nbins, lower_limit_m, upper_limit_m);
        h_Par_b_Pi0HighW[iter-1] = new TH1D("Par_b_Pi0HighW", "", Nbins, lower_limit_b, upper_limit_b);
        h_Par_T_Pi0HighW[iter-1] = new TH1D("Par_T_Pi0HighW", "", Nbins, lower_limit_T, upper_limit_T);
        
        h_Par_a_QElike[iter-1] = new TH1D("Par_a_QElike", "", Nbins, lower_limit_a, upper_limit_a);
        h_Par_m_QElike[iter-1] = new TH1D("Par_m_QElike", "", Nbins, lower_limit_m, upper_limit_m);
        h_Par_b_QElike[iter-1] = new TH1D("Par_b_QElike", "", Nbins, lower_limit_b, upper_limit_b);
        h_Par_T_QElike[iter-1] = new TH1D("Par_T_QElike", "", Nbins, lower_limit_T, upper_limit_T);
        
        h_Par_a_PionProd[iter-1] = new TH1D("Par_a_PionProd", "", Nbins, lower_limit_a, upper_limit_a);
        h_Par_m_PionProd[iter-1] = new TH1D("Par_m_PionProd", "", Nbins, lower_limit_m, upper_limit_m);
        h_Par_b_PionProd[iter-1] = new TH1D("Par_b_PionProd", "", Nbins, lower_limit_b, upper_limit_b);
        h_Par_T_PionProd[iter-1] = new TH1D("Par_T_PionProd", "", Nbins, lower_limit_T, upper_limit_T);
    }
    
    
    // Loop over error bands
    // =====================
    
    text_file << std::endl;
    text_file << " ================ " << std::endl;
    text_file << "  PHYSICS TUNING  " << std::endl;
    text_file << " ================ " << std::endl;
    text_file << std::endl;
        
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
            PhysicsFitter fitter = PhysicsFitter(*universe, var->m_hists, type_model, option_material, option_fit_function, n_iterations);
            
            
            // Perform fit
            fitter.Fit_Combined();
            
            
            // Get fit parameters for each iteration and fill corresponding histograms
            for ( int iter = 1; iter <= n_iterations; ++iter )
            {
                double par_a_Signal = fitter.m_Par_a_Signal[iter-1];
                double par_m_Signal = fitter.m_Par_m_Signal[iter-1];
                double par_b_Signal = fitter.m_Par_b_Signal[iter-1];
                double par_T_Signal = fitter.m_Par_T_Signal[iter-1];
                
                double par_a_Pi0HighW = fitter.m_Par_a_Pi0HighW[iter-1];
                double par_m_Pi0HighW = fitter.m_Par_m_Pi0HighW[iter-1];
                double par_b_Pi0HighW = fitter.m_Par_b_Pi0HighW[iter-1];
                double par_T_Pi0HighW = fitter.m_Par_T_Pi0HighW[iter-1];
                
                double par_a_QElike = fitter.m_Par_a_QElike[iter-1];
                double par_m_QElike = fitter.m_Par_m_QElike[iter-1];
                double par_b_QElike = fitter.m_Par_b_QElike[iter-1];
                double par_T_QElike = fitter.m_Par_T_QElike[iter-1];
                
                double par_a_PionProd = fitter.m_Par_a_PionProd[iter-1];
                double par_m_PionProd = fitter.m_Par_m_PionProd[iter-1];
                double par_b_PionProd = fitter.m_Par_b_PionProd[iter-1];
                double par_T_PionProd = fitter.m_Par_T_PionProd[iter-1];
                
                h_Par_a_Signal[iter-1] -> Fill(par_a_Signal);
                h_Par_m_Signal[iter-1] -> Fill(par_m_Signal);
                h_Par_b_Signal[iter-1] -> Fill(par_b_Signal);
                h_Par_T_Signal[iter-1] -> Fill(par_T_Signal);
                
                h_Par_a_Pi0HighW[iter-1] -> Fill(par_a_Pi0HighW);
                h_Par_m_Pi0HighW[iter-1] -> Fill(par_m_Pi0HighW);
                h_Par_b_Pi0HighW[iter-1] -> Fill(par_b_Pi0HighW);
                h_Par_T_Pi0HighW[iter-1] -> Fill(par_T_Pi0HighW);
                
                h_Par_a_QElike[iter-1] -> Fill(par_a_QElike);
                h_Par_m_QElike[iter-1] -> Fill(par_m_QElike);
                h_Par_b_QElike[iter-1] -> Fill(par_b_QElike);
                h_Par_T_QElike[iter-1] -> Fill(par_T_QElike);
                
                h_Par_a_PionProd[iter-1] -> Fill(par_a_PionProd);
                h_Par_m_PionProd[iter-1] -> Fill(par_m_PionProd);
                h_Par_b_PionProd[iter-1] -> Fill(par_b_PionProd);
                h_Par_T_PionProd[iter-1] -> Fill(par_T_PionProd);
            }
            
            
            // Fill histograms of weights
            int Nbins = hw_Weight_Signal.univHist(universe)->GetNbinsX();
            for ( int bin = 1; bin <= Nbins; ++bin )
            {
                // Bin center and contents
                double bin_center = hw_Weight_Signal.univHist(universe)->GetBinCenter(bin);
                
                double bin_content_Signal   = fitter.m_function_Signal->Eval(bin_center);
                double bin_content_Pi0HighW = fitter.m_function_BackgrPi0HighW->Eval(bin_center);
                double bin_content_QElike   = fitter.m_function_BackgrQElike->Eval(bin_center);
                double bin_content_PionProd = fitter.m_function_BackgrPionProd->Eval(bin_center);
                
                // Fill histogram if content is positive 
                if ( bin_content_Signal < 0.0 )   bin_content_Signal   = 0.0;
                if ( bin_content_Pi0HighW < 0.0 ) bin_content_Pi0HighW = 0.0;
                if ( bin_content_QElike < 0.0 )   bin_content_QElike   = 0.0;
                if ( bin_content_PionProd < 0.0 ) bin_content_PionProd = 0.0;
                
                hw_Weight_Signal.univHist(universe)   -> SetBinContent(bin, bin_content_Signal);
                hw_Weight_Pi0HighW.univHist(universe) -> SetBinContent(bin, bin_content_Pi0HighW);
                hw_Weight_QElike.univHist(universe)   -> SetBinContent(bin, bin_content_QElike);
                hw_Weight_PionProd.univHist(universe) -> SetBinContent(bin, bin_content_PionProd);
            }
            
            
            // Print information of fit functions
            std::string universe_name = universe->ShortName();
            text_file << " \tUniverse: " << universe_name << std::endl;
            text_file << std::endl;
            
            double par_a_Signal, par_err_a_Signal;
            double par_m_Signal, par_err_m_Signal;
            double par_b_Signal, par_err_b_Signal;
            double par_T_Signal, par_err_T_Signal;
            
            double par_a_Pi0HighW, par_err_a_Pi0HighW;
            double par_m_Pi0HighW, par_err_m_Pi0HighW;
            double par_b_Pi0HighW, par_err_b_Pi0HighW;
            double par_T_Pi0HighW, par_err_T_Pi0HighW;
            
            double par_a_QElike, par_err_a_QElike;
            double par_m_QElike, par_err_m_QElike;
            double par_b_QElike, par_err_b_QElike;
            double par_T_QElike, par_err_T_QElike;
            
            double par_a_PionProd, par_err_a_PionProd;
            double par_m_PionProd, par_err_m_PionProd;
            double par_b_PionProd, par_err_b_PionProd;
            double par_T_PionProd, par_err_T_PionProd;
            
            if ( fitter.m_option_fit_function == "Scalar" ) {
                par_a_Signal = fitter.m_function_Signal->GetParameter(0);
                par_err_a_Signal = fitter.m_function_Signal->GetParError(0);
                
                par_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(0);
                par_err_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(0);
                
                par_a_QElike = fitter.m_function_BackgrQElike->GetParameter(0);
                par_err_a_QElike = fitter.m_function_BackgrQElike->GetParError(0);
                
                par_a_PionProd = fitter.m_function_BackgrPionProd->GetParameter(0);
                par_err_a_PionProd = fitter.m_function_BackgrPionProd->GetParError(0);
            }
            
            else if ( fitter.m_option_fit_function == "Linear" ) {
                par_a_Signal = fitter.m_function_Signal->GetParameter(0);
                par_m_Signal = fitter.m_function_Signal->GetParameter(1);
                par_err_a_Signal = fitter.m_function_Signal->GetParError(0);
                par_err_m_Signal = fitter.m_function_Signal->GetParError(1);
                
                par_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(0);
                par_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(1);
                par_err_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(0);
                par_err_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(1);
                
                par_a_QElike = fitter.m_function_BackgrQElike->GetParameter(0);
                par_m_QElike = fitter.m_function_BackgrQElike->GetParameter(1);
                par_err_a_QElike = fitter.m_function_BackgrQElike->GetParError(0);
                par_err_m_QElike = fitter.m_function_BackgrQElike->GetParError(1);
                
                par_a_PionProd = fitter.m_function_BackgrPionProd->GetParameter(0);
                par_m_PionProd = fitter.m_function_BackgrPionProd->GetParameter(1);
                par_err_a_PionProd = fitter.m_function_BackgrPionProd->GetParError(0);
                par_err_m_PionProd = fitter.m_function_BackgrPionProd->GetParError(1);
            }
            
            else if ( fitter.m_option_fit_function == "LinearAlt" ) {
                par_a_Signal = fitter.m_function_Signal->GetParameter(0);
                par_m_Signal = fitter.m_function_Signal->GetParameter(1);
                par_T_Signal = fitter.m_function_Signal->GetParameter(2);
                par_err_a_Signal = fitter.m_function_Signal->GetParError(0);
                par_err_m_Signal = fitter.m_function_Signal->GetParError(1);
                par_err_T_Signal = fitter.m_function_Signal->GetParError(2);
                
                par_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(0);
                par_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(1);
                par_T_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(2);
                par_err_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(0);
                par_err_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(1);
                par_err_T_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(2);
                
                par_a_QElike = fitter.m_function_BackgrQElike->GetParameter(0);
                par_m_QElike = fitter.m_function_BackgrQElike->GetParameter(1);
                par_T_QElike = fitter.m_function_BackgrQElike->GetParameter(2);
                par_err_a_QElike = fitter.m_function_BackgrQElike->GetParError(0);
                par_err_m_QElike = fitter.m_function_BackgrQElike->GetParError(1);
                par_err_T_QElike = fitter.m_function_BackgrQElike->GetParError(2);
                
                par_a_PionProd = fitter.m_function_BackgrPionProd->GetParameter(0);
                par_m_PionProd = fitter.m_function_BackgrPionProd->GetParameter(1);
                par_T_PionProd = fitter.m_function_BackgrPionProd->GetParameter(2);
                par_err_a_PionProd = fitter.m_function_BackgrPionProd->GetParError(0);
                par_err_m_PionProd = fitter.m_function_BackgrPionProd->GetParError(1);
                par_err_T_PionProd = fitter.m_function_BackgrPionProd->GetParError(2);
            }
            
            else if ( fitter.m_option_fit_function == "Bilinear" ) {
                par_a_Signal = fitter.m_function_Signal->GetParameter(0);
                par_m_Signal = fitter.m_function_Signal->GetParameter(1);
                par_b_Signal = fitter.m_function_Signal->GetParameter(2);
                par_err_a_Signal = fitter.m_function_Signal->GetParError(0);
                par_err_m_Signal = fitter.m_function_Signal->GetParError(1);
                par_err_b_Signal = fitter.m_function_Signal->GetParError(2);
                
                par_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(0);
                par_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(1);
                par_b_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(2);
                par_err_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(0);
                par_err_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(1);
                par_err_b_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(2);
                
                par_a_QElike = fitter.m_function_BackgrQElike->GetParameter(0);
                par_m_QElike = fitter.m_function_BackgrQElike->GetParameter(1);
                par_b_QElike = fitter.m_function_BackgrQElike->GetParameter(2);
                par_err_a_QElike = fitter.m_function_BackgrQElike->GetParError(0);
                par_err_m_QElike = fitter.m_function_BackgrQElike->GetParError(1);
                par_err_b_QElike = fitter.m_function_BackgrQElike->GetParError(2);
                
                par_a_PionProd = fitter.m_function_BackgrPionProd->GetParameter(0);
                par_m_PionProd = fitter.m_function_BackgrPionProd->GetParameter(1);
                par_b_PionProd = fitter.m_function_BackgrPionProd->GetParameter(2);
                par_err_a_PionProd = fitter.m_function_BackgrPionProd->GetParError(0);
                par_err_m_PionProd = fitter.m_function_BackgrPionProd->GetParError(1);
                par_err_b_PionProd = fitter.m_function_BackgrPionProd->GetParError(2);
            }
            
            else if ( fitter.m_option_fit_function == "BilinearAlt" ) {
                par_a_Signal = fitter.m_function_Signal->GetParameter(0);
                par_m_Signal = fitter.m_function_Signal->GetParameter(1);
                par_b_Signal = fitter.m_function_Signal->GetParameter(2);
                par_T_Signal = fitter.m_function_Signal->GetParameter(3);
                par_err_a_Signal = fitter.m_function_Signal->GetParError(0);
                par_err_m_Signal = fitter.m_function_Signal->GetParError(1);
                par_err_b_Signal = fitter.m_function_Signal->GetParError(2);
                par_err_T_Signal = fitter.m_function_Signal->GetParError(3);
                
                par_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(0);
                par_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(1);
                par_b_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(2);
                par_T_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParameter(3);
                par_err_a_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(0);
                par_err_m_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(1);
                par_err_b_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(2);
                par_err_T_Pi0HighW = fitter.m_function_BackgrPi0HighW->GetParError(3);
                
                par_a_QElike = fitter.m_function_BackgrQElike->GetParameter(0);
                par_m_QElike = fitter.m_function_BackgrQElike->GetParameter(1);
                par_b_QElike = fitter.m_function_BackgrQElike->GetParameter(2);
                par_T_QElike = fitter.m_function_BackgrQElike->GetParameter(3);
                par_err_a_QElike = fitter.m_function_BackgrQElike->GetParError(0);
                par_err_m_QElike = fitter.m_function_BackgrQElike->GetParError(1);
                par_err_b_QElike = fitter.m_function_BackgrQElike->GetParError(2);
                par_err_T_QElike = fitter.m_function_BackgrQElike->GetParError(3);
                
                par_a_PionProd = fitter.m_function_BackgrPionProd->GetParameter(0);
                par_m_PionProd = fitter.m_function_BackgrPionProd->GetParameter(1);
                par_b_PionProd = fitter.m_function_BackgrPionProd->GetParameter(2);
                par_T_PionProd = fitter.m_function_BackgrPionProd->GetParameter(3);
                par_err_a_PionProd = fitter.m_function_BackgrPionProd->GetParError(0);
                par_err_m_PionProd = fitter.m_function_BackgrPionProd->GetParError(1);
                par_err_b_PionProd = fitter.m_function_BackgrPionProd->GetParError(2);
                par_err_T_PionProd = fitter.m_function_BackgrPionProd->GetParError(3);
            }
            
            delete fitter.m_function_Signal;
            delete fitter.m_function_BackgrPi0HighW;
            delete fitter.m_function_BackgrQElike;
            delete fitter.m_function_BackgrPionProd;
            
            text_file << " \t\tTrue signal: " << std::endl;
            text_file << " \t\t\tConstant 'a': " << par_a_Signal << " +/- " << par_err_a_Signal  << std::endl;
            text_file << " \t\t\tSlope 'm':    " << par_m_Signal << " +/- " << par_err_m_Signal  << std::endl;
            if ( fitter.m_option_fit_function == "LinearAlt" )
                text_file << " \t\t\tPivot 'T':    " << par_T_Signal << " +/- " << par_err_T_Signal  << std::endl;
            if ( fitter.m_option_fit_function == "Bilinear" || fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSlope 'b':    " << par_b_Signal << " +/- " << par_err_b_Signal  << std::endl;
            if ( fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSwitch 'T':   " << par_T_Signal << " +/- " << par_err_T_Signal  << std::endl;
            text_file << std::endl;
            
            text_file << " \t\tTrue high-W pi0: " << std::endl;
            text_file << " \t\t\tConstant 'a': " << par_a_Pi0HighW << " +/- " << par_err_a_Pi0HighW  << std::endl;
            text_file << " \t\t\tSlope 'm':    " << par_m_Pi0HighW << " +/- " << par_err_m_Pi0HighW  << std::endl;
            if ( fitter.m_option_fit_function == "LinearAlt" )
                text_file << " \t\t\tPivot 'T':    " << par_T_Pi0HighW << " +/- " << par_err_T_Pi0HighW  << std::endl;
            if ( fitter.m_option_fit_function == "Bilinear" || fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSlope 'b':    " << par_b_Pi0HighW << " +/- " << par_err_b_Pi0HighW  << std::endl;
            if ( fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSwitch 'T':   " << par_T_Pi0HighW << " +/- " << par_err_T_Pi0HighW  << std::endl;
            text_file << std::endl;
            
            text_file << " \t\tTrue QE-like: " << std::endl;
            text_file << " \t\t\tConstant 'a': " << par_a_QElike << " +/- " << par_err_a_QElike  << std::endl;
            text_file << " \t\t\tSlope 'm':    " << par_m_QElike << " +/- " << par_err_m_QElike  << std::endl;
            if ( fitter.m_option_fit_function == "LinearAlt" )
                text_file << " \t\t\tPivot 'T':    " << par_T_QElike << " +/- " << par_err_T_QElike  << std::endl;
            if ( fitter.m_option_fit_function == "Bilinear" || fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSlope 'b':    " << par_b_QElike << " +/- " << par_err_b_QElike  << std::endl;
            if ( fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSwitch 'T':   " << par_T_QElike << " +/- " << par_err_T_QElike  << std::endl;
            text_file << std::endl;
            
            text_file << " \t\tTrue pion production: " << std::endl;
            text_file << " \t\t\tConstant 'a': " << par_a_PionProd << " +/- " << par_err_a_PionProd  << std::endl;
            text_file << " \t\t\tSlope 'm':    " << par_m_PionProd << " +/- " << par_err_m_PionProd  << std::endl;
            if ( fitter.m_option_fit_function == "LinearAlt" )
                text_file << " \t\t\tPivot 'T':    " << par_T_PionProd << " +/- " << par_err_T_PionProd  << std::endl;
            if ( fitter.m_option_fit_function == "Bilinear" || fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSlope 'b':    " << par_b_PionProd << " +/- " << par_err_b_PionProd  << std::endl;
            if ( fitter.m_option_fit_function == "BilinearAlt" )
                text_file << " \t\t\tSwitch 'T':   " << par_T_PionProd << " +/- " << par_err_T_PionProd  << std::endl;
            text_file << std::endl;
            
        }  // End of loop over universes
        
    }  // End of loop over error bands
    
    
    // Write fit parameter histograms
    // ==============================
    
    fout.cd();
    
    for ( int iter = 1; iter <= n_iterations; ++iter ) {
        h_Par_a_Signal[iter-1]   -> Write();
        h_Par_a_Pi0HighW[iter-1] -> Write();
        h_Par_a_QElike[iter-1]   -> Write();
        h_Par_a_PionProd[iter-1] -> Write();
        
        if ( option_fit_function == "Linear" || option_fit_function == "LinearAlt" || option_fit_function == "Bilinear" || option_fit_function == "BilinearAlt" ) {
            h_Par_m_Signal[iter-1]   -> Write();
            h_Par_m_Pi0HighW[iter-1] -> Write();
            h_Par_m_QElike[iter-1]   -> Write();
            h_Par_m_PionProd[iter-1] -> Write();
        }
        
        if ( option_fit_function == "LinearAlt" || option_fit_function == "BilinearAlt" ) {
            h_Par_T_Signal[iter-1]   -> Write();
            h_Par_T_Pi0HighW[iter-1] -> Write();
            h_Par_T_QElike[iter-1]   -> Write();
            h_Par_T_PionProd[iter-1] -> Write();
        }
        
        if ( option_fit_function == "Bilinear" || option_fit_function == "BilinearAlt" ) {
            h_Par_b_Signal[iter-1]   -> Write();
            h_Par_b_Pi0HighW[iter-1] -> Write();
            h_Par_b_QElike[iter-1]   -> Write();
            h_Par_b_PionProd[iter-1] -> Write();
        }
    }
    
    for ( int iter = 1; iter <= n_iterations; ++iter ) {
        delete h_Par_a_Signal[iter-1];
        delete h_Par_a_Pi0HighW[iter-1];
        delete h_Par_a_QElike[iter-1];
        delete h_Par_a_PionProd[iter-1];
        
        delete h_Par_m_Signal[iter-1];
        delete h_Par_m_Pi0HighW[iter-1];
        delete h_Par_m_QElike[iter-1];
        delete h_Par_m_PionProd[iter-1];
        
        delete h_Par_T_Signal[iter-1];
        delete h_Par_T_Pi0HighW[iter-1];
        delete h_Par_T_QElike[iter-1];
        delete h_Par_T_PionProd[iter-1];
        
        delete h_Par_b_Signal[iter-1];
        delete h_Par_b_Pi0HighW[iter-1];
        delete h_Par_b_QElike[iter-1];
        delete h_Par_b_PionProd[iter-1];
    }
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
 * The only input needed is the variable use to extract its MC histograms,
 * since it already contains the information of the plastic weights because
 * those have been already loaded beforehand. */

void ScalePlasticBackgr(Variable* var)
{
    // Signal region
    // =============
    
    // Get entries of MC before tuning
    double Nentries_SigReg          = var->m_hists.m_mc_SigReg.hist->GetEntries();
    double Nentries_SigReg_PlasUp   = var->m_hists.m_mc_SigReg_BackgrPlasUp.hist->GetEntries();
    double Nentries_SigReg_PlasBetw = var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist->GetEntries();
    double Nentries_SigReg_PlasDown = var->m_hists.m_mc_SigReg_BackgrPlasDown.hist->GetEntries();
    
    // Subtract non-tuned plastic from total MC
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasUp.hist,   -1);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist, -1);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasDown.hist, -1);
    
    // Scale plastic
    var->m_hists.m_mc_SigReg_BackgrPlasUp.hist   -> Multiply(var->m_hists.m_mc_SigReg_BackgrPlasUp.hist,   var->m_hists.m_mc_Weight_SigReg_TruePlasUp.hist);
    var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist -> Multiply(var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist, var->m_hists.m_mc_Weight_SigReg_TruePlasBetw.hist);
    var->m_hists.m_mc_SigReg_BackgrPlasDown.hist -> Multiply(var->m_hists.m_mc_SigReg_BackgrPlasDown.hist, var->m_hists.m_mc_Weight_SigReg_TruePlasDown.hist);
    
    // Add tuned plastic to total MC
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasUp.hist);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPlasDown.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_SigReg.hist                -> SetEntries(Nentries_SigReg);
    var->m_hists.m_mc_SigReg_BackgrPlasUp.hist   -> SetEntries(Nentries_SigReg_PlasUp);
    var->m_hists.m_mc_SigReg_BackgrPlasBetw.hist -> SetEntries(Nentries_SigReg_PlasBetw);
    var->m_hists.m_mc_SigReg_BackgrPlasDown.hist -> SetEntries(Nentries_SigReg_PlasDown);
    
    
    // Pion-like shower sideband
    // =========================
    
    // Get entries of MC before tuning
    double Nentries_PionBlobSB          = var->m_hists.m_mc_PionBlobSB.hist->GetEntries();
    double Nentries_PionBlobSB_PlasUp   = var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist->GetEntries();
    double Nentries_PionBlobSB_PlasBetw = var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist->GetEntries();
    double Nentries_PionBlobSB_PlasDown = var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist->GetEntries();
    
    // Subtract non-tuned plastic from total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist,   -1);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist, -1);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist, -1);
    
    // Scale plastic
    var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist   -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist,   var->m_hists.m_mc_Weight_PionBlobSB_TruePlasUp.hist);
    var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasBetw.hist);
    var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist, var->m_hists.m_mc_Weight_PionBlobSB_TruePlasDown.hist);
    
    // Add tuned plastic to total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_PionBlobSB.hist                -> SetEntries(Nentries_PionBlobSB);
    var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.hist   -> SetEntries(Nentries_PionBlobSB_PlasUp);
    var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.hist -> SetEntries(Nentries_PionBlobSB_PlasBetw);
    var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.hist -> SetEntries(Nentries_PionBlobSB_PlasDown);
    
    
    // Proton-like shower sideband
    // ===========================
    
    // Get entries of MC before tuning
    double Nentries_ProtonBlobSB          = var->m_hists.m_mc_ProtonBlobSB.hist->GetEntries();
    double Nentries_ProtonBlobSB_PlasUp   = var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist->GetEntries();
    double Nentries_ProtonBlobSB_PlasBetw = var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist->GetEntries();
    double Nentries_ProtonBlobSB_PlasDown = var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist->GetEntries();
    
    // Subtract non-tuned plastic from total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist,   -1);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist, -1);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist, -1);
    
    // Scale plastic
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist   -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist,   var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasUp.hist);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasBetw.hist);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist, var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasDown.hist);
    
    // Add tuned plastic to total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_ProtonBlobSB.hist                -> SetEntries(Nentries_ProtonBlobSB);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.hist   -> SetEntries(Nentries_ProtonBlobSB_PlasUp);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.hist -> SetEntries(Nentries_ProtonBlobSB_PlasBetw);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.hist -> SetEntries(Nentries_ProtonBlobSB_PlasDown);
    
    
    // High-W sideband
    // ===============
    
    // Get entries of MC before tuning
    double Nentries_HighWSB          = var->m_hists.m_mc_HighWSB.hist->GetEntries();
    double Nentries_HighWSB_PlasUp   = var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist->GetEntries();
    double Nentries_HighWSB_PlasBetw = var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist->GetEntries();
    double Nentries_HighWSB_PlasDown = var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist->GetEntries();
    
    // Subtract non-tuned plastic from total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist,   -1);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist, -1);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist, -1);
    
    // Scale plastic
    var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist   -> Multiply(var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist,   var->m_hists.m_mc_Weight_HighWSB_TruePlasUp.hist);
    var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist -> Multiply(var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist, var->m_hists.m_mc_Weight_HighWSB_TruePlasBetw.hist);
    var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist -> Multiply(var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist, var->m_hists.m_mc_Weight_HighWSB_TruePlasDown.hist);
    
    // Add tuned plastic to total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_HighWSB.hist                -> SetEntries(Nentries_HighWSB);
    var->m_hists.m_mc_HighWSB_BackgrPlasUp.hist   -> SetEntries(Nentries_HighWSB_PlasUp);
    var->m_hists.m_mc_HighWSB_BackgrPlasBetw.hist -> SetEntries(Nentries_HighWSB_PlasBetw);
    var->m_hists.m_mc_HighWSB_BackgrPlasDown.hist -> SetEntries(Nentries_HighWSB_PlasDown);
}





// ==============================================================================
//  SCALE PHYSICS BACKGROUND
// ==============================================================================

/* This function scales true signal and physics background HistWrappers
 * of each variable by using MC physics weight HistWrappers that cointain
 * the functional form of the physics weights accordingly binned.
 * 
 * The scaling is done by three steps, similarly as plastic:
 * -> First, subtract the true physics HistWrapper from the total MC HistWrapper.
 * -> Second, scale true physics HW multiplying it by the MC physics weight HW.
 * -> Third, add the scaled true physics HW to the total MC HW.
 * 
 * There are two inputs for these functions:
 * -> The variable whose histograms are going to be scaled.
 * -> All of the physics weights that are used in the tuning. */

/* ---------------------------------------------
    THIS FUNCTION SCALES ALL PHYSICS BACKGROUND
    ON SIGNAL REGION AND PHYSICS SIDEBANDS
   --------------------------------------------- */
void ScalePhysicsBackgr(Variable* var,
                        CVHW hw_Weight_Pi0HighW,
                        CVHW hw_Weight_QElike,
                        CVHW hw_Weight_PionProd)
{
    // Signal region
    // =============
    
    // Get entries of MC before tuning
    double Nentries_SigReg          = var->m_hists.m_mc_SigReg.hist->GetEntries();
    double Nentries_SigReg_Pi0HighW = var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist->GetEntries();
    double Nentries_SigReg_QElike   = var->m_hists.m_mc_SigReg_BackgrQElike.hist->GetEntries();
    double Nentries_SigReg_PionProd = var->m_hists.m_mc_SigReg_BackgrPionProd.hist->GetEntries();
    
    // Subtract non-tuned physics background from total MC
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist, -1);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrQElike.hist,   -1);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPionProd.hist, -1);
    
    // Scale physics background
    var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist -> Multiply(var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist, hw_Weight_Pi0HighW.hist);
    var->m_hists.m_mc_SigReg_BackgrQElike.hist   -> Multiply(var->m_hists.m_mc_SigReg_BackgrQElike.hist,   hw_Weight_QElike.hist);
    var->m_hists.m_mc_SigReg_BackgrPionProd.hist -> Multiply(var->m_hists.m_mc_SigReg_BackgrPionProd.hist, hw_Weight_PionProd.hist);
    
    // Add tuned physics background to total MC
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrQElike.hist);
    var->m_hists.m_mc_SigReg.hist -> Add(var->m_hists.m_mc_SigReg_BackgrPionProd.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_SigReg.hist                -> SetEntries(Nentries_SigReg);
    var->m_hists.m_mc_SigReg_BackgrPi0HighW.hist -> SetEntries(Nentries_SigReg_Pi0HighW);
    var->m_hists.m_mc_SigReg_BackgrQElike.hist   -> SetEntries(Nentries_SigReg_QElike);
    var->m_hists.m_mc_SigReg_BackgrPionProd.hist -> SetEntries(Nentries_SigReg_PionProd);
    
    
    // Pion-like shower sideband
    // =========================
    
    // Get entries of MC before tuning
    double Nentries_PionBlobSB          = var->m_hists.m_mc_PionBlobSB.hist->GetEntries();
    double Nentries_PionBlobSB_Pi0HighW = var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist->GetEntries();
    double Nentries_PionBlobSB_QElike   = var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist->GetEntries();
    double Nentries_PionBlobSB_PionProd = var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist->GetEntries();
    
    // Subtract non-tuned physics background from total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist, -1);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist,   -1);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist, -1);
    
    // Scale physics background
    var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist, hw_Weight_Pi0HighW.hist);
    var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist   -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist,   hw_Weight_QElike.hist);
    var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist -> Multiply(var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist, hw_Weight_PionProd.hist);
    
    // Add tuned physics background to total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist);
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_PionBlobSB.hist                -> SetEntries(Nentries_PionBlobSB);
    var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.hist -> SetEntries(Nentries_PionBlobSB_Pi0HighW);
    var->m_hists.m_mc_PionBlobSB_BackgrQElike.hist   -> SetEntries(Nentries_PionBlobSB_QElike);
    var->m_hists.m_mc_PionBlobSB_BackgrPionProd.hist -> SetEntries(Nentries_PionBlobSB_PionProd);
    
    
    // Proton-like shower sideband
    // ===========================
    
    // Get entries of MC before tuning
    double Nentries_ProtonBlobSB          = var->m_hists.m_mc_ProtonBlobSB.hist->GetEntries();
    double Nentries_ProtonBlobSB_Pi0HighW = var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist->GetEntries();
    double Nentries_ProtonBlobSB_QElike   = var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist->GetEntries();
    double Nentries_ProtonBlobSB_PionProd = var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist->GetEntries();
    
    // Subtract non-tuned physics background from total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist, -1);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist,   -1);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist, -1);
    
    // Scale physics background
    var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist, hw_Weight_Pi0HighW.hist);
    var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist   -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist,   hw_Weight_QElike.hist);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist -> Multiply(var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist, hw_Weight_PionProd.hist);
    
    // Add tuned physics background to total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist);
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_ProtonBlobSB.hist                -> SetEntries(Nentries_ProtonBlobSB);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.hist -> SetEntries(Nentries_ProtonBlobSB_Pi0HighW);
    var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.hist   -> SetEntries(Nentries_ProtonBlobSB_QElike);
    var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.hist -> SetEntries(Nentries_ProtonBlobSB_PionProd);
    
    
    // High-W sideband
    // ===============
    
    // Get entries of MC before tuning
    double Nentries_HighWSB          = var->m_hists.m_mc_HighWSB.hist->GetEntries();
    double Nentries_HighWSB_Pi0HighW = var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist->GetEntries();
    double Nentries_HighWSB_QElike   = var->m_hists.m_mc_HighWSB_BackgrQElike.hist->GetEntries();
    double Nentries_HighWSB_PionProd = var->m_hists.m_mc_HighWSB_BackgrPionProd.hist->GetEntries();
    
    // Subtract non-tuned physics background from total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist, -1);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrQElike.hist,   -1);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPionProd.hist, -1);
    
    // Scale physics background
    var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist -> Multiply(var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist, hw_Weight_Pi0HighW.hist);
    var->m_hists.m_mc_HighWSB_BackgrQElike.hist   -> Multiply(var->m_hists.m_mc_HighWSB_BackgrQElike.hist,   hw_Weight_QElike.hist);
    var->m_hists.m_mc_HighWSB_BackgrPionProd.hist -> Multiply(var->m_hists.m_mc_HighWSB_BackgrPionProd.hist, hw_Weight_PionProd.hist);
    
    // Add tuned physics background to total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrQElike.hist);
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_BackgrPionProd.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_HighWSB.hist                -> SetEntries(Nentries_HighWSB);
    var->m_hists.m_mc_HighWSB_BackgrPi0HighW.hist -> SetEntries(Nentries_HighWSB_Pi0HighW);
    var->m_hists.m_mc_HighWSB_BackgrQElike.hist   -> SetEntries(Nentries_HighWSB_QElike);
    var->m_hists.m_mc_HighWSB_BackgrPionProd.hist -> SetEntries(Nentries_HighWSB_PionProd);
}



/* -------------------------------------------
    THIS FUNCTION SCALES TRUE SIGNAL BUT ONLY
    ON THE PHYSICS SIDEBANDS
   ------------------------------------------- */
void ScaleSignal(Variable* var,
                 CVHW hw_Weight_Signal)
{
    // Pion-like shower sideband
    // =========================
    
    // Get entries of MC before tuning
    double Nentries_PionBlobSB        = var->m_hists.m_mc_PionBlobSB.hist->GetEntries();
    double Nentries_PionBlobSB_Signal = var->m_hists.m_mc_PionBlobSB_Signal.hist->GetEntries();
    
    // Subtract non-tuned signal from total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_Signal.hist, -1);
    
    // Scale signal
    var->m_hists.m_mc_PionBlobSB_Signal.hist -> Multiply(var->m_hists.m_mc_PionBlobSB_Signal.hist, hw_Weight_Signal.hist);
    
    // Add tuned signal to total MC
    var->m_hists.m_mc_PionBlobSB.hist -> Add(var->m_hists.m_mc_PionBlobSB_Signal.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_PionBlobSB.hist        -> SetEntries(Nentries_PionBlobSB);
    var->m_hists.m_mc_PionBlobSB_Signal.hist -> SetEntries(Nentries_PionBlobSB_Signal);
    
    
    // Proton-like shower sideband
    // ===========================
    
    // Get entries of MC before tuning
    double Nentries_ProtonBlobSB        = var->m_hists.m_mc_ProtonBlobSB.hist->GetEntries();
    double Nentries_ProtonBlobSB_Signal = var->m_hists.m_mc_ProtonBlobSB_Signal.hist->GetEntries();
    
    // Subtract non-tuned signal from total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_Signal.hist, -1);
    
    // Scale signal
    var->m_hists.m_mc_ProtonBlobSB_Signal.hist -> Multiply(var->m_hists.m_mc_ProtonBlobSB_Signal.hist, hw_Weight_Signal.hist);
    
    // Add tuned signal to total MC
    var->m_hists.m_mc_ProtonBlobSB.hist -> Add(var->m_hists.m_mc_ProtonBlobSB_Signal.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_ProtonBlobSB.hist        -> SetEntries(Nentries_ProtonBlobSB);
    var->m_hists.m_mc_ProtonBlobSB_Signal.hist -> SetEntries(Nentries_ProtonBlobSB_Signal);
    
    
    // High-W sideband
    // ===============
    
    // Get entries of MC before tuning
    double Nentries_HighWSB        = var->m_hists.m_mc_HighWSB.hist->GetEntries();
    double Nentries_HighWSB_Signal = var->m_hists.m_mc_HighWSB_Signal.hist->GetEntries();
    
    // Subtract non-tuned signal from total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_Signal.hist, -1);
    
    // Scale signal
    var->m_hists.m_mc_HighWSB_Signal.hist -> Multiply(var->m_hists.m_mc_HighWSB_Signal.hist, hw_Weight_Signal.hist);
    
    // Add tuned signal to total MC
    var->m_hists.m_mc_HighWSB.hist -> Add(var->m_hists.m_mc_HighWSB_Signal.hist);
    
    // Set entries of MC after tuning
    var->m_hists.m_mc_HighWSB.hist        -> SetEntries(Nentries_HighWSB);
    var->m_hists.m_mc_HighWSB_Signal.hist -> SetEntries(Nentries_HighWSB_Signal);
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void TunePhysics(std::string option_date,
                 std::string option_model,
                 std::string option_material,
                 bool do_systematics             = true,
                 std::string option_fit_function = "Linear",
                 std::string option_fit_method   = "Combined",
                 int n_iterations                = 1)
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
    
    // MC plastic scale factors
    std::string mc_fin_plasweight_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/%s/AfterTuning", option_date_mc.c_str(),
                                                                                                                                 option_material.c_str());
    
    // MC before tuning
    std::string mc_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/%s/BeforeTuning", option_date_mc.c_str(),
                                                                                                                       option_material.c_str());
    
    // Data
    std::string data_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/data/%s/%s", option_date_data.c_str(),
                                                                                                              option_material.c_str());
    
    
    // Input files
    // ===========
    
    // MC plastic scale functions
    TFile mc_fin_plasweight(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fin_plasweight_topdir.c_str(),
                                                                                                          option_model.c_str(),
                                                                                                          option_systematics.c_str(),
                                                                                                          option_material.c_str()), "READ");
    
    // MC distributions before tuning
    TFile mc_fin(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fin_topdir.c_str(),
                                                                                                option_model.c_str(),
                                                                                                option_systematics.c_str(),
                                                                                                option_material.c_str()), "READ");
    
    // Data distributions
    TFile data_fin(Form("%s/Data_PhysicsTuning_AllPlaylists_%s.root", data_fin_topdir.c_str(),
                                                                      option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Top directory
    // =============
    
    // MC after tuning
    std::string mc_fout_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/%s/AfterTuning", option_date_mc.c_str(),
                                                                                                                       option_material.c_str());
    
    
    // Output files
    // ============
    
    // MC distributions after plastic tuning only
    TFile mc_fout_after_plastic(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fout_topdir.c_str(),
                                                                                                              option_model.c_str(),
                                                                                                              option_systematics.c_str(),
                                                                                                              option_material.c_str()), "RECREATE");
    
    // MC distributions after plastic + physics tuning (signal NOT tuned) AND physics scale functions
    TFile mc_fout_after_physics(Form("%s/MC_AfterPhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_%s.root", mc_fout_topdir.c_str(),
                                                                                                                    option_model.c_str(),
                                                                                                                    option_systematics.c_str(),
                                                                                                                    option_fit_function.c_str(),
                                                                                                                    option_material.c_str()), "RECREATE");
    
    // MC distributions after plastic + physics tuning (INCLUDING signal tuned) AND physics scale functions
    TFile mc_fout_after_physics_sigtun(Form("%s/MC_AfterPhysicsTuningWithSignalTune_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_%s.root", mc_fout_topdir.c_str(),
                                                                                                                                         option_model.c_str(),
                                                                                                                                         option_systematics.c_str(),
                                                                                                                                         option_fit_function.c_str(),
                                                                                                                                         option_material.c_str()), "RECREATE");
    
    // Fit parameters for each iteration
    TFile mc_fout_parameters(Form("%s/MC_Parameters_%sFit_%s.root", mc_fout_topdir.c_str(),
                                                                    option_fit_function.c_str(),
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
    std::cout << " Tuning physics background... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << mc_pot   << std::endl;
    std::cout << " \tData POT: " << data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Write POT to output files (MC-only outputs)
    WritePOT(mc_fout_after_plastic,        true, mc_pot);
    WritePOT(mc_fout_after_physics,        true, mc_pot);
    WritePOT(mc_fout_after_physics_sigtun, true, mc_pot);
    
    
    // Text file with scale function info
    std::string text_scale_topdir = Form("/minerva/data/users/gonzalo/MAT/PhysicsTuning/ScaleFunctions/%s/%s", option_date_mc.c_str(),
                                                                                                               option_material.c_str());
    
    std::ofstream text_scale(Form("%s/PhysicsScaleFunctions_%sFit_%s.txt", text_scale_topdir.c_str(),
                                                                           option_fit_function.c_str(),
                                                                           option_material.c_str()));
    
    
    
    // =========================================
    //  Prepare histograms before tuning
    // =========================================
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Initialize MC physics weight histograms
    for ( auto var : variables )
        var -> InitMCWeights_PhysBackgr(util.m_error_bands);
    
    
    // Load MC plastic weights from input files
    for ( auto var : variables ) {
        var -> LoadMCWeights_PlasBackgr_In_SigReg(mc_fin_plasweight, util.m_error_bands);
        var -> LoadMCWeights_PlasBackgr_In_PhysSB(mc_fin_plasweight, util.m_error_bands);
    }
    
    
    // Load distribution histograms before tuning
    for ( auto var : variables ) {
        var -> LoadMCHists_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PhysSB(data_fin);
    }
    
    
    
    // =========================================
    //  Tune plastic
    // =========================================
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Scale plastic background
        ScalePlasticBackgr(var);
        
        // Sync MC histograms
        var -> SyncMCHists_PhysSB();
        
        // Write MC histograms after plastic tuning
        var -> WriteMCHists_PhysSB(mc_fout_after_plastic);
    }
    
    
    
    // =========================================
    //  Tune physics and write histograms
    // =========================================
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Fill MC physics weights
        GetMCPhysicsWeight(util, var,
                           var->m_hists.m_mc_Weight_Signal,
                           var->m_hists.m_mc_Weight_BackgrPi0HighW,
                           var->m_hists.m_mc_Weight_BackgrQElike,
                           var->m_hists.m_mc_Weight_BackgrPionProd,
                           type_model, option_material,
                           option_fit_method, option_fit_function,
                           n_iterations, mc_fout_parameters, text_scale);
        
        
        // Sync MC physics weights
        var -> SyncMCWeights_PhysBackgr();
        
        
        // Tune physics background only (NOT signal)
        // -----------------------------------------
        
        // Scale physics background
        ScalePhysicsBackgr(var,
                           var->m_hists.m_mc_Weight_BackgrPi0HighW,
                           var->m_hists.m_mc_Weight_BackgrQElike,
                           var->m_hists.m_mc_Weight_BackgrPionProd);
        
        // Sync MC histograms
        var -> SyncMCHists_PhysSB();
        
        // Write MC histograms and physics weights after physics background-only tuning
        var -> WriteMCHists_PhysSB(mc_fout_after_physics);
        var -> WriteMCWeights_PhysBackgr(mc_fout_after_physics);
        
        
        // Tune signal only (physics background already tuned)
        // ---------------------------------------------------
        
        // Scale signal
        ScaleSignal(var,
                    var->m_hists.m_mc_Weight_Signal);
        
        // Sync MC histograms
        var -> SyncMCHists_PhysSB();
        
        // Write MC histograms and MC physics weights after physics background + signal tuning
        var -> WriteMCHists_PhysSB(mc_fout_after_physics_sigtun);
        var -> WriteMCWeights_PhysBackgr(mc_fout_after_physics_sigtun);
    }
    
    
    // Close scale function text file
    text_scale.close();
    
    
    // Close ROOT files
    mc_fin.Close();
    data_fin.Close();
    
    mc_fout_after_plastic.Close();
    mc_fout_after_physics.Close();
    mc_fout_after_physics_sigtun.Close();
    mc_fout_parameters.Close();
}


#endif  // TunePhysics_C