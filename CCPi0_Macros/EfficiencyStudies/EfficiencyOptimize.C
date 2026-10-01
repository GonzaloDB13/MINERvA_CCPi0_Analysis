#ifndef EfficiencyOptimize_C
#define EfficiencyOptimize_C

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
//  GET VECTOR OF CUTS
// ========================================================================================================================

std::vector<EnumCuts> GetCutsVector()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    //cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobTheta);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobdEdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxFront);
    cuts_vec.push_back(kRecoCut_BlobdEdxShowerEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_MaxTwoBlobs);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}





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
    std::cout << " \tStarting loop to fill efficiency optimization distributions on " << option_material << std::endl;
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
    
    
    // Get vector of cuts to apply
    std::vector<EnumCuts> selection_cuts = GetCutsVector();
    
    
    // Loop over entries
    // =================
    
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Boolean to check if vertical-only universe has been checked by reco cuts, by default it's false.
        // Once the first time a vertical-only universe is evaluated, it becomes true and
        // there won't be a need to check again until the next entry
        bool vert_universe_checked_cuts = false;
        
        
        // Boolean to check if vertical-only universe passes all signal region cuts, except the muon track angle
        // since that cut will also depend on the corresponding muon angle-dependent signal.
        // Similarly, it only needs to be evaluated once for a vertical-only universe
        bool vert_universe_passes_cuts = false;
        
        
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
                
                
                // Get true variables to define if event is CC1pi0 signal (except true muon angle)
                bool true_cc1pi0 = event.m_universe->IsTrueCC_1Pi0();
                
                bool true_material = false;
                if ( option_material == "lead" )
                    true_material = (event.m_universe->IsTrueTgt4Pb() || event.m_universe->IsTrueTgt5Pb());
                else if ( option_material == "iron" )
                    true_material = event.m_universe->IsTrueTgt5Fe();
                
                
                // Get true muon angle
                double true_muon_theta = event.m_universe->GetMuonTheta_True();
                
                
                // Fill efficiency denominator (only when using Truth three)
                // ===========================
                
                if ( type_DataMCTruth == kTruth )
                {
                    // Loop over variables and fill histograms
                    for ( auto var : variables )
                    {
                        // For true-only variables
                        if ( var->m_is_true )
                        {
                            // For events that meet conditions of CC1pi0 signal without true muon angle
                            if ( true_material && true_cc1pi0 && ValidateMCTruthVariable(event, var) )
                            {
                                if ( var->Name() == "Eff_ThetaMu10" && true_muon_theta <= 10.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu11" && true_muon_theta <= 11.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu12" && true_muon_theta <= 12.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu13" && true_muon_theta <= 13.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu14" && true_muon_theta <= 14.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu15" && true_muon_theta <= 15.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu16" && true_muon_theta <= 16.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu17" && true_muon_theta <= 17.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu18" && true_muon_theta <= 18.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu19" && true_muon_theta <= 19.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu20" && true_muon_theta <= 20.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu21" && true_muon_theta <= 21.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu22" && true_muon_theta <= 22.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu23" && true_muon_theta <= 23.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu24" && true_muon_theta <= 24.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                                if ( var->Name() == "Eff_ThetaMu25" && true_muon_theta <= 25.0 ) {
                                    double var_value = var->GetValue(*event.m_universe);
                                    var->m_hists.m_mc_EffDenominator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                }
                            }
                        }
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
                        vert_universe_passes_cuts = PassesCuts(event, selection_cuts);
                        
                        // Once universe has been checked for the first time, be sure that
                        // other vertical-only universes within this error band are not checked
                        vert_universe_checked_cuts = true;
                    }
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_cuts )
                        event.m_passes_cuts = vert_universe_passes_cuts;
                }
                
                
                // If universe is not vertical-only, check cuts universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else
                    event.m_passes_cuts = PassesCuts(event, selection_cuts);
                
                
                // Fill histograms
                // ===============
                
                // Get reco muon angle to define if event belongs to the muon angle-dependent signal region
                double reco_muon_theta = event.m_universe->GetMuonTheta();
                
                
                // Check if event passes reco cuts (excluding reco muon angle)
                if ( event.m_passes_cuts )
                {
                    // Loop over variables
                    for ( auto var : variables )
                    {
                        // Fill MC
                        if ( type_DataMCTruth == kMC )
                        {
                            // For true-only variables
                            if ( var->m_is_true )
                            {
                                // For events that meet conditions of CC1pi0 signal without true muon angle
                                if ( true_material && true_cc1pi0 && ValidateMCTruthVariable(event, var) )
                                {
                                    if ( var->Name() == "Eff_ThetaMu10" && true_muon_theta <= 10.0 && reco_muon_theta <= 10.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu11" && true_muon_theta <= 11.0 && reco_muon_theta <= 11.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu12" && true_muon_theta <= 12.0 && reco_muon_theta <= 12.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu13" && true_muon_theta <= 13.0 && reco_muon_theta <= 13.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu14" && true_muon_theta <= 14.0 && reco_muon_theta <= 14.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu15" && true_muon_theta <= 15.0 && reco_muon_theta <= 15.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu16" && true_muon_theta <= 16.0 && reco_muon_theta <= 16.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu17" && true_muon_theta <= 17.0 && reco_muon_theta <= 17.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu18" && true_muon_theta <= 18.0 && reco_muon_theta <= 18.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu19" && true_muon_theta <= 19.0 && reco_muon_theta <= 19.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu20" && true_muon_theta <= 20.0 && reco_muon_theta <= 20.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu21" && true_muon_theta <= 21.0 && reco_muon_theta <= 21.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu22" && true_muon_theta <= 22.0 && reco_muon_theta <= 22.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu23" && true_muon_theta <= 23.0 && reco_muon_theta <= 23.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu24" && true_muon_theta <= 24.0 && reco_muon_theta <= 24.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                    if ( var->Name() == "Eff_ThetaMu25" && true_muon_theta <= 25.0 && reco_muon_theta <= 25.0 ) {
                                        double var_value = var->GetValue(*event.m_universe);
                                        var->m_hists.m_mc_EffNumerator.FillUniverse(*event.m_universe, var_value, event.m_weight);
                                    }
                                }
                            }
                        }  // End of fill MC
                        
                    }  // End of loop over variables
                }
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
    std::vector<Variable*> variables = GetEffStudyVariables(true);  // Include true variables
    
    
    // Initialize variables
    for ( auto var : variables )
    {
        // For true-only variables
        if ( var->m_is_true ) {
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
        // For true-only variables
        if ( var->m_is_true ) {
            var -> SyncEffNumerator();
            if ( util.m_do_truth ) var -> SyncEffDenominator();
        }
    }
    
    
    // Write histograms
    for ( auto var : variables )
    {
        // For true-only variables
        if ( var->m_is_true ) {
            var -> WriteEffNumerator(fout);
            if ( util.m_do_truth ) var -> WriteEffDenominator(fout);
        }
    }
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

void SetMacroUtilMC(TFile& fout_lead,
                    TFile& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    bool is_grid,
                    const EnumModels& type_model,
                    bool do_truth,
                    bool do_systematics)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string, do_truth, is_grid, do_systematics, type_model);
    util.PrintMacroConfiguration("EfficiencyOptimize");
    
    
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

void EfficiencyOptimize(std::string plist_string,
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
    std::string fout_name_lead = Form("MC_EfficiencyOptimize_MnvGENIE%s_%s_%s_lead", option_model.c_str(),
                                                                                     option_systematics.c_str(),
                                                                                     plist_string.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_iron = Form("MC_EfficiencyOptimize_MnvGENIE%s_%s_%s_iron", option_model.c_str(),
                                                                                     option_systematics.c_str(),
                                                                                     plist_string.c_str());
    
    
    // If run number is different than 0, add it to output file names
    if ( run != 0 ) {
        fout_name_lead = fout_name_lead + Form("_%d", run);
        fout_name_iron = fout_name_iron + Form("_%d", run);
    }
    
    
    // If macro is not in grid, add top output directories manually
    if ( !is_grid ) {
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/EfficiencyStudies/root_files_interactive_test/";  // Test sample
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
    SetMacroUtilMC(fout_lead, fout_iron,
                   plist_string, file_list, is_grid, type_model,
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


#endif  // EfficiencyOptimize_C