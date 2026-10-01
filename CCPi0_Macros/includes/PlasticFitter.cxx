#ifndef PlasticFitter_cxx
#define PlasticFitter_cxx

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "PlasticFitter.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TF1.h"



// ==========================================================================
//
// HOW IS FIT FUNCTION DEFINED:
// 
//  a + m (x - pivotX)
//
//  -> a: Constant
//  -> m: Slope
//
// ==========================================================================



// ==========================================================================
//  Static member variables
// ==========================================================================

// Histograms to fit
TH1D* PlasticFitter::m_data          = NULL;
TH1D* PlasticFitter::m_mc_Tgt4Pb     = NULL;
TH1D* PlasticFitter::m_mc_Tgt5Pb     = NULL;
TH1D* PlasticFitter::m_mc_Tgt5Fe     = NULL;
TH1D* PlasticFitter::m_mc_DominPlas  = NULL;
TH1D* PlasticFitter::m_mc_OtherPlas1 = NULL;
TH1D* PlasticFitter::m_mc_OtherPlas2 = NULL;
TH1D* PlasticFitter::m_mc_OtherMat   = NULL;


// Function pivot in X
double PlasticFitter::m_function_pivotX = 0.0;





// ==========================================================================
//  Constructors
// ==========================================================================

// Parametrized constructor
// ========================

PlasticFitter::PlasticFitter(const CVUniverse& universe,
                             Histograms hists,
                             const EnumModels& type_model,
                             std::string phys_region,
                             std::string plas_sideband)
    : m_model_type(type_model),
      m_phys_region(phys_region),
      m_plas_sideband(plas_sideband)
{
    Initialize(universe, hists);
}


// Destructor
// ==========

PlasticFitter::~PlasticFitter() {}





// ==========================================================================
//  Initialize
// ==========================================================================

void PlasticFitter::Initialize(const CVUniverse& universe,
                               Histograms hists)
{
    // Signal region histograms
    // ========================
    
    if ( m_phys_region == "SigReg" )
    {
        // Plastic upstream
        if ( m_plas_sideband == "PlasUpSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasUpSB_In_SigReg);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasUpSB_In_SigReg_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic between
        else if ( m_plas_sideband == "PlasBetwSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasBetwSB_In_SigReg);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic downstream
        else if ( m_plas_sideband == "PlasDownSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasDownSB_In_SigReg);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasDownSB_In_SigReg_TrueOtherMat.univHist(&universe));
        }
        
        else {
            std::cout << " PlasticFitter::Initialize() ERROR: Wrong plastic sideband input!!! " << std::endl;
            exit(1);
        }
    }
    
    
    // Pion-like shower sideband histograms
    // ====================================
    
    else if ( m_phys_region == "PionBlobSB" )
    {
        // Plastic upstream
        if ( m_plas_sideband == "PlasUpSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasUpSB_In_PionBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic between
        else if ( m_plas_sideband == "PlasBetwSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasBetwSB_In_PionBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic downstream
        else if ( m_plas_sideband == "PlasDownSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasDownSB_In_PionBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        else {
            std::cout << " PlasticFitter::Initialize() ERROR: Wrong plastic sideband input!!! " << std::endl;
            exit(1);
        }
    }
    
    
    // Proton-like shower sideband histograms
    // ======================================
    
    else if ( m_phys_region == "ProtonBlobSB" )
    {
        // Plastic upstream
        if ( m_plas_sideband == "PlasUpSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasUpSB_In_ProtonBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic between
        else if ( m_plas_sideband == "PlasBetwSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasBetwSB_In_ProtonBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic downstream
        else if ( m_plas_sideband == "PlasDownSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasDownSB_In_ProtonBlobSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.univHist(&universe));
        }
        
        else {
            std::cout << " PlasticFitter::Initialize() ERROR: Wrong plastic sideband input!!! " << std::endl;
            exit(1);
        }
    }
    
    
    // High-W sideband histograms
    // ==========================
    
    else if ( m_phys_region == "HighWSB" )
    {
        // Plastic upstream
        if ( m_plas_sideband == "PlasUpSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasUpSB_In_HighWSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic between
        else if ( m_plas_sideband == "PlasBetwSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasBetwSB_In_HighWSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.univHist(&universe));
        }
        
        // Plastic downstream
        else if ( m_plas_sideband == "PlasDownSB" ) {
            m_data          = new TH1D(*hists.m_data_PlasDownSB_In_HighWSB);
            m_mc_Tgt4Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.univHist(&universe)); 
            m_mc_Tgt5Pb     = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.univHist(&universe));
            m_mc_Tgt5Fe     = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.univHist(&universe));
            m_mc_DominPlas  = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.univHist(&universe));
            m_mc_OtherPlas1 = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.univHist(&universe));
            m_mc_OtherPlas2 = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.univHist(&universe));
            m_mc_OtherMat   = new TH1D(*hists.m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.univHist(&universe));
        }
        
        else {
            std::cout << " PlasticFitter::Initialize() ERROR: Wrong plastic sideband input!!! " << std::endl;
            exit(1);
        }
    }
    
    else {
        std::cout << " PlasticFitter::Initialize() ERROR: Wrong physics region input!!! " << std::endl;
        exit(1);
    }
    
    
    // Pivot in fit function
    // =====================
    
    if ( m_phys_region == "SigReg" )            m_function_pivotX = 0.5125;
    else if ( m_phys_region == "PionBlobSB" )   m_function_pivotX = 0.5125;
    else if ( m_phys_region == "ProtonBlobSB" ) m_function_pivotX = 0.5125;
    else if ( m_phys_region == "HighWSB" )      m_function_pivotX = 0.625;
    
    
    // Minimum and maximum in X
    // ========================
    
    TH1D* h_data_temp = (TH1D*)m_data->Clone("data_temp");
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
//  Chi2 function
// ==========================================================================

double PlasticFitter::Chi2Function(const double* scale_vector)
{
    // Chi2 to minimize
    double chi2 = 0.0;
    
    
    // Clone input histograms
    // ======================
    
    TH1D* h_data_temp          = (TH1D*)m_data          -> Clone("data_temp");
    TH1D* h_mc_Tgt4Pb_temp     = (TH1D*)m_mc_Tgt4Pb     -> Clone("mc_Tgt4Pb_temp");
    TH1D* h_mc_Tgt5Pb_temp     = (TH1D*)m_mc_Tgt5Pb     -> Clone("mc_Tgt5Pb_temp");
    TH1D* h_mc_Tgt5Fe_temp     = (TH1D*)m_mc_Tgt5Fe     -> Clone("mc_Tgt5Fe_temp");
    TH1D* h_mc_DominPlas_temp  = (TH1D*)m_mc_DominPlas  -> Clone("mc_DominPlas_temp");
    TH1D* h_mc_OtherPlas1_temp = (TH1D*)m_mc_OtherPlas1 -> Clone("mc_OtherPlas1_temp");
    TH1D* h_mc_OtherPlas2_temp = (TH1D*)m_mc_OtherPlas2 -> Clone("mc_OtherPlas2_temp");
    TH1D* h_mc_OtherMat_temp   = (TH1D*)m_mc_OtherMat   -> Clone("mc_OtherMat_temp");
    
    
    // Calculate generalized chi2
    // ==========================
    
    // Get number of bins
    int Nbins = h_data_temp->GetNbinsX() + 1;  // Include overflow
    
    // Loop over bins
    for ( int bin = 1; bin <= Nbins; ++bin )
    {
        // Bin info
        double bin_center = h_data_temp->GetBinCenter(bin);
        
        // Scale parameter
        double scale = std::max(0.0, scale_vector[0] + scale_vector[1] * (bin_center - m_function_pivotX));
        
        // Calculate chi2 component
        double comp_mc_Tgt4Pb     = h_mc_Tgt4Pb_temp     -> GetBinContent(bin);
        double comp_mc_Tgt5Pb     = h_mc_Tgt5Pb_temp     -> GetBinContent(bin);
        double comp_mc_Tgt5Fe     = h_mc_Tgt5Fe_temp     -> GetBinContent(bin);
        double comp_mc_DominPlas  = h_mc_DominPlas_temp  -> GetBinContent(bin);
        double comp_mc_OtherPlas1 = h_mc_OtherPlas1_temp -> GetBinContent(bin);
        double comp_mc_OtherPlas2 = h_mc_OtherPlas2_temp -> GetBinContent(bin);
        double comp_mc_OtherMat   = h_mc_OtherMat_temp   -> GetBinContent(bin);
        double comp_data          = h_data_temp          -> GetBinContent(bin);
        
        if ( comp_data > 0.0 ) {
            chi2 += std::pow(comp_mc_Tgt4Pb +
                             comp_mc_Tgt5Pb +
                             comp_mc_Tgt5Fe +
                             (scale * comp_mc_DominPlas) +
                             comp_mc_OtherPlas1 +
                             comp_mc_OtherPlas2 +
                             comp_mc_OtherMat   -
                             comp_data, 2.0) / comp_data;
        }
    }  // End of loop over bins
    
    // Release memory
    delete h_data_temp;
    delete h_mc_Tgt4Pb_temp;
    delete h_mc_Tgt5Pb_temp;
    delete h_mc_Tgt5Fe_temp;
    delete h_mc_DominPlas_temp;
    delete h_mc_OtherPlas1_temp;
    delete h_mc_OtherPlas2_temp;
    delete h_mc_OtherMat_temp;
    
    // Return chi2
    return chi2;
}





// ==========================================================================
//  Fit function
// ==========================================================================

void PlasticFitter::Fit()
{
    // Define linear function parameters
    // =================================
    
    double start_a,       start_m;
    double start_err_a,   start_err_m;
    double lower_limit_a, lower_limit_m;
    double upper_limit_a, upper_limit_m;
    
    if ( m_model_type == kGENIE || m_model_type == kMnvGENIEv1 ||
         m_model_type == kMnvGENIEv1_noNonResPi || m_model_type == kMnvGENIEv1_noD2 || m_model_type == kMnvGENIEv1_noPionTune ||
         m_model_type == kMnvGENIEv2_MINOS || m_model_type == kMnvGENIEv2_JOINT || m_model_type == kMnvGENIEv2_NU1PI ||
         m_model_type == kMnvGENIEv2_NUNPI || m_model_type == kMnvGENIEv2_NUPI0 || m_model_type == kMnvGENIEv2_MENU1PI ) {
        start_a       = 1.0;
        start_err_a   = 0.00000001;
        lower_limit_a = 0.0;
        upper_limit_a = 3.0;
        
        start_m       = 0.5;
        start_err_m   = 0.00000001;
        lower_limit_m = -5.0;
        upper_limit_m = 5.0;
    }
    else {
        std::cout << " PlasticFitter::Fit() ERROR: Can't fit data, select correct MC model!!! " << std::endl;
        exit(1);
    }
    
    
    // Perform minimization
    // ====================
    
    int Nparameters = 2;
    
    ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2");
    ROOT::Math::Functor functor(&Chi2Function, Nparameters);
    
    minimizer -> SetFunction(functor);
    minimizer -> SetMaxFunctionCalls(10000000);
    minimizer -> SetMaxIterations(10000000);
    minimizer -> SetTolerance(0.01);
    minimizer -> SetPrintLevel(2);
    
    // Initialize parameters
    minimizer -> SetVariable(0, "Par_a", start_a, 0.00000001);
    minimizer -> SetVariable(1, "Par_m", start_m, 0.00000001);
    minimizer -> SetVariableLimits(0, lower_limit_a, upper_limit_a);
    minimizer -> SetVariableLimits(1, lower_limit_m, upper_limit_m);
    
    // Run minimizer
    minimizer -> Minimize();
    
    
    // Get parameters after fit
    // ========================
    
    double final_a = minimizer->X()[0];
    double final_m = minimizer->X()[1];
    double final_err_a = minimizer->Errors()[0];
    double final_err_m = minimizer->Errors()[1];
    
    TF1* function = new TF1("function", "max(0.0, [0] + [1] * (x-[2]))", m_Xmin, m_Xmax);
    function -> SetParameter(0, final_a);
    function -> SetParameter(1, final_m);
    function -> SetParameter(2, m_function_pivotX);
    function -> SetParError(0, final_err_a);
    function -> SetParError(1, final_err_m);
    
    m_function = function;
}


#endif  // PlasticFitter_cxx