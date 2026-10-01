#ifndef PlotCutStudies_C
#define PlotCutStudies_C

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
#include "BookHistograms.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#include "../includes/Variable2D.h"
#endif  // __CINT__

#include "TFile.h"





// ========================================================================================================================
//  PLOT VARIABLES
// ========================================================================================================================

void PlotVariable(CCPi0::MacroUtil util,
                  TFile& mc_fin,
                  TFile& data_fin,
                  std::string option_date_mc,
                  std::string option_cut,
                  std::string option_material)
{
    // Construct plot output directory
    std::string output_topdir = Form("/minerva/data/users/gonzalo/MAT/CutStudies/plots/%s/%s", option_date_mc.c_str(),
                                                                                               option_material.c_str());
    
    
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
    
    
    
    // ============
    //  MICHEL CUT
    // ============
    
    if ( option_cut == "Michel" )
    {
        // Load variables
        std::vector<Variable*> variables = GetMichelVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title
            std::string title_str;
            if ( var->Name() == "MuonPt_BeforeMichel" )            title_str = "Muon #font[12]{p}_{#font[132]{T}} before Michel cut";
            else if ( var->Name() == "NstartPointVertexMichels" )  title_str = "Michels around start point vertices";
            else if ( var->Name() == "NstopPointVertexMichels" )   title_str = "Michels around stop point vertices";
            else if ( var->Name() == "NkinkedVertexMichels" )      title_str = "Michels around kinked vertices";
            else if ( var->Name() == "StartPointVertexMichelPdg" ) title_str = "Start point vertices Michel PDG";
            else if ( var->Name() == "StopPointVertexMichelPdg" )  title_str = "Stop point vertices Michel PDG";
            else if ( var->Name() == "KinkedVertexMichelPdg" )     title_str = "Kinked vertices Michel PDG";
            
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeMichel" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
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
                                  output_topdir + "/08-Michel/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of Michel candidates
            if ( var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
                arrow_X.push_back(1.0);
                arrow_Ymax.push_back(0.8);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("L");
                
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
                                  output_topdir + "/08-Michel/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
                
                PlotMCPurityPerBin_Selection(plot_info,
                                             (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                             output_topdir + "/08-Michel/" + var->Name() + "_SelRatio_" + option_material,
                                             title_str + material_title,
                                             arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                             var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
            }
            
            // MC signal/backgr stack of Michel true PDG
            if ( var->Name() == "StartPointVertexMichelPdg" || var->Name() == "StopPointVertexMichelPdg" || var->Name() == "KinkedVertexMichelPdg" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
                PlotStackedMC(plot_info,
                              var->m_hists.m_mc_Selection.hist,
                              (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasUp.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasBetw.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasDown.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrOther.univHist(cv_univ),
                              output_topdir + "/08-Michel/" + var->Name() + "_" + option_material,
                              title_str + material_title,
                              arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                              "", "Number of Michel candidates", "TR");
            }
        }
    }
    
    
    
    // ===========
    //  TRACK CUT
    // ===========
    
    if ( option_cut == "Track" )
    {
        // Load variables
        std::vector<Variable*> variables = GetTrackVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title
            std::string title_str;
            if ( var->Name() == "MuonPt_BeforeTrack" )      title_str = "Muon #font[12]{p}_{#font[132]{T}} before long track cut";
            else if ( var->Name() == "NprimTracks" )        title_str = "Number of primrary tracks";
            else if ( var->Name() == "NsecTracks" )         title_str = "Number of secondary tracks";
            else if ( var->Name() == "PrimTrackPionScore" ) title_str = "Pion log-likelihood ratio score";
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeTrack" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
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
                                  output_topdir + "/09-Track/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of primary and secondary tracks
            else if ( var->Name() == "NprimTracks" || var->Name() == "NsecTracks" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
                if ( var->Name() == "NprimTracks" ) {
                    arrow_X.push_back(2.0);
                    arrow_Ymax.push_back(0.7);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("L");
                }
                if ( var->Name() == "NsecTracks" ) {
                    arrow_X.push_back(1.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("L");
                }
                else if ( var->Name() == "PrimTrackPionScore" ) {
                    arrow_X.push_back(4.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.13);
                    arrow_dir.push_back("L");
                }
                
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
                                  output_topdir + "/09-Track/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
                
                PlotMCPurityPerBin_Selection(plot_info,
                                             (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                             output_topdir + "/09-Track/" + var->Name() + "_SelRatio_" + option_material,
                                             title_str + material_title,
                                             arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                             var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
            }
            
            // Primary track LLR score
            else if ( var->Name() == "PrimTrackPionScore" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
                arrow_X.push_back(4.0);
                arrow_Ymax.push_back(0.8);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("L");
                
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/09-Track/" + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of tracks", "TL");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/09-Track/" + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel, "PDG purity / bin", "TL");
            }
        }
    }
    
    
    
    // ===============
    //  ANGLESCAN CUT
    // ===============
    
    if ( option_cut == "AngleScan" )
    {
        // Load variables
        std::vector<Variable*> variables = GetAngleScanVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title
            std::string title_str;
            if ( var->Name() == "MuonPt_BeforeAngleScan" )      title_str = "Muon #font[12]{p}_{#font[132]{T}} before AngleScan cut";
            else if ( var->Name() == "AngleScanNblobs" )        title_str = "Shower candidates";
            else if ( var->Name() == "NblobsPassBasicQuality" ) title_str = "Shower cand. w/good quality";
            else if ( var->Name() == "NblobCandidates" )        title_str = "Shower cand. w/3D axis";
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeAngleScan" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
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
                                  output_topdir + "/10-AngleScan/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of shower candidates
            else if ( var->Name() == "AngleScanNblobs" || var->Name() == "NblobsPassBasicQuality" || var->Name() == "NblobCandidates" ) {
                std::vector<double> arrow_X;
                std::vector<double> arrow_Ymax;
                std::vector<double> arrow_length;
                std::vector<std::string> arrow_dir;
                
                arrow_X.push_back(1.0);
                arrow_Ymax.push_back(0.8);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("R");
                
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
                                  output_topdir + "/10-AngleScan/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
                
                PlotMCPurityPerBin_Selection(plot_info,
                                             (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                             output_topdir + "/10-AngleScan/" + var->Name() + "_SelRatio_" + option_material,
                                             title_str + material_title,
                                             arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                             var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
            }
        }
    }
    
    
    
    // ============================
    //  BLOB ANGLE W.R.T. MUON CUT
    // ============================
    
    if ( option_cut == "BlobAngleWRTMuon" )
    {
        // Load variables
        std::vector<Variable*> variables = GetBlobAngleWRTMuonVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            
            if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before shower angle #xi cut";
            }
            else if ( var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" ) {
                title_str  = "N_{showers} before shower angle #xi cut";
            }
            else if ( var->Name() == "BlobAngleWRTMuon_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/";
            }
            else if ( var->Name() == "BlobAngleWRTMuon_Multi" ) {
                title_str = "Multi-shower events";
                shower_dir = "MultiBlob/";
            }
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            if ( option_material == "lead" ) {
                if ( var->Name() == "BlobAngleWRTMuon_Single" ) {
                    arrow_X.push_back(12.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("R");
                }
                else if ( var->Name() == "BlobAngleWRTMuon_Multi" ) {
                    arrow_X.push_back(6.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("R");
                }
            }
            else if ( option_material == "iron" ) {
                if ( var->Name() == "BlobAngleWRTMuon_Single" ) {
                    arrow_X.push_back(12.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("R");
                }
                else if ( var->Name() == "BlobAngleWRTMuon_Multi" ) {
                    arrow_X.push_back(6.0);
                    arrow_Ymax.push_back(0.8);
                    arrow_length.push_back(0.15);
                    arrow_dir.push_back("R");
                }
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ) {
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
                                  output_topdir + "/11-BlobAngleWRTMuon/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" ) {
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
                                  output_topdir + "/11-BlobAngleWRTMuon/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Shower angle w.r.t. muon
            else {
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/11-BlobAngleWRTMuon/" + shower_dir + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers / 3 deg", "TR");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/11-BlobAngleWRTMuon/" + shower_dir + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel + " [" + var->Units() + "]", "PDG purity / bin", "TR");
            }
        }
    }
    
    
    
    // ====================
    //  BLOB DEVIATION CUT
    // ====================
    
    if ( option_cut == "BlobDeviation" )
    {
        // Load variables
        std::vector<Variable*> variables     = GetBlobDeviationVariables();
        std::vector<Variable2D*> variables2D = GetBlobDeviationVariables2D();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        LoadVariables2D(util, mc_fin, kMC, variables2D, option_cut);
        LoadVariables2D(util, data_fin, kData, variables2D, option_cut);
        
        
        // 1D variables
        // ============
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string unit_per_bin;
            
            if ( var->Name() == "MuonPt_BeforeBlobDeviation" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before shower deviation cut";
            }
            else if ( var->Name() == "Nblobs_BeforeBlobDeviation" ) {
                title_str  = "N_{showers} before shower deviation cut";
            }
            else if ( var->Name() == "BlobProjDeviation_Single" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/ProjDeviation/";
                unit_per_bin = "20 mm";
            }
            else if ( var->Name() == "BlobAngleDeviation_Single" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/AngleDeviation/";
                unit_per_bin = "3 deg";
            }
            else if ( var->Name() == "BlobProjDeviation_Multi" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/ProjDeviation/";
                unit_per_bin = "20 mm";
            }
            else if ( var->Name() == "BlobAngleDeviation_Multi" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/AngleDeviation/";
                unit_per_bin = "3 deg";
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeBlobDeviation" ) {
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
                                  output_topdir + "/12-BlobDeviation/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_BeforeBlobDeviation" ) {
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
                                  output_topdir + "/12-BlobDeviation/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Shower deviation
            else {
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/12-BlobDeviation/" + shower_dir + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers / " + unit_per_bin, "TR");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/12-BlobDeviation/" + shower_dir + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel + " [" + var->Units() + "]", "PDG purity / bin", "TR");
                
                PlotMCUnitNorm_PdgV1(plot_info,
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                     output_topdir + "/12-BlobDeviation/" + shower_dir + var->Name() + "_PdgUnitNorm_" + option_material,
                                     title_str + material_title,
                                     arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                     var->m_hists.m_xlabel + " [" + var->Units() + "]", "Arbitrary units", "TR");
            }
        }
        
        
        // 2D variables
        // ============
        for ( auto var2D : variables2D )
        {
            // Construct plot info object
            PlotInfo plot_info(var2D, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string output_str;
            std::string xlabel_str   = var2D->m_hists2D.m_xlabel + " [" + var2D->Xunits() + "]";
            std::string ylabel_str   = var2D->m_hists2D.m_ylabel + " [" + var2D->Yunits() + "]";
            std::string unit_per_bin = "20 mm #times 3 deg";
            
            if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/AngleDeviationVSProjDeviation/";
                output_str = output_topdir + "/12-BlobDeviation/" + shower_dir + var2D->Name();
            }
            else if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/AngleDeviationVSProjDeviation/";
                output_str = output_topdir + "/12-BlobDeviation/" + shower_dir + var2D->Name();
            }
            
            // Function and vertical lines inputs
            std::vector<std::string> functions;
            std::vector<double> x1_functions;
            std::vector<double> x2_functions;
            std::vector<double> x_vertlines;
            std::vector<double> y1_vertlines;
            std::vector<double> y2_vertlines;
            
            if ( option_material == "lead" ) {
                if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Single" ) {
                    functions.push_back("90.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(125.0);
                    functions.push_back("18.0");
                    x1_functions.push_back(125.0);
                    x2_functions.push_back(350.0);
                    // functions.push_back("0.173913 + 0.142609*x");
                    // x1_functions.push_back(125.0);
                    // x2_functions.push_back(700.0);
                    // functions.push_back("100");
                    // x1_functions.push_back(700.0);
                    // x2_functions.push_back(1500.0);
                    x_vertlines.push_back(125.0);
                    y1_vertlines.push_back(18.0);
                    y2_vertlines.push_back(90.0);
                    x_vertlines.push_back(350.0);
                    y1_vertlines.push_back(0.0);
                    y2_vertlines.push_back(18.0);
                }
                else if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Multi" ) {
                    functions.push_back("90.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(180.0);
                    functions.push_back("21.0");
                    x1_functions.push_back(180.0);
                    x2_functions.push_back(400.0);
                    // functions.push_back("-4.09091 + 0.0627273*x");
                    // x1_functions.push_back(400.0);
                    // x2_functions.push_back(1500.0);
                    x_vertlines.push_back(180.0);
                    y1_vertlines.push_back(21.0);
                    y2_vertlines.push_back(90.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(0.0);
                    y2_vertlines.push_back(21.0);
                }
            }
            else if ( option_material == "iron" ) {
                if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Single" ) {
                    functions.push_back("50.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("21.0");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    // functions.push_back("12.2222 + 0.087778*x");
                    // x1_functions.push_back(100.0);
                    // x2_functions.push_back(1000.0);
                    // functions.push_back("100");
                    // x1_functions.push_back(1000.0);
                    // x2_functions.push_back(1500.0);
                    x_vertlines.push_back(100.0);
                    y1_vertlines.push_back(21.0);
                    y2_vertlines.push_back(50.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(0.0);
                    y2_vertlines.push_back(21.0);
                }
                else if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Multi" ) {
                    functions.push_back("60.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(150.0);
                    functions.push_back("25.0");
                    x1_functions.push_back(150.0);
                    x2_functions.push_back(400.0);
                    // functions.push_back("17.0588 + 0.0529412*x");
                    // x1_functions.push_back(150.0);
                    // x2_functions.push_back(1500.0);
                    x_vertlines.push_back(150.0);
                    y1_vertlines.push_back(25.0);
                    y2_vertlines.push_back(60.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(0.0);
                    y2_vertlines.push_back(25.0);
                }
            }
            
            // MC and data
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D.hist,
                   output_str + "_MC_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_data_ObjectPdg2D,
                   output_str + "_Data_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            // PDG purity
            PlotMCPurityPerBin_Pdg2D(plot_info,
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_EM.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.univHist(cv_univ),
                                     output_str, title_str + material_title, option_material,
                                     functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                                     xlabel_str, ylabel_str, "",
                                     false, false);
        }
    }
    
    
    
    // =======================
    //  BLOB ENERGY VS dx CUT
    // =======================
    
    if ( option_cut == "BlobEnergyVSdx" )
    {
        // Load variables
        std::vector<Variable*> variables     = GetBlobEnergyVSdxVariables();
        std::vector<Variable2D*> variables2D = GetBlobEnergyVSdxVariables2D();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        LoadVariables2D(util, mc_fin, kMC, variables2D, option_cut);
        LoadVariables2D(util, data_fin, kData, variables2D, option_cut);
        
        
        // 1D variables
        // ============
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string unit_per_bin;
            
            if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before #font[12]{E}_{#font[132]{shower}} VS #font[12]{dx} cut";
            }
            else if ( var->Name() == "Nblobs_BeforeBlobEnergyVSdx" ) {
                title_str  = "N_{showers} before #font[12]{E}_{#font[132]{shower}} VS #font[12]{dx} cut";
            }
            else if ( var->Name() == "BlobEcalo_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobEcalo_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ) {
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
                                  output_topdir + "/13-BlobEcaloVSdx/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_BeforeBlobEnergyVSdx" ) {
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
                                  output_topdir + "/13-BlobEcaloVSdx/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Shower energy and similar variables
            else {
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers / " + unit_per_bin, "TR");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel + " [" + var->Units() + "]", "PDG purity / bin", "TR");
                
                PlotMCUnitNorm_PdgV2(plot_info,
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                     output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var->Name() + "_PdgUnitNorm_" + option_material,
                                     title_str + material_title,
                                     arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                     var->m_hists.m_xlabel + " [" + var->Units() + "]", "Arbitrary units", "TR");
            }
        }
        
        
        // 2D variables
        // ============
        for ( auto var2D : variables2D )
        {
            // Construct plot info object
            PlotInfo plot_info(var2D, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string output_str;
            std::string xlabel_str = var2D->m_hists2D.m_xlabel + " [" + var2D->Xunits() + "]";
            std::string ylabel_str = var2D->m_hists2D.m_ylabel + " [" + var2D->Yunits() + "]";
            std::string unit_per_bin;
            
            if ( var2D->Name() == "BlobEcaloVSdx_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/EcaloVSdx/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part1" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/EcaloVSdx/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part1" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/13-BlobEcaloVSdx/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            
            // Function and vertical lines inputs
            std::vector<std::string> functions;
            std::vector<double> x1_functions;
            std::vector<double> x2_functions;
            std::vector<double> x_vertlines;
            std::vector<double> y1_vertlines;
            std::vector<double> y2_vertlines;
            
            if ( option_material == "lead" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part1" ) {
                    functions.push_back("360.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    functions.push_back("0.0375*x*x + 2.85*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    x_vertlines.push_back(50.0);
                    y1_vertlines.push_back(237.0);
                    y2_vertlines.push_back(360.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part1" ) {
                    functions.push_back("300 + 2.5*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    x_vertlines.push_back(40.0);
                    y1_vertlines.push_back(200.0);
                    y2_vertlines.push_back(400.5);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part1" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part1" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part1" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.0 + 0.01*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part1" ) {
                    functions.push_back("6.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("5.33 + 0.00667*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part1" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.05882 + 0.00294118*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part1" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(150.0);
                //     functions.push_back("2.5 + 0.03*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(250.0);
                //     functions.push_back("1.875 + 0.003125*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(250.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            else if ( option_material == "iron" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part1" ) {
                    functions.push_back("340.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    functions.push_back("4.54545*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    x_vertlines.push_back(55.0);
                    y1_vertlines.push_back(250.0);
                    y2_vertlines.push_back(340.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part1" ) {
                    functions.push_back("240");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    x_vertlines.push_back(34.0);
                    y1_vertlines.push_back(170.0);
                    y2_vertlines.push_back(240.0);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part1" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part1" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part1" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("2.67 + 0.0133*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part1" ) {
                    functions.push_back("5.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.75 + 0.0125*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(260.0);
                    x_vertlines.push_back(260.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part1" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(350.0);
                //     functions.push_back("2.64706 + 0.00235294*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(350.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part1" ) {
                //     functions.push_back("6.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("-2.0 + 0.04*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.22222 + 0.00278*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            
            // MC and data
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D.hist,
                   output_str + "_MC_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_data_ObjectPdg2D,
                   output_str + "_Data_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            // PDG purity
            PlotMCPurityPerBin_Pdg2D(plot_info,
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_EM.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.univHist(cv_univ),
                                     output_str, title_str + material_title, option_material,
                                     functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                                     xlabel_str, ylabel_str, "",
                                     false, false);
        }
    }
    
    
    
    // ==============================
    //  BLOB END dE/dx VS ENERGY CUT
    // ==============================
    
    if ( option_cut == "BlobdEdxEnd" )
    {
        // Load variables
        std::vector<Variable*> variables     = GetBlobdEdxEndVariables();
        std::vector<Variable2D*> variables2D = GetBlobdEdxEndVariables2D();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        LoadVariables2D(util, mc_fin, kMC, variables2D, option_cut);
        LoadVariables2D(util, data_fin, kData, variables2D, option_cut);
        
        
        // 1D variables
        // ============
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string unit_per_bin;
            
            if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before end #font[12]{dE/dx} cut";
            }
            else if ( var->Name() == "Nblobs_BeforeBlobdEdxEnd" ) {
                title_str  = "N_{showers} before end #font[12]{dE/dx} cut";
            }
            else if ( var->Name() == "BlobEcalo_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobEcalo_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ) {
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
                                  output_topdir + "/14-BlobdEdxEnd/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_BeforeBlobdEdxEnd" ) {
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
                                  output_topdir + "/14-BlobdEdxEnd/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Shower energy and similar variables
            else {
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers / " + unit_per_bin, "TR");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel + " [" + var->Units() + "]", "PDG purity / bin", "TR");
                
                PlotMCUnitNorm_PdgV2(plot_info,
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                     output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var->Name() + "_PdgUnitNorm_" + option_material,
                                     title_str + material_title,
                                     arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                     var->m_hists.m_xlabel + " [" + var->Units() + "]", "Arbitrary units", "TR");
            }
        }
        
        
        // 2D variables
        // ============
        for ( auto var2D : variables2D )
        {
            // Construct plot info object
            PlotInfo plot_info(var2D, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string output_str;
            std::string xlabel_str = var2D->m_hists2D.m_xlabel + " [" + var2D->Xunits() + "]";
            std::string ylabel_str = var2D->m_hists2D.m_ylabel + " [" + var2D->Yunits() + "]";
            std::string unit_per_bin;
            
            if ( var2D->Name() == "BlobEcaloVSdx_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/EcaloVSdx/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part2" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/EcaloVSdx/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part2" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/14-BlobdEdxEnd/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            
            // Function and vertical lines inputs
            std::vector<std::string> functions;
            std::vector<double> x1_functions;
            std::vector<double> x2_functions;
            std::vector<double> x_vertlines;
            std::vector<double> y1_vertlines;
            std::vector<double> y2_vertlines;
            
            if ( option_material == "lead" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part2" ) {
                    functions.push_back("360.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    functions.push_back("0.0375*x*x + 2.85*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    x_vertlines.push_back(50.0);
                    y1_vertlines.push_back(237.0);
                    y2_vertlines.push_back(360.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part2" ) {
                    functions.push_back("300 + 2.5*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    x_vertlines.push_back(40.0);
                    y1_vertlines.push_back(200.0);
                    y2_vertlines.push_back(400.5);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part2" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part2" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part2" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.0 + 0.01*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part2" ) {
                    functions.push_back("6.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("5.33 + 0.00667*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part2" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.05882 + 0.00294118*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part2" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(150.0);
                //     functions.push_back("2.5 + 0.03*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(250.0);
                //     functions.push_back("1.875 + 0.003125*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(250.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            else if ( option_material == "iron" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part2" ) {
                    functions.push_back("340.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    functions.push_back("4.54545*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    x_vertlines.push_back(55.0);
                    y1_vertlines.push_back(250.0);
                    y2_vertlines.push_back(340.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part2" ) {
                    functions.push_back("240");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    x_vertlines.push_back(34.0);
                    y1_vertlines.push_back(170.0);
                    y2_vertlines.push_back(240.0);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part2" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part2" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part2" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("2.67 + 0.0133*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part2" ) {
                    functions.push_back("5.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.75 + 0.0125*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(260.0);
                    x_vertlines.push_back(260.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part2" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(350.0);
                //     functions.push_back("2.64706 + 0.00235294*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(350.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part2" ) {
                //     functions.push_back("6.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("-2.0 + 0.04*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.22222 + 0.00278*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            
            // MC and data
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D.hist,
                   output_str + "_MC_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_data_ObjectPdg2D,
                   output_str + "_Data_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            // PDG purity
            PlotMCPurityPerBin_Pdg2D(plot_info,
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_EM.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.univHist(cv_univ),
                                     output_str, title_str + material_title, option_material,
                                     functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                                     xlabel_str, ylabel_str, "",
                                     false, false);
        }
    }
    
    
    
    // ===================
    //  AFTER BLOB ENERGY
    // ===================
    
    if ( option_cut == "AfterBlobEnergy" )
    {
        // Load variables
        std::vector<Variable*> variables     = GetAfterBlobEnergyVariables();
        std::vector<Variable2D*> variables2D = GetAfterBlobEnergyVariables2D();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        LoadVariables2D(util, mc_fin, kMC, variables2D, option_cut);
        LoadVariables2D(util, data_fin, kData, variables2D, option_cut);
        
        
        // 1D variables
        // ============
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string unit_per_bin;
            
            if ( var->Name() == "MuonPt_AfterBlobEnergy" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} after blob energy cuts";
            }
            else if ( var->Name() == "Nblobs_AfterBlobEnergy" ) {
                title_str  = "N_{showers} after blob energy cuts";
            }
            else if ( var->Name() == "BlobEcalo_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobEcalo_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/Ecalo/";
                unit_per_bin = "20 MeV";
            }
            else if ( var->Name() == "Blobdx_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dx/";
                unit_per_bin = "2 cm";
            }
            else if ( var->Name() == "BlobdEdxMean_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMean/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxFront_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFront/";
                unit_per_bin = "0.25 MeV/cm";
            }
            else if ( var->Name() == "BlobdEdxEnd_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEnd/";
                unit_per_bin = "0.25 MeV/cm";
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_AfterBlobEnergy" ) {
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
                                  output_topdir + "/14-5-AfterEnergyCuts/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_AfterBlobEnergy" ) {
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
                                  output_topdir + "/14-5-AfterEnergyCuts/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Shower energy and similar variables
            else {
                PlotDataStackedMC(plot_info,
                                  var->m_hists.m_data_ObjectPdg, var->m_hists.m_mc_ObjectPdg.hist,
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                  (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                  output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var->Name() + "_Pdg_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers / " + unit_per_bin, "TR");
                
                PlotMCPurityPerBin_Pdg(plot_info,
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Neutron.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Muon.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_OthPdg.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_MCXtalk.univHist(cv_univ),
                                       (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Overlay.univHist(cv_univ),
                                       output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var->Name() + "_PdgRatio_" + option_material,
                                       title_str + material_title,
                                       arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                       var->m_hists.m_xlabel + " [" + var->Units() + "]", "PDG purity / bin", "TR");
                
                PlotMCUnitNorm_PdgV2(plot_info,
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pi0.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Proton.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_Pion.univHist(cv_univ),
                                     (MnvH1D*)var->m_hists.m_mc_ObjectPdg_EM.univHist(cv_univ),
                                     output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var->Name() + "_PdgUnitNorm_" + option_material,
                                     title_str + material_title,
                                     arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                     var->m_hists.m_xlabel + " [" + var->Units() + "]", "Arbitrary units", "TR");
            }
        }
        
        
        // 2D variables
        // ============
        for ( auto var2D : variables2D )
        {
            // Construct plot info object
            PlotInfo plot_info(var2D, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            std::string output_str;
            std::string xlabel_str = var2D->m_hists2D.m_xlabel + " [" + var2D->Xunits() + "]";
            std::string ylabel_str = var2D->m_hists2D.m_ylabel + " [" + var2D->Yunits() + "]";
            std::string unit_per_bin;
            
            if ( var2D->Name() == "BlobEcaloVSdx_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/EcaloVSdx/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part3" ) {
                title_str    = "Single-shower events";
                shower_dir   = "SingleBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/EcaloVSdx/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "20 MeV #times 2 cm";
            }
            else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxMeanVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxFrontVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part3" ) {
                title_str    = "Multi-shower events";
                shower_dir   = "MultiBlob/dEdxEndVSEcalo/";
                output_str   = output_topdir + "/14-5-AfterEnergyCuts/" + shower_dir + var2D->Name();
                unit_per_bin = "0.25 MeV/cm #times 20 MeV";
            }
            
            // Function and vertical lines inputs
            std::vector<std::string> functions;
            std::vector<double> x1_functions;
            std::vector<double> x2_functions;
            std::vector<double> x_vertlines;
            std::vector<double> y1_vertlines;
            std::vector<double> y2_vertlines;
            
            if ( option_material == "lead" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part3" ) {
                    functions.push_back("360.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    functions.push_back("0.0375*x*x + 2.85*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(50.0);
                    x_vertlines.push_back(50.0);
                    y1_vertlines.push_back(237.0);
                    y2_vertlines.push_back(360.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part3" ) {
                    functions.push_back("300 + 2.5*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(40.0);
                    x_vertlines.push_back(40.0);
                    y1_vertlines.push_back(200.0);
                    y2_vertlines.push_back(400.5);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part3" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part3" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part3" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.0 + 0.01*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part3" ) {
                    functions.push_back("6.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("5.33 + 0.00667*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part3" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.05882 + 0.00294118*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part3" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(150.0);
                //     functions.push_back("2.5 + 0.03*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(250.0);
                //     functions.push_back("1.875 + 0.003125*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(250.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            else if ( option_material == "iron" ) {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part3" ) {
                    functions.push_back("340.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    functions.push_back("4.54545*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(55.0);
                    x_vertlines.push_back(55.0);
                    y1_vertlines.push_back(250.0);
                    y2_vertlines.push_back(340.0);
                }
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part3" ) {
                    functions.push_back("240");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    functions.push_back("5.0*x");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(34.0);
                    x_vertlines.push_back(34.0);
                    y1_vertlines.push_back(170.0);
                    y2_vertlines.push_back(240.0);
                }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part3" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(80.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(7.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(80.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part3" ) {
                //     functions.push_back("7.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(100.0);
                //     functions.push_back("4 + 0.03*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("3.0");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(200.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                // }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part3" ) {
                    functions.push_back("4.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("2.67 + 0.0133*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(400.0);
                    x_vertlines.push_back(400.0);
                    y1_vertlines.push_back(8.0);
                    y2_vertlines.push_back(22.0);
                }
                else if ( var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part3" ) {
                    functions.push_back("5.0");
                    x1_functions.push_back(0.0);
                    x2_functions.push_back(100.0);
                    functions.push_back("3.75 + 0.0125*x");
                    x1_functions.push_back(100.0);
                    x2_functions.push_back(260.0);
                    x_vertlines.push_back(260.0);
                    y1_vertlines.push_back(7.0);
                    y2_vertlines.push_back(22.0);
                }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part3" ) {
                //     functions.push_back("5.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(350.0);
                //     functions.push_back("2.64706 + 0.00235294*x");
                //     x1_functions.push_back(150.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(150.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(3.0);
                //     x_vertlines.push_back(350.0);
                //     y1_vertlines.push_back(5.0);
                //     y2_vertlines.push_back(22.0);
                // }
                // else if ( var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part3" ) {
                //     functions.push_back("6.0");
                //     x1_functions.push_back(0.0);
                //     x2_functions.push_back(200.0);
                //     functions.push_back("-2.0 + 0.04*x");
                //     x1_functions.push_back(200.0);
                //     x2_functions.push_back(300.0);
                //     functions.push_back("2.22222 + 0.00278*x");
                //     x1_functions.push_back(100.0);
                //     x2_functions.push_back(1500.0);
                //     x_vertlines.push_back(300.0);
                //     y1_vertlines.push_back(10.0);
                //     y2_vertlines.push_back(22.0);
                //     x_vertlines.push_back(100.0);
                //     y1_vertlines.push_back(0.0);
                //     y2_vertlines.push_back(2.5);
                // }
            }
            
            // MC and data
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D.hist,
                   output_str + "_MC_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            Plot2D(plot_info,
                   (MnvH2D*)var2D->m_hists2D.m_data_ObjectPdg2D,
                   output_str + "_Data_" + option_material, title_str + material_title,
                   functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                   xlabel_str, ylabel_str, "Showers / " + unit_per_bin,
                   true, true);
            
            // PDG purity
            PlotMCPurityPerBin_Pdg2D(plot_info,
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_EM.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.univHist(cv_univ),
                                     (MnvH2D*)var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.univHist(cv_univ),
                                     output_str, title_str + material_title, option_material,
                                     functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
                                     xlabel_str, ylabel_str, "",
                                     false, false);
        }
    }
    
    
    
    // =================
    //  BLOB MICHEL CUT
    // =================
    
    if ( option_cut == "BlobMichel" )
    {
        // Load variables
        std::vector<Variable*> variables = GetBlobMichelVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            if ( var->Name() == "BlobEndPointMichel_Single" || var->Name() == "BlobEndPointMichel_Multi" ) {
                arrow_X.push_back(1.0);
                arrow_Ymax.push_back(0.8);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("L");
            }
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            
            if ( var->Name() == "MuonPt_BeforeBlobMichel" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before showers w/Michel cut";
            }
            else if ( var->Name() == "Nblobs_BeforeBlobMichel" ) {
                title_str  = "N_{showers} before showers w/Michel cut";
            }
            else if ( var->Name() == "BlobStartPointMichel_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/StartPoint/";
            }
            else if ( var->Name() == "BlobEndPointMichel_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/EndPoint/";
            }
            else if ( var->Name() == "BlobPdg_Single" ) {
                title_str  = "Shower PDG (single-shower events)";
                shower_dir = "SingleBlob/";
            }
            else if ( var->Name() == "BlobStartPointMichel_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/StartPoint/";
            }
            else if ( var->Name() == "BlobEndPointMichel_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/EndPoint/";
            }
            else if ( var->Name() == "BlobPdg_Multi" ) {
                title_str  = "Shower PDG (multi-shower events)";
                shower_dir = "MultiBlob/";
            }
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeBlobMichel" ) {
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
                                  output_topdir + "/15-BlobMichel/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Number of showers before cut
            else if ( var->Name() == "Nblobs_BeforeBlobMichel" ) {
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
                                  output_topdir + "/15-BlobMichel/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Events", "TR");
            }
            
            // Number of Michels in showers
            else if ( var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobEndPointMichel_Single" ||
                      var->Name() == "BlobStartPointMichel_Multi"  || var->Name() == "BlobEndPointMichel_Multi" ) {
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
                                  output_topdir + "/15-BlobMichel/" + shower_dir + var->Name() + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "Number of showers", "TR");
                
                PlotMCPurityPerBin_Selection(plot_info,
                                             (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                             output_topdir + "/15-BlobMichel/" + shower_dir + var->Name() + "_SelRatio_" + option_material,
                                             title_str + material_title,
                                             arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                             var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
            }
            
            // Michel PDG in showers
            else {
                PlotStackedMC(plot_info,
                              var->m_hists.m_mc_Selection.hist,
                              (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasUp.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasBetw.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPlasDown.univHist(cv_univ),
                              (MnvH1D*)var->m_hists.m_mc_Selection_BackgrOther.univHist(cv_univ),
                              output_topdir + "/15-BlobMichel/" + shower_dir + var->Name() + "_" + option_material,
                              title_str + material_title,
                              arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                              "", "Number of showers", "TR");
            }
        }
    }
    
    
    
    // =====================
    //  HADRONIC ENERGY CUT
    // =====================
    
    if ( option_cut == "Recoil" )
    {
        // Load variables
        std::vector<Variable*> variables = GetEnergyVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            if ( var->Name() == "NoPi0RecoilE_Single" ) {
                arrow_X.push_back(0.8);
                arrow_Ymax.push_back(0.7);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("L");
            }
            else if ( var->Name() == "NoPi0RecoilE_Multi" ) {
                arrow_X.push_back(0.5);
                arrow_Ymax.push_back(0.8);
                arrow_length.push_back(0.15);
                arrow_dir.push_back("L");
            }
            
            // Plot title and shower directory
            std::string title_str;
            std::string shower_dir;
            
            if ( var->Name() == "MuonPt_BeforeEnergy" ) {
                title_str  = "Muon #font[12]{p}_{#font[132]{T}} before hadronic energy cut";
            }
            else if ( var->Name() == "NoPi0RecoilE_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/NoPi0RecoilE/";
            }
            else if ( var->Name() == "RecoilE_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/RecoilE/";
            }
            else if ( var->Name() == "Q2_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/Q2/";
            }
            else if ( var->Name() == "W2_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/W2/";
            }
            else if ( var->Name() == "W_Single" ) {
                title_str  = "Single-shower events";
                shower_dir = "SingleBlob/W/";
            }
            else if ( var->Name() == "NoPi0RecoilE_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/NoPi0RecoilE/";
            }
            else if ( var->Name() == "RecoilE_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/RecoilE/";
            }
            else if ( var->Name() == "Q2_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/Q2/";
            }
            else if ( var->Name() == "W2_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/W2/";
            }
            else if ( var->Name() == "W_Multi" ) {
                title_str  = "Multi-shower events";
                shower_dir = "MultiBlob/W/";
            }
            
            
            // Muon Pt before cut
            if ( var->Name() == "MuonPt_BeforeEnergy" ) {
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
                                  output_topdir + "/16-HadronicEnergy/" + var->Name() + "_" + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
            }
            
            // Hadronic energy and related variables
            else {
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
                                  output_topdir + "/16-HadronicEnergy/" + shower_dir + var->Name() + option_material,
                                  title_str + material_title,
                                  arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                  "", "", "TR");
                
                PlotMCPurityPerBin_Selection(plot_info,
                                             (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                             (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                             output_topdir + "/16-HadronicEnergy/" + shower_dir + var->Name() + "_SelRatio_" + option_material,
                                             title_str + material_title,
                                             arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                             var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
            }
        }
    }
    
    
    
    // ==========
    //  ALL CUTS
    // ==========
    
    if ( option_cut == "AllCuts" )
    {
        // Load variables
        std::vector<Variable*> variables = GetXsecVariables();
        
        LoadVariables(util, mc_fin, kMC, variables, option_cut);
        LoadVariables(util, data_fin, kData, variables, option_cut);
        
        
        // Loop over variables
        for ( auto var : variables )
        {
            // Construct plot info object
            PlotInfo plot_info(var, mc_pot, data_pot,
                               do_frac_uncertainty, do_cov_area_norm, include_stat_error, do_bin_width_norm, print_format);
            
            // Arrow cuts
            std::vector<double> arrow_X;
            std::vector<double> arrow_Ymax;
            std::vector<double> arrow_length;
            std::vector<std::string> arrow_dir;
            
            // Plot title
            std::string title_str  = "Muon #font[12]{p}_{#font[132]{T}} after all cuts";
            
            // Muon Pt
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
                              output_topdir + "/AllCuts/" + var->Name() + "_" + option_material,
                              title_str + material_title,
                              arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                              "", "", "TR");
            
            PlotMCPurityPerBin_Selection(plot_info,
                                         (MnvH1D*)var->m_hists.m_mc_Selection_Signal.univHist(cv_univ),
                                         (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPi0HighW.univHist(cv_univ),
                                         (MnvH1D*)var->m_hists.m_mc_Selection_BackgrQElike.univHist(cv_univ),
                                         (MnvH1D*)var->m_hists.m_mc_Selection_BackgrPionProd.univHist(cv_univ),
                                         output_topdir + "/AllCuts/" + var->Name() + "_SelRatio_" + option_material,
                                         title_str + material_title,
                                         arrow_X, arrow_Ymax, arrow_length, arrow_dir,
                                         var->m_hists.m_xlabel, "MC physics purity / bin", "TR");
        }
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void PlotCutStudies(std::string option_date,
                    std::string option_cut,
                    std::string option_material,
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
    std::string mc_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/mc/%s/%s", option_date_mc.c_str(),
                                                                                                       option_material.c_str());
    
    // Data
    std::string data_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/data/%s/%s", option_date_data.c_str(),
                                                                                                           option_material.c_str());
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin(Form("%s/MC_CutStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_%s.root", mc_fin_topdir.c_str(),
                                                                                       option_model.c_str(),
                                                                                       option_systematics.c_str(),
                                                                                       option_material.c_str()), "READ");
    
    // Data
    TFile data_fin(Form("%s/Data_CutStudies_AllPlaylists_%s.root", data_fin_topdir.c_str(),
                                                                   option_material.c_str()), "READ");
    
    
    
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
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, false, do_systematics, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    SetMacroUtilPOT(mc_fin, data_fin, util);
    
    std::cout << std::endl;
    std::cout << " Plotting cut studies... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << mc_pot   << std::endl;
    std::cout << " \tData POT: " << data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Plot
    PlotVariable(util, mc_fin, data_fin, option_date_mc, option_cut, option_material);
    
    
    // Close ROOT files
    mc_fin.Close();
    data_fin.Close();
}


#endif  // PlotCutStudies_C