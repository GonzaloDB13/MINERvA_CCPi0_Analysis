#ifndef ArachneEvents_C
#define ArachneEvents_C

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
//  LOOP OVER ERROR BANDS AND FILL HISTOGRAMS
// ========================================================================================================================

void LoopAndFillHistograms(const CCPi0::MacroUtil& util,
                           const EnumDataMCTruth& type_DataMCTruth,
                           std::ofstream& fout,
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
        
        bool vert_universe_checked_cuts = false;
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
                
                
                // Define two CCPi0 event
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                
                
                // Check reconstruction cuts
                // =========================
                
                // Check if universe of this error band is vertical-only.
                // If it is vertical-only, cuts would need to be checked only once
                if ( universe->IsVerticalOnly() )
                {
                    if ( !vert_universe_checked_cuts ) {
                        // vert_universe_passes_cuts = IsSigReg(event);
                        vert_universe_passes_cuts = PassesCuts(event, cuts_until_AngleScan);
                        vert_universe_checked_cuts = true;
                    }
                    
                    if ( vert_universe_checked_cuts ) {
                        event.m_is_SigReg = vert_universe_passes_cuts;
                    }
                }
                
                
                // If universe is not vertical-only, check cuts on both events universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else {
                    // vert_universe_passes_cuts = IsSigReg(event);
                    vert_universe_passes_cuts = PassesCuts(event, cuts_until_AngleScan);
                }
                
                
                // Print Arachne links
                // ===================
                
                // Check if event is in physics signal region
                if ( vert_universe_passes_cuts )
                {
                    // Fill MC (signal only)
                    if ( type_DataMCTruth == kMC && event.m_is_signal )
                    {
                        // Check if at least one shower is true photon from pi0
                        int Nblobs = event.m_universe->NblobCandidates();
                        bool pi0_blob = false;
                        
                        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
                            int blob_pdg = event.m_universe->GetBlobTruePDG(blob_index);
                            if ( blob_pdg == 1 ) {
                                pi0_blob = true;
                                break;
                            }
                        }
                        
                        double recoil_true = event.m_universe->GetRecoilE_True();
                        double recoil_reco = event.m_universe->GetRecoilE();
                        if ( !(recoil_true > 1.5) ) break;
                        
                        // Print Arachne link
                        if ( pi0_blob ) {
                            const char* std_header   = "http://minerva05.fnal.gov/Arachne/arachne.html";
                            const char* detector     = "SIM_minerva";
                            const char* reco_version = "v21r1p1";
                            int run    = event.m_universe->GetInt("mc_run");
                            int subrun = event.m_universe->GetInt("mc_subrun");
                            int gate   = event.m_universe->GetInt("mc_nthEvtInFile") + 1;
                            std::vector<int> slices = event.m_universe->GetVecInt("slice_numbers");
                            
                            for ( unsigned int i = 0; i < slices.size(); ++i  ) {
                                std::string arachne_link = Form("%s?det=%s&recoVer=%s&run=%i&subrun=%i&gate=%i&slice=%i",
                                                                std_header, detector, reco_version, run, subrun, gate, slices[i]);
                                fout << arachne_link << std::endl;
                            }
                        }
                    }  // End of fill MC
                    
                    
                    // Fill data
                    if ( type_DataMCTruth == kData )
                    {
                        // int NprimTracks = event.m_universe->NprimTracks();
                        // int NsecTracks  = event.m_universe->NsecTracks();
                        // if ( !(NprimTracks > 1 && NsecTracks > 1) ) break;
                        
                        // int Nblobs = event.m_universe->NblobCandidates();
                        // if ( Nblobs <= 1 ) break;
                        
                        int Nblobs = event.m_universe->NblobCandidates();
                        double no_pi0_recoil = event.m_universe->GetNoPi0RecoilE();
                        if ( !(Nblobs > 1 && no_pi0_recoil > 1.0) ) break;
                        
                        // Print Arachne link
                        const char* std_header   = "http://minerva05.fnal.gov/Arachne/arachne.html";
                        const char* detector     = "MV";
                        const char* reco_version = "v21r1p1";
                        int run    = event.m_universe->GetInt("ev_run");
                        int subrun = event.m_universe->GetInt("ev_subrun");
                        int gate   = event.m_universe->GetInt("ev_gate");
                        std::vector<int> slices = event.m_universe->GetVecInt("slice_numbers");
                        
                        for ( unsigned int i = 0; i < slices.size(); ++i  ) {
                            std::string arachne_link = Form("%s?det=%s&recoVer=%s&run=%i&subrun=%i&gate=%i&slice=%i",
                                                            std_header, detector, reco_version, run, subrun, gate, slices[i]);
                            fout << arachne_link << std::endl;
                        }
                    }  // End of fill data
                    
                }  // End of check if event is physics signal region
                
            }  // End of loop over universes
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
    
#endif  // __CINT__
}





// ========================================================================================================================
//  PROCESS MC TUPLES
// ========================================================================================================================

void ProcessMCTuples(const CCPi0::MacroUtil& util,
                     std::ofstream& fout,
                     const EnumModels& type_model,
                     std::string option_material)
{
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, fout, type_model, option_material);
}





// ========================================================================================================================
//  PROCESS DATA TUPLES
// ========================================================================================================================

void ProcessDataTuples(const CCPi0::MacroUtil& util,
                       std::ofstream& fout,
                       std::string option_material)
{
    LoopAndFillHistograms(util, kData, fout, kDataNoModel, option_material);
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

// =============
//  Monte Carlo
// =============
void SetMacroUtilMC(std::ofstream& fout_lead,
                    std::ofstream& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model)
{
    // Set MacroUtil
    // (Set 'Truth' option as TRUE and 'Systematics' option as FALSE)
    CCPi0::MacroUtil util(file_list, plist_string, false, false, type_model);
    util.PrintMacroConfiguration("ArachneEvents");
    
    
    // Process iron
    ProcessMCTuples(util, fout_lead, type_model, "lead");
    
    
    // Process iron
    ProcessMCTuples(util, fout_iron, type_model, "iron");
}



// ======
//  Data
// ======
void SetMacroUtilData(std::ofstream& fout_lead,
                      std::ofstream& fout_iron,
                      std::string plist_string,
                      std::string file_list)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string);
    util.PrintMacroConfiguration("ArachneEvents");
    
    
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

void ArachneEvents(bool is_mc,
                   std::string plist_string,
                   std::string option_model = "data",
                   std::string full_or_test = "test")
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Stop program if running over data and MC-only options are on
    assert(!(!is_mc && type_model != kDataNoModel) && " When running data, 'option_model' must be set as 'data'!!! ");
    assert(!(is_mc  && type_model == kDataNoModel) && " When running MC, 'option_model' must not be set as 'data'!!! ");
    
    
    // File list:
    // -> For grid jobs, uses input parameter
    // -> Interactively, uses 'test' sample as default (to change this, add a 3rd argument = "full")
    const std::string file_list = GetPlaylistFile(is_mc, plist_string, full_or_test);
    
    
    
    // =========================================
    //  Output file
    // =========================================
    
    // General option for output file names
    const std::string header = "/minerva/data/users/gonzalo/MAT/SupportStudies/ArachneEvents";
    
    
    // Construct output file name on LEAD
    std::string fout_name_lead = is_mc ?
        Form("%s/lead/%s/MCSignal_ArachneEvents_%s", header.c_str(),
                                                     plist_string.c_str(),
                                                     full_or_test.c_str()) :
        Form("%s/lead/%s/Data_ArachneEvents_%s", header.c_str(),
                                                 plist_string.c_str(),
                                                 full_or_test.c_str());
    
    
    // Construct output file name on IRON
    std::string fout_name_iron = is_mc ?
        Form("%s/iron/%s/MCSignal_ArachneEvents_%s", header.c_str(),
                                                     plist_string.c_str(),
                                                     full_or_test.c_str()) :
        Form("%s/iron/%s/Data_ArachneEvents_%s", header.c_str(),
                                                 plist_string.c_str(),
                                                 full_or_test.c_str());
    
    
    // Define output files
    std::ofstream fout_lead(Form("%s.txt", fout_name_lead.c_str()));
    std::ofstream fout_iron(Form("%s.txt", fout_name_iron.c_str()));
    
    
    
    // =========================================
    //  Set MacroUtil and fill histograms
    // =========================================
    
    // Monte Carlo
    if ( is_mc )
        SetMacroUtilMC(fout_lead, fout_iron, plist_string, file_list, type_model);
    
    // Data
    else
        SetMacroUtilData(fout_lead, fout_iron, plist_string, file_list);
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // ArachneEvents_C