#ifndef CutStudies_C
#define CutStudies_C

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
#include "BookHistograms.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#include "../includes/Variable2D.h"
#endif  // __CINT__

#include "TFile.h"
#include "TStopwatch.h"





// ========================================================================================================================
//  VECTOR OF CUTS
// ========================================================================================================================

std::vector<EnumCuts> GetCutsVector(std::string cut_to_study)
{
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kNoRecoCuts);
    if ( cut_to_study == "NoCuts" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    if ( cut_to_study == "Until_Vertex" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    if ( cut_to_study == "Until_NuHelicity" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_MinosMatch);
    if ( cut_to_study == "Until_MinosMatch" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_MuonCharge);
    if ( cut_to_study == "Until_MuonCharge" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    if ( cut_to_study == "Until_MuonAngle" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_DeadTime);
    if ( cut_to_study == "Until_DeadTime" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    if ( cut_to_study == "Until_Fiducial" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    if ( cut_to_study == "Until_Michel" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_LongTracks);
    if ( cut_to_study == "Until_Track" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    if ( cut_to_study == "Until_AngleScan" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    if ( cut_to_study == "Until_BlobAngleWRTMuon" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    if ( cut_to_study == "Until_BlobDeviation" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    if ( cut_to_study == "Until_BlobEnergyVSdx" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    if ( cut_to_study == "Until_BlobdEdxEnd" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    if ( cut_to_study == "Until_BlobMichel" ) return cuts_vec;
    
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    if ( cut_to_study == "Until_Recoil" ) return cuts_vec;
    
    return cuts_vec;
}





// ========================================================================================================================
//  LOOP OVER ERROR BANDS AND FILL TRUTH HISTOGRAMS
// ========================================================================================================================

#ifndef __CINT__

void LoopAndFillHistogramsTruth(const CCPi0::MacroUtil& util,
                                const EnumDataMCTruth& type_DataMCTruth,
                                const EnumModels& type_model,
                                std::string option_material,
                                std::vector<Variable*>& eventcount_variables_truth)
{
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
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Loop over error bands
        for ( auto error_band : error_bands )
        {
            // Get universes of this error band
            std::vector<CVUniverse*> universes = error_band.second;
            
            
            // Loop over universes
            for ( auto universe : universes )
            {
                // Set entry of this universe
                universe -> SetEntry(i_event);
                
                
                // Define CCPi0 event
                // (Set material option and include cut on fiducial volume)
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                
                
                // Fill histograms in Truth
                // ========================
                
                if ( type_DataMCTruth == kTruth ) {
                    FillEventCountVariablesTruth(type_DataMCTruth, event, eventcount_variables_truth);
                    
                    // After filling, go to next universe immediately
                    continue;
                }
            }  // End of loop over universes
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
}

#endif  // __CINT__





// ========================================================================================================================
//  LOOP OVER ERROR BANDS AND FILL HISTOGRAMS
// ========================================================================================================================

#ifndef __CINT__

void LoopAndFillHistograms(const CCPi0::MacroUtil& util,
                           const EnumDataMCTruth& type_DataMCTruth,
                           const EnumModels& type_model,
                           std::string option_material,
                           std::vector<Variable*>& eventcount_variables,
                           std::vector<Variable*>& rejectedevent_variables,
                           std::vector<Variable*>& michel_variables,
                           std::vector<Variable*>& track_variables,
                           std::vector<Variable*>& anglescan_variables,
                           std::vector<Variable*>& blobanglewrtmuon_variables,
                           std::vector<Variable*>& blobdeviation_variables,
                           std::vector<Variable*>& blobenergyVSdx_variables,
                           std::vector<Variable*>& blobdEdxend_variables,
                           std::vector<Variable*>& afterblobenergy_variables,
                           std::vector<Variable*>& blobmichel_variables,
                           std::vector<Variable*>& energy_variables,
                           std::vector<Variable*>& xsection_variables,
                           std::vector<Variable2D*>& blobdeviation_variables2D,
                           std::vector<Variable2D*>& blobenergyVSdx_variables2D,
                           std::vector<Variable2D*>& blobdEdxend_variables2D,
                           std::vector<Variable2D*>& afterblobenergy_variables2D)
{
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
    
    
    // Get vector of cuts for each cut to study
    std::vector<EnumCuts> no_cuts                     = GetCutsVector("NoCuts");
    std::vector<EnumCuts> cuts_until_Vertex           = GetCutsVector("Until_Vertex");
    std::vector<EnumCuts> cuts_until_NuHelicity       = GetCutsVector("Until_NuHelicity");
    std::vector<EnumCuts> cuts_until_MinosMatch       = GetCutsVector("Until_MinosMatch");
    std::vector<EnumCuts> cuts_until_MuonCharge       = GetCutsVector("Until_MuonCharge");
    std::vector<EnumCuts> cuts_until_MuonAngle        = GetCutsVector("Until_MuonAngle");
    std::vector<EnumCuts> cuts_until_DeadTime         = GetCutsVector("Until_DeadTime");
    std::vector<EnumCuts> cuts_until_Fiducial         = GetCutsVector("Until_Fiducial");
    std::vector<EnumCuts> cuts_until_Michel           = GetCutsVector("Until_Michel");
    std::vector<EnumCuts> cuts_until_Track            = GetCutsVector("Until_Track");
    std::vector<EnumCuts> cuts_until_AngleScan        = GetCutsVector("Until_AngleScan");
    std::vector<EnumCuts> cuts_until_BlobAngleWRTMuon = GetCutsVector("Until_BlobAngleWRTMuon");
    std::vector<EnumCuts> cuts_until_BlobDeviation    = GetCutsVector("Until_BlobDeviation");
    std::vector<EnumCuts> cuts_until_BlobEnergyVSdx   = GetCutsVector("Until_BlobEnergyVSdx");
    std::vector<EnumCuts> cuts_until_BlobdEdxEnd      = GetCutsVector("Until_BlobdEdxEnd");
    std::vector<EnumCuts> cuts_until_BlobMichel       = GetCutsVector("Until_BlobMichel");
    std::vector<EnumCuts> cuts_until_Recoil           = GetCutsVector("Until_Recoil");
    
    
    // Loop over entries
    // =================
    
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Boolean to check if vertical-only universe has been checked by cuts, by default it's false.
        // Once the first time a vertical-only universe is evaluated, it becomes true and
        // there won't be a need to check again until the next entry
        bool vert_universe_checked_Vertex           = false;
        bool vert_universe_checked_NuHelicity       = false;
        bool vert_universe_checked_MinosMatch       = false;
        bool vert_universe_checked_MuonCharge       = false;
        bool vert_universe_checked_MuonAngle        = false;
        bool vert_universe_checked_DeadTime         = false;
        bool vert_universe_checked_Fiducial         = false;
        bool vert_universe_checked_Michel           = false;
        bool vert_universe_checked_Track            = false;
        bool vert_universe_checked_AngleScan        = false;
        bool vert_universe_checked_BlobAngleWRTMuon = false;
        bool vert_universe_checked_BlobDeviation    = false;
        bool vert_universe_checked_BlobEnergyVSdx   = false;
        bool vert_universe_checked_BlobdEdxEnd      = false;
        bool vert_universe_checked_BlobMichel       = false;
        bool vert_universe_checked_Recoil           = false;
        
        
        // Booleans to check if vertical-only universe fulfills reconstructed cuts.
        // Similarly, it only needs to be evaluated once for a vertical-only universe
        bool vert_universe_passes_Vertex           = false;
        bool vert_universe_passes_NuHelicity       = false;
        bool vert_universe_passes_MinosMatch       = false;
        bool vert_universe_passes_MuonCharge       = false;
        bool vert_universe_passes_MuonAngle        = false;
        bool vert_universe_passes_DeadTime         = false;
        bool vert_universe_passes_Fiducial         = false;
        bool vert_universe_passes_Michel           = false;
        bool vert_universe_passes_Track            = false;
        bool vert_universe_passes_AngleScan        = false;
        bool vert_universe_passes_BlobAngleWRTMuon = false;
        bool vert_universe_passes_BlobDeviation    = false;
        bool vert_universe_passes_BlobEnergyVSdx   = false;
        bool vert_universe_passes_BlobdEdxEnd      = false;
        bool vert_universe_passes_BlobMichel       = false;
        bool vert_universe_passes_Recoil           = false;
        
        
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
                
                
                // Define CCPi0 event
                // (Set material option and include cut on fiducial volume)
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                
                
                // Check cuts
                // ==========
                
                bool event_passes_Vertex           = false;
                bool event_passes_NuHelicity       = false;
                bool event_passes_MinosMatch       = false;
                bool event_passes_MuonCharge       = false;
                bool event_passes_MuonAngle        = false;
                bool event_passes_DeadTime         = false;
                bool event_passes_Fiducial         = false;
                bool event_passes_Michel           = false;
                bool event_passes_Track            = false;
                bool event_passes_AngleScan        = false;
                bool event_passes_BlobAngleWRTMuon = false;
                bool event_passes_BlobDeviation    = false;
                bool event_passes_BlobEnergyVSdx   = false;
                bool event_passes_BlobdEdxEnd      = false;
                bool event_passes_BlobMichel       = false;
                bool event_passes_Recoil           = false;
                
                
                // Check if universe of this error band is vertical-only.
                // If it is vertical-only, cuts would need to be checked only once
                if ( universe->IsVerticalOnly() )
                {
                    // If cuts haven't been checked for this set of vertical-only universes, check cuts for the first time
                    // Once universe has been checked for the first time, be sure that other vertical-only universes within this error band are not checked
                    if ( !vert_universe_checked_Vertex ) {
                        vert_universe_passes_Vertex = PassesCuts(event, cuts_until_Vertex);
                        vert_universe_checked_Vertex = true;
                    }
                    if ( !vert_universe_checked_NuHelicity ) {
                        vert_universe_passes_NuHelicity = PassesCuts(event, cuts_until_NuHelicity);
                        vert_universe_checked_NuHelicity = true;
                    }
                    if ( !vert_universe_checked_MinosMatch ) {
                        vert_universe_passes_MinosMatch = PassesCuts(event, cuts_until_MinosMatch);
                        vert_universe_checked_MinosMatch = true;
                    }
                    if ( !vert_universe_checked_MuonCharge ) {
                        vert_universe_passes_MuonCharge = PassesCuts(event, cuts_until_MuonCharge);
                        vert_universe_checked_MuonCharge = true;
                    }
                    if ( !vert_universe_checked_MuonAngle ) {
                        vert_universe_passes_MuonAngle = PassesCuts(event, cuts_until_MuonAngle);
                        vert_universe_checked_MuonAngle = true;
                    }
                    if ( !vert_universe_checked_DeadTime ) {
                        vert_universe_passes_DeadTime = PassesCuts(event, cuts_until_DeadTime);
                        vert_universe_checked_DeadTime = true;
                    }
                    if ( !vert_universe_checked_Fiducial ) {
                        vert_universe_passes_Fiducial = PassesCuts(event, cuts_until_Fiducial);
                        vert_universe_checked_Fiducial = true;
                    }
                    if ( !vert_universe_checked_Michel ) {
                        vert_universe_passes_Michel = PassesCuts(event, cuts_until_Michel);
                        vert_universe_checked_Michel = true;
                    }
                    if ( !vert_universe_checked_Track ) {
                        vert_universe_passes_Track = PassesCuts(event, cuts_until_Track);
                        vert_universe_checked_Track = true;
                    }
                    if ( !vert_universe_checked_AngleScan ) {
                        vert_universe_passes_AngleScan = PassesCuts(event, cuts_until_AngleScan);
                        vert_universe_checked_AngleScan = true;
                    }
                    if ( !vert_universe_checked_BlobAngleWRTMuon ) {
                        vert_universe_passes_BlobAngleWRTMuon = PassesCuts(event, cuts_until_BlobAngleWRTMuon);
                        vert_universe_checked_BlobAngleWRTMuon = true;
                    }
                    if ( !vert_universe_checked_BlobDeviation ) {
                        vert_universe_passes_BlobDeviation = PassesCuts(event, cuts_until_BlobDeviation);
                        vert_universe_checked_BlobDeviation = true;
                    }
                    if ( !vert_universe_checked_BlobEnergyVSdx ) {
                        vert_universe_passes_BlobEnergyVSdx = PassesCuts(event, cuts_until_BlobEnergyVSdx);
                        vert_universe_checked_BlobEnergyVSdx = true;
                    }
                    if ( !vert_universe_checked_BlobdEdxEnd ) {
                        vert_universe_passes_BlobdEdxEnd = PassesCuts(event, cuts_until_BlobdEdxEnd);
                        vert_universe_checked_BlobdEdxEnd = true;
                    }
                    if ( !vert_universe_checked_BlobMichel ) {
                        vert_universe_passes_BlobMichel = PassesCuts(event, cuts_until_BlobMichel);
                        vert_universe_checked_BlobMichel = true;
                    }
                    if ( !vert_universe_checked_Recoil ) {
                        vert_universe_passes_Recoil = PassesCuts(event, cuts_until_Recoil);
                        vert_universe_checked_Recoil = true;
                    }
                    
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_Vertex )           event_passes_Vertex           = vert_universe_passes_Vertex;
                    if ( vert_universe_checked_NuHelicity )       event_passes_NuHelicity       = vert_universe_passes_NuHelicity;
                    if ( vert_universe_checked_MinosMatch )       event_passes_MinosMatch       = vert_universe_passes_MinosMatch;
                    if ( vert_universe_checked_MuonCharge )       event_passes_MuonCharge       = vert_universe_passes_MuonCharge;
                    if ( vert_universe_checked_MuonAngle )        event_passes_MuonAngle        = vert_universe_passes_MuonAngle;
                    if ( vert_universe_checked_DeadTime )         event_passes_DeadTime         = vert_universe_passes_DeadTime;
                    if ( vert_universe_checked_Fiducial )         event_passes_Fiducial         = vert_universe_passes_Fiducial;
                    if ( vert_universe_checked_Michel )           event_passes_Michel           = vert_universe_passes_Michel;
                    if ( vert_universe_checked_Track )            event_passes_Track            = vert_universe_passes_Track;
                    if ( vert_universe_checked_AngleScan )        event_passes_AngleScan        = vert_universe_passes_AngleScan;
                    if ( vert_universe_checked_BlobAngleWRTMuon ) event_passes_BlobAngleWRTMuon = vert_universe_passes_BlobAngleWRTMuon;
                    if ( vert_universe_checked_BlobDeviation )    event_passes_BlobDeviation    = vert_universe_passes_BlobDeviation;
                    if ( vert_universe_checked_BlobEnergyVSdx )   event_passes_BlobEnergyVSdx   = vert_universe_passes_BlobEnergyVSdx;
                    if ( vert_universe_checked_BlobdEdxEnd )      event_passes_BlobdEdxEnd      = vert_universe_passes_BlobdEdxEnd;
                    if ( vert_universe_checked_BlobMichel )       event_passes_BlobMichel       = vert_universe_passes_BlobMichel;
                    if ( vert_universe_checked_Recoil )           event_passes_Recoil           = vert_universe_passes_Recoil;
                }
                
                
                // If universe is not vertical-only, check cuts universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else {
                    event_passes_Vertex           = PassesCuts(event, cuts_until_Vertex);
                    event_passes_NuHelicity       = PassesCuts(event, cuts_until_NuHelicity);
                    event_passes_MinosMatch       = PassesCuts(event, cuts_until_MinosMatch);
                    event_passes_MuonCharge       = PassesCuts(event, cuts_until_MuonCharge);
                    event_passes_MuonAngle        = PassesCuts(event, cuts_until_MuonAngle);
                    event_passes_DeadTime         = PassesCuts(event, cuts_until_DeadTime);
                    event_passes_Fiducial         = PassesCuts(event, cuts_until_Fiducial);
                    event_passes_Michel           = PassesCuts(event, cuts_until_Michel);
                    event_passes_Track            = PassesCuts(event, cuts_until_Track);
                    event_passes_AngleScan        = PassesCuts(event, cuts_until_AngleScan);
                    event_passes_BlobAngleWRTMuon = PassesCuts(event, cuts_until_BlobAngleWRTMuon);
                    event_passes_BlobDeviation    = PassesCuts(event, cuts_until_BlobDeviation);
                    event_passes_BlobEnergyVSdx   = PassesCuts(event, cuts_until_BlobEnergyVSdx);
                    event_passes_BlobdEdxEnd      = PassesCuts(event, cuts_until_BlobdEdxEnd);
                    event_passes_BlobMichel       = PassesCuts(event, cuts_until_BlobMichel);
                    event_passes_Recoil           = PassesCuts(event, cuts_until_Recoil);
                }
                
                
                // Fill histograms in MC reco and data
                // ===================================
                
                if ( type_DataMCTruth == kMC || type_DataMCTruth == kData )
                {
                    // No cuts
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "NoCuts");
                    
                    
                    // Interaction vertex cut
                    if ( !event_passes_Vertex ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "Vertex");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "Vertex");
                    
                    
                    // Neutrino helicity cut
                    if ( !event_passes_NuHelicity ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "NuHelicity");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "NuHelicity");
                    
                    
                    // MINOS match cut
                    if ( !event_passes_MinosMatch ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "MinosMatch");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "MinosMatch");
                    
                    
                    // Muon charge cut
                    if ( !event_passes_MuonCharge ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "MuonCharge");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "MuonCharge");
                    
                    
                    // Muon track angle cut
                    if ( !event_passes_MuonAngle ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "MuonAngle");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "MuonAngle");
                    
                    
                    // Dead time cut
                    if ( !event_passes_DeadTime ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "DeadTime");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "DeadTime");
                    
                    
                    // Fiducial volume cut
                    if ( !event_passes_Fiducial ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "Fiducial");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "Fiducial");
                    
                    
                    // Michel cut
                    FillVariables(type_DataMCTruth, event, michel_variables, "Michel");
                    
                    if ( !event_passes_Michel ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "Michel");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "Michel");
                    
                    
                    // Long track cut
                    FillVariables(type_DataMCTruth, event, track_variables, "Track");
                    
                    if ( !event_passes_Track ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "Track");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "Track");
                    
                    
                    // AngleScan cut
                    FillVariables(type_DataMCTruth, event, anglescan_variables, "AngleScan");
                    
                    if ( !event_passes_AngleScan ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "AngleScan");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "AngleScan");
                    
                    
                    // Blob angle w.r.t. muon cut
                    FillVariables(type_DataMCTruth, event, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
                    
                    if ( !event_passes_BlobAngleWRTMuon ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "BlobAngleWRTMuon");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "BlobAngleWRTMuon");
                    
                    
                    // Blob deviation cut
                    FillVariables(type_DataMCTruth,   event, blobdeviation_variables,   "BlobDeviation");
                    FillVariables2D(type_DataMCTruth, event, blobdeviation_variables2D, "BlobDeviation");
                    
                    if ( !event_passes_BlobDeviation ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "BlobDeviation");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "BlobDeviation");
                    
                    
                    // Blob energy VS dx cut
                    FillVariables(type_DataMCTruth,   event, blobenergyVSdx_variables,   "BlobEnergyVSdx");
                    FillVariables2D(type_DataMCTruth, event, blobenergyVSdx_variables2D, "BlobEnergyVSdx");
                    
                    if ( !event_passes_BlobEnergyVSdx ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "BlobEnergyVSdx");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "BlobEnergyVSdx");
                    
                    
                    // Blob end dE/dx cut
                    FillVariables(type_DataMCTruth,   event, blobdEdxend_variables,   "BlobdEdxEnd");
                    FillVariables2D(type_DataMCTruth, event, blobdEdxend_variables2D, "BlobdEdxEnd");
                    
                    if ( !event_passes_BlobdEdxEnd ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "BlobdEdxEnd");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "BlobdEdxEnd");
                    
                    FillVariables(type_DataMCTruth,   event, afterblobenergy_variables,   "AfterBlobEnergy");
                    FillVariables2D(type_DataMCTruth, event, afterblobenergy_variables2D, "AfterBlobEnergy");
                    
                    
                    // Blob Michel cut
                    FillVariables(type_DataMCTruth, event, blobmichel_variables, "BlobMichel");
                    
                    if ( !event_passes_BlobMichel ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "BlobMichel");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "BlobMichel");
                    
                    
                    // Recoil energy cut
                    FillVariables(type_DataMCTruth, event, energy_variables, "Recoil");
                    
                    if ( !event_passes_Recoil ) {
                        FillRejectedEventVariables(type_DataMCTruth, event, rejectedevent_variables, "Recoil");
                        continue;
                    }
                    FillEventCountVariables(type_DataMCTruth, event, eventcount_variables, "Recoil");
                    
                    
                    // All cuts
                    FillVariables(type_DataMCTruth, event, xsection_variables, "AllCuts");
                }
                
            }  // End of loop over universes
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
}

#endif  // __CINT__





// ========================================================================================================================
//  PROCESS MC TUPLES
// ========================================================================================================================

void ProcessMCTuples(const CCPi0::MacroUtil& util,
                     TFile& fout,
                     const EnumModels& type_model,
                     std::string option_material)
{
    // Get variables
    std::vector<Variable*> eventcount_variables_truth = GetEventCountVariablesTruth();
    std::vector<Variable*> eventcount_variables       = GetEventCountVariables();
    std::vector<Variable*> rejectedevent_variables    = GetRejectedEventVariables();
    
    std::vector<Variable*> michel_variables           = GetMichelVariables();
    std::vector<Variable*> track_variables            = GetTrackVariables();
    std::vector<Variable*> anglescan_variables        = GetAngleScanVariables();
    std::vector<Variable*> blobanglewrtmuon_variables = GetBlobAngleWRTMuonVariables();
    std::vector<Variable*> blobdeviation_variables    = GetBlobDeviationVariables();
    std::vector<Variable*> blobenergyVSdx_variables   = GetBlobEnergyVSdxVariables();
    std::vector<Variable*> blobdEdxend_variables      = GetBlobdEdxEndVariables();
    std::vector<Variable*> afterblobenergy_variables  = GetAfterBlobEnergyVariables();
    std::vector<Variable*> blobmichel_variables       = GetBlobMichelVariables();
    std::vector<Variable*> energy_variables           = GetEnergyVariables();
    std::vector<Variable*> xsection_variables         = GetXsecVariables();
    
    std::vector<Variable2D*> blobdeviation_variables2D   = GetBlobDeviationVariables2D();
    std::vector<Variable2D*> blobenergyVSdx_variables2D  = GetBlobEnergyVSdxVariables2D();
    std::vector<Variable2D*> blobdEdxend_variables2D     = GetBlobdEdxEndVariables2D();
    std::vector<Variable2D*> afterblobenergy_variables2D = GetAfterBlobEnergyVariables2D();
    
    
    // Initialize histograms
    InitializeEventCountVariablesTruth(util, kTruth, eventcount_variables_truth);
    InitializeEventCountVariables(util, kMC, eventcount_variables);
    InitializeRejectedEventVariables(util, kMC, rejectedevent_variables);
    
    InitializeVariables(util, kMC, michel_variables,           "Michel");
    InitializeVariables(util, kMC, track_variables,            "Track");
    InitializeVariables(util, kMC, anglescan_variables,        "AngleScan");
    InitializeVariables(util, kMC, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    InitializeVariables(util, kMC, blobdeviation_variables,    "BlobDeviation");
    InitializeVariables(util, kMC, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    InitializeVariables(util, kMC, blobdEdxend_variables,      "BlobdEdxEnd");
    InitializeVariables(util, kMC, afterblobenergy_variables,  "AfterBlobEnergy");
    InitializeVariables(util, kMC, blobmichel_variables,       "BlobMichel");
    InitializeVariables(util, kMC, energy_variables,           "Recoil");
    InitializeVariables(util, kMC, xsection_variables,         "AllCuts");
    
    InitializeVariables2D(util, kMC, blobdeviation_variables2D,   "BlobDeviation");
    InitializeVariables2D(util, kMC, blobenergyVSdx_variables2D,  "BlobEnergyVSdx");
    InitializeVariables2D(util, kMC, blobdEdxend_variables2D,     "BlobdEdxEnd");
    InitializeVariables2D(util, kMC, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    
    // Loop and fill histograms
    if ( util.m_do_truth ) {
        MinervaUniverse::SetTruth(true);
        LoopAndFillHistogramsTruth(util, kTruth, type_model, option_material,
                                   eventcount_variables_truth);
    }
    
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, type_model, option_material,
                          eventcount_variables,
                          rejectedevent_variables,
                          michel_variables,
                          track_variables,
                          anglescan_variables,
                          blobanglewrtmuon_variables,
                          blobdeviation_variables,
                          blobenergyVSdx_variables,
                          blobdEdxend_variables,
                          afterblobenergy_variables,
                          blobmichel_variables,
                          energy_variables,
                          xsection_variables,
                          blobdeviation_variables2D,
                          blobenergyVSdx_variables2D,
                          blobdEdxend_variables2D,
                          afterblobenergy_variables2D);
    
    
    // Write POT
    WritePOT(fout, true, util.m_mc_pot);
    
    
    // Sync and write histograms (in that order)
    SyncAndWriteEventCountVariablesTruth(fout, kTruth, eventcount_variables_truth);
    SyncAndWriteEventCountVariables(fout, kMC, eventcount_variables);
    SyncAndWriteRejectedEventVariables(fout, kMC, rejectedevent_variables);
    
    SyncAndWriteVariables(fout, kMC, michel_variables,           "Michel");
    SyncAndWriteVariables(fout, kMC, track_variables,            "Track");
    SyncAndWriteVariables(fout, kMC, anglescan_variables,        "AngleScan");
    SyncAndWriteVariables(fout, kMC, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    
    SyncAndWriteVariables(fout,   kMC, blobdeviation_variables,   "BlobDeviation");
    SyncAndWriteVariables2D(fout, kMC, blobdeviation_variables2D, "BlobDeviation");
    
    SyncAndWriteVariables(fout,   kMC, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    SyncAndWriteVariables2D(fout, kMC, blobenergyVSdx_variables2D, "BlobEnergyVSdx");
    
    SyncAndWriteVariables(fout,   kMC, blobdEdxend_variables,   "BlobdEdxEnd");
    SyncAndWriteVariables2D(fout, kMC, blobdEdxend_variables2D, "BlobdEdxEnd");
    
    SyncAndWriteVariables(fout,   kMC, afterblobenergy_variables,   "AfterBlobEnergy");
    SyncAndWriteVariables2D(fout, kMC, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    SyncAndWriteVariables(fout, kMC, blobmichel_variables, "BlobMichel");
    SyncAndWriteVariables(fout, kMC, energy_variables,     "Recoil");
    SyncAndWriteVariables(fout, kMC, xsection_variables,   "AllCuts");
}





// ========================================================================================================================
//  PROCESS DATA TUPLES
// ========================================================================================================================

void ProcessDataTuples(const CCPi0::MacroUtil& util,
                       TFile& fout,
                       std::string option_material)
{
    // Get variables
    std::vector<Variable*> eventcount_variables    = GetEventCountVariables();
    std::vector<Variable*> rejectedevent_variables = GetRejectedEventVariables();
    
    std::vector<Variable*> michel_variables           = GetMichelVariables();
    std::vector<Variable*> track_variables            = GetTrackVariables();
    std::vector<Variable*> anglescan_variables        = GetAngleScanVariables();
    std::vector<Variable*> blobanglewrtmuon_variables = GetBlobAngleWRTMuonVariables();
    std::vector<Variable*> blobdeviation_variables    = GetBlobDeviationVariables();
    std::vector<Variable*> blobenergyVSdx_variables   = GetBlobEnergyVSdxVariables();
    std::vector<Variable*> blobdEdxend_variables      = GetBlobdEdxEndVariables();
    std::vector<Variable*> afterblobenergy_variables  = GetAfterBlobEnergyVariables();
    std::vector<Variable*> blobmichel_variables       = GetBlobMichelVariables();
    std::vector<Variable*> energy_variables           = GetEnergyVariables();
    std::vector<Variable*> xsection_variables         = GetXsecVariables();
    
    std::vector<Variable2D*> blobdeviation_variables2D   = GetBlobDeviationVariables2D();
    std::vector<Variable2D*> blobenergyVSdx_variables2D  = GetBlobEnergyVSdxVariables2D();
    std::vector<Variable2D*> blobdEdxend_variables2D     = GetBlobdEdxEndVariables2D();
    std::vector<Variable2D*> afterblobenergy_variables2D = GetAfterBlobEnergyVariables2D();
    
    
    // Initialize histograms
    InitializeEventCountVariables(util, kData, eventcount_variables);
    InitializeRejectedEventVariables(util, kData, rejectedevent_variables);
    
    InitializeVariables(util, kData, michel_variables,           "Michel");
    InitializeVariables(util, kData, track_variables,            "Track");
    InitializeVariables(util, kData, anglescan_variables,        "AngleScan");
    InitializeVariables(util, kData, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    InitializeVariables(util, kData, blobdeviation_variables,    "BlobDeviation");
    InitializeVariables(util, kData, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    InitializeVariables(util, kData, blobdEdxend_variables,      "BlobdEdxEnd");
    InitializeVariables(util, kData, afterblobenergy_variables,  "AfterBlobEnergy");
    InitializeVariables(util, kData, blobmichel_variables,       "BlobMichel");
    InitializeVariables(util, kData, energy_variables,           "Recoil");
    InitializeVariables(util, kData, xsection_variables,         "AllCuts");
    
    InitializeVariables2D(util, kData, blobdeviation_variables2D,   "BlobDeviation");
    InitializeVariables2D(util, kData, blobenergyVSdx_variables2D,  "BlobEnergyVSdx");
    InitializeVariables2D(util, kData, blobdEdxend_variables2D,     "BlobdEdxEnd");
    InitializeVariables2D(util, kData, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    
    // Loop and fill histograms
    LoopAndFillHistograms(util, kData, kDataNoModel, option_material,
                          eventcount_variables,
                          rejectedevent_variables,
                          michel_variables,
                          track_variables,
                          anglescan_variables,
                          blobanglewrtmuon_variables,
                          blobdeviation_variables,
                          blobenergyVSdx_variables,
                          blobdEdxend_variables,
                          afterblobenergy_variables,
                          blobmichel_variables,
                          energy_variables,
                          xsection_variables,
                          blobdeviation_variables2D,
                          blobenergyVSdx_variables2D,
                          blobdEdxend_variables2D,
                          afterblobenergy_variables2D);
    
    
    // Write POT
    WritePOT(fout, false, util.m_data_pot);
    
    
    // Write histograms (in that order)
    SyncAndWriteEventCountVariables(fout, kData, eventcount_variables);
    SyncAndWriteRejectedEventVariables(fout, kData, rejectedevent_variables);
    
    SyncAndWriteVariables(fout, kData, michel_variables,           "Michel");
    SyncAndWriteVariables(fout, kData, track_variables,            "Track");
    SyncAndWriteVariables(fout, kData, anglescan_variables,        "AngleScan");
    SyncAndWriteVariables(fout, kData, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    
    SyncAndWriteVariables(fout,   kData, blobdeviation_variables,   "BlobDeviation");
    SyncAndWriteVariables2D(fout, kData, blobdeviation_variables2D, "BlobDeviation");
    
    SyncAndWriteVariables(fout,   kData, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    SyncAndWriteVariables2D(fout, kData, blobenergyVSdx_variables2D, "BlobEnergyVSdx");
    
    SyncAndWriteVariables(fout,   kData, blobdEdxend_variables,   "BlobdEdxEnd");
    SyncAndWriteVariables2D(fout, kData, blobdEdxend_variables2D, "BlobdEdxEnd");
    
    SyncAndWriteVariables(fout,   kData, afterblobenergy_variables,   "AfterBlobEnergy");
    SyncAndWriteVariables2D(fout, kData, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    SyncAndWriteVariables(fout, kData, blobmichel_variables, "BlobMichel");
    SyncAndWriteVariables(fout, kData, energy_variables,     "Recoil");
    SyncAndWriteVariables(fout, kData, xsection_variables,   "AllCuts");
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

// =============
//  Monte Carlo
// =============
void SetMacroUtilMC(TFile& fout_lead, TFile& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model)
{
    // Set MacroUtil
    // (Set 'Truth' option as TRUE and 'Systematics' option as FALSE)
    CCPi0::MacroUtil util(file_list, plist_string, true, false, type_model);
    util.PrintMacroConfiguration("CutStudies");
    
    
    // Process event selection
    ProcessMCTuples(util, fout_lead, type_model, "lead");
    ProcessMCTuples(util, fout_iron, type_model, "iron");
}



// ======
//  Data
// ======
void SetMacroUtilData(TFile& fout_lead, TFile& fout_iron,
                      std::string plist_string,
                      std::string file_list)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string);
    util.PrintMacroConfiguration("CutStudies");
    
    
    // Process event selection
    ProcessDataTuples(util, fout_lead, "lead");
    ProcessDataTuples(util, fout_iron, "iron");
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void CutStudies(bool is_mc,
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
    
    // Output file names
    // =================
    
    // General option for output file names
    const std::string option_mc_data = is_mc ? "MC" : "Data";
    
    
    // Construct output file name on LEAD
    std::string fout_name_lead = is_mc ?
        Form("%s_CutStudies_MnvGENIE%s_%s_lead", option_mc_data.c_str(),
                                                 option_model.c_str(),
                                                 plist_string.c_str()) :
        Form("%s_CutStudies_%s_lead", option_mc_data.c_str(),
                                      plist_string.c_str());
    
    // Construct output file name on IRON
    std::string fout_name_iron = is_mc ?
        Form("%s_CutStudies_MnvGENIE%s_%s_iron", option_mc_data.c_str(),
                                                 option_model.c_str(),
                                                 plist_string.c_str()) :
        Form("%s_CutStudies_%s_iron", option_mc_data.c_str(),
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
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/CutStudies/root_files_interactive_test/";  // Test sample
        const std::string is_mc_dir   = is_mc ? "mc/" : "data/";
        
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
        SetMacroUtilMC(fout_lead, fout_iron,
                       plist_string, file_list, type_model);
    
    // Data
    else
        SetMacroUtilData(fout_lead, fout_iron,
                         plist_string, file_list);
    
    
    // Close ROOT files
    fout_lead.Close();
    fout_iron.Close();
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // CutStudies_C