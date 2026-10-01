#ifndef PlotCrossSectionExtraction_C
#define PlotCrossSectionExtraction_C

#include <cmath>
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
                                      bool do_bin_width_norm = true,
                                      int max_bins           = -1)
{
    // Bin width normalize (if possible)
    PlotUtils::MnvH1D* histo_clone = (PlotUtils::MnvH1D*)histo->Clone("");
    if ( do_bin_width_norm ) histo_clone -> Scale(histo_clone->GetNormBinWidth(), "width");
    
    
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





// =====================================================
//  Write cross-section info
// =====================================================

void WriteXsectionInfo(PlotUtils::MnvH1D* histo,
                       TH2D* stat_cov_histo,
                       TH2D* syst_cov_histo,
                       TH2D* total_cov_histo,
                       std::string option_date_mc,
                       std::string option_material)
{
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream xsec_text(Form("%s/CrossSectionInfo_%s.txt", text_topdir.c_str(),
                                                               option_material.c_str()));
    
    
    // Set decimal precision
    xsec_text << std::setprecision(3) << std::fixed;
    
    
    // Get number of bins (covariance matrices written in TH2D form include under/overflow)
    int size   = total_cov_histo->GetNbinsX() - 2;
    int NbinsX = histo->GetNbinsX();
    
    
    
    // Get histos with full/stat/syst errors
    // =====================================
    
    TH1D* h_xsec_total = (TH1D*)histo->GetCVHistoWithError(true, false).Clone("");
    PlotUtils::MnvH1D* h_xsec_totalerr = new PlotUtils::MnvH1D(*h_xsec_total);
    
    TH1D* h_xsec_stat = (TH1D*)histo->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* h_xsec_statonly = new PlotUtils::MnvH1D(*h_xsec_stat);
    
    TH1D* h_xsec_syst = (TH1D*)histo->GetCVHistoWithError(false, false).Clone("");
    PlotUtils::MnvH1D* h_xsec_systonly = new PlotUtils::MnvH1D(*h_xsec_syst);
    
    
    if ( option_material == "lead" ) {
        xsec_text << std::endl;
        xsec_text << " ========================= " << std::endl;
        xsec_text << "  LEAD CROSS-SECTION INFO  " << std::endl;
        xsec_text << " ========================= " << std::endl;
        xsec_text << std::endl;
    }
    
    else if ( option_material == "iron" ) {
        xsec_text << std::endl;
        xsec_text << " ========================= " << std::endl;
        xsec_text << "  IRON CROSS-SECTION INFO  " << std::endl;
        xsec_text << " ========================= " << std::endl;
        xsec_text << std::endl;
    }
    
    
    // Get x-section per bin (WITHOUT bin width normalization)
    xsec_text << std::endl;
    xsec_text << " WITHOUT BIN WIDTH NORMALIZATION: " << std::endl;
    xsec_text << " -------------------------------  " << std::endl;
    xsec_text << " [ x 10^(-41) cm2/GeV/c/nucleon ] " << std::endl;
    xsec_text << std::endl;
    
    for ( int bin = 1; bin <= NbinsX; ++bin ) {
        double bin_low_edge  = h_xsec_totalerr->GetBinLowEdge(bin);
        double bin_high_edge = bin_low_edge + histo->GetBinWidth(bin);
        double bin_xsec      = h_xsec_totalerr->GetBinContent(bin);
        double bin_stat_err  = h_xsec_statonly->GetBinError(bin);
        double bin_syst_err  = h_xsec_systonly->GetBinError(bin);
        double bin_total_err = h_xsec_totalerr->GetBinError(bin);
        
        bin_xsec      /= 1.0e-41;
        bin_stat_err  /= 1.0e-41;
        bin_syst_err  /= 1.0e-41;
        bin_total_err /= 1.0e-41;
        
        xsec_text << " \t Bin #" << bin << " (" << bin_low_edge << " GeV/c , " << bin_high_edge << " GeV/c ) : "
                  << " X-sec: " << bin_xsec << " | Stat err: " << bin_stat_err << " | Syst err: " << bin_syst_err << " | Total err: " << bin_total_err << std::endl;
    }
    xsec_text << std::endl;
    
    
    // Bin-width normalize histograms
    h_xsec_totalerr -> Scale(h_xsec_totalerr->GetNormBinWidth(), "width");
    h_xsec_statonly -> Scale(h_xsec_statonly->GetNormBinWidth(), "width");
    h_xsec_systonly -> Scale(h_xsec_systonly->GetNormBinWidth(), "width");
    
    
    // Get x-section per bin (WITH bin width normalization)
    xsec_text << std::endl;
    xsec_text << " WITH BIN WIDTH NORMALIZATION: " << std::endl;
    xsec_text << " ----------------------------  " << std::endl;
    xsec_text << " [ x 10^(-41) cm2/GeV/c/nucleon ] " << std::endl;
    xsec_text << std::endl;
    
    for ( int bin = 1; bin <= NbinsX; ++bin ) {
        double bin_low_edge  = h_xsec_totalerr->GetBinLowEdge(bin);
        double bin_high_edge = bin_low_edge + histo->GetBinWidth(bin);
        double bin_xsec      = h_xsec_totalerr->GetBinContent(bin);
        double bin_stat_err  = h_xsec_statonly->GetBinError(bin);
        double bin_syst_err  = h_xsec_systonly->GetBinError(bin);
        double bin_total_err = h_xsec_totalerr->GetBinError(bin);
        
        bin_xsec      /= 1.0e-41;
        bin_stat_err  /= 1.0e-41;
        bin_syst_err  /= 1.0e-41;
        bin_total_err /= 1.0e-41;
        
        xsec_text << " \t Bin #" << bin << " (" << bin_low_edge << " GeV/c , " << bin_high_edge << " GeV/c ) : "
                  << " X-sec: " << bin_xsec << " | Stat err: " << bin_stat_err << " | Syst err: " << bin_syst_err << " | Total err: " << bin_total_err << std::endl;
    }
    xsec_text << std::endl;
    
    
    
    // Get integrated x-section with errors
    // ====================================
    
    double integral, stat_err, syst_err, total_err;
    double integral_3bins, stat_err_3bins, syst_err_3bins, total_err_3bins;
    
    
    // Set 'do_bin_width_norm' as false since the x-section histograms are already bin width normalized
    GetIntegralAndErrorFromCovMatrix(h_xsec_totalerr, stat_cov_histo, syst_cov_histo, total_cov_histo,
                                     integral, stat_err, syst_err, total_err, false);
    
    GetIntegralAndErrorFromCovMatrix(h_xsec_totalerr, stat_cov_histo, syst_cov_histo, total_cov_histo,
                                     integral_3bins, stat_err_3bins, syst_err_3bins, total_err_3bins, false, 3);
    
    
    xsec_text << std::endl;
    xsec_text << " INTEGRATED CROSS-SECTION: "  << std::endl;
    xsec_text << " ------------------------  "  << std::endl;
    xsec_text << " [ x 10^(-39) cm2/nucleon ] " << std::endl;
    xsec_text << std::endl;
    
    xsec_text << " \tData cross-section: " << integral/1.0e-39  << std::endl;
    xsec_text << " \t\tStat. error: "      << stat_err/1.0e-39  << std::endl;
    xsec_text << " \t\tSyst. error: "      << syst_err/1.0e-39  << std::endl;
    xsec_text << " \t\tTotal error: "      << total_err/1.0e-39 << std::endl;
    xsec_text << std::endl;
    
    
    xsec_text << std::endl;
    xsec_text << " INTEGRATED CROSS-SECTION IN FIRST 3 BINS: " << std::endl;
    xsec_text << " ----------------------------------------  " << std::endl;
    xsec_text << " [ x 10^(-41) cm2/nucleon ] "                << std::endl;
    xsec_text << std::endl;
    
    xsec_text << " \tData cross-section: " << integral_3bins/1.0e-41  << std::endl;
    xsec_text << " \t\tStat. error: "      << stat_err_3bins/1.0e-41  << std::endl;
    xsec_text << " \t\tSyst. error: "      << syst_err_3bins/1.0e-41  << std::endl;
    xsec_text << " \t\tTotal error: "      << total_err_3bins/1.0e-41 << std::endl;
    xsec_text << std::endl;
}





// =====================================================
//  Write covariance matrix into text file
// =====================================================

void WriteIndividualCovMatrixIntoTable(TH2D* h_cov_matrix,
                                       std::string file_name,
                                       std::string option_date_mc,
                                       std::string option_material)
{
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream text_file(Form("%s/Table_%s_%s.txt", text_topdir.c_str(),
                                                       file_name.c_str(),
                                                       option_material.c_str()));
    
    
    // Set decimal precision
    text_file << std::setprecision(3) << std::fixed;
    
    
    // Get numbers of rows and columns
    int NbinsX = h_cov_matrix->GetNbinsX();
    int NbinsY = h_cov_matrix->GetNbinsY();
    
    
    
    // Print covariance matrix into table form
    // =======================================
    
    if ( option_material == "lead" ) {
        text_file << std::endl;
        text_file << " ============================================ " << std::endl;
        text_file << "  LEAD COVARIANCE MATRIX TABLE IN LATEX MODE  " << std::endl;
        text_file << " ============================================ " << std::endl;    
    }
    
    else if ( option_material == "iron" ) {
        text_file << std::endl;
        text_file << " ============================================ " << std::endl;
        text_file << "  IRON COVARIANCE MATRIX TABLE IN LATEX MODE  " << std::endl;
        text_file << " ============================================ " << std::endl;    
    }
    text_file << std::endl;
    text_file << " [ x 10^(-84) (cm2/GeV/c/nucleon)^2 ] " << std::endl;
    text_file << std::endl;
    text_file << " Name of error matrix: " << file_name << std::endl;
    text_file << std::endl;
    
    // REMEMBER:
    // --------
    // First and last bins of TH2D represent under/overflow bins in the covariance matrix
    // Avoid looping over bin = 1 and bin = NbinsX/Y
    
    // Loop over rows in Y
    for ( int y = 2; y < NbinsY; ++y )
    {
        // Loop over columns in X
        for ( int x = 2; x < NbinsX; ++x )
        {
            // Get covariance in that bin
            double cov_xy = h_cov_matrix->GetBinContent(x, y);
            cov_xy /= 1.0e-84;
            
            // Print covariance
            text_file << cov_xy << " & ";
            
        }  // End of loop over columns
        
        // Pass to next row
        text_file << std::endl;
        
    }  // End of loop over rows
}





// ========================================================================================================================
//  PRE-CROSS-SECTION PLOTS
// ========================================================================================================================

void Plot_PreXsection(CCPi0::MacroUtil util,
                      TFile& fin_prexsec,
                      std::string option_date_mc,
                      std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/PreCrossSectionExtraction/plots/%s/%s", option_date_mc.c_str(),
                                                                                                              option_material.c_str());
    
    
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream evsel_text(Form("%s/EventSelectionInfo_%s.txt", text_topdir.c_str(),
                                                                  option_material.c_str()));
                                                                  
    evsel_text << std::setprecision(2) << std::fixed;
    
    
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
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        std::string var_name = var->Name();
        
        PlotUtils::MnvH1D* h_data = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("data_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_Signal = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_Signal", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_NonTuned = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_NonTuned", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_Tuned    = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_Tuned",    var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned          = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned",          var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_Pi0HighW = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_Pi0HighW", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_QElike   = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_QElike",   var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PionProd = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_PionProd", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasUp   = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_PlasUp",   var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasBetw = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_PlasBetw", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasDown = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_PlasDown", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrNonTuned_Other    = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrNonTuned_Other",    var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_BackgrTuned          = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned",          var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_Pi0HighW = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_Pi0HighW", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_QElike   = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_QElike",   var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_PionProd = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_PionProd", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasUp   = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_PlasUp",   var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasBetw = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_PlasBetw", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasDown = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_PlasDown", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_BackgrTuned_Other    = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("mc_%s_BackgrTuned_Other",    var_name.c_str()));
        
        TH2D* h_mc_TotalStatCovMatrix = (TH2D*)fin_prexsec.Get(Form("mc_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalSystCovMatrix = (TH2D*)fin_prexsec.Get(Form("mc_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalCovMatrix     = (TH2D*)fin_prexsec.Get(Form("mc_TotalCovMatrix_%s",     var_name.c_str()));
        
        TH2D* h_data_TotalStatCovMatrix = (TH2D*)fin_prexsec.Get(Form("data_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix = (TH2D*)fin_prexsec.Get(Form("data_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalCovMatrix     = (TH2D*)fin_prexsec.Get(Form("data_TotalCovMatrix_%s",     var_name.c_str()));
        
        
//         // ************************************************************************************************************************
//         // Data pre-background subtraction
//         PlotUtils::MnvH1D* h_data_clone = (PlotUtils::MnvH1D*)h_data->Clone("h_data_clone");
//         if ( do_bin_width_norm ) h_data_clone->Scale(h_data_clone->GetNormBinWidth(), "width");
//         
//         TH1* h_data_staterr = (TH1*)h_data_clone->GetCVHistoWithStatError().Clone("h_data_staterr");
//         TH1* h_data_systerr = (TH1*)h_data_clone->GetCVHistoWithError(false, do_cov_area_norm).Clone("h_data_systerr");
//         
//         
//         // MC tuned background
//         PlotUtils::MnvH1D* h_mc_BackgrTuned_clone = (PlotUtils::MnvH1D*)h_mc_BackgrTuned->Clone("h_mc_BackgrTuned_clone");
//         if ( do_bin_width_norm ) h_mc_BackgrTuned_clone->Scale(h_mc_BackgrTuned_clone->GetNormBinWidth(), "width");
//         
//         TH1* h_mc_BackgrTuned_staterr = (TH1*)h_mc_BackgrTuned_clone->GetCVHistoWithStatError().Clone("h_mc_BackgrTuned_staterr");
//         TH1* h_mc_BackgrTuned_systerr = (TH1*)h_mc_BackgrTuned_clone->GetCVHistoWithError(false, do_cov_area_norm).Clone("h_mc_BackgrTuned_systerr");
//         
//         
//         // Print histograms info
//         std::cout << std::fixed;
//         std::cout << std::setprecision(2);
//         
//         std::cout << std::endl;
//         std::cout << " DATA BEFORE BACKGROUND SUBTRACTION: " << std::endl;
//         for ( int bin = 1; bin <= h_data->GetNbinsX(); ++bin ) {
//             std::cout << " \tBin #" << bin << ": " << h_data_clone->GetBinContent(bin) << " +/- "
//                                                    << h_data_staterr->GetBinError(bin) << " +/- "
//                                                    << h_data_systerr->GetBinError(bin) << std::endl;
//         }
//         
//         std::cout << std::endl;
//         std::cout << " MC TUNED BACKGROUND: " << std::endl;
//         for ( int bin = 1; bin <= h_mc_BackgrTuned->GetNbinsX(); ++bin ) {
//             std::cout << " \tBin #" << bin << ": " << h_mc_BackgrTuned_clone->GetBinContent(bin) << " +/- "
//                                                    << h_mc_BackgrTuned_staterr->GetBinError(bin) << " +/- "
//                                                    << h_mc_BackgrTuned_systerr->GetBinError(bin) << std::endl;
//         }
//         
//         delete h_data_clone;
//         delete h_data_staterr;
//         delete h_data_systerr;
//         delete h_mc_BackgrTuned_clone;
//         delete h_mc_BackgrTuned_staterr;
//         delete h_mc_BackgrTuned_systerr;
//         // ************************************************************************************************************************
        
        
        // Plot before background tuning
        // =============================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data, h_mc_NonTuned,
                   output_topdir + "/BeforeBackgrTuning/DataMC_" + option_material,
                   "Event rate before backgr. tuning" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, true, false, false);
        
        // Data-MC with background
        // -----------------------
        PlotDataMCWithBackgr(plot_info,
                             h_data, h_mc_NonTuned, h_mc_BackgrNonTuned,
                             output_topdir + "/BeforeBackgrTuning/DataMCWithBackgr_" + option_material,
                             "Event rate before backgr. tuning" + material_title,
                             "", "", "TR", -1.0, -1.0,
                             true, true, false, false, true, false, false);
        
        // Data-MC stacked
        // ---------------
        PlotDataStackedMC(plot_info,
                          h_data, h_mc_NonTuned,
                          h_mc_Signal,
                          h_mc_BackgrNonTuned_Pi0HighW,
                          h_mc_BackgrNonTuned_QElike,
                          h_mc_BackgrNonTuned_PionProd,
                          h_mc_BackgrNonTuned_PlasUp,
                          h_mc_BackgrNonTuned_PlasBetw,
                          h_mc_BackgrNonTuned_PlasDown,
                          h_mc_BackgrNonTuned_Other,
                          output_topdir + "/BeforeBackgrTuning/DataStackedMC_" + option_material,
                          "Event rate before backgr. tuning" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, true, false, false);
        
        // Data-MC ratio
        // -------------
        double Ymin_ratio = 0.0;
        double Ymax_ratio = 0.0;
        
        if ( option_material == "lead" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 3.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data, h_mc_NonTuned,
                        output_topdir + "/BeforeBackgrTuning/DataMCRatio_" + option_material,
                        "Data-MC ratio before backgr. tuning" + material_title,
                        "", "", Ymin_ratio, Ymax_ratio,
                        true, true, false, false, true, false, false);
        
        // MC fractional errors
        // --------------------
        PlotErrorSummary(plot_info,
                         h_mc_NonTuned,
                         output_topdir + "/BeforeBackgrTuning/MCFracErrors_" + option_material,
                         "MC fractional errors before tuning" + material_title, 0.4, true);
        
        // MC backgr stacked
        // -----------------
        PlotStackedMC(plot_info,
                      h_mc_NonTuned,
                      h_mc_BackgrNonTuned_Pi0HighW,
                      h_mc_BackgrNonTuned_QElike,
                      h_mc_BackgrNonTuned_PionProd,
                      h_mc_BackgrNonTuned_PlasUp,
                      h_mc_BackgrNonTuned_PlasBetw,
                      h_mc_BackgrNonTuned_PlasDown,
                      h_mc_BackgrNonTuned_Other,
                      output_topdir + "/BeforeBackgrTuning/StackedMCBackgr_" + option_material,
                      "MC background before tuning" + material_title,
                      "", "", "TR", -1.0, -1.0,
                      true, false, true, false, false);
        
        
        // Plot after background tuning
        // ============================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data, h_mc_Tuned,
                   output_topdir + "/AfterBackgrTuning/DataMC_" + option_material,
                   "Event rate after backgr. tuning" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, false, false, true);
        
        // Data-MC with background
        // -----------------------
        PlotDataMCWithBackgr(plot_info,
                             h_data, h_mc_Tuned, h_mc_BackgrTuned,
                             output_topdir + "/AfterBackgrTuning/DataMCWithBackgr_" + option_material,
                             "Event rate after backgr. tuning" + material_title,
                             "", "", "TR", -1.0, -1.0,
                             true, true, false, false, false, false, true);
        
        // Data-MC stacked
        // ---------------
        PlotDataStackedMC(plot_info,
                          h_data, h_mc_Tuned,
                          h_mc_Signal,
                          h_mc_BackgrTuned_Pi0HighW,
                          h_mc_BackgrTuned_QElike,
                          h_mc_BackgrTuned_PionProd,
                          h_mc_BackgrTuned_PlasUp,
                          h_mc_BackgrTuned_PlasBetw,
                          h_mc_BackgrTuned_PlasDown,
                          h_mc_BackgrTuned_Other,
                          output_topdir + "/AfterBackgrTuning/DataStackedMC_" + option_material,
                          "Event rate after backgr. tuning" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, false, false, true);
        
        // Data-MC ratio
        // -------------
        if ( option_material == "lead" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 3.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data, h_mc_Tuned,
                        output_topdir + "/AfterBackgrTuning/DataMCRatio_" + option_material,
                        "Data-MC ratio after backgr. tuning" + material_title,
                        "", "", Ymin_ratio, Ymax_ratio,
                        true, true, false, false, false, false, true);
        
        // MC fractional errors
        // --------------------
        PlotErrorSummary(plot_info,
                         h_mc_Tuned,
                         output_topdir + "/AfterBackgrTuning/MCFracErrors_" + option_material,
                         "MC fractional errors after backgr. tuning" + material_title, 0.4, true);
        
        // MC stacked backgr
        // -----------------
        PlotStackedMC(plot_info,
                      h_mc_Tuned,
                      h_mc_BackgrTuned_Pi0HighW,
                      h_mc_BackgrTuned_QElike,
                      h_mc_BackgrTuned_PionProd,
                      h_mc_BackgrTuned_PlasUp,
                      h_mc_BackgrTuned_PlasBetw,
                      h_mc_BackgrTuned_PlasDown,
                      h_mc_BackgrTuned_Other,
                      output_topdir + "/AfterBackgrTuning/StackedMCBackgr_" + option_material,
                      "MC background after tuning" + material_title,
                      "", "", "TR", -1.0, -1.0,
                      true, false, false, false, true);
        
        
        // Save event selection info to text file
        // ======================================
        
        if ( option_material == "lead" ) {
            evsel_text << std::endl;
            evsel_text << " =========================== " << std::endl;
            evsel_text << "  LEAD EVENT SELECTION INFO  " << std::endl;
            evsel_text << " =========================== " << std::endl;
            evsel_text << std::endl;
        }
        
        else if ( option_material == "iron" ) {
            evsel_text << std::endl;
            evsel_text << " =========================== " << std::endl;
            evsel_text << "  IRON EVENT SELECTION INFO  " << std::endl;
            evsel_text << " =========================== " << std::endl;
            evsel_text << std::endl;
        }
        
        double mc_integral, mc_stat_err, mc_syst_err, mc_total_err;
        double data_integral, data_stat_err, data_syst_err, data_total_err;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_Tuned, h_mc_TotalStatCovMatrix, h_mc_TotalSystCovMatrix, h_mc_TotalCovMatrix,
                                         mc_integral, mc_stat_err, mc_syst_err, mc_total_err);
        
        GetIntegralAndErrorFromCovMatrix(h_data, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                                         data_integral, data_stat_err, data_syst_err, data_total_err);
        
        evsel_text << " \tMC tuned event rate: " << mc_integral  << std::endl;
        evsel_text << " \t\tStat. error: "       << mc_stat_err  << std::endl;
        evsel_text << " \t\tSyst. error: "       << mc_syst_err  << std::endl;
        evsel_text << " \t\tTotal error: "       << mc_total_err << std::endl;
        evsel_text << std::endl;
        evsel_text << " \tData event rate: " << data_integral  << std::endl;
        evsel_text << " \t\tStat. error: "   << data_stat_err  << std::endl;
        evsel_text << " \t\tSyst. error: "   << data_syst_err  << std::endl;
        evsel_text << " \t\tTotal error: "   << data_total_err << std::endl;
        evsel_text << std::endl;
        
        evsel_text.close();
        
        
        // Delete dummy histograms
        // =======================
        
        delete h_data;
        delete h_mc_Signal;
        delete h_mc_NonTuned;
        delete h_mc_BackgrNonTuned_Pi0HighW;
        delete h_mc_BackgrNonTuned_QElike;
        delete h_mc_BackgrNonTuned_PionProd;
        delete h_mc_BackgrNonTuned_PlasUp;
        delete h_mc_BackgrNonTuned_PlasBetw;
        delete h_mc_BackgrNonTuned_PlasDown;
        delete h_mc_BackgrNonTuned_Other;
        delete h_mc_Tuned;
        delete h_mc_BackgrTuned_Pi0HighW;
        delete h_mc_BackgrTuned_QElike;
        delete h_mc_BackgrTuned_PionProd;
        delete h_mc_BackgrTuned_PlasUp;
        delete h_mc_BackgrTuned_PlasBetw;
        delete h_mc_BackgrTuned_PlasDown;
        delete h_mc_BackgrTuned_Other;
        delete h_mc_TotalStatCovMatrix;
        delete h_mc_TotalSystCovMatrix;
        delete h_mc_TotalCovMatrix;
        delete h_data_TotalStatCovMatrix;
        delete h_data_TotalSystCovMatrix;
        delete h_data_TotalCovMatrix;
    }
}





// ========================================================================================================================
//  BACKGROUND SUBTRACTION PLOTS
// ========================================================================================================================

void Plot_BackgrSubtr(CCPi0::MacroUtil util,
                      TFile& fin_backgrsubtr,
                      std::string option_date_mc,
                      std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/BackgroundSubtraction/plots/%s/%s", option_date_mc.c_str(),
                                                                                                          option_material.c_str());
    
    
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream backgrsubtr_text(Form("%s/BackgrSubtractionInfo_%s.txt", text_topdir.c_str(),
                                                                           option_material.c_str()));
                                                                  
    backgrsubtr_text << std::setprecision(2) << std::fixed;
    
    
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
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        std::string var_name = var->Name();
        
        PlotUtils::MnvH1D* h_mc_BackgrSubtr   = (PlotUtils::MnvH1D*)fin_backgrsubtr.Get(Form("mc_BackgrSubtr_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_BackgrSubtr = (PlotUtils::MnvH1D*)fin_backgrsubtr.Get(Form("data_BackgrSubtr_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalStatCovMatrix = (TH2D*)fin_backgrsubtr.Get(Form("mc_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalSystCovMatrix = (TH2D*)fin_backgrsubtr.Get(Form("mc_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalCovMatrix     = (TH2D*)fin_backgrsubtr.Get(Form("mc_TotalCovMatrix_%s",     var_name.c_str()));
        
        TH2D* h_data_TotalStatCovMatrix = (TH2D*)fin_backgrsubtr.Get(Form("data_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix = (TH2D*)fin_backgrsubtr.Get(Form("data_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalCovMatrix     = (TH2D*)fin_backgrsubtr.Get(Form("data_TotalCovMatrix_%s",     var_name.c_str()));
        
        
//         // ************************************************************************************************************************
//         // Data post-background subtraction
//         PlotUtils::MnvH1D* h_data_BackgrSubtr_clone = (PlotUtils::MnvH1D*)h_data_BackgrSubtr->Clone("h_data_BackgrSubtr_clone");
//         if ( do_bin_width_norm )
//             h_data_BackgrSubtr_clone->Scale(h_data_BackgrSubtr_clone->GetNormBinWidth(), "width");
//         
//         TH1* h_data_BackgrSubtr_staterr = (TH1*)h_data_BackgrSubtr_clone->GetCVHistoWithStatError().Clone("h_data_BackgrSubtr_staterr");
//         TH1* h_data_BackgrSubtr_systerr = (TH1*)h_data_BackgrSubtr_clone->GetCVHistoWithError(false, do_cov_area_norm).Clone("h_data_BackgrSubtr_systerr");
//         
//         
//         // Print histograms info
//         std::cout << std::fixed;
//         std::cout << std::setprecision(2);
//         
//         std::cout << std::endl;
//         std::cout << " DATA AFTER BACKGROUND SUBTRACTION: " << std::endl;
//         for ( int bin = 1; bin <= h_data_BackgrSubtr->GetNbinsX(); ++bin ) {
//             std::cout << " \tBin #" << bin << ": " << h_data_BackgrSubtr_clone->GetBinContent(bin) << " +/- "
//                                                    << h_data_BackgrSubtr_staterr->GetBinError(bin) << " +/- "
//                                                    << h_data_BackgrSubtr_systerr->GetBinError(bin) << std::endl;
//         }
//         std::cout << std::endl;
//         std::cout << std::endl;
//         
//         delete h_data_BackgrSubtr_clone;
//         delete h_data_BackgrSubtr_staterr;
//         delete h_data_BackgrSubtr_systerr;
//         // ************************************************************************************************************************
        
        
        // Plot backgr-subtracted distributions
        // ====================================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data_BackgrSubtr, h_mc_BackgrSubtr,
                   output_topdir + "/BackgrSubtracted/DataMC_" + option_material,
                   "Background-subtracted event rates" + material_title,
                   "", "", "TR", -1.0, -1.0);
        
        // Data-MC ratio
        // -------------
        double Ymin_ratio = 0.0;
        double Ymax_ratio = 0.0;
        
        if ( option_material == "lead" ) {
            Ymin_ratio = -1.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 4.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data_BackgrSubtr, h_mc_BackgrSubtr,
                        output_topdir + "/BackgrSubtracted/DataMCRatio_" + option_material,
                        "Data-MC ratio backgr. subtraction" + material_title, "", "", Ymin_ratio, Ymax_ratio);
        
        // Fractional errors
        // -----------------
        double mc_Ymax_error   = 0.0;
        double data_Ymax_error = 0.0;
        
        if ( option_material == "lead" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.6;
        }
        if ( option_material == "iron" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.35;
        }
        PlotErrorSummary(plot_info,
                         h_mc_BackgrSubtr,
                         output_topdir + "/BackgrSubtracted/MCFracErrors_" + option_material,
                         "MC fractional errors after backgr. subtraction" + material_title, mc_Ymax_error, true);
        
        PlotErrorSummary(plot_info,
                         h_data_BackgrSubtr,
                         output_topdir + "/BackgrSubtracted/DataFracErrors_" + option_material,
                         "Fractional errors after backgr. subtraction" + material_title, data_Ymax_error, true);
        
        
        // Save backgr subtraction info to text file
        // =========================================
        
        if ( option_material == "lead" ) {
            backgrsubtr_text << std::endl;
            backgrsubtr_text << " ============================================ " << std::endl;
            backgrsubtr_text << "  LEAD BACKGROUND SUBTRACTED EVENT RATE INFO  " << std::endl;
            backgrsubtr_text << " ============================================ " << std::endl;
            backgrsubtr_text << std::endl;
        }
        
        else if ( option_material == "iron" ) {
            backgrsubtr_text << std::endl;
            backgrsubtr_text << " ============================================ " << std::endl;
            backgrsubtr_text << "  IRON BACKGROUND SUBTRACTED EVENT RATE INFO  " << std::endl;
            backgrsubtr_text << " ============================================ " << std::endl;
            backgrsubtr_text << std::endl;
        }
        
        double mc_integral, mc_stat_err, mc_syst_err, mc_total_err;
        double data_integral, data_stat_err, data_syst_err, data_total_err;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_BackgrSubtr, h_mc_TotalStatCovMatrix, h_mc_TotalSystCovMatrix, h_mc_TotalCovMatrix,
                                         mc_integral, mc_stat_err, mc_syst_err, mc_total_err);
        
        GetIntegralAndErrorFromCovMatrix(h_data_BackgrSubtr, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                                         data_integral, data_stat_err, data_syst_err, data_total_err);
        
        backgrsubtr_text << " \tMC event rate: " << mc_integral  << std::endl;
        backgrsubtr_text << " \t\tStat. error: " << mc_stat_err  << std::endl;
        backgrsubtr_text << " \t\tSyst. error: " << mc_syst_err  << std::endl;
        backgrsubtr_text << " \t\tTotal error: " << mc_total_err << std::endl;
        backgrsubtr_text << std::endl;
        backgrsubtr_text << " \tData event rate: " << data_integral  << std::endl;
        backgrsubtr_text << " \t\tStat. error: "   << data_stat_err  << std::endl;
        backgrsubtr_text << " \t\tSyst. error: "   << data_syst_err  << std::endl;
        backgrsubtr_text << " \t\tTotal error: "   << data_total_err << std::endl;
        backgrsubtr_text << std::endl;
        
        backgrsubtr_text.close();
        
        
        // Delete dummy histograms
        // =======================
        
        delete h_mc_BackgrSubtr;
        delete h_data_BackgrSubtr;
        delete h_mc_TotalStatCovMatrix;
        delete h_mc_TotalSystCovMatrix;
        delete h_mc_TotalCovMatrix;
        delete h_data_TotalStatCovMatrix;
        delete h_data_TotalSystCovMatrix;
        delete h_data_TotalCovMatrix;
    }
}





// ========================================================================================================================
//  UNFOLDING PLOTS
// ========================================================================================================================

void Plot_Unfolding(CCPi0::MacroUtil util,
                    TFile& fin_prexsec,
                    TFile& fin_unfolding,
                    std::string option_date_mc,
                    std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/Unfolding/plots/%s/%s", option_date_mc.c_str(),
                                                                                              option_material.c_str());
    
    
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream unfold_text(Form("%s/UnfoldEventRateInfo_%s.txt", text_topdir.c_str(),
                                                                    option_material.c_str()));
                                                                  
    unfold_text << std::setprecision(2) << std::fixed;
    
    
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
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        std::string var_name = var->Name();
        
        PlotUtils::MnvH1D* h_EffNumerator = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("EffNumerator_%s",   var_name.c_str()));
        
        PlotUtils::MnvH2D* h_Migration = (PlotUtils::MnvH2D*)fin_prexsec.Get(Form("MigrationMatrix_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_Folded   = (PlotUtils::MnvH1D*)fin_unfolding.Get(Form("mc_Folded_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_Folded = (PlotUtils::MnvH1D*)fin_unfolding.Get(Form("data_Folded_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_Unfolded   = (PlotUtils::MnvH1D*)fin_unfolding.Get(Form("mc_Unfolded_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_Unfolded = (PlotUtils::MnvH1D*)fin_unfolding.Get(Form("data_Unfolded_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_UnfoldClosure = (PlotUtils::MnvH1D*)fin_unfolding.Get(Form("mc_UnfoldClosure_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalStatCovMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalStatCovMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalStatCovMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalStatCovMatrix_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalSystCovMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalSystCovMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalSystCovMatrix_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalCovMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalCovMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalCovMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalCovMatrix_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalStatCorrMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalStatCorrMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalStatCorrMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalStatCorrMatrix_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalSystCorrMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalSystCorrMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalSystCorrMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalSystCorrMatrix_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalCorrMatrix   = (TH2D*)fin_unfolding.Get(Form("mc_TotalCorrMatrix_%s",   var_name.c_str()));
        TH2D* h_data_TotalCorrMatrix = (TH2D*)fin_unfolding.Get(Form("data_TotalCorrMatrix_%s", var_name.c_str()));
        
        
        // Plot folded distributions
        // =========================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data_Folded, h_mc_Folded,
                   output_topdir + "/Folded/DataMC_" + option_material,
                   "Background-subtracted event rates" + material_title,
                   "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]");
        
        // Data-MC ratio
        // -------------
        double Ymin_ratio = 0.0;
        double Ymax_ratio = 0.0;
        
        if ( option_material == "lead" ) {
            Ymin_ratio = -1.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 4.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data_Folded, h_mc_Folded,
                        output_topdir + "/Folded/DataMCRatio_" + option_material,
                        "Data-MC ratio after backgr. subtraction" + material_title,
                        "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "", Ymin_ratio, Ymax_ratio);
        
        // Fractional errors
        // -----------------
        double mc_Ymax_error   = 0.0;
        double data_Ymax_error = 0.0;
        
        if ( option_material == "lead" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.6;
        }
        if ( option_material == "iron" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.35;
        }
        PlotErrorSummary(plot_info,
                         h_mc_Folded,
                         output_topdir + "/Folded/MCFracErrors_" + option_material,
                         "MC fractional errors after backgr. subtraction" + material_title, mc_Ymax_error, true,
                         "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]");
        
        PlotErrorSummary(plot_info,
                         h_data_Folded,
                         output_topdir + "/Folded/DataFracErrors_" + option_material,
                         "Fractional errors after backgr. subtraction" + material_title, data_Ymax_error, true,
                         "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]");
        
        
        // Plot unfolded distributions
        // ===========================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data_Unfolded, h_mc_Unfolded,
                   output_topdir + "/Unfolded/DataMC_" + option_material,
                   "Unfolded event rates" + material_title,
                   "Muon transverse momentum [GeV/c]");
        
        // Data-MC ratio
        // -------------
        if ( option_material == "lead" ) {
            Ymin_ratio = -1.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 4.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data_Unfolded, h_mc_Unfolded,
                        output_topdir + "/Unfolded/DataMCRatio_" + option_material,
                        "Data-MC ratio after unfolding" + material_title,
                        "Muon transverse momentum [GeV/c]", "", Ymin_ratio, Ymax_ratio);
        
        // Fractional errors
        // -----------------
        PlotErrorSummary(plot_info,
                         h_mc_Unfolded,
                         output_topdir + "/Unfolded/MCFracErrors_" + option_material,
                         "MC fractional errors after unfolding" + material_title, mc_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        PlotErrorSummary(plot_info, h_data_Unfolded,
                         output_topdir + "/Unfolded/DataFracErrors_" + option_material,
                         "Fractional errors after unfolding" + material_title, data_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        // Closure ratio
        // -------------
        PlotClosureRatio(plot_info, h_mc_UnfoldClosure,
                         output_topdir + "/Unfolded/UnfoldingClosure_" + option_material,
                         "Unfolding closure" + material_title,
                         "Muon transverse momentum [GeV/c]", "1 - (Unfolded MC / Eff. Num.)");
        
        
        // Plot migration matrix
        // =====================
        
        PlotRowNormMigration(plot_info,
                             h_Migration,
                             output_topdir + "/Unfolded/MigrationMatrix_" + option_material,
                             "Migration matrix" + material_title,
                             "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]",
                             "True muon #font[12]{p}_{#font[132]{T}} [GeV/c]", 
                             "% of true row in reco bin", true, false);
        
        PlotRowNormMigration(plot_info,
                             h_Migration,
                             output_topdir + "/Unfolded/MigrationMatrixWithFlows_" + option_material,
                             "Migration matrix w/ overflow" + material_title,
                             "Reconstructed muon #font[12]{p}_{#font[132]{T}} bin number",
                             "True muon #font[12]{p}_{#font[132]{T}} bin number",
                             "% of true row in reco bin", true, true);
        
        
        // Plot covariance matrices
        // ========================
        
        // Monte Carlo
        // -----------
        PlotCovarianceMatrix(plot_info, h_mc_TotalStatCovMatrix,
                             output_topdir + "/Covariance/MCStatCovMatrix_" + option_material,
                             "MC stat. covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_mc_TotalSystCovMatrix,
                             output_topdir + "/Covariance/MCSystCovMatrix_" + option_material,
                             "MC syst. covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_mc_TotalCovMatrix,
                             output_topdir + "/Covariance/MCTotalCovMatrix_" + option_material,
                             "MC total covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        // Data
        // ----
        PlotCovarianceMatrix(plot_info, h_data_TotalStatCovMatrix,
                             output_topdir + "/Covariance/DataStatCovMatrix_" + option_material,
                             "Stat. covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_TotalSystCovMatrix,
                             output_topdir + "/Covariance/DataSystCovMatrix_" + option_material,
                             "Syst. covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_TotalCovMatrix,
                             output_topdir + "/Covariance/DataTotalCovMatrix_" + option_material,
                             "Total covariance matrix after unfolding" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        
        // Plot correlation matrices
        // =========================
        
        // Monte Carlo
        // -----------
        PlotCorrelationMatrix(plot_info, h_mc_TotalStatCorrMatrix,
                              output_topdir + "/Correlation/MCStatCorrMatrix_" + option_material,
                              "MC stat. correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_mc_TotalSystCorrMatrix,
                              output_topdir + "/Correlation/MCSystCorrMatrix_" + option_material,
                              "MC syst. correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_mc_TotalCorrMatrix,
                              output_topdir + "/Correlation/MCTotalCorrMatrix_" + option_material,
                              "MC total correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        // Data
        // ----
        PlotCorrelationMatrix(plot_info, h_data_TotalStatCorrMatrix,
                              output_topdir + "/Correlation/DataStatCorrMatrix_" + option_material,
                              "Stat. correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_TotalSystCorrMatrix,
                              output_topdir + "/Correlation/DataSystCorrMatrix_" + option_material,
                              "Syst. correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_TotalCorrMatrix,
                              output_topdir + "/Correlation/DataTotalCorrMatrix_" + option_material,
                              "Total correlation matrix after unfolding" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        
        // Save unfolding info to text file
        // ================================
        
        if ( option_material == "lead" ) {
            unfold_text << std::endl;
            unfold_text << " ========================================== " << std::endl;
            unfold_text << "  LEAD FOLDED AND UNFOLDED EVENT RATE INFO  " << std::endl;
            unfold_text << " ========================================== " << std::endl;
            unfold_text << std::endl;
        }
        
        else if ( option_material == "iron" ) {
            unfold_text << std::endl;
            unfold_text << " ========================================== " << std::endl;
            unfold_text << "  IRON FOLDED AND UNFOLDED EVENT RATE INFO  " << std::endl;
            unfold_text << " ========================================== " << std::endl;
            unfold_text << std::endl;
        }
        
        double mc_integral, mc_stat_err, mc_syst_err, mc_total_err;
        double data_integral, data_stat_err, data_syst_err, data_total_err;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_Folded, h_mc_TotalStatCovMatrix, h_mc_TotalSystCovMatrix, h_mc_TotalCovMatrix,
                                         mc_integral, mc_stat_err, mc_syst_err, mc_total_err);
        
        GetIntegralAndErrorFromCovMatrix(h_data_Folded, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                                         data_integral, data_stat_err, data_syst_err, data_total_err);
        
        unfold_text << std::endl;
        unfold_text << " FOLDED EVENT RATE: " << std::endl;
        unfold_text << " -----------------  " << std::endl;
        unfold_text << std::endl;
        unfold_text << " \tMC event rate: " << mc_integral  << std::endl;
        unfold_text << " \t\tStat. error: " << mc_stat_err  << std::endl;
        unfold_text << " \t\tSyst. error: " << mc_syst_err  << std::endl;
        unfold_text << " \t\tTotal error: " << mc_total_err << std::endl;
        unfold_text << std::endl;
        unfold_text << " \tData event rate: " << data_integral  << std::endl;
        unfold_text << " \t\tStat. error: "   << data_stat_err  << std::endl;
        unfold_text << " \t\tSyst. error: "   << data_syst_err  << std::endl;
        unfold_text << " \t\tTotal error: "   << data_total_err << std::endl;
        unfold_text << std::endl;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_Unfolded, h_mc_TotalStatCovMatrix, h_mc_TotalSystCovMatrix, h_mc_TotalCovMatrix,
                                         mc_integral, mc_stat_err, mc_syst_err, mc_total_err);
        
        GetIntegralAndErrorFromCovMatrix(h_data_Unfolded, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                                         data_integral, data_stat_err, data_syst_err, data_total_err);
        
        unfold_text << std::endl;
        unfold_text << " UNFOLDED EVENT RATE: " << std::endl;
        unfold_text << " -------------------  " << std::endl;
        unfold_text << std::endl;
        unfold_text << " \tMC event rate: " << mc_integral  << std::endl;
        unfold_text << " \t\tStat. error: " << mc_stat_err  << std::endl;
        unfold_text << " \t\tSyst. error: " << mc_syst_err  << std::endl;
        unfold_text << " \t\tTotal error: " << mc_total_err << std::endl;
        unfold_text << std::endl;
        unfold_text << " \tData event rate: " << data_integral  << std::endl;
        unfold_text << " \t\tStat. error: "   << data_stat_err  << std::endl;
        unfold_text << " \t\tSyst. error: "   << data_syst_err  << std::endl;
        unfold_text << " \t\tTotal error: "   << data_total_err << std::endl;
        unfold_text << std::endl;
        
        unfold_text.close();
        
        
        // Delete dummy histograms
        // =======================
        
        delete h_Migration;
        delete h_mc_UnfoldClosure;
        delete h_mc_Unfolded;
        delete h_mc_TotalStatCovMatrix;
        delete h_mc_TotalSystCovMatrix;
        delete h_mc_TotalCovMatrix;
        delete h_mc_TotalStatCorrMatrix;
        delete h_mc_TotalSystCorrMatrix;
        delete h_mc_TotalCorrMatrix;
        delete h_data_Unfolded;
        delete h_data_TotalStatCovMatrix;
        delete h_data_TotalSystCovMatrix;
        delete h_data_TotalCovMatrix;
        delete h_data_TotalStatCorrMatrix;
        delete h_data_TotalSystCorrMatrix;
        delete h_data_TotalCorrMatrix;
    }
}





// ========================================================================================================================
//  EFFICIENCY CORRECTION PLOTS
// ========================================================================================================================

void Plot_EffCorr(CCPi0::MacroUtil util,
                  TFile& fin_prexsec,
                  TFile& fin_effcorr,
                  std::string option_date_mc,
                  std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/EfficiencyCorrection/plots/%s/%s", option_date_mc.c_str(),
                                                                                                         option_material.c_str());
    
    
    // Create text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream eff_text(Form("%s/EfficiencyInfo_%s.txt", text_topdir.c_str(),
                                                            option_material.c_str()));
                                                                  
    eff_text << std::setprecision(2) << std::fixed;
    
    
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
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        std::string var_name = var->Name();
        
        PlotUtils::MnvH1D* h_EffNumerator   = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("EffNumerator_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_EffDenominator = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("EffDenominator_%s", var_name.c_str()));
        PlotUtils::MnvH1D* h_Efficiency     = (PlotUtils::MnvH1D*)fin_prexsec.Get(Form("Efficiency_%s",     var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_EffCorrected   = (PlotUtils::MnvH1D*)fin_effcorr.Get(Form("mc_EffCorrected_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_EffCorrected = (PlotUtils::MnvH1D*)fin_effcorr.Get(Form("data_EffCorrected_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_EffCorrClosure = (PlotUtils::MnvH1D*)fin_effcorr.Get(Form("mc_EffCorrClosure_%s", var_name.c_str()));
        
        TH2D* h_mc_TotalStatCovMatrix = (TH2D*)fin_effcorr.Get(Form("mc_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalSystCovMatrix = (TH2D*)fin_effcorr.Get(Form("mc_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_mc_TotalCovMatrix     = (TH2D*)fin_effcorr.Get(Form("mc_TotalCovMatrix_%s",     var_name.c_str()));
        
        TH2D* h_data_TotalStatCovMatrix = (TH2D*)fin_effcorr.Get(Form("data_TotalStatCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix = (TH2D*)fin_effcorr.Get(Form("data_TotalSystCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_TotalCovMatrix     = (TH2D*)fin_effcorr.Get(Form("data_TotalCovMatrix_%s",     var_name.c_str()));
        
        
        // Plot eff-corrected distributions
        // ================================
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data_EffCorrected, h_mc_EffCorrected,
                   output_topdir + "/EffCorrected/DataMC_" + option_material,
                   "Efficiency-corrected event rates" + material_title,
                   "Muon transverse momentum [GeV/c]");
        
        // Data-MC ratio
        // -------------
        double Ymin_ratio = 0.0;
        double Ymax_ratio = 0.0;
        
        if ( option_material == "lead" ) {
            Ymin_ratio = -1.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 4.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data_EffCorrected, h_mc_EffCorrected,
                        output_topdir + "/EffCorrected/DataMCRatio_" + option_material,
                        "Data-MC ratio after efficiency correction" + material_title,
                        "Muon transverse momentum [GeV/c]", "", Ymin_ratio, Ymax_ratio);
        
        // Fractional errors
        // -----------------
        double mc_Ymax_error   = 0.0;
        double data_Ymax_error = 0.0;
        
        if ( option_material == "lead" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.6;
        }
        if ( option_material == "iron" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.35;
        }
        PlotErrorSummary(plot_info,
                         h_mc_EffCorrected,
                         output_topdir + "/EffCorrected/MCFracErrors_" + option_material,
                         "MC fractional errors after eff. correction" + material_title, mc_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        PlotErrorSummary(plot_info,
                         h_data_EffCorrected,
                         output_topdir + "/EffCorrected/DataFracErrors_" + option_material,
                         "Fractional errors after eff. correction" + material_title, data_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        // Closure ratio
        // -------------
        PlotClosureRatio(plot_info, h_mc_EffCorrClosure,
                         output_topdir + "/EffCorrected/EffCorrectionClosure_" + option_material,
                         "Eff. correction closure" + material_title,
                         "Muon transverse momentum [GeV/c]", "1 - (Eff.-corr. MC / Eff. Denom.)");
        
        
        // Plot efficiency
        // ===============
        
        // Efficiency numerator
        // --------------------
        PlotMC(plot_info, h_EffNumerator,
               output_topdir + "/Efficiency/EffNumerator_" + option_material,
               "Efficiency numerator" + material_title,
               "Muon transverse momentum [GeV/c]", "",
               -1.0, -1.0, -1.0, true, false);
        
        PlotErrorSummary(plot_info, h_EffNumerator,
                         output_topdir + "/Efficiency/EffNumeratorFracErrors_" + option_material,
                         "Efficiency numerator fractional errors" + material_title, 0.5, true,
                         "Muon transverse momentum [GeV/c]");
        
        // Efficiency denominator
        // ----------------------
        PlotMC(plot_info, h_EffDenominator,
               output_topdir + "/Efficiency/EffDenominator_" + option_material,
               "Efficiency denominator" + material_title,
               "Muon transverse momentum [GeV/c]", "",
               -1.0, -1.0, -1.0, true, false);
        
        PlotErrorSummary(plot_info, h_EffDenominator,
                         output_topdir + "/Efficiency/EffDenominatorFracErrors_" + option_material,
                         "Efficiency denominator fractional errors" + material_title, 0.5, true,
                         "Muon transverse momentum [GeV/c]");
        
        // Efficiency
        // ----------
        PlotMC(plot_info, h_Efficiency,
               output_topdir + "/Efficiency/Efficiency_" + option_material,
               "Efficiency" + material_title,
               "Muon transverse momentum [GeV/c]", "Efficiency / 0.075 GeV/c",
               -1.0, -1.0, -1.0, true, false, false, false, false, false, false, false);
        
        PlotErrorSummary(plot_info, h_Efficiency,
                         output_topdir + "/Efficiency/EfficiencyFracErrors_" + option_material,
                         "Efficiency fractional errors" + material_title, 0.4, true,
                         "Muon transverse momentum [GeV/c]");
        
        
        // Save efficiency info to text file
        // =================================
        
        if ( option_material == "lead" ) {
            eff_text << std::endl;
            eff_text << " ====================== " << std::endl;
            eff_text << "  LEAD EFFICIENCY INFO  " << std::endl;
            eff_text << " ====================== " << std::endl;
            eff_text << std::endl;
        }
        
        else if ( option_material == "iron" ) {
            eff_text << std::endl;
            eff_text << " ====================== " << std::endl;
            eff_text << "  IRON EFFICIENCY INFO  " << std::endl;
            eff_text << " ====================== " << std::endl;
            eff_text << std::endl;
        }
        
        double area_effnum = h_EffNumerator->Integral(1, h_EffNumerator->GetNbinsX());
        double area_effden = h_EffDenominator->Integral(1, h_EffDenominator->GetNbinsX());
        double eff_overall = area_effnum/area_effden;
        
        eff_text << std::endl;
        eff_text << " EFFICIENCY: " << std::endl;
        eff_text << " ----------  " << std::endl;
        eff_text << std::endl;
        eff_text << " \tArea eff. numerator:   " << area_effnum << std::endl;
        eff_text << " \tArea eff. denominator: " << area_effden << std::endl;
        eff_text << " \tOverall efficiency:    " << (eff_overall*100.0) << "% " << std::endl;
        eff_text << std::endl;
        
        double mc_integral, mc_stat_err, mc_syst_err, mc_total_err;
        double data_integral, data_stat_err, data_syst_err, data_total_err;
        
        GetIntegralAndErrorFromCovMatrix(h_mc_EffCorrected, h_mc_TotalStatCovMatrix, h_mc_TotalSystCovMatrix, h_mc_TotalCovMatrix,
                                         mc_integral, mc_stat_err, mc_syst_err, mc_total_err);
        
        GetIntegralAndErrorFromCovMatrix(h_data_EffCorrected, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                                         data_integral, data_stat_err, data_syst_err, data_total_err);
        
        eff_text << std::endl;
        eff_text << " EFFICIENCY CORRECTED EVENT RATE: " << std::endl;
        eff_text << " -------------------------------  " << std::endl;
        eff_text << std::endl;
        eff_text << " \tMC event rate: " << mc_integral  << std::endl;
        eff_text << " \t\tStat. error: " << mc_stat_err  << std::endl;
        eff_text << " \t\tSyst. error: " << mc_syst_err  << std::endl;
        eff_text << " \t\tTotal error: " << mc_total_err << std::endl;
        eff_text << std::endl;
        eff_text << " \tData event rate: " << data_integral  << std::endl;
        eff_text << " \t\tStat. error: "   << data_stat_err  << std::endl;
        eff_text << " \t\tSyst. error: "   << data_syst_err  << std::endl;
        eff_text << " \t\tTotal error: "   << data_total_err << std::endl;
        eff_text << std::endl;
        
        eff_text.close();
        
        
        // Delete dummy histograms
        // =======================
        
        delete h_EffNumerator;
        delete h_EffDenominator;
        delete h_Efficiency;
        delete h_mc_EffCorrected;
        delete h_data_EffCorrected;
        delete h_mc_TotalStatCovMatrix;
        delete h_mc_TotalSystCovMatrix;
        delete h_mc_TotalCovMatrix;
        delete h_data_TotalStatCovMatrix;
        delete h_data_TotalSystCovMatrix;
        delete h_data_TotalCovMatrix;
    }
}





// ========================================================================================================================
//  CROSS SECTION PLOTS
// ========================================================================================================================

void Plot_Xsection(CCPi0::MacroUtil util,
                   TFile& fin_xsec,
                   std::string option_date_mc,
                   std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/CrossSection/plots/%s/%s", option_date_mc.c_str(),
                                                                                                 option_material.c_str());
    
    
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
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        // ===============
        
        std::string var_name = var->Name();
        
        PlotUtils::MnvH1D* h_mc_CrossSection   = (PlotUtils::MnvH1D*)fin_xsec.Get(Form("mc_CrossSection_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_CrossSection = (PlotUtils::MnvH1D*)fin_xsec.Get(Form("data_CrossSection_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_mc_CrossSection_OldFlux   = (PlotUtils::MnvH1D*)fin_xsec.Get(Form("mc_CrossSection_OldFlux_%s",   var_name.c_str()));
        PlotUtils::MnvH1D* h_data_CrossSection_OldFlux = (PlotUtils::MnvH1D*)fin_xsec.Get(Form("data_CrossSection_OldFlux_%s", var_name.c_str()));
        
        PlotUtils::MnvH1D* h_Flux          = (PlotUtils::MnvH1D*)fin_xsec.Get("Flux");
        PlotUtils::MnvH1D* h_Flux_Old      = (PlotUtils::MnvH1D*)fin_xsec.Get("Flux_Old");
        PlotUtils::MnvH1D* h_FluxTotal     = (PlotUtils::MnvH1D*)fin_xsec.Get("FluxTotal");
        PlotUtils::MnvH1D* h_FluxTotal_Old = (PlotUtils::MnvH1D*)fin_xsec.Get("FluxTotal_Old");
        PlotUtils::MnvH1D* h_FluxRatio     = (PlotUtils::MnvH1D*)fin_xsec.Get("FluxRatio");
        
        TH2D* h_data_TotalStatCovMatrix    = (TH2D*)fin_xsec.Get(Form("data_TotalStatCovMatrix_%s",    var_name.c_str()));
        TH2D* h_data_FluxCovMatrix         = (TH2D*)fin_xsec.Get(Form("data_FluxCovMatrix_%s",         var_name.c_str()));
        TH2D* h_data_XsecCCQE2p2hCovMatrix = (TH2D*)fin_xsec.Get(Form("data_XsecCCQE2p2hCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_XsecResPiCovMatrix    = (TH2D*)fin_xsec.Get(Form("data_XsecResPiCovMatrix_%s",    var_name.c_str()));
        TH2D* h_data_XsecNonResPiCovMatrix = (TH2D*)fin_xsec.Get(Form("data_XsecNonResPiCovMatrix_%s", var_name.c_str()));
        TH2D* h_data_XsecDISCovMatrix      = (TH2D*)fin_xsec.Get(Form("data_XsecDISCovMatrix_%s",      var_name.c_str()));
        TH2D* h_data_FSINuclCovMatrix      = (TH2D*)fin_xsec.Get(Form("data_FSINuclCovMatrix_%s",      var_name.c_str()));
        TH2D* h_data_FSIPionCovMatrix      = (TH2D*)fin_xsec.Get(Form("data_FSIPionCovMatrix_%s",      var_name.c_str()));
        TH2D* h_data_MuonCovMatrix         = (TH2D*)fin_xsec.Get(Form("data_MuonCovMatrix_%s",         var_name.c_str()));
        TH2D* h_data_PartRespCovMatrix     = (TH2D*)fin_xsec.Get(Form("data_PartRespCovMatrix_%s",     var_name.c_str()));
        TH2D* h_data_OtherCovMatrix        = (TH2D*)fin_xsec.Get(Form("data_OtherCovMatrix_%s",        var_name.c_str()));
        TH2D* h_data_TotalSystCovMatrix    = (TH2D*)fin_xsec.Get(Form("data_TotalSystCovMatrix_%s",    var_name.c_str()));
        TH2D* h_data_TotalCovMatrix        = (TH2D*)fin_xsec.Get(Form("data_TotalCovMatrix_%s",        var_name.c_str()));
        
        TH2D* h_data_TotalStatCorrMatrix    = (TH2D*)fin_xsec.Get(Form("data_TotalStatCorrMatrix_%s",    var_name.c_str()));
        TH2D* h_data_FluxCorrMatrix         = (TH2D*)fin_xsec.Get(Form("data_FluxCorrMatrix_%s",         var_name.c_str()));
        TH2D* h_data_XsecCCQE2p2hCorrMatrix = (TH2D*)fin_xsec.Get(Form("data_XsecCCQE2p2hCorrMatrix_%s", var_name.c_str()));
        TH2D* h_data_XsecResPiCorrMatrix    = (TH2D*)fin_xsec.Get(Form("data_XsecResPiCorrMatrix_%s",    var_name.c_str()));
        TH2D* h_data_XsecNonResPiCorrMatrix = (TH2D*)fin_xsec.Get(Form("data_XsecNonResPiCorrMatrix_%s", var_name.c_str()));
        TH2D* h_data_XsecDISCorrMatrix      = (TH2D*)fin_xsec.Get(Form("data_XsecDISCorrMatrix_%s",      var_name.c_str()));
        TH2D* h_data_FSINuclCorrMatrix      = (TH2D*)fin_xsec.Get(Form("data_FSINuclCorrMatrix_%s",      var_name.c_str()));
        TH2D* h_data_FSIPionCorrMatrix      = (TH2D*)fin_xsec.Get(Form("data_FSIPionCorrMatrix_%s",      var_name.c_str()));
        TH2D* h_data_MuonCorrMatrix         = (TH2D*)fin_xsec.Get(Form("data_MuonCorrMatrix_%s",         var_name.c_str()));
        TH2D* h_data_PartRespCorrMatrix     = (TH2D*)fin_xsec.Get(Form("data_PartRespCorrMatrix_%s",     var_name.c_str()));
        TH2D* h_data_OtherCorrMatrix        = (TH2D*)fin_xsec.Get(Form("data_OtherCorrMatrix_%s",        var_name.c_str()));
        TH2D* h_data_TotalSystCorrMatrix    = (TH2D*)fin_xsec.Get(Form("data_TotalSystCorrMatrix_%s",    var_name.c_str()));
        TH2D* h_data_TotalCorrMatrix        = (TH2D*)fin_xsec.Get(Form("data_TotalCorrMatrix_%s",        var_name.c_str()));
        
        
        // Plot cross-section
        // ==================
        
        std::string xsec_title;
        if ( option_material == "lead" )      xsec_title = "#nu_{#mu} + Pb #rightarrow #mu^{-} + #pi^{0} + nucleon(s) + #font[12]{A'}";
        else if ( option_material == "iron" ) xsec_title = "#nu_{#mu} + Fe #rightarrow #mu^{-} + #pi^{0} + nucleon(s) + #font[12]{A'}";
        
        // Data-MC
        // -------
        PlotDataMC(plot_info,
                   h_data_CrossSection, h_mc_CrossSection,
                   output_topdir + "/CrossSection/DataMC_" + option_material,
                   xsec_title,
                   "Muon transverse momentum [GeV/c]", "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
        
        // Data-MC ratio
        // -------------
        double Ymin_ratio = 0.0;
        double Ymax_ratio = 0.0;
        
        if ( option_material == "lead" ) {
            Ymin_ratio = -1.0;
            Ymax_ratio = 2.0;
        }
        else if ( option_material == "iron" ) {
            Ymin_ratio = 0.0;
            Ymax_ratio = 4.0;
        }
        PlotDataMCRatio(plot_info,
                        h_data_CrossSection, h_mc_CrossSection,
                        output_topdir + "/CrossSection/DataMCRatio_" + option_material,
                        "Data-MC cross-section ratio" + material_title,
                        "Muon transverse momentum [GeV/c]", "", Ymin_ratio, Ymax_ratio);
        
        // Fractional errors
        // -----------------
        double mc_Ymax_error   = 0.0;
        double data_Ymax_error = 0.0;
        
        if ( option_material == "lead" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.6;
        }
        if ( option_material == "iron" ) {
            mc_Ymax_error = 0.5;
            data_Ymax_error = 0.35;
        }
        PlotErrorSummary(plot_info,
                         h_mc_CrossSection,
                         output_topdir + "/CrossSection/MCFracErrors_" + option_material,
                         "MC cross-section fractional errors" + material_title, mc_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        PlotErrorSummary(plot_info,
                         h_data_CrossSection,
                         output_topdir + "/CrossSection/DataFracErrors_" + option_material,
                         "Cross-section fractional errors" + material_title, data_Ymax_error, true,
                         "Muon transverse momentum [GeV/c]");
        
        
        // Plot error groups
        // =================
        
        // Error group names
        std::vector<std::string> error_group_names;
        error_group_names.push_back("Neutrino Flux");
        error_group_names.push_back("X-Sec: EL/CCQE/2p2h/RPA");
        error_group_names.push_back("X-Sec: RES pion");
        error_group_names.push_back("X-Sec: Non-RES pion");
        error_group_names.push_back("X-Sec: DIS");
        error_group_names.push_back("FSI: Nucleons");
        error_group_names.push_back("FSI: Pions");
        error_group_names.push_back("Muon Reconstruction");
        error_group_names.push_back("Particle Response");
        error_group_names.push_back("Other");
        
        // Error group Y max
        std::vector<double> error_group_Ymax;
        
        if ( option_material == "lead" ) {
            error_group_Ymax.push_back(0.15);  // Flux
            error_group_Ymax.push_back(0.1);   // EL/CCQE/2p2h/RPA x-section
            error_group_Ymax.push_back(0.2);   // RES pion x-section
            error_group_Ymax.push_back(0.5);   // Non-RES pion x-section
            error_group_Ymax.push_back(0.2);   // DIS x-section
            error_group_Ymax.push_back(0.35);  // FSI nucleons
            error_group_Ymax.push_back(0.2);   // FSI pions
            error_group_Ymax.push_back(0.35);  // Muon reconstruction
            error_group_Ymax.push_back(0.7);   // Particle response
            error_group_Ymax.push_back(0.05);  // Other
        }
        else if ( option_material == "iron" ) {
            error_group_Ymax.push_back(0.08);  // Flux
            error_group_Ymax.push_back(0.18);  // EL/CCQE/2p2h/RPA x-section
            error_group_Ymax.push_back(0.15);  // RES pion x-section
            error_group_Ymax.push_back(0.1);   // Non-RES pion x-section
            error_group_Ymax.push_back(0.1);   // DIS x-section
            error_group_Ymax.push_back(0.2);   // FSI nucleons
            error_group_Ymax.push_back(0.15);  // FSI pions
            error_group_Ymax.push_back(0.2);   // Muon reconstruction
            error_group_Ymax.push_back(0.25);  // Particle response
            error_group_Ymax.push_back(0.05);  // Other
        }
        
        PlotErrorGroups(plot_info,
                        h_data_CrossSection,
                        output_topdir + "/ErrorGroups/", option_material,
                        error_group_names, error_group_Ymax, true,
                        "Muon transverse momentum [GeV/c]");
        
        
        // Plot flux
        // =========
        
        PlotFlux(plot_info,
                 h_Flux,
                 output_topdir + "/Flux/FluxPerPOT_" + option_material,
                 "Neutrino flux" + material_title,
                 "Neutrino energy [GeV]", "#nu_{#mu} / m^{2} / GeV / POT");
        
        PlotFlux(plot_info,
                 h_FluxTotal,
                 output_topdir + "/Flux/FluxTotal_" + option_material,
                 "Neutrino flux" + material_title,
                 "Neutrino energy [GeV]", "#nu_{#mu} / cm^{2} / GeV");
        
        PlotFlux(plot_info,
                 h_Flux_Old,
                 output_topdir + "/Flux/FluxPerPOT_Old_" + option_material,
                 "Neutrino flux (FHC #nu-e only)" + material_title,
                 "Neutrino energy [GeV]", "#nu_{#mu} / m^{2} / GeV / POT");
        
        PlotFlux(plot_info,
                 h_FluxTotal_Old,
                 output_topdir + "/Flux/FluxTotal_Old_" + option_material,
                 "Neutrino flux (FHC #nu-e only)" + material_title,
                 "Neutrino energy [GeV]", "#nu_{#mu} / cm^{2} / GeV");
        
        PlotFluxRatio(plot_info,
                      h_FluxRatio,
                      output_topdir + "/Flux/FluxRatio_" + option_material,
                      "Flux: Full constraint VS FHC #nu-e only" + material_title,
                      "Neutrino energy [GeV]", "Full constraint / FHC #nu-e only");
        
        
        // Write x-section info
        // ====================
        
        WriteXsectionInfo(h_data_CrossSection, h_data_TotalStatCovMatrix, h_data_TotalSystCovMatrix, h_data_TotalCovMatrix,
                          option_date_mc, option_material);
        
        
        // Plot covariance matrices (data only)
        // ====================================
        
        PlotCovarianceMatrix(plot_info, h_data_TotalStatCovMatrix,
                             output_topdir + "/Covariance/DataStatCovMatrix_" + option_material,
                             "Statistical covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_FluxCovMatrix,
                             output_topdir + "/Covariance/DataFluxCovMatrix_" + option_material,
                             "Flux covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_XsecCCQE2p2hCovMatrix,
                             output_topdir + "/Covariance/DataXsecCCQE2p2hCovMatrix_" + option_material,
                             "EL/CCQE/2p2h/RPA x-section cov. matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_XsecResPiCovMatrix,
                             output_topdir + "/Covariance/DataXsecResPiCovMatrix_" + option_material,
                             "RES #pi x-section cov. matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_XsecNonResPiCovMatrix,
                             output_topdir + "/Covariance/DataXsecNonResPiCovMatrix_" + option_material,
                             "Non-RES #pi x-section cov. matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_XsecDISCovMatrix,
                             output_topdir + "/Covariance/DataXsecDISCovMatrix_" + option_material,
                             "DIS x-section covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_FSINuclCovMatrix,
                             output_topdir + "/Covariance/DataFSINuclCovMatrix_" + option_material,
                             "Nucleon FSI covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_FSIPionCovMatrix,
                             output_topdir + "/Covariance/DataFSIPionCovMatrix_" + option_material,
                             "Pion FSI covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_MuonCovMatrix,
                             output_topdir + "/Covariance/DataMuonCovMatrix_" + option_material,
                             "Muon reconstruction cov. matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_PartRespCovMatrix,
                             output_topdir + "/Covariance/DataPartRespCovMatrix_" + option_material,
                             "Particle response cov. matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_OtherCovMatrix,
                             output_topdir + "/Covariance/DataOtherCovMatrix_" + option_material,
                             "Other syst. covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_TotalSystCovMatrix,
                             output_topdir + "/Covariance/DataSystCovMatrix_" + option_material,
                             "Systematic covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        PlotCovarianceMatrix(plot_info, h_data_TotalCovMatrix,
                             output_topdir + "/Covariance/DataTotalCovMatrix_" + option_material,
                             "Total stat. + syst. covariance matrix" + material_title,
                             "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin covariance");
        
        
        // Write covariance matrix into tables
        // ===================================
        
        WriteIndividualCovMatrixIntoTable(h_data_TotalStatCovMatrix,    "StatCov",         option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_FluxCovMatrix,         "FluxCov",         option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_XsecCCQE2p2hCovMatrix, "XsecCCQE2p2hCov", option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_XsecResPiCovMatrix,    "XsecResPiCov",    option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_XsecNonResPiCovMatrix, "XsecNonResPiCov", option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_XsecDISCovMatrix,      "XsecDISCov",      option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_FSINuclCovMatrix,      "FSINuclCov",      option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_FSIPionCovMatrix,      "FSIPionCov",      option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_MuonCovMatrix,         "MuonCov",         option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_PartRespCovMatrix,     "PartRespCov",     option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_OtherCovMatrix,        "OtherCov",        option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_TotalSystCovMatrix,    "TotalSystCov",    option_date_mc, option_material);
        WriteIndividualCovMatrixIntoTable(h_data_TotalCovMatrix,        "TotalCov",        option_date_mc, option_material);
        
        
        // Plot correlation matrices (data only)
        // =====================================
        
        PlotCorrelationMatrix(plot_info, h_data_TotalStatCorrMatrix,
                              output_topdir + "/Correlation/DataStatCorrMatrix_" + option_material,
                              "Statistical correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_FluxCorrMatrix,
                              output_topdir + "/Correlation/DataFluxCorrMatrix_" + option_material,
                              "Flux correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_XsecCCQE2p2hCorrMatrix,
                              output_topdir + "/Correlation/DataXsecCCQE2p2hCorrMatrix_" + option_material,
                              "EL/CCQE/2p2h/RPA x-section corr. matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_XsecResPiCorrMatrix,
                              output_topdir + "/Correlation/DataXsecResPiCorrMatrix_" + option_material,
                              "RES #pi x-section corr. matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_XsecNonResPiCorrMatrix,
                              output_topdir + "/Correlation/DataXsecNonResPiCorrMatrix_" + option_material,
                              "Non-RES #pi x-section corr. matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_XsecDISCorrMatrix,
                              output_topdir + "/Correlation/DataXsecDISCorrMatrix_" + option_material,
                              "DIS x-section correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_FSINuclCorrMatrix,
                              output_topdir + "/Correlation/DataFSINuclCorrMatrix_" + option_material,
                              "Nucleon FSI correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_FSIPionCorrMatrix,
                              output_topdir + "/Correlation/DataFSIPionCorrMatrix_" + option_material,
                              "Pion FSI correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_MuonCorrMatrix,
                              output_topdir + "/Correlation/DataMuonCorrMatrix_" + option_material,
                              "Muon reconstruction corr. matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_PartRespCorrMatrix,
                              output_topdir + "/Correlation/DataPartRespCorrMatrix_" + option_material,
                              "Particle response corr. matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_OtherCorrMatrix,
                              output_topdir + "/Correlation/DataOtherCorrMatrix_" + option_material,
                              "Other syst. correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_TotalSystCorrMatrix,
                              output_topdir + "/Correlation/DataSystCorrMatrix_" + option_material,
                              "Systematic correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        PlotCorrelationMatrix(plot_info, h_data_TotalCorrMatrix,
                              output_topdir + "/Correlation/DataTotalCorrMatrix_" + option_material,
                              "Total stat. + syst. correlation matrix" + material_title,
                              "Muon #font[12]{p}_{#font[132]{T}} bin number", "Muon #font[12]{p}_{#font[132]{T}} bin number", "Bin-to-bin correlation");
        
        
        // Delete dummy histograms
        // =======================
        
        delete h_mc_CrossSection;
        delete h_data_CrossSection;
        delete h_data_TotalStatCovMatrix;
        delete h_data_FluxCovMatrix;
        delete h_data_XsecCCQE2p2hCovMatrix;
        delete h_data_XsecResPiCovMatrix;
        delete h_data_XsecNonResPiCovMatrix;
        delete h_data_XsecDISCovMatrix;
        delete h_data_FSINuclCovMatrix;
        delete h_data_FSIPionCovMatrix;
        delete h_data_MuonCovMatrix;
        delete h_data_PartRespCovMatrix;
        delete h_data_OtherCovMatrix;
        delete h_data_TotalSystCovMatrix;
        delete h_data_TotalCovMatrix;
        delete h_data_TotalStatCorrMatrix;
        delete h_data_FluxCorrMatrix;
        delete h_data_XsecCCQE2p2hCorrMatrix;
        delete h_data_XsecResPiCorrMatrix;
        delete h_data_XsecNonResPiCorrMatrix;
        delete h_data_XsecDISCorrMatrix;
        delete h_data_FSINuclCorrMatrix;
        delete h_data_FSIPionCorrMatrix;
        delete h_data_MuonCorrMatrix;
        delete h_data_PartRespCorrMatrix;
        delete h_data_OtherCorrMatrix;
        delete h_data_TotalSystCorrMatrix;
        delete h_data_TotalCorrMatrix;
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotCrossSectionExtraction(std::string option_date,
                                std::string option_model,
                                std::string option_bkg_fit_function,
                                bool plot_unfolding   = true)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file 
    const std::string option_date_mc   = option_date + "_" + option_model;
    
    
    
    // =========================================
    //  Input files before unfolding
    // =========================================
    
    // Top directories
    std::string fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/lead", option_date_mc.c_str());
    std::string fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/iron", option_date_mc.c_str());
    
    
    // Pre-cross-section extraction input
    TFile fin_prexsec_lead(Form("%s/PreCrossSectionExtraction_MnvGENIE%s_Bkg%sFit_lead.root", fin_topdir_lead.c_str(),
                                                                                              option_model.c_str(),
                                                                                              option_bkg_fit_function.c_str()), "READ");
    
    TFile fin_prexsec_iron(Form("%s/PreCrossSectionExtraction_MnvGENIE%s_Bkg%sFit_iron.root", fin_topdir_iron.c_str(),
                                                                                              option_model.c_str(),
                                                                                              option_bkg_fit_function.c_str()), "READ");
    
    
    // Background subtraction input
    TFile fin_backgrsubtr_lead(Form("%s/BackgroundSubtraction_MnvGENIE%s_Bkg%sFit_lead.root", fin_topdir_lead.c_str(),
                                                                                              option_model.c_str(),
                                                                                              option_bkg_fit_function.c_str()), "READ");
    
    TFile fin_backgrsubtr_iron(Form("%s/BackgroundSubtraction_MnvGENIE%s_Bkg%sFit_iron.root", fin_topdir_iron.c_str(),
                                                                                              option_model.c_str(),
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
    SetMacroUtilPOT(fin_prexsec_lead, util);
    
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << " \tData POT: " << util.m_data_pot << std::endl;
    std::cout << std::endl;
    
    
    // // Extraction info text files
    // std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
    //                                                                                        option_material.c_str());
    
    // std::ofstream evsel_text(Form("%s/EventSelectionInfo_%s.txt", text_topdir.c_str(),
    //                                                               option_material.c_str()));
    
    // std::ofstream backgrsubtr_text(Form("%s/BackgrSubtractionInfo_%s.txt", text_topdir.c_str(),
    //                                                                        option_material.c_str()));
    
    // std::ofstream unfold_text(Form("%s/UnfoldEventRateInfo_%s.txt", text_topdir.c_str(),
    //                                                                 option_material.c_str()));
    
    // std::ofstream eff_text(Form("%s/EfficiencyInfo_%s.txt", text_topdir.c_str(),
    //                                                         option_material.c_str()));
    
    // std::ofstream xsec_text(Form("%s/CrossSectionInfo_%s.txt", text_topdir.c_str(),
    //                                                            option_material.c_str()));
    
    
    // =========================================
    //  Plot before unfolding
    // =========================================
    
    std::cout << " Plotting cross-section extraction steps... " << std::endl;
    std::cout << std::endl;
    
    
    // Pre-cross-section extraction
    std::cout << " \tPlotting pre-cross-section histograms... " << std::endl;
    std::cout << std::endl;
    
    Plot_PreXsection(util, fin_prexsec_lead, option_date_mc, "lead");
    Plot_PreXsection(util, fin_prexsec_iron, option_date_mc, "iron");
    
    
    // Background subtraction
    std::cout << " \tPlotting background subtraction... " << std::endl;
    std::cout << std::endl;
    
    Plot_BackgrSubtr(util, fin_backgrsubtr_lead, option_date_mc, "lead");
    Plot_BackgrSubtr(util, fin_backgrsubtr_iron, option_date_mc, "iron");
    
    
    // Exit function if requested
    if ( !plot_unfolding ) {
        fin_prexsec_lead.Close();
        fin_prexsec_iron.Close();
        
        fin_backgrsubtr_lead.Close();
        fin_backgrsubtr_iron.Close();
        
        return;
    }
    
    
    
    // =========================================
    //  Input files from unfolding
    // =========================================
    
    // Unfolding
    TFile fin_unfolding_lead(Form("%s/Unfolding_MnvGENIE%s_Bkg%sFit_lead.root", fin_topdir_lead.c_str(),
                                                                                option_model.c_str(),
                                                                                option_bkg_fit_function.c_str()), "READ");
    
    TFile fin_unfolding_iron(Form("%s/Unfolding_MnvGENIE%s_Bkg%sFit_iron.root", fin_topdir_iron.c_str(),
                                                                                option_model.c_str(),
                                                                                option_bkg_fit_function.c_str()), "READ");
    
    
    // Efficiency correction
    TFile fin_effcorr_lead(Form("%s/EfficiencyCorrection_MnvGENIE%s_Bkg%sFit_lead.root", fin_topdir_lead.c_str(),
                                                                                         option_model.c_str(),
                                                                                         option_bkg_fit_function.c_str()), "READ");
    
    TFile fin_effcorr_iron(Form("%s/EfficiencyCorrection_MnvGENIE%s_Bkg%sFit_iron.root", fin_topdir_iron.c_str(),
                                                                                         option_model.c_str(),
                                                                                         option_bkg_fit_function.c_str()), "READ");
    
    
    // Cross-section
    TFile fin_xsec_lead(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_lead.root", fin_topdir_lead.c_str(),
                                                                              option_model.c_str(),
                                                                              option_bkg_fit_function.c_str()), "READ");
    
    TFile fin_xsec_iron(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_iron.root", fin_topdir_iron.c_str(),
                                                                              option_model.c_str(),
                                                                              option_bkg_fit_function.c_str()), "READ");
    
    
    
    // =========================================
    //  Plot unfolding and following steps
    // =========================================
    
    // Unfolding
    std::cout << " \tPlotting unfolded distributions and migration matrices... " << std::endl;
    std::cout << std::endl;
    
    Plot_Unfolding(util, fin_prexsec_lead, fin_unfolding_lead, option_date_mc, "lead");
    Plot_Unfolding(util, fin_prexsec_iron, fin_unfolding_iron, option_date_mc, "iron");
    
    
    // Efficiency correction
    std::cout << " \tPlotting efficiency correction... " << std::endl;
    std::cout << std::endl;
    
    Plot_EffCorr(util, fin_prexsec_lead, fin_effcorr_lead, option_date_mc, "lead");
    Plot_EffCorr(util, fin_prexsec_iron, fin_effcorr_iron, option_date_mc, "iron");
    
    
    // Cross-section
    std::cout << " \tPlotting normalization and cross-section results... " << std::endl;
    std::cout << std::endl;
    
    Plot_Xsection(util, fin_xsec_lead, option_date_mc, "lead");
    Plot_Xsection(util, fin_xsec_iron, option_date_mc, "iron");
    
    
    // Close ROOT files
    fin_prexsec_lead.Close();
    fin_prexsec_iron.Close();
    
    fin_backgrsubtr_lead.Close();
    fin_backgrsubtr_iron.Close();
    
    fin_unfolding_lead.Close();
    fin_unfolding_iron.Close();
    
    fin_effcorr_lead.Close();
    fin_effcorr_iron.Close();
    
    fin_xsec_lead.Close();
    fin_xsec_iron.Close();
}


#endif  // PlotCrossSectionExtraction_C