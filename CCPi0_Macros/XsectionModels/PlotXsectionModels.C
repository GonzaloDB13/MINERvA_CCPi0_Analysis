#ifndef PlotXsectionModels_C
#define PlotXsectionModels_C

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
//  PLOT FUNCTION
// ========================================================================================================================

void Plot(CCPi0::MacroUtil util,
          TFile* xsec_fin,
          TFile& data_fin,
          std::string option_date_data,
          std::string option_xsec_model,
          std::string option_material,
          bool use_genie2,
          bool use_genie3,
          bool use_neut)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/XsectionModels/plots/%s/%s/%s", option_date_data.c_str(),
                                                                                                      option_material.c_str(),
                                                                                                      option_xsec_model.c_str());
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Material title
    std::string material_title;
    if ( option_material == "lead" )      material_title = " - [Lead]";
    else if ( option_material == "iron" ) material_title = " - [Iron]";
    
    
    // X-section model title
    std::string xsec_title;
    if ( option_xsec_model == "v0" )                xsec_title = "GENIE 2.12.6";
    else if ( option_xsec_model == "v1" )           xsec_title = "MINER#nuA Tune 4.0.1";
    else if ( option_xsec_model == "v1noNonResPi" ) xsec_title = "Mn#nuTune 4.0.1 w/o non-RES #pi";
    else if ( option_xsec_model == "v1noD2" )       xsec_title = "Mn#nuTune 4.0.1 w/o deuterium #pi";
    else if ( option_xsec_model == "v1noPionTune" ) xsec_title = "Mn#nuTune 4.0.1 w/o any #pi tune";
    else if ( option_xsec_model == "v2MINOS" )      xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'MINOS'";
    else if ( option_xsec_model == "v2JOINT" )      xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'JOINT'";
    else if ( option_xsec_model == "v2NU1PI" )      xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NU1PI'";
    else if ( option_xsec_model == "v2NUNPI" )      xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NUNPI'";
    else if ( option_xsec_model == "v2NUPI0" )      xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'NUPI0'";
    else if ( option_xsec_model == "v2MENU1PI" )    xsec_title = "Mn#nu 4.0.1 w/#pi low-#font[12]{Q^{2}} 'MENU1PI'";
    else if ( option_xsec_model == "GENIE3_02a" )   xsec_title = "GENIE 3.0.6 RFG #font[12]{hA}";
    else if ( option_xsec_model == "GENIE3_02b" )   xsec_title = "GENIE 3.0.6 RFG #font[12]{hN}";
    else if ( option_xsec_model == "GENIE3_10a" )   xsec_title = "GENIE 3.0.6 LFG #font[12]{hA}";
    else if ( option_xsec_model == "GENIE3_10b" )   xsec_title = "GENIE 3.0.6 LFG #font[12]{hN}";
    else if ( option_xsec_model == "NEUT_LFG" )     xsec_title = "NEUT 5.0.2 LFG";
    
    
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
        
        // X-section models
        PlotUtils::MnvH1D* h_mc_CrossSection;
        PlotUtils::MnvH1D* h_mc_CrossSection_QE;
        PlotUtils::MnvH1D* h_mc_CrossSection_MEC;
        PlotUtils::MnvH1D* h_mc_CrossSection_DeltaRES;
        PlotUtils::MnvH1D* h_mc_CrossSection_OtherRES;
        PlotUtils::MnvH1D* h_mc_CrossSection_RES;
        PlotUtils::MnvH1D* h_mc_CrossSection_SoftDIS;
        PlotUtils::MnvH1D* h_mc_CrossSection_TrueDIS;
        PlotUtils::MnvH1D* h_mc_CrossSection_Other;
        
        if ( use_genie2 ) {
            h_mc_CrossSection          = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s",          var_name.c_str()));
            h_mc_CrossSection_QE       = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_QE",       var_name.c_str()));
            h_mc_CrossSection_MEC      = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_MEC",      var_name.c_str()));
            h_mc_CrossSection_DeltaRES = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_DeltaRES", var_name.c_str()));
            h_mc_CrossSection_OtherRES = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_OtherRES", var_name.c_str()));
            h_mc_CrossSection_SoftDIS  = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_SoftDIS",  var_name.c_str()));
            h_mc_CrossSection_TrueDIS  = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_TrueDIS",  var_name.c_str()));
            h_mc_CrossSection_Other    = (PlotUtils::MnvH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_Other",    var_name.c_str()));
        }
        else if ( use_genie3 || use_neut ) {
            TH1D* mc_CrossSection         = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s",         var_name.c_str()));
            TH1D* mc_CrossSection_QE      = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_QE",      var_name.c_str()));
            TH1D* mc_CrossSection_MEC     = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_MEC",     var_name.c_str()));
            TH1D* mc_CrossSection_RES     = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_RES",     var_name.c_str()));
            TH1D* mc_CrossSection_SoftDIS = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_SoftDIS", var_name.c_str()));
            TH1D* mc_CrossSection_TrueDIS = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_TrueDIS", var_name.c_str()));
            TH1D* mc_CrossSection_Other   = (TH1D*)xsec_fin->Get(Form("mc_CrossSection_%s_Other",   var_name.c_str()));
            
            h_mc_CrossSection         = new PlotUtils::MnvH1D(*mc_CrossSection);
            h_mc_CrossSection_QE      = new PlotUtils::MnvH1D(*mc_CrossSection_QE);
            h_mc_CrossSection_MEC     = new PlotUtils::MnvH1D(*mc_CrossSection_MEC);
            h_mc_CrossSection_RES     = new PlotUtils::MnvH1D(*mc_CrossSection_RES);
            h_mc_CrossSection_SoftDIS = new PlotUtils::MnvH1D(*mc_CrossSection_SoftDIS);
            h_mc_CrossSection_TrueDIS = new PlotUtils::MnvH1D(*mc_CrossSection_TrueDIS);
            h_mc_CrossSection_Other   = new PlotUtils::MnvH1D(*mc_CrossSection_Other);
            
            h_mc_CrossSection         -> ClearAllErrorBands();
            h_mc_CrossSection_QE      -> ClearAllErrorBands();
            h_mc_CrossSection_MEC     -> ClearAllErrorBands();
            h_mc_CrossSection_RES     -> ClearAllErrorBands();
            h_mc_CrossSection_SoftDIS -> ClearAllErrorBands();
            h_mc_CrossSection_TrueDIS -> ClearAllErrorBands();
            h_mc_CrossSection_Other   -> ClearAllErrorBands();
            
            h_mc_CrossSection         -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_QE      -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_MEC     -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_RES     -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_SoftDIS -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_TrueDIS -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
            h_mc_CrossSection_Other   -> AddMissingErrorBandsAndFillWithCV(*h_data_CrossSection);
        }
        
        
        // Plot data-MC
        // ============
        
        // GENIE 2
        if ( use_genie2 ) {
            PlotDataMC_XsecModels(plot_info,
                                  h_data_CrossSection, h_mc_CrossSection,
                                  h_mc_CrossSection_QE,
                                  h_mc_CrossSection_MEC,
                                  h_mc_CrossSection_DeltaRES,
                                  h_mc_CrossSection_OtherRES,
                                  h_mc_CrossSection_SoftDIS,
                                  h_mc_CrossSection_TrueDIS,
                                  h_mc_CrossSection_Other,
                                  output_topdir + "/DataMC_" + option_material,
                                  xsec_title + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
            
            PlotDataMC_XsecModels(plot_info,
                                  h_data_CrossSection, h_mc_CrossSection,
                                  h_mc_CrossSection_QE,
                                  h_mc_CrossSection_MEC,
                                  h_mc_CrossSection_DeltaRES,
                                  h_mc_CrossSection_OtherRES,
                                  h_mc_CrossSection_SoftDIS,
                                  h_mc_CrossSection_TrueDIS,
                                  h_mc_CrossSection_Other,
                                  output_topdir + "/DataMC_ShapeOnly_" + option_material,
                                  xsec_title + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                  "TR", -1.0, -1.0, true, false, true);
        }
        
        // GENIE 3 or NEUT
        else if ( use_genie3 || use_neut ) {
            PlotDataMC_XsecModels(plot_info,
                                  h_data_CrossSection, h_mc_CrossSection,
                                  h_mc_CrossSection_QE,
                                  h_mc_CrossSection_MEC,
                                  h_mc_CrossSection_RES,
                                  h_mc_CrossSection_SoftDIS,
                                  h_mc_CrossSection_TrueDIS,
                                  h_mc_CrossSection_Other,
                                  output_topdir + "/DataMC_" + option_material,
                                  xsec_title + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
            
            PlotDataMC_XsecModels(plot_info,
                                  h_data_CrossSection, h_mc_CrossSection,
                                  h_mc_CrossSection_QE,
                                  h_mc_CrossSection_MEC,
                                  h_mc_CrossSection_RES,
                                  h_mc_CrossSection_SoftDIS,
                                  h_mc_CrossSection_TrueDIS,
                                  h_mc_CrossSection_Other,
                                  output_topdir + "/DataMC_ShapeOnly_" + option_material,
                                  xsec_title + material_title,
                                  "Muon transverse momentum [GeV/c]",
                                  "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                  "TR", -1.0, -1.0, true, false, true);
        }
        
        
        // Plot data-MC ratio
        // ==================
        
        double Ymax = 0.0;
        if ( option_material == "lead" ) {
            if ( option_xsec_model == "v0" )                Ymax = 2.5;
            else if ( option_xsec_model == "v1" )           Ymax = 2.5;
            else if ( option_xsec_model == "v1noNonResPi" ) Ymax = 2.5;
            else if ( option_xsec_model == "v1noD2" )       Ymax = 2.5;
            else if ( option_xsec_model == "v1noPionTune" ) Ymax = 2.5;
            else if ( option_xsec_model == "v2MINOS" )      Ymax = 2.5;
            else if ( option_xsec_model == "v2JOINT" )      Ymax = 2.5;
            else if ( option_xsec_model == "v2NU1PI" )      Ymax = 2.5;
            else if ( option_xsec_model == "v2NUNPI" )      Ymax = 2.5;
            else if ( option_xsec_model == "v2NUPI0" )      Ymax = 2.5;
            else if ( option_xsec_model == "v2MENU1PI" )    Ymax = 2.5;
            else if ( option_xsec_model == "GENIE3_02a" )   Ymax = 2.5;
            else if ( option_xsec_model == "GENIE3_02b" )   Ymax = 2.5;
            else if ( option_xsec_model == "GENIE3_10a" )   Ymax = 2.2;
            else if ( option_xsec_model == "GENIE3_10b" )   Ymax = 2.5;
            else if ( option_xsec_model == "NEUT_LFG" )     Ymax = 2.5;
        }
        else if ( option_material == "iron" ) {
            if ( option_xsec_model == "v0" )                Ymax = 4.5;
            else if ( option_xsec_model == "v1" )           Ymax = 4.5;
            else if ( option_xsec_model == "v1noNonResPi" ) Ymax = 4.5;
            else if ( option_xsec_model == "v1noD2" )       Ymax = 4.5;
            else if ( option_xsec_model == "v1noPionTune" ) Ymax = 4.5;
            else if ( option_xsec_model == "v2MINOS" )      Ymax = 5.5;
            else if ( option_xsec_model == "v2JOINT" )      Ymax = 5.5;
            else if ( option_xsec_model == "v2NU1PI" )      Ymax = 5.5;
            else if ( option_xsec_model == "v2NUNPI" )      Ymax = 5.5;
            else if ( option_xsec_model == "v2NUPI0" )      Ymax = 7.0;
            else if ( option_xsec_model == "v2MENU1PI" )    Ymax = 12.0;
            else if ( option_xsec_model == "GENIE3_02a" )   Ymax = 4.5;
            else if ( option_xsec_model == "GENIE3_02b" )   Ymax = 4.5;
            else if ( option_xsec_model == "GENIE3_10a" )   Ymax = 4.5;
            else if ( option_xsec_model == "GENIE3_10b" )   Ymax = 4.5;
            else if ( option_xsec_model == "NEUT_LFG" )     Ymax = 4.5;
        }
        
        // GENIE 2
        if ( use_genie2 ) {
            PlotDataMCRatio_XsecModels(plot_info,
                                       h_data_CrossSection, h_mc_CrossSection,
                                       h_mc_CrossSection_QE,
                                       h_mc_CrossSection_MEC,
                                       h_mc_CrossSection_DeltaRES,
                                       h_mc_CrossSection_OtherRES,
                                       h_mc_CrossSection_SoftDIS,
                                       h_mc_CrossSection_TrueDIS,
                                       h_mc_CrossSection_Other,
                                       output_topdir + "/DataMCRatio_" + option_material,
                                       xsec_title + material_title,
                                       "Muon transverse momentum [GeV/c]",
                                       "Data / MC", "TR", 0.0, Ymax);
        }
        
        // GENIE 3 and NEUT
        else if ( use_genie3 || use_neut ) {
            PlotDataMCRatio_XsecModels(plot_info,
                                       h_data_CrossSection, h_mc_CrossSection,
                                       h_mc_CrossSection_QE,
                                       h_mc_CrossSection_MEC,
                                       h_mc_CrossSection_RES,
                                       h_mc_CrossSection_SoftDIS,
                                       h_mc_CrossSection_TrueDIS,
                                       h_mc_CrossSection_Other,
                                       output_topdir + "/DataMCRatio_" + option_material,
                                       xsec_title + material_title,
                                       "Muon transverse momentum [GeV/c]",
                                       "Data / MC", "TR", 0.0, Ymax);
        }
        
        
        // Plot data-MC stacked
        // ====================
        
        // GENIE 2
        if ( use_genie2 ) {
            PlotDataStackedMC_XsecModels(plot_info,
                                         h_data_CrossSection, h_mc_CrossSection,
                                         h_mc_CrossSection_QE,
                                         h_mc_CrossSection_MEC,
                                         h_mc_CrossSection_DeltaRES,
                                         h_mc_CrossSection_OtherRES,
                                         h_mc_CrossSection_SoftDIS,
                                         h_mc_CrossSection_TrueDIS,
                                         h_mc_CrossSection_Other,
                                         output_topdir + "/DataStackedMC_" + option_material,
                                         xsec_title + material_title,
                                         "Muon transverse momentum [GeV/c]",
                                         "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
            
            PlotDataStackedMC_XsecModels(plot_info,
                                         h_data_CrossSection, h_mc_CrossSection,
                                         h_mc_CrossSection_QE,
                                         h_mc_CrossSection_MEC,
                                         h_mc_CrossSection_DeltaRES,
                                         h_mc_CrossSection_OtherRES,
                                         h_mc_CrossSection_SoftDIS,
                                         h_mc_CrossSection_TrueDIS,
                                         h_mc_CrossSection_Other,
                                         output_topdir + "/DataStackedMC_ShapeOnly_" + option_material,
                                         xsec_title + material_title,
                                         "Muon transverse momentum [GeV/c]",
                                         "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                         "TR", -1.0, -1.0, true, false, true);
        }
        
        // GENIE 3 and NEUT
        else if ( use_genie3 || use_neut ) {
            PlotDataStackedMC_XsecModels(plot_info,
                                         h_data_CrossSection, h_mc_CrossSection,
                                         h_mc_CrossSection_QE,
                                         h_mc_CrossSection_MEC,
                                         h_mc_CrossSection_RES,
                                         h_mc_CrossSection_SoftDIS,
                                         h_mc_CrossSection_TrueDIS,
                                         h_mc_CrossSection_Other,
                                         output_topdir + "/DataStackedMC_" + option_material,
                                         xsec_title + material_title,
                                         "Muon transverse momentum [GeV/c]",
                                         "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} [cm^{2}/GeV/c/nucleon]");
            
            PlotDataStackedMC_XsecModels(plot_info,
                                         h_data_CrossSection, h_mc_CrossSection,
                                         h_mc_CrossSection_QE,
                                         h_mc_CrossSection_MEC,
                                         h_mc_CrossSection_RES,
                                         h_mc_CrossSection_SoftDIS,
                                         h_mc_CrossSection_TrueDIS,
                                         h_mc_CrossSection_Other,
                                         output_topdir + "/DataStackedMC_ShapeOnly_" + option_material,
                                         xsec_title + material_title,
                                         "Muon transverse momentum [GeV/c]",
                                         "#font[12]{d}#sigma/#font[12]{dp}_{#mu,#font[132]{T}} (Arbitrary units)",
                                         "TR", -1.0, -1.0, true, false, true);
        }
        
        
        // Delete dummy histograms
        delete h_data_CrossSection;
        delete h_mc_CrossSection;
        delete h_mc_CrossSection_QE;
        delete h_mc_CrossSection_MEC;
        if ( use_genie2 ) delete h_mc_CrossSection_DeltaRES;
        if ( use_genie2 ) delete h_mc_CrossSection_OtherRES;
        if ( use_genie3 || use_neut ) delete h_mc_CrossSection_RES;
        delete h_mc_CrossSection_SoftDIS;
        delete h_mc_CrossSection_TrueDIS;
        delete h_mc_CrossSection_Other;
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotXsectionModels(std::string option_date,
                        std::string option_base_model,
                        std::string option_xsec_model,
                        std::string option_bkg_fit_function = "Bilinear")
{
    // Get MC base model for MacroUtil
    EnumModels type_model;
    GetModel(option_base_model, type_model);
    
    
    // Define generator to use
    bool use_genie2 = false;
    bool use_genie3 = false;
    bool use_neut   = false;
    
    if ( option_xsec_model == "v0" || option_xsec_model == "v1" ||
         option_xsec_model == "v1noNonResPi" || option_xsec_model == "v1noD2" || option_xsec_model == "v1noPionTune" ||
         option_xsec_model == "v2MINOS" || option_xsec_model == "v2JOINT" || option_xsec_model == "v2NU1PI" ||
         option_xsec_model == "v2NUNPI" || option_xsec_model == "v2NUPI0" || option_xsec_model == "v2MENU1PI" ) {
        use_genie2 = true;
    }
    else if ( option_xsec_model == "GENIE3_02a" || option_xsec_model == "GENIE3_02b" ||
              option_xsec_model == "GENIE3_10a" || option_xsec_model == "GENIE3_10b" ) {
        use_genie3 = true;
    }
    else if ( option_xsec_model == "NEUT_LFG" ) {
        use_neut = true;
    }
    else {
        std::cout << " BAD X-SECTION MODEL!!! " << std::endl;
        exit(1);
    }
    
    
    // Options for input/output file
    const std::string option_date_data = option_date + "_" + option_base_model;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // X-section models
    std::string xsec_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/%s/lead", option_date_data.c_str());
    std::string xsec_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/%s/iron", option_date_data.c_str());
    
    // Data
    std::string data_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/lead", option_date_data.c_str());
    std::string data_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/iron", option_date_data.c_str());
    
    
    // Read input files
    // ================
    
    // X-section models
    TFile* xsec_fin_lead;
    TFile* xsec_fin_iron;
    
    if ( use_genie2 ) {
        xsec_fin_lead = new TFile(Form("%s/XsectionModels_MnvGENIE%s_lead.root", xsec_fin_topdir_lead.c_str(),
                                                                                 option_xsec_model.c_str()), "READ");
        
        xsec_fin_iron = new TFile(Form("%s/XsectionModels_MnvGENIE%s_iron.root", xsec_fin_topdir_iron.c_str(),
                                                                                 option_xsec_model.c_str()), "READ");
    }
    else if ( use_genie3 || use_neut ) {
        xsec_fin_lead = new TFile(Form("%s/XsectionModels_%s_lead.root", xsec_fin_topdir_lead.c_str(),
                                                                         option_xsec_model.c_str()), "READ");
        
        xsec_fin_iron = new TFile(Form("%s/XsectionModels_%s_iron.root", xsec_fin_topdir_iron.c_str(),
                                                                         option_xsec_model.c_str()), "READ");
    }
    
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
    Plot(util, xsec_fin_lead, data_fin_lead, option_date_data, option_xsec_model, "lead",
         use_genie2, use_genie3, use_neut);
    
    
    // Iron
    Plot(util, xsec_fin_iron, data_fin_iron, option_date_data, option_xsec_model, "iron",
         use_genie2, use_genie3, use_neut);
    
    
    // Close ROOT files
    xsec_fin_lead -> Close();
    xsec_fin_iron -> Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
}


#endif  // PlotXsectionModels_C