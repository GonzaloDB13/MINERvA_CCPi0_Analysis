#ifndef PlotEventSelection_C
#define PlotEventSelection_C

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
//#include "../includes/plotting_functions.h"
#include "../includes/plotting_functions_finalversion.h"
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#endif  // __CINT__

#include "TFile.h"





// ========================================================================================================================
//  PLOTTING FUNCTION
// ========================================================================================================================

void Plot(CCPi0::MacroUtil util,
          TFile& mc_fin,
          TFile& data_fin,
          std::string option_date_mc,
          std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/EventSelection/plots/%s/%s", option_date_mc.c_str(),
                                                                                                   option_material.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(true);  // Include true variables
    
    
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
    
    
    // Loop over variables to load histograms
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> LoadMCHists_Selection(mc_fin, util.m_error_bands);
            var -> LoadDataHists_Selection(data_fin);
            var -> LoadMigrationHists(mc_fin, util.m_error_bands);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> LoadEffNumerator(mc_fin, util.m_error_bands);
            
            if ( util.m_do_truth )
                var -> LoadEffDenominator(mc_fin, util.m_error_bands_truth);
        }
    }
    
    
    // =========================================
    //  Loop over variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Construct plot info object
        PlotInfo plot_info(var, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // For reco-only variables
        // =======================
        
        if ( !(var->m_is_true) )
        {
            // Plot event selection
            PlotDataMC(plot_info,
                       var->m_hists.m_data_Selection, var->m_hists.m_mc_Selection.hist, //NULL, NULL,
                       output_topdir + "/EventSelection/DataMC_" + option_material,
                       "Event selection" + material_title,
                       "", "", "TR", -1.0, -1.0,
                       true, true, false, false, true);
            
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
                              output_topdir + "/EventSelection/DataStackedMC_" + option_material,
                              "Event selection" + material_title,
                              "", "", "TR", -1.0, -1.0,
                              true, true, false, true);
            
            PlotDataMCRatio(plot_info,
                            var->m_hists.m_data_Selection, var->m_hists.m_mc_Selection.hist,
                            output_topdir + "/EventSelection/DataMCRatio_" + option_material,
                            "Data-MC ratio from event selection" + material_title,
                            "", "", 0.0, 2.0,
                            true, true, false, false, true);
            
            PlotErrorSummary(plot_info,
                             var->m_hists.m_mc_Selection.hist,
                             output_topdir + "/EventSelection/FracErrors_" + option_material,
                             "Event selection fractional errors" + material_title, 0.4, true);
        }
        
        
        // For true-only variables
        // =======================
        
        else if ( var->m_is_true )
        {
            // Plot efficiency numerator
            PlotMC(plot_info,
                   var->m_hists.m_mc_EffNumerator.hist,
                   output_topdir + "/EffNumerator/MC_" + option_material,
                   "Efficiency numerator" + material_title,
                   "Muon transverse momentum [GeV/c]", "",
                   -1.0, -1.0, -1.0, true, false);
            
            PlotErrorSummary(plot_info,
                             var->m_hists.m_mc_EffNumerator.hist,
                             output_topdir + "/EffNumerator/FracErrors_" + option_material,
                             "Eff. numerator fractional errors" + material_title, 0.5, true,
                             "Muon transverse momentum [GeV/c]");
            
            
            if ( util.m_do_truth )
            {
                // Add missing error bands to efficiency denominator
                var -> m_hists.m_mc_EffDenominator.hist->AddMissingErrorBandsAndFillWithCV(*var->m_hists.m_mc_EffNumerator.hist);
                
                
                // Plot efficiency denominator
                PlotMC(plot_info,
                       var->m_hists.m_mc_EffDenominator.hist,
                       output_topdir + "/EffDenominator/MC_" + option_material,
                       "Efficiency denominator" + material_title,
                       "Muon transverse momentum [GeV/c]", "",
                       -1.0, -1.0, -1.0, true, false);
                
                PlotErrorSummary(plot_info,
                                 var->m_hists.m_mc_EffDenominator.hist,
                                 output_topdir + "/EffDenominator/FracErrors_" + option_material,
                                 "Eff. denominator fractional errors" + material_title, 0.4, true,
                                 "Muon transverse momentum [GeV/c]");
                
                
                // Get efficiency
                var->m_hists.m_Efficiency = var->m_hists.m_mc_EffNumerator.hist->Clone(uniq());
                var->m_hists.m_Efficiency -> Divide(var->m_hists.m_mc_EffNumerator.hist,
                                                    var->m_hists.m_mc_EffDenominator.hist);
                
                
                // Plot efficiency
                PlotMC(plot_info,
                       var->m_hists.m_Efficiency,
                       output_topdir + "/Efficiency/MC_" + option_material,
                       "Efficiency" + material_title,
                       "Muon transverse momentum [GeV/c]", "Efficiency / 0.075 GeV/c",
                       -1.0, -1.0, -1.0, true, false, false, false, false, false, false, false);
                
                PlotErrorSummary(plot_info,
                                 var->m_hists.m_Efficiency,
                                 output_topdir + "/Efficiency/FracErrors_" + option_material,
                                 "Efficiency fractional errors" + material_title, 0.4, true,
                                 "Muon transverse momentum [GeV/c]");
            }
        }
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotEventSelection(std::string option_date,
                        std::string option_model = "v1",
                        bool do_systematics      = true)
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
    std::string mc_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/lead", option_date_mc.c_str());
    std::string mc_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/lead", option_date_data.c_str());
    std::string data_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/iron", option_date_data.c_str());
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_EventSelection_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_lead_topdir.c_str(),
                                                                                                  option_model.c_str(),
                                                                                                  option_systematics.c_str()), "READ");

    TFile mc_fin_iron(Form("%s/MC_EventSelection_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_iron_topdir.c_str(),
                                                                                                  option_model.c_str(),
                                                                                                  option_systematics.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_EventSelection_AllPlaylists_lead.root", data_fin_lead_topdir.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_EventSelection_AllPlaylists_iron.root", data_fin_iron_topdir.c_str()), "READ");
    
    
    
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
    // (TRUTH option set as 'true')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, do_systematics, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot_lead   = GetPOT(mc_fin_lead,   true);
    double mc_pot_iron   = GetPOT(mc_fin_iron,   true);
    double data_pot_lead = GetPOT(data_fin_lead, false);
    double data_pot_iron = GetPOT(data_fin_iron, false);
    
    if ( std::fabs(mc_pot_lead   - mc_pot_iron)   > std::numeric_limits<double>::epsilon() ||
         std::fabs(data_pot_lead - data_pot_iron) > std::numeric_limits<double>::epsilon() ) {
        std::cout << std::endl;
        std::cout << " WARNING: POT in lead and iron files are not the same. Setting MacroUtil POT using lead file. " << std::endl;
        std::cout << std::endl;
    }
    
    SetMacroUtilPOT(mc_fin_lead, data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " Lead: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron: " << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_iron << std::endl;
    std::cout << std::endl;
    
    
    // Text file with POT info
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/POTinfo/%s", option_date_mc.c_str());
    
    std::ofstream text_pot(Form("%s/POT_EventSelection_AllPlaylists.txt", text_topdir.c_str()));
    
    text_pot << std::endl;
    text_pot << " ALL PLAYLISTS " << std::endl;
    text_pot << std::endl;
    text_pot << " Lead: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron: " << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_iron   << std::endl;
    text_pot << " \tData POT: " << data_pot_iron << std::endl;
    text_pot << std::endl;
    
    text_pot.close();
    
    
    
    // =========================================
    //  Plot functions
    // =========================================
    
    std::cout << " Plotting event selection... " << std::endl;
    std::cout << std::endl;
    
    
    // Lead
    Plot(util, mc_fin_lead, data_fin_lead, option_date_mc, "lead");
    
    
    // Iron
    Plot(util, mc_fin_iron, data_fin_iron, option_date_mc, "iron");
    
    
    // Close ROOT files
    mc_fin_lead.Close();
    mc_fin_iron.Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
}


#endif  // PlotEventSelection_C