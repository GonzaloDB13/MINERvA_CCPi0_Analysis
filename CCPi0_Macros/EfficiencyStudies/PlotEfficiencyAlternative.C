#ifndef PlotEfficiencyOptimize_C
#define PlotEfficiencyOptimize_C

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
//  PLOTTING FUNCTION
// ========================================================================================================================

void Plot(CCPi0::MacroUtil util,
          TFile& fin,
          std::string option_date_mc,
          std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/EfficiencyStudies/plots/%s/%s", option_date_mc.c_str(),
                                                                                                      option_material.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetEffStudyVariables(true);  // Include true variables
    
    
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
    
    
    // Loop over variables to load histograms
    for ( auto var : variables )
    {
        // For true-only variables
        if ( var->m_is_true ) {
            var -> LoadEffNumerator(fin, util.m_error_bands);
            if ( util.m_do_truth ) var -> LoadEffDenominator(fin, util.m_error_bands_truth);
        }
    }
    
    
    // Loop over variables for plotting
    // ================================
    
    for ( auto var : variables )
    {
        // Construct PlotInfo
        PlotInfo plot_info(var, util.m_mc_pot, util.m_data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Plot title and name
        std::string title_str;
        std::string name_str;
        
        if ( var->Name() == "Eff_ThetaMu10" ) {
            title_str = " (true #theta_{#mu} < 10 deg)";
            name_str  = "TrueThetaMu10_";
        }
        else if ( var->Name() == "Eff_ThetaMu11" ) {
            title_str = " (true #theta_{#mu} < 11 deg)";
            name_str  = "TrueThetaMu11_";
        }
        else if ( var->Name() == "Eff_ThetaMu12" ) {
            title_str = " (true #theta_{#mu} < 12 deg)";
            name_str  = "TrueThetaMu12_";
        }
        else if ( var->Name() == "Eff_ThetaMu13" ) {
            title_str = " (true #theta_{#mu} < 13 deg)";
            name_str  = "TrueThetaMu13_";
        }
        else if ( var->Name() == "Eff_ThetaMu14" ) {
            title_str = " (true #theta_{#mu} < 14 deg)";
            name_str  = "TrueThetaMu14_";
        }
        else if ( var->Name() == "Eff_ThetaMu15" ) {
            title_str = " (true #theta_{#mu} < 15 deg)";
            name_str  = "TrueThetaMu15_";
        }
        else if ( var->Name() == "Eff_ThetaMu16" ) {
            title_str = " (true #theta_{#mu} < 16 deg)";
            name_str  = "TrueThetaMu16_";
        }
        else if ( var->Name() == "Eff_ThetaMu17" ) {
            title_str = " (true #theta_{#mu} < 17 deg)";
            name_str  = "TrueThetaMu17_";
        }
        else if ( var->Name() == "Eff_ThetaMu18" ) {
            title_str = " (true #theta_{#mu} < 18 deg)";
            name_str  = "TrueThetaMu18_";
        }
        else if ( var->Name() == "Eff_ThetaMu19" ) {
            title_str = " (true #theta_{#mu} < 19 deg)";
            name_str  = "TrueThetaMu19_";
        }
        else if ( var->Name() == "Eff_ThetaMu20" ) {
            title_str = " (true #theta_{#mu} < 20 deg)";
            name_str  = "TrueThetaMu20_";
        }
        else if ( var->Name() == "Eff_ThetaMu21" ) {
            title_str = " (true #theta_{#mu} < 21 deg)";
            name_str  = "TrueThetaMu21_";
        }
        else if ( var->Name() == "Eff_ThetaMu22" ) {
            title_str = " (true #theta_{#mu} < 22 deg)";
            name_str  = "TrueThetaMu22_";
        }
        else if ( var->Name() == "Eff_ThetaMu23" ) {
            title_str = " (true #theta_{#mu} < 23 deg)";
            name_str  = "TrueThetaMu23_";
        }
        else if ( var->Name() == "Eff_ThetaMu24" ) {
            title_str = " (true #theta_{#mu} < 24 deg)";
            name_str  = "TrueThetaMu24_";
        }
        else if ( var->Name() == "Eff_ThetaMu25" ) {
            title_str = " (true #theta_{#mu} < 25 deg)";
            name_str  = "TrueThetaMu25_";
        }
        
        
        // Plot for true-only variables
        if ( var->m_is_true )
        {
            // Plot efficiency numerator
            double Ymax_eff_num;
            if ( option_material == "lead" )      Ymax_eff_num = 1700.0;
            else if ( option_material == "iron" ) Ymax_eff_num = 1000.0;
            
            PlotMCUserDefinedY(plot_info, var->m_hists.m_mc_EffNumerator.hist,
                               output_topdir + "/EffNumeratorOptimize/EffNumerator_" + name_str + option_material,
                               "Eff. numerator" + title_str + material_title,
                               "", "", -1.0, Ymax_eff_num, true, false);
            
            PlotErrorSummary(plot_info, var->m_hists.m_mc_EffNumerator.hist,
                             output_topdir + "/EffNumeratorOptimize/EffNumeratorErrors_" + name_str + option_material,
                             "Eff. numerator errors" + title_str + material_title, 0.6, true);
            
            
            if ( util.m_do_truth )
            {
                // Add missing error bands to efficiency denominator
                var -> m_hists.m_mc_EffDenominator.hist->AddMissingErrorBandsAndFillWithCV(*var->m_hists.m_mc_EffNumerator.hist);
                
                
                // Plot efficiency denominator
                double Ymax_eff_den;
                if ( option_material == "lead" )      Ymax_eff_den = 45000.0;
                else if ( option_material == "iron" ) Ymax_eff_den = 20000.0;
                
                PlotMCUserDefinedY(plot_info, var->m_hists.m_mc_EffDenominator.hist,
                                   output_topdir + "/EffDenominatorOptimize/EffDenominator_" + name_str + option_material,
                                   "Eff. denominator" + title_str + material_title,
                                   "", "", -1.0, Ymax_eff_den, true, false);
                
                PlotErrorSummary(plot_info, var->m_hists.m_mc_EffDenominator.hist,
                                 output_topdir + "/EffDenominatorOptimize/EffDenominatorErrors_" + name_str + option_material,
                                 "Eff. denominator errors" + title_str + material_title, 0.6, true);
                
                
                // Get efficiency
                var->m_hists.m_Efficiency = var->m_hists.m_mc_EffNumerator.hist->Clone(uniq());
                var->m_hists.m_Efficiency -> Divide(var->m_hists.m_mc_EffNumerator.hist,
                                                    var->m_hists.m_mc_EffDenominator.hist);
                
                
                // Plot efficiency
                double Ymax_eff;
                if ( option_material == "lead" )      Ymax_eff = 0.1;
                else if ( option_material == "iron" ) Ymax_eff = 0.2;
                
                PlotMCUserDefinedY(plot_info, var->m_hists.m_Efficiency,
                                   output_topdir + "/EfficiencyOptimize/Efficiency_" + name_str + option_material,
                                   "Efficiency" + title_str + material_title,
                                   "", "Efficiency / 0.075 GeV/c", -1.0, Ymax_eff, false, false);
                
                PlotErrorSummary(plot_info, var->m_hists.m_Efficiency,
                                 output_topdir + "/EfficiencyOptimize/EfficiencyErrors_" + name_str + option_material,
                                 "Efficiency errors" + title_str + material_title, 0.4, true);
            }
        }
    }
}




// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotEfficiencyOptimize(std::string option_date,
                            std::string option_model = "v1")
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file 
    const std::string option_date_mc = option_date + "_" + option_model;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    std::string fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EfficiencyStudies/%s/lead", option_date_mc.c_str());
    std::string fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EfficiencyStudies/%s/iron", option_date_mc.c_str());
    
    
    // Input files
    // ===========
    
    TFile fin_lead(Form("%s/MC_EfficiencyOptimize_MnvGENIE%s_WithSyst_AllPlaylists_lead.root", fin_topdir_lead.c_str(),
                                                                                               option_model.c_str()), "READ");
    
    TFile fin_iron(Form("%s/MC_EfficiencyOptimize_MnvGENIE%s_WithSyst_AllPlaylists_iron.root", fin_topdir_iron.c_str(),
                                                                                               option_model.c_str()), "READ");
    
    
    
    // =========================================
    //  MacroUtil
    // =========================================
    
    // Playlist string
    // (Use only to load MC chain and access systematics)
    const std::string plist_string = "minervame1A";
    
    
    // Set playlists MC input
    // (Similarly, only to load MC and access systematics)
    const std::string mc_file_list = GetPlaylistFile(true,  plist_string, "test");
    
    
    // Set MacroUtil
    // (GRID option set as 'false', TRUTH and SYSTEMATICS options set as 'true')
    CCPi0::MacroUtil util(mc_file_list, plist_string, true, false, true, type_model);
    
    
    // Set MacroUtil POT
    SetMacroUtilPOT(fin_lead, util, true);
    
    std::cout << std::endl;
    std::cout << " Plotting efficiency optimization... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << std::endl;
    
    
    
    // =========================================
    //  Plot functions
    // =========================================
    
    // Lead
    Plot(util, fin_lead, option_date_mc, "lead");
    
    
    // Iron
    Plot(util, fin_iron, option_date_mc, "iron");
    
    
    // Close ROOT files
    fin_lead.Close();
    fin_iron.Close();
}


#endif  // PlotEfficiencyOptimize_C