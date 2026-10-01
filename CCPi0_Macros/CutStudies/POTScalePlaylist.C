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
#include "BookHistograms.h"

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
    
    
    // Load MC histograms
    LoadEventCountVariablesTruth(util, mc_fin, kTruth, eventcount_variables_truth);
    LoadEventCountVariables(util, mc_fin, kMC, eventcount_variables);
    LoadRejectedEventVariables(util, mc_fin, kMC, rejectedevent_variables);
    
    LoadVariables(util, mc_fin, kMC, michel_variables,           "Michel");
    LoadVariables(util, mc_fin, kMC, track_variables,            "Track");
    LoadVariables(util, mc_fin, kMC, anglescan_variables,        "AngleScan");
    LoadVariables(util, mc_fin, kMC, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    
    LoadVariables(util,   mc_fin, kMC, blobdeviation_variables,   "BlobDeviation");
    LoadVariables2D(util, mc_fin, kMC, blobdeviation_variables2D, "BlobDeviation");
    
    LoadVariables(util,   mc_fin, kMC, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    LoadVariables2D(util, mc_fin, kMC, blobenergyVSdx_variables2D, "BlobEnergyVSdx");
    
    LoadVariables(util,   mc_fin, kMC, blobdEdxend_variables,   "BlobdEdxEnd");
    LoadVariables2D(util, mc_fin, kMC, blobdEdxend_variables2D, "BlobdEdxEnd");
    
    LoadVariables(util,   mc_fin, kMC, afterblobenergy_variables,   "AfterBlobEnergy");
    LoadVariables2D(util, mc_fin, kMC, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    LoadVariables(util, mc_fin, kMC, blobmichel_variables, "BlobMichel");
    LoadVariables(util, mc_fin, kMC, energy_variables,     "Recoil");
    LoadVariables(util, mc_fin, kMC, xsection_variables,   "AllCuts");
    
    
    // Get POT from input files
    double mc_pot   = GetPOT(mc_fin,   true);
    double data_pot = GetPOT(data_fin, false);
    
    
    // POT-scale MC histograms
    ScaleEventCountVariablesTruth(eventcount_variables_truth, mc_pot, data_pot);
    ScaleEventCountVariables(eventcount_variables, mc_pot, data_pot);
    ScaleRejectedEventVariables(rejectedevent_variables, mc_pot, data_pot);
    
    ScaleMCVariables(michel_variables,           mc_pot, data_pot, "Michel");
    ScaleMCVariables(track_variables,            mc_pot, data_pot, "Track");
    ScaleMCVariables(anglescan_variables,        mc_pot, data_pot, "AngleScan");
    ScaleMCVariables(blobanglewrtmuon_variables, mc_pot, data_pot, "BlobAngleWRTMuon");
    
    ScaleMCVariables(blobdeviation_variables,     mc_pot, data_pot, "BlobDeviation");
    ScaleMCVariables2D(blobdeviation_variables2D, mc_pot, data_pot, "BlobDeviation");
    
    ScaleMCVariables(blobenergyVSdx_variables,     mc_pot, data_pot, "BlobEnergyVSdx");
    ScaleMCVariables2D(blobenergyVSdx_variables2D, mc_pot, data_pot, "BlobEnergyVSdx");
    
    ScaleMCVariables(blobdEdxend_variables,     mc_pot, data_pot, "BlobdEdxEnd");
    ScaleMCVariables2D(blobdEdxend_variables2D, mc_pot, data_pot, "BlobdEdxEnd");
    
    ScaleMCVariables(afterblobenergy_variables,     mc_pot, data_pot, "AfterBlobEnergy");
    ScaleMCVariables2D(afterblobenergy_variables2D, mc_pot, data_pot, "AfterBlobEnergy");
    
    ScaleMCVariables(blobmichel_variables, mc_pot, data_pot, "BlobMichel");
    ScaleMCVariables(energy_variables,     mc_pot, data_pot, "Recoil");
    ScaleMCVariables(xsection_variables,   mc_pot, data_pot, "AllCuts");
    
    
    // Sync and write MC histograms
    SyncAndWriteEventCountVariablesTruth(mc_fout, kTruth, eventcount_variables_truth);
    SyncAndWriteEventCountVariables(mc_fout, kMC, eventcount_variables);
    SyncAndWriteRejectedEventVariables(mc_fout, kMC, rejectedevent_variables);
    
    SyncAndWriteVariables(mc_fout, kMC, michel_variables,           "Michel");
    SyncAndWriteVariables(mc_fout, kMC, track_variables,            "Track");
    SyncAndWriteVariables(mc_fout, kMC, anglescan_variables,        "AngleScan");
    SyncAndWriteVariables(mc_fout, kMC, blobanglewrtmuon_variables, "BlobAngleWRTMuon");
    
    SyncAndWriteVariables(mc_fout,   kMC, blobdeviation_variables,   "BlobDeviation");
    SyncAndWriteVariables2D(mc_fout, kMC, blobdeviation_variables2D, "BlobDeviation");
    
    SyncAndWriteVariables(mc_fout,   kMC, blobenergyVSdx_variables,   "BlobEnergyVSdx");
    SyncAndWriteVariables2D(mc_fout, kMC, blobenergyVSdx_variables2D, "BlobEnergyVSdx");
    
    SyncAndWriteVariables(mc_fout,   kMC, blobdEdxend_variables,   "BlobdEdxEnd");
    SyncAndWriteVariables2D(mc_fout, kMC, blobdEdxend_variables2D, "BlobdEdxEnd");
    
    SyncAndWriteVariables(mc_fout,   kMC, afterblobenergy_variables,   "AfterBlobEnergy");
    SyncAndWriteVariables2D(mc_fout, kMC, afterblobenergy_variables2D, "AfterBlobEnergy");
    
    SyncAndWriteVariables(mc_fout, kMC, blobmichel_variables, "BlobMichel");
    SyncAndWriteVariables(mc_fout, kMC, energy_variables,     "Recoil");
    SyncAndWriteVariables(mc_fout, kMC, xsection_variables,   "AllCuts");
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
    std::string mc_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/mc/%s/lead", option_date_mc.c_str());
    std::string mc_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_lead_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/data/%s/lead", option_date.c_str());
    std::string data_fin_iron_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/data/%s/iron", option_date.c_str());
    
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_CutStudies_MnvGENIE%s_%s_%s_lead.root", mc_fin_lead_topdir.c_str(),
                                                                          option_model.c_str(),
                                                                          option_systematics.c_str(),
                                                                          plist_string.c_str()), "READ");
    
    TFile mc_fin_iron(Form("%s/MC_CutStudies_MnvGENIE%s_%s_%s_iron.root", mc_fin_iron_topdir.c_str(),
                                                                          option_model.c_str(),
                                                                          option_systematics.c_str(),
                                                                          plist_string.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_CutStudies_%s_lead.root", data_fin_lead_topdir.c_str(),
                                                                plist_string.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_CutStudies_%s_iron.root", data_fin_iron_topdir.c_str(),
                                                                plist_string.c_str()), "READ");
    
    
    
    // =========================================
    //  Output files
    // =========================================
    
    // Monte Carlo
    TFile mc_fout_lead(Form("%s/MC_CutStudies_MnvGENIE%s_%s_POTScaled_%s_lead.root", mc_fin_lead_topdir.c_str(),
                                                                                     option_model.c_str(),
                                                                                     option_systematics.c_str(),
                                                                                     plist_string.c_str()), "RECREATE");
    
    TFile mc_fout_iron(Form("%s/MC_CutStudies_MnvGENIE%s_%s_POTScaled_%s_iron.root", mc_fin_iron_topdir.c_str(),
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