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

void POTScale_EvSel(CCPi0::MacroUtil util,
                    TFile& mc_fin,
                    TFile& data_fin,
                    TFile& mc_fout)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(true);  // Include true variables
    
    
    // Load MC and data histograms from input files
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
            if ( util.m_do_truth ) var -> LoadEffDenominator(mc_fin, util.m_error_bands_truth);
        }
    }
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms and write them to MC output file
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
            var -> SyncMCHists_Selection();
            var -> WriteMCHists_Selection(mc_fout);
            
            var -> ScaleMigrationHists(mc_pot, data_pot);
            var -> SyncMigrationHists();
            var -> WriteMigrationHists(mc_fout);
        }
        
        // For true-only variables (don't scale them!!)
        else if ( var->m_is_true ) {
            var -> ScaleEffNumerator(mc_pot, data_pot);
            var -> SyncEffNumerator();
            var -> WriteEffNumerator(mc_fout);
            
            if ( util.m_do_truth ) {
                var -> ScaleEffDenominator(mc_pot, data_pot);
                var -> SyncEffDenominator();
                var -> WriteEffDenominator(mc_fout);
            }
        }
    }
}





// ========================================================================================================================
//  SCALE MC FOR PRE-PLASTIC TUNING DISTRIBUTIONS
// ========================================================================================================================

void POTScale_PlasTun(CCPi0::MacroUtil util,
                      TFile& mc_fin,
                      TFile& data_fin,
                      TFile& mc_fout)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Load MC and data histograms from input files
    for ( auto var : variables ) {
        var -> LoadMCHists_PlasSB_In_SigReg(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_SigReg(data_fin);
        
        var -> LoadMCHists_PlasSB_In_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PlasSB_In_PhysSB(data_fin);
    }
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms and write them to MC output file
    for ( auto var : variables ) {
        var -> ScaleMCHists_PlasSB_In_SigReg(mc_pot, data_pot);
        var -> SyncMCHists_PlasSB_In_SigReg();
        var -> WriteMCHists_PlasSB_In_SigReg(mc_fout);
        
        var -> ScaleMCHists_PlasSB_In_PhysSB(mc_pot, data_pot);
        var -> SyncMCHists_PlasSB_In_PhysSB();
        var -> WriteMCHists_PlasSB_In_PhysSB(mc_fout);
    }
}





// ========================================================================================================================
//  SCALE MC FOR PRE-PHYSICS TUNING DISTRIBUTIONS
// ========================================================================================================================

void POTScale_PhysTun(CCPi0::MacroUtil util,
                      TFile& mc_fin,
                      TFile& data_fin,
                      TFile& mc_fout)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Load MC and data histograms from input files
    for ( auto var : variables ) {
        var -> LoadMCHists_PhysSB(mc_fin, util.m_error_bands);
        var -> LoadDataHists_PhysSB(data_fin);
    }
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms and write them to MC output file
    for ( auto var : variables ) {
        var -> ScaleMCHists_PhysSB(mc_pot, data_pot);
        var -> SyncMCHists_PhysSB();
        var -> WriteMCHists_PhysSB(mc_fout);
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
    
    // Monte Carlo
    std::string mc_fin_evsel_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/lead", option_date_mc.c_str());
    std::string mc_fin_evsel_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/iron", option_date_mc.c_str());
    
    std::string mc_fin_plastun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/lead/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_plastun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/mc/%s/iron/BeforeTuning", option_date_mc.c_str());
    
    std::string mc_fin_phystun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/lead/BeforeTuning", option_date_mc.c_str());
    std::string mc_fin_phystun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/iron/BeforeTuning", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_evsel_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/lead", option_date.c_str());
    std::string data_fin_evsel_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/iron", option_date.c_str());
    
    std::string data_fin_plastun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/data/%s/lead", option_date.c_str());
    std::string data_fin_plastun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PlasticTuning/data/%s/iron", option_date.c_str());
    
    std::string data_fin_phystun_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/data/%s/lead", option_date.c_str());
    std::string data_fin_phystun_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/data/%s/iron", option_date.c_str());
    
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_evsel_lead(Form("%s/MC_EventSelection_MnvGENIE%s_%s_%s_lead.root", mc_fin_evsel_lead_topdir.c_str(),
                                                                                    option_model.c_str(),
                                                                                    option_systematics.c_str(),
                                                                                    plist_string.c_str()), "READ");
    
    TFile mc_fin_evsel_iron(Form("%s/MC_EventSelection_MnvGENIE%s_%s_%s_iron.root", mc_fin_evsel_iron_topdir.c_str(),
                                                                                    option_model.c_str(),
                                                                                    option_systematics.c_str(),
                                                                                    plist_string.c_str()), "READ");
    
    TFile mc_fin_plastun_lead(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_%s_lead.root", mc_fin_plastun_lead_topdir.c_str(),
                                                                                           option_model.c_str(),
                                                                                           option_systematics.c_str(),
                                                                                           plist_string.c_str()), "READ");
    
    TFile mc_fin_plastun_iron(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_%s_iron.root", mc_fin_plastun_iron_topdir.c_str(),
                                                                                           option_model.c_str(),
                                                                                           option_systematics.c_str(),
                                                                                           plist_string.c_str()), "READ");
    
    TFile mc_fin_phystun_lead(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_%s_lead.root", mc_fin_phystun_lead_topdir.c_str(),
                                                                                           option_model.c_str(),
                                                                                           option_systematics.c_str(),
                                                                                           plist_string.c_str()), "READ");
    
    TFile mc_fin_phystun_iron(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_%s_iron.root", mc_fin_phystun_iron_topdir.c_str(),
                                                                                           option_model.c_str(),
                                                                                           option_systematics.c_str(),
                                                                                           plist_string.c_str()), "READ");
    
    
    // Data
    TFile data_fin_evsel_lead(Form("%s/Data_EventSelection_%s_lead.root", data_fin_evsel_lead_topdir.c_str(),
                                                                          plist_string.c_str()), "READ");
    
    TFile data_fin_evsel_iron(Form("%s/Data_EventSelection_%s_iron.root", data_fin_evsel_iron_topdir.c_str(),
                                                                          plist_string.c_str()), "READ");
    
    TFile data_fin_plastun_lead(Form("%s/Data_PlasticTuning_%s_lead.root", data_fin_plastun_lead_topdir.c_str(),
                                                                           plist_string.c_str()), "READ");
    
    TFile data_fin_plastun_iron(Form("%s/Data_PlasticTuning_%s_iron.root", data_fin_plastun_iron_topdir.c_str(),
                                                                           plist_string.c_str()), "READ");
    
    TFile data_fin_phystun_lead(Form("%s/Data_PhysicsTuning_%s_lead.root", data_fin_phystun_lead_topdir.c_str(),
                                                                           plist_string.c_str()), "READ");
    
    TFile data_fin_phystun_iron(Form("%s/Data_PhysicsTuning_%s_iron.root", data_fin_phystun_iron_topdir.c_str(),
                                                                           plist_string.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Monte Carlo
    TFile mc_fout_evsel_lead(Form("%s/MC_EventSelection_MnvGENIE%s_%s_POTScaled_%s_lead.root", mc_fin_evsel_lead_topdir.c_str(),
                                                                                               option_model.c_str(),
                                                                                               option_systematics.c_str(),
                                                                                               plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_evsel_iron(Form("%s/MC_EventSelection_MnvGENIE%s_%s_POTScaled_%s_iron.root", mc_fin_evsel_iron_topdir.c_str(),
                                                                                               option_model.c_str(),
                                                                                               option_systematics.c_str(),
                                                                                               plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_plastun_lead(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_POTScaled_%s_lead.root", mc_fin_plastun_lead_topdir.c_str(),
                                                                                                      option_model.c_str(),
                                                                                                      option_systematics.c_str(),
                                                                                                      plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_plastun_iron(Form("%s/MC_BeforePlasticTuning_MnvGENIE%s_%s_POTScaled_%s_iron.root", mc_fin_plastun_iron_topdir.c_str(),
                                                                                                      option_model.c_str(),
                                                                                                      option_systematics.c_str(),
                                                                                                      plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_phystun_lead(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_POTScaled_%s_lead.root", mc_fin_phystun_lead_topdir.c_str(),
                                                                                                      option_model.c_str(),
                                                                                                      option_systematics.c_str(),
                                                                                                      plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_phystun_iron(Form("%s/MC_BeforePhysicsTuning_MnvGENIE%s_%s_POTScaled_%s_iron.root", mc_fin_phystun_iron_topdir.c_str(),
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
    double mc_pot_evsel_lead   = GetPOT(mc_fin_evsel_lead,   true);
    double mc_pot_evsel_iron   = GetPOT(mc_fin_evsel_iron,   true);
    double data_pot_evsel_lead = GetPOT(data_fin_evsel_lead, false);
    double data_pot_evsel_iron = GetPOT(data_fin_evsel_iron, false);
    
    double mc_pot_plastun_lead   = GetPOT(mc_fin_plastun_lead,   true);
    double mc_pot_plastun_iron   = GetPOT(mc_fin_plastun_iron,   true);
    double data_pot_plastun_lead = GetPOT(data_fin_plastun_lead, false);
    double data_pot_plastun_iron = GetPOT(data_fin_plastun_iron, false);
    
    double mc_pot_phystun_lead   = GetPOT(mc_fin_phystun_lead,   true);
    double mc_pot_phystun_iron   = GetPOT(mc_fin_phystun_iron,   true);
    double data_pot_phystun_lead = GetPOT(data_fin_phystun_lead, false);
    double data_pot_phystun_iron = GetPOT(data_fin_phystun_iron, false);
    
    SetMacroUtilPOT(mc_fin_evsel_lead, data_fin_evsel_lead, util);
    
    std::cout << std::endl;
    std::cout << " PLAYLIST: " << plist_string << std::endl;
    std::cout << std::endl;
    std::cout << " Event selection: " << std::endl;
    std::cout << " ---------------  " << std::endl;
    std::cout << " Lead:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_evsel_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_evsel_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_evsel_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_evsel_iron << std::endl;
    std::cout << std::endl;
    std::cout << " Pre-plastic tuning: " << std::endl;
    std::cout << " ------------------  " << std::endl;
    std::cout << " Lead:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_plastun_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_plastun_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_plastun_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_plastun_iron << std::endl;
    std::cout << std::endl;
    std::cout << " Pre-physics tuning: " << std::endl;
    std::cout << " ------------------  " << std::endl;
    std::cout << " Lead:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_phystun_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_phystun_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_phystun_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_phystun_iron << std::endl;
    std::cout << std::endl;
    
    
    // Write POT to output files (MC-only outputs)
    WritePOT(mc_fout_evsel_lead, true, mc_pot_evsel_lead);
    WritePOT(mc_fout_evsel_iron, true, mc_pot_evsel_iron);
    
    WritePOT(mc_fout_plastun_lead, true, mc_pot_plastun_lead);
    WritePOT(mc_fout_plastun_iron, true, mc_pot_plastun_iron);
    
    WritePOT(mc_fout_phystun_lead, true, mc_pot_phystun_lead);
    WritePOT(mc_fout_phystun_iron, true, mc_pot_phystun_iron);
    
    
    // Output POT text file
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/POTinfo/%s", option_date_mc.c_str());
    
    std::ofstream text_pot(Form("%s/POT_%s.txt", text_topdir.c_str(),
                                                 plist_string.c_str()));
    
    text_pot << std::endl;
    text_pot << " PLAYLIST: " << plist_string << std::endl;
    text_pot << std::endl;
    text_pot << " Event selection: " << std::endl;
    text_pot << " ---------------  " << std::endl;
    text_pot << " Lead:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_evsel_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_evsel_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_evsel_iron   << std::endl;
    text_pot << " \tData POT: " << data_pot_evsel_iron << std::endl;
    text_pot << std::endl;
    text_pot << " Pre-plastic tuning: " << std::endl;
    text_pot << " ------------------  " << std::endl;
    text_pot << " Lead:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_plastun_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_plastun_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_plastun_iron   << std::endl;
    text_pot << " \tData POT: " << data_pot_plastun_iron << std::endl;
    text_pot << std::endl;
    text_pot << " Pre-physics tuning: " << std::endl;
    text_pot << " ------------------  " << std::endl;
    text_pot << " Lead:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_phystun_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_phystun_lead << std::endl;
    text_pot << std::endl;
    text_pot << " Iron:" << std::endl;
    text_pot << " \tMC POT:   " << mc_pot_phystun_lead   << std::endl;
    text_pot << " \tData POT: " << data_pot_phystun_iron << std::endl;
    text_pot << std::endl;
    
    text_pot.close();
    
    
    
    // =========================================
    //  Scale MC files
    // =========================================
    
    // Event selection
    std::cout << " POT-scaling event selection files on LEAD... " << std::endl;
    std::cout << std::endl;
    POTScale_EvSel(util, mc_fin_evsel_lead, data_fin_evsel_lead, mc_fout_evsel_lead);
    
    std::cout << " POT-scaling event selection files on IRON... " << std::endl;
    std::cout << std::endl;
    POTScale_EvSel(util, mc_fin_evsel_iron, data_fin_evsel_iron, mc_fout_evsel_iron);
    
    
    // Pre-plastic tuning
    std::cout << " POT-scaling pre-plastic tuning files on LEAD... " << std::endl;
    std::cout << std::endl;
    POTScale_PlasTun(util, mc_fin_plastun_lead, data_fin_plastun_lead, mc_fout_plastun_lead);
    
    std::cout << " POT-scaling pre-plastic tuning files on IRON... " << std::endl;
    std::cout << std::endl;
    POTScale_PlasTun(util, mc_fin_plastun_iron, data_fin_plastun_iron, mc_fout_plastun_iron);
    
    
    // Pre-physics tuning
    std::cout << " POT-scaling pre-physics tuning files on LEAD... " << std::endl;
    std::cout << std::endl;
    POTScale_PhysTun(util, mc_fin_phystun_lead, data_fin_phystun_lead, mc_fout_phystun_lead);
    
    std::cout << " POT-scaling pre-physics tuning files on IRON... " << std::endl;
    std::cout << std::endl;
    POTScale_PhysTun(util, mc_fin_phystun_iron, data_fin_phystun_iron, mc_fout_phystun_iron);
    
    
    // Close ROOT files
    mc_fin_evsel_lead.Close();
    mc_fin_evsel_iron.Close();
    
    data_fin_evsel_lead.Close();
    data_fin_evsel_iron.Close();
    
    mc_fout_evsel_lead.Close();
    mc_fout_evsel_iron.Close();
    
    mc_fin_plastun_lead.Close();
    mc_fin_plastun_iron.Close();
    mc_fin_phystun_lead.Close();
    mc_fin_phystun_iron.Close();
    
    data_fin_plastun_lead.Close();
    data_fin_plastun_iron.Close();
    data_fin_phystun_lead.Close();
    data_fin_phystun_iron.Close();
    
    mc_fout_plastun_lead.Close();
    mc_fout_plastun_iron.Close();
    mc_fout_phystun_lead.Close();
    mc_fout_phystun_iron.Close();
    
    
    std::cout << " Success with playlist: " << plist_string << "!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // POTScalePlaylist_C