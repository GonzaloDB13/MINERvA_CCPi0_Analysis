#ifndef SupportStudies_C
#define SupportStudies_C

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
                           std::vector<Variable2D*>& variables2D,
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
        bool vert_universe_passes_cuts = false;
        
        bool vert_universe_IsSigReg_nofid     = false;
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
                
                
                // Check reconstruction cuts
                // =========================
                
                // Check if universe of this error band is vertical-only.
                // If it is vertical-only, cuts would need to be checked only once
                if ( universe->IsVerticalOnly() )
                {
                    // If cuts of any of both kinds of events haven't been checked for this set of vertical-only universes,
                    // check cuts for the first time
                    if ( !vert_universe_checked_cuts ) {
                        vert_universe_passes_cuts = PassesCut(event, kRecoCut_InteractionVertex);
                        
                        // Once the event of this universe has been checked for the first time,
                        // make sure that other events in this vertical-only universes within this error band are not checked
                        vert_universe_checked_cuts = true;
                    }
                    
                    // Do similar for events without fiducial volume cut
                    if ( !vert_universe_checked_cuts_nofid ) {
                        vert_universe_IsSigReg_nofid     = IsSigReg(event_nofid);
                        vert_universe_IsRecoPb_nofid     = IsRecoPb(event_nofid);
                        vert_universe_IsRecoFe_nofid     = IsRecoFe(event_nofid);
                        vert_universe_IsPlasUpSB_nofid   = IsPlasUpSB(event_nofid);
                        vert_universe_IsPlasBetwSB_nofid = IsPlasBetwSB(event_nofid);
                        vert_universe_IsPlasDownSB_nofid = IsPlasDownSB(event_nofid);
                        
                        vert_universe_checked_cuts_nofid = true;
                    }
                    
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_cuts_nofid ) {
                        event_nofid.m_is_SigReg     = vert_universe_IsSigReg_nofid;
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
                    vert_universe_passes_cuts = PassesCut(event, kRecoCut_InteractionVertex);
                    
                    event_nofid.m_is_SigReg     = IsSigReg(event_nofid);
                    event_nofid.m_is_RecoPb     = IsRecoPb(event_nofid);
                    event_nofid.m_is_RecoFe     = IsRecoFe(event_nofid);
                    event_nofid.m_is_PlasUpSB   = IsPlasUpSB(event_nofid);
                    event_nofid.m_is_PlasBetwSB = IsPlasBetwSB(event_nofid);
                    event_nofid.m_is_PlasDownSB = IsPlasDownSB(event_nofid);
                }
                
                
                // Fill true photon info
                // =====================
                
                // Check if event passes vertex cut
                if ( vert_universe_passes_cuts && event.m_is_signal )
                {
                    // Loop over 1D variables
                    for ( auto var : variables )
                    {
                        // Check right variables
                        if ( var->Name() == "Gamma1TrueE" || var->Name() == "Gamma2TrueE" )
                        {
                            // Fill MC
                            if ( type_DataMCTruth == kMC ) {
                                if ( var->GetValue(*event.m_universe) > 0.0 )
                                    ccpi0_event::FillMCHists_MatSelection(event, var);
                            }
                        }
                    }  // End of loop over 1D variables
                    
                    // Loop over 2D variables
                    for ( auto var2D : variables2D )
                    {
                        // Check right variables
                        if ( var2D->Name() == "Gamma1EnergyLoss" || var2D->Name() == "Gamma2EnergyLoss" )
                        {
                            // Fill MC
                            if ( type_DataMCTruth == kMC ) {
                                if ( var2D->GetValueY(*event.m_universe) > 0.0 )
                                    ccpi0_event::FillMCHists_MatSelection2D(event, var2D);
                            }
                        }
                    }  // End of loop over 1D variables
                    
                }  // End of fill event selection histograms
                
                
                // Fill muon vertex in Pb/Fe and plastic sidebands
                // ===============================================
                
                // Check if event is either signal region or physis sidebands
                if ( event_nofid.m_is_SigReg )
                {
                    // Loop over 1D variables
                    for ( auto var : variables )
                    {
                        // Check right variable
                        if ( var->Name() == "MuonVertexPlane" )
                        {
                            // Fill MC
                            if ( type_DataMCTruth == kMC )
                            {
                                // Signal region
                                if ( event_nofid.m_is_SigReg ) {
                                    if ( event_nofid.m_is_RecoPb )          ccpi0_event::FillMCHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_RecoFe )     ccpi0_event::FillMCHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasUpSB )   ccpi0_event::FillMCHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillMCHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillMCHists_MatSelection(event_nofid, var);
                                }
                            }  // End of fill MC
                        
                            // Fill data
                            else if ( type_DataMCTruth == kData )
                            {
                                // Signal region
                                if ( event_nofid.m_is_SigReg ) {
                                    if ( event_nofid.m_is_RecoPb )          ccpi0_event::FillDataHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_RecoFe )     ccpi0_event::FillDataHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasUpSB )   ccpi0_event::FillDataHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasBetwSB ) ccpi0_event::FillDataHists_MatSelection(event_nofid, var);
                                    else if ( event_nofid.m_is_PlasDownSB ) ccpi0_event::FillDataHists_MatSelection(event_nofid, var);
                                }
                            }  // End of fill data
                        }
                    }  // End of loop over 1D variables
                    
                }  // End of fill muon vertex in Pb/Fe and plastic sidebands
                
            }  // End of loop over universes
            
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
    std::vector<Variable*>   variables   = GetSupportVariables();
    std::vector<Variable2D*> variables2D = GetSupportVariables2D();
    
    
    // Initialize variables
    // ====================
    
    for ( auto var : variables ) {
        var -> InitMCHists_MatSelection(util.m_error_bands);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> InitMCHists_MatSelection2D(util.m_error_bands);
    }
    
    
    // Loop and fill histograms
    // ========================
    
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, variables, variables2D, type_model, option_material);
    
    
    // Write POT
    // =========
    
    WritePOT(fout, true, util.m_mc_pot);
    
    
    // Sync and write histograms
    // =========================
    
    for ( auto var : variables ) {
        var -> SyncMCHists_MatSelection();
        var -> WriteMCHists_MatSelection(fout);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> SyncMCHists_MatSelection2D();
        var2D -> WriteMCHists_MatSelection2D(fout);
    }
}





// ========================================================================================================================
//  PROCESS DATA TUPLES
// ========================================================================================================================

void ProcessDataTuples(const CCPi0::MacroUtil& util,
                       TFile& fout,
                       std::string option_material)
{
    // Get variables
    std::vector<Variable*>   variables   = GetSupportVariables();
    std::vector<Variable2D*> variables2D = GetSupportVariables2D();
    
    
    // Initialize variables
    // ====================
    
    for ( auto var : variables ) {
        var -> InitDataHists_MatSelection();
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> InitDataHists_MatSelection2D();
    }
    
    
    // Loop and fill histograms
    // ========================
    
    LoopAndFillHistograms(util, kData, variables, variables2D, kDataNoModel, option_material);
    
    
    // Write POT
    // =========
    
    WritePOT(fout, false, util.m_data_pot);
    
    
    // Write histograms
    // ================
    
    for ( auto var : variables ) {
        var -> WriteDataHists_MatSelection(fout);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> WriteDataHists_MatSelection2D(fout);
    }
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

// =============
//  Monte Carlo
// =============
void SetMacroUtilMC(TFile& fout_lead,
                    TFile& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model)
{
    // Set MacroUtil
    // (Set 'Truth' option as TRUE and 'Systematics' option as FALSE)
    CCPi0::MacroUtil util(file_list, plist_string, false, false, type_model);
    util.PrintMacroConfiguration("SupportStudies");
    
    
    // Process iron
    ProcessMCTuples(util, fout_lead, type_model, "lead");
    
    
    // Process iron
    ProcessMCTuples(util, fout_iron, type_model, "iron");
}



// ======
//  Data
// ======
void SetMacroUtilData(TFile& fout_lead,
                      TFile& fout_iron,
                      std::string plist_string,
                      std::string file_list)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string);
    util.PrintMacroConfiguration("SupportStudies");
    
    
    // Process LEAD
    ProcessDataTuples(util, fout_lead, "lead");
    
    
    // Process IRON
    ProcessDataTuples(util, fout_iron, "iron");
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void SupportStudies(bool is_mc,
                    std::string plist_string,
                    std::string option_model = "data",
                    bool is_grid             = false,
                    std::string input_file   = "",
                    int run                  = 0)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Stop program if running over data and MC-only options are on
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
    const std::string option_mc_data = is_mc ? "MC" : "Data";
    
    
    // Construct output file name on LEAD
    std::string fout_name_lead = is_mc ?
        Form("%s_SupportStudies_MnvGENIE%s_%s_lead", option_mc_data.c_str(),
                                                     option_model.c_str(),
                                                     plist_string.c_str()) :
        Form("%s_SupportStudies_%s_lead", option_mc_data.c_str(),
                                          plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_iron = is_mc ?
        Form("%s_SupportStudies_MnvGENIE%s_%s_iron", option_mc_data.c_str(),
                                                     option_model.c_str(),
                                                     plist_string.c_str()) :
        Form("%s_SupportStudies_%s_iron", option_mc_data.c_str(),
                                          plist_string.c_str());
    
    
    // Define output file names for interactive or grid
    // ================================================
    
    // If run number is different than 0, add it to output file names
    if ( run != 0 ) {
        fout_name_lead = fout_name_lead + Form("_%d", run);
        fout_name_iron = fout_name_iron + Form("_%d", run);
    }
    
    
    // If macro is not in grid, add top output directories manually
    if ( !is_grid ) {
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/SupportStudies/root_files_interactive_test/";  // Test sample
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
    
    // Monte Carlo
    if ( is_mc )
        SetMacroUtilMC(fout_lead, fout_iron, plist_string, file_list, type_model);
    
    // Data
    else
        SetMacroUtilData(fout_lead, fout_iron, plist_string, file_list);
    
    
    // Close ROOT files
    fout_lead.Close();
    fout_iron.Close();
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // SupportStudies_C