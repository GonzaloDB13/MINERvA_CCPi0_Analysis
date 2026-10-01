#ifndef EventSelection_C
#define EventSelection_C

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
    std::cout << " \tStarting loop to fill distributions on " << option_material << "... " << std::endl;
    SetupLoop(type_DataMCTruth, type_model, util, is_mc, is_truth, n_entries);
    
    
    // Get error bands
    UniverseMap error_bands;
    
    if ( type_DataMCTruth == kMC || type_DataMCTruth == kTruth ) {
        if ( is_truth ) error_bands = util.m_error_bands_truth;
        else            error_bands = util.m_error_bands;
    }
    
    else if ( type_DataMCTruth == kData ) {
        CVUniverse* data_universe = util.m_data_universe;
        std::vector<CVUniverse*> data_band = {data_universe};
        error_bands["cv_data"] = data_band;
    }
    
    
    // Define TStopwatch
    TStopwatch sw;
    
    
    // Loop over entries
    // =================
    
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Booleans to check if any event on a vertical-only universe has been checked by reco cuts, by default it's false.
        // Once the first time an event on a vertical-only universe is evaluated, it becomes true and
        // there won't be a need to check again until the next entry.
        // Create two booleans for both kinds of events to evaluate: with and without fiducial cut
        bool vert_universe_checked_cuts       = false;
        bool vert_universe_checked_cuts_nofid = false;
        
        
        // Booleans to check if any event in a vertical-only universe fulfills signal region or any plastic/physics sideband.
        // Similarly, it only needs to be evaluated once for an event on a vertical-only universe.
        // Create multiple booleans for both kinds of events to evaluate: with and without fiducial cut
        bool vert_universe_IsSigReg       = false;
        bool vert_universe_IsPionBlobSB   = false;
        bool vert_universe_IsProtonBlobSB = false;
        bool vert_universe_IsHighWSB      = false;
        
        bool vert_universe_IsSigReg_nofid       = false;
        bool vert_universe_IsPionBlobSB_nofid   = false;
        bool vert_universe_IsProtonBlobSB_nofid = false;
        bool vert_universe_IsHighWSB_nofid      = false;
        
        bool vert_universe_IsRecoPb_nofid     = false;
        bool vert_universe_IsRecoFe_nofid     = false;
        bool vert_universe_IsPlasUpSB_nofid   = false;
        bool vert_universe_IsPlasBetwSB_nofid = false;
        bool vert_universe_IsPlasDownSB_nofid = false;
        
        
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
                
                
                // Define two kind of CCPi0 events:
                // a) For event selection and physics sidebands purposes (WITH cut on fiducial volume)
                // b) For plastic sidebands purposes (WITHOUT cut on fiducial volume)
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                CCPi0Event event_nofid(is_mc, is_truth, universe, type_model, option_material, false);
                
                
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
                    // If cuts of any of both kinds of events haven't been checked for this set of vertical-only universes,
                    // check cuts for the first time
                    if ( !vert_universe_checked_cuts ) {
                        vert_universe_IsSigReg       = IsSigReg(event);
                        vert_universe_IsPionBlobSB   = IsPionBlobSB(event);
                        vert_universe_IsProtonBlobSB = IsProtonBlobSB(event);
                        vert_universe_IsHighWSB      = IsHighWSB(event);
                        
                        // Once the event of this universe has been checked for the first time,
                        // make sure that other events in this vertical-only universes within this error band are not checked
                        vert_universe_checked_cuts = true;
                    }
                    
                    // Do similar for events without fiducial volume cut
                    if ( !vert_universe_checked_cuts_nofid ) {
                        vert_universe_IsSigReg_nofid       = IsSigReg(event_nofid);
                        vert_universe_IsPionBlobSB_nofid   = IsPionBlobSB(event_nofid);
                        vert_universe_IsProtonBlobSB_nofid = IsProtonBlobSB(event_nofid);
                        vert_universe_IsHighWSB_nofid      = IsHighWSB(event_nofid);
                        
                        vert_universe_IsRecoPb_nofid     = IsRecoPb(event_nofid);
                        vert_universe_IsRecoFe_nofid     = IsRecoFe(event_nofid);
                        vert_universe_IsPlasUpSB_nofid   = IsPlasUpSB(event_nofid);
                        vert_universe_IsPlasBetwSB_nofid = IsPlasBetwSB(event_nofid);
                        vert_universe_IsPlasDownSB_nofid = IsPlasDownSB(event_nofid);
                        
                        vert_universe_checked_cuts_nofid = true;
                    }
                    
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_cuts ) {
                        event.m_is_SigReg       = vert_universe_IsSigReg;
                        event.m_is_PionBlobSB   = vert_universe_IsPionBlobSB;
                        event.m_is_ProtonBlobSB = vert_universe_IsProtonBlobSB;
                        event.m_is_HighWSB      = vert_universe_IsHighWSB;
                    }
                    
                    if ( vert_universe_checked_cuts_nofid ) {
                        event_nofid.m_is_SigReg       = vert_universe_IsSigReg_nofid;
                        event_nofid.m_is_PionBlobSB   = vert_universe_IsPionBlobSB_nofid;
                        event_nofid.m_is_ProtonBlobSB = vert_universe_IsProtonBlobSB_nofid;
                        event_nofid.m_is_HighWSB      = vert_universe_IsHighWSB_nofid;
                        
                        event_nofid.m_is_RecoPb     = vert_universe_IsRecoPb_nofid;
                        event_nofid.m_is_RecoFe     = vert_universe_IsRecoFe_nofid;
                        event_nofid.m_is_PlasUpSB   = vert_universe_IsPlasUpSB_nofid;
                        event_nofid.m_is_PlasBetwSB = vert_universe_IsPlasBetwSB_nofid;
                        event_nofid.m_is_PlasDownSB = vert_universe_IsPlasDownSB_nofid;
                    }
                }
                
                
                // If universe is not vertical-only, check cuts on both events universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else {
                    event.m_is_SigReg       = IsSigReg(event);
                    event.m_is_PionBlobSB   = IsPionBlobSB(event);
                    event.m_is_ProtonBlobSB = IsProtonBlobSB(event);
                    event.m_is_HighWSB      = IsHighWSB(event);
                    
                    event_nofid.m_is_SigReg       = IsSigReg(event_nofid);
                    event_nofid.m_is_PionBlobSB   = IsPionBlobSB(event_nofid);
                    event_nofid.m_is_ProtonBlobSB = IsProtonBlobSB(event_nofid);
                    event_nofid.m_is_HighWSB      = IsHighWSB(event_nofid);
                    
                    event_nofid.m_is_RecoPb     = IsRecoPb(event_nofid);
                    event_nofid.m_is_RecoFe     = IsRecoFe(event_nofid);
                    event_nofid.m_is_PlasUpSB   = IsPlasUpSB(event_nofid);
                    event_nofid.m_is_PlasBetwSB = IsPlasBetwSB(event_nofid);
                    event_nofid.m_is_PlasDownSB = IsPlasDownSB(event_nofid);
                }
                
                
                // Fill event selection histograms
                // ===============================
                
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
                        
                        // Fill data
                        if ( type_DataMCTruth == kData ) {
                            if ( !(var->m_is_true) )
                                ccpi0_event::FillDataHists_Selection(event, var);
                        }
                    }  // End of loop over variables
                    
                }  // End of fill event selection histograms
                
                
                // Fill plastic sideband histograms
                // ================================
                
                // Check if event is either signal region or physis sidebands
                if ( event_nofid.m_is_SigReg || event_nofid.m_is_PionBlobSB || event_nofid.m_is_ProtonBlobSB || event_nofid.m_is_HighWSB )
                {
                    // Loop over variables
                    for ( auto var : variables )
                    {
                        // Fill MC
                        if ( type_DataMCTruth == kMC )
                        {
                            // For reco-only variables
                            if ( !(var->m_is_true) )
                            {
                                // Signal region
                                if ( event_nofid.m_is_SigReg ) {
                                    if ( event_nofid.m_is_RecoPb )          ccpi0_event::FillMCHists_RecoPb_In_SigReg(event_nofid,     var);
                                    else if ( event_nofid.m_is_RecoFe )     ccpi0_event::FillMCHists_RecoFe_In_SigReg(event_nofid,     var);
                                    else if ( event_nofid.m_is_PlasUpSB )   ccpi0_event::FillMCHists_PlasUpSB_In_SigReg(event_nofid,   var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillMCHists_PlasBetwSB_In_SigReg(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillMCHists_PlasDownSB_In_SigReg(event_nofid, var);
                                }
                                
                                // Pion-like shower sideband
                                else if ( event_nofid.m_is_PionBlobSB ) {
                                    if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillMCHists_PlasUpSB_In_PionBlobSB(event_nofid,   var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillMCHists_PlasBetwSB_In_PionBlobSB(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillMCHists_PlasDownSB_In_PionBlobSB(event_nofid, var);
                                }
                                
                                // Proton-like shower sideband
                                else if ( event_nofid.m_is_ProtonBlobSB ) {
                                    if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillMCHists_PlasUpSB_In_ProtonBlobSB(event_nofid,   var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillMCHists_PlasBetwSB_In_ProtonBlobSB(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillMCHists_PlasDownSB_In_ProtonBlobSB(event_nofid, var);
                                }
                                
                                // High-W sideband
                                else if ( event_nofid.m_is_HighWSB ) {
                                    if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillMCHists_PlasUpSB_In_HighWSB(event_nofid,   var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillMCHists_PlasBetwSB_In_HighWSB(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillMCHists_PlasDownSB_In_HighWSB(event_nofid, var);
                                }
                            }
                        }  // End of fill MC
                        
                        // Fill data
                        else if ( type_DataMCTruth == kData )
                        {
                            // Signal region
                            if ( event_nofid.m_is_SigReg ) {
                                if ( event_nofid.m_is_RecoPb )          ccpi0_event::FillDataHists_RecoPb_In_SigReg(event_nofid,     var);
                                else if ( event_nofid.m_is_RecoFe )     ccpi0_event::FillDataHists_RecoFe_In_SigReg(event_nofid,     var);
                                else if ( event_nofid.m_is_PlasUpSB )   ccpi0_event::FillDataHists_PlasUpSB_In_SigReg(event_nofid,   var);
                                else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillDataHists_PlasBetwSB_In_SigReg(event_nofid, var);
                                else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillDataHists_PlasDownSB_In_SigReg(event_nofid, var);
                            }
                            
                            // Pion-like shower sideband
                            else if ( event_nofid.m_is_PionBlobSB ) {
                                if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillDataHists_PlasUpSB_In_PionBlobSB(event_nofid,   var);
                                else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillDataHists_PlasBetwSB_In_PionBlobSB(event_nofid, var);
                                else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillDataHists_PlasDownSB_In_PionBlobSB(event_nofid, var);
                            }
                            
                            // Proton-like shower sideband
                            else if ( event_nofid.m_is_ProtonBlobSB ) {
                                if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillDataHists_PlasUpSB_In_ProtonBlobSB(event_nofid,   var);
                                else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillDataHists_PlasBetwSB_In_ProtonBlobSB(event_nofid, var);
                                else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillDataHists_PlasDownSB_In_ProtonBlobSB(event_nofid, var);
                            }
                            
                            // High-W sideband
                            else if ( event_nofid.m_is_HighWSB ) {
                                if ( event_nofid.m_is_PlasUpSB )        ccpi0_event::FillDataHists_PlasUpSB_In_HighWSB(event_nofid,   var);
                                else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillDataHists_PlasBetwSB_In_HighWSB(event_nofid, var);
                                else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillDataHists_PlasDownSB_In_HighWSB(event_nofid, var);
                            }
                        }  // End of fill data
                        
                    }  // End of loop over variables
                    
                }  // End of fill plastic sideband histograms
                
                
                // Fill physics sideband histograms
                // ================================
                
                // Check if event is in either signal region or any physics sideband
                if ( event.m_is_SigReg || event.m_is_PionBlobSB || event.m_is_ProtonBlobSB || event.m_is_HighWSB )
                {
                    // Loop over variables
                    for ( auto var : variables )
                    {
                        // Fill MC
                        if ( type_DataMCTruth == kMC )
                        {
                            // For reco-only variables
                            if ( !(var->m_is_true) ) 
                            {
                                if ( event.m_is_SigReg )            ccpi0_event::FillMCHists_SigReg(event,       var);
                                else if ( event.m_is_PionBlobSB )   ccpi0_event::FillMCHists_PionBlobSB(event,   var);
                                else if ( event.m_is_ProtonBlobSB ) ccpi0_event::FillMCHists_ProtonBlobSB(event, var);
                                else if ( event.m_is_HighWSB )      ccpi0_event::FillMCHists_HighWSB(event,      var);
                            }
                        }
                        
                        // Fill data
                        if ( type_DataMCTruth == kData )
                        {
                            if ( event.m_is_SigReg )            ccpi0_event::FillDataHists_SigReg(event,       var);
                            else if ( event.m_is_PionBlobSB )   ccpi0_event::FillDataHists_PionBlobSB(event,   var);
                            else if ( event.m_is_ProtonBlobSB ) ccpi0_event::FillDataHists_ProtonBlobSB(event, var);
                            else if ( event.m_is_HighWSB )      ccpi0_event::FillDataHists_HighWSB(event,      var);
                        }
                        
                    }  // End of loop over variables
                    
                }  // End of fill physics sideband histograms
                
            }  // End of loop over universes
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
    
#endif  // __CINT__
}





// ========================================================================================================================
//  PROCESS MC TUPLES
// ========================================================================================================================

void ProcessMCTuples(const CCPi0::MacroUtil& util,
                     TFile& fout_evsel,
                     TFile& fout_plastun,
                     TFile& fout_phystun,
                     const EnumModels& type_model,
                     std::string option_material)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(true);  // Include true variables
    
    
    // Initialize variables
    // ====================
    
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) )
        {
            // Event selection and migration
            var -> InitMCHists_Selection(util.m_error_bands);
            var -> InitMigrationHists(util.m_error_bands);
            
            // Plastic sidebands
            var -> InitMCHists_PlasSB_In_SigReg(util.m_error_bands);
            var -> InitMCHists_PlasSB_In_PhysSB(util.m_error_bands);
            
            // Physics sidebands
            var -> InitMCHists_PhysSB(util.m_error_bands);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> InitEffNumerator(util.m_error_bands);
            if ( util.m_do_truth ) var -> InitEffDenominator(util.m_error_bands_truth);
        }
    }
    
    
    // Loop and fill histograms
    // ========================
    
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, variables, type_model, option_material);
    
    if ( util.m_do_truth ) {
        MinervaUniverse::SetTruth(true);
        LoopAndFillHistograms(util, kTruth, variables, type_model, option_material);
    }
    
    
    // Write POT
    // =========
    
    WritePOT(fout_evsel,   true, util.m_mc_pot);
    WritePOT(fout_plastun, true, util.m_mc_pot);
    WritePOT(fout_phystun, true, util.m_mc_pot);
    
    
    // Sync histograms
    // ===============
    
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) )
        {
            // Event selection and migration
            var -> SyncMCHists_Selection();
            var -> SyncMigrationHists();
            
            // Plastic sidebands
            var -> SyncMCHists_PlasSB_In_SigReg();
            var -> SyncMCHists_PlasSB_In_PhysSB();
            
            // Physics sidebands
            var -> SyncMCHists_PhysSB();
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> SyncEffNumerator();
            if ( util.m_do_truth ) var -> SyncEffDenominator();
        }
    }
    
    
    // Write histograms
    // ================
    
    for ( auto var : variables )
    {
        // For reco-only variables
        if ( !(var->m_is_true) )
        {
            // Event selection and migration
            var -> WriteMCHists_Selection(fout_evsel);
            var -> WriteMigrationHists(fout_evsel);
            
            // Plastic sidebands
            var -> WriteMCHists_PlasSB_In_SigReg(fout_plastun);
            var -> WriteMCHists_PlasSB_In_PhysSB(fout_plastun);
            
            // Physics sidebands
            var -> WriteMCHists_PhysSB(fout_phystun);
        }
        
        // For true-only variables
        else if ( var->m_is_true ) {
            var -> WriteEffNumerator(fout_evsel);
            if ( util.m_do_truth ) var -> WriteEffDenominator(fout_evsel);
        }
    }
}





// ========================================================================================================================
//  PROCESS DATA TUPLES
// ========================================================================================================================

void ProcessDataTuples(const CCPi0::MacroUtil& util,
                       TFile& fout_evsel,
                       TFile& fout_plastun,
                       TFile& fout_phystun,
                       std::string option_material)
{
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    
    // Initialize variables
    // ====================
    
    for ( auto var : variables )
    {
        // Event selection
        var -> InitDataHists_Selection();
        
        // Plastic sidebands
        var -> InitDataHists_PlasSB_In_SigReg();
        var -> InitDataHists_PlasSB_In_PhysSB();
        
        // Physics sidebands
        var -> InitDataHists_PhysSB();
    }
    
    
    // Loop and fill histograms
    // ========================
    
    LoopAndFillHistograms(util, kData, variables, kDataNoModel, option_material);
    
    
    // Write POT
    // =========
    
    WritePOT(fout_evsel,   false, util.m_data_pot);
    WritePOT(fout_plastun, false, util.m_data_pot);
    WritePOT(fout_phystun, false, util.m_data_pot);
    
    
    // Write histograms
    // ================
    
    for ( auto var : variables )
    {
        // Event selection
        var -> WriteDataHists_Selection(fout_evsel);
        
        // Plastic sidebands
        var -> WriteDataHists_PlasSB_In_SigReg(fout_plastun);
        var -> WriteDataHists_PlasSB_In_PhysSB(fout_plastun);
        
        // Physics sidebands
        var -> WriteDataHists_PhysSB(fout_phystun);
    }
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

// =============
//  Monte Carlo
// =============
void SetMacroUtilMC(TFile& fout_evsel_lead,   TFile& fout_evsel_iron,
                    TFile& fout_plastun_lead, TFile& fout_plastun_iron,
                    TFile& fout_phystun_lead, TFile& fout_phystun_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model,
                    bool do_truth,
                    bool do_systematics)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string, do_truth, do_systematics, type_model);
    util.PrintMacroConfiguration("EventSelection");
    
    
    // Process iron
    ProcessMCTuples(util, fout_evsel_lead, fout_plastun_lead, fout_phystun_lead, type_model, "lead");
    
    
    // Process iron
    ProcessMCTuples(util, fout_evsel_iron, fout_plastun_iron, fout_phystun_iron, type_model, "iron");
}



// ======
//  Data
// ======
void SetMacroUtilData(TFile& fout_evsel_lead,   TFile& fout_evsel_iron,
                      TFile& fout_plastun_lead, TFile& fout_plastun_iron,
                      TFile& fout_phystun_lead, TFile& fout_phystun_iron,
                      std::string plist_string,
                      std::string file_list)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string);
    util.PrintMacroConfiguration("EventSelection");
    
    
    // Process LEAD
    ProcessDataTuples(util, fout_evsel_lead, fout_plastun_lead, fout_phystun_lead, "lead");
    
    
    // Process IRON
    ProcessDataTuples(util, fout_evsel_iron, fout_plastun_iron, fout_phystun_iron, "iron");
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void EventSelection(bool is_mc,
                    std::string plist_string,
                    std::string option_model = "data",
                    bool is_grid             = false,
                    bool do_truth            = true,
                    bool do_systematics      = false,
                    std::string input_file   = "",
                    int run                  = 0)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Stop program if running over data and MC-only options are on
    assert(!(!is_mc && do_truth)       && " When running data, 'do_truth' can't be true!!! ");
    assert(!(!is_mc && do_systematics) && " When running data, 'do_systematics' can't be true!!! ");
    assert(!(!is_mc && type_model != kDataNoModel) && " When running data, 'option_model' must be set as 'data'!!! ");
    assert(!(is_mc  && type_model == kDataNoModel) && " When running MC, 'option_model' must not be set as 'data'!!! ");
    
    
    // Stop program if input file is not specified when running in the grid
    assert(!(is_grid && input_file.empty()) && " Input file must be specified when running on grid!!! ");
    
    
    // File list:
    // -> For grid jobs, uses input parameter
    // -> Interactively, uses 'test' sample as default (to change this, add a 3rd argument = "full")
    const std::string file_list = is_grid ? input_file : GetPlaylistFile(is_mc, plist_string);
    
    
    
    // =========================================
    //  Output file
    // =========================================
    
    // General options for output file names
    const std::string option_mc_data     = is_mc          ? "MC"        : "Data";
    const std::string option_systematics = do_systematics ? "WithSyst"  : "NoSyst";
    
    
    // Event selection file names
    // ==========================
    
    // Construct output file name on LEAD
    std::string fout_name_evsel_lead = is_mc ?
        Form("%s_EventSelection_MnvGENIE%s_%s_%s_lead", option_mc_data.c_str(),
                                                        option_model.c_str(),
                                                        option_systematics.c_str(),
                                                        plist_string.c_str()) :
        Form("%s_EventSelection_%s_lead", option_mc_data.c_str(),
                                          plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_evsel_iron = is_mc ?
        Form("%s_EventSelection_MnvGENIE%s_%s_%s_iron", option_mc_data.c_str(),
                                                        option_model.c_str(),
                                                        option_systematics.c_str(),
                                                        plist_string.c_str()) :
        Form("%s_EventSelection_%s_iron", option_mc_data.c_str(),
                                          plist_string.c_str());
    
    
    // Pre-plastic tuning file names
    // =============================
    
    // Construct output file name on LEAD
    std::string fout_name_plastun_lead = is_mc ?
        Form("%s_BeforePlasticTuning_MnvGENIE%s_%s_%s_lead", option_mc_data.c_str(),
                                                             option_model.c_str(),
                                                             option_systematics.c_str(),
                                                             plist_string.c_str()) :
        Form("%s_PlasticTuning_%s_lead", option_mc_data.c_str(),
                                         plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_plastun_iron = is_mc ?
        Form("%s_BeforePlasticTuning_MnvGENIE%s_%s_%s_iron", option_mc_data.c_str(),
                                                             option_model.c_str(),
                                                             option_systematics.c_str(),
                                                             plist_string.c_str()) :
        Form("%s_PlasticTuning_%s_iron", option_mc_data.c_str(),
                                         plist_string.c_str());
    
    
    // Pre-physics tuning file names
    // =============================
    
    // Construct output file name on LEAD
    std::string fout_name_phystun_lead = is_mc ?
        Form("%s_BeforePhysicsTuning_MnvGENIE%s_%s_%s_lead", option_mc_data.c_str(),
                                                             option_model.c_str(),
                                                             option_systematics.c_str(),
                                                             plist_string.c_str()) :
        Form("%s_PhysicsTuning_%s_lead", option_mc_data.c_str(),
                                         plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_phystun_iron = is_mc ?
        Form("%s_BeforePhysicsTuning_MnvGENIE%s_%s_%s_iron", option_mc_data.c_str(),
                                                             option_model.c_str(),
                                                             option_systematics.c_str(),
                                                             plist_string.c_str()) :
        Form("%s_PhysicsTuning_%s_iron", option_mc_data.c_str(),
                                         plist_string.c_str());
    
    
    // Define output file names for interactive or grid
    // ================================================
    
    // If run number is different than 0, add it to output file names
    if ( run != 0 ) {
        fout_name_evsel_lead = fout_name_evsel_lead + Form("_%d", run);
        fout_name_evsel_iron = fout_name_evsel_iron + Form("_%d", run);
        
        fout_name_plastun_lead = fout_name_plastun_lead + Form("_%d", run);
        fout_name_plastun_iron = fout_name_plastun_iron + Form("_%d", run);
        
        fout_name_phystun_lead = fout_name_phystun_lead + Form("_%d", run);
        fout_name_phystun_iron = fout_name_phystun_iron + Form("_%d", run);
    }
    
    
    // If macro is not in grid, add top output directories manually
    if ( !is_grid ) {
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/EventSelection/root_files_interactive_test/";  // Test sample
        const std::string is_mc_dir   = is_mc ? "mc/" : "data/";
        
        fout_name_evsel_lead = fout_topdir + is_mc_dir + fout_name_evsel_lead;
        fout_name_evsel_iron = fout_topdir + is_mc_dir + fout_name_evsel_iron;
        
        fout_name_plastun_lead = fout_topdir + is_mc_dir + fout_name_plastun_lead;
        fout_name_plastun_iron = fout_topdir + is_mc_dir + fout_name_plastun_iron;
        
        fout_name_phystun_lead = fout_topdir + is_mc_dir + fout_name_phystun_lead;
        fout_name_phystun_iron = fout_topdir + is_mc_dir + fout_name_phystun_iron;
    }
    
    
    // Add ROOT extension
    fout_name_evsel_lead = fout_name_evsel_lead + ".root";
    fout_name_evsel_iron = fout_name_evsel_iron + ".root";
    
    fout_name_plastun_lead = fout_name_plastun_lead + ".root";
    fout_name_plastun_iron = fout_name_plastun_iron + ".root";
    
    fout_name_phystun_lead = fout_name_phystun_lead + ".root";
    fout_name_phystun_iron = fout_name_phystun_iron + ".root";
    
    
    // Define output files
    TFile fout_evsel_lead(fout_name_evsel_lead.c_str(), "RECREATE");
    TFile fout_evsel_iron(fout_name_evsel_iron.c_str(), "RECREATE");
    
    TFile fout_plastun_lead(fout_name_plastun_lead.c_str(), "RECREATE");
    TFile fout_plastun_iron(fout_name_plastun_iron.c_str(), "RECREATE");
    
    TFile fout_phystun_lead(fout_name_phystun_lead.c_str(), "RECREATE");
    TFile fout_phystun_iron(fout_name_phystun_iron.c_str(), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  Set MacroUtil and fill histograms
    // =========================================
    
    // Monte Carlo
    if ( is_mc )
        SetMacroUtilMC(fout_evsel_lead, fout_evsel_iron,
                       fout_plastun_lead, fout_plastun_iron,
                       fout_phystun_lead, fout_phystun_iron,
                       plist_string, file_list, type_model,
                       do_truth, do_systematics);
    
    // Data
    else
        SetMacroUtilData(fout_evsel_lead, fout_evsel_iron,
                         fout_plastun_lead, fout_plastun_iron,
                         fout_phystun_lead, fout_phystun_iron,
                         plist_string, file_list);
    
    
    // Close ROOT files
    fout_evsel_lead.Close();
    fout_evsel_iron.Close();
    
    fout_plastun_lead.Close();
    fout_plastun_iron.Close();
    
    fout_phystun_lead.Close();
    fout_phystun_iron.Close();
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // EventSelection_C