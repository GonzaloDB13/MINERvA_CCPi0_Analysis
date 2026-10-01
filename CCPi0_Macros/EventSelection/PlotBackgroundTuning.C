#ifndef PlotBackgroundTuning_C
#define PlotBackgroundTuning_C

#include <iostream>
#include <vector>
#include <limits>

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
//  PLASTIC TUNING PLOTS
// ========================================================================================================================

void PlotPlasticTuning(CCPi0::MacroUtil util,
                       TFile& mc_fin,
                       TFile& data_fin,
                       std::string option_date_mc,
                       std::string option_material,
                       std::string before_or_after,
                       int Npars) 
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/PlasticTuning/plots/%s/%s/%s", option_date_mc.c_str(),
                                                                                                     option_material.c_str(),
                                                                                                     before_or_after.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Loop over variables to load histograms
    for ( auto var : variables ) {
        var -> LoadMCHists_PlasSB_In_SigReg(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_SigReg(data_fin);
        
        var -> LoadMCHists_PlasSB_In_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_PhysSB(data_fin);
        
        if ( before_or_after == "AfterTuning" ) {
            var -> LoadMCWeights_PlasBackgr_In_SigReg(mc_fin, util.m_error_bands);
            var -> LoadMCWeights_PlasBackgr_In_PhysSB(mc_fin, util.m_error_bands);
        }
    }
    
    
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
    
    
    // CV universe to use in MC stack histograms
    CVUniverse* cv_univ = (util.m_error_bands["cv"])[0];
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // Chi2 options for plotting
    double chi2  = -1.0;
    int chi2_ndf = -1;
    
    
    // =========================================
    //  Loop over variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Background label
        bool backgr_no_tuned   = false;
        bool plas_backgr_tuned = false;
        
        if ( before_or_after == "AfterTuning" ) {
            backgr_no_tuned   = false;
            plas_backgr_tuned = true;
        }
        else {
            backgr_no_tuned   = true;
            plas_backgr_tuned = false;
        }
        
        
        // MC weights (only after tuning)
        // ==============================
        
        if ( before_or_after == "AfterTuning" )
        {
            // Signal region
            PlotPlasticWeights(plot_info,
                               var->m_hists.m_mc_Weight_SigReg_TruePlasUp.hist,
                               var->m_hists.m_mc_Weight_SigReg_TruePlasBetw.hist,
                               var->m_hists.m_mc_Weight_SigReg_TruePlasDown.hist,
                               output_topdir + "/SigReg/Scale_" + before_or_after + "_" + option_material,
                               "Physics signal region" + material_title,
                               "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Plastic weight / Bin");
            
            // Charged pion sideband
            PlotPlasticWeights(plot_info,
                               var->m_hists.m_mc_Weight_PionBlobSB_TruePlasUp.hist,
                               var->m_hists.m_mc_Weight_PionBlobSB_TruePlasBetw.hist,
                               var->m_hists.m_mc_Weight_PionBlobSB_TruePlasDown.hist,
                               output_topdir + "/PionBlobSB/Scale_" + before_or_after + "_" + option_material,
                               "Charged pion sideband" + material_title,
                               "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Plastic weight / Bin ");
            
            // Proton sideband
            PlotPlasticWeights(plot_info,
                               var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasUp.hist,
                               var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasBetw.hist,
                               var->m_hists.m_mc_Weight_ProtonBlobSB_TruePlasDown.hist,
                               output_topdir + "/ProtonBlobSB/Scale_" + before_or_after + "_" + option_material,
                               "Proton sideband" + material_title,
                               "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Plastic weight / Bin ");
            
            // High-W sideband
            PlotPlasticWeights(plot_info,
                               var->m_hists.m_mc_Weight_HighWSB_TruePlasUp.hist,
                               var->m_hists.m_mc_Weight_HighWSB_TruePlasBetw.hist,
                               var->m_hists.m_mc_Weight_HighWSB_TruePlasDown.hist,
                               output_topdir + "/HighWSB/Scale_" + before_or_after + "_" + option_material,
                               "High-#font[12]{W} sideband" + material_title,
                               "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Plastic weight / Bin");
        }
        
        
        // Signal region
        // =============
        
        // Reconstructed Pb
        PlotDataMC(plot_info,
                   var->m_hists.m_data_RecoPb_In_SigReg, var->m_hists.m_mc_RecoPb_In_SigReg.hist,
                   output_topdir + "/SigReg/RecoPb_DataMC_" + before_or_after + "_" + option_material,
                   "Reco Pb in physics signal region" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_RecoPb_In_SigReg, var->m_hists.m_mc_RecoPb_In_SigReg.hist,
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoPb_In_SigReg_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/SigReg/RecoPb_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Reco Pb in physics signal region" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_RecoPb_In_SigReg, var->m_hists.m_mc_RecoPb_In_SigReg.hist,
                        output_topdir + "/SigReg/RecoPb_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Reco Pb in physics signal region" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_RecoPb_In_SigReg.hist,
                         output_topdir + "/SigReg/RecoPb_FracErrors_" + before_or_after + "_" + option_material,
                         "Reco Pb in physics signal region" + material_title, 0.4, true);
        
        
        // Reconstructed Fe
        PlotDataMC(plot_info,
                   var->m_hists.m_data_RecoFe_In_SigReg, var->m_hists.m_mc_RecoFe_In_SigReg.hist,
                   output_topdir + "/SigReg/RecoFe_DataMC_" + before_or_after + "_" + option_material,
                   "Reco Fe in physics signal region" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_RecoFe_In_SigReg, var->m_hists.m_mc_RecoFe_In_SigReg.hist,
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_RecoFe_In_SigReg_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/SigReg/RecoFe_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Reco Fe in physics signal region" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_RecoFe_In_SigReg, var->m_hists.m_mc_RecoFe_In_SigReg.hist,
                        output_topdir + "/SigReg/RecoFe_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Reco Fe in physics signal region" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_RecoFe_In_SigReg.hist,
                         output_topdir + "/SigReg/RecoFe_FracErrors_" + before_or_after + "_" + option_material,
                         "Reco Fe in physics signal region" + material_title, 0.4, true);
        
        
        // Plastic upstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasUpSB_In_SigReg, var->m_hists.m_mc_PlasUpSB_In_SigReg.hist,
                   output_topdir + "/SigReg/PlasUpSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic up. SB in physics signal region" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasUpSB_In_SigReg, var->m_hists.m_mc_PlasUpSB_In_SigReg.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_SigReg_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/SigReg/PlasUpSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic up. SB in physics signal region" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasUpSB_In_SigReg, var->m_hists.m_mc_PlasUpSB_In_SigReg.hist,
                        output_topdir + "/SigReg/PlasUpSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic up. SB in physics signal region" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasUpSB_In_SigReg.hist,
                         output_topdir + "/SigReg/PlasUpSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic up. SB in physics signal region" + material_title, 0.4, true);
        
        
        // Plastic between sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasBetwSB_In_SigReg, var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist,
                   output_topdir + "/SigReg/PlasBetwSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic betw. SB in physics signal region" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasBetwSB_In_SigReg, var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/SigReg/PlasBetwSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic betw. SB in physics signal region" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasBetwSB_In_SigReg, var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist,
                        output_topdir + "/SigReg/PlasBetwSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic betw. SB in physics signal region" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasBetwSB_In_SigReg.hist,
                         output_topdir + "/SigReg/PlasBetwSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic betw. SB in physics signal region" + material_title, 0.4, true);
        
        
        // Plastic downstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasDownSB_In_SigReg, var->m_hists.m_mc_PlasDownSB_In_SigReg.hist,
                   output_topdir + "/SigReg/PlasDownSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic down. SB in physics signal region" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasDownSB_In_SigReg, var->m_hists.m_mc_PlasDownSB_In_SigReg.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_SigReg_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/SigReg/PlasDownSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic down. SB in physics signal region" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasDownSB_In_SigReg, var->m_hists.m_mc_PlasDownSB_In_SigReg.hist,
                        output_topdir + "/SigReg/PlasDownSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic down. SB in physics signal region" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasDownSB_In_SigReg.hist,
                         output_topdir + "/SigReg/PlasDownSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic down. SB in physics signal region" + material_title, 0.4, true);
        
        
        // Charged pion sideband
        // =====================
        
        // Plastic upstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasUpSB_In_PionBlobSB, var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist,
                   output_topdir + "/PionBlobSB/PlasUpSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic up. SB in physics charged pion SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasUpSB_In_PionBlobSB, var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/PionBlobSB/PlasUpSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic up. SB in physics charged pion SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasUpSB_In_PionBlobSB, var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist,
                        output_topdir + "/PionBlobSB/PlasUpSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic up. SB in physics charged pion SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasUpSB_In_PionBlobSB.hist,
                         output_topdir + "/PionBlobSB/PlasUpSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic up. SB in physics charged pion SB" + material_title, 0.4, true);
        
        
        // Plastic between sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasBetwSB_In_PionBlobSB, var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist,
                   output_topdir + "/PionBlobSB/PlasBetwSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic betw. SB in physics charged pion SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasBetwSB_In_PionBlobSB, var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/PionBlobSB/PlasBetwSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic betw. SB in physics charged pion SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasBetwSB_In_PionBlobSB, var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist,
                        output_topdir + "/PionBlobSB/PlasBetwSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic betw. SB in physics charged pion SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasBetwSB_In_PionBlobSB.hist,
                         output_topdir + "/PionBlobSB/PlasBetwSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic betw. SB in physics charged pion SB" + material_title, 0.4, true);
        
        
        // Plastic downstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasDownSB_In_PionBlobSB, var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist,
                   output_topdir + "/PionBlobSB/PlasDownSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic down. SB in physics charged pion SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasDownSB_In_PionBlobSB, var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/PionBlobSB/PlasDownSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic down. SB in physics charged pion SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasDownSB_In_PionBlobSB, var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist,
                        output_topdir + "/PionBlobSB/PlasDownSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic down. SB in physics charged pion SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasDownSB_In_PionBlobSB.hist,
                         output_topdir + "/PionBlobSB/PlasDownSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic down. SB in physics charged pion SB" + material_title, 0.4, true);
        
        
        // Proton sideband
        // ===============
        
        // Plastic upstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasUpSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist,
                   output_topdir + "/ProtonBlobSB/PlasUpSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic up. SB in physics proton SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasUpSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/ProtonBlobSB/PlasUpSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic up. SB in physics proton SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasUpSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist,
                        output_topdir + "/ProtonBlobSB/PlasUpSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic up. SB in physics proton SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.hist,
                         output_topdir + "/ProtonBlobSB/PlasUpSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic up. SB in physics proton SB" + material_title, 0.4, true);
        
        
        // Plastic between sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasBetwSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist,
                   output_topdir + "/ProtonBlobSB/PlasBetwSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic betw. SB in physics proton SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasBetwSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/ProtonBlobSB/PlasBetwSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic betw. SB in physics proton SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasBetwSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist,
                        output_topdir + "/ProtonBlobSB/PlasBetwSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic betw. SB in physics proton SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.hist,
                         output_topdir + "/ProtonBlobSB/PlasBetwSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic betw. SB in physics proton SB" + material_title, 0.4, true);
        
        
        // Plastic downstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasDownSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist,
                   output_topdir + "/ProtonBlobSB/PlasDownSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic down. SB in physics proton SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasDownSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/ProtonBlobSB/PlasDownSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic down. SB in physics proton SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasDownSB_In_ProtonBlobSB, var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist,
                        output_topdir + "/ProtonBlobSB/PlasDownSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic down. SB in physics proton SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.hist,
                         output_topdir + "/ProtonBlobSB/PlasDownSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic down. SB in physics proton SB" + material_title, 0.4, true);
        
        
        // High-W sideband
        // ===============
        
        // Plastic upstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasUpSB_In_HighWSB, var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist,
                   output_topdir + "/HighWSB/PlasUpSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic up. SB in physics high-#font[12]{W} SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasUpSB_In_HighWSB, var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/HighWSB/PlasUpSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic up. SB in physics high-#font[12]{W} SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasUpSB_In_HighWSB, var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist,
                        output_topdir + "/HighWSB/PlasUpSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic up. SB in physics high-#font[12]{W} SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasUpSB_In_HighWSB.hist,
                         output_topdir + "/HighWSB/PlasUpSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic up. SB in physics high-#font[12]{W} SB" + material_title, 0.4, true);
        
        
        // Plastic between sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasBetwSB_In_HighWSB, var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist,
                   output_topdir + "/HighWSB/PlasBetwSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic betw. SB in physics high-#font[12]{W} SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasBetwSB_In_HighWSB, var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/HighWSB/PlasBetwSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic betw. SB in physics high-#font[12]{W} SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasBetwSB_In_HighWSB, var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist,
                        output_topdir + "/HighWSB/PlasBetwSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic betw. SB in physics high-#font[12]{W} SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasBetwSB_In_HighWSB.hist,
                         output_topdir + "/HighWSB/PlasBetwSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic betw. SB in physics high-#font[12]{W} SB" + material_title, 0.4, true);
        
        
        // Plastic downstream sideband
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PlasDownSB_In_HighWSB, var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist,
                   output_topdir + "/HighWSB/PlasDownSB_DataMC_" + before_or_after + "_" + option_material,
                   "Plastic down. SB in physics high-#font[12]{W} SB" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, false, backgr_no_tuned, plas_backgr_tuned, false, false, false, false,
                   true, false, chi2, Npars, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PlasDownSB_In_HighWSB, var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.univHist(cv_univ),
                          output_topdir + "/HighWSB/PlasDownSB_DataStackedMC_" + before_or_after + "_" + option_material,
                          "Plastic down. SB in physics high-#font[12]{W} SB" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, false, backgr_no_tuned, plas_backgr_tuned, false, false, false,
                          true, false, chi2, Npars, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PlasDownSB_In_HighWSB, var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist,
                        output_topdir + "/HighWSB/PlasDownSB_DataMCRatio_" + before_or_after + "_" + option_material,
                        "Plastic down. SB in physics high-#font[12]{W} SB" + material_title,
                        "", "", 0.0, 3.0,
                        true, true, false, false, backgr_no_tuned, plas_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PlasDownSB_In_HighWSB.hist,
                         output_topdir + "/HighWSB/PlasDownSB_FracErrors_" + before_or_after + "_" + option_material,
                         "Plastic down. SB in physics high-#font[12]{W} SB" + material_title, 0.4, true);
    }
}





// ========================================================================================================================
//  PHYSICS TUNING PLOTS
// ========================================================================================================================

void PlotPhysicsTuning(CCPi0::MacroUtil util,
                       TFile& mc_fin,
                       TFile& data_fin,
                       std::string option_date_mc,
                       std::string option_material,
                       std::string before_or_after,
                       std::string option_physfit_function,
                       int Npars)
{
    // Construct plot output directory
    std::string output_topdir;
    output_topdir = Form("/minerva/data/users/gonzalo/MAT/PhysicsTuning/plots/%s/%s/%s", option_date_mc.c_str(),
                                                                                         option_material.c_str(),
                                                                                         before_or_after.c_str());
                                                                                                     
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Loop over variables to load histograms
    for ( auto var : variables ) {
        var -> LoadMCHists_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PhysSB(data_fin);
        
        if ( before_or_after == "AfterPhysBackgrTuning" || before_or_after == "AfterSignalTuning" )
            var -> LoadMCWeights_PhysBackgr(mc_fin, util.m_error_bands);
    }
    
    
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
    
    
    // CV universe to use in MC stack histograms
    CVUniverse* cv_univ = (util.m_error_bands["cv"])[0];
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    
    // =========================================
    //  Loop over variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Construct plot info object
        // ==========================
        
        PlotInfo plot_info(var, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Background label
        bool signal_tuned      = false;
        bool backgr_no_tuned   = false;
        bool plas_backgr_tuned = false;
        bool all_backgr_tuned  = false;
        
        if ( before_or_after == "BeforeTuning" ) {
            signal_tuned      = false;
            backgr_no_tuned   = true;
            plas_backgr_tuned = false;
            all_backgr_tuned  = false;
        }
        else if ( before_or_after == "AfterPlasticTuning" ) {
            signal_tuned      = false;
            backgr_no_tuned   = false;
            plas_backgr_tuned = true;
            all_backgr_tuned  = false;
        }
        else if ( before_or_after == "AfterPhysBackgrTuning" ) {
            signal_tuned      = false;
            backgr_no_tuned   = false;
            plas_backgr_tuned = false;
            all_backgr_tuned  = true;
        }
        else if ( before_or_after == "AfterSignalTuning" ) {
            signal_tuned      = true;
            backgr_no_tuned   = false;
            plas_backgr_tuned = false;
            all_backgr_tuned  = true;
        }
        
        
        // Generalized chi2 info
        // =====================
        // Don't use under and overflow bins according to MnvPlotter::Chi2DataMC()
        
        double chi2 = 0.0;
        int ndf_SigReg       = 0;
        int ndf_PionBlobSB   = 0;
        int ndf_ProtonBlobSB = 0;
        int ndf_HighWSB      = 0;
        
        // Calculate data-MC chi2
        chi2 += CalculateChi2DataMC(plot_info, var->m_hists.m_data_SigReg,       var->m_hists.m_mc_SigReg.hist,       ndf_SigReg);
        chi2 += CalculateChi2DataMC(plot_info, var->m_hists.m_data_PionBlobSB,   var->m_hists.m_mc_PionBlobSB.hist,   ndf_PionBlobSB);
        chi2 += CalculateChi2DataMC(plot_info, var->m_hists.m_data_ProtonBlobSB, var->m_hists.m_mc_ProtonBlobSB.hist, ndf_ProtonBlobSB);
        chi2 += CalculateChi2DataMC(plot_info, var->m_hists.m_data_HighWSB,      var->m_hists.m_mc_HighWSB.hist,      ndf_HighWSB);
        
        // Get total number of d.o.f
        int chi2_ndf = ndf_SigReg + ndf_PionBlobSB + ndf_ProtonBlobSB + ndf_HighWSB - Npars;
        
        
        // MC weights (only after physics tuning)
        // ======================================
        
        if ( before_or_after == "AfterPhysBackgrTuning" || before_or_after == "AfterSignalTuning" )
        {
            PlotPhysicsWeights(plot_info,
                               var->m_hists.m_mc_Weight_Signal.hist,
                               var->m_hists.m_mc_Weight_BackgrPi0HighW.hist,
                               var->m_hists.m_mc_Weight_BackgrQElike.hist,
                               var->m_hists.m_mc_Weight_BackgrPionProd.hist,
                               output_topdir + "/Scale_" + before_or_after + "_" + option_material,
                               "Physics scale functions" + material_title,
                               "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Physics weight / Bin");
        }
        
        
        // Signal region
        // =============
        
        if ( before_or_after == "BeforeTuning" || before_or_after == "AfterPlasticTuning" || before_or_after == "AfterPhysBackgrTuning" )
        {
            PlotDataMC(plot_info,
                       var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                       output_topdir + "/SigReg/DataMC_" + before_or_after + "_" + option_material,
                       "Physics signal region" + material_title,
                       "", "", "TR", -1.0, -1.0,
                       true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false, false,
                       true, true, chi2, -1, chi2_ndf);
            
            PlotDataStackedMC(plot_info,
                              var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                              (MnvH1D*)var->m_hists.m_mc_SigReg_Signal.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPi0HighW.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrQElike.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPionProd.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasUp.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasBetw.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasDown.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrOther.univHist(cv_univ),
                              output_topdir + "/SigReg/DataStackedMC_" + before_or_after + "_" + option_material,
                              "Physics signal region" + material_title,
                              "", "", "TR", -1.0, -1.0,
                              true, true, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false,
                              true, true, chi2, -1, chi2_ndf);
            
            PlotDataMCRatio(plot_info,
                            var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                            output_topdir + "/SigReg/DataMCRatio_" + before_or_after + "_" + option_material,
                            "Physics signal region" + material_title,
                            "", "", 0.0, 2.0,
                            true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned);
        }
        
        else if ( before_or_after == "AfterSignalTuning" )
        {
            PlotDataMC(plot_info,
                       var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                       output_topdir + "/SigReg/DataMC_" + before_or_after + "_" + option_material,
                       "Physics signal region" + material_title,
                       "", "", "TR", -1.0, -1.0,
                       true, true, false, false, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false, false,
                       true, true, chi2, -1, chi2_ndf);
            
            PlotDataStackedMC(plot_info,
                              var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                              (MnvH1D*)var->m_hists.m_mc_SigReg_Signal.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPi0HighW.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrQElike.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPionProd.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasUp.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasBetw.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrPlasDown.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_SigReg_BackgrOther.univHist(cv_univ),
                              output_topdir + "/SigReg/DataStackedMC_" + before_or_after + "_" + option_material,
                              "Physics signal region" + material_title,
                              "", "", "TR", -1.0, -1.0,
                              true, true, false, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false,
                              true, true, chi2, -1, chi2_ndf);
            
            PlotDataMCRatio(plot_info,
                            var->m_hists.m_data_SigReg, var->m_hists.m_mc_SigReg.hist,
                            output_topdir + "/SigReg/DataMCRatio_" + before_or_after + "_" + option_material,
                            "Physics signal region" + material_title,
                            "", "", 0.0, 2.0,
                            true, true, false, false, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned);
        }
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_SigReg.hist,
                         output_topdir + "/SigReg/FracErrors_" + before_or_after + "_" + option_material,
                         "Physics signal region" + material_title, 0.4, true);
        
        
        // Charged pion sideband
        // =====================
        
        PlotDataMC(plot_info,
                   var->m_hists.m_data_PionBlobSB, var->m_hists.m_mc_PionBlobSB.hist,
                   output_topdir + "/PionBlobSB/DataMC_" + before_or_after + "_" + option_material,
                   "Charged pion sideband" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false, false,
                   true, true, chi2, -1, chi2_ndf);
        
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_PionBlobSB, var->m_hists.m_mc_PionBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_Signal.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrPi0HighW.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrQElike.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrPionProd.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrPlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrPlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrPlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_PionBlobSB_BackgrOther.univHist(cv_univ),
                          output_topdir + "/PionBlobSB/DataStackedMC_" + before_or_after + "_" + option_material,
                          "Charged pion sideband" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false,
                          true, true, chi2, -1, chi2_ndf);
        
        PlotDataMCRatio(plot_info,
                        var->m_hists.m_data_PionBlobSB, var->m_hists.m_mc_PionBlobSB.hist,
                        output_topdir + "/PionBlobSB/DataMCRatio_" + before_or_after + "_" + option_material,
                        "Charged pion sideband" + material_title,
                        "", "", 0.0, 2.0,
                        true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_PionBlobSB.hist,
                         output_topdir + "/PionBlobSB/FracErrors_" + before_or_after + "_" + option_material,
                         "Charged pion sideband" + material_title, 0.4, true);
        
        
        // Proton sideband
        // ===============
        
        PlotDataMC(plot_info, var->m_hists.m_data_ProtonBlobSB, var->m_hists.m_mc_ProtonBlobSB.hist,
                   output_topdir + "/ProtonBlobSB/DataMC_" + before_or_after + "_" + option_material,
                   "Proton sideband" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false, false,
                   true, true, chi2, -1, chi2_ndf);
        
        PlotDataStackedMC(plot_info, var->m_hists.m_data_ProtonBlobSB, var->m_hists.m_mc_ProtonBlobSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_Signal.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrQElike.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrPionProd.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_ProtonBlobSB_BackgrOther.univHist(cv_univ),
                          output_topdir + "/ProtonBlobSB/DataStackedMC_" + before_or_after + "_" + option_material,
                          "Proton sideband" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false,
                          true, true, chi2, -1, chi2_ndf);
        
        PlotDataMCRatio(plot_info, var->m_hists.m_data_ProtonBlobSB, var->m_hists.m_mc_ProtonBlobSB.hist,
                        output_topdir + "/ProtonBlobSB/DataMCRatio_" + before_or_after + "_" + option_material,
                        "Proton sideband" + material_title,
                        "", "", 0.0, 2.0,
                        true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_ProtonBlobSB.hist,
                         output_topdir + "/ProtonBlobSB/FracErrors_" + before_or_after + "_" + option_material,
                         "Proton sideband" + material_title, 0.4, true);
        
        
        // High-W sideband
        // ===============
        
        PlotDataMC(plot_info, var->m_hists.m_data_HighWSB, var->m_hists.m_mc_HighWSB.hist,
                   output_topdir + "/HighWSB/DataMC_" + before_or_after + "_" + option_material,
                   "High-#font[12]{W} sideband" + material_title,
                   "", "", "TR", -1.0, -1.0,
                   true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false, false,
                   true, true, chi2, -1, chi2_ndf);
        
        PlotDataStackedMC(plot_info, var->m_hists.m_data_HighWSB, var->m_hists.m_mc_HighWSB.hist,
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_Signal.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrPi0HighW.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrQElike.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrPionProd.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrPlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrPlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrPlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_HighWSB_BackgrOther.univHist(cv_univ),
                          output_topdir + "/HighWSB/DataStackedMC_" + before_or_after + "_" + option_material,
                          "High-#font[12]{W} sideband" + material_title,
                          "", "", "TR", -1.0, -1.0,
                          true, true, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned, false, false,
                          true, true, chi2, -1, chi2_ndf);
        
        PlotDataMCRatio(plot_info, var->m_hists.m_data_HighWSB, var->m_hists.m_mc_HighWSB.hist,
                        output_topdir + "/HighWSB/DataMCRatio_" + before_or_after + "_" + option_material,
                        "High-#font[12]{W} sideband" + material_title,
                        "", "", 0.0, 2.0,
                        true, true, false, signal_tuned, backgr_no_tuned, plas_backgr_tuned, all_backgr_tuned);
        
        PlotErrorSummary(plot_info,
                         var->m_hists.m_mc_HighWSB.hist,
                         output_topdir + "/HighWSB/FracErrors_" + before_or_after + "_" + option_material,
                         "High-#font[12]{W} sideband" + material_title, 0.4, true);
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotBackgroundTuning(std::string option_date,
                          std::string option_model            = "v1",
                          std::string option_physfit_function = "Linear",
                          bool do_systematics                 = true)
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
    
    // MC plastic tuning on LEAD
    std::string mc_fin_plastun_before_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/lead/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_plastun_after_lead_topdir  = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/lead/AfterTuning",  option_date_mc.c_str());
    
    
    // MC plastic tuning on IRON
    std::string mc_fin_plastun_before_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/iron/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_plastun_after_iron_topdir  = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/iron/AfterTuning",  option_date_mc.c_str());
    
    
    // MC physics tuning on LEAD
    std::string mc_fin_phystun_before_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/lead/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_phystun_after_lead_topdir  = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/lead/AfterTuning",  option_date_mc.c_str());
    
    
    // MC physics tuning on IRON
    std::string mc_fin_phystun_before_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/iron/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_phystun_after_iron_topdir  = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/iron/AfterTuning",  option_date_mc.c_str());
    
    
    // Data plastic distributions on LEAD
    std::string data_fin_plastun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/data/%s/lead", option_date_data.c_str());
    
    
    // Data plastic distributions on IRON
    std::string data_fin_plastun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/data/%s/iron", option_date_data.c_str());
    
    
    // Data physics distributions on LEAD
    std::string data_fin_phystun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/data/%s/lead", option_date_data.c_str());
    
    
    // Data physics distributions on IRON
    std::string data_fin_phystun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/data/%s/iron", option_date_data.c_str());
    
    
    // Input files
    // ===========
    
    // MC plastic tuning on LEAD
    TFile mc_fin_plastun_before_lead(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_plastun_before_lead_topdir.c_str(),
                                                                                                                      option_model.c_str(),
                                                                                                                      option_systematics.c_str()), "READ");
    
    TFile mc_fin_plastun_after_lead(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_plastun_after_lead_topdir.c_str(),
                                                                                                                    option_model.c_str(),
                                                                                                                    option_systematics.c_str()), "READ");
        
    
    // MC plastic tuning on IRON
    TFile mc_fin_plastun_before_iron(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_plastun_before_iron_topdir.c_str(),
                                                                                                                      option_model.c_str(),
                                                                                                                      option_systematics.c_str()), "READ");
    
    TFile mc_fin_plastun_after_iron(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_plastun_after_iron_topdir.c_str(),
                                                                                                                    option_model.c_str(),
                                                                                                                    option_systematics.c_str()), "READ");
    
    
    // MC physics tuning on LEAD
    TFile mc_fin_phystun_before_lead(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_phystun_before_lead_topdir.c_str(),
                                                                                                                      option_model.c_str(),
                                                                                                                      option_systematics.c_str()), "READ");
    
    TFile mc_fin_phystun_afterplas_lead(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_phystun_after_lead_topdir.c_str(),
                                                                                                                        option_model.c_str(),
                                                                                                                        option_systematics.c_str()), "READ");
    
    TFile mc_fin_phystun_afterphys_lead(Form("%s/MC_AfterPhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_lead.root", mc_fin_phystun_after_lead_topdir.c_str(),
                                                                                                                              option_model.c_str(),
                                                                                                                              option_systematics.c_str(),
                                                                                                                              option_physfit_function.c_str()), "READ");
    
    TFile mc_fin_phystun_afterphys_sigtun_lead(Form("%s/MC_AfterPhysicsTuningWithSignalTune_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_lead.root", mc_fin_phystun_after_lead_topdir.c_str(),
                                                                                                                                                   option_model.c_str(),
                                                                                                                                                   option_systematics.c_str(),
                                                                                                                                                   option_physfit_function.c_str()), "READ");
    
    
    // MC physics tuning on IRON
    TFile mc_fin_phystun_before_iron(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_phystun_before_iron_topdir.c_str(),
                                                                                                                      option_model.c_str(),
                                                                                                                      option_systematics.c_str()), "READ");
    
    TFile mc_fin_phystun_afterplas_iron(Form("%s/MC_AfterPlasticTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_phystun_after_iron_topdir.c_str(),
                                                                                                                        option_model.c_str(),
                                                                                                                        option_systematics.c_str()), "READ");
    
    TFile mc_fin_phystun_afterphys_iron(Form("%s/MC_AfterPhysicsTuning_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_iron.root", mc_fin_phystun_after_iron_topdir.c_str(),
                                                                                                                              option_model.c_str(),
                                                                                                                              option_systematics.c_str(),
                                                                                                                              option_physfit_function.c_str()), "READ");
    
    TFile mc_fin_phystun_afterphys_sigtun_iron(Form("%s/MC_AfterPhysicsTuningWithSignalTune_MnvGENIE%s_%s_POTScaled_AllPlaylists_%sFit_iron.root", mc_fin_phystun_after_iron_topdir.c_str(),
                                                                                                                                                   option_model.c_str(),
                                                                                                                                                   option_systematics.c_str(),
                                                                                                                                                   option_physfit_function.c_str()), "READ");
    
    
    // Data plastic distributions on LEAD
    TFile data_fin_plastun_lead(Form("%s/Data_PlasticTuning_AllPlaylists_lead.root", data_fin_plastun_lead_topdir.c_str()), "READ");
    
    
    // Data plastic distributions on IRON
    TFile data_fin_plastun_iron(Form("%s/Data_PlasticTuning_AllPlaylists_iron.root", data_fin_plastun_iron_topdir.c_str()), "READ");
    
    
    // Data physics distributions on LEAD
    TFile data_fin_phystun_lead(Form("%s/Data_PhysicsTuning_AllPlaylists_lead.root", data_fin_phystun_lead_topdir.c_str()), "READ");
    
    
    // Data physics distributions on IRON
    TFile data_fin_phystun_iron(Form("%s/Data_PhysicsTuning_AllPlaylists_iron.root", data_fin_phystun_iron_topdir.c_str()), "READ");
    
    
    
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
    double mc_pot_plastun_lead   = GetPOT(mc_fin_plastun_after_lead, true);
    double mc_pot_plastun_iron   = GetPOT(mc_fin_plastun_after_iron, true);
    double data_pot_plastun_lead = GetPOT(data_fin_plastun_lead,     false);
    double data_pot_plastun_iron = GetPOT(data_fin_plastun_iron,     false);
    
    if ( std::fabs(mc_pot_plastun_lead   - mc_pot_plastun_iron)   > std::numeric_limits<double>::epsilon() ||
         std::fabs(data_pot_plastun_lead - data_pot_plastun_iron) > std::numeric_limits<double>::epsilon() ) {
        std::cout << std::endl;
        std::cout << " WARNING: Plastic tuning POT in lead and iron files are not the same. " << std::endl;
        std::cout << std::endl;
    }
    
    double mc_pot_phystun_lead   = GetPOT(mc_fin_phystun_afterphys_lead, true);
    double mc_pot_phystun_iron   = GetPOT(mc_fin_phystun_afterphys_iron, true);
    double data_pot_phystun_lead = GetPOT(data_fin_phystun_lead,         false);
    double data_pot_phystun_iron = GetPOT(data_fin_phystun_iron,         false);
    
    if ( std::fabs(mc_pot_phystun_lead   - mc_pot_phystun_iron)   > std::numeric_limits<double>::epsilon() ||
         std::fabs(data_pot_phystun_lead - data_pot_phystun_iron) > std::numeric_limits<double>::epsilon() ) {
        std::cout << std::endl;
        std::cout << " WARNING: Physics tuning POT in lead and iron files are not the same. Setting MacroUtil POT using lead file. " << std::endl;
        std::cout << std::endl;
    }
    
    SetMacroUtilPOT(mc_fin_phystun_afterphys_lead, data_fin_phystun_lead, util);
    
    std::cout << std::endl;
    std::cout << " Plastic tuning: " << std::endl;
    std::cout << " --------------  " << std::endl;
    std::cout << " Lead: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_plastun_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_plastun_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_plastun_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_plastun_iron << std::endl;
    std::cout << std::endl;
    std::cout << " Physics tuning: " << std::endl;
    std::cout << " --------------  " << std::endl;
    std::cout << " Lead: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_phystun_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_phystun_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_phystun_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_phystun_iron << std::endl;
    std::cout << std::endl;
    
    
    // Text file with POT info
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/POTinfo/%s", option_date_mc.c_str());
    
    std::ofstream text_pot(Form("%s/POT_BackgroundTuning_AllPlaylists.txt", text_topdir.c_str()));
    
    text_pot << std::endl;
    text_pot << " ALL PLAYLISTS " << std::endl;
    text_pot << std::endl;
    text_pot << " Plastic tuning: " << std::endl;
    text_pot << " --------------  " << std::endl;
    text_pot << " Lead: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_plastun_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_plastun_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_plastun_iron   << std::endl;
    text_pot << " \tData POT: " << data_pot_plastun_iron << std::endl;
    text_pot << std::endl;
    text_pot << " Physics tuning: " << std::endl;
    text_pot << " --------------  " << std::endl;
    text_pot << " Lead: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_phystun_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_phystun_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_phystun_iron   << std::endl;
    text_pot << " \tData POT: " << data_pot_phystun_iron << std::endl;
    text_pot << std::endl;
    
    text_pot.close();
    
    
    
    // =========================================
    //  Plot functions
    // =========================================
    
    std::cout << " Plotting background tuning... " << std::endl;
    std::cout << std::endl;
    
    
    // Fit parameters
    int Npars_before = 0;
    
    int Npars_after_plastun = 2;
    
    int Npars_after_phystun;
    if ( option_physfit_function == "Scalar" )          Npars_after_phystun = 4;
    else if ( option_physfit_function == "Linear" )     Npars_after_phystun = 8;
    else if ( option_physfit_function == "LnearAlt" )   Npars_after_phystun = 12;
    else if ( option_physfit_function == "Bilinear" )   Npars_after_phystun = 12;
    else if ( option_physfit_function == "BilinerAlt" ) Npars_after_phystun = 16;
    
    
    // Plastic tuning
    // ==============
    
    // Before tuning
    std::cout << " \tPlotting material distributions before plastic tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPlasticTuning(util, mc_fin_plastun_before_lead, data_fin_plastun_lead,
                      option_date_mc, "lead", "BeforeTuning", Npars_before);
    
    PlotPlasticTuning(util, mc_fin_plastun_before_iron, data_fin_plastun_iron,
                      option_date_mc, "iron", "BeforeTuning", Npars_before);
    
    
    // After tuning
    std::cout << " \tPlotting material distributions after plastic tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPlasticTuning(util, mc_fin_plastun_after_lead, data_fin_plastun_lead,
                      option_date_mc, "lead", "AfterTuning", Npars_after_plastun);
    
    PlotPlasticTuning(util, mc_fin_plastun_after_iron, data_fin_plastun_iron,
                      option_date_mc, "iron", "AfterTuning", Npars_after_plastun);
    
    
    // Physics tuning
    // ==============
    
    // Before tuning
    std::cout << " \tPlotting signal region and physics sidebands before any tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPhysicsTuning(util, mc_fin_phystun_before_lead, data_fin_phystun_lead,
                      option_date_mc, "lead", "BeforeTuning",
                      option_physfit_function, Npars_before);
    
    PlotPhysicsTuning(util, mc_fin_phystun_before_iron, data_fin_phystun_iron,
                      option_date_mc, "iron", "BeforeTuning",
                      option_physfit_function, Npars_before);
    
    
    // After plastic tuning
    std::cout << " \tPlotting signal region and physics sidebands after plastic tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterplas_lead, data_fin_phystun_lead,
                      option_date_mc, "lead", "AfterPlasticTuning",
                      option_physfit_function, Npars_before);
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterplas_iron, data_fin_phystun_iron,
                      option_date_mc, "iron", "AfterPlasticTuning",
                      option_physfit_function, Npars_before);
    
    
    // After physics background tuning
    std::cout << " \tPlotting signal region and physics sidebands after physics background tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterphys_lead, data_fin_phystun_lead,
                      option_date_mc, "lead", "AfterPhysBackgrTuning",
                      option_physfit_function, Npars_after_phystun);
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterphys_iron, data_fin_phystun_iron,
                      option_date_mc, "iron", "AfterPhysBackgrTuning",
                      option_physfit_function, Npars_after_phystun);
    
    
    // After physics background + signal tuning
    std::cout << " \tPlotting signal region and physics sidebands after signal tuning... " << std::endl;
    std::cout << std::endl;
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterphys_sigtun_lead, data_fin_phystun_lead,
                      option_date_mc, "lead", "AfterSignalTuning",
                      option_physfit_function, Npars_after_phystun);
    
    PlotPhysicsTuning(util, mc_fin_phystun_afterphys_sigtun_iron, data_fin_phystun_iron,
                      option_date_mc, "iron", "AfterSignalTuning",
                      option_physfit_function, Npars_after_phystun);
    
    
    // Close ROOT files
    mc_fin_plastun_before_lead.Close();
    mc_fin_plastun_before_iron.Close();
    
    mc_fin_plastun_after_lead.Close();
    mc_fin_plastun_after_iron.Close();
    
    data_fin_plastun_lead.Close();
    data_fin_plastun_iron.Close();
    
    mc_fin_phystun_before_lead.Close();
    mc_fin_phystun_before_iron.Close();
    
    mc_fin_phystun_afterplas_lead.Close();
    mc_fin_phystun_afterplas_iron.Close();
    
    mc_fin_phystun_afterphys_iron.Close();
    mc_fin_phystun_afterphys_iron.Close();
    
    mc_fin_phystun_afterphys_sigtun_lead.Close();
    mc_fin_phystun_afterphys_sigtun_iron.Close();
    
    data_fin_phystun_lead.Close();
    data_fin_phystun_iron.Close();
}


#endif  // PlotBackgroundTuning_C