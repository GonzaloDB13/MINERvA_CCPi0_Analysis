#ifndef CCPi0MacroUtil_cxx
#define CCPi0MacroUtil_cxx

#include "MacroUtil.h"
#include "Systematics.h"  // GetSystematicUniversesMap()
#include "myPlotStyle.h"  // Load myPlotStyle() in Initialize()



// ==============================================================================
//  CONSTRUCTOR -- ONLY DATA
// ==============================================================================

CCPi0::MacroUtil::MacroUtil(const std::string& data_file_list,
                            const std::string& plist_name)
    : PlotUtils::MacroUtil("CCPi0AnaTool", data_file_list, plist_name),
      m_do_data(true),
      m_do_mc(false),
      m_do_truth(false),
      m_do_systematics(false),
      m_model_type(kDataNoModel)
{
    Initialize();
}





// ==============================================================================
//  CONSTRUCTOR -- ONLY MC (AND TRUTH)
// ==============================================================================

CCPi0::MacroUtil::MacroUtil(const std::string& mc_file_list,
                            const std::string& plist_name,
                            const bool do_truth,
                            const bool do_systematics,
                            const EnumModels& type_model)
    : PlotUtils::MacroUtil("CCPi0AnaTool", mc_file_list, plist_name, do_truth),
      m_do_data(false),
      m_do_mc(true),
      m_do_truth(do_truth),
      m_do_systematics(do_systematics),
      m_model_type(type_model)
{
    Initialize();
}





// ==============================================================================
//  CONSTRUCTOR -- DATA AND MC (AND TRUTH)
// ==============================================================================

CCPi0::MacroUtil::MacroUtil(const std::string& mc_file_list,
                            const std::string& data_file_list,
                            const std::string& plist_name,
                            const bool do_truth,
                            const bool do_systematics,
                            const EnumModels& type_model)
    : PlotUtils::MacroUtil("CCPi0AnaTool", mc_file_list, data_file_list, plist_name, do_truth),
      m_do_data(true),
      m_do_mc(true),
      m_do_truth(do_truth),
      m_do_systematics(do_systematics),
      m_model_type(type_model)
{
    Initialize();
}





// ==============================================================================
//  INITIALIZATION
// ==============================================================================

// Initialize MacroUtil
// ====================

void CCPi0::MacroUtil::Initialize()
{
    myPlotStyle();
    
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    
    InitializeSystematics();
}



// Initialize systematics
// ======================

void CCPi0::MacroUtil::InitializeSystematics()
{
    using namespace PlotUtils;
    
    
    // Set playlist
    if      ( m_plist_string == "minervame1A" ) MinervaUniverse::SetPlaylist("minervame1a");
    else if ( m_plist_string == "minervame1B" ) MinervaUniverse::SetPlaylist("minervame1b");
    else if ( m_plist_string == "minervame1C" ) MinervaUniverse::SetPlaylist("minervame1c");
    else if ( m_plist_string == "minervame1D" ) MinervaUniverse::SetPlaylist("minervame1d");
    else if ( m_plist_string == "minervame1E" ) MinervaUniverse::SetPlaylist("minervame1e");
    else if ( m_plist_string == "minervame1F" ) MinervaUniverse::SetPlaylist("minervame1f");
    else if ( m_plist_string == "minervame1G" ) MinervaUniverse::SetPlaylist("minervame1g");
    else if ( m_plist_string == "minervame1L" ) MinervaUniverse::SetPlaylist("minervame1l");
    else if ( m_plist_string == "minervame1M" ) MinervaUniverse::SetPlaylist("minervame1m");
    else if ( m_plist_string == "minervame1N" ) MinervaUniverse::SetPlaylist("minervame1n");
    else if ( m_plist_string == "minervame1O" ) MinervaUniverse::SetPlaylist("minervame1o");
    else if ( m_plist_string == "minervame1P" ) MinervaUniverse::SetPlaylist("minervame1p");
    
    
    // Set error bands
    m_data_universe     = new CVUniverse(m_data);
    m_error_bands       = systematics::GetSystematicUniversesMap(m_mc,    m_model_type, false, m_do_systematics);
    m_error_bands_truth = systematics::GetSystematicUniversesMap(m_truth, m_model_type, true,  m_do_systematics);
    
    
    // Set basic analysis parameters
    MinervaUniverse::SetAnalysisNuPDG(CCPi0AnaConstants::kAnalysisNuPDG);
    MinervaUniverse::SetNFluxUniverses(CCPi0AnaConstants::kNFluxUniverses);
    MinervaUniverse::SetNuEConstraint(CCPi0AnaConstants::kUseNueConstraint);
    
    
    // Use non-resonant pion reweight?
    bool use_non_res_pi = true;
    if ( m_model_type == kGENIE || m_model_type == kMnvGENIEv1_noNonResPi ||
         m_model_type == kMnvGENIEv1_noPionTune || m_model_type == kDataNoModel ) use_non_res_pi = false;
    MinervaUniverse::SetNonResPiReweight(use_non_res_pi);
    
    
    // Use deuterium pion tune? (ANL, BNL re-analyses)
    bool use_d2_pi_tune = true;
    if ( m_model_type == kGENIE || m_model_type == kMnvGENIEv1_noD2 ||
         m_model_type == kMnvGENIEv1_noPionTune || m_model_type == kDataNoModel ) use_d2_pi_tune = false;
    MinervaUniverse::SetDeuteriumGeniePiTune(use_d2_pi_tune);
    
    
    // Use CCQE axial mass tune with Z-expansion? (NOTE: Don't use this never)
    bool use_z_exp_ma = false;
    MinervaUniverse::SetZExpansionFaReweight(use_z_exp_ma);
    
    
    // MINERvA hadron reweighter
    MinervaUniverse::SetReadoutVolume(CCPi0AnaConstants::kMHRReadoutVolume);
    MinervaUniverse::SetMHRWeightNeutronCVReweight(CCPi0AnaConstants::kUseMHRWeightNeutronCVReweight);
    MinervaUniverse::SetMHRWeightElastics(CCPi0AnaConstants::kUseMHRWeightElastics);
}





// ==============================================================================
//  FUNCTIONS
// ==============================================================================

// Print macro configuration
// =========================

void CCPi0::MacroUtil::PrintMacroConfiguration(std::string macro_name)
{
    std::cout << std::endl;
    std::cout << " MacroUtil configuration of this macro: " << macro_name << std::endl;
    
    std::cout << std::endl;
    std::cout << " Analyzing playlist " << m_plist_string << "... " << std::endl;
    
    std::cout << std::endl;
    if ( m_do_data ) std::cout << " \tNumber of data files: " << m_data->GetChain()->GetListOfFiles()->GetEntriesFast() << std::endl;
    if ( m_do_mc ) {
        std::cout << " \tNumber of MC reco files: " << m_mc->GetChain()->GetListOfFiles()->GetEntriesFast() << std::endl;
        if ( m_do_truth ) std::cout << " \tNumber of Truth files:   " << m_truth->GetChain()->GetListOfFiles()->GetEntriesFast() << std::endl;
    }
    
    std::cout << std::endl;
    if ( m_do_data ) std::cout << " \tData POT: " << m_data_pot << std::endl;
    if ( m_do_mc )   std::cout << " \tMC POT: " << m_mc_pot   << std::endl;
    std::cout << std::endl;
    
    std::string truth_str = m_do_truth ? "Yes" : "No";
    if ( m_do_mc ) std::cout << " \tUse Truth tree? " << truth_str << std::endl;
    
    std::string syst_str = m_do_systematics ? "Yes" : "No";
    if ( m_do_mc ) {
        std::cout << " \tInclude systematics? " << syst_str << std::endl;
        if ( m_do_systematics ) std::cout << " \tNumber of flux universes: " << MinervaUniverse::GetNFluxUniverses() << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << " \tAnalysis nu PDG: " << MinervaUniverse::GetAnalysisNuPDG() << std::endl;
    
    std::string nue_str = MinervaUniverse::UseNuEConstraint() ? "Yes" : "No";
    std::cout << " \tUse nu-e constraint? " << nue_str << std::endl;
    std::cout << std::endl;
    
    if ( m_do_mc ) {
        std::string non_res_pi_str = MinervaUniverse::UseNonResPiReweight() ? "Yes" : "No";
        std::cout << " \tUse non-res pion reweight? " << non_res_pi_str << std::endl;
        
        std::string deut_tune_str = MinervaUniverse::UseDeuteriumGeniePiTune() ? "Yes" : "No";
        std::cout << " \tUse deuterium tune? " << deut_tune_str << std::endl;
        
        std::string zexp_tune_str = MinervaUniverse::UseZExpansionFaReweight() ? "Yes" : "No";
        std::cout << " \tUse Z-expansion tune? " << zexp_tune_str << std::endl;
        std::cout << std::endl;
        
        std::cout << " \tHadron reweighter readout volume: " << CCPi0AnaConstants::kMHRReadoutVolume << std::endl;
        
        std::string hrw_neu_str = CCPi0AnaConstants::kUseMHRWeightNeutronCVReweight ? "Yes" : "No";
        std::cout << " \tHadron reweighter: Use neutron CV reweight? " << hrw_neu_str << std::endl;
        
        std::string hrw_elas_str = CCPi0AnaConstants::kUseMHRWeightElastics ? "Yes" : "No";
        std::cout << " \tHadron reweighter: Use elastic weight? " << hrw_elas_str << std::endl;
    }
}



// Setup loop (for looping and filling functions)
// ==============================================

void SetupLoop(const EnumDataMCTruth& type_DataMCTruth,
               const EnumModels& type_model,
               const CCPi0::MacroUtil& util,
               bool& is_mc, bool& is_truth, Long64_t& n_entries)
{
    std::string option_model;
    
    switch ( type_model )
    {    
        case kGENIE :
            option_model = "GENIE 2.12.6";
            break;
            
        case kMnvGENIEv1 :
            option_model = "MnvGENIE v1";
            break;
            
        case kMnvGENIEv1_noNonResPi :
            option_model = "MnvGENIE v1 w/o non-resonant pion tune";
            break;
            
        case kMnvGENIEv1_noD2 :
            option_model = "MnvGENIE v1 w/o deuterium pion tune";
            break;
            
        case kMnvGENIEv1_noPionTune :
            option_model = "MnvGENIE v1 w/o non-resonant and deuterium pion tunes";
            break;
            
        case kMnvGENIEv2_MINOS :
            option_model = "MnvGENIE v2 MINOS";
            break;
            
        case kMnvGENIEv2_JOINT :
            option_model = "MnvGENIE v2 JOINT";
            break;
            
        case kMnvGENIEv2_NU1PI :
            option_model = "MnvGENIE v2 NU1PI";
            break;
            
        case kMnvGENIEv2_NUNPI :
            option_model = "MnvGENIE v2 NUNPI";
            break;
            
        case kMnvGENIEv2_NUPI0 :
            option_model = "MnvGENIE v2 NUPI0";
            break;
            
        case kMnvGENIEv2_MENU1PI :
            option_model = "MnvGENIE v2 ME NU1PI";
            break;
            
        case kDataNoModel :
            option_model = "";
            break;
            
        default : {
            std::cout << " SetupLoop() ERROR: MC MODEL NOT SPECIFIED!!! " << std::endl;
            exit(1);
        }
    }
    
    switch ( type_DataMCTruth )
    {
        case kData : {
            is_mc     = false;
            is_truth  = false;
            n_entries = util.GetDataEntries();
            std::cout << std::endl;
            std::cout << " \tLooping over " << n_entries << " entries of DATA... " << std::endl;
            break;
        }
        case kMC : {
            is_mc     = true;
            is_truth  = false;
            n_entries = util.GetMCEntries();
            std::cout << std::endl;
            std::cout << " \tLooping over " << n_entries << " entries of MC using " << option_model << " model... " << std::endl;
            break;
        }
        case kTruth : {
            is_mc     = true;
            is_truth  = true;
            n_entries = util.GetTruthEntries();
            std::cout << std::endl;
            std::cout << " \tLooping over " << n_entries << " entries of TRUTH using " << option_model << " model... " << std::endl;
            break;
        }
        default : {
            std::cout << " SetupLoop() ERROR: DATA/MC/TRUTH TYPE NOT CORRECTLY SPECIFIED!!! " << std::endl;
            exit(1);
        }
    }
}


#endif  // CCPi0MacroUtil_cxx