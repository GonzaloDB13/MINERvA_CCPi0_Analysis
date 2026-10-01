#ifndef PlotFullXsectionModels_C
#define PlotFullXsectionModels_C

#include <iostream>
#include <vector>

#include "../includes/CVUniverse.h"
#include "../includes/MacroUtil.h"
#include "../includes/CCPi0Event.h"
#include "../includes/TruthMatching.h"
#include "../includes/Constants.h"
#include "../includes/Binning.h"
#include "../includes/GetVariables.h"
#include "../includes/common_functions.h"
#include "../includes/plotting_functions_finalversion.h"
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#endif  // __CINT__

#include "PlotUtils/MnvPlotter.h"

#include "TFile.h"





// ========================================================================================================================
//  HELPER FUNCTIONS
// ========================================================================================================================

// =====================================================
//  Get integral and errors from matrices
// =====================================================

void GetIntegralAndErrorFromCovMatrix(PlotUtils::MnvH1D* histo,
                                      TH2D* stat_cov_histo,
                                      TH2D* syst_cov_histo,
                                      TH2D* total_cov_histo,
                                      double& integral,
                                      double& stat_err,
                                      double& syst_err,
                                      double& total_err,
                                      bool use_bin_width_norm = true,
                                      int max_bins            = -1)
{
    // Bin width normalize (if possible)
    PlotUtils::MnvH1D* histo_clone = (PlotUtils::MnvH1D*)histo->Clone("");
    if ( use_bin_width_norm ) histo_clone -> Scale(histo_clone->GetNormBinWidth(), "width");
    
    
    // Get number of bins (covariance matrices written in TH2D form include under/overflow)
    int size   = total_cov_histo->GetNbinsX() - 2;
    int NbinsX = histo_clone->GetNbinsX();
    
    if ( max_bins != -1 ) {
        size   = max_bins;
        NbinsX = max_bins;
    }
    
    if ( size != NbinsX ) {
        std::cout << " ERROR IN THE NUMBER OF HISTOGRAM BINS AND COVARIANCE MATRIX DIMENSIONALITY!!! " << std::endl;
        exit(1);
    }
    
    
    // Define covariances in TMatrixD form
    TMatrixD stat_cov_matrix(size, size);
    TMatrixD syst_cov_matrix(size, size);
    TMatrixD total_cov_matrix(size, size);
    
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            stat_cov_matrix[x][y]  = stat_cov_histo->GetBinContent(x+2, y+2);  // Get TH2D info from bins 2 to 14
            syst_cov_matrix[x][y]  = syst_cov_histo->GetBinContent(x+2, y+2);
            total_cov_matrix[x][y] = total_cov_histo->GetBinContent(x+2, y+2);
        }
    }
    
    
    // Get vector of bin widths
    TMatrixD ket_norm_widths(size, 1);
    TMatrixD bra_norm_widths(1, size);
    
    double first_bin_width = histo_clone->GetBinWidth(1);
    for ( int bin = 1; bin <= NbinsX; ++bin ) {
        double bin_width_norm = first_bin_width / histo_clone->GetBinWidth(bin);
        ket_norm_widths[bin-1][0] = bin_width_norm;
        bra_norm_widths[0][bin-1] = bin_width_norm;
    }
    
    
    // Calculate integral and errors
    integral = histo_clone->Integral(1, NbinsX);
    
    TMatrixD stat_err_unitmatrix  = bra_norm_widths * (stat_cov_matrix * ket_norm_widths);
    TMatrixD syst_err_unitmatrix  = bra_norm_widths * (syst_cov_matrix * ket_norm_widths);
    TMatrixD total_err_unitmatrix = bra_norm_widths * (total_cov_matrix * ket_norm_widths);
    
    stat_err  = std::sqrt(stat_err_unitmatrix[0][0]);
    syst_err  = std::sqrt(syst_err_unitmatrix[0][0]);
    total_err = std::sqrt(total_err_unitmatrix[0][0]);
    
    delete histo_clone;
}





// ========================================================================================================================
//  PLOT FUNCTION
// ========================================================================================================================

void Plot(CCPi0::MacroUtil util,
          TFile& xsec_fin_v0,
          TFile& xsec_fin_v1,
          TFile& xsec_fin_v1noNonResPi,
          TFile& xsec_fin_v1noD2,
          TFile& xsec_fin_v1noPionTune,
          TFile& xsec_fin_v2MINOS,
          TFile& xsec_fin_v2JOINT,
          TFile& xsec_fin_v2NU1PI,
          TFile& xsec_fin_v2NUNPI,
          TFile& xsec_fin_v2NUPI0,
          TFile& xsec_fin_v2MENU1PI,
          TFile& xsec_fin_GENIE3_02a,
          TFile& xsec_fin_GENIE3_02b,
          TFile& xsec_fin_GENIE3_10a,
          TFile& xsec_fin_GENIE3_10b,
          TFile& xsec_fin_NEUT_LFG,
          TFile& data_fin,
          std::string option_date_data,
          std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/XsectionModels/plots/%s/%s", option_date_data.c_str(),
                                                                                                   option_material.c_str());
    
    
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/XsectionModels/XsectionInfo/%s/%s", option_date_data.c_str(),
                                                                                                        option_material.c_str());
    
    std::ofstream text_file(Form("%s/XsectionInfo_%s.txt", text_topdir.c_str(),
                                                           option_material.c_str()));
    
    text_file << std::setprecision(3) << std::fixed;
    
    text_file << std::endl;
    text_file << " ============================== " << std::endl;
    text_file << "  CROSS-SECTION DATA-MC MODELS  " << std::endl;
    text_file << " ============================== " << std::endl;
    text_file << std::endl;
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Material title
    std::string material_title;
    if ( option_material == "lead" )      material_title = " - [Lead]";
    else if ( option_material == "iron" ) material_title = " - [Iron]";
    
    
    // PlotInfo properties
    const bool do_frac_uncertainty = true;
    const bool do_cov_area_norm    = false;
    const bool include_stat_error  = true;
    const bool do_bin_width_norm   = true;
    const std::string print_format = "eps";
    
    
    // =========================================
    //  Loop over variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Get variable name
        std::string var_name = var->Name();
        
        
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        // Data
        PlotUtils::MnvH1D* h_data_CrossSection = (PlotUtils::MnvH1D*)data_fin.Get(Form("data_CrossSection_%s", var_name.c_str()));
        
        TH2D* h_data_TotalStatCovMatrix = (TH2D*)data_fin.Get(Form("data_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix = (TH2D*)data_fin.Get(Form("data_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalCovMatrix     = (TH2D*)data_fin.Get(Form("data_TotalCovMatrix_%s",     var_name.c_str()));
        
        // X-section models from GENIE 2
        PlotUtils::MnvH1D* h_mc_CrossSection_v0           = (PlotUtils::MnvH1D*)xsec_fin_v0.Get(          Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v1           = (PlotUtils::MnvH1D*)xsec_fin_v1.Get(          Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v1noNonResPi = (PlotUtils::MnvH1D*)xsec_fin_v1noNonResPi.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v1noD2       = (PlotUtils::MnvH1D*)xsec_fin_v1noD2.Get(      Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v1noPionTune = (PlotUtils::MnvH1D*)xsec_fin_v1noPionTune.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2MINOS      = (PlotUtils::MnvH1D*)xsec_fin_v2MINOS.Get(     Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2JOINT      = (PlotUtils::MnvH1D*)xsec_fin_v2JOINT.Get(     Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2NU1PI      = (PlotUtils::MnvH1D*)xsec_fin_v2NU1PI.Get(     Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2NUNPI      = (PlotUtils::MnvH1D*)xsec_fin_v2NUNPI.Get(     Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2NUPI0      = (PlotUtils::MnvH1D*)xsec_fin_v2NUPI0.Get(     Form("mc_CrossSection_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_v2MENU1PI    = (PlotUtils::MnvH1D*)xsec_fin_v2MENU1PI.Get(   Form("mc_CrossSection_%s", var_name.c_str()));
        
        // X-section models from GENIE 3 and NEUT
        TH1D* mc_CrossSection_GENIE3_02a = (TH1D*)xsec_fin_GENIE3_02a.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        TH1D* mc_CrossSection_GENIE3_02b = (TH1D*)xsec_fin_GENIE3_02b.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        TH1D* mc_CrossSection_GENIE3_10a = (TH1D*)xsec_fin_GENIE3_10a.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        TH1D* mc_CrossSection_GENIE3_10b = (TH1D*)xsec_fin_GENIE3_10b.Get(Form("mc_CrossSection_%s", var_name.c_str()));
        TH1D* mc_CrossSection_NEUT_LFG   = (TH1D*)xsec_fin_NEUT_LFG.Get(  Form("mc_CrossSection_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_CrossSection_GENIE3_02a = new PlotUtils::MnvH1D(*mc_CrossSection_GENIE3_02a);
        PlotUtils::MnvH1D* h_mc_CrossSection_GENIE3_02b = new PlotUtils::MnvH1D(*mc_CrossSection_GENIE3_02b);
        PlotUtils::MnvH1D* h_mc_CrossSection_GENIE3_10a = new PlotUtils::MnvH1D(*mc_CrossSection_GENIE3_10a);
        PlotUtils::MnvH1D* h_mc_CrossSection_GENIE3_10b = new PlotUtils::MnvH1D(*mc_CrossSection_GENIE3_10b);
        PlotUtils::MnvH1D* h_mc_CrossSection_NEUT_LFG   = new PlotUtils::MnvH1D(*mc_CrossSection_NEUT_LFG);
        
        h_mc_CrossSection_GENIE3_02a -> ClearAllErrorBands();
        h_mc_CrossSection_GENIE3_02b -> ClearAllErrorBands();
        h_mc_CrossSection_GENIE3_10a -> ClearAllErrorBands();
        h_mc_CrossSection_GENIE3_10b -> ClearAllErrorBands();
        h_mc_CrossSection_NEUT_LFG   -> ClearAllErrorBands();
        
        h_mc_CrossSection_GENIE3_02a -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        h_mc_CrossSection_GENIE3_02b -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        h_mc_CrossSection_GENIE3_10a -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        h_mc_CrossSection_GENIE3_10b -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        h_mc_CrossSection_NEUT_LFG   -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        
        
        // Plot x-section models
        // =====================
        
        // POT normalized
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_v0,
                                  h_mc_CrossSection_v1noNonResPi,
                                  h_mc_CrossSection_v1noD2,
                                  h_mc_CrossSection_v1noPionTune,
                                  output_topdir + "/FullXsecModels_VER1_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
        
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_v2MINOS,
                                  h_mc_CrossSection_v2JOINT,
                                  h_mc_CrossSection_v2NU1PI,
                                  h_mc_CrossSection_v2NUNPI,
                                  h_mc_CrossSection_v2NUPI0,
                                  h_mc_CrossSection_v2MENU1PI,
                                  output_topdir + "/FullXsecModels_VER2_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
        
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_GENIE3_02a,
                                  h_mc_CrossSection_GENIE3_02b,
                                  h_mc_CrossSection_GENIE3_10a,
                                  h_mc_CrossSection_GENIE3_10b,
                                  h_mc_CrossSection_NEUT_LFG,
                                  output_topdir + "/FullXsecModels_VER3_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
        
        // Area normalized
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_v0,
                                  h_mc_CrossSection_v1noNonResPi,
                                  h_mc_CrossSection_v1noD2,
                                  h_mc_CrossSection_v1noPionTune,
                                  output_topdir + "/FullXsecModels_ShapeOnly_VER1_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                  -1.0, -1.0, true, false, true);
        
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_v2MINOS,
                                  h_mc_CrossSection_v2JOINT,
                                  h_mc_CrossSection_v2NU1PI,
                                  h_mc_CrossSection_v2NUNPI,
                                  h_mc_CrossSection_v2NUPI0,
                                  h_mc_CrossSection_v2MENU1PI,
                                  output_topdir + "/FullXsecModels_ShapeOnly_VER2_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                  -1.0, -1.0, true, false, true);
        
        PlotDataMC_FullXsecModels(plot_info,
                                  h_data_CrossSection,
                                  h_mc_CrossSection_v1,
                                  h_mc_CrossSection_GENIE3_02a,
                                  h_mc_CrossSection_GENIE3_02b,
                                  h_mc_CrossSection_GENIE3_10a,
                                  h_mc_CrossSection_GENIE3_10b,
                                  h_mc_CrossSection_NEUT_LFG,
                                  output_topdir + "/FullXsecModels_ShapeOnly_VER3_" + option_material,
                                  "Cross-section models" + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                  -1.0, -1.0, true, false, true);
        
        
        // Plot x-section model ratios
        // ===========================
        
        double Ymax1 = 0.0;
        double Ymax2 = 0.0;
        double Ymax3 = 0.0;
        if ( option_material == "lead" ) {
            Ymax1 = 2.8;
            Ymax2 = 2.8;
            Ymax3 = 2.8;
        }
        else if ( option_material == "iron" ) {
            Ymax1 = 4.4;
            Ymax2 = 4.4;
            Ymax3 = 4.4;
        }
        
        PlotDataMCRatio_FullXsecModels(plot_info,
                                       h_data_CrossSection,
                                       h_mc_CrossSection_v1,
                                       h_mc_CrossSection_v0,
                                       h_mc_CrossSection_v1noNonResPi,
                                       h_mc_CrossSection_v1noD2,
                                       h_mc_CrossSection_v1noPionTune,
                                       output_topdir + "/FullXsecModelsRatio_VER1_" + option_material,
                                       "Cross-section models ratio" + material_title,
                                       "Muon transverse momentum [GeV/c]",
                                       "Ratio to MINER#nuA Tune 4.0.1", 0.0, Ymax1);
        
        PlotDataMCRatio_FullXsecModels(plot_info,
                                       h_data_CrossSection,
                                       h_mc_CrossSection_v1,
                                       h_mc_CrossSection_v2MINOS,
                                       h_mc_CrossSection_v2JOINT,
                                       h_mc_CrossSection_v2NU1PI,
                                       h_mc_CrossSection_v2NUNPI,
                                       h_mc_CrossSection_v2NUPI0,
                                       h_mc_CrossSection_v2MENU1PI,
                                       output_topdir + "/FullXsecModelsRatio_VER2_" + option_material,
                                       "Cross-section models ratio" + material_title,
                                       "Muon transverse momentum [GeV/c]",
                                       "Ratio to MINER#nuA Tune 4.0.1", 0.0, Ymax2);
        
        PlotDataMCRatio_FullXsecModels(plot_info,
                                       h_data_CrossSection,
                                       h_mc_CrossSection_v1,
                                       h_mc_CrossSection_GENIE3_02a,
                                       h_mc_CrossSection_GENIE3_02b,
                                       h_mc_CrossSection_GENIE3_10a,
                                       h_mc_CrossSection_GENIE3_10b,
                                       h_mc_CrossSection_NEUT_LFG,
                                       output_topdir + "/FullXsecModelsRatio_VER3_" + option_material,
                                       "Cross-section models ratio" + material_title,
                                       "Muon transverse momentum [GeV/c]",
                                       "Ratio to MINER#nuA Tune 4.0.1", 0.0, Ymax3);
        
        
        // Get integrated x-section
        // ========================
        
        double integral, stat_err, syst_err, total_err;
        
        text_file << std::endl;
        text_file << " INTEGRATED CROSS-SECTION: "  << std::endl;
        text_file << " ------------------------  "  << std::endl;
        text_file << " [ x 10^(-39) cm2/nucleon ] " << std::endl;
        text_file << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v0, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " GENIE 2.12.6 ............. " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " MINERvA Tune 4.0.1 ....... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noNonResPi, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/o non-RES pi ..... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noD2, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/o deuterium pi ... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noPionTune, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/o any pi tune .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2MINOS, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 MINOS .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2JOINT, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 JOINT .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NU1PI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 NU1PI .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NUNPI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 NUNPI .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NUPI0, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 NUPI0 .... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2MENU1PI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " 4.0.1 w/ low-Q2 MENU1PI .. " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_02a, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " GENIE 3.0.6 02a .......... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_02b, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " GENIE 3.0.6 02b .......... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_10a, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " GENIE 3.0.6 10a .......... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_10b, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " GENIE 3.0.6 10b .......... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_NEUT_LFG, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true);
        text_file << " NEUT w/LFG ............... " << " X-sec: " << integral/1.0e-39
                                                    << " | Stat err: " << stat_err/1.0e-39 << " | Syst err: " << syst_err/1.0e-39 << " | Total err: " << total_err/1.0e-39 << std::endl;
        
        text_file << std::endl;
        
        
        // Get integrated x-section in first 3 bins
        // ========================================
        
        text_file << std::endl;
        text_file << " INTEGRATED CROSS-SECTION IN FIRST 3 BINS: " << std::endl;
        text_file << " ----------------------------------------  " << std::endl;
        text_file << " [ x 10^(-41) cm2/nucleon ] "                << std::endl;
        text_file << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v0, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " GENIE 2.12.6 ............. " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " MINERvA Tune 4.0.1 ....... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noNonResPi, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/o non-RES pi ..... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noD2, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/o deuterium pi ... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v1noPionTune, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/o any pi tune .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2MINOS, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 MINOS .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2JOINT, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 JOINT .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NU1PI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 NU1PI .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NUNPI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 NUNPI .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2NUPI0, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 NUPI0 .... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_v2MENU1PI, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " 4.0.1 w/ low-Q2 MENU1PI .. " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_02a, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " GENIE 3.0.6 RFG hA........ " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_02b, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " GENIE 3.0.6 RFG hN........ " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_10a, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " GENIE 3.0.6 LFG hA ....... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_GENIE3_10b, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " GENIE 3.0.6 LFG hN ....... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_CrossSection_NEUT_LFG, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix, integral, stat_err, syst_err, total_err, true, 3);
        text_file << " NEUT 5.0.2 LFG ........... " << " X-sec: " << integral/1.0e-41
                                                    << " | Stat err: " << stat_err/1.0e-41 << " | Syst err: " << syst_err/1.0e-41 << " | Total err: " << total_err/1.0e-41 << std::endl;
        
        text_file << std::endl;
        
        
        // Get data-MC chi2
        // ================
        
        int ndf = 0;
        PlotUtils::MnvPlotter mnv_plotter;
        
        // Arguments of Chi2DataMC():
        // -> Data histogram
        // -> MC histogram
        // -> Degrees of freedom
        // -> MC scale (in my case = 1)
        // -> Use data error matrix? (YES)
        // -> Use only shape errors? (NO)
        // -> Use MC histogram stat error matrix? (NO)
        // -> Chi2ByBin TMatrix (in my case = NULL)
        double chi2_v0           = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v0,           ndf, 1.0, true, false, false, NULL);
        double chi2_v1           = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v1,           ndf, 1.0, true, false, false, NULL);
        double chi2_v1noNonResPi = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v1noNonResPi, ndf, 1.0, true, false, false, NULL);
        double chi2_v1noD2       = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v1noD2,       ndf, 1.0, true, false, false, NULL);
        double chi2_v1noPionTune = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v1noPionTune, ndf, 1.0, true, false, false, NULL);
        double chi2_v2MINOS      = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2MINOS,      ndf, 1.0, true, false, false, NULL);
        double chi2_v2JOINT      = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2JOINT,      ndf, 1.0, true, false, false, NULL);
        double chi2_v2NU1PI      = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2NU1PI,      ndf, 1.0, true, false, false, NULL);
        double chi2_v2NUNPI      = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2NUNPI,      ndf, 1.0, true, false, false, NULL);
        double chi2_v2NUPI0      = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2NUPI0,      ndf, 1.0, true, false, false, NULL);
        double chi2_v2MENU1PI    = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_v2MENU1PI,    ndf, 1.0, true, false, false, NULL);
        double chi2_GENIE3_02a   = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_GENIE3_02a,   ndf, 1.0, true, false, false, NULL);
        double chi2_GENIE3_02b   = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_GENIE3_02b,   ndf, 1.0, true, false, false, NULL);
        double chi2_GENIE3_10a   = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_GENIE3_10a,   ndf, 1.0, true, false, false, NULL);
        double chi2_GENIE3_10b   = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_GENIE3_10b,   ndf, 1.0, true, false, false, NULL);
        double chi2_NEUT_LFG     = mnv_plotter.Chi2DataMC(h_data_CrossSection, h_mc_CrossSection_NEUT_LFG,     ndf, 1.0, true, false, false, NULL);
        
        // Print info
        text_file << std::setprecision(1) << std::fixed;
        
        text_file << std::endl;
        text_file << " CHI2 (ndf = " << ndf << "): " << std::endl;
        text_file << " ---------------  "  << std::endl;
        text_file << std::endl;
        text_file << " GENIE 2.12.6 ............. " << chi2_v0           << std::endl;
        text_file << " MINERvA Tune 4.0.1 ....... " << chi2_v1           << std::endl;
        text_file << " 4.0.1 w/o non-RES pi ..... " << chi2_v1noNonResPi << std::endl;
        text_file << " 4.0.1 w/o deuterium pi ... " << chi2_v1noD2       << std::endl;
        text_file << " 4.0.1 w/o any pi tune .... " << chi2_v1noPionTune << std::endl;
        text_file << " 4.0.1 w/ low-Q2 MINOS .... " << chi2_v2MINOS      << std::endl;
        text_file << " 4.0.1 w/ low-Q2 JOINT .... " << chi2_v2JOINT      << std::endl;
        text_file << " 4.0.1 w/ low-Q2 NU1PI .... " << chi2_v2NU1PI      << std::endl;
        text_file << " 4.0.1 w/ low-Q2 NUNPI .... " << chi2_v2NUNPI      << std::endl;
        text_file << " 4.0.1 w/ low-Q2 NUPI0 .... " << chi2_v2NUPI0      << std::endl;
        text_file << " 4.0.1 w/ low-Q2 MENU1PI .. " << chi2_v2MENU1PI    << std::endl;
        text_file << " GENIE 3.0.6 RFG hA ....... " << chi2_GENIE3_02a   << std::endl;
        text_file << " GENIE 3.0.6 RFG hN ....... " << chi2_GENIE3_02b   << std::endl;
        text_file << " GENIE 3.0.6 LFG hA ....... " << chi2_GENIE3_10a   << std::endl;
        text_file << " GENIE 3.0.6 LFG hN ....... " << chi2_GENIE3_10b   << std::endl;
        text_file << " NEUT 5.0.2 LFG ........... " << chi2_NEUT_LFG     << std::endl;
        text_file << std::endl;
        
        
        // Delete dummy histograms
        delete h_data_CrossSection;
        delete h_mc_CrossSection_v0;
        delete h_mc_CrossSection_v1;
        delete h_mc_CrossSection_v1noNonResPi;
        delete h_mc_CrossSection_v1noD2;
        delete h_mc_CrossSection_v1noPionTune;
        delete h_mc_CrossSection_v2MINOS;
        delete h_mc_CrossSection_v2JOINT;
        delete h_mc_CrossSection_v2NU1PI;
        delete h_mc_CrossSection_v2NUNPI;
        delete h_mc_CrossSection_v2NUPI0;
        delete h_mc_CrossSection_v2MENU1PI;
        delete h_mc_CrossSection_GENIE3_02a;
        delete h_mc_CrossSection_GENIE3_02b;
        delete h_mc_CrossSection_GENIE3_10a;
        delete h_mc_CrossSection_GENIE3_10b;
        delete h_mc_CrossSection_NEUT_LFG;
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotFullXsectionModels(std::string option_date,
                            std::string option_base_model,
                            std::string option_bkg_fit_function = "Bilinear")
{
    // Get MC base model for MacroUtil
    EnumModels type_model;
    GetModel(option_base_model, type_model);
    
    
    // Options for input/output file 
    const std::string option_date_data = option_date + "_" + option_base_model;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // Monte Carlo
    std::string xsec_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/%s/lead", option_date_data.c_str());
    std::string xsec_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/%s/iron", option_date_data.c_str());
    
    // Data
    std::string data_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/lead", option_date_data.c_str());
    std::string data_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/iron", option_date_data.c_str());
    
    
    // Read input files
    // ================
    
    // X-section models
    TFile xsec_fin_v0_lead(Form(          "%s/XsectionModels_MnvGENIEv0_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v1_lead(Form(          "%s/XsectionModels_MnvGENIEv1_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v1noNonResPi_lead(Form("%s/XsectionModels_MnvGENIEv1noNonResPi_lead.root", xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v1noD2_lead(Form(      "%s/XsectionModels_MnvGENIEv1noD2_lead.root",       xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v1noPionTune_lead(Form("%s/XsectionModels_MnvGENIEv1noPionTune_lead.root", xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2MINOS_lead(Form(     "%s/XsectionModels_MnvGENIEv2MINOS_lead.root",      xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2JOINT_lead(Form(     "%s/XsectionModels_MnvGENIEv2JOINT_lead.root",      xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2NU1PI_lead(Form(     "%s/XsectionModels_MnvGENIEv2NU1PI_lead.root",      xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2NUNPI_lead(Form(     "%s/XsectionModels_MnvGENIEv2NUNPI_lead.root",      xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2NUPI0_lead(Form(     "%s/XsectionModels_MnvGENIEv2NUPI0_lead.root",      xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_v2MENU1PI_lead(Form(   "%s/XsectionModels_MnvGENIEv2MENU1PI_lead.root",    xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_GENIE3_02a_lead(Form(  "%s/XsectionModels_GENIE3_02a_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_GENIE3_02b_lead(Form(  "%s/XsectionModels_GENIE3_02b_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_GENIE3_10a_lead(Form(  "%s/XsectionModels_GENIE3_10a_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_GENIE3_10b_lead(Form(  "%s/XsectionModels_GENIE3_10b_lead.root",           xsec_fin_topdir_lead.c_str()), "READ");
    TFile xsec_fin_NEUT_LFG_lead(Form(    "%s/XsectionModels_NEUT_LFG_lead.root",             xsec_fin_topdir_lead.c_str()), "READ");
    
    TFile xsec_fin_v0_iron(Form(          "%s/XsectionModels_MnvGENIEv0_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v1_iron(Form(          "%s/XsectionModels_MnvGENIEv1_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v1noNonResPi_iron(Form("%s/XsectionModels_MnvGENIEv1noNonResPi_iron.root", xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v1noD2_iron(Form(      "%s/XsectionModels_MnvGENIEv1noD2_iron.root",       xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v1noPionTune_iron(Form("%s/XsectionModels_MnvGENIEv1noPionTune_iron.root", xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2MINOS_iron(Form(     "%s/XsectionModels_MnvGENIEv2MINOS_iron.root",      xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2JOINT_iron(Form(     "%s/XsectionModels_MnvGENIEv2JOINT_iron.root",      xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2NU1PI_iron(Form(     "%s/XsectionModels_MnvGENIEv2NU1PI_iron.root",      xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2NUNPI_iron(Form(     "%s/XsectionModels_MnvGENIEv2NUNPI_iron.root",      xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2NUPI0_iron(Form(     "%s/XsectionModels_MnvGENIEv2NUPI0_iron.root",      xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_v2MENU1PI_iron(Form(   "%s/XsectionModels_MnvGENIEv2MENU1PI_iron.root",    xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_GENIE3_02a_iron(Form(  "%s/XsectionModels_GENIE3_02a_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_GENIE3_02b_iron(Form(  "%s/XsectionModels_GENIE3_02b_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_GENIE3_10a_iron(Form(  "%s/XsectionModels_GENIE3_10a_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_GENIE3_10b_iron(Form(  "%s/XsectionModels_GENIE3_10b_iron.root",           xsec_fin_topdir_iron.c_str()), "READ");
    TFile xsec_fin_NEUT_LFG_iron(Form(    "%s/XsectionModels_NEUT_LFG_iron.root",             xsec_fin_topdir_iron.c_str()), "READ");
    
    // Data
    TFile data_fin_lead(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_lead.root", data_fin_topdir_lead.c_str(),
                                                                              option_base_model.c_str(),
                                                                              option_bkg_fit_function.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_iron.root", data_fin_topdir_iron.c_str(),
                                                                              option_base_model.c_str(),
                                                                              option_bkg_fit_function.c_str()), "READ");
    
    
    
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
    // (TRUTH option set as 'false', SYSTEMATICS option set as 'true')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, false, true, type_model);
    
    
    // Set MacroUtil POT
    SetMacroUtilPOT(data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << " \tData POT: " << util.m_data_pot << std::endl;
    std::cout << std::endl;
    
    
    
    // =========================================
    //  Plot functions
    // =========================================
    
    std::cout << " Plotting X-section models... " << std::endl;
    std::cout << std::endl;
    
    
    // Lead
    Plot(util, xsec_fin_v0_lead, xsec_fin_v1_lead, xsec_fin_v1noNonResPi_lead, xsec_fin_v1noD2_lead, xsec_fin_v1noPionTune_lead,
         xsec_fin_v2MINOS_lead, xsec_fin_v2JOINT_lead, xsec_fin_v2NU1PI_lead, xsec_fin_v2NUNPI_lead, xsec_fin_v2NUPI0_lead, xsec_fin_v2MENU1PI_lead,
         xsec_fin_GENIE3_02a_lead, xsec_fin_GENIE3_02b_lead, xsec_fin_GENIE3_10a_lead, xsec_fin_GENIE3_10b_lead, xsec_fin_NEUT_LFG_lead,
         data_fin_lead, option_date_data, "lead");
    
    
    // Iron
    Plot(util, xsec_fin_v0_iron, xsec_fin_v1_iron, xsec_fin_v1noNonResPi_iron, xsec_fin_v1noD2_iron, xsec_fin_v1noPionTune_iron,
         xsec_fin_v2MINOS_iron, xsec_fin_v2JOINT_iron, xsec_fin_v2NU1PI_iron, xsec_fin_v2NUNPI_iron, xsec_fin_v2NUPI0_iron, xsec_fin_v2MENU1PI_iron,
         xsec_fin_GENIE3_02a_iron, xsec_fin_GENIE3_02b_iron, xsec_fin_GENIE3_10a_iron, xsec_fin_GENIE3_10b_iron, xsec_fin_NEUT_LFG_iron,
         data_fin_iron, option_date_data, "iron");
    
    
    // Close ROOT files
    xsec_fin_v0_lead.Close();
    xsec_fin_v1_lead.Close();
    xsec_fin_v1noNonResPi_lead.Close();
    xsec_fin_v1noD2_lead.Close();
    xsec_fin_v1noPionTune_lead.Close();
    xsec_fin_v2MINOS_lead.Close();
    xsec_fin_v2JOINT_lead.Close();
    xsec_fin_v2NU1PI_lead.Close();
    xsec_fin_v2NUNPI_lead.Close();
    xsec_fin_v2NUPI0_lead.Close();
    xsec_fin_GENIE3_02a_lead.Close();
    xsec_fin_GENIE3_02b_lead.Close();
    xsec_fin_GENIE3_10a_lead.Close();
    xsec_fin_GENIE3_10b_lead.Close();
    xsec_fin_NEUT_LFG_lead.Close();
    
    xsec_fin_v0_iron.Close();
    xsec_fin_v1_iron.Close();
    xsec_fin_v1noNonResPi_iron.Close();
    xsec_fin_v1noD2_iron.Close();
    xsec_fin_v1noPionTune_iron.Close();
    xsec_fin_v2MINOS_iron.Close();
    xsec_fin_v2JOINT_iron.Close();
    xsec_fin_v2NU1PI_iron.Close();
    xsec_fin_v2NUNPI_iron.Close();
    xsec_fin_v2NUPI0_iron.Close();
    xsec_fin_v2MENU1PI_iron.Close();
    xsec_fin_GENIE3_02a_iron.Close();
    xsec_fin_GENIE3_02b_iron.Close();
    xsec_fin_GENIE3_10a_iron.Close();
    xsec_fin_GENIE3_10b_iron.Close();
    xsec_fin_NEUT_LFG_iron.Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
}


#endif  // PlotFullXsectionModels_C