#ifndef FakeDataModels_C
#define FakeDataModels_C

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
#include "TStopwatch.h"





// ========================================================================================================================
//  LOOP OVER ERROR BANDS AND FILL HISTOGRAMS
// ========================================================================================================================

void LoopAndFillHistograms(const CCPi0::MacroUtil& util,
                           const EnumDataMCTruth& type_DataMCTruth,
                           std::vector<Variable*>& variables,
                           const EnumModels& type_model,
                           std::string option_material)
{
#ifndef __CINT__

    // Setup loop
    bool is_mc, is_truth;
    Long64_t n_entries;
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " \tStarting loop to fill fake data model distributions on " << option_material << "... " << std::endl;
    SetupLoop(type_DataMCTruth, type_model, util, is_mc, is_truth, n_entries);
    
    
    // Get error bands
    UniverseMap error_bands;
    
    if ( type_DataMCTruth == kMC || type_DataMCTruth == kTruth ) {
        if ( is_truth ) error_bands = util.m_error_bands_truth;
        else            error_bands = util.m_error_bands;
    }
    
    else if ( type_DataMCTruth == kData ) {
        std::cout << " ERROR: DON'T FILL DATA HISTOGRAMS WITH THIS FUNCTION!!! " << std::endl;
        exit(1);
    }
    
    
    // Define TStopwatch
    TStopwatch sw;
    
    
    // Loop over entries
    // =================
    
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Boolean to check if event is signal or not.
        // First, assume that event is signal and start looping, if found that event is not signal when looping over universes,
        // no need to keep looping at all, just skip event and go to next one
        bool event_is_signal = true;
        
        
        // Boolean to check if vertical-only universe has been checked by reco cuts, by default it's false.
        // Once the first time a vertical-only universe is evaluated, it becomes true and
        // there won't be a need to check again until the next entry
        bool vert_universe_checked_cuts = false;
        
        
        // Booleans to check if vertical-only universe fulfills physics signal region.
        // Similarly, it only needs to be evaluated once for a vertical-only universe
        bool vert_universe_IsSigReg = false;
        
        
        // Loop over error bands
        // =====================
        
        for ( auto error_band : error_bands )
        {
            // Get universes of this error band
            std::vector<CVUniverse*> universes = error_band.second;
            
            
            // Loop over universes
            // ===================
            
            for ( auto universe : universes )
            {
                // Set entry of this universe
                universe -> SetEntry(i_event);
                
                
                // Define CCPi0 event (include cut on fiducial volume)
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                
                
                // Find if event is not CCPi0 signal. If it's not, exit loop over universes
                if ( event.m_signal_backgr_type != kSignal ) {
                    event_is_signal = false;
                    break;
                }
                
                
                // Fill efficiency denominator (only when using Truth tree)
                // ===========================
                
                if ( type_DataMCTruth == kTruth )
                {
                    // Loop over variables and fill histograms
                    for ( auto var : variables )
                    {
                        // For true-only variables
                        if ( var->m_is_true )
                            ccpi0_event::FillEffDenominator(event, var);
                    }
                    
                    // After filling, go to next universe immediately
                    continue;
                }
                
                
                // Check reconstruction cuts
                // =========================
                
                // Check if universe of this error band is vertical-only.
                // If it is vertical-only, cuts would need to be checked only once
                if ( universe->IsVerticalOnly() )
                {
                    // If cuts haven't been checked for this set of vertical-only universes,
                    // check cuts for the first time
                    if ( !vert_universe_checked_cuts ) {
                        vert_universe_IsSigReg = IsSigReg(event);
                        
                        // Once universe has been checked for the first time, be sure that
                        // other vertical-only universes within this error band are not checked
                        vert_universe_checked_cuts = true;
                    }
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_cuts )
                        event.m_is_SigReg = vert_universe_IsSigReg;
                }
                
                
                // If universe is not vertical-only, check cuts universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else
                    event.m_is_SigReg = IsSigReg(event);
                
                
                // Fill histograms
                // ===============
                
                // Check if event is in signal region
                if ( event.m_is_SigReg )
                {
                    // Loop over variables
                    for ( auto var : variables )
                    {
                        // Fill MC
                        if ( type_DataMCTruth == kMC )
                        {
                            // For reco-only variables
                            if ( !(var->m_is_true) ) {
                                ccpi0_event::FillMCHists_Selection(event, var);
                                ccpi0_event::FillMigrationHists(event, var, variables);
                            }
                            
                            // For true-only variables
                            else if ( var->m_is_true )
                                ccpi0_event::FillEffNumerator(event, var);
                        }
                        
                    }  // End of loop over variables
                }
            }  // End of loop over universes
            
            
            // If event is not CCPi0 signal, exit loop over error bands
            if ( !event_is_signal ) break;
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
    
#endif  // __CINT__
}





// ========================================================================================================================
//  PROCESS MC TUPLES
// ========================================================================================================================

void ProcessMCTuples(const CCPi0::MacroUtil& util,
                     TFile& fout,
                     const EnumModels& type_model,
                     std::string option_material)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(true);  // Include true variables
    
    
    // Initialize variables
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> InitMCHists_Selection(util.m_error_bands);
            var -> InitMigrationHists(util.m_error_bands);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> InitEffNumerator(util.m_error_bands);
            if ( util.m_do_truth ) var -> InitEffDenominator(util.m_error_bands_truth);
        }
    }
    
    
    // Loop and fill histograms
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, variables, type_model, option_material);
    
    if ( util.m_do_truth ) {
        MinervaUniverse::SetTruth(true);
        LoopAndFillHistograms(util, kTruth, variables, type_model, option_material);
    }
    
    
    // Write POT
    WritePOT(fout, true, util.m_mc_pot);
    
    
    // Sync histograms
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> SyncMCHists_Selection();
            var -> SyncMigrationHists();
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> SyncEffNumerator();
            if ( util.m_do_truth ) var -> SyncEffDenominator();
        }
    }
    
    
    // Write histograms
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) ) {
            var -> WriteMCHists_Selection(fout);
            var -> WriteMigrationHists(fout);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> WriteEffNumerator(fout);
            if ( util.m_do_truth ) var -> WriteEffDenominator(fout);
        }
    }
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

void SetMacroUtilMC(TFile& fout_lead, TFile& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model,
                    bool do_truth,
                    bool do_systematics)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string, do_truth, do_systematics, type_model);
    util.PrintMacroConfiguration("FakeDataModels");
    
    
    // Process event selection on LEAD
    ProcessMCTuples(util, fout_lead, type_model, "lead");
    
    
    // Process event selection on IRON
    ProcessMCTuples(util, fout_iron, type_model, "iron");
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void FakeDataModels(std::string plist_string,
                    std::string option_model = "v1",
                    bool is_grid             = false,
                    bool do_truth            = true,
                    bool do_systematics      = false,
                    std::string input_file   = "",
                    int run                  = 0)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Stop program if input file is not specified when running in the grid
    assert(!(is_grid && input_file.empty()) && " Input file must be specified when running on grid!!! ");
    
    
    // File list:
    // -> For grid jobs, uses input parameter
    // -> Interactively, uses 'test' sample as default (to change this, add a 3rd argument = "full")
    const std::string file_list = is_grid ? input_file : GetPlaylistFile(true, plist_string);  // Set 'true' to always use MC
    
    
    
    // =========================================
    //  Output file
    // =========================================
    
    // General option for output file names
    const std::string option_systematics = do_systematics ? "WithSyst" : "NoSyst";
    
    
    // Construct output file name on LEAD
    std::string fout_name_lead = Form("MC_FakeData_MnvGENIE%s_%s_%s_lead", option_model.c_str(),
                                                                           option_systematics.c_str(),
                                                                           plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_iron = Form("MC_FakeData_MnvGENIE%s_%s_%s_iron", option_model.c_str(),
                                                                           option_systematics.c_str(),
                                                                           plist_string.c_str());
    
    
    // If run number is different than 0, add it to output file names
    if ( run != 0 ) {
        fout_name_lead = fout_name_lead + Form("_%d", run);
        fout_name_iron = fout_name_iron + Form("_%d", run);
    }
    
    
    // If macro is not in grid, add top output directories manually
    if ( !is_grid ) {
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/FakeDataModels/root_files_interactive_test/";  // Test sample
        fout_name_lead = fout_topdir + fout_name_lead;
        fout_name_iron = fout_topdir + fout_name_iron;
    }
    
    
    // Add ROOT extension
    fout_name_lead = fout_name_lead + ".root";
    fout_name_iron = fout_name_iron + ".root";
    
    
    // Define output files
    TFile fout_lead(fout_name_lead.c_str(), "RECREATE");
    TFile fout_iron(fout_name_iron.c_str(), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  Set MacroUtil and fill histograms
    // =========================================
    
    SetMacroUtilMC(fout_lead, fout_iron,
                   plist_string, file_list, type_model,
                   do_truth, do_systematics);
    
    
    // Close ROOT files
    fout_lead.Close();
    fout_iron.Close();
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // FakeDataModels_C