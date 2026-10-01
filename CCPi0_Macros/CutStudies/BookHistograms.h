#ifndef BookHistograms_h
#define BookHistograms_h

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
#include "../includes/Variable2D.h"
#endif  // __CINT__



// Forward declare Variable classes
class Variable;
class Variable2D;



// ========================================================================================================================
//  INITIALIZE HISTOGRAMS OF EVENT COUNT VARIABLES
// ========================================================================================================================

// Event count in Truth
// ====================
void InitializeEventCountVariablesTruth(const CCPi0::MacroUtil& util,
                                        const EnumDataMCTruth& type_DataMCTruth,
                                        std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kTruth )
        {
            if ( util.m_do_truth )
                var -> InitMCHists_Selection(util.m_error_bands_truth);
        }
    }
}


// Event count in MC Reco and Data
// ===============================
void InitializeEventCountVariables(const CCPi0::MacroUtil& util,
                                   const EnumDataMCTruth& type_DataMCTruth,
                                   std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
                var -> InitMCHists_ObjectPdg(util.m_error_bands);
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
                var -> InitDataHists_Selection();
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
                var -> InitDataHists_ObjectPdg();
        }
    }
}


// Rejected events
// ===============
void InitializeRejectedEventVariables(const CCPi0::MacroUtil& util,
                                      const EnumDataMCTruth& type_DataMCTruth,
                                      std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
                var -> InitMCHists_ObjectPdg(util.m_error_bands);
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
                var -> InitDataHists_Selection();
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
                var -> InitDataHists_ObjectPdg();
        }
    }
}





// ========================================================================================================================
//  FILL HISTOGRAMS OF EVENT COUNT VARIABLES
// ========================================================================================================================

// Event count in Truth
// ====================
void FillEventCountVariablesTruth(const EnumDataMCTruth& type_DataMCTruth,
                                  const CCPi0Event& event,
                                  std::vector<Variable*> variables)
{
    // Loop over variables
    for ( auto var : variables )
    {
        if ( var->Name() == "EventCount_True" )
        {
            if ( type_DataMCTruth == kTruth )
            {
                var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                if ( event.m_signal_backgr_type == kSignal )
                    var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                else {
                    var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                    if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, 0.5, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, 0.5, event.m_weight);
                }
            }
        }
    }
}


// Event count in MC Reco and Data
// ===============================
void FillEventCountVariables(const EnumDataMCTruth& type_DataMCTruth,
                             const CCPi0Event& event,
                             std::vector<Variable*> variables,
                             std::string cut_to_study)
{
    // Get cut number
    double cut_number;
    if ( cut_to_study == "NoCuts" )                cut_number = 0.5;  // Use bin center
    else if ( cut_to_study == "Vertex" )           cut_number = 1.5;
    else if ( cut_to_study == "NuHelicity" )       cut_number = 2.5;
    else if ( cut_to_study == "MinosMatch" )       cut_number = 3.5;
    else if ( cut_to_study == "MuonCharge" )       cut_number = 4.5;
    else if ( cut_to_study == "MuonAngle" )        cut_number = 5.5;
    else if ( cut_to_study == "DeadTime" )         cut_number = 6.5;
    else if ( cut_to_study == "Fiducial" )         cut_number = 7.5;
    else if ( cut_to_study == "Michel" )           cut_number = 8.5;
    else if ( cut_to_study == "Track" )            cut_number = 9.5;
    else if ( cut_to_study == "AngleScan" )        cut_number = 10.5;
    else if ( cut_to_study == "BlobAngleWRTMuon" ) cut_number = 11.5;
    else if ( cut_to_study == "BlobDeviation" )    cut_number = 12.5;
    else if ( cut_to_study == "BlobEnergyVSdx" )   cut_number = 13.5;
    else if ( cut_to_study == "BlobdEdxEnd" )      cut_number = 14.5;
    else if ( cut_to_study == "BlobMichel" )       cut_number = 15.5;
    else if ( cut_to_study == "Recoil" )           cut_number = 16.5;
    
    
    // Loop over variables
    for ( auto var : variables )
    {
        // Event count for all events
        // ==========================
        if ( var->Name() == "EventCount" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                if ( event.m_signal_backgr_type == kSignal )
                    var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                else {
                    var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
                var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
        }
        
        
        // Event count for single-blob events
        // ==================================
        if ( var->Name() == "EventCount_Single" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs == 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kSignal )
                        var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else {
                        var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Event count for multi-blob events
        // =================================
        if ( var->Name() == "EventCount_Multi" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs > 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kSignal )
                        var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else {
                        var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Blob PDG count for single-blob events
        // =====================================
        if ( var->Name() == "BlobPdg_Single" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs == 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                    var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_ObjectPdg->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Blob PDG count for multi-blob events
        // ====================================
        if ( var->Name() == "BlobPdg_Multi" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs > 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                        if ( Nblobs > 5 ) continue;
                        int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData ) {
                    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                        var->m_hists.m_data_ObjectPdg->Fill(cut_number, event.m_weight);
                    }
                }
            }
        }
    }
}


// Rejected events
// ===============
void FillRejectedEventVariables(const EnumDataMCTruth& type_DataMCTruth,
                                const CCPi0Event& event,
                                std::vector<Variable*> variables,
                                std::string cut_to_study)
{
    // Get cut number
    double cut_number;
    if ( cut_to_study == "NoCuts" )                cut_number = 0.5;  // Use bin center
    else if ( cut_to_study == "Vertex" )           cut_number = 1.5;
    else if ( cut_to_study == "NuHelicity" )       cut_number = 2.5;
    else if ( cut_to_study == "MinosMatch" )       cut_number = 3.5;
    else if ( cut_to_study == "MuonCharge" )       cut_number = 4.5;
    else if ( cut_to_study == "MuonAngle" )        cut_number = 5.5;
    else if ( cut_to_study == "DeadTime" )         cut_number = 6.5;
    else if ( cut_to_study == "Fiducial" )         cut_number = 7.5;
    else if ( cut_to_study == "Michel" )           cut_number = 8.5;
    else if ( cut_to_study == "Track" )            cut_number = 9.5;
    else if ( cut_to_study == "AngleScan" )        cut_number = 10.5;
    else if ( cut_to_study == "BlobAngleWRTMuon" ) cut_number = 11.5;
    else if ( cut_to_study == "BlobDeviation" )    cut_number = 12.5;
    else if ( cut_to_study == "BlobEnergyVSdx" )   cut_number = 13.5;
    else if ( cut_to_study == "BlobdEdxEnd" )      cut_number = 14.5;
    else if ( cut_to_study == "BlobMichel" )       cut_number = 15.5;
    else if ( cut_to_study == "Recoil" )           cut_number = 16.5;
    
    
    // Loop over variables
    for ( auto var : variables )
    {
        // All rejected events
        // ===================
        if ( var->Name() == "RejectedEvent" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                if ( event.m_signal_backgr_type == kSignal )
                    var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                else {
                    var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
                var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
        }
        
        
        // Single-blob rejected events
        // ===========================
        if ( var->Name() == "RejectedEvent_Single" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs == 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kSignal )
                        var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else {
                        var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Multi-blob rejected events
        // ==========================
        if ( var->Name() == "RejectedEvent_Multi" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs > 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( event.m_signal_backgr_type == kSignal )
                        var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else {
                        var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_Selection->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Blob PDG for single-blob rejected events
        // ========================================
        if ( var->Name() == "RejectedBlobPdg_Single" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs == 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                    var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                }
                
                // Data
                if ( type_DataMCTruth == kData )
                    var->m_hists.m_data_ObjectPdg->Fill(cut_number, event.m_weight);
            }
        }
        
        
        // Blob PDG for multi-blob rejected events
        // =======================================
        if ( var->Name() == "RejectedBlobPdg_Multi" )
        {
            int Nblobs = event.m_universe->NblobCandidates();
            if ( cut_number > 10.0 && Nblobs > 1 )
            {
                // Monte Carlo
                if ( type_DataMCTruth == kMC )
                {
                    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                        if ( Nblobs > 5 ) continue;
                        int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, cut_number, event.m_weight);
                    }
                }
                
                // Data
                if ( type_DataMCTruth == kData ) {
                    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                        var->m_hists.m_data_ObjectPdg->Fill(cut_number, event.m_weight);
                    }
                }
            }
        }
    }
}





// ========================================================================================================================
//  SYNC AND WRITE HISTOGRAMS OF EVENT COUNT VARIABLES
// ========================================================================================================================

// Event count in Truth
// ====================
void SyncAndWriteEventCountVariablesTruth(TFile& fout,
                                          const EnumDataMCTruth& type_DataMCTruth,
                                          std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kTruth ) {
            var -> SyncMCHists_Selection();
            var -> WriteMCHists_Selection(fout);
        }
    }
}


// Event count in MC Reco and Data
// ===============================
void SyncAndWriteEventCountVariables(TFile& fout,
                                     const EnumDataMCTruth& type_DataMCTruth,
                                     std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" ) {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" ) {
                var -> SyncMCHists_ObjectPdg();
                var -> WriteMCHists_ObjectPdg(fout);
            }
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
                var -> WriteDataHists_Selection(fout);
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
                var -> WriteDataHists_ObjectPdg(fout);
        }
    }
}


// Rejected events
// ===============
void SyncAndWriteRejectedEventVariables(TFile& fout,
                                        const EnumDataMCTruth& type_DataMCTruth,
                                        std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" ) {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" ) {
                var -> SyncMCHists_ObjectPdg();
                var -> WriteMCHists_ObjectPdg(fout);
            }
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
                var -> WriteDataHists_Selection(fout);
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
                var -> WriteDataHists_ObjectPdg(fout);
        }
    }
}





// ========================================================================================================================
//  LOAD HISTOGRAMS OF EVENT COUNT VARIABLES
// ========================================================================================================================

// Event count in Truth
// ====================
void LoadEventCountVariablesTruth(CCPi0::MacroUtil util,
                                  TFile& fin,
                                  const EnumDataMCTruth& type_DataMCTruth,
                                  std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kTruth )
        {
            if ( util.m_do_truth )
                var -> LoadMCHists_Selection(fin, util.m_error_bands_truth);
        }
    }
}


// Event count in MC Reco and Data
// ===============================
void LoadEventCountVariables(CCPi0::MacroUtil util,
                             TFile& fin,
                             const EnumDataMCTruth& type_DataMCTruth,
                             std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
                var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
                var -> LoadDataHists_Selection(fin);
            
            else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
                var -> LoadDataHists_ObjectPdg(fin);
        }
    }
}


// Rejected events
// ===============
void LoadRejectedEventVariables(CCPi0::MacroUtil util,
                                TFile& fin,
                                const EnumDataMCTruth& type_DataMCTruth,
                                std::vector<Variable*> variables)
{
    for ( auto var : variables )
    {
        if ( type_DataMCTruth == kMC )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
                var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
        }
        
        if ( type_DataMCTruth == kData )
        {
            if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
                var -> LoadDataHists_Selection(fin);
            
            else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
                var -> LoadDataHists_ObjectPdg(fin);
        }
    }
}





// ========================================================================================================================
//  SCALE HISTOGRAMS OF EVENT COUNT VARIABLES
// ========================================================================================================================

// Event count in Truth
// ====================
void ScaleEventCountVariablesTruth(std::vector<Variable*> variables,
                                   double mc_pot,
                                   double data_pot)
{
    for ( auto var : variables ) {
        var -> ScaleMCHists_Selection(mc_pot, data_pot);
    }
}


// Event count in MC Reco and Data
// ===============================
void ScaleEventCountVariables(std::vector<Variable*> variables,
                              double mc_pot,
                              double data_pot)
{
    for ( auto var : variables )
    {
        if ( var->Name() == "EventCount" || var->Name() == "EventCount_Single" || var->Name() == "EventCount_Multi" )
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        
        else if ( var->Name() == "BlobPdg_Single" || var->Name() == "BlobPdg_Multi" )
            var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
    }
}


// Rejected events
// ===============
void ScaleRejectedEventVariables(std::vector<Variable*> variables,
                                 double mc_pot,
                                 double data_pot)
{
    for ( auto var : variables )
    {
        if ( var->Name() == "RejectedEvent" || var->Name() == "RejectedEvent_Single" || var->Name() == "RejectedEvent_Multi" )
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        
        else if ( var->Name() == "RejectedBlobPdg_Single" || var->Name() == "RejectedBlobPdg_Multi" )
            var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
    }
}





// ========================================================================================================================
//  INITIALIZE HISTOGRAMS OF VARIABLES
// ========================================================================================================================

void InitializeVariables(const CCPi0::MacroUtil& util,
                         const EnumDataMCTruth& type_DataMCTruth,
                         std::vector<Variable*> variables,
                         std::string cut_to_study)
{
    for ( auto var : variables )
    {
        // Michel cut
        if ( cut_to_study == "Michel" )
        {
            if ( type_DataMCTruth == kMC )
                var -> InitMCHists_Selection(util.m_error_bands);
                
            if ( type_DataMCTruth == kData ) {
                if ( var->Name() == "MuonPt_BeforeMichel"      ||
                     var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" )
                    var -> InitDataHists_Selection();
            }
        }
        
        
        // Long track cut
        if ( cut_to_study == "Track" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "PrimTrackPionScore" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "PrimTrackPionScore" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // AngleScan cut
        if ( cut_to_study == "AngleScan" )
        {
            if ( type_DataMCTruth == kMC )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> InitDataHists_Selection();
        }
        
        
        // Blob angle w.r.t. muon cut
        if ( cut_to_study == "BlobAngleWRTMuon" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    var -> InitMCHists_Selection(util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                    var -> InitMCHists_ObjectPdg(util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    var -> InitDataHists_Selection();
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                    var -> InitDataHists_ObjectPdg();
            }
        }
        
        
        // Blob Michel cuts
        if ( cut_to_study == "BlobMichel" )
        {
            if ( type_DataMCTruth == kMC )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            if ( type_DataMCTruth == kData ) {
                if ( var->Name() == "MuonPt_BeforeBlobMichel"     ||
                     var->Name() == "Nblobs_BeforeBlobMichel"     ||
                     var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobStartPointMichel_Multi" ||
                     var->Name() == "BlobEndPointMichel_Single"   || var->Name() == "BlobEndPointMichel_Multi" )
                    var -> InitDataHists_Selection();
            }
        }
        
        
        // Recoil energy cut
        if ( cut_to_study == "Recoil" )
        {
            if ( type_DataMCTruth == kMC )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> InitDataHists_Selection();
        }
        
        
        // All cuts
        if ( cut_to_study == "AllCuts" )
        {
            if ( type_DataMCTruth == kMC )
                var -> InitMCHists_Selection(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> InitDataHists_Selection();
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  INITIALIZE HISTOGRAMS OF 2D VARIABLES
// ========================================================================================================================

void InitializeVariables2D(const CCPi0::MacroUtil& util,
                           const EnumDataMCTruth& type_DataMCTruth,
                           std::vector<Variable2D*> variables2D,
                           std::string cut_to_study)
{
    for ( auto var2D : variables2D )
    {
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> InitMCHists_ObjectPdg2D(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> InitDataHists_ObjectPdg2D();
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> InitMCHists_ObjectPdg2D(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> InitDataHists_ObjectPdg2D();
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> InitMCHists_ObjectPdg2D(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> InitDataHists_ObjectPdg2D();
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> InitMCHists_ObjectPdg2D(util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> InitDataHists_ObjectPdg2D();
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  FILL HISTOGRAMS OF VARIABLES
// ========================================================================================================================

void FillVariables(const EnumDataMCTruth& type_DataMCTruth,
                   const CCPi0Event& event,
                   std::vector<Variable*> variables,
                   std::string cut_to_study)
{
    for ( auto var : variables )
    {
        // Michel cut
        // ==========
        if ( cut_to_study == "Michel" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeMichel"      ||
                     var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else {
                    if ( var->Name() == "StartPointVertexMichelPdg" ) {
                        int Nmichels = event.m_universe->NstartPointVertexMichels();
                        
                        for ( int michel_index = 0; michel_index < Nmichels; ++michel_index ) {
                            double value = var->GetValue(*event.m_universe, michel_index);
                            var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                            
                            if ( event.m_signal_backgr_type == kSignal )
                                var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                            else {
                                var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                                if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                            }
                        }
                    }
                    
                    if ( var->Name() == "StopPointVertexMichelPdg" ) {
                        int Nmichels = event.m_universe->NstopPointVertexMichels();
                        
                        for ( int michel_index = 0; michel_index < Nmichels; ++michel_index ) {
                            double value = var->GetValue(*event.m_universe, michel_index);
                            var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                            
                            if ( event.m_signal_backgr_type == kSignal )
                                var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                            else {
                                var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                                if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                            }
                        }
                    }
                    
                    if ( var->Name() == "KinkedVertexMichelPdg" ) {
                        int Nmichels = event.m_universe->NkinkedVertexMichels();
                        
                        for ( int michel_index = 0; michel_index < Nmichels; ++michel_index ) {
                            double value = var->GetValue(*event.m_universe, michel_index);
                            var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                            
                            if ( event.m_signal_backgr_type == kSignal )
                                var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                            else {
                                var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                                if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                            }
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeMichel"      ||
                     var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" )
                    ccpi0_event::FillDataHists_Selection(event, var);
            }
        }  // End of Michel cut
        
        
        // Long track cut
        // ==============
        if ( cut_to_study == "Track" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "PrimTrackPionScore" ) {
                    int Ntracks = event.m_universe->NprimTracks();
                    
                    if ( Ntracks == 1 ) {
                        int track_contained = event.m_universe->PrimTrackIsContained();
                        int track_kinked    = event.m_universe->PrimTrackIsKinked();
                        double value        = var->GetValue(*event.m_universe);
                        
                        if ( track_contained == 1 && track_kinked == 0 ) {
                            int track_pdg = event.m_universe->PrimTrackTruePDG();
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( track_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( track_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "PrimTrackPionScore" ) {
                    int Ntracks = event.m_universe->NprimTracks();
                    
                    if ( Ntracks == 1 ) {
                        int track_contained = event.m_universe->PrimTrackIsContained();
                        int track_kinked    = event.m_universe->PrimTrackIsKinked();
                        double value        = var->GetValue(*event.m_universe);
                        
                        if ( track_contained == 1 && track_kinked == 0 )
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
            }
        }  // End of long track cut
        
        
        // AngleScan cut
        // =============
        if ( cut_to_study == "AngleScan" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeAngleScan" ||
                     var->Name() == "AngleScanNblobs" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "NblobsPassBasicQuality" ) {
                    int Nblobs_angle_scan = event.m_universe->AngleScanNblobs();
                    
                    if ( Nblobs_angle_scan > 0 )
                        ccpi0_event::FillMCHists_Selection(event, var);
                    else continue;
                }
                
                else if ( var->Name() == "NblobCandidates" ) {
                    int Nblobs_angle_scan  = event.m_universe->AngleScanNblobs();
                    if ( Nblobs_angle_scan <= 0 ) continue;
                    
                    int Nblobs_basic_quality = event.m_universe->NblobsPassBasicQuality();
                    if ( Nblobs_basic_quality <= 0 ) continue;
                    
                    int Nblobs = event.m_universe->NblobCandidates();
                    int Nblobs_good = 0;
                    
                    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                        if ( blob_index > 5 ) break;
                        
                        bool start_end_good = event.m_universe->GetBlobIsGoodStartAndEndPoints(blob_index);
                        bool bad_fit_type1  = event.m_universe->GetBlobIsBadFitType1(blob_index);
                        bool bad_fit_type2  = event.m_universe->GetBlobIsBadFitType2(blob_index);
                        bool bad_fit_type3  = event.m_universe->GetBlobIsBadFitType3(blob_index);
                        
                        if ( start_end_good && !bad_fit_type1 && !bad_fit_type2 && !bad_fit_type3 )
                        ++Nblobs_good;
                    }
                    if ( Nblobs != Nblobs_good ) continue;
                    ccpi0_event::FillMCHists_Selection(event, var);
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeAngleScan" ||
                     var->Name() == "AngleScanNblobs" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "NblobsPassBasicQuality" ) {
                    int Nblobs_angle_scan = event.m_universe->AngleScanNblobs();
                    
                    if ( Nblobs_angle_scan > 0 ) ccpi0_event::FillDataHists_Selection(event, var);
                    else continue;
                }
                
                else if ( var->Name() == "NblobCandidates" ) {
                    int Nblobs_angle_scan    = event.m_universe->AngleScanNblobs();
                    int Nblobs_basic_quality = event.m_universe->NblobsPassBasicQuality();
                    
                    if ( Nblobs_angle_scan > 0 && Nblobs_basic_quality > 0 )
                        ccpi0_event::FillDataHists_Selection(event, var);
                    else continue;
                }
            }
        }  // End of AngleScan cut
        
        
        // Blob angle w.r.t. muon cut
        // ==========================
        if ( cut_to_study == "BlobAngleWRTMuon" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobAngleWRTMuon_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobAngleWRTMuon_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob angle w.r.t. muon cut
        
        
        // Blob deviation cut
        // ==================
        if ( cut_to_study == "BlobDeviation" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobProjDeviation_Single" || var->Name() == "BlobAngleDeviation_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobProjDeviation_Multi" || var->Name() == "BlobAngleDeviation_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobProjDeviation_Single" || var->Name() == "BlobAngleDeviation_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobProjDeviation_Multi" || var->Name() == "BlobAngleDeviation_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob deviation cut
        
        
        // Blob energy VS dx cuts
        // ======================
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"    || var->Name() == "Blobdx_Single_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1" || var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxEnd_Single_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part1"    || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" || var->Name() == "BlobdEdxEnd_Multi_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"    || var->Name() == "Blobdx_Single_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1" || var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxEnd_Single_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part1"    || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" || var->Name() == "BlobdEdxEnd_Multi_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob energy VS dx cuts
        
        
        // Blob end dE/dx cuts
        // ===================
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"    || var->Name() == "Blobdx_Single_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2" || var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxEnd_Single_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part2"    || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" || var->Name() == "BlobdEdxEnd_Multi_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"    || var->Name() == "Blobdx_Single_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2" || var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxEnd_Single_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part2"    || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" || var->Name() == "BlobdEdxEnd_Multi_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob end dE/dx cuts
        
        
        // After blob energy (not an actual cut)
        // =====================================
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"    || var->Name() == "Blobdx_Single_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3" || var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxEnd_Single_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg = event.m_universe->GetBlobTruePDG(1);
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                        else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part3"    || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" || var->Name() == "BlobdEdxEnd_Multi_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_ObjectPdg.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( blob_pdg == 1 )      var->m_hists.m_mc_ObjectPdg_Pi0.FillUniverse(    *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 2 ) var->m_hists.m_mc_ObjectPdg_Proton.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 3 ) var->m_hists.m_mc_ObjectPdg_Neutron.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 4 ) var->m_hists.m_mc_ObjectPdg_Pion.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 5 ) var->m_hists.m_mc_ObjectPdg_EM.FillUniverse(     *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 6 ) var->m_hists.m_mc_ObjectPdg_Muon.FillUniverse(   *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 7 ) var->m_hists.m_mc_ObjectPdg_OthPdg.FillUniverse( *event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 8 ) var->m_hists.m_mc_ObjectPdg_MCXtalk.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( blob_pdg == 9 ) var->m_hists.m_mc_ObjectPdg_Overlay.FillUniverse(*event.m_universe, value, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"    || var->Name() == "Blobdx_Single_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3" || var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxEnd_Single_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobEcalo_Multi_Part3"    || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Multi_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" || var->Name() == "BlobdEdxEnd_Multi_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_ObjectPdg->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of after blob energy
        
        
        // Blob Michel cut
        // ===============
        if ( cut_to_study == "BlobMichel" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobMichel" ||
                     var->Name() == "Nblobs_BeforeBlobMichel" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobEndPointMichel_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        
                        var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( event.m_signal_backgr_type == kSignal )
                            var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                        else {
                            var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                        }
                    }
                }
                
                else if ( var->Name() == "BlobStartPointMichel_Multi" || var->Name() == "BlobEndPointMichel_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            
                            var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( event.m_signal_backgr_type == kSignal )
                                var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                            else {
                                var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                                if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                            }
                        }
                    }
                }
                
                else if ( var->Name() == "BlobPdg_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int value = event.m_universe->GetBlobTruePDG(1);
                        
                        var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                        if ( event.m_signal_backgr_type == kSignal )
                            var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                        else {
                            var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                            else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                        }
                    }
                }
                
                else if ( var->Name() == "BlobPdg_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int value = event.m_universe->GetBlobTruePDG(blob_index);
                            
                            var->m_hists.m_mc_Selection.FillUniverse(*event.m_universe, value, event.m_weight);
                            if ( event.m_signal_backgr_type == kSignal )
                                var->m_hists.m_mc_Selection_Signal.FillUniverse(*event.m_universe, value, event.m_weight);
                            else {
                                var->m_hists.m_mc_Selection_Backgr.FillUniverse(*event.m_universe, value, event.m_weight);
                                if ( event.m_signal_backgr_type == kBackgrPi0HighW )      var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrQElike )   var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPionProd ) var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasUp )   var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(  *event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasBetw ) var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrPlasDown ) var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*event.m_universe, value, event.m_weight);
                                else if ( event.m_signal_backgr_type == kBackgrOther )    var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(   *event.m_universe, value, event.m_weight);
                            }
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobMichel" ||
                     var->Name() == "Nblobs_BeforeBlobMichel" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobEndPointMichel_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double value = var->GetValue(*event.m_universe, 1);
                        var->m_hists.m_data_Selection->Fill(value, event.m_weight);
                    }
                }
                
                else if ( var->Name() == "BlobStartPointMichel_Multi" || var->Name() == "BlobEndPointMichel_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double value = var->GetValue(*event.m_universe, blob_index);
                            var->m_hists.m_data_Selection->Fill(value, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob Michel cut
        
        
        // Recoil energy cut
        // =================
        if ( cut_to_study == "Recoil" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC ) 
            {
                if ( var->Name() == "MuonPt_BeforeEnergy" )
                    ccpi0_event::FillMCHists_Selection(event, var);
                
                else if ( var->Name() == "NoPi0RecoilE_Single" || var->Name() == "RecoilE_Single" ||
                          var->Name() == "Q2_Single"           || var->Name() == "W2_Single"      || var->Name() == "W_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    if ( Nblobs == 1 ) ccpi0_event::FillMCHists_Selection(event, var);
                }
                
                else if ( var->Name() == "NoPi0RecoilE_Multi" || var->Name() == "RecoilE_Multi" ||
                          var->Name() == "Q2_Multi"           || var->Name() == "W2_Multi"      || var->Name() == "W_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    if ( Nblobs > 1 ) ccpi0_event::FillMCHists_Selection(event, var);
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeEnergy" )
                    ccpi0_event::FillDataHists_Selection(event, var);
                
                else if ( var->Name() == "NoPi0RecoilE_Single" || var->Name() == "RecoilE_Single" ||
                          var->Name() == "Q2_Single"           || var->Name() == "W2_Single"      || var->Name() == "W_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    if ( Nblobs == 1 ) ccpi0_event::FillDataHists_Selection(event, var);
                }
                
                else if ( var->Name() == "NoPi0RecoilE_Multi" || var->Name() == "RecoilE_Multi" ||
                          var->Name() == "Q2_Multi"           || var->Name() == "W2_Multi"      || var->Name() == "W_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    if ( Nblobs > 1 ) ccpi0_event::FillDataHists_Selection(event, var);
                }
            }
        }  // End of recoil cut
        
        
        // All cuts
        // ========
        if ( cut_to_study == "AllCuts" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC ) 
            {
                if ( var->Name() == "MuonPt" )
                    ccpi0_event::FillMCHists_Selection(event, var);
            }
            
            // Data
            if ( type_DataMCTruth == kData ) 
            {
                if ( var->Name() == "MuonPt" )
                    ccpi0_event::FillDataHists_Selection(event, var);
            }
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  FILL HISTOGRAMS OF 2D VARIABLES
// ========================================================================================================================

void FillVariables2D(const EnumDataMCTruth& type_DataMCTruth,
                     const CCPi0Event& event,
                     std::vector<Variable2D*> variables2D,
                     std::string cut_to_study)
{
    for ( auto var2D : variables2D )
    {
        // Blob deviation cut
        // ==================
        if ( cut_to_study == "BlobDeviation" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg  = event.m_universe->GetBlobTruePDG(1);
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        
                        var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg  = event.m_universe->GetBlobTruePDG(blob_index);
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            
                            var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Single" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobAngleDeviationVSDeviation_Multi" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob deviation cut
        
        
        // Blob energy VS dx cuts
        // ======================
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part1"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part1" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part1" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg  = event.m_universe->GetBlobTruePDG(1);
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        
                        var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part1"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part1" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part1" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg  = event.m_universe->GetBlobTruePDG(blob_index);
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            
                            var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part1"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part1" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part1" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part1"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part1" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part1" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part1" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob energy VS dx cuts
        
        
        // Blob end dE/dx cuts
        // ===================
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part2"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part2" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part2" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg  = event.m_universe->GetBlobTruePDG(1);
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        
                        var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part2"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part2" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part2" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg  = event.m_universe->GetBlobTruePDG(blob_index);
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            
                            var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part2"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part2" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part2" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part2"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part2" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part2" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part2" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
        }  // End of blob end dE/dx cuts
        
        
        // After blob energy (not an actual cut)
        // =====================================
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            // Monte Carlo
            if ( type_DataMCTruth == kMC )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part3"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part3" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part3" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        int blob_pdg  = event.m_universe->GetBlobTruePDG(1);
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        
                        var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part3"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part3" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part3" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg  = event.m_universe->GetBlobTruePDG(blob_index);
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            
                            var2D->m_hists2D.m_mc_ObjectPdg2D.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            if ( blob_pdg == 1 )      var2D->m_hists2D.m_mc_ObjectPdg2D_Pi0.FillUniverse(    *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 2 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Proton.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 3 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Neutron.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 4 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Pion.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 5 ) var2D->m_hists2D.m_mc_ObjectPdg2D_EM.FillUniverse(     *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 6 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Muon.FillUniverse(   *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 7 ) var2D->m_hists2D.m_mc_ObjectPdg2D_OthPdg.FillUniverse( *event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 8 ) var2D->m_hists2D.m_mc_ObjectPdg2D_MCXtalk.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                            else if ( blob_pdg == 9 ) var2D->m_hists2D.m_mc_ObjectPdg2D_Overlay.FillUniverse(*event.m_universe, valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
            
            // Data
            if ( type_DataMCTruth == kData )
            {
                if ( var2D->Name() == "BlobEcaloVSdx_Single_Part3"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Single_Part3" ||
                     var2D->Name() == "BlobdEdxFrontVSEcalo_Single_Part3" || var2D->Name() == "BlobdEdxEndVSEcalo_Single_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs == 1 ) {
                        double valueX = var2D->GetValueX(*event.m_universe, 1);
                        double valueY = var2D->GetValueY(*event.m_universe, 1);
                        var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                    }
                }
                
                else if ( var2D->Name() == "BlobEcaloVSdx_Multi_Part3"        || var2D->Name() == "BlobdEdxMeanVSEcalo_Multi_Part3" ||
                          var2D->Name() == "BlobdEdxFrontVSEcalo_Multi_Part3" || var2D->Name() == "BlobdEdxEndVSEcalo_Multi_Part3" ) {
                    int Nblobs = event.m_universe->NblobCandidates();
                    
                    if ( Nblobs > 1 ) {
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            double valueX = var2D->GetValueX(*event.m_universe, blob_index);
                            double valueY = var2D->GetValueY(*event.m_universe, blob_index);
                            var2D->m_hists2D.m_data_ObjectPdg2D->Fill(valueX, valueY, event.m_weight);
                        }
                    }
                }
            }
        }  // End of after blob energy
        
    }  // End of loop over variables
}





// ========================================================================================================================
//  SYNC AND WRITE HISTOGRAMS OF VARIABLES
// ========================================================================================================================

void SyncAndWriteVariables(TFile& fout,
                           const EnumDataMCTruth& type_DataMCTruth,
                           std::vector<Variable*> variables,
                           std::string cut_to_study)
{
    for ( auto var : variables )
    {
        // Michel cut
        if ( cut_to_study == "Michel" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeMichel"      ||
                     var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" )
                    var -> WriteDataHists_Selection(fout);
            }
        }
        
        
        // Long track cut
        if ( cut_to_study == "Track" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "PrimTrackPionScore" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    var -> WriteDataHists_Selection(fout);
                
                else if ( var->Name() == "PrimTrackPionScore" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // AngleScan cut
        if ( cut_to_study == "AngleScan" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var -> WriteDataHists_Selection(fout);
        }
        
        
        // Blob angle w.r.t. muon cut
        if ( cut_to_study == "BlobAngleWRTMuon" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    var -> WriteDataHists_Selection(fout);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    var -> WriteDataHists_Selection(fout);
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    var -> WriteDataHists_Selection(fout);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" ) {
                    var -> WriteDataHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" ) {
                    var -> SyncMCHists_Selection();
                    var -> WriteMCHists_Selection(fout);
                }
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" ) {
                    var -> SyncMCHists_ObjectPdg();
                    var -> WriteMCHists_ObjectPdg(fout);
                }
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    var -> WriteDataHists_Selection(fout);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                    var -> WriteDataHists_ObjectPdg(fout);
            }
        }
        
        
        // Blob Michel cut
        if ( cut_to_study == "BlobMichel" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobMichel"     ||
                     var->Name() == "Nblobs_BeforeBlobMichel"     ||
                     var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobStartPointMichel_Multi" ||
                     var->Name() == "BlobEndPointMichel_Single"   || var->Name() == "BlobEndPointMichel_Multi" )
                    var -> WriteDataHists_Selection(fout);
            }
        }
        
        
        // Recoil energy cut
        if ( cut_to_study == "Recoil" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var -> WriteDataHists_Selection(fout);
        }
        
        
        // All cuts
        if ( cut_to_study == "AllCuts" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var -> SyncMCHists_Selection();
                var -> WriteMCHists_Selection(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var -> WriteDataHists_Selection(fout);
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  SYNC AND WRITE HISTOGRAMS OF 2D VARIABLES
// ========================================================================================================================

void SyncAndWriteVariables2D(TFile& fout,
                             const EnumDataMCTruth& type_DataMCTruth,
                             std::vector<Variable2D*> variables2D,
                             std::string cut_to_study)
{
    for ( auto var2D : variables2D )
    {
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var2D -> SyncMCHists_ObjectPdg2D();
                var2D -> WriteMCHists_ObjectPdg2D(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var2D -> WriteDataHists_ObjectPdg2D(fout);
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var2D -> SyncMCHists_ObjectPdg2D();
                var2D -> WriteMCHists_ObjectPdg2D(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var2D -> WriteDataHists_ObjectPdg2D(fout);
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var2D -> SyncMCHists_ObjectPdg2D();
                var2D -> WriteMCHists_ObjectPdg2D(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var2D -> WriteDataHists_ObjectPdg2D(fout);
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
            {
                var2D -> SyncMCHists_ObjectPdg2D();
                var2D -> WriteMCHists_ObjectPdg2D(fout);
            }
            
            if ( type_DataMCTruth == kData )
                var2D -> WriteDataHists_ObjectPdg2D(fout);
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  LOAD HISTOGRAMS OF VARIABLES
// ========================================================================================================================

void LoadVariables(CCPi0::MacroUtil util,
                   TFile& fin,
                   const EnumDataMCTruth& type_DataMCTruth,
                   std::vector<Variable*> variables,
                   std::string cut_to_study)
{
    for ( auto var : variables )
    {
        // Michel cut
        if ( cut_to_study == "Michel" )
        {
            if ( type_DataMCTruth == kMC )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeMichel"      ||
                     var->Name() == "NstartPointVertexMichels" || var->Name() == "NstopPointVertexMichels" || var->Name() == "NkinkedVertexMichels" )
                    var -> LoadDataHists_Selection(fin);
            }
        }
        
        
        // Long track cut
        if ( cut_to_study == "Track" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "PrimTrackPionScore" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeTrack" ||
                     var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "PrimTrackPionScore" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // AngleScan cut
        if ( cut_to_study == "AngleScan" )
        {
            if ( type_DataMCTruth == kMC )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> LoadDataHists_Selection(fin);
        }
        
        
        // Blob angle w.r.t. muon cut
        if ( cut_to_study == "BlobAngleWRTMuon" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                     var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                     var->Name() == "Nblobs_BeforeBlobDeviation" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                          var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                     var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                          var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                          var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                          var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                          var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                     var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                          var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                          var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                          var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                          var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    var -> LoadMCHists_Selection(fin, util.m_error_bands);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                    var -> LoadMCHists_ObjectPdg(fin, util.m_error_bands);
            }
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                     var->Name() == "Nblobs_AfterBlobEnergy" )
                    var -> LoadDataHists_Selection(fin);
                
                else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                          var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                          var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                          var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                          var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                    var -> LoadDataHists_ObjectPdg(fin);
            }
        }
        
        
        // Blob Michel cut
        if ( cut_to_study == "BlobMichel" )
        {
            if ( type_DataMCTruth == kMC )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
            {
                if ( var->Name() == "MuonPt_BeforeBlobMichel"     ||
                     var->Name() == "Nblobs_BeforeBlobMichel"     ||
                     var->Name() == "BlobStartPointMichel_Single" || var->Name() == "BlobStartPointMichel_Multi" ||
                     var->Name() == "BlobEndPointMichel_Single"   || var->Name() == "BlobEndPointMichel_Multi" )
                    var -> LoadDataHists_Selection(fin);
            }
        }
        
        
        // Recoil energy cut
        if ( cut_to_study == "Recoil" )
        {
            if ( type_DataMCTruth == kMC )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> LoadDataHists_Selection(fin);
        }
        
        
        // All cuts
        if ( cut_to_study == "AllCuts" )
        {
            if ( type_DataMCTruth == kMC )
                var -> LoadMCHists_Selection(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var -> LoadDataHists_Selection(fin);
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  LOAD HISTOGRAMS OF 2D VARIABLES
// ========================================================================================================================

void LoadVariables2D(CCPi0::MacroUtil util,
                     TFile& fin,
                     const EnumDataMCTruth& type_DataMCTruth,
                     std::vector<Variable2D*> variables2D,
                     std::string cut_to_study)
{
    for ( auto var2D : variables2D )
    {
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> LoadMCHists_ObjectPdg2D(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> LoadDataHists_ObjectPdg2D(fin);
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> LoadMCHists_ObjectPdg2D(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> LoadDataHists_ObjectPdg2D(fin);
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> LoadMCHists_ObjectPdg2D(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> LoadDataHists_ObjectPdg2D(fin);
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" )
        {
            if ( type_DataMCTruth == kMC )
                var2D -> LoadMCHists_ObjectPdg2D(fin, util.m_error_bands);
            
            if ( type_DataMCTruth == kData )
                var2D -> LoadDataHists_ObjectPdg2D(fin);
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  SCALE MC HISTOGRAMS OF VARIABLES
// ========================================================================================================================

void ScaleMCVariables(std::vector<Variable*> variables,
                      double mc_pot,
                      double data_pot,
                      std::string cut_to_study)
{
    for ( auto var : variables )
    {
        // Michel cut
        if ( cut_to_study == "Michel" ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        }
        
        
        // Long track cut
        if ( cut_to_study == "Track" ) {
            if ( var->Name() == "MuonPt_BeforeTrack" ||
                 var->Name() == "NprimTracks"        || var->Name() == "NsecTracks" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "PrimTrackPionScore" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // AngleScan cut
        if ( cut_to_study == "AngleScan" ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        }
        
        
        // Blob angle w.r.t. muon cut
        if ( cut_to_study == "BlobAngleWRTMuon" ) {
            if ( var->Name() == "MuonPt_BeforeBlobAngleWRTMuon" ||
                 var->Name() == "Nblobs_BeforeBlobAngleWRTMuon" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "BlobAngleWRTMuon_Single" || var->Name() == "BlobAngleWRTMuon_Multi" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" ) {
            if ( var->Name() == "MuonPt_BeforeBlobDeviation" ||
                 var->Name() == "Nblobs_BeforeBlobDeviation" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "BlobProjDeviation_Single"  || var->Name() == "BlobProjDeviation_Multi" ||
                      var->Name() == "BlobAngleDeviation_Single" || var->Name() == "BlobAngleDeviation_Multi" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" ) {
            if ( var->Name() == "MuonPt_BeforeBlobEnergyVSdx" ||
                 var->Name() == "Nblobs_BeforeBlobEnergyVSdx" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "BlobEcalo_Single_Part1"     || var->Name() == "BlobEcalo_Multi_Part1"     ||
                      var->Name() == "Blobdx_Single_Part1"        || var->Name() == "Blobdx_Multi_Part1"        ||
                      var->Name() == "BlobdEdxMean_Single_Part1"  || var->Name() == "BlobdEdxMean_Multi_Part1"  ||
                      var->Name() == "BlobdEdxFront_Single_Part1" || var->Name() == "BlobdEdxFront_Multi_Part1" ||
                      var->Name() == "BlobdEdxEnd_Single_Part1"   || var->Name() == "BlobdEdxEnd_Multi_Part1" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" ) {
            if ( var->Name() == "MuonPt_BeforeBlobdEdxEnd" ||
                 var->Name() == "Nblobs_BeforeBlobdEdxEnd" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "BlobEcalo_Single_Part2"     || var->Name() == "BlobEcalo_Multi_Part2"     ||
                      var->Name() == "Blobdx_Single_Part2"        || var->Name() == "Blobdx_Multi_Part2"        ||
                      var->Name() == "BlobdEdxMean_Single_Part2"  || var->Name() == "BlobdEdxMean_Multi_Part2"  ||
                      var->Name() == "BlobdEdxFront_Single_Part2" || var->Name() == "BlobdEdxFront_Multi_Part2" ||
                      var->Name() == "BlobdEdxEnd_Single_Part2"   || var->Name() == "BlobdEdxEnd_Multi_Part2" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" ) {
            if ( var->Name() == "MuonPt_AfterBlobEnergy" ||
                 var->Name() == "Nblobs_AfterBlobEnergy" )
                var -> ScaleMCHists_Selection(mc_pot, data_pot);
            
            else if ( var->Name() == "BlobEcalo_Single_Part3"     || var->Name() == "BlobEcalo_Multi_Part3"     ||
                      var->Name() == "Blobdx_Single_Part3"        || var->Name() == "Blobdx_Multi_Part3"        ||
                      var->Name() == "BlobdEdxMean_Single_Part3"  || var->Name() == "BlobdEdxMean_Multi_Part3"  ||
                      var->Name() == "BlobdEdxFront_Single_Part3" || var->Name() == "BlobdEdxFront_Multi_Part3" ||
                      var->Name() == "BlobdEdxEnd_Single_Part3"   || var->Name() == "BlobdEdxEnd_Multi_Part3" )
                var -> ScaleMCHists_ObjectPdg(mc_pot, data_pot);
        }
        
        
        // Blob Michel cut
        if ( cut_to_study == "BlobMichel" ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        }
        
        
        // Recoil energy cut
        if ( cut_to_study == "Recoil" ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        }
        
        
        // All cuts
        if ( cut_to_study == "AllCuts" ) {
            var -> ScaleMCHists_Selection(mc_pot, data_pot);
        }
    }  // End of loop over variables
}





// ========================================================================================================================
//  SCALE MC HISTOGRAMS OF 2D VARIABLES
// ========================================================================================================================

void ScaleMCVariables2D(std::vector<Variable2D*> variables2D,
                        double mc_pot,
                        double data_pot,
                        std::string cut_to_study)
{
    for ( auto var2D : variables2D )
    {
        // Blob deviation cut
        if ( cut_to_study == "BlobDeviation" ) {
            var2D -> ScaleMCHists_ObjectPdg2D(mc_pot, data_pot);
        }
        
        
        // Blob energy VS dx cuts
        if ( cut_to_study == "BlobEnergyVSdx" ) {
            var2D -> ScaleMCHists_ObjectPdg2D(mc_pot, data_pot);
        }
        
        
        // Blob end dE/dx cuts
        if ( cut_to_study == "BlobdEdxEnd" ) {
            var2D -> ScaleMCHists_ObjectPdg2D(mc_pot, data_pot);
        }
        
        
        // After blob energy (not an actual cut)
        if ( cut_to_study == "AfterBlobEnergy" ) {
            var2D -> ScaleMCHists_ObjectPdg2D(mc_pot, data_pot);
        }
    }  // End of loop over variables
}


#endif  // BookHistograms_h