#ifndef PlotUnfoldStatStudies_C
#define PlotUnfoldStatStudies_C

#include <iostream>
#include <string>
#include <vector>

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
#endif  // __CINT__

#include "TFile.h"





// ========================================================================================================================
//  PLOT FUNCTION
// ========================================================================================================================

void Plot(CCPi0::MacroUtil util,
          TFile& fin,
          std::string option_date_data,
          std::string option_material,
          double uncfactor)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/UnfoldStatStudies/plots/%s/%s", option_date_data.c_str(),
                                                                                                      option_material.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Material title
    std::string material_title;
    if ( option_material == "lead" )      material_title = " - [Lead]";
    else if ( option_material == "iron" ) material_title = " - [Iron]";
    
    
    // X-section model title
    std::string uncfactor_title = "Uncertainty factor: " + std::to_string(uncfactor);
    
    
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
        PlotInfo plot_info(var, -1.0, -1.0,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Load histograms
        TGraph* gr_StatFactor10      = (TGraph*)fin.Get("StatFactor10.000000");
        TGraph* gr_StatFactor11      = (TGraph*)fin.Get("StatFactor11.000000");
        TGraph* gr_StatFactor11_1345 = (TGraph*)fin.Get("StatFactor11.134500");
        TGraph* gr_StatFactor12      = (TGraph*)fin.Get("StatFactor12.000000");
        TGraph* gr_StatFactor13      = (TGraph*)fin.Get("StatFactor13.000000");
        
        // Plot
        PlotUnfoldStatStudyGraphs(plot_info,
                                  gr_StatFactor10,
                                  gr_StatFactor11,
                                  gr_StatFactor11_1345,
                                  gr_StatFactor12,
                                  gr_StatFactor13,
                                  output_topdir + "/Uncfactor" + std::to_string(uncfactor) + "_" + option_material,
                                  uncfactor_title + material_title,
                                  "Number of unfolding iterations", "Mean #chi^{2} (N.d.f. = 13)");
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotUnfoldStatStudies(std::string option_date,
                           std::string option_model,
                           std::string option_material)
{
    // Get MC model for MacroUtil
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file
    const std::string option_date_mc = option_date + "_" + option_model;
    
    
    
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
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directory
    std::string fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/UnfoldStatStudies/%s/%s/PlotMCSampleSizeScan", option_date_mc.c_str(),
                                                                                                                             option_material.c_str());
    
    
    // If plotting lead files
    // ======================
    
    if ( option_material == "lead" ) {
        TFile fin_uncfactor5(        Form("%s/Uncfactor5_%s.root",         fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor6(        Form("%s/Uncfactor6_%s.root",         fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor7(        Form("%s/Uncfactor7_%s.root",         fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8(        Form("%s/Uncfactor8_%s.root",         fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8_1(      Form("%s/Uncfactor8.1_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8_2(      Form("%s/Uncfactor8.2_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8_4(      Form("%s/Uncfactor8.4_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8_6(      Form("%s/Uncfactor8.6_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor8_8(      Form("%s/Uncfactor8.8_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor9(        Form("%s/Uncfactor9_%s.root",         fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor10(       Form("%s/Uncfactor10_%s.root",        fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor100000000(Form("%s/Uncfactor100000000_%s.root", fin_topdir.c_str(), option_material.c_str()));
        
        
        // Plot
        std::cout << std::endl;
        std::cout << " Plotting unfolding statistical studies results... " << std::endl;
        std::cout << std::endl;
        
        Plot(util, fin_uncfactor5,   option_date_mc, option_material, 5.0);
        Plot(util, fin_uncfactor6,   option_date_mc, option_material, 6.0);
        Plot(util, fin_uncfactor7,   option_date_mc, option_material, 7.0);
        Plot(util, fin_uncfactor8,   option_date_mc, option_material, 8.0);
        Plot(util, fin_uncfactor8_1, option_date_mc, option_material, 8.1);
        Plot(util, fin_uncfactor8_2, option_date_mc, option_material, 8.2);
        Plot(util, fin_uncfactor8_4, option_date_mc, option_material, 8.4);
        Plot(util, fin_uncfactor8_6, option_date_mc, option_material, 8.6);
        Plot(util, fin_uncfactor8_8, option_date_mc, option_material, 8.8);
        Plot(util, fin_uncfactor9,   option_date_mc, option_material, 9.0);
        Plot(util, fin_uncfactor10,  option_date_mc, option_material, 10.0);
        Plot(util, fin_uncfactor100000000, option_date_mc, option_material, 100000000.0);
    }
    
    
    // If plotting iron files
    // ======================
    
    if ( option_material == "iron" ) {
        TFile fin_uncfactor1(        Form("%s/Uncfactor1_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor2(        Form("%s/Uncfactor2_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3(        Form("%s/Uncfactor3_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3_2(      Form("%s/Uncfactor3.2_%s.root",     fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3_3(      Form("%s/Uncfactor3.3_%s.root",     fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3_4(      Form("%s/Uncfactor3.4_%s.root",     fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3_6(      Form("%s/Uncfactor3.6_%s.root",     fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor3_8(      Form("%s/Uncfactor3.8_%s.root",     fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor4(        Form("%s/Uncfactor4_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor5(        Form("%s/Uncfactor5_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor6(        Form("%s/Uncfactor6_%s.root",       fin_topdir.c_str(), option_material.c_str()));
        TFile fin_uncfactor100000000(Form("%s/Uncfactor100000000_%s.root", fin_topdir.c_str(), option_material.c_str()));
        
        
        // Plot
        std::cout << std::endl;
        std::cout << " Plotting unfolding statistical studies results... " << std::endl;
        std::cout << std::endl;
        
        Plot(util, fin_uncfactor1,   option_date_mc, option_material, 1.0);
        Plot(util, fin_uncfactor2,   option_date_mc, option_material, 2.0);
        Plot(util, fin_uncfactor3,   option_date_mc, option_material, 3.0);
        Plot(util, fin_uncfactor3_2, option_date_mc, option_material, 3.2);
        Plot(util, fin_uncfactor3_3, option_date_mc, option_material, 3.3);
        Plot(util, fin_uncfactor3_4, option_date_mc, option_material, 3.4);
        Plot(util, fin_uncfactor3_6, option_date_mc, option_material, 3.6);
        Plot(util, fin_uncfactor3_8, option_date_mc, option_material, 3.8);
        Plot(util, fin_uncfactor4,   option_date_mc, option_material, 4.0);
        Plot(util, fin_uncfactor5,   option_date_mc, option_material, 5.0);
        Plot(util, fin_uncfactor6,   option_date_mc, option_material, 6.0);
        Plot(util, fin_uncfactor100000000, option_date_mc, option_material, 100000000.0);
    }
}


#endif  // PlotUnfoldStatStudies_C