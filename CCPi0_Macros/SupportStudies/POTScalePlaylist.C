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
#include "../includes/Variable2D.h"
#endif  // __CINT__

#include "TFile.h"





// ========================================================================================================================
//  SCALE MC FOR EVENT SELECTION DISTRIBUTIONS
// ========================================================================================================================

void POTScale(CCPi0::MacroUtil util,
              TFile& mc_fin,
              TFile& data_fin,
              TFile& mc_fout)
{
    // Get variables
    std::vector<Variable*>   variables   = GetSupportVariables();
    std::vector<Variable2D*> variables2D = GetSupportVariables2D();
    
    
    // Load MC and data histograms from input files
    for ( auto var : variables ) {
        var -> LoadMCHists_MatSelection(mc_fin, util.m_error_bands);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> LoadMCHists_MatSelection2D(mc_fin, util.m_error_bands);
    }
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms and write them to MC output file
    for ( auto var : variables ) {
        var -> ScaleMCHists_MatSelection(mc_pot, data_pot);
        var -> SyncMCHists_MatSelection();
        var -> WriteMCHists_MatSelection(mc_fout);
    }
    
    for ( auto var2D : variables2D ) {
        var2D -> ScaleMCHists_MatSelection2D(mc_pot, data_pot);
        var2D -> SyncMCHists_MatSelection2D();
        var2D -> WriteMCHists_MatSelection2D(mc_fout);
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
                      bool do_systematics      = false)
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
    std::string mc_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/mc/%s/lead", option_date_mc.c_str());
    std::string mc_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/data/%s/lead", option_date.c_str());
    std::string data_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/SupportStudies/data/%s/iron", option_date.c_str());
    
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_%s_lead.root", mc_fin_lead_topdir.c_str(),
                                                                              option_model.c_str(),
                                                                              option_systematics.c_str(),
                                                                              plist_string.c_str()), "READ");
    
    TFile mc_fin_iron(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_%s_iron.root", mc_fin_iron_topdir.c_str(),
                                                                              option_model.c_str(),
                                                                              option_systematics.c_str(),
                                                                              plist_string.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_SupportStudies_%s_lead.root", data_fin_lead_topdir.c_str(),
                                                                    plist_string.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_SupportStudies_%s_iron.root", data_fin_iron_topdir.c_str(),
                                                                    plist_string.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Monte Carlo
    TFile mc_fout_lead(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_POTScaled_%s_lead.root", mc_fin_lead_topdir.c_str(),
                                                                                         option_model.c_str(),
                                                                                         option_systematics.c_str(),
                                                                                         plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_iron(Form("%s/MC_SupportStudies_MnvGENIE%s_%s_POTScaled_%s_iron.root", mc_fin_iron_topdir.c_str(),
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
    // (TRUTH option set as 'true' and SYSTEMATICS option set as 'false')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, false, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot_lead   = GetPOT(mc_fin_lead,   true);
    double mc_pot_iron   = GetPOT(mc_fin_iron,   true);
    double data_pot_lead = GetPOT(data_fin_lead, false);
    double data_pot_iron = GetPOT(data_fin_iron, false);
    
    SetMacroUtilPOT(mc_fin_lead, data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " PLAYLIST: " << plist_string << std::endl;
    std::cout << std::endl;
    std::cout << " Lead:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_lead   << std::endl;
    std::cout << " \tData POT: " << data_pot_lead << std::endl;
    std::cout << std::endl;
    std::cout << " Iron:" << std::endl;
    std::cout << " \tMC POT:   " << mc_pot_iron   << std::endl;
    std::cout << " \tData POT: " << data_pot_iron << std::endl;
    std::cout << std::endl;
    
    
    // Write POT to output files (MC-only outputs)
    WritePOT(mc_fout_lead, true, mc_pot_lead);
    WritePOT(mc_fout_iron, true, mc_pot_iron);
    
    
    
    // =========================================
    //  Scale MC files
    // =========================================
    
    // Lead
    std::cout << " POT-scaling files on LEAD... " << std::endl;
    std::cout << std::endl;
    POTScale(util, mc_fin_lead, data_fin_lead, mc_fout_lead);
    
    
    // Iron
    std::cout << " POT-scaling files on IRON... " << std::endl;
    std::cout << std::endl;
    POTScale(util, mc_fin_iron, data_fin_iron, mc_fout_iron);
    
    
    // Close ROOT files
    mc_fin_lead.Close();
    mc_fin_iron.Close();
    
    data_fin_lead.Close();
    data_fin_iron.Close();
    
    mc_fout_lead.Close();
    mc_fout_iron.Close();
    
    
    std::cout << " Success with playlist: " << plist_string << "!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // POTScalePlaylist_C