#ifndef PhysicsFitter_cxx
#define PhysicsFitter_cxx

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "PhysicsFitter.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TF1.h"



// ==============================================================================================
// 
// HOW ARE FIT FUNCTIONS DEFINED BY TYPE:
// =====================================
//
// SCALAR: y = a
// ------
//  -> a: Constant
//
// LINEAR: y = a + m (x - m_pivotX)
// ------
//  -> a: Constant
//  -> m: Slope
//
// LINEAR ALTERNATIVE: y = a + m (x - T)
// ------------------
//  -> a: Constant
//  -> m: Slope
//  -> T: Pivot location in x
//
// BILINEAR: y = [1.0 - b * max(0, m_switchX - x)] * [a + m (x - m_pivotX)]
// --------
//  -> a: Constant of linear function for x > 0.325
//  -> m: Slope of linear function for x > 0.325
//  -> b: Slope of linear function for x < 0.325
//
// BILINEAR ALTERNATIVE: y = [1.0 - b * max(0, T - x)] * [a + m (x - m_pivotX)]
// --------------------
//  -> a: Constant of linear function for x > T
//  -> m: Slope of linear function for x > T
//  -> b: Slope of linear function for x < T
//  -> T: Switch point in X between both linear functions
//
// Input-defined parameters (depend on the material):
// -------------------------------------------------
//  -> m_pivotX: Pivot point of standard linear function, usually defined at the peak
//  -> m_switchX: Switch point between standard linear function and linear function for low-pT
//
// ==============================================================================================



// ==========================================================================
//  Static member variables
// ==========================================================================

// Signal region
TH1D* PhysicsFitter::m_data_SigReg              = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_Signal         = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrPi0HighW = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrQElike   = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrPionProd = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrPlasUp   = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrPlasBetw = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrPlasDown = NULL;
TH1D* PhysicsFitter::m_mc_SigReg_BackgrOther    = NULL;


// Pion-like shower sideband
TH1D* PhysicsFitter::m_data_PionBlobSB              = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_Signal         = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrPi0HighW = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrQElike   = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrPionProd = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrPlasUp   = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrPlasBetw = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrPlasDown = NULL;
TH1D* PhysicsFitter::m_mc_PionBlobSB_BackgrOther    = NULL;


// Proton-like shower sideband
TH1D* PhysicsFitter::m_data_ProtonBlobSB              = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_Signal         = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrPi0HighW = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrQElike   = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrPionProd = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrPlasUp   = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrPlasBetw = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrPlasDown = NULL;
TH1D* PhysicsFitter::m_mc_ProtonBlobSB_BackgrOther    = NULL;


// High-W sideband
TH1D* PhysicsFitter::m_data_HighWSB              = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_Signal         = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrPi0HighW = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrQElike   = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrPionProd = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrPlasUp   = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrPlasBetw = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrPlasDown = NULL;
TH1D* PhysicsFitter::m_mc_HighWSB_BackgrOther    = NULL;


// Fit option
std::string PhysicsFitter::m_option_fit_function = "";


// Function pivot and switch in X
double PhysicsFitter::m_function_pivotX  = 0.0;
double PhysicsFitter::m_function_switchX = 0.0;





// ==========================================================================
//  Constructors
// ==========================================================================

// Parametrized constructor
// ========================

PhysicsFitter::PhysicsFitter(const CVUniverse& universe,
                             Histograms hists,
                             const EnumModels& type_model,
                             std::string option_material,
                             std::string option_fit_function,
                             int n_iterations)
    : m_model_type(type_model),
      m_option_material(option_material),
      m_n_iterations(n_iterations)
{
    Initialize(universe, hists, option_material, option_fit_function);
}


// Destructor
// ==========

PhysicsFitter::~PhysicsFitter() {}





// ==========================================================================
//  Initialize
// ==========================================================================

void PhysicsFitter::Initialize(const CVUniverse& universe,
                               Histograms hists,
                               std::string option_material,
                               std::string option_fit_function)
{
    // Histograms
    // ==========
    
    // Signal region
    m_data_SigReg              = new TH1D(*hists.m_data_SigReg);
    m_mc_SigReg_Signal         = new TH1D(*hists.m_mc_SigReg_Signal.univHist(&universe)); 
    m_mc_SigReg_BackgrPi0HighW = new TH1D(*hists.m_mc_SigReg_BackgrPi0HighW.univHist(&universe));
    m_mc_SigReg_BackgrQElike   = new TH1D(*hists.m_mc_SigReg_BackgrQElike.univHist(&universe));
    m_mc_SigReg_BackgrPionProd = new TH1D(*hists.m_mc_SigReg_BackgrPionProd.univHist(&universe));
    m_mc_SigReg_BackgrPlasUp   = new TH1D(*hists.m_mc_SigReg_BackgrPlasUp.univHist(&universe));
    m_mc_SigReg_BackgrPlasBetw = new TH1D(*hists.m_mc_SigReg_BackgrPlasBetw.univHist(&universe));
    m_mc_SigReg_BackgrPlasDown = new TH1D(*hists.m_mc_SigReg_BackgrPlasDown.univHist(&universe));
    m_mc_SigReg_BackgrOther    = new TH1D(*hists.m_mc_SigReg_BackgrOther.univHist(&universe));
    
    
    // Pion-like shower sideband
    m_data_PionBlobSB              = new TH1D(*hists.m_data_PionBlobSB);
    m_mc_PionBlobSB_Signal         = new TH1D(*hists.m_mc_PionBlobSB_Signal.univHist(&universe)); 
    m_mc_PionBlobSB_BackgrPi0HighW = new TH1D(*hists.m_mc_PionBlobSB_BackgrPi0HighW.univHist(&universe));
    m_mc_PionBlobSB_BackgrQElike   = new TH1D(*hists.m_mc_PionBlobSB_BackgrQElike.univHist(&universe));
    m_mc_PionBlobSB_BackgrPionProd = new TH1D(*hists.m_mc_PionBlobSB_BackgrPionProd.univHist(&universe));
    m_mc_PionBlobSB_BackgrPlasUp   = new TH1D(*hists.m_mc_PionBlobSB_BackgrPlasUp.univHist(&universe));
    m_mc_PionBlobSB_BackgrPlasBetw = new TH1D(*hists.m_mc_PionBlobSB_BackgrPlasBetw.univHist(&universe));
    m_mc_PionBlobSB_BackgrPlasDown = new TH1D(*hists.m_mc_PionBlobSB_BackgrPlasDown.univHist(&universe));
    m_mc_PionBlobSB_BackgrOther    = new TH1D(*hists.m_mc_PionBlobSB_BackgrOther.univHist(&universe));
    
    
    // Proton-like shower sideband
    m_data_ProtonBlobSB              = new TH1D(*hists.m_data_ProtonBlobSB);
    m_mc_ProtonBlobSB_Signal         = new TH1D(*hists.m_mc_ProtonBlobSB_Signal.univHist(&universe)); 
    m_mc_ProtonBlobSB_BackgrPi0HighW = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrPi0HighW.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrQElike   = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrQElike.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrPionProd = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrPionProd.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrPlasUp   = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrPlasUp.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrPlasBetw = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrPlasBetw.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrPlasDown = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrPlasDown.univHist(&universe));
    m_mc_ProtonBlobSB_BackgrOther    = new TH1D(*hists.m_mc_ProtonBlobSB_BackgrOther.univHist(&universe));
    
    
    // High-W sideband
    m_data_HighWSB              = new TH1D(*hists.m_data_HighWSB);
    m_mc_HighWSB_Signal         = new TH1D(*hists.m_mc_HighWSB_Signal.univHist(&universe)); 
    m_mc_HighWSB_BackgrPi0HighW = new TH1D(*hists.m_mc_HighWSB_BackgrPi0HighW.univHist(&universe));
    m_mc_HighWSB_BackgrQElike   = new TH1D(*hists.m_mc_HighWSB_BackgrQElike.univHist(&universe));
    m_mc_HighWSB_BackgrPionProd = new TH1D(*hists.m_mc_HighWSB_BackgrPionProd.univHist(&universe));
    m_mc_HighWSB_BackgrPlasUp   = new TH1D(*hists.m_mc_HighWSB_BackgrPlasUp.univHist(&universe));
    m_mc_HighWSB_BackgrPlasBetw = new TH1D(*hists.m_mc_HighWSB_BackgrPlasBetw.univHist(&universe));
    m_mc_HighWSB_BackgrPlasDown = new TH1D(*hists.m_mc_HighWSB_BackgrPlasDown.univHist(&universe));
    m_mc_HighWSB_BackgrOther    = new TH1D(*hists.m_mc_HighWSB_BackgrOther.univHist(&universe));
    
    
    // Fit function
    // ============
    
    if ( option_fit_function == "Scalar" ) {
        m_option_fit_function = "Scalar";
    }
    
    else if ( option_fit_function == "Linear" ) {
        m_option_fit_function = "Linear";
        m_function_pivotX = 0.5125;
    }
    
    else if ( option_fit_function == "LinearAlt" ) {
        m_option_fit_function = "LinearAlt";
    }
    
    else if ( option_fit_function == "Bilinear" ) {
        m_option_fit_function = "Bilinear";
        m_function_pivotX  = 0.5125;
        if ( option_material == "lead" )      m_function_switchX = 0.250;
        else if ( option_material == "iron" ) m_function_switchX = 0.250;
    }
    
    else if ( option_fit_function == "BilinearAlt" ) {
        m_option_fit_function = "BilinearAlt";
        m_function_pivotX = 0.5125;
    }
    
    
    // Minimum and maximum in X
    // ========================
    
    TH1D* h_data_temp = (TH1D*)m_data_SigReg->Clone("data_temp");
    int Nbins = h_data_temp->GetNbinsX();
    
    // X min info
    double firstbin_low_edge = h_data_temp->GetBinLowEdge(1);
    m_Xmin = firstbin_low_edge;
    
    // Xmax info
    double lastbin_low_edge = h_data_temp->GetBinLowEdge(Nbins);
    double lastbin_width    = h_data_temp->GetBinWidth(Nbins);
    m_Xmax = lastbin_low_edge + lastbin_width;
}





// ==========================================================================
//  Combined chi2 function
// ==========================================================================

double PhysicsFitter::Chi2Function_Combined(const double* scale_vector)
{
    // Chi2 to minimize
    double chi2 = 0.0;
    
    
    // Clone input histograms
    // ======================
    
    // Signal region
    TH1D* h_data_SigReg_temp        = (TH1D*)m_data_SigReg              -> Clone("data_SigReg_temp");
    TH1D* h_mc_SigReg_Signal_temp   = (TH1D*)m_mc_SigReg_Signal         -> Clone("mc_SigReg_Signal_temp");
    TH1D* h_mc_SigReg_Pi0HighW_temp = (TH1D*)m_mc_SigReg_BackgrPi0HighW -> Clone("mc_SigReg_QElike_temp");
    TH1D* h_mc_SigReg_QElike_temp   = (TH1D*)m_mc_SigReg_BackgrQElike   -> Clone("mc_SigReg_QElike_temp");
    TH1D* h_mc_SigReg_PionProd_temp = (TH1D*)m_mc_SigReg_BackgrPionProd -> Clone("mc_SigReg_PionProd_temp");
    TH1D* h_mc_SigReg_PlasUp_temp   = (TH1D*)m_mc_SigReg_BackgrPlasUp   -> Clone("mc_SigReg_PlasUp_temp");
    TH1D* h_mc_SigReg_PlasBetw_temp = (TH1D*)m_mc_SigReg_BackgrPlasBetw -> Clone("mc_SigReg_PlasBetw_temp");
    TH1D* h_mc_SigReg_PlasDown_temp = (TH1D*)m_mc_SigReg_BackgrPlasDown -> Clone("mc_SigReg_PlasDown_temp");
    TH1D* h_mc_SigReg_Other_temp    = (TH1D*)m_mc_SigReg_BackgrOther    -> Clone("mc_SigReg_Other_temp");
    
    // Pion-like shower sideband
    TH1D* h_data_PionBlobSB_temp        = (TH1D*)m_data_PionBlobSB              -> Clone("data_PionBlobSB_temp");
    TH1D* h_mc_PionBlobSB_Signal_temp   = (TH1D*)m_mc_PionBlobSB_Signal         -> Clone("mc_PionBlobSB_Signal_temp");
    TH1D* h_mc_PionBlobSB_Pi0HighW_temp = (TH1D*)m_mc_PionBlobSB_BackgrPi0HighW -> Clone("mc_PionBlobSB_QElike_temp");
    TH1D* h_mc_PionBlobSB_QElike_temp   = (TH1D*)m_mc_PionBlobSB_BackgrQElike   -> Clone("mc_PionBlobSB_QElike_temp");
    TH1D* h_mc_PionBlobSB_PionProd_temp = (TH1D*)m_mc_PionBlobSB_BackgrPionProd -> Clone("mc_PionBlobSB_PionProd_temp");
    TH1D* h_mc_PionBlobSB_PlasUp_temp   = (TH1D*)m_mc_PionBlobSB_BackgrPlasUp   -> Clone("mc_PionBlobSB_PlasUp_temp");
    TH1D* h_mc_PionBlobSB_PlasBetw_temp = (TH1D*)m_mc_PionBlobSB_BackgrPlasBetw -> Clone("mc_PionBlobSB_PlasBetw_temp");
    TH1D* h_mc_PionBlobSB_PlasDown_temp = (TH1D*)m_mc_PionBlobSB_BackgrPlasDown -> Clone("mc_PionBlobSB_PlasDown_temp");
    TH1D* h_mc_PionBlobSB_Other_temp    = (TH1D*)m_mc_PionBlobSB_BackgrOther    -> Clone("mc_PionBlobSB_Other_temp");
    
    // Proton-like shower sideband
    TH1D* h_data_ProtonBlobSB_temp        = (TH1D*)m_data_ProtonBlobSB              -> Clone("data_ProtonBlobSB_temp");
    TH1D* h_mc_ProtonBlobSB_Signal_temp   = (TH1D*)m_mc_ProtonBlobSB_Signal         -> Clone("mc_ProtonBlobSB_Signal_temp");
    TH1D* h_mc_ProtonBlobSB_Pi0HighW_temp = (TH1D*)m_mc_ProtonBlobSB_BackgrPi0HighW -> Clone("mc_ProtonBlobSB_QElike_temp");
    TH1D* h_mc_ProtonBlobSB_QElike_temp   = (TH1D*)m_mc_ProtonBlobSB_BackgrQElike   -> Clone("mc_ProtonBlobSB_QElike_temp");
    TH1D* h_mc_ProtonBlobSB_PionProd_temp = (TH1D*)m_mc_ProtonBlobSB_BackgrPionProd -> Clone("mc_ProtonBlobSB_PionProd_temp");
    TH1D* h_mc_ProtonBlobSB_PlasUp_temp   = (TH1D*)m_mc_ProtonBlobSB_BackgrPlasUp   -> Clone("mc_ProtonBlobSB_PlasUp_temp");
    TH1D* h_mc_ProtonBlobSB_PlasBetw_temp = (TH1D*)m_mc_ProtonBlobSB_BackgrPlasBetw -> Clone("mc_ProtonBlobSB_PlasBetw_temp");
    TH1D* h_mc_ProtonBlobSB_PlasDown_temp = (TH1D*)m_mc_ProtonBlobSB_BackgrPlasDown -> Clone("mc_ProtonBlobSB_PlasDown_temp");
    TH1D* h_mc_ProtonBlobSB_Other_temp    = (TH1D*)m_mc_ProtonBlobSB_BackgrOther    -> Clone("mc_ProtonBlobSB_Other_temp");
    
    // High-W sideband
    TH1D* h_data_HighWSB_temp        = (TH1D*)m_data_HighWSB              -> Clone("data_HighWSB_temp");
    TH1D* h_mc_HighWSB_Signal_temp   = (TH1D*)m_mc_HighWSB_Signal         -> Clone("mc_HighWSB_Signal_temp");
    TH1D* h_mc_HighWSB_Pi0HighW_temp = (TH1D*)m_mc_HighWSB_BackgrPi0HighW -> Clone("mc_HighWSB_QElike_temp");
    TH1D* h_mc_HighWSB_QElike_temp   = (TH1D*)m_mc_HighWSB_BackgrQElike   -> Clone("mc_HighWSB_QElike_temp");
    TH1D* h_mc_HighWSB_PionProd_temp = (TH1D*)m_mc_HighWSB_BackgrPionProd -> Clone("mc_HighWSB_PionProd_temp");
    TH1D* h_mc_HighWSB_PlasUp_temp   = (TH1D*)m_mc_HighWSB_BackgrPlasUp   -> Clone("mc_HighWSB_PlasUp_temp");
    TH1D* h_mc_HighWSB_PlasBetw_temp = (TH1D*)m_mc_HighWSB_BackgrPlasBetw -> Clone("mc_HighWSB_PlasBetw_temp");
    TH1D* h_mc_HighWSB_PlasDown_temp = (TH1D*)m_mc_HighWSB_BackgrPlasDown -> Clone("mc_HighWSB_PlasDown_temp");
    TH1D* h_mc_HighWSB_Other_temp    = (TH1D*)m_mc_HighWSB_BackgrOther    -> Clone("mc_HighWSB_Other_temp");
    
    
    // Calculate generalized chi2
    // ==========================
    
    // Get number of bins
    int Nbins = h_data_SigReg_temp->GetNbinsX();  // DON'T include overflow
    
    // Loop over bins
    for ( int bin = 1; bin <= Nbins; ++bin )
    {
        // Bin info
        double bin_center = h_data_SigReg_temp->GetBinCenter(bin);
        
        // Scale parameters
        double scale_Signal   = 1.0;
        double scale_Pi0HighW = 1.0;
        double scale_QElike   = 1.0;
        double scale_PionProd = 1.0;
        
        if ( m_option_fit_function == "Scalar" ) {
            scale_Signal   = std::max(0.0, scale_vector[0]);
            scale_Pi0HighW = std::max(0.0, scale_vector[1]);
            scale_QElike   = std::max(0.0, scale_vector[2]);
            scale_PionProd = std::max(0.0, scale_vector[3]);
        }
        
        else if ( m_option_fit_function == "Linear" ) {
            scale_Signal   = std::max(0.0, scale_vector[0] + scale_vector[1] * (bin_center - m_function_pivotX));
            scale_Pi0HighW = std::max(0.0, scale_vector[2] + scale_vector[3] * (bin_center - m_function_pivotX));
            scale_QElike   = std::max(0.0, scale_vector[4] + scale_vector[5] * (bin_center - m_function_pivotX));
            scale_PionProd = std::max(0.0, scale_vector[6] + scale_vector[7] * (bin_center - m_function_pivotX));
        }
        
        else if ( m_option_fit_function == "LinearAlt" ) {
            scale_Signal   = std::max(0.0, scale_vector[0] + scale_vector[1]  * (bin_center - scale_vector[2] ));
            scale_Pi0HighW = std::max(0.0, scale_vector[3] + scale_vector[4]  * (bin_center - scale_vector[5] ));
            scale_QElike   = std::max(0.0, scale_vector[6] + scale_vector[7]  * (bin_center - scale_vector[8] ));
            scale_PionProd = std::max(0.0, scale_vector[9] + scale_vector[10] * (bin_center - scale_vector[11]));
        }
        
        else if ( m_option_fit_function == "Bilinear" ) {
            scale_Signal   = std::max(0.0, (1.0 - scale_vector[2]  * std::max(0.0, m_function_switchX - bin_center)) * (scale_vector[0] + scale_vector[1]  * (bin_center - m_function_pivotX)));
            scale_Pi0HighW = std::max(0.0, (1.0 - scale_vector[5]  * std::max(0.0, m_function_switchX - bin_center)) * (scale_vector[3] + scale_vector[4]  * (bin_center - m_function_pivotX)));
            scale_QElike   = std::max(0.0, (1.0 - scale_vector[8]  * std::max(0.0, m_function_switchX - bin_center)) * (scale_vector[6] + scale_vector[7]  * (bin_center - m_function_pivotX)));
            scale_PionProd = std::max(0.0, (1.0 - scale_vector[11] * std::max(0.0, m_function_switchX - bin_center)) * (scale_vector[9] + scale_vector[10] * (bin_center - m_function_pivotX)));
        }
        
        else if ( m_option_fit_function == "BilinearAlt" ) {
            scale_Signal   = std::max(0.0, (1.0 - scale_vector[2]  * std::max(0.0, scale_vector[3]  - bin_center)) * (scale_vector[0]  + scale_vector[1]  * (bin_center - m_function_pivotX)));
            scale_Pi0HighW = std::max(0.0, (1.0 - scale_vector[6]  * std::max(0.0, scale_vector[7]  - bin_center)) * (scale_vector[4]  + scale_vector[5]  * (bin_center - m_function_pivotX)));
            scale_QElike   = std::max(0.0, (1.0 - scale_vector[10] * std::max(0.0, scale_vector[11] - bin_center)) * (scale_vector[8]  + scale_vector[9]  * (bin_center - m_function_pivotX)));
            scale_PionProd = std::max(0.0, (1.0 - scale_vector[14] * std::max(0.0, scale_vector[15] - bin_center)) * (scale_vector[12] + scale_vector[13] * (bin_center - m_function_pivotX)));
        }
        
        // Chi2 component for signal region
        double comp_mc_SigReg_Signal   = h_mc_SigReg_Signal_temp   -> GetBinContent(bin);
        double comp_mc_SigReg_Pi0HighW = h_mc_SigReg_Pi0HighW_temp -> GetBinContent(bin);
        double comp_mc_SigReg_QElike   = h_mc_SigReg_QElike_temp   -> GetBinContent(bin);
        double comp_mc_SigReg_PionProd = h_mc_SigReg_PionProd_temp -> GetBinContent(bin);
        double comp_mc_SigReg_PlasUp   = h_mc_SigReg_PlasUp_temp   -> GetBinContent(bin);
        double comp_mc_SigReg_PlasBetw = h_mc_SigReg_PlasBetw_temp -> GetBinContent(bin);
        double comp_mc_SigReg_PlasDown = h_mc_SigReg_PlasDown_temp -> GetBinContent(bin);
        double comp_mc_SigReg_Other    = h_mc_SigReg_Other_temp    -> GetBinContent(bin);
        double comp_data_SigReg        = h_data_SigReg_temp        -> GetBinContent(bin);
        
        if ( comp_data_SigReg > 0.0 ) {
            chi2 += std::pow((scale_Signal   * comp_mc_SigReg_Signal)   +
                             (scale_Pi0HighW * comp_mc_SigReg_Pi0HighW) +
                             (scale_QElike   * comp_mc_SigReg_QElike)   +
                             (scale_PionProd * comp_mc_SigReg_PionProd) +
                             comp_mc_SigReg_PlasUp   +
                             comp_mc_SigReg_PlasBetw +
                             comp_mc_SigReg_PlasDown +
                             comp_mc_SigReg_Other    -
                             comp_data_SigReg, 2.0) / comp_data_SigReg;
        }
        
        // Chi2 component for pion-like shower sideband
        double comp_mc_PionBlobSB_Signal   = h_mc_PionBlobSB_Signal_temp   -> GetBinContent(bin);
        double comp_mc_PionBlobSB_Pi0HighW = h_mc_PionBlobSB_Pi0HighW_temp -> GetBinContent(bin);
        double comp_mc_PionBlobSB_QElike   = h_mc_PionBlobSB_QElike_temp   -> GetBinContent(bin);
        double comp_mc_PionBlobSB_PionProd = h_mc_PionBlobSB_PionProd_temp -> GetBinContent(bin);
        double comp_mc_PionBlobSB_PlasUp   = h_mc_PionBlobSB_PlasUp_temp   -> GetBinContent(bin);
        double comp_mc_PionBlobSB_PlasBetw = h_mc_PionBlobSB_PlasBetw_temp -> GetBinContent(bin);
        double comp_mc_PionBlobSB_PlasDown = h_mc_PionBlobSB_PlasDown_temp -> GetBinContent(bin);
        double comp_mc_PionBlobSB_Other    = h_mc_PionBlobSB_Other_temp    -> GetBinContent(bin);
        double comp_data_PionBlobSB        = h_data_PionBlobSB_temp        -> GetBinContent(bin);
        
        if ( comp_data_PionBlobSB > 0.0 ) {
            chi2 += std::pow((scale_Signal   * comp_mc_PionBlobSB_Signal)   +
                             (scale_Pi0HighW * comp_mc_PionBlobSB_Pi0HighW) +
                             (scale_QElike   * comp_mc_PionBlobSB_QElike)   +
                             (scale_PionProd * comp_mc_PionBlobSB_PionProd) +
                             comp_mc_PionBlobSB_PlasUp   +
                             comp_mc_PionBlobSB_PlasBetw +
                             comp_mc_PionBlobSB_PlasDown +
                             comp_mc_PionBlobSB_Other    -
                             comp_data_PionBlobSB, 2.0) / comp_data_PionBlobSB;
        }
        
        // Chi2 component for proton-like shower sideband
        double comp_mc_ProtonBlobSB_Signal   = h_mc_ProtonBlobSB_Signal_temp   -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_Pi0HighW = h_mc_ProtonBlobSB_Pi0HighW_temp -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_QElike   = h_mc_ProtonBlobSB_QElike_temp   -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_PionProd = h_mc_ProtonBlobSB_PionProd_temp -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_PlasUp   = h_mc_ProtonBlobSB_PlasUp_temp   -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_PlasBetw = h_mc_ProtonBlobSB_PlasBetw_temp -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_PlasDown = h_mc_ProtonBlobSB_PlasDown_temp -> GetBinContent(bin);
        double comp_mc_ProtonBlobSB_Other    = h_mc_ProtonBlobSB_Other_temp    -> GetBinContent(bin);
        double comp_data_ProtonBlobSB        = h_data_ProtonBlobSB_temp        -> GetBinContent(bin);
        
        if ( comp_data_ProtonBlobSB > 0.0 ) {
            chi2 += std::pow((scale_Signal   * comp_mc_ProtonBlobSB_Signal)   +
                             (scale_Pi0HighW * comp_mc_ProtonBlobSB_Pi0HighW) +
                             (scale_QElike   * comp_mc_ProtonBlobSB_QElike)   +
                             (scale_PionProd * comp_mc_ProtonBlobSB_PionProd) +
                             comp_mc_ProtonBlobSB_PlasUp   +
                             comp_mc_ProtonBlobSB_PlasBetw +
                             comp_mc_ProtonBlobSB_PlasDown +
                             comp_mc_ProtonBlobSB_Other    -
                             comp_data_ProtonBlobSB, 2.0) / comp_data_ProtonBlobSB;
        }
        
        // Chi2 component for high-W sideband
        double comp_mc_HighWSB_Signal   = h_mc_HighWSB_Signal_temp   -> GetBinContent(bin);
        double comp_mc_HighWSB_Pi0HighW = h_mc_HighWSB_Pi0HighW_temp -> GetBinContent(bin);
        double comp_mc_HighWSB_QElike   = h_mc_HighWSB_QElike_temp   -> GetBinContent(bin);
        double comp_mc_HighWSB_PionProd = h_mc_HighWSB_PionProd_temp -> GetBinContent(bin);
        double comp_mc_HighWSB_PlasUp   = h_mc_HighWSB_PlasUp_temp   -> GetBinContent(bin);
        double comp_mc_HighWSB_PlasBetw = h_mc_HighWSB_PlasBetw_temp -> GetBinContent(bin);
        double comp_mc_HighWSB_PlasDown = h_mc_HighWSB_PlasDown_temp -> GetBinContent(bin);
        double comp_mc_HighWSB_Other    = h_mc_HighWSB_Other_temp    -> GetBinContent(bin);
        double comp_data_HighWSB        = h_data_HighWSB_temp        -> GetBinContent(bin);
        
        if ( comp_data_HighWSB > 0.0 ) {
            chi2 += std::pow((scale_Signal   * comp_mc_HighWSB_Signal)   +
                             (scale_Pi0HighW * comp_mc_HighWSB_Pi0HighW) +
                             (scale_QElike   * comp_mc_HighWSB_QElike)   +
                             (scale_PionProd * comp_mc_HighWSB_PionProd) +
                             comp_mc_HighWSB_PlasUp   +
                             comp_mc_HighWSB_PlasBetw +
                             comp_mc_HighWSB_PlasDown +
                             comp_mc_HighWSB_Other    -
                             comp_data_HighWSB, 2.0) / comp_data_HighWSB;
        }
    }  // End of loop over bins
    
    // Release memory
    delete h_data_SigReg_temp;
    delete h_mc_SigReg_Signal_temp;
    delete h_mc_SigReg_Pi0HighW_temp;
    delete h_mc_SigReg_QElike_temp;
    delete h_mc_SigReg_PionProd_temp;
    delete h_mc_SigReg_PlasUp_temp;
    delete h_mc_SigReg_PlasBetw_temp;
    delete h_mc_SigReg_PlasDown_temp;
    delete h_mc_SigReg_Other_temp;
    
    delete h_data_PionBlobSB_temp;
    delete h_mc_PionBlobSB_Signal_temp;
    delete h_mc_PionBlobSB_Pi0HighW_temp;
    delete h_mc_PionBlobSB_QElike_temp;
    delete h_mc_PionBlobSB_PionProd_temp;
    delete h_mc_PionBlobSB_PlasUp_temp;
    delete h_mc_PionBlobSB_PlasBetw_temp;
    delete h_mc_PionBlobSB_PlasDown_temp;
    delete h_mc_PionBlobSB_Other_temp;
    
    delete h_data_ProtonBlobSB_temp;
    delete h_mc_ProtonBlobSB_Signal_temp;
    delete h_mc_ProtonBlobSB_Pi0HighW_temp;
    delete h_mc_ProtonBlobSB_QElike_temp;
    delete h_mc_ProtonBlobSB_PionProd_temp;
    delete h_mc_ProtonBlobSB_PlasUp_temp;
    delete h_mc_ProtonBlobSB_PlasBetw_temp;
    delete h_mc_ProtonBlobSB_PlasDown_temp;
    delete h_mc_ProtonBlobSB_Other_temp;
    
    delete h_data_HighWSB_temp;
    delete h_mc_HighWSB_Signal_temp;
    delete h_mc_HighWSB_Pi0HighW_temp;
    delete h_mc_HighWSB_QElike_temp;
    delete h_mc_HighWSB_PionProd_temp;
    delete h_mc_HighWSB_PlasUp_temp;
    delete h_mc_HighWSB_PlasBetw_temp;
    delete h_mc_HighWSB_PlasDown_temp;
    delete h_mc_HighWSB_Other_temp;
    
    // Return chi2
    return chi2;
}





// ==========================================================================
//  Combined fit function
// ==========================================================================

void PhysicsFitter::Fit_Combined()
{
    // Define parameters
    // =================
    
    // Initial parameters
    double start_a_Signal,   start_err_a_Signal,   lower_limit_a_Signal,   upper_limit_a_Signal;
    double start_a_Pi0HighW, start_err_a_Pi0HighW, lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW;
    double start_a_QElike,   start_err_a_QElike,   lower_limit_a_QElike,   upper_limit_a_QElike;
    double start_a_PionProd, start_err_a_PionProd, lower_limit_a_PionProd, upper_limit_a_PionProd;
    
    double start_m_Signal,   start_err_m_Signal,   lower_limit_m_Signal,   upper_limit_m_Signal;
    double start_m_Pi0HighW, start_err_m_Pi0HighW, lower_limit_m_Pi0HighW, upper_limit_m_Pi0HighW;
    double start_m_QElike,   start_err_m_QElike,   lower_limit_m_QElike,   upper_limit_m_QElike;
    double start_m_PionProd, start_err_m_PionProd, lower_limit_m_PionProd, upper_limit_m_PionProd;
    
    double start_T_Signal,   start_err_T_Signal,   lower_limit_T_Signal,   upper_limit_T_Signal;
    double start_T_Pi0HighW, start_err_T_Pi0HighW, lower_limit_T_Pi0HighW, upper_limit_T_Pi0HighW;
    double start_T_QElike,   start_err_T_QElike,   lower_limit_T_QElike,   upper_limit_T_QElike;
    double start_T_PionProd, start_err_T_PionProd, lower_limit_T_PionProd, upper_limit_T_PionProd;
    
    double start_b_Signal,   start_err_b_Signal,   lower_limit_b_Signal,   upper_limit_b_Signal;
    double start_b_Pi0HighW, start_err_b_Pi0HighW, lower_limit_b_Pi0HighW, upper_limit_b_Pi0HighW;
    double start_b_QElike,   start_err_b_QElike,   lower_limit_b_QElike,   upper_limit_b_QElike;
    double start_b_PionProd, start_err_b_PionProd, lower_limit_b_PionProd, upper_limit_b_PionProd;
    
    
    // Final parameters
    double final_a_Signal,   final_err_a_Signal;
    double final_a_Pi0HighW, final_err_a_Pi0HighW;
    double final_a_QElike,   final_err_a_QElike;
    double final_a_PionProd, final_err_a_PionProd;
    
    double final_m_Signal,   final_err_m_Signal;
    double final_m_Pi0HighW, final_err_m_Pi0HighW;
    double final_m_QElike,   final_err_m_QElike;
    double final_m_PionProd, final_err_m_PionProd;
    
    double final_T_Signal,   final_err_T_Signal;
    double final_T_Pi0HighW, final_err_T_Pi0HighW;
    double final_T_QElike,   final_err_T_QElike;
    double final_T_PionProd, final_err_T_PionProd;
    
    double final_b_Signal,   final_err_b_Signal;
    double final_b_Pi0HighW, final_err_b_Pi0HighW;
    double final_b_QElike,   final_err_b_QElike;
    double final_b_PionProd, final_err_b_PionProd;
    
    
    // Initialize parameters
    // =====================
    
    if ( m_model_type == kGENIE || m_model_type == kMnvGENIEv1 ||
         m_model_type == kMnvGENIEv1_noNonResPi || m_model_type == kMnvGENIEv1_noD2 || m_model_type == kMnvGENIEv1_noPionTune ||
         m_model_type == kMnvGENIEv2_MINOS || m_model_type == kMnvGENIEv2_JOINT || m_model_type == kMnvGENIEv2_NU1PI ||
         m_model_type == kMnvGENIEv2_NUNPI || m_model_type == kMnvGENIEv2_NUPI0 || m_model_type == kMnvGENIEv2_MENU1PI )
    {
        // For lead tune
        // =============
        if ( m_option_material == "lead" )
        {
            // Scalar fit
            if ( m_option_fit_function == "Scalar" ) {
                start_a_Signal   = 1.0, start_err_a_Signal   = 0.00000001, lower_limit_a_Signal   = 0.0, upper_limit_a_Signal   = 3.0;
                start_a_Pi0HighW = 1.0, start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0, upper_limit_a_Pi0HighW = 3.0;
                start_a_QElike   = 1.0, start_err_a_QElike   = 0.00000001, lower_limit_a_QElike   = 0.0, upper_limit_a_QElike   = 3.0;
                start_a_PionProd = 1.0, start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.0, upper_limit_a_PionProd = 3.0;
            }
            
            // Linear or linear alternative fit
            else if ( m_option_fit_function == "Linear" || m_option_fit_function == "LinearAlt" ) {
                start_a_Signal = 1.0,   start_err_a_Signal = 0.00000001, lower_limit_a_Signal = 0.0,  upper_limit_a_Signal = 3.0;
                start_m_Signal = 0.05,  start_err_m_Signal = 0.00000001, lower_limit_m_Signal = -5.0, upper_limit_m_Signal = 5.0;
                start_T_Signal = 0.550, start_err_T_Signal = 0.00000001, lower_limit_T_Signal = 0.0,  upper_limit_T_Signal = 2.0;
                
                start_a_Pi0HighW = 1.0,   start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0,  upper_limit_a_Pi0HighW = 3.0;
                start_m_Pi0HighW = 0.05,  start_err_m_Pi0HighW = 0.00000001, lower_limit_m_Pi0HighW = -5.0, upper_limit_m_Pi0HighW = 5.0;
                start_T_Pi0HighW = 0.550, start_err_T_Pi0HighW = 0.00000001, lower_limit_T_Pi0HighW = 0.0,  upper_limit_T_Pi0HighW = 2.0;
                
                start_a_QElike = 1.0,   start_err_a_QElike = 0.00000001, lower_limit_a_QElike = 0.0,  upper_limit_a_QElike = 3.0;
                start_m_QElike = 0.05,  start_err_m_QElike = 0.00000001, lower_limit_m_QElike = -5.0, upper_limit_m_QElike = 5.0;
                start_T_QElike = 0.550, start_err_T_QElike = 0.00000001, lower_limit_T_QElike = 0.0,  upper_limit_T_QElike = 2.0;
                
                start_a_PionProd = 1.0,   start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.5,  upper_limit_a_PionProd = 3.0;
                start_m_PionProd = 0.05,  start_err_m_PionProd = 0.00000001, lower_limit_m_PionProd = -5.0, upper_limit_m_PionProd = 5.0;
                start_T_PionProd = 0.550, start_err_T_PionProd = 0.00000001, lower_limit_T_PionProd = 0.0,  upper_limit_T_PionProd = 2.0;
            }
            
            // Bilinear fit
            else if ( m_option_fit_function == "Bilinear" || m_option_fit_function == "BilinearAlt" ) {
                start_a_Signal = 1.0,   start_err_a_Signal = 0.00000001, lower_limit_a_Signal = 0.0,   upper_limit_a_Signal = 3.0;
                start_m_Signal = 0.05,  start_err_m_Signal = 0.00000001, lower_limit_m_Signal = -5.0,  upper_limit_m_Signal = 5.0;
                start_b_Signal = 0.1,   start_err_b_Signal = 0.00000001, lower_limit_b_Signal = -50.0, upper_limit_b_Signal = 50.0;
                start_T_Signal = 0.150, start_err_T_Signal = 0.00000001, lower_limit_T_Signal = 0.0,   upper_limit_T_Signal = m_function_switchX;
                
                start_a_Pi0HighW = 1.0,   start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0,   upper_limit_a_Pi0HighW = 3.0;
                start_m_Pi0HighW = 0.05,  start_err_m_Pi0HighW = 0.00000001, lower_limit_m_Pi0HighW = -5.0,  upper_limit_m_Pi0HighW = 5.0;
                start_b_Pi0HighW = 0.1,   start_err_b_Pi0HighW = 0.00000001, lower_limit_b_Pi0HighW = -50.0, upper_limit_b_Pi0HighW = 50.0;
                start_T_Pi0HighW = 0.150, start_err_T_Pi0HighW = 0.00000001, lower_limit_T_Pi0HighW = 0.0,   upper_limit_T_Pi0HighW = m_function_switchX;
                
                start_a_QElike = 1.0,   start_err_a_QElike = 0.00000001, lower_limit_a_QElike = 0.0,   upper_limit_a_QElike = 3.0;
                start_m_QElike = 0.05,  start_err_m_QElike = 0.00000001, lower_limit_m_QElike = -5.0,  upper_limit_m_QElike = 5.0;
                start_b_QElike = 0.1,   start_err_b_QElike = 0.00000001, lower_limit_b_QElike = -50.0, upper_limit_b_QElike = 50.0;
                start_T_QElike = 0.150, start_err_T_QElike = 0.00000001, lower_limit_T_QElike = 0.0,   upper_limit_T_QElike = m_function_switchX;
                
                start_a_PionProd = 1.0,   start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.0,   upper_limit_a_PionProd = 3.0;
                start_m_PionProd = 0.05,  start_err_m_PionProd = 0.00000001, lower_limit_m_PionProd = -5.0,  upper_limit_m_PionProd = 5.0;
                start_b_PionProd = 0.1,   start_err_b_PionProd = 0.00000001, lower_limit_b_PionProd = -50.0, upper_limit_b_PionProd = 50.0;
                start_T_PionProd = 0.150, start_err_T_PionProd = 0.00000001, lower_limit_T_PionProd = 0.0,   upper_limit_T_PionProd = m_function_switchX;
            }
        }
        
        
        // For iron tune
        // =============
        else if ( m_option_material == "iron" )
        {
            // Scalar fit
            if ( m_option_fit_function == "Scalar" ) {
                start_a_Signal   = 1.0, start_err_a_Signal   = 0.00000001, lower_limit_a_Signal   = 0.0, upper_limit_a_Signal   = 3.0;
                start_a_Pi0HighW = 1.0, start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0, upper_limit_a_Pi0HighW = 3.0;
                start_a_QElike   = 1.0, start_err_a_QElike   = 0.00000001, lower_limit_a_QElike   = 0.0, upper_limit_a_QElike   = 3.0;
                start_a_PionProd = 1.0, start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.0, upper_limit_a_PionProd = 3.0;
            }
            
            // Linear or linear alternative fit
            else if ( m_option_fit_function == "Linear" || m_option_fit_function == "LinearAlt" ) {
                start_a_Signal = 1.0,   start_err_a_Signal = 0.00000001, lower_limit_a_Signal = 0.0,  upper_limit_a_Signal = 3.0;
                start_m_Signal = 0.05,  start_err_m_Signal = 0.00000001, lower_limit_m_Signal = -5.0, upper_limit_m_Signal = 5.0;
                start_T_Signal = 0.550, start_err_T_Signal = 0.00000001, lower_limit_T_Signal = 0.0,  upper_limit_T_Signal = 2.0;
                
                start_a_Pi0HighW = 1.0,   start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0,  upper_limit_a_Pi0HighW = 3.0;
                start_m_Pi0HighW = 0.05,  start_err_m_Pi0HighW = 0.00000001, lower_limit_m_Pi0HighW = -5.0, upper_limit_m_Pi0HighW = 5.0;
                start_T_Pi0HighW = 0.550, start_err_T_Pi0HighW = 0.00000001, lower_limit_T_Pi0HighW = 0.0,  upper_limit_T_Pi0HighW = 2.0;
                
                start_a_QElike = 1.0,   start_err_a_QElike = 0.00000001, lower_limit_a_QElike = 0.0,  upper_limit_a_QElike = 3.0;
                start_m_QElike = 0.05,  start_err_m_QElike = 0.00000001, lower_limit_m_QElike = -5.0, upper_limit_m_QElike = 5.0;
                start_T_QElike = 0.550, start_err_T_QElike = 0.00000001, lower_limit_T_QElike = 0.0,  upper_limit_T_QElike = 2.0;
                
                start_a_PionProd = 1.0,   start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.0,  upper_limit_a_PionProd = 3.0;
                start_m_PionProd = 0.05,  start_err_m_PionProd = 0.00000001, lower_limit_m_PionProd = -5.0, upper_limit_m_PionProd = 5.0;
                start_T_PionProd = 0.550, start_err_T_PionProd = 0.00000001, lower_limit_T_PionProd = 0.0,  upper_limit_T_PionProd = 2.0;
            }
            
            // Bilinear fit
            else if ( m_option_fit_function == "Bilinear" || m_option_fit_function == "BilinearAlt" ) {
                start_a_Signal = 1.0,   start_err_a_Signal = 0.00000001, lower_limit_a_Signal = 0.0,   upper_limit_a_Signal = 3.0;
                start_m_Signal = 0.05,  start_err_m_Signal = 0.00000001, lower_limit_m_Signal = -5.0,  upper_limit_m_Signal = 5.0;
                start_b_Signal = 0.1,   start_err_b_Signal = 0.00000001, lower_limit_b_Signal = -50.0, upper_limit_b_Signal = 50.0;
                start_T_Signal = 0.150, start_err_T_Signal = 0.00000001, lower_limit_T_Signal = 0.0,   upper_limit_T_Signal = m_function_switchX;
                
                start_a_Pi0HighW = 1.0,   start_err_a_Pi0HighW = 0.00000001, lower_limit_a_Pi0HighW = 0.0,   upper_limit_a_Pi0HighW = 3.0;
                start_m_Pi0HighW = 0.05,  start_err_m_Pi0HighW = 0.00000001, lower_limit_m_Pi0HighW = -5.0,  upper_limit_m_Pi0HighW = 5.0;
                start_b_Pi0HighW = 0.1,   start_err_b_Pi0HighW = 0.00000001, lower_limit_b_Pi0HighW = -50.0, upper_limit_b_Pi0HighW = 50.0;
                start_T_Pi0HighW = 0.150, start_err_T_Pi0HighW = 0.00000001, lower_limit_T_Pi0HighW = 0.0,   upper_limit_T_Pi0HighW = m_function_switchX;
                
                start_a_QElike = 1.0,   start_err_a_QElike = 0.00000001, lower_limit_a_QElike = 0.0,   upper_limit_a_QElike = 3.0;
                start_m_QElike = 0.05,  start_err_m_QElike = 0.00000001, lower_limit_m_QElike = -5.0,  upper_limit_m_QElike = 5.0;
                start_b_QElike = 0.1,   start_err_b_QElike = 0.00000001, lower_limit_b_QElike = -50.0, upper_limit_b_QElike = 50.0;
                start_T_QElike = 0.150, start_err_T_QElike = 0.00000001, lower_limit_T_QElike = 0.0,   upper_limit_T_QElike = m_function_switchX;
                
                start_a_PionProd = 1.0,   start_err_a_PionProd = 0.00000001, lower_limit_a_PionProd = 0.0,   upper_limit_a_PionProd = 3.0;
                start_m_PionProd = 0.05,  start_err_m_PionProd = 0.00000001, lower_limit_m_PionProd = -5.0,  upper_limit_m_PionProd = 5.0;
                start_b_PionProd = 0.1,   start_err_b_PionProd = 0.00000001, lower_limit_b_PionProd = -50.0, upper_limit_b_PionProd = 50.0;
                start_T_PionProd = 0.150, start_err_T_PionProd = 0.00000001, lower_limit_T_PionProd = 0.0,   upper_limit_T_PionProd = m_function_switchX;
            }
        }
    }
    
    
    // Perform minimization
    // ====================
    
    // Get number of parameters to minimize
    int Nparameters;
    if ( m_option_fit_function == "Scalar" )           Nparameters = 4;
    else if ( m_option_fit_function == "Linear" )      Nparameters = 8;
    else if ( m_option_fit_function == "LinearAlt" )   Nparameters = 12;
    else if ( m_option_fit_function == "Bilinear" )    Nparameters = 12;
    else if ( m_option_fit_function == "BilinearAlt" ) Nparameters = 16;
    
    
    // Define minimizer
    ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2");
    ROOT::Math::Functor functor(&Chi2Function_Combined, Nparameters);
    
    minimizer -> SetFunction(functor);
    minimizer -> SetMaxFunctionCalls(30000);
    minimizer -> SetMaxIterations(30000);
    minimizer -> SetTolerance(0.01);
    minimizer -> SetPrintLevel(2);
    
    
    // Set minimizer parameters
    if ( m_option_fit_function == "Scalar" ) {
        minimizer -> SetVariable(0, "Par_a_Signal",   start_a_Signal,   0.00000001);
        minimizer -> SetVariable(1, "Par_a_Pi0HighW", start_a_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(2, "Par_a_QElike",   start_a_QElike,   0.00000001);
        minimizer -> SetVariable(3, "Par_a_PionProd", start_a_PionProd, 0.00000001);
        
        minimizer -> SetVariableLimits(0, lower_limit_a_Signal,   upper_limit_a_Signal);
        minimizer -> SetVariableLimits(1, lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW);
        minimizer -> SetVariableLimits(2, lower_limit_a_QElike,   upper_limit_a_QElike);
        minimizer -> SetVariableLimits(3, lower_limit_a_PionProd, upper_limit_a_PionProd);
    }
    
    else if ( m_option_fit_function == "Linear" ) {
        minimizer -> SetVariable(0, "Par_a_Signal",   start_a_Signal,   0.00000001);
        minimizer -> SetVariable(1, "Par_m_Signal",   start_m_Signal,   0.00000001);
        minimizer -> SetVariable(2, "Par_a_Pi0HighW", start_a_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(3, "Par_m_Pi0HighW", start_m_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(4, "Par_a_QElike",   start_a_QElike,   0.00000001);
        minimizer -> SetVariable(5, "Par_m_QElike",   start_m_QElike,   0.00000001);
        minimizer -> SetVariable(6, "Par_a_PionProd", start_a_PionProd, 0.00000001);
        minimizer -> SetVariable(7, "Par_m_PionProd", start_m_PionProd, 0.00000001);
        
        minimizer -> SetVariableLimits(0, lower_limit_a_Signal,   upper_limit_a_Signal);
        minimizer -> SetVariableLimits(1, lower_limit_m_Signal,   upper_limit_m_Signal);
        minimizer -> SetVariableLimits(2, lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW);
        minimizer -> SetVariableLimits(3, lower_limit_m_Pi0HighW, upper_limit_m_Pi0HighW);
        minimizer -> SetVariableLimits(4, lower_limit_a_QElike,   upper_limit_a_QElike);
        minimizer -> SetVariableLimits(5, lower_limit_m_QElike,   upper_limit_m_QElike);
        minimizer -> SetVariableLimits(6, lower_limit_a_PionProd, upper_limit_a_PionProd);
        minimizer -> SetVariableLimits(7, lower_limit_m_PionProd, upper_limit_m_PionProd);
    }
    
    else if ( m_option_fit_function == "LinearAlt" ) {
        minimizer -> SetVariable(0,  "Par_a_Signal",   start_a_Signal,   0.00000001);
        minimizer -> SetVariable(1,  "Par_m_Signal",   start_m_Signal,   0.00000001);
        minimizer -> SetVariable(2,  "Par_T_Signal",   start_T_Signal,   0.00000001);
        minimizer -> SetVariable(3,  "Par_a_Pi0HighW", start_a_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(4,  "Par_m_Pi0HighW", start_m_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(5,  "Par_T_Pi0HighW", start_T_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(6,  "Par_a_QElike",   start_a_QElike,   0.00000001);
        minimizer -> SetVariable(7,  "Par_m_QElike",   start_m_QElike,   0.00000001);
        minimizer -> SetVariable(8,  "Par_T_QElike",   start_T_QElike,   0.00000001);
        minimizer -> SetVariable(9,  "Par_a_PionProd", start_a_PionProd, 0.00000001);
        minimizer -> SetVariable(10, "Par_m_PionProd", start_m_PionProd, 0.00000001);
        minimizer -> SetVariable(11, "Par_T_PionProd", start_T_PionProd, 0.00000001);
        
        minimizer -> SetVariableLimits(0,  lower_limit_a_Signal,   upper_limit_a_Signal);
        minimizer -> SetVariableLimits(1,  lower_limit_m_Signal,   upper_limit_m_Signal);
        minimizer -> SetVariableLimits(2,  lower_limit_T_Signal,   upper_limit_T_Signal);
        minimizer -> SetVariableLimits(3,  lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW);
        minimizer -> SetVariableLimits(4,  lower_limit_m_Pi0HighW, upper_limit_m_Pi0HighW);
        minimizer -> SetVariableLimits(5,  lower_limit_T_Pi0HighW, upper_limit_T_Pi0HighW);
        minimizer -> SetVariableLimits(6,  lower_limit_a_QElike,   upper_limit_a_QElike);
        minimizer -> SetVariableLimits(7,  lower_limit_m_QElike,   upper_limit_m_QElike);
        minimizer -> SetVariableLimits(8,  lower_limit_T_QElike,   upper_limit_T_QElike);
        minimizer -> SetVariableLimits(9,  lower_limit_a_PionProd, upper_limit_a_PionProd);
        minimizer -> SetVariableLimits(10, lower_limit_m_PionProd, upper_limit_m_PionProd);
        minimizer -> SetVariableLimits(11, lower_limit_T_PionProd, upper_limit_T_PionProd);
    }
    
    else if ( m_option_fit_function == "Bilinear" ) {
        minimizer -> SetVariable(0,  "Par_a_Signal",   start_a_Signal,   0.00000001);
        minimizer -> SetVariable(1,  "Par_m_Signal",   start_m_Signal,   0.00000001);
        minimizer -> SetVariable(2,  "Par_b_Signal",   start_b_Signal,   0.00000001);
        minimizer -> SetVariable(3,  "Par_a_Pi0HighW", start_a_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(4,  "Par_m_Pi0HighW", start_m_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(5,  "Par_b_Pi0HighW", start_b_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(6,  "Par_a_QElike",   start_a_QElike,   0.00000001);
        minimizer -> SetVariable(7,  "Par_m_QElike",   start_m_QElike,   0.00000001);
        minimizer -> SetVariable(8,  "Par_b_QElike",   start_b_QElike,   0.00000001);
        minimizer -> SetVariable(9,  "Par_a_PionProd", start_a_PionProd, 0.00000001);
        minimizer -> SetVariable(10, "Par_m_PionProd", start_m_PionProd, 0.00000001);
        minimizer -> SetVariable(11, "Par_b_PionProd", start_b_PionProd, 0.00000001);
        
        minimizer -> SetVariableLimits(0,  lower_limit_a_Signal,   upper_limit_a_Signal);
        minimizer -> SetVariableLimits(1,  lower_limit_m_Signal,   upper_limit_m_Signal);
        minimizer -> SetVariableLimits(2,  lower_limit_b_Signal,   upper_limit_b_Signal);
        minimizer -> SetVariableLimits(3,  lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW);
        minimizer -> SetVariableLimits(4,  lower_limit_m_Pi0HighW, upper_limit_m_Pi0HighW);
        minimizer -> SetVariableLimits(5,  lower_limit_b_Pi0HighW, upper_limit_b_Pi0HighW);
        minimizer -> SetVariableLimits(6,  lower_limit_a_QElike,   upper_limit_a_QElike);
        minimizer -> SetVariableLimits(7,  lower_limit_m_QElike,   upper_limit_m_QElike);
        minimizer -> SetVariableLimits(8,  lower_limit_b_QElike,   upper_limit_b_QElike);
        minimizer -> SetVariableLimits(9,  lower_limit_a_PionProd, upper_limit_a_PionProd);
        minimizer -> SetVariableLimits(10, lower_limit_m_PionProd, upper_limit_m_PionProd);
        minimizer -> SetVariableLimits(11, lower_limit_b_PionProd, upper_limit_b_PionProd);
    }
    
    else if ( m_option_fit_function == "BilinearAlt" ) {
        minimizer -> SetVariable(0,  "Par_a_Signal",   start_a_Signal,   0.00000001);
        minimizer -> SetVariable(1,  "Par_m_Signal",   start_m_Signal,   0.00000001);
        minimizer -> SetVariable(2,  "Par_b_Signal",   start_b_Signal,   0.00000001);
        minimizer -> SetVariable(3,  "Par_T_Signal",   start_T_Signal,   0.00000001);
        minimizer -> SetVariable(4,  "Par_a_Pi0HighW", start_a_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(5,  "Par_m_Pi0HighW", start_m_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(6,  "Par_b_Pi0HighW", start_b_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(7,  "Par_T_Pi0HighW", start_T_Pi0HighW, 0.00000001);
        minimizer -> SetVariable(8,  "Par_a_QElike",   start_a_QElike,   0.00000001);
        minimizer -> SetVariable(9,  "Par_m_QElike",   start_m_QElike,   0.00000001);
        minimizer -> SetVariable(10, "Par_b_QElike",   start_b_QElike,   0.00000001);
        minimizer -> SetVariable(11, "Par_T_QElike",   start_T_QElike,   0.00000001);
        minimizer -> SetVariable(12, "Par_a_PionProd", start_a_PionProd, 0.00000001);
        minimizer -> SetVariable(13, "Par_m_PionProd", start_m_PionProd, 0.00000001);
        minimizer -> SetVariable(14, "Par_b_PionProd", start_b_PionProd, 0.00000001);
        minimizer -> SetVariable(15, "Par_T_PionProd", start_T_PionProd, 0.00000001);
        
        minimizer -> SetVariableLimits(0,  lower_limit_a_Signal,   upper_limit_a_Signal);
        minimizer -> SetVariableLimits(1,  lower_limit_m_Signal,   upper_limit_m_Signal);
        minimizer -> SetVariableLimits(2,  lower_limit_b_Signal,   upper_limit_b_Signal);
        minimizer -> SetVariableLimits(3,  lower_limit_T_Signal,   upper_limit_T_Signal);
        minimizer -> SetVariableLimits(4,  lower_limit_a_Pi0HighW, upper_limit_a_Pi0HighW);
        minimizer -> SetVariableLimits(5,  lower_limit_m_Pi0HighW, upper_limit_m_Pi0HighW);
        minimizer -> SetVariableLimits(6,  lower_limit_b_Pi0HighW, upper_limit_b_Pi0HighW);
        minimizer -> SetVariableLimits(7,  lower_limit_T_Pi0HighW, upper_limit_T_Pi0HighW);
        minimizer -> SetVariableLimits(8,  lower_limit_a_QElike,   upper_limit_a_QElike);
        minimizer -> SetVariableLimits(9,  lower_limit_m_QElike,   upper_limit_m_QElike);
        minimizer -> SetVariableLimits(10, lower_limit_b_QElike,   upper_limit_b_QElike);
        minimizer -> SetVariableLimits(11, lower_limit_T_QElike,   upper_limit_T_QElike);
        minimizer -> SetVariableLimits(12, lower_limit_a_PionProd, upper_limit_a_PionProd);
        minimizer -> SetVariableLimits(13, lower_limit_m_PionProd, upper_limit_m_PionProd);
        minimizer -> SetVariableLimits(14, lower_limit_b_PionProd, upper_limit_b_PionProd);
        minimizer -> SetVariableLimits(15, lower_limit_T_PionProd, upper_limit_T_PionProd);
    }
    
    
    // Run minimizer
    minimizer -> Minimize();
    
    
    // Retrieve parameters
    if ( m_option_fit_function == "Scalar" ) {
        final_a_Signal     = minimizer->X()[0];
        final_err_a_Signal = minimizer->Errors()[0];
        
        final_a_Pi0HighW     = minimizer->X()[1];
        final_err_m_Pi0HighW = minimizer->Errors()[1];
        
        final_a_QElike     = minimizer->X()[2];
        final_err_m_QElike = minimizer->Errors()[2];
        
        final_a_PionProd     = minimizer->X()[3];
        final_err_m_PionProd = minimizer->Errors()[3];
    }
    
    else if ( m_option_fit_function == "Linear" ) {
        final_a_Signal     = minimizer->X()[0];
        final_m_Signal     = minimizer->X()[1];
        final_err_a_Signal = minimizer->Errors()[0];
        final_err_m_Signal = minimizer->Errors()[1];
        
        final_a_Pi0HighW     = minimizer->X()[2];
        final_m_Pi0HighW     = minimizer->X()[3];
        final_err_a_Pi0HighW = minimizer->Errors()[2];
        final_err_m_Pi0HighW = minimizer->Errors()[3];
        
        final_a_QElike     = minimizer->X()[4];
        final_m_QElike     = minimizer->X()[5];
        final_err_a_QElike = minimizer->Errors()[4];
        final_err_m_QElike = minimizer->Errors()[5];
        
        final_a_PionProd     = minimizer->X()[6];
        final_m_PionProd     = minimizer->X()[7];
        final_err_a_PionProd = minimizer->Errors()[6];
        final_err_m_PionProd = minimizer->Errors()[7];
    }
    
    else if ( m_option_fit_function == "LinearAlt" ) {
        final_a_Signal     = minimizer->X()[0];
        final_m_Signal     = minimizer->X()[1];
        final_T_Signal     = minimizer->X()[2];
        final_err_a_Signal = minimizer->Errors()[0];
        final_err_m_Signal = minimizer->Errors()[1];
        final_err_T_Signal = minimizer->Errors()[2];
        
        final_a_Pi0HighW     = minimizer->X()[3];
        final_m_Pi0HighW     = minimizer->X()[4];
        final_T_Pi0HighW     = minimizer->X()[5];
        final_err_a_Pi0HighW = minimizer->Errors()[3];
        final_err_m_Pi0HighW = minimizer->Errors()[4];
        final_err_T_Pi0HighW = minimizer->Errors()[5];
        
        final_a_QElike     = minimizer->X()[6];
        final_m_QElike     = minimizer->X()[7];
        final_T_QElike     = minimizer->X()[8];
        final_err_a_QElike = minimizer->Errors()[6];
        final_err_m_QElike = minimizer->Errors()[7];
        final_err_T_QElike = minimizer->Errors()[8];
        
        final_a_PionProd     = minimizer->X()[9];
        final_m_PionProd     = minimizer->X()[10];
        final_T_PionProd     = minimizer->X()[11];
        final_err_a_PionProd = minimizer->Errors()[9];
        final_err_m_PionProd = minimizer->Errors()[10];
        final_err_T_PionProd = minimizer->Errors()[11];
    }
    
    else if ( m_option_fit_function == "Bilinear" ) {
        final_a_Signal     = minimizer->X()[0];
        final_m_Signal     = minimizer->X()[1];
        final_b_Signal     = minimizer->X()[2];
        final_err_a_Signal = minimizer->Errors()[0];
        final_err_m_Signal = minimizer->Errors()[1];
        final_err_b_Signal = minimizer->Errors()[2];
        
        final_a_Pi0HighW     = minimizer->X()[3];
        final_m_Pi0HighW     = minimizer->X()[4];
        final_b_Pi0HighW     = minimizer->X()[5];
        final_err_a_Pi0HighW = minimizer->Errors()[3];
        final_err_m_Pi0HighW = minimizer->Errors()[4];
        final_err_b_Pi0HighW = minimizer->Errors()[5];
        
        final_a_QElike     = minimizer->X()[6];
        final_m_QElike     = minimizer->X()[7];
        final_b_QElike     = minimizer->X()[8];
        final_err_a_QElike = minimizer->Errors()[6];
        final_err_m_QElike = minimizer->Errors()[7];
        final_err_b_QElike = minimizer->Errors()[8];
        
        final_a_PionProd     = minimizer->X()[9];
        final_m_PionProd     = minimizer->X()[10];
        final_b_PionProd     = minimizer->X()[11];
        final_err_a_PionProd = minimizer->Errors()[9];
        final_err_m_PionProd = minimizer->Errors()[10];
        final_err_b_PionProd = minimizer->Errors()[11];
    }
    
    else if ( m_option_fit_function == "BilinearAlt" ) {
        final_a_Signal     = minimizer->X()[0];
        final_m_Signal     = minimizer->X()[1];
        final_b_Signal     = minimizer->X()[2];
        final_T_Signal     = minimizer->X()[3];
        final_err_a_Signal = minimizer->Errors()[0];
        final_err_m_Signal = minimizer->Errors()[1];
        final_err_b_Signal = minimizer->Errors()[2];
        final_err_T_Signal = minimizer->Errors()[3];
        
        final_a_Pi0HighW     = minimizer->X()[4];
        final_m_Pi0HighW     = minimizer->X()[5];
        final_b_Pi0HighW     = minimizer->X()[6];
        final_T_Pi0HighW     = minimizer->X()[7];
        final_err_a_Pi0HighW = minimizer->Errors()[4];
        final_err_m_Pi0HighW = minimizer->Errors()[5];
        final_err_b_Pi0HighW = minimizer->Errors()[6];
        final_err_T_Pi0HighW = minimizer->Errors()[7];
        
        final_a_QElike     = minimizer->X()[8];
        final_m_QElike     = minimizer->X()[9];
        final_b_QElike     = minimizer->X()[10];
        final_T_QElike     = minimizer->X()[11];
        final_err_a_QElike = minimizer->Errors()[8];
        final_err_m_QElike = minimizer->Errors()[9];
        final_err_b_QElike = minimizer->Errors()[10];
        final_err_T_QElike = minimizer->Errors()[11];
        
        final_a_PionProd     = minimizer->X()[12];
        final_m_PionProd     = minimizer->X()[13];
        final_b_PionProd     = minimizer->X()[14];
        final_T_PionProd     = minimizer->X()[15];
        final_err_a_PionProd = minimizer->Errors()[12];
        final_err_m_PionProd = minimizer->Errors()[13];
        final_err_b_PionProd = minimizer->Errors()[14];
        final_err_T_PionProd = minimizer->Errors()[15];
    }
    
    
    // Fill vector of fit final parameters
    m_Par_a_Signal.push_back(final_a_Signal);
    m_Par_m_Signal.push_back(final_m_Signal);
    m_Par_b_Signal.push_back(final_b_Signal);
    m_Par_T_Signal.push_back(final_T_Signal);
    
    m_Par_a_Pi0HighW.push_back(final_a_Pi0HighW);
    m_Par_m_Pi0HighW.push_back(final_m_Pi0HighW);
    m_Par_b_Pi0HighW.push_back(final_b_Pi0HighW);
    m_Par_T_Pi0HighW.push_back(final_T_Pi0HighW);
    
    m_Par_a_QElike.push_back(final_a_QElike);
    m_Par_m_QElike.push_back(final_m_QElike);
    m_Par_b_QElike.push_back(final_b_QElike);
    m_Par_T_QElike.push_back(final_T_QElike);
    
    m_Par_a_PionProd.push_back(final_a_PionProd);
    m_Par_m_PionProd.push_back(final_m_PionProd);
    m_Par_b_PionProd.push_back(final_b_PionProd);
    m_Par_T_PionProd.push_back(final_T_PionProd);
    
    
    // Release memory
    delete minimizer;
    
    
    // Get scale functions
    // ===================
    
    // Scalar fit
    if ( m_option_fit_function == "Scalar" ) {
        TF1* function_Signal = new TF1("function_Signal", "max(0.0, [0])", m_Xmin, m_Xmax);
        function_Signal -> SetParameter(0, final_a_Signal);
        function_Signal -> SetParError(0, final_err_a_Signal);
        m_function_Signal = function_Signal;
        
        TF1* function_Pi0HighW = new TF1("function_Pi0HighW", "max(0.0, [0])", m_Xmin, m_Xmax);
        function_Pi0HighW -> SetParameter(0, final_a_Pi0HighW);
        function_Pi0HighW -> SetParError(0, final_err_a_Pi0HighW);
        m_function_BackgrPi0HighW = function_Pi0HighW;
        
        TF1* function_QElike = new TF1("function_QElike", "max(0.0, [0])", m_Xmin, m_Xmax);
        function_QElike -> SetParameter(0, final_a_QElike);
        function_QElike -> SetParError(0, final_err_a_QElike);
        m_function_BackgrQElike = function_QElike;
        
        TF1* function_PionProd = new TF1("function_PionProd", "max(0.0, [0])", m_Xmin, m_Xmax);
        function_PionProd -> SetParameter(0, final_a_PionProd);
        function_PionProd -> SetParError(0, final_err_a_PionProd);
        m_function_BackgrPionProd = function_PionProd;
    }
    
    // Linear fit
    else if ( m_option_fit_function == "Linear" ) {
        TF1* function_Signal = new TF1("function_Signal", "max(0.0, [0] + [1] * (x - [2]]))", m_Xmin, m_Xmax);
        function_Signal -> SetParameter(0, final_a_Signal);
        function_Signal -> SetParameter(1, final_m_Signal);
        function_Signal -> SetParameter(2, m_function_pivotX);
        function_Signal -> SetParError(0, final_err_a_Signal);
        function_Signal -> SetParError(1, final_err_m_Signal);
        m_function_Signal = function_Signal;
        
        TF1* function_Pi0HighW = new TF1("function_Pi0HighW", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_Pi0HighW -> SetParameter(0, final_a_Pi0HighW);
        function_Pi0HighW -> SetParameter(1, final_m_Pi0HighW);
        function_Pi0HighW -> SetParameter(2, m_function_pivotX);
        function_Pi0HighW -> SetParError(0, final_err_a_Pi0HighW);
        function_Pi0HighW -> SetParError(1, final_err_m_Pi0HighW);
        m_function_BackgrPi0HighW = function_Pi0HighW;
        
        TF1* function_QElike = new TF1("function_QElike", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_QElike -> SetParameter(0, final_a_QElike);
        function_QElike -> SetParameter(1, final_m_QElike);
        function_QElike -> SetParameter(2, m_function_pivotX);
        function_QElike -> SetParError(0, final_err_a_QElike);
        function_QElike -> SetParError(1, final_err_m_QElike);
        m_function_BackgrQElike = function_QElike;
        
        TF1* function_PionProd = new TF1("function_PionProd", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_PionProd -> SetParameter(0, final_a_PionProd);
        function_PionProd -> SetParameter(1, final_m_PionProd);
        function_PionProd -> SetParameter(2, m_function_pivotX);
        function_PionProd -> SetParError(0, final_err_a_PionProd);
        function_PionProd -> SetParError(1, final_err_m_PionProd);
        m_function_BackgrPionProd = function_PionProd;
    }
    
    // Linear alternative fit
    else if ( m_option_fit_function == "LinearAlt" ) {
        TF1* function_Signal = new TF1("function_Signal", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_Signal -> SetParameter(0, final_a_Signal);
        function_Signal -> SetParameter(1, final_m_Signal);
        function_Signal -> SetParameter(2, final_T_Signal);
        function_Signal -> SetParError(0, final_err_a_Signal);
        function_Signal -> SetParError(1, final_err_m_Signal);
        function_Signal -> SetParError(2, final_err_T_Signal);
        m_function_Signal = function_Signal;
        
        TF1* function_Pi0HighW = new TF1("function_Pi0HighW", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_Pi0HighW -> SetParameter(0, final_a_Pi0HighW);
        function_Pi0HighW -> SetParameter(1, final_m_Pi0HighW);
        function_Pi0HighW -> SetParameter(2, final_T_Pi0HighW);
        function_Pi0HighW -> SetParError(0, final_err_a_Pi0HighW);
        function_Pi0HighW -> SetParError(1, final_err_m_Pi0HighW);
        function_Pi0HighW -> SetParError(2, final_err_T_Pi0HighW);
        m_function_BackgrPi0HighW = function_Pi0HighW;
        
        TF1* function_QElike = new TF1("function_QElike", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_QElike -> SetParameter(0, final_a_QElike);
        function_QElike -> SetParameter(1, final_m_QElike);
        function_QElike -> SetParameter(2, final_T_QElike);
        function_QElike -> SetParError(0, final_err_a_QElike);
        function_QElike -> SetParError(1, final_err_m_QElike);
        function_QElike -> SetParError(2, final_err_T_QElike);
        m_function_BackgrQElike = function_QElike;
        
        TF1* function_PionProd = new TF1("function_PionProd", "max(0.0, [0] + [1] * (x - [2]))", m_Xmin, m_Xmax);
        function_PionProd -> SetParameter(0, final_a_PionProd);
        function_PionProd -> SetParameter(1, final_m_PionProd);
        function_PionProd -> SetParameter(2, final_T_PionProd);
        function_PionProd -> SetParError(0, final_err_a_PionProd);
        function_PionProd -> SetParError(1, final_err_m_PionProd);
        function_PionProd -> SetParError(2, final_err_T_PionProd);
        m_function_BackgrPionProd = function_PionProd;
    }
    
    // Bilinear fit
    else if ( m_option_fit_function == "Bilinear" ) {
        TF1* function_Signal = new TF1("function_Signal", "max(0.0, (1.0 - [2] * max(0.0, [4] - x)) * ([0] + [1] * (x - [3])))", m_Xmin, m_Xmax);
        function_Signal -> SetParameter(0, final_a_Signal);
        function_Signal -> SetParameter(1, final_m_Signal);
        function_Signal -> SetParameter(2, final_b_Signal);
        function_Signal -> SetParameter(3, m_function_pivotX);
        function_Signal -> SetParameter(4, m_function_switchX);
        function_Signal -> SetParError(0, final_err_a_Signal);
        function_Signal -> SetParError(1, final_err_m_Signal);
        function_Signal -> SetParError(2, final_err_b_Signal);
        m_function_Signal = function_Signal;
        
        TF1* function_Pi0HighW = new TF1("function_Pi0HighW", "max(0.0, (1.0 - [2] * max(0.0, [4] - x)) * ([0] + [1] * (x - [3])))", m_Xmin, m_Xmax);
        function_Pi0HighW -> SetParameter(0, final_a_Pi0HighW);
        function_Pi0HighW -> SetParameter(1, final_m_Pi0HighW);
        function_Pi0HighW -> SetParameter(2, final_b_Pi0HighW);
        function_Pi0HighW -> SetParameter(3, m_function_pivotX);
        function_Pi0HighW -> SetParameter(4, m_function_switchX);
        function_Pi0HighW -> SetParError(0, final_err_a_Pi0HighW);
        function_Pi0HighW -> SetParError(1, final_err_m_Pi0HighW);
        function_Pi0HighW -> SetParError(2, final_err_b_Pi0HighW);
        m_function_BackgrPi0HighW = function_Pi0HighW;
        
        TF1* function_QElike = new TF1("function_QElike", "max(0.0, (1.0 - [2] * max(0.0, [4] - x)) * ([0] + [1] * (x - [3])))", m_Xmin, m_Xmax);
        function_QElike -> SetParameter(0, final_a_QElike);
        function_QElike -> SetParameter(1, final_m_QElike);
        function_QElike -> SetParameter(2, final_b_QElike);
        function_QElike -> SetParameter(3, m_function_pivotX);
        function_QElike -> SetParameter(4, m_function_switchX);
        function_QElike -> SetParError(0, final_err_a_QElike);
        function_QElike -> SetParError(1, final_err_m_QElike);
        function_QElike -> SetParError(2, final_err_b_QElike);
        m_function_BackgrQElike = function_QElike;
        
        TF1* function_PionProd = new TF1("function_PionProd", "max(0.0, (1.0 - [2] * max(0.0, [4] - x)) * ([0] + [1] * (x - [3])))", m_Xmin, m_Xmax);
        function_PionProd -> SetParameter(0, final_a_PionProd);
        function_PionProd -> SetParameter(1, final_m_PionProd);
        function_PionProd -> SetParameter(2, final_b_PionProd);
        function_PionProd -> SetParameter(3, m_function_pivotX);
        function_PionProd -> SetParameter(4, m_function_switchX);
        function_PionProd -> SetParError(0, final_err_a_PionProd);
        function_PionProd -> SetParError(1, final_err_m_PionProd);
        function_PionProd -> SetParError(2, final_err_b_PionProd);
        m_function_BackgrPionProd = function_PionProd;
    }
    
    // Bilinear alternative fit
    else if ( m_option_fit_function == "BilinearAlt" ) {
        TF1* function_Signal = new TF1("function_Signal", "max(0.0, (1.0 - [2] * max(0.0, [3] - x)) * ([0] + [1] * (x - [4])))", m_Xmin, m_Xmax);
        function_Signal -> SetParameter(0, final_a_Signal);
        function_Signal -> SetParameter(1, final_m_Signal);
        function_Signal -> SetParameter(2, final_b_Signal);
        function_Signal -> SetParameter(3, final_T_Signal);
        function_Signal -> SetParameter(4, m_function_pivotX);
        function_Signal -> SetParError(0, final_err_a_Signal);
        function_Signal -> SetParError(1, final_err_m_Signal);
        function_Signal -> SetParError(2, final_err_b_Signal);
        function_Signal -> SetParError(3, final_err_T_Signal);
        m_function_Signal = function_Signal;
        
        TF1* function_Pi0HighW = new TF1("function_Pi0HighW", "max(0.0, (1.0 - [2] * max(0.0, [3] - x)) * ([0] + [1] * (x - [4])))", m_Xmin, m_Xmax);
        function_Pi0HighW -> SetParameter(0, final_a_Pi0HighW);
        function_Pi0HighW -> SetParameter(1, final_m_Pi0HighW);
        function_Pi0HighW -> SetParameter(2, final_b_Pi0HighW);
        function_Pi0HighW -> SetParameter(3, final_T_Pi0HighW);
        function_Pi0HighW -> SetParameter(4, m_function_pivotX);
        function_Pi0HighW -> SetParError(0, final_err_a_Pi0HighW);
        function_Pi0HighW -> SetParError(1, final_err_m_Pi0HighW);
        function_Pi0HighW -> SetParError(2, final_err_b_Pi0HighW);
        function_Pi0HighW -> SetParError(3, final_err_T_Pi0HighW);
        m_function_BackgrPi0HighW = function_Pi0HighW;
        
        TF1* function_QElike = new TF1("function_QElike", "max(0.0, (1.0 - [2] * max(0.0, [3] - x)) * ([0] + [1] * (x - [4])))", m_Xmin, m_Xmax);
        function_QElike -> SetParameter(0, final_a_QElike);
        function_QElike -> SetParameter(1, final_m_QElike);
        function_QElike -> SetParameter(2, final_b_QElike);
        function_QElike -> SetParameter(3, final_T_QElike);
        function_QElike -> SetParameter(4, m_function_pivotX);
        function_QElike -> SetParError(0, final_err_a_QElike);
        function_QElike -> SetParError(1, final_err_m_QElike);
        function_QElike -> SetParError(2, final_err_b_QElike);
        function_QElike -> SetParError(3, final_err_T_QElike);
        m_function_BackgrQElike = function_QElike;
        
        TF1* function_PionProd = new TF1("function_PionProd", "max(0.0, (1.0 - [2] * max(0.0, [3] - x)) * ([0] + [1] * (x - [4])))", m_Xmin, m_Xmax);
        function_PionProd -> SetParameter(0, final_a_PionProd);
        function_PionProd -> SetParameter(1, final_m_PionProd);
        function_PionProd -> SetParameter(2, final_b_PionProd);
        function_PionProd -> SetParameter(3, final_T_PionProd);
        function_PionProd -> SetParameter(4, m_function_pivotX);
        function_PionProd -> SetParError(0, final_err_a_PionProd);
        function_PionProd -> SetParError(1, final_err_m_PionProd);
        function_PionProd -> SetParError(2, final_err_b_PionProd);
        function_PionProd -> SetParError(3, final_err_T_PionProd);
        m_function_BackgrPionProd = function_PionProd;
    }
}


#endif  // PhysicsFitter_cxx