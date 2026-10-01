#ifndef MakeTransWarpInputs_C
#define MakeTransWarpInputs_C

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
#include "TRandom.h"





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void MakeTransWarpInputs(std::string option_date,
                         std::string option_model_baseline,
                         std::string option_model_fakedata,
                         std::string option_material)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model_baseline, type_model);
    
    
    // Options for input/output file
    const std::string option_date_mc = option_date + "_" + option_model_baseline;
    const std::string option_date_fd = option_date + "_" + option_model_fakedata;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // Baseline MC
    std::string mc_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/%s", option_date_mc.c_str(),
                                                                                                           option_material.c_str());
    
    // Fake data
    std::string fd_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/FakeDataModels/%s/%s", option_date_fd.c_str(),
                                                                                                        option_material.c_str());
    
    
    // Input files
    // ===========
    
    // Baseline MC
    TFile mc_fin(Form("%s/MC_EventSelection_MnvGENIE%s_WithSyst_POTScaled_AllPlaylists_%s.root", mc_fin_topdir.c_str(),
                                                                                                 option_model_baseline.c_str(),
                                                                                                 option_material.c_str()), "READ");
    
    // Fake data
    TFile fd_fin(Form("%s/MC_FakeData_MnvGENIE%s_WithSyst_POTScaled_AllPlaylists_%s.root", fd_fin_topdir.c_str(),
                                                                                           option_model_fakedata.c_str(),
                                                                                           option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Top directory
    std::string fout_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/WarpingStudies/%s/%s/TransWarpInputs", option_date_mc.c_str(),
                                                                                                                      option_material.c_str());
    
    // Create output file
    TFile fout(Form("%s/BaselineMC_%s_FakeData_%s_%s.root", fout_topdir.c_str(),
                                                            option_model_baseline.c_str(),
                                                            option_model_fakedata.c_str(),
                                                            option_material.c_str()), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
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
    
    
    // Set MacroUtil POT using MC input file
    SetMacroUtilPOT(mc_fin, util, true);
    
    
    // Get POT from MC and data files, and write them
    double mc_pot = GetPOT(mc_fin, true);
    double fd_pot = GetPOT(fd_fin, true);  // Use fake data POT as "data POT"
    
    std::cout << std::endl;
    std::cout << " \tMC POT:        " << mc_pot << std::endl;
    std::cout << " \tFake data POT: " << fd_pot << std::endl;
    std::cout << std::endl;
    std::cout << " Preparing TransWarpExtract input file... " << std:: endl;
    std::cout << std::endl;
    
    WritePOT(fout, mc_pot, fd_pot);
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    
    // =========================================
    //  Make histograms
    // =========================================
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Get variable name
        std::string var_name = var->Name();
        
        
        // Write baseline MC histograms
        // ============================
        
        // Reco signal
        PlotUtils::MnvH1D* h_reco_signal = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_Selection_%s_Signal", var_name.c_str()));
        h_reco_signal -> SetName(Form("h_reco_%s", var_name.c_str()));
        
        // Efficiency numerator
        PlotUtils::MnvH1D* h_reco_effnum = (PlotUtils::MnvH1D*)mc_fin.Get(Form("mc_EffNumerator_%s_True", var_name.c_str()));
        h_reco_effnum -> SetName(Form("h_reco_truth_%s", var_name.c_str()));
        
        // Migration matrix
        PlotUtils::MnvH2D* h_migration = (PlotUtils::MnvH2D*)mc_fin.Get(Form("mc_Migration_%s", var_name.c_str()));
        h_migration -> SetName(Form("h_migration_%s", var_name.c_str()));
        
        // Write
        fout.cd();
        h_reco_signal -> Write();
        h_reco_effnum -> Write();
        h_migration   -> Write();
        
        
        // Write fake data histograms
        // ==========================
        
        // Reco signal
        PlotUtils::MnvH1D* h_data_signal = (PlotUtils::MnvH1D*)fd_fin.Get(Form("mc_Selection_%s_Signal", var_name.c_str()));
        h_data_signal -> SetName(Form("h_data_%s", var_name.c_str()));
        
        // Efficiency numerator
        PlotUtils::MnvH1D* h_data_effnum = (PlotUtils::MnvH1D*)fd_fin.Get(Form("mc_EffNumerator_%s_True", var_name.c_str()));
        h_data_effnum -> SetName(Form("h_data_truth_%s", var_name.c_str()));
        
        // Write
        fout.cd();
        h_data_signal -> Write();
        h_data_effnum -> Write();
        
        
        // // Write histograms with fake signal enhanced 2 times
        // // ==================================================
        
        // // Scale signal and eff. numerator
        // h_data_signal -> Scale(2.0);
        // h_data_effnum -> Scale(2.0);
        
        // // Write histograms
        // fout.cd();
        // h_data_signal -> Write(Form("h_data_%s_2xSignal",       var_name.c_str()));
        // h_data_effnum -> Write(Form("h_data_truth_%s_2xSignal", var_name.c_str()));
    }
    
    
    // Close files
    mc_fin.Close();
    fd_fin.Close();
    fout.Close();
}


#endif  // MakeTransWarpInputs_C