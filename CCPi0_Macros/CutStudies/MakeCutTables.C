#ifndef PlotCutStudies_C
#define PlotCutStudies_C

#include <iomanip>
#include <iostream>
#include <vector>
#include <limits>

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
//  MAKE CUT EFFICIENCY AND PURITY TABLES
// ========================================================================================================================

void MakeTables(CCPi0::MacroUtil util,
                TFile& mc_fin,
                TFile& data_fin,
                std::string option_date_mc,
                std::string option_material)
{
    // Make text files
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/CutStudies/Tables/%s/%s", option_date_mc.c_str(),
                                                                                              option_material.c_str());
    
    std::ofstream text_event_count(Form("%s/EventCount_%s.txt", text_topdir.c_str(),
                                                                option_material.c_str()));
    
    std::ofstream text_rejected_events(Form("%s/RejectedEvents_%s.txt", text_topdir.c_str(),
                                                                        option_material.c_str()));
    
    
    // Set decimal precision
    text_event_count << std::fixed << std::setprecision(2);
    text_rejected_events << std::fixed << std::setprecision(2);
    
    
    // Load variables
    std::vector<Variable*> eventcount_variables_truth = GetEventCountVariablesTruth();
    std::vector<Variable*> eventcount_variables       = GetEventCountVariables();
    std::vector<Variable*> rejectedevent_variables    = GetRejectedEventVariables();
    
    LoadEventCountVariablesTruth(util, mc_fin, kTruth, eventcount_variables_truth);
    
    LoadEventCountVariables(util, mc_fin, kMC, eventcount_variables);
    LoadEventCountVariables(util, data_fin, kData, eventcount_variables);
    
    LoadRejectedEventVariables(util, mc_fin, kMC, rejectedevent_variables);
    LoadRejectedEventVariables(util, data_fin, kData, rejectedevent_variables);
    
    
    
    // Get event count info from Truth tree
    // ====================================
    
    double truth_Nevents;
    double truth_Nevents_Signal;
    double truth_Nevents_Pi0HighW;
    double truth_Nevents_QElike;
    double truth_Nevents_PionProd;
    double truth_Nevents_PlasUp;
    double truth_Nevents_PlasBetw;
    double truth_Nevents_PlasDown;
    double truth_Nevents_Other;
    
    
    for ( auto var : eventcount_variables_truth ) {
        truth_Nevents          = var->m_hists.m_mc_Selection.hist->GetBinContent(1);
        truth_Nevents_Signal   = var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(1);
        truth_Nevents_Pi0HighW = var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(1);
        truth_Nevents_QElike   = var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(1);
        truth_Nevents_PionProd = var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(1);
        truth_Nevents_PlasUp   = var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(1);
        truth_Nevents_PlasBetw = var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(1);
        truth_Nevents_PlasDown = var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(1);
        truth_Nevents_Other    = var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(1);
    }
    
    
    
    // Get event count info from MC Reco and Data trees
    // ================================================
    
    // Number of events
    std::vector<int> data_Nevents;
    std::vector<double> mc_Nevents;
    std::vector<double> mc_Nevents_Signal;
    std::vector<double> mc_Nevents_Pi0HighW;
    std::vector<double> mc_Nevents_QElike;
    std::vector<double> mc_Nevents_PionProd;
    std::vector<double> mc_Nevents_PlasUp;
    std::vector<double> mc_Nevents_PlasBetw;
    std::vector<double> mc_Nevents_PlasDown;
    std::vector<double> mc_Nevents_Other;
    
    // Number of single-shower events
    std::vector<int> data_Nevents_SingleBlob;
    std::vector<double> mc_Nevents_SingleBlob;
    std::vector<double> mc_Nevents_SingleBlob_Signal;
    std::vector<double> mc_Nevents_SingleBlob_Pi0HighW;
    std::vector<double> mc_Nevents_SingleBlob_QElike;
    std::vector<double> mc_Nevents_SingleBlob_PionProd;
    std::vector<double> mc_Nevents_SingleBlob_PlasUp;
    std::vector<double> mc_Nevents_SingleBlob_PlasBetw;
    std::vector<double> mc_Nevents_SingleBlob_PlasDown;
    std::vector<double> mc_Nevents_SingleBlob_Other;
    
    // Number of multi-shower events
    std::vector<int> data_Nevents_MultiBlob;
    std::vector<double> mc_Nevents_MultiBlob;
    std::vector<double> mc_Nevents_MultiBlob_Signal;
    std::vector<double> mc_Nevents_MultiBlob_Pi0HighW;
    std::vector<double> mc_Nevents_MultiBlob_QElike;
    std::vector<double> mc_Nevents_MultiBlob_PionProd;
    std::vector<double> mc_Nevents_MultiBlob_PlasUp;
    std::vector<double> mc_Nevents_MultiBlob_PlasBetw;
    std::vector<double> mc_Nevents_MultiBlob_PlasDown;
    std::vector<double> mc_Nevents_MultiBlob_Other;
    
    // Blob PDG of single-blob events
    std::vector<int> data_Nblobs_SingleBlob;
    std::vector<double> mc_Nblobs_SingleBlob;
    std::vector<double> mc_Nblobs_SingleBlob_Pi0;
    std::vector<double> mc_Nblobs_SingleBlob_Proton;
    std::vector<double> mc_Nblobs_SingleBlob_Neutron;
    std::vector<double> mc_Nblobs_SingleBlob_Pion;
    std::vector<double> mc_Nblobs_SingleBlob_EM;
    std::vector<double> mc_Nblobs_SingleBlob_Muon;
    std::vector<double> mc_Nblobs_SingleBlob_OthPdg;
    std::vector<double> mc_Nblobs_SingleBlob_MCXtalk;
    std::vector<double> mc_Nblobs_SingleBlob_Overlay;
    
    // Blob PDG of multi-blob events
    std::vector<int> data_Nblobs_MultiBlob;
    std::vector<double> mc_Nblobs_MultiBlob;
    std::vector<double> mc_Nblobs_MultiBlob_Pi0;
    std::vector<double> mc_Nblobs_MultiBlob_Proton;
    std::vector<double> mc_Nblobs_MultiBlob_Neutron;
    std::vector<double> mc_Nblobs_MultiBlob_Pion;
    std::vector<double> mc_Nblobs_MultiBlob_EM;
    std::vector<double> mc_Nblobs_MultiBlob_Muon;
    std::vector<double> mc_Nblobs_MultiBlob_OthPdg;
    std::vector<double> mc_Nblobs_MultiBlob_MCXtalk;
    std::vector<double> mc_Nblobs_MultiBlob_Overlay;
    
    
    for ( auto var : eventcount_variables )
    {
        // Number of events
        if ( var->Name() == "EventCount" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_Nevents.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_Nevents.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_Nevents_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_Nevents_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_Nevents_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_Nevents_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_Nevents_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_Nevents_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_Nevents_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_Nevents_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Number of single-shower events
        else if ( var->Name() == "EventCount_Single" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_Nevents_SingleBlob.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_Nevents_SingleBlob.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_Nevents_SingleBlob_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Number of multi-shower events
        else if ( var->Name() == "EventCount_Multi" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_Nevents_MultiBlob.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_Nevents_MultiBlob.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_Nevents_MultiBlob_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Blob PDG of single-blob events
        else if ( var->Name() == "BlobPdg_Single" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_Nblobs_SingleBlob.push_back((int)var->m_hists.m_data_ObjectPdg->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob.push_back(var->m_hists.m_mc_ObjectPdg.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Pi0.push_back(var->m_hists.m_mc_ObjectPdg_Pi0.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Proton.push_back(var->m_hists.m_mc_ObjectPdg_Proton.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Neutron.push_back(var->m_hists.m_mc_ObjectPdg_Neutron.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Pion.push_back(var->m_hists.m_mc_ObjectPdg_Pion.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_EM.push_back(var->m_hists.m_mc_ObjectPdg_EM.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Muon.push_back(var->m_hists.m_mc_ObjectPdg_Muon.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_OthPdg.push_back(var->m_hists.m_mc_ObjectPdg_OthPdg.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_MCXtalk.push_back(var->m_hists.m_mc_ObjectPdg_MCXtalk.hist->GetBinContent(cut+1));
                mc_Nblobs_SingleBlob_Overlay.push_back(var->m_hists.m_mc_ObjectPdg_Overlay.hist->GetBinContent(cut+1));
            }
        }
        
        // Blob PDG of single-blob events
        else if ( var->Name() == "BlobPdg_Multi" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_Nblobs_MultiBlob.push_back((int)var->m_hists.m_data_ObjectPdg->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob.push_back(var->m_hists.m_mc_ObjectPdg.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Pi0.push_back(var->m_hists.m_mc_ObjectPdg_Pi0.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Proton.push_back(var->m_hists.m_mc_ObjectPdg_Proton.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Neutron.push_back(var->m_hists.m_mc_ObjectPdg_Neutron.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Pion.push_back(var->m_hists.m_mc_ObjectPdg_Pion.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_EM.push_back(var->m_hists.m_mc_ObjectPdg_EM.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Muon.push_back(var->m_hists.m_mc_ObjectPdg_Muon.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_OthPdg.push_back(var->m_hists.m_mc_ObjectPdg_OthPdg.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_MCXtalk.push_back(var->m_hists.m_mc_ObjectPdg_MCXtalk.hist->GetBinContent(cut+1));
                mc_Nblobs_MultiBlob_Overlay.push_back(var->m_hists.m_mc_ObjectPdg_Overlay.hist->GetBinContent(cut+1));
            }
        }
    }
    
    
    
    // Get rejected events info from MC Reco and Data trees
    // ====================================================
    
    // Number of rejected events
    std::vector<int> data_NrejectedEvents;
    std::vector<double> mc_NrejectedEvents;
    std::vector<double> mc_NrejectedEvents_Signal;
    std::vector<double> mc_NrejectedEvents_Pi0HighW;
    std::vector<double> mc_NrejectedEvents_QElike;
    std::vector<double> mc_NrejectedEvents_PionProd;
    std::vector<double> mc_NrejectedEvents_PlasUp;
    std::vector<double> mc_NrejectedEvents_PlasBetw;
    std::vector<double> mc_NrejectedEvents_PlasDown;
    std::vector<double> mc_NrejectedEvents_Other;
    
    // Number of single-shower rejected events
    std::vector<int> data_NrejectedEvents_SingleBlob;
    std::vector<double> mc_NrejectedEvents_SingleBlob;
    std::vector<double> mc_NrejectedEvents_SingleBlob_Signal;
    std::vector<double> mc_NrejectedEvents_SingleBlob_Pi0HighW;
    std::vector<double> mc_NrejectedEvents_SingleBlob_QElike;
    std::vector<double> mc_NrejectedEvents_SingleBlob_PionProd;
    std::vector<double> mc_NrejectedEvents_SingleBlob_PlasUp;
    std::vector<double> mc_NrejectedEvents_SingleBlob_PlasBetw;
    std::vector<double> mc_NrejectedEvents_SingleBlob_PlasDown;
    std::vector<double> mc_NrejectedEvents_SingleBlob_Other;
    
    // Number of multi-shower rejected events
    std::vector<int> data_NrejectedEvents_MultiBlob;
    std::vector<double> mc_NrejectedEvents_MultiBlob;
    std::vector<double> mc_NrejectedEvents_MultiBlob_Signal;
    std::vector<double> mc_NrejectedEvents_MultiBlob_Pi0HighW;
    std::vector<double> mc_NrejectedEvents_MultiBlob_QElike;
    std::vector<double> mc_NrejectedEvents_MultiBlob_PionProd;
    std::vector<double> mc_NrejectedEvents_MultiBlob_PlasUp;
    std::vector<double> mc_NrejectedEvents_MultiBlob_PlasBetw;
    std::vector<double> mc_NrejectedEvents_MultiBlob_PlasDown;
    std::vector<double> mc_NrejectedEvents_MultiBlob_Other;
    
    // Blob PDG of single-blob rejected events
    std::vector<int> data_NrejectedBlobs_SingleBlob;
    std::vector<double> mc_NrejectedBlobs_SingleBlob;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Pi0;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Proton;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Neutron;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Pion;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_EM;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Muon;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_OthPdg;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_MCXtalk;
    std::vector<double> mc_NrejectedBlobs_SingleBlob_Overlay;
    
    // Blob PDG of multi-blob rejected events
    std::vector<int> data_NrejectedBlobs_MultiBlob;
    std::vector<double> mc_NrejectedBlobs_MultiBlob;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Pi0;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Proton;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Neutron;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Pion;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_EM;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Muon;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_OthPdg;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_MCXtalk;
    std::vector<double> mc_NrejectedBlobs_MultiBlob_Overlay;
    
    
    for ( auto var : rejectedevent_variables )
    {
        // Number of events
        if ( var->Name() == "RejectedEvent" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_NrejectedEvents.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_NrejectedEvents.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Number of single-shower events
        else if ( var->Name() == "RejectedEvent_Single" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_NrejectedEvents_SingleBlob.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_SingleBlob_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Number of multi-shower events
        else if ( var->Name() == "RejectedEvent_Multi" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_NrejectedEvents_MultiBlob.push_back((int)var->m_hists.m_data_Selection->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob.push_back(var->m_hists.m_mc_Selection.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_Signal.push_back(var->m_hists.m_mc_Selection_Signal.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_Pi0HighW.push_back(var->m_hists.m_mc_Selection_BackgrPi0HighW.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_QElike.push_back(var->m_hists.m_mc_Selection_BackgrQElike.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_PionProd.push_back(var->m_hists.m_mc_Selection_BackgrPionProd.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_PlasUp.push_back(var->m_hists.m_mc_Selection_BackgrPlasUp.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_PlasBetw.push_back(var->m_hists.m_mc_Selection_BackgrPlasBetw.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_PlasDown.push_back(var->m_hists.m_mc_Selection_BackgrPlasDown.hist->GetBinContent(cut+1));
                mc_NrejectedEvents_MultiBlob_Other.push_back(var->m_hists.m_mc_Selection_BackgrOther.hist->GetBinContent(cut+1));
            }
        }
        
        // Blob PDG of single-blob events
        else if ( var->Name() == "RejectedBlobPdg_Single" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_NrejectedBlobs_SingleBlob.push_back((int)var->m_hists.m_data_ObjectPdg->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob.push_back(var->m_hists.m_mc_ObjectPdg.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Pi0.push_back(var->m_hists.m_mc_ObjectPdg_Pi0.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Proton.push_back(var->m_hists.m_mc_ObjectPdg_Proton.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Neutron.push_back(var->m_hists.m_mc_ObjectPdg_Neutron.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Pion.push_back(var->m_hists.m_mc_ObjectPdg_Pion.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_EM.push_back(var->m_hists.m_mc_ObjectPdg_EM.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Muon.push_back(var->m_hists.m_mc_ObjectPdg_Muon.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_OthPdg.push_back(var->m_hists.m_mc_ObjectPdg_OthPdg.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_MCXtalk.push_back(var->m_hists.m_mc_ObjectPdg_MCXtalk.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_SingleBlob_Overlay.push_back(var->m_hists.m_mc_ObjectPdg_Overlay.hist->GetBinContent(cut+1));
            }
        }
        
        // Blob PDG of single-blob events
        else if ( var->Name() == "RejectedBlobPdg_Multi" )
        {
            for ( int cut = 0; cut <= 16; ++cut ) {
                data_NrejectedBlobs_MultiBlob.push_back((int)var->m_hists.m_data_ObjectPdg->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob.push_back(var->m_hists.m_mc_ObjectPdg.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Pi0.push_back(var->m_hists.m_mc_ObjectPdg_Pi0.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Proton.push_back(var->m_hists.m_mc_ObjectPdg_Proton.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Neutron.push_back(var->m_hists.m_mc_ObjectPdg_Neutron.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Pion.push_back(var->m_hists.m_mc_ObjectPdg_Pion.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_EM.push_back(var->m_hists.m_mc_ObjectPdg_EM.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Muon.push_back(var->m_hists.m_mc_ObjectPdg_Muon.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_OthPdg.push_back(var->m_hists.m_mc_ObjectPdg_OthPdg.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_MCXtalk.push_back(var->m_hists.m_mc_ObjectPdg_MCXtalk.hist->GetBinContent(cut+1));
                mc_NrejectedBlobs_MultiBlob_Overlay.push_back(var->m_hists.m_mc_ObjectPdg_Overlay.hist->GetBinContent(cut+1));
            }
        }
    }
    
    
    // Print event count table
    // =======================
    
    text_event_count << std::endl;
    text_event_count << " ============ " << std::endl;
    text_event_count << "  ALL EVENTS  " << std::endl;
    text_event_count << " ============ " << std::endl;
    text_event_count << std::endl;
    
    // Truth tree row
    text_event_count << "X"                                               << " "  // No data entries in Truth tree
                     << (truth_Nevents_Signal/truth_Nevents)*100.0        << " "  // Signal purity (%)
                     << (truth_Nevents_Signal/truth_Nevents_Signal)*100.0 << " "  // Signal efficiency (%)
                     << (truth_Nevents_Signal/truth_Nevents_Signal)*100.0 << " "  // Signal cut efficiency (%)
                     << (truth_Nevents_Pi0HighW/truth_Nevents)*100.0      << " "  // Background (%)
                     << (truth_Nevents_QElike/truth_Nevents)*100.0        << " "
                     << (truth_Nevents_PionProd/truth_Nevents)*100.0      << " "
                     << (truth_Nevents_PlasUp/truth_Nevents)*100.0        << " "
                     << (truth_Nevents_PlasBetw/truth_Nevents)*100.0      << " "
                     << (truth_Nevents_PlasDown/truth_Nevents)*100.0      << " "
                     << (truth_Nevents_Other/truth_Nevents)*100.0         << std::endl;
    
    // No cuts reco row
    text_event_count << data_Nevents[0]                                   << " "  // Data events
                     << (mc_Nevents_Signal[0]/mc_Nevents[0])*100.0        << " "  // Signal purity (%)
                     << (mc_Nevents_Signal[0]/truth_Nevents_Signal)*100.0 << " "  // Signal efficiency (%)
                     << (mc_Nevents_Signal[0]/mc_Nevents_Signal[0])*100.0 << " "  // Signal cut efficiency (%)
                     << (mc_Nevents_Pi0HighW[0]/mc_Nevents[0])*100.0      << " "  // Background (%)
                     << (mc_Nevents_QElike[0]/mc_Nevents[0])*100.0        << " "
                     << (mc_Nevents_PionProd[0]/mc_Nevents[0])*100.0      << " "
                     << (mc_Nevents_PlasUp[0]/mc_Nevents[0])*100.0        << " "
                     << (mc_Nevents_PlasBetw[0]/mc_Nevents[0])*100.0      << " "
                     << (mc_Nevents_PlasDown[0]/mc_Nevents[0])*100.0      << " "
                     << (mc_Nevents_Other[0]/mc_Nevents[0])*100.0         << std::endl;
    
    // All cuts reco rows
    for ( int cut = 1; cut <= 16; ++cut ) {
        text_event_count << data_Nevents[cut]                                       << " "  // Data events
                         << (mc_Nevents_Signal[cut]/mc_Nevents[cut])*100.0          << " "  // Signal purity (%)
                         << (mc_Nevents_Signal[cut]/truth_Nevents_Signal)*100.0     << " "  // Signal efficiency (%)
                         << (mc_Nevents_Signal[cut]/mc_Nevents_Signal[cut-1])*100.0 << " "  // Signal cut efficiency (%)
                         << (mc_Nevents_Pi0HighW[cut]/mc_Nevents[cut])*100.0        << " "  // Background (%)
                         << (mc_Nevents_QElike[cut]/mc_Nevents[cut])*100.0          << " "
                         << (mc_Nevents_PionProd[cut]/mc_Nevents[cut])*100.0        << " "
                         << (mc_Nevents_PlasUp[cut]/mc_Nevents[cut])*100.0          << " "
                         << (mc_Nevents_PlasBetw[cut]/mc_Nevents[cut])*100.0        << " "
                         << (mc_Nevents_PlasDown[cut]/mc_Nevents[cut])*100.0        << " "
                         << (mc_Nevents_Other[cut]/mc_Nevents[cut])*100.0           << std::endl;
    }
    text_event_count << std::endl;
    text_event_count << std::endl;
    
    
    
    // Print single-shower event count table
    // =====================================
    
    text_event_count << std::endl;
    text_event_count << " ====================== " << std::endl;
    text_event_count << "  SINGLE-SHOWER EVENTS  " << std::endl;
    text_event_count << " ====================== " << std::endl;
    text_event_count << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_event_count << data_Nevents_SingleBlob[cut]                                           << " "  // Data events
                         << (mc_Nevents_SingleBlob_Signal[cut]/mc_Nevents_SingleBlob[cut])*100.0   << " "  // Signal (%)
                         << (mc_Nevents_SingleBlob_Pi0HighW[cut]/mc_Nevents_SingleBlob[cut])*100.0 << " "  // Background (%)
                         << (mc_Nevents_SingleBlob_QElike[cut]/mc_Nevents_SingleBlob[cut])*100.0   << " "
                         << (mc_Nevents_SingleBlob_PionProd[cut]/mc_Nevents_SingleBlob[cut])*100.0 << " "
                         << (mc_Nevents_SingleBlob_PlasUp[cut]/mc_Nevents_SingleBlob[cut])*100.0   << " "
                         << (mc_Nevents_SingleBlob_PlasBetw[cut]/mc_Nevents_SingleBlob[cut])*100.0 << " "
                         << (mc_Nevents_SingleBlob_PlasDown[cut]/mc_Nevents_SingleBlob[cut])*100.0 << " "
                         << (mc_Nevents_SingleBlob_Other[cut]/mc_Nevents_SingleBlob[cut])*100.0    << std::endl;
    }
    text_event_count << std::endl;
    text_event_count << std::endl;
    
    
    
    // Print multi-shower event count table
    // ====================================
    
    text_event_count << std::endl;
    text_event_count << " ===================== " << std::endl;
    text_event_count << "  MULTI-SHOWER EVENTS  " << std::endl;
    text_event_count << " ===================== " << std::endl;
    text_event_count << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_event_count << data_Nevents_MultiBlob[cut]                                          << " "  // Data events
                         << (mc_Nevents_MultiBlob_Signal[cut]/mc_Nevents_MultiBlob[cut])*100.0   << " "  // Signal (%)
                         << (mc_Nevents_MultiBlob_Pi0HighW[cut]/mc_Nevents_MultiBlob[cut])*100.0 << " "  // Background (%)
                         << (mc_Nevents_MultiBlob_QElike[cut]/mc_Nevents_MultiBlob[cut])*100.0   << " "
                         << (mc_Nevents_MultiBlob_PionProd[cut]/mc_Nevents_MultiBlob[cut])*100.0 << " "
                         << (mc_Nevents_MultiBlob_PlasUp[cut]/mc_Nevents_MultiBlob[cut])*100.0   << " "
                         << (mc_Nevents_MultiBlob_PlasBetw[cut]/mc_Nevents_MultiBlob[cut])*100.0 << " "
                         << (mc_Nevents_MultiBlob_PlasDown[cut]/mc_Nevents_MultiBlob[cut])*100.0 << " "
                         << (mc_Nevents_MultiBlob_Other[cut]/mc_Nevents_MultiBlob[cut])*100.0    << std::endl;
    }
    text_event_count << std::endl;
    text_event_count << std::endl;
    
    
    
    // Print single-shower PDG table
    // =============================
    
    text_event_count << std::endl;
    text_event_count << " =================== " << std::endl;
    text_event_count << "  SINGLE-SHOWER PDG  " << std::endl;
    text_event_count << " =================== " << std::endl;
    text_event_count << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_event_count << data_Nblobs_SingleBlob[cut]                                         << " "
                         << (mc_Nblobs_SingleBlob_Pi0[cut]/mc_Nblobs_SingleBlob[cut])*100.0     << " "
                         << (mc_Nblobs_SingleBlob_Proton[cut]/mc_Nblobs_SingleBlob[cut])*100.0  << " "
                         << (mc_Nblobs_SingleBlob_Neutron[cut]/mc_Nblobs_SingleBlob[cut])*100.0 << " "
                         << (mc_Nblobs_SingleBlob_Pion[cut]/mc_Nblobs_SingleBlob[cut])*100.0    << " "
                         << (mc_Nblobs_SingleBlob_EM[cut]/mc_Nblobs_SingleBlob[cut])*100.0      << " "
                         << (mc_Nblobs_SingleBlob_Muon[cut]/mc_Nblobs_SingleBlob[cut])*100.0    << " "
                         << (mc_Nblobs_SingleBlob_OthPdg[cut]/mc_Nblobs_SingleBlob[cut])*100.0  << " "
                         << (mc_Nblobs_SingleBlob_MCXtalk[cut]/mc_Nblobs_SingleBlob[cut])*100.0 << " "
                         << (mc_Nblobs_SingleBlob_Overlay[cut]/mc_Nblobs_SingleBlob[cut])*100.0 << std::endl;
    }
    text_event_count << std::endl;
    text_event_count << std::endl;
    
    
    
    // Print multi-shower PDG table
    // ============================
    
    text_event_count << std::endl;
    text_event_count << " ================== " << std::endl;
    text_event_count << "  MULTI-SHOWER PDG  " << std::endl;
    text_event_count << " ================== " << std::endl;
    text_event_count << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_event_count << data_Nblobs_MultiBlob[cut]                                        << " "
                         << (mc_Nblobs_MultiBlob_Pi0[cut]/mc_Nblobs_MultiBlob[cut])*100.0     << " "
                         << (mc_Nblobs_MultiBlob_Proton[cut]/mc_Nblobs_MultiBlob[cut])*100.0  << " "
                         << (mc_Nblobs_MultiBlob_Neutron[cut]/mc_Nblobs_MultiBlob[cut])*100.0 << " "
                         << (mc_Nblobs_MultiBlob_Pion[cut]/mc_Nblobs_MultiBlob[cut])*100.0    << " "
                         << (mc_Nblobs_MultiBlob_EM[cut]/mc_Nblobs_MultiBlob[cut])*100.0      << " "
                         << (mc_Nblobs_MultiBlob_Muon[cut]/mc_Nblobs_MultiBlob[cut])*100.0    << " "
                         << (mc_Nblobs_MultiBlob_OthPdg[cut]/mc_Nblobs_MultiBlob[cut])*100.0  << " "
                         << (mc_Nblobs_MultiBlob_MCXtalk[cut]/mc_Nblobs_MultiBlob[cut])*100.0 << " "
                         << (mc_Nblobs_MultiBlob_Overlay[cut]/mc_Nblobs_MultiBlob[cut])*100.0 << std::endl;
    }
    text_event_count << std::endl;
    text_event_count << std::endl;
    
    
    
    // Print rejected events table
    // ===========================
    
    text_rejected_events << std::endl;
    text_rejected_events << " ===================== " << std::endl;
    text_rejected_events << "  ALL REJECTED EVENTS  " << std::endl;
    text_rejected_events << " ===================== " << std::endl;
    text_rejected_events << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_rejected_events << data_NrejectedEvents[cut]                                         << " "  // Data events
                             << (mc_NrejectedEvents_Signal[cut]/mc_NrejectedEvents[cut])*100.0    << " "  // Signal (%)
                             << (mc_NrejectedEvents_Pi0HighW[cut]/mc_NrejectedEvents[cut])*100.0  << " "  // Background (%)
                             << (mc_NrejectedEvents_QElike[cut]/mc_NrejectedEvents[cut])*100.0    << " "
                             << (mc_NrejectedEvents_PionProd[cut]/mc_NrejectedEvents[cut])*100.0  << " "
                             << (mc_NrejectedEvents_PlasUp[cut]/mc_NrejectedEvents[cut])*100.0    << " "
                             << (mc_NrejectedEvents_PlasBetw[cut]/mc_NrejectedEvents[cut])*100.0  << " "
                             << (mc_NrejectedEvents_PlasDown[cut]/mc_NrejectedEvents[cut])*100.0  << " "
                             << (mc_NrejectedEvents_Other[cut]/mc_NrejectedEvents[cut])*100.0     << std::endl;
    }
    text_rejected_events << std::endl;
    text_rejected_events << std::endl;
    
    
    
    // Print single-shower rejected events table
    // =========================================
    
    text_rejected_events << std::endl;
    text_rejected_events << " =============================== " << std::endl;
    text_rejected_events << "  SINGLE-SHOWER REJECTED EVENTS  " << std::endl;
    text_rejected_events << " =============================== " << std::endl;
    text_rejected_events << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_rejected_events << data_NrejectedEvents_SingleBlob[cut]                                                   << " "  // Data events
                             << (mc_NrejectedEvents_SingleBlob_Signal[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0   << " "  // Signal (%)
                             << (mc_NrejectedEvents_SingleBlob_Pi0HighW[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0 << " "  // Background (%)
                             << (mc_NrejectedEvents_SingleBlob_QElike[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0   << " "
                             << (mc_NrejectedEvents_SingleBlob_PionProd[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_SingleBlob_PlasUp[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0   << " "
                             << (mc_NrejectedEvents_SingleBlob_PlasBetw[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_SingleBlob_PlasDown[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_SingleBlob_Other[cut]/mc_NrejectedEvents_SingleBlob[cut])*100.0    << std::endl;
    }
    text_rejected_events << std::endl;
    text_rejected_events << std::endl;
    
    
    
    // Print multi-shower rejected events table
    // ========================================
    
    text_rejected_events << std::endl;
    text_rejected_events << " ============================== " << std::endl;
    text_rejected_events << "  MULTI-SHOWER REJECTED EVENTS  " << std::endl;
    text_rejected_events << " ============================== " << std::endl;
    text_rejected_events << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_rejected_events << data_NrejectedEvents_MultiBlob[cut]                                                  << " "  // Data events
                             << (mc_NrejectedEvents_MultiBlob_Signal[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0   << " "  // Signal (%)
                             << (mc_NrejectedEvents_MultiBlob_Pi0HighW[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0 << " "  // Background (%)
                             << (mc_NrejectedEvents_MultiBlob_QElike[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0   << " "
                             << (mc_NrejectedEvents_MultiBlob_PionProd[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_MultiBlob_PlasUp[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0   << " "
                             << (mc_NrejectedEvents_MultiBlob_PlasBetw[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_MultiBlob_PlasDown[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0 << " "
                             << (mc_NrejectedEvents_MultiBlob_Other[cut]/mc_NrejectedEvents_MultiBlob[cut])*100.0    << std::endl;
    }
    text_rejected_events << std::endl;
    text_rejected_events << std::endl;
    
    
    
    // Print single-shower rejected events PDG table
    // =============================================
    
    text_rejected_events << std::endl;
    text_rejected_events << " ============================ " << std::endl;
    text_rejected_events << "  REJECTED SINGLE-SHOWER PDG  " << std::endl;
    text_rejected_events << " ============================ " << std::endl;
    text_rejected_events << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_rejected_events << data_NrejectedBlobs_SingleBlob[cut]                                                 << " "
                             << (mc_NrejectedBlobs_SingleBlob_Pi0[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0     << " "
                             << (mc_NrejectedBlobs_SingleBlob_Proton[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0  << " "
                             << (mc_NrejectedBlobs_SingleBlob_Neutron[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0 << " "
                             << (mc_NrejectedBlobs_SingleBlob_Pion[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0    << " "
                             << (mc_NrejectedBlobs_SingleBlob_EM[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0      << " "
                             << (mc_NrejectedBlobs_SingleBlob_Muon[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0    << " "
                             << (mc_NrejectedBlobs_SingleBlob_OthPdg[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0  << " "
                             << (mc_NrejectedBlobs_SingleBlob_MCXtalk[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0 << " "
                             << (mc_NrejectedBlobs_SingleBlob_Overlay[cut]/mc_NrejectedBlobs_SingleBlob[cut])*100.0 << std::endl;
    }
    text_rejected_events << std::endl;
    text_rejected_events << std::endl;
    
    
    
    // Print multi-shower rejected events PDG table
    // ============================================
    
    text_rejected_events << std::endl;
    text_rejected_events << " =========================== " << std::endl;
    text_rejected_events << "  REJECTED MULTI-SHOWER PDG  " << std::endl;
    text_rejected_events << " =========================== " << std::endl;
    text_rejected_events << std::endl;
    
    // All cuts reco rows
    for ( int cut = 0; cut <= 16; ++cut ) {
        text_rejected_events << data_NrejectedBlobs_MultiBlob[cut]                                                << " "
                             << (mc_NrejectedBlobs_MultiBlob_Pi0[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0     << " "
                             << (mc_NrejectedBlobs_MultiBlob_Proton[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0  << " "
                             << (mc_NrejectedBlobs_MultiBlob_Neutron[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0 << " "
                             << (mc_NrejectedBlobs_MultiBlob_Pion[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0    << " "
                             << (mc_NrejectedBlobs_MultiBlob_EM[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0      << " "
                             << (mc_NrejectedBlobs_MultiBlob_Muon[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0    << " "
                             << (mc_NrejectedBlobs_MultiBlob_OthPdg[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0  << " "
                             << (mc_NrejectedBlobs_MultiBlob_MCXtalk[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0 << " "
                             << (mc_NrejectedBlobs_MultiBlob_Overlay[cut]/mc_NrejectedBlobs_MultiBlob[cut])*100.0 << std::endl;
    }
    text_rejected_events << std::endl;
    text_rejected_events << std::endl;
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void MakeCutTables(std::string option_date,
                   std::string option_model = "v1",
                   bool do_systematics      = false)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output file 
    const std::string option_systematics = do_systematics ? "WithSyst" : "NoSyst";
    const std::string option_date_mc     = option_date + "_" + option_model;
    const std::string option_date_data   = option_date;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // Monte Carlo
    std::string mc_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/mc/%s/lead", option_date_mc.c_str());
    
    std::string mc_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/mc/%s/iron", option_date_mc.c_str());
    
    
    // Data
    std::string data_fin_topdir_lead = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/data/%s/lead", option_date_data.c_str());
    
    std::string data_fin_topdir_iron = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CutStudies/data/%s/iron", option_date_data.c_str());
    
    
    // Input files
    // ===========
    
    // Monte Carlo
    TFile mc_fin_lead(Form("%s/MC_CutStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_lead.root", mc_fin_topdir_lead.c_str(),
                                                                                              option_model.c_str(),
                                                                                              option_systematics.c_str()), "READ");
    
    TFile mc_fin_iron(Form("%s/MC_CutStudies_MnvGENIE%s_%s_POTScaled_AllPlaylists_iron.root", mc_fin_topdir_iron.c_str(),
                                                                                              option_model.c_str(),
                                                                                              option_systematics.c_str()), "READ");
    
    
    // Data
    TFile data_fin_lead(Form("%s/Data_CutStudies_AllPlaylists_lead.root", data_fin_topdir_lead.c_str()), "READ");
    
    TFile data_fin_iron(Form("%s/Data_CutStudies_AllPlaylists_iron.root", data_fin_topdir_iron.c_str()), "READ");
    
    
    
    // =========================================
    //  MacroUtil
    // =========================================
    
    // Playlist string
    // (Use only to load MC chain and access systematics)
    const std::string plist_string = "minervame1A";
    
    
    // Set playlists MC and data input
    // (Similarly, only to load MC and access systematics)
    const std::string mc_file_list   = GetPlaylistFile(true,  plist_string, "test");
    const std::string data_file_list = GetPlaylistFile(false, plist_string, "test");
    
    
    // Set MacroUtil
    // (TRUTH option set as 'true', and SYSTEMATICS option set as 'false')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, false, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot   = GetPOT(mc_fin_lead,   true);
    double data_pot = GetPOT(data_fin_lead, false);
    
    SetMacroUtilPOT(mc_fin_lead, data_fin_lead, util);
    
    std::cout << std::endl;
    std::cout << " Making cut efficiency and purity tables... " << std::endl;
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << mc_pot   << std::endl;
    std::cout << " \tData POT: " << data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Make tables
    MakeTables(util, mc_fin_lead, data_fin_lead, option_date_mc, "lead");
    
    MakeTables(util, mc_fin_iron, data_fin_iron, option_date_mc, "iron");
    
    
    // Close ROOT files
    mc_fin_lead.Close();
    data_fin_lead.Close();
    
    mc_fin_iron.Close();
    data_fin_iron.Close();
}


#endif  // PlotCutStudies_C