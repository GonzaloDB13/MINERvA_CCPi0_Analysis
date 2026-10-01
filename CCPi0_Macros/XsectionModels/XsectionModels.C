#ifndef XsectionModels_C
#define XsectionModels_C

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
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#endif  // __CINT__

#include "TFile.h"





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void XsectionModels(std::string option_date,
                    std::string option_base_model,
                    std::string option_xsec_model,
                    std::string option_material,
                    std::string option_bkg_fit_function = "Bilinear")
{
    // Get MC base model for MacroUtil
    EnumModels type_model;
    GetModel(option_base_model, type_model);
    
    
    // Options for input/output files
    const std::string option_date_mc   = option_date + "_" + option_xsec_model;
    const std::string option_date_data = option_date + "_" + option_base_model;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // MC x-section model
    std::string mc_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/FakeDataModels/%s/%s", option_date_mc.c_str(),
                                                                                                        option_material.c_str());
    
    // Data
    std::string data_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/%s", option_date_data.c_str(),
                                                                                                                  option_material.c_str());
    
    
    // Define input files
    // ==================
    
    // MC x-section model
    TFile mc_fin(Form("%s/MC_FakeData_MnvGENIE%s_WithSyst_POTScaled_AllPlaylists_%s.root", mc_fin_topdir.c_str(),
                                                                                           option_xsec_model.c_str(),
                                                                                           option_material.c_str()), "READ");
    
    // Data
    TFile data_fin(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_%s.root", data_fin_topdir.c_str(),
                                                                       option_base_model.c_str(),
                                                                       option_bkg_fit_function.c_str(),
                                                                       option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  Output file
    // =========================================
    
    // Top directory
    std::string fout_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/%s/%s", option_date_data.c_str(),
                                                                                                      option_material.c_str());
    
    // Create output file
    TFile fout(Form("%s/XsectionModels_MnvGENIE%s_%s.root", fout_topdir.c_str(),
                                                            option_xsec_model.c_str(),
                                                            option_material.c_str()), "RECREATE");
    
    
    
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
    SetMacroUtilPOT(mc_fin, data_fin, util);
    
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << " \tData POT: " << util.m_data_pot << std::endl;
    std::cout << std::endl;
    
    WritePOT(fout, util.m_mc_pot, util.m_data_pot);
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    
    // =========================================
    //  Get x-sections from eff. denominators
    // =========================================
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Get variable name
        std::string var_name = var->Name();
        
        
        // Get x-section model eff. denominators
        PlotUtils::MnvH1D* h_mc_CrossSection          = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True",          var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_QE       = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_QE",       var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_MEC      = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_MEC",      var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_DeltaRES = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_DeltaRES", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_OtherRES = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_OtherRES", var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_SoftDIS  = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_SoftDIS",  var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_TrueDIS  = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_TrueDIS",  var_name.c_str()));
        PlotUtils::MnvH1D* h_mc_CrossSection_Other    = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffDenominator_%s_True_Other",    var_name.c_str()));
        
        h_mc_CrossSection          -> SetName(Form("mc_CrossSection_%s",          var_name.c_str()));
        h_mc_CrossSection_QE       -> SetName(Form("mc_CrossSection_%s_QE",       var_name.c_str()));
        h_mc_CrossSection_MEC      -> SetName(Form("mc_CrossSection_%s_MEC",      var_name.c_str()));
        h_mc_CrossSection_DeltaRES -> SetName(Form("mc_CrossSection_%s_DeltaRES", var_name.c_str()));
        h_mc_CrossSection_OtherRES -> SetName(Form("mc_CrossSection_%s_OtherRES", var_name.c_str()));
        h_mc_CrossSection_SoftDIS  -> SetName(Form("mc_CrossSection_%s_SoftDIS",  var_name.c_str()));
        h_mc_CrossSection_TrueDIS  -> SetName(Form("mc_CrossSection_%s_TrueDIS",  var_name.c_str()));
        h_mc_CrossSection_Other    -> SetName(Form("mc_CrossSection_%s_Other",    var_name.c_str()));
        
        
        // Get integrated flux and number of nucleons
        PlotUtils::MnvH1D* h_IntegrFlux = (PlotUtils::MnvH1D*)data_fin.Get("IntegratedFlux");
        
        PlotUtils::MnvH1D* h_mc_Nnucleons = (PlotUtils::MnvH1D*)data_fin.Get("mc_Nnucleons");
        double mc_Nnucleons = h_mc_Nnucleons->GetBinContent(1);
        
        
        // Add missing error bands
        h_mc_CrossSection          -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_QE       -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_MEC      -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_DeltaRES -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_OtherRES -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_SoftDIS  -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_TrueDIS  -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        h_mc_CrossSection_Other    -> AddMissingErrorBandsAndFillWithCV(*h_IntegrFlux);
        
        
        // Divide eff. denominator over flux
        h_mc_CrossSection          -> Divide(h_mc_CrossSection,          h_IntegrFlux);
        h_mc_CrossSection_QE       -> Divide(h_mc_CrossSection_QE,       h_IntegrFlux);
        h_mc_CrossSection_MEC      -> Divide(h_mc_CrossSection_MEC,      h_IntegrFlux);
        h_mc_CrossSection_DeltaRES -> Divide(h_mc_CrossSection_DeltaRES, h_IntegrFlux);
        h_mc_CrossSection_OtherRES -> Divide(h_mc_CrossSection_OtherRES, h_IntegrFlux);
        h_mc_CrossSection_SoftDIS  -> Divide(h_mc_CrossSection_SoftDIS,  h_IntegrFlux);
        h_mc_CrossSection_TrueDIS  -> Divide(h_mc_CrossSection_TrueDIS,  h_IntegrFlux);
        h_mc_CrossSection_Other    -> Divide(h_mc_CrossSection_Other,    h_IntegrFlux);
        
        
        // Divide by number of targets and data POT
        h_mc_CrossSection          -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_QE       -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_MEC      -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_DeltaRES -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_OtherRES -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_SoftDIS  -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_TrueDIS  -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        h_mc_CrossSection_Other    -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
        
        
        // Write x-section histograms
        fout.cd();
        
        h_mc_CrossSection          -> Write();
        h_mc_CrossSection_QE       -> Write();
        h_mc_CrossSection_MEC      -> Write();
        h_mc_CrossSection_DeltaRES -> Write();
        h_mc_CrossSection_OtherRES -> Write();
        h_mc_CrossSection_SoftDIS  -> Write();
        h_mc_CrossSection_TrueDIS  -> Write();
        h_mc_CrossSection_Other    -> Write();
    }
    
    
    // Close ROOT files
    mc_fin.Close();
    data_fin.Close();
    fout.Close();
}


#endif  // XsectionModels_C