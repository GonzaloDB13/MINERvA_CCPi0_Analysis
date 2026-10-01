#ifndef PlotSidebandStudies_C
#define PlotSidebandStudies_C

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
#include "../includes/plotting_functions.h"
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#include "../includes/Variable2D.h"
#endif  // __CINT__

#include "TFile.h"





// ========================================================================================================================
//  PLOT VARIABLES
// ========================================================================================================================

void PlotSidebands(CCPi0::MacroUtil util,
                   TFile& mc_fin,
                   TFile& data_fin,
                   std::string option_date_mc,
                   std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/SidebandStudies/plots/%s/%s", option_date_mc.c_str(),
                                                                                                    option_material.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetSidebandStudyVariables();
    
    
    // Material title
    std::string material_title;
    if ( option_material == "lead" )      material_title = " - [Lead]";
    else if ( option_material == "iron" ) material_title = " - [Iron]";
    
    
    // PlotInfo properties
    const bool do_frac_uncertainty = true;
    const bool do_cov_area_norm    = false;
    const bool include_stat_error  = true;
    const bool do_bin_width_norm   = true;
    const std::string print_format = "png";
    
    
    // CV universe to use in MC stack histograms
    CVUniverse* cv_univ = (util.m_error_bands["cv"])[0];
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // Loop over variables to load histograms
    for ( auto var : variables ) {
        var -> LoadMCHists_Selection(mc_fin, util.m_error_bands);
        var -> LoadDataHists_Selection(data_fin);
    }
    
    
    
    // =========================================
    //  Loop over variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Construct plot info object
        PlotInfo plot_info(var, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Dummy cut arrows
        std::vector<double> arrow_X;
        std::vector<double> arrow_Ymax;
        std::vector<double> arrow_length;
        std::vector<std::string> arrow_dir;
        
        
        // Plot title
        std::string title_str;
        std::string conf_dir;
        
        if ( var->Name() == "SigReg_Conf1" ) {
            title_str = "Physics signal region - Configuration 1";
            conf_dir  = "/Configuration1/";
        }
        else if ( var->Name() == "PionSB_Conf1" ) {
            title_str = "Pion sideband - Configuration 1";
            conf_dir  = "/Configuration1/";
        }
        else if ( var->Name() == "ProtonSB_Conf1" ) {
            title_str = "Proton sideband - Configuration 1";
            conf_dir  = "/Configuration1/";
        }
        else if ( var->Name() == "HighWSB_Conf1" ) {
            title_str = "High-W sideband - Configuration 1";
            conf_dir  = "/Configuration1/";
        }
        else if ( var->Name() == "SigReg_Conf2" ) {
            title_str = "Physics signal region - Configuration 2";
            conf_dir  = "/Configuration2/";
        }
        else if ( var->Name() == "PionSB_Conf2" ) {
            title_str = "Pion sideband - Configuration 2";
            conf_dir  = "/Configuration2/";
        }
        else if ( var->Name() == "ProtonSB_Conf2" ) {
            title_str = "Proton sideband - Configuration 2";
            conf_dir  = "/Configuration2/";
        }
        else if ( var->Name() == "HighWSB_Conf2" ) {
            title_str = "High-W sideband - Configuration 2";
            conf_dir  = "/Configuration2/";
        }
        else if ( var->Name() == "SigReg_Conf3" ) {
            title_str = "Physics signal region - Configuration 3";
            conf_dir  = "/Configuration3/";
        }
        else if ( var->Name() == "PionSB_Conf3" ) {
            title_str = "Pion sideband - Configuration 3";
            conf_dir  = "/Configuration3/";
        }
        else if ( var->Name() == "ProtonSB_Conf3" ) {
            title_str = "Proton sideband - Configuration 3";
            conf_dir  = "/Configuration3/";
        }
        else if ( var->Name() == "HighWSB_Conf3" ) {
            title_str = "High-W sideband - Configuration 3";
            conf_dir  = "/Configuration3/";
        }
        else if ( var->Name() == "SigReg_Conf4" ) {
            title_str = "Physics signal region - Configuration 4";
            conf_dir  = "/Configuration4/";
        }
        else if ( var->Name() == "PionSB_Conf4" ) {
            title_str = "Pion sideband - Configuration 4";
            conf_dir  = "/Configuration4/";
        }
        else if ( var->Name() == "ProtonSB_Conf4" ) {
            title_str = "Proton sideband - Configuration 4";
            conf_dir  = "/Configuration4/";
        }
        else if ( var->Name() == "HighWSB_Conf4" ) {
            title_str = "High-W sideband - Configuration 4";
            conf_dir  = "/Configuration4/";
        }
        
        
        // Plot
        PlotDataStackedMC(plot_info,
                          var->m_hists.m_data_Selection, var->m_hists.m_mc_Selection.hist,
                          (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasUp.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasBetw.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasDown.univHist(cv_univ),
                          (MnvH1D*)var->m_hists.m_mc_Selection_BackgrOther.univHist(cv_univ),
                          output_topdir + conf_dir + var->Name() + "_" + option_material,
                          title_str + material_title,
                          arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                          "", "", "TR");
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotSidebandStudies(std::string option_date,
                         std::string option_model = "v1",
                         bool do_systematics      = false)
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
    
    // Monte Carlo
    std::string mc_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SidebandStudies/mc/%s/lead", option_date_mc.c_str());
    
    std::string mc_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SidebandStudies/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SidebandStudies/data/%s/lead", option_date_data.c_str());
    
    std::string data_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SidebandStudies/data/%s/iron", option_date_data.c_str());
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_SidebandStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_topdir_lead.c_str(),
                                                                                                   option_model.c_str(),
                                                                                                   option_systematics.c_str()), "READ");
    
    TFile mc_fin_iron(Form("%s/MC_SidebandStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_topdir_iron.c_str(),
                                                                                                   option_model.c_str(),
                                                                                                   option_systematics.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_SidebandStudies_AllPlaylists_lead.root", data_fin_topdir_lead.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_SidebandStudies_AllPlaylists_iron.root", data_fin_topdir_iron.c_str()), "READ");
    
    
    
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
    // (TRUTH and SYSTEMATICS options set as 'false')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, false, false, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot   = GetPOT(mc_fin_lead,   true);
    double data_pot = GetPOT(data_fin_lead, false);
    
    SetMacroUtilPOT(mc_fin_lead, data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " Plotting cut studies... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << mc_pot   << std::endl;
    std::cout << " \tData POT: " << data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Plot
    PlotSidebands(util, mc_fin_lead, data_fin_lead, option_date_mc, "lead");
    
    PlotSidebands(util, mc_fin_iron, data_fin_iron, option_date_mc, "iron");
    
    
    // Close ROOT files
    mc_fin_lead.Close();
    mc_fin_iron.Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
}


#endif  // PlotSidebandStudies_C