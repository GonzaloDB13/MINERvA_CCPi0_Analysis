#ifndef PlotWarping_C
#define PlotWarping_C

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
#include "TProfile.h"





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotTransWarpOutputs(std::string option_date,
                          std::string option_model_baseline,
                          std::string option_model_fakedata,
                          std::string option_material)
{
    // Get MC model for MacroUtil based on input
    EnumModels type_model;
    GetModel(option_model_baseline, type_model);
    
    
    // Options for input file
    const std::string option_date_mc = option_date + "_" + option_model_baseline;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Warping results
    std::string fin_warp_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/WarpingStudies/%s/%s/TransWarpOutputs", option_date_mc.c_str(),
                                                                                                                           option_material.c_str());
    
    TFile fin_warp(Form("%s/Warping_MC_%s_FakeData_%s_%s.root", fin_warp_topdir.c_str(),
                                                                option_model_baseline.c_str(),
                                                                option_model_fakedata.c_str(),
                                                                option_material.c_str()), "READ");
    
    
    // POT file
    std::string fin_pot_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/WarpingStudies/%s/%s/TransWarpInputs", option_date_mc.c_str(),
                                                                                                                         option_material.c_str());
    TFile fin_pot(Form("%s/BaselineMC_%s_FakeData_%s_%s.root", fin_pot_topdir.c_str(),
                                                               option_model_baseline.c_str(),
                                                               option_model_fakedata.c_str(),
                                                               option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  MacroUtil and variables
    // =========================================
    
    // Playlist string
    // (Use only to load MC chain and access systematics)
    const std::string plist_string = "minervame1A";
    
    
    // Set playlists MC and data input
    // (Similarly, only to load MC and access systematics)
    const std::string mc_file_list   = GetPlaylistFile(true,  plist_string, "test");
    const std::string data_file_list = GetPlaylistFile(false, plist_string, "test");
    
    
    // Set MacroUtil
    // (SYSTEMATICS and TRUTH options set as 'true')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, true, type_model);
    
    
    // Set MacroUtil POT
    SetMacroUtilPOT(fin_pot, util);
    
    std::cout << std::endl;
    std::cout << "Plotting warping results... " << std:: endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << " \tData POT: " << util.m_data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    
    // =========================================
    //  Plot warping results
    // =========================================
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Get variable name
        std::string var_name = var->Name();
        
        
        // Plot output topdir
        std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/WarpingStudies/plots/%s/%s/FakeData%s", option_date_mc.c_str(),
                                                                                                                  option_material.c_str(),
                                                                                                                  option_model_fakedata.c_str());
        
        
        // Get fake data histograms
        PlotUtils::MnvH1D* mnvh_data = (PlotUtils::MnvH1D*)fin_warp.Get("Input_Hists/h_data");
        TH1D* h_data = (TH1D*)mnvh_data->GetCVHistoWithStatError().Clone("h_data");
        h_data -> SetTitle("Folded fake data");
        PlotUtils::MnvH1D* h_fake_data = new PlotUtils::MnvH1D(*h_data);
        
        PlotUtils::MnvH1D* mnvh_data_truth = (PlotUtils::MnvH1D*)fin_warp.Get("Input_Hists/h_data_truth");
        TH1D* h_data_truth = (TH1D*)mnvh_data_truth->GetCVHistoWithStatError().Clone("h_data_truth");
        h_data_truth -> SetTitle("True fake data");
        PlotUtils::MnvH1D* h_fake_data_truth = new PlotUtils::MnvH1D(*h_data_truth);
        
        
        // Get baseline MC histograms
        PlotUtils::MnvH1D* mnvh_mc_reco = (PlotUtils::MnvH1D*)fin_warp.Get("Input_Hists/h_mc_reco");
        TH1D* h_mc_reco = (TH1D*)mnvh_mc_reco->GetCVHistoWithStatError().Clone("h_mc_reco");
        h_mc_reco -> SetTitle("Reco simulation");
        PlotUtils::MnvH1D* h_model_reco = new PlotUtils::MnvH1D(*h_mc_reco);
        
        PlotUtils::MnvH1D* mnvh_mc_truth = (PlotUtils::MnvH1D*)fin_warp.Get("Input_Hists/h_mc_truth");
        TH1D* h_mc_truth = (TH1D*)mnvh_mc_truth->GetCVHistoWithStatError().Clone("h_mc_truth");
        h_mc_truth -> SetTitle("True simulation");
        PlotUtils::MnvH1D* h_model_truth = new PlotUtils::MnvH1D(*h_mc_truth);
        
        
        // Number of degrees of freedom
        int ndf = h_fake_data->GetNbinsX();
        
        
        // Get chi2 histograms
        PlotUtils::MnvH2D* h_chi2        = (PlotUtils::MnvH2D*)fin_warp.Get("Chi2_Iteration_Dists/h_chi2_modelData_trueData_iter_chi2");
        PlotUtils::MnvH1D* h_median_chi2 = (PlotUtils::MnvH1D*)fin_warp.Get("Chi2_Iteration_Dists/h_median_chi2_modelData_trueData_iter_chi2");
        TProfile* prof_avg_chi2          = (TProfile*)fin_warp.Get("Chi2_Iteration_Dists/m_avg_chi2_modelData_trueData_iter_chi2");
        
        
        // Material title
        std::string material_title;
        if ( option_material == "lead" )      material_title = " - [Lead]";
        else if ( option_material == "iron" ) material_title = " - [Iron]";
        
        
        // Fake data title
        std::string fakedata_title;
        if ( option_model_fakedata == "v0" )                fakedata_title = "GENIE 2.12.6";
        else if ( option_model_fakedata == "v1" )           fakedata_title = "MINER#nuA Tune 4.0.1";
        else if ( option_model_fakedata == "v1noNonResPi" ) fakedata_title = "Mn#nuTune 4.0.1 w/o non-RES #pi";
        else if ( option_model_fakedata == "v1noD2" )       fakedata_title = "Mn#nuTune 4.0.1 w/o deuterium #pi";
        else if ( option_model_fakedata == "v1noPionTune" ) fakedata_title = "Mn#nuTune 4.0.1 w/o any #pi tune";
        else if ( option_model_fakedata == "v2MINOS" )      fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'MINOS'";
        else if ( option_model_fakedata == "v2JOINT" )      fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'JOINT'";
        else if ( option_model_fakedata == "v2NU1PI" )      fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NU1PI'";
        else if ( option_model_fakedata == "v2NUNPI" )      fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NUNPI'";
        else if ( option_model_fakedata == "v2NUPI0" )      fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NUPI0'";
        else if ( option_model_fakedata == "v2MENU1PI" )    fakedata_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'MENU1PI'";
        
        
        // PlotInfo properties
        const bool do_frac_uncertainty = true;
        const bool do_cov_area_norm    = false;
        const bool include_stat_error  = true;
        const bool do_bin_width_norm   = true;
        const std::string print_format = "eps";
        
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Plot fake data and MC model histogram
        PlotFakeDataMC(plot_info, h_fake_data, h_model_reco,
                       output_topdir + "/DataMC_" + option_material,
                       "Fake data: " + fakedata_title + material_title,
                       "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "", "TR", -1.0, -1.0, true);
        
        PlotFakeDataMCRatio(plot_info, h_fake_data, h_model_reco,
                            output_topdir + "/DataMCRatio_" + option_material,
                            "Fake data: " + fakedata_title + material_title,
                            "Reconstructed muon #font[12]{p}_{#font[132]{T}} [GeV/c]", "Fake Data / MC", 0.0, 2.0);
        
        
        // Plot warping chi2 info
        PlotWarpingChi2FullInfo(plot_info, h_chi2, h_median_chi2, prof_avg_chi2,
                                output_topdir + "/Chi2FullInfo_" + option_material,
                                "Fake data: " + fakedata_title + material_title,
                                "Number of unfolding iterations", "#chi^{2} (Unfolded : True fake data)", "Number of Poisson universes", (double)ndf, 50.0);
        
        PlotWarpingMedianChi2(plot_info, h_median_chi2,
                              output_topdir + "/MedianChi2_" + option_material,
                              "Fake data: " + fakedata_title + material_title,
                              "Number of unfolding iterations", "Median #chi^{2} (Unfolded : True fake data)", (double)ndf);
        
        PlotWarpingChi2AtNiter(plot_info, h_chi2, h_median_chi2, prof_avg_chi2,
                               output_topdir + "/Chi2AtNiter1_" + option_material,
                               "Fake data: " + fakedata_title + material_title,
                               "#chi^{2} (Unfolded : True fake data) at N_{iter} = 1",
                               "Number of Poisson universes", (double)ndf, 1);
        
        PlotWarpingChi2AtNiter(plot_info, h_chi2, h_median_chi2, prof_avg_chi2,
                               output_topdir + "/Chi2AtNiter2_" + option_material,
                               "Fake data: " + fakedata_title + material_title,
                               "#chi^{2} (Unfolded : True fake data) at N_{iter} = 2",
                               "Number of Poisson universes", (double)ndf, 2);
    }
    
    
    // Close files
    fin_warp.Close();
    fin_pot.Close();
}


#endif  // PlotWarping_C