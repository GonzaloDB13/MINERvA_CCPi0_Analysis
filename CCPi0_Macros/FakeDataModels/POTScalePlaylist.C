#ifndef POTScalePlaylist_C
#define POTScalePlaylist_C

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





// ========================================================================================================================
//  SCALE MC FOR EVENT SELECTION DISTRIBUTIONS
// ========================================================================================================================

void POTScale(CCPi0::MacroUtil util,
              TFile& fd_fin,
              TFile& data_fin,
              TFile& fd_fout)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(true);  // Include true variables
    
    
    // Load MC and data histograms from input files
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> LoadMCHists_Selection(fd_fin, util.m_error_bands);
            var -> LoadDataHists_Selection(data_fin);
            var -> LoadMigrationHists(fd_fin, util.m_error_bands);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> LoadEffNumerator(fd_fin, util.m_error_bands);
            if ( util.m_do_truth ) var -> LoadEffDenominator(fd_fin, util.m_error_bands_truth);
        }
    }
    
    
    // Get POT from input files
    double fd_pot   = GetPOT(fd_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms and write them to MC output file
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> ScaleMCHists_Selection(fd_pot, data_pot);
            var -> SyncMCHists_Selection();
            var -> WriteMCHists_Selection(fd_fout);
            
            var -> ScaleMigrationHists(fd_pot, data_pot);
            var -> SyncMigrationHists();
            var -> WriteMigrationHists(fd_fout);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> ScaleEffNumerator(fd_pot, data_pot);
            var -> SyncEffNumerator();
            var -> WriteEffNumerator(fd_fout);
            
            if ( util.m_do_truth ) {
                var -> ScaleEffDenominator(fd_pot, data_pot);
                var -> SyncEffDenominator();
                var -> WriteEffDenominator(fd_fout);
            }
        }
    }
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void POTScalePlaylist(std::string plist_string,
                      std::string option_date,
                      std::string option_model = "v1",
                      bool do_systematics      = true)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file name
    const std::string option_systematics = do_systematics ? "WithSyst" : "NoSyst";
    const std::string option_date_mc     = option_date + "_" + option_model;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // Fake data
    std::string fd_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/FakeDataModels/%s/lead", option_date_mc.c_str());
    std::string fd_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/FakeDataModels/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/lead", option_date.c_str());
    std::string data_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/iron", option_date.c_str());
    
    
    
    // Input files
    // ===========
    
    // Fake data MC
    TFile fd_fin_lead(Form("%s/MC_FakeData_MnvGENIE%s_%s_%s_lead.root", fd_fin_lead_topdir.c_str(),
                                                                        option_model.c_str(),
                                                                        option_systematics.c_str(),
                                                                        plist_string.c_str()), "READ");
    
    TFile fd_fin_iron(Form("%s/MC_FakeData_MnvGENIE%s_%s_%s_iron.root", fd_fin_iron_topdir.c_str(),
                                                                        option_model.c_str(),
                                                                        option_systematics.c_str(),
                                                                        plist_string.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_EventSelection_%s_lead.root", data_fin_lead_topdir.c_str(),
                                                                    plist_string.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_EventSelection_%s_iron.root", data_fin_iron_topdir.c_str(),
                                                                    plist_string.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Fake data MC
    TFile fd_fout_lead(Form("%s/MC_FakeData_MnvGENIE%s_%s_POTScaled_%s_lead.root", fd_fin_lead_topdir.c_str(),
                                                                                   option_model.c_str(),
                                                                                   option_systematics.c_str(),
                                                                                   plist_string.c_str()), "RECREATE");
    
    TFile fd_fout_iron(Form("%s/MC_FakeData_MnvGENIE%s_%s_POTScaled_%s_iron.root", fd_fin_iron_topdir.c_str(),
                                                                                   option_model.c_str(),
                                                                                   option_systematics.c_str(),
                                                                                   plist_string.c_str()), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  MacroUtil
    // =========================================
    
    // MC and data file lists, MC is used only to access its systematics
    // (Using full sample for this, just in case)
    const std::string mc_file_list   = GetPlaylistFile(true,  plist_string, "test");
    const std::string data_file_list = GetPlaylistFile(false, plist_string, "test");
    
    
    // Set MacroUtil
    // (TRUTH option set as 'true')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, do_systematics, type_model);
    
    
    // Set MacroUtil POT
    double fd_pot_lead   = GetPOT(fd_fin_lead,   true);
    double fd_pot_iron   = GetPOT(fd_fin_iron,   true);
    double data_pot_lead = GetPOT(data_fin_lead, false);
    double data_pot_iron = GetPOT(data_fin_iron, false);
    
    SetMacroUtilPOT(fd_fin_lead, data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " PLAYLIST: " << plist_string << std::endl;
    std::cout << std::endl;
    std::cout << " Lead:" << std::endl;
    std::cout << " \tFake data POT: " << fd_pot_lead   << std::endl;
    std::cout << " \tData POT:      " << data_pot_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron:" << std::endl;
    std::cout << " \tFake data POT: " << fd_pot_iron   << std::endl;
    std::cout << " \tData POT:      " << data_pot_iron << std::endl;
    std::cout << std::endl;
    
    
    // Write POT to output files (MC-only outputs)
    WritePOT(fd_fout_lead, true, fd_pot_lead);
    WritePOT(fd_fout_iron, true, fd_pot_iron);
    
    
    // Output POT text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/FakeDataModels/%s/POTinfo", option_date_mc.c_str());
    
    std::ofstream text_pot(Form("%s/POT_%s.txt", text_topdir.c_str(),
                                                 plist_string.c_str()));
    
    text_pot << std::endl;
    text_pot << " PLAYLIST: " << plist_string << std::endl;
    text_pot << std::endl;
    text_pot << " Lead:" << std::endl;
    text_pot << " \tFake data POT: " << fd_pot_lead   << std::endl;
    text_pot << " \tData POT:      " << data_pot_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron:" << std::endl;
    text_pot << " \tFake data POT: " << fd_pot_iron   << std::endl;
    text_pot << " \tData POT:      " << data_pot_iron << std::endl;
    text_pot << std::endl;
    
    text_pot.close();
    
    
    
    // =========================================
    //  Scale MC files
    // =========================================
    
    // Fake data
    std::cout << " POT-scaling fake data files on LEAD... " << std::endl;
    std::cout << std::endl;
    POTScale(util, fd_fin_lead, data_fin_lead, fd_fout_lead);
    
    std::cout << " POT-scaling fake data files on IRON... " << std::endl;
    std::cout << std::endl;
    POTScale(util, fd_fin_iron, data_fin_iron, fd_fout_iron);
    
    
    // Close ROOT files
    fd_fin_lead.Close();
    fd_fin_iron.Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
    
    fd_fout_lead.Close();
    fd_fout_iron.Close();
    
    
    std::cout << " Success with playlist: " << plist_string << "!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // POTScalePlaylist_C