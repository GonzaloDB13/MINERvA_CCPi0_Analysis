#ifndef PlotSupportStudies_C
#define PlotSupportStudies_C

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
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/SupportStudies/plots/%s/%s/", option_date_mc.c_str(),
                                                                                                    option_material.c_str());
    
    
    // Get variables
    std::vector<Variable*>   variables   = GetSupportVariables();
    std::vector<Variable2D*> variables2D = GetSupportVariables2D();
    
    
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
    for ( auto var : variables ) {
        var -> LoadMCHists_MatSelection(mc_fin, util.m_error_bands);
        var -> LoadDataHists_MatSelection(data_fin);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> LoadMCHists_MatSelection2D(mc_fin, util.m_error_bands);
    }
    
    
    
    // =========================================
    //  Loop over 1D variables for plotting
    // =========================================
    
    for ( auto var : variables )
    {
        // Construct plot info object
        PlotInfo plot_info(var, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Plot title
        std::string title_str;
        std::string conf_dir;
        
        if ( var->Name() == "MuonVertexPlane" )  title_str = "Muon vertex plane";
        else if ( var->Name() == "Gamma1TrueE" ) title_str = "Leading photon";
        else if ( var->Name() == "Gamma2TrueE" ) title_str = "Secondary photon";
        
        
        // Plot muon vertex plane
        if ( var->Name() == "MuonVertexPlane" ) {
            PlotDataStackedMC(plot_info,
                              var->m_hists.m_data_MatSelection, var->m_hists.m_mc_MatSelection.hist,
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt4Pb.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt5Pb.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt5Fe.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasUp.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasBetw.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasDown.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_MatSelection_TrueOtherMat.univHist(cv_univ),
                              output_topdir + var->Name() + "_" + option_material,
                              title_str, "Plane number", "Events / plane", "TL");
            
            PlotUtils::MnvH1D* mnv_TrueTgt4Pb_clone   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt4Pb.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TrueTgt5Pb_clone   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt5Pb.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TrueTgt5Fe_clone   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TrueTgt5Fe.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TruePlasUp_clone   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasUp.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TruePlasBetw_clone = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasBetw.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TruePlasDown_clone = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TruePlasDown.univHist(cv_univ)->Clone("");
            PlotUtils::MnvH1D* mnv_TrueOtherMat_clone = (PlotUtils::MnvH1D*)var->m_hists.m_mc_MatSelection_TrueOtherMat.univHist(cv_univ)->Clone("");
            
            for ( int bin = 1; bin <= mnv_TrueTgt4Pb_clone->GetNbinsX(); ++bin ) {
                double tgt4pb   = mnv_TrueTgt4Pb_clone->GetBinContent(bin);
                double tgt5pb   = mnv_TrueTgt5Pb_clone->GetBinContent(bin);
                double tgt5fe   = mnv_TrueTgt5Fe_clone->GetBinContent(bin);
                double plasup   = mnv_TruePlasUp_clone->GetBinContent(bin);
                double plasbetw = mnv_TruePlasBetw_clone->GetBinContent(bin);
                double plasdown = mnv_TruePlasDown_clone->GetBinContent(bin);
                double othermat = mnv_TrueOtherMat_clone->GetBinContent(bin);
                double total    = tgt4pb + tgt5pb + tgt5fe + plasup + plasbetw + plasdown + othermat;
                
                if ( total > 0.0 ) {
                    mnv_TrueTgt4Pb_clone   -> SetBinContent(bin, tgt4pb/total);
                    mnv_TrueTgt5Pb_clone   -> SetBinContent(bin, tgt5pb/total);
                    mnv_TrueTgt5Fe_clone   -> SetBinContent(bin, tgt5fe/total);
                    mnv_TruePlasUp_clone   -> SetBinContent(bin, plasup/total);
                    mnv_TruePlasBetw_clone -> SetBinContent(bin, plasbetw/total);
                    mnv_TruePlasDown_clone -> SetBinContent(bin, plasdown/total);
                    mnv_TrueOtherMat_clone -> SetBinContent(bin, othermat/total);
                }
                else {
                    mnv_TrueTgt4Pb_clone   -> SetBinContent(bin, 0.0);
                    mnv_TrueTgt5Pb_clone   -> SetBinContent(bin, 0.0);
                    mnv_TrueTgt5Fe_clone   -> SetBinContent(bin, 0.0);
                    mnv_TruePlasUp_clone   -> SetBinContent(bin, 0.0);
                    mnv_TruePlasBetw_clone -> SetBinContent(bin, 0.0);
                    mnv_TruePlasDown_clone -> SetBinContent(bin, 0.0);
                    mnv_TrueOtherMat_clone -> SetBinContent(bin, 0.0);
                }
            }
            
            PlotStackedMC(plot_info,
                          var->m_hists.m_mc_MatSelection.hist,
                          mnv_TrueTgt4Pb_clone,
                          mnv_TrueTgt5Pb_clone,
                          mnv_TrueTgt5Fe_clone,
                          mnv_TruePlasUp_clone,
                          mnv_TruePlasBetw_clone,
                          mnv_TruePlasDown_clone,
                          mnv_TrueOtherMat_clone,
                          output_topdir + "MuonVertexPlaneRatio_" + option_material,
                          "Fraction of predicted events per plane", "Plane number", "MC fraction / plane");
        }
        
        
        // Plot true photon energy
        if ( var->Name() == "Gamma1TrueE" || var->Name() == "Gamma2TrueE" ) {
            Plot1D(plot_info,
                   var->m_hists.m_mc_MatSelection.hist,
                   output_topdir + var->Name() + "_" + option_material, title_str + material_title, "", "Photons / 50 MeV");
        }
    }
    
    
    
    // =========================================
    //  Loop over 2D variables for plotting
    // =========================================
    
    for ( auto var2D : variables2D )
    {
        // Construct plot info object
        PlotInfo plot_info(var2D, mc_pot, data_pot,
                           do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
        
        
        // Plot title
        std::string title_str;
        std::string conf_dir;
        
        if ( var2D->Name() == "Gamma1EnergyLoss" )      title_str = "Leading photon";
        else if ( var2D->Name() == "Gamma2EnergyLoss" ) title_str = "Secondary photon";
        
        
        // Vectors of functions/vertlines and ther limits
        std::vector<std::string> functions = {"x"};
        std::vector<double> x1_functions = {0.0};
        std::vector<double> x2_functions = {2.5};
        std::vector<double> x_vertlines;
        std::vector<double> y1_vertlines;
        std::vector<double> y2_vertlines;
        
        
        // Plot 2D
        Plot2D(plot_info,
               var2D->m_hists2D.m_mc_MatSelection2D.hist,
               output_topdir + var2D->Name() + "_" + option_material,
               title_str + material_title,
               functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
               "Energy left in active material [GeV]", "True photon energy [GeV]", "Events / 25 MeV #times 25 MeV",
               true, true);
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotSupportStudies(std::string option_date,
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
    std::string mc_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/mc/%s/lead", option_date_mc.c_str());
    
    std::string mc_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/data/%s/lead", option_date_data.c_str());
    
    std::string data_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/data/%s/iron", option_date_data.c_str());
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_topdir_lead.c_str(),
                                                                                                  option_model.c_str(),
                                                                                                  option_systematics.c_str()), "READ");
    
    TFile mc_fin_iron(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_topdir_iron.c_str(),
                                                                                                  option_model.c_str(),
                                                                                                  option_systematics.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_SupportStudies_AllPlaylists_lead.root", data_fin_topdir_lead.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_SupportStudies_AllPlaylists_iron.root", data_fin_topdir_iron.c_str()), "READ");
    
    
    
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


#endif  // PlotSupportStudies_C