#ifndef Histograms2D_cxx
#define Histograms2D_cxx

#include <algorithm>

#include "Histograms2D.h"



// ==========================================================================
//  DEFAULT CONSTRUCTOR
// ==========================================================================

Histograms2D::Histograms2D()
    : m_label(),
      m_xlabel(),
      m_binsx_array(0),
      m_binsx_vector(0),
      m_ylabel(),
      m_binsy_array(0),
      m_binsy_vector(0),
      
      // Event selection
      m_data_Selection2D(), m_mc_Selection2D(),
      m_mc_Selection2D_Signal(), m_mc_Selection2D_Backgr(),
      m_mc_Selection2D_BackgrPi0HighW(), m_mc_Selection2D_BackgrQElike(), m_mc_Selection2D_BackgrPionProd(),
      m_mc_Selection2D_BackgrPlasUp(), m_mc_Selection2D_BackgrPlasBetw(), m_mc_Selection2D_BackgrPlasDown(), m_mc_Selection2D_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection2D(), m_mc_MatSelection2D(),
      m_mc_MatSelection2D_TrueTgt4Pb(), m_mc_MatSelection2D_TrueTgt5Pb(), m_mc_MatSelection2D_TrueTgt5Fe(),
      m_mc_MatSelection2D_TruePlasUp(), m_mc_MatSelection2D_TruePlasBetw(), m_mc_MatSelection2D_TruePlasDown(), m_mc_MatSelection2D_TrueOtherMat(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg2D(), m_mc_ObjectPdg2D(),
      m_mc_ObjectPdg2D_Pi0(), m_mc_ObjectPdg2D_Proton(), m_mc_ObjectPdg2D_Neutron(), m_mc_ObjectPdg2D_Pion(), m_mc_ObjectPdg2D_EM(),
      m_mc_ObjectPdg2D_Muon(), m_mc_ObjectPdg2D_OthPdg(), m_mc_ObjectPdg2D_MCXtalk(),m_mc_ObjectPdg2D_Overlay()
{}





// ==========================================================================
//  UNIFORM BIN SIZE CONSTRUCTOR
// ==========================================================================

Histograms2D::Histograms2D(const std::string label,
                           const std::string xlabel,
                           const int nbinsx, const double xmin, const double xmax,
                           const std::string ylabel,
                           const int nbinsy, const double ymin, const double ymax)
    : m_label(label),
      m_xlabel(xlabel),
      m_binsx_array(MakeUniformBinArray(nbinsx, xmin, xmax)),
      m_binsx_vector(GetVecFromArray(m_binsx_array)),
      m_ylabel(ylabel),
      m_binsy_array(MakeUniformBinArray(nbinsy, ymin, ymax)),
      m_binsy_vector(GetVecFromArray(m_binsy_array)),
      
      // Event selection
      m_data_Selection2D(), m_mc_Selection2D(),
      m_mc_Selection2D_Signal(), m_mc_Selection2D_Backgr(),
      m_mc_Selection2D_BackgrPi0HighW(), m_mc_Selection2D_BackgrQElike(), m_mc_Selection2D_BackgrPionProd(),
      m_mc_Selection2D_BackgrPlasUp(), m_mc_Selection2D_BackgrPlasBetw(), m_mc_Selection2D_BackgrPlasDown(), m_mc_Selection2D_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection2D(), m_mc_MatSelection2D(),
      m_mc_MatSelection2D_TrueTgt4Pb(), m_mc_MatSelection2D_TrueTgt5Pb(), m_mc_MatSelection2D_TrueTgt5Fe(),
      m_mc_MatSelection2D_TruePlasUp(), m_mc_MatSelection2D_TruePlasBetw(), m_mc_MatSelection2D_TruePlasDown(), m_mc_MatSelection2D_TrueOtherMat(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg2D(), m_mc_ObjectPdg2D(),
      m_mc_ObjectPdg2D_Pi0(), m_mc_ObjectPdg2D_Proton(), m_mc_ObjectPdg2D_Neutron(), m_mc_ObjectPdg2D_Pion(), m_mc_ObjectPdg2D_EM(),
      m_mc_ObjectPdg2D_Muon(), m_mc_ObjectPdg2D_OthPdg(), m_mc_ObjectPdg2D_MCXtalk(),m_mc_ObjectPdg2D_Overlay()
{}





// ==========================================================================
//  VARIABLE BIN SIZE CONSTRUCTOR
// ==========================================================================

Histograms2D::Histograms2D(const std::string label,
                           const std::string xlabel,
                           const TArrayD& binsx_array,
                           const std::string ylabel,
                           const TArrayD& binsy_array)
    : m_label(label),
      m_xlabel(xlabel),
      m_binsx_array(GetSortedArray(binsx_array)),
      m_binsx_vector(GetVecFromArray(m_binsx_array)),
      m_ylabel(ylabel),
      m_binsy_array(GetSortedArray(binsy_array)),
      m_binsy_vector(GetVecFromArray(m_binsy_array)),
      
      // Event selection
      m_data_Selection2D(), m_mc_Selection2D(),
      m_mc_Selection2D_Signal(), m_mc_Selection2D_Backgr(),
      m_mc_Selection2D_BackgrPi0HighW(), m_mc_Selection2D_BackgrQElike(), m_mc_Selection2D_BackgrPionProd(),
      m_mc_Selection2D_BackgrPlasUp(), m_mc_Selection2D_BackgrPlasBetw(), m_mc_Selection2D_BackgrPlasDown(), m_mc_Selection2D_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection2D(), m_mc_MatSelection2D(),
      m_mc_MatSelection2D_TrueTgt4Pb(), m_mc_MatSelection2D_TrueTgt5Pb(), m_mc_MatSelection2D_TrueTgt5Fe(),
      m_mc_MatSelection2D_TruePlasUp(), m_mc_MatSelection2D_TruePlasBetw(), m_mc_MatSelection2D_TruePlasDown(), m_mc_MatSelection2D_TrueOtherMat(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg2D(), m_mc_ObjectPdg2D(),
      m_mc_ObjectPdg2D_Pi0(), m_mc_ObjectPdg2D_Proton(), m_mc_ObjectPdg2D_Neutron(), m_mc_ObjectPdg2D_Pion(), m_mc_ObjectPdg2D_EM(),
      m_mc_ObjectPdg2D_Muon(), m_mc_ObjectPdg2D_OthPdg(), m_mc_ObjectPdg2D_MCXtalk(),m_mc_ObjectPdg2D_Overlay()
{}





// ==========================================================================
//  INITIALIZE SELECTION HISTOGRAMS
// ==========================================================================

// Monte Carlo
// ===========

template<typename T>
void Histograms2D::InitMCHists_Selection2D(T systematic_univs)
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH2D* mc_Selection2D        = new MH2D(Form("mc_Selection2D_%s",        label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_Signal = new MH2D(Form("mc_Selection2D_%s_Signal", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_Backgr = new MH2D(Form("mc_Selection2D_%s_Backgr", label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    MH2D* mc_Selection2D_BackgrPi0HighW = new MH2D(Form("mc_Selection2D_%s_BackgrPi0HighW", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrQElike   = new MH2D(Form("mc_Selection2D_%s_BackgrQElike",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrPionProd = new MH2D(Form("mc_Selection2D_%s_BackgrPionProd", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrPlasUp   = new MH2D(Form("mc_Selection2D_%s_BackgrPlasUp",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrPlasBetw = new MH2D(Form("mc_Selection2D_%s_BackgrPlasBetw", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrPlasDown = new MH2D(Form("mc_Selection2D_%s_BackgrPlasDown", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_Selection2D_BackgrOther    = new MH2D(Form("mc_Selection2D_%s_BackgrOther",    label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Selection2D        = CVH2DW(mc_Selection2D,        systematic_univs, clear_bands);
    m_mc_Selection2D_Signal = CVH2DW(mc_Selection2D_Signal, systematic_univs, clear_bands);
    m_mc_Selection2D_Backgr = CVH2DW(mc_Selection2D_Backgr, systematic_univs, clear_bands);
    
    m_mc_Selection2D_BackgrPi0HighW = CVH2DW(mc_Selection2D_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrQElike   = CVH2DW(mc_Selection2D_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrPionProd = CVH2DW(mc_Selection2D_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrPlasUp   = CVH2DW(mc_Selection2D_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrPlasBetw = CVH2DW(mc_Selection2D_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrPlasDown = CVH2DW(mc_Selection2D_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_Selection2D_BackgrOther    = CVH2DW(mc_Selection2D_BackgrOther,    systematic_univs, clear_bands);
    
    delete mc_Selection2D;
    delete mc_Selection2D_Signal;
    delete mc_Selection2D_Backgr;
    delete mc_Selection2D_BackgrPi0HighW;
    delete mc_Selection2D_BackgrQElike;
    delete mc_Selection2D_BackgrPionProd;
    delete mc_Selection2D_BackgrPlasUp;
    delete mc_Selection2D_BackgrPlasBetw;
    delete mc_Selection2D_BackgrPlasDown;
    delete mc_Selection2D_BackgrOther;
}



// Data
// ====

void Histograms2D::InitDataHists_Selection2D()
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    m_data_Selection2D = new MH2D(Form("data_Selection2D_%s", label), label, NBinsX(), binsx, NBinsY(), binsy);
}





// ==========================================================================
//  INITIALIZE SELECTION HISTOGRAMS WITH MATERIAL BREAKDOWN
// ==========================================================================

// Monte Carlo
// ===========

template<typename T>
void Histograms2D::InitMCHists_MatSelection2D(T systematic_univs)
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH2D* mc_MatSelection2D = new MH2D(Form("mc_MatSelection2D_%s", label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    MH2D* mc_MatSelection2D_TrueTgt4Pb   = new MH2D(Form("mc_MatSelection2D_%s_TrueTgt4Pb",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TrueTgt5Pb   = new MH2D(Form("mc_MatSelection2D_%s_TrueTgt5Pb",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TrueTgt5Fe   = new MH2D(Form("mc_MatSelection2D_%s_TrueTgt5Fe",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TruePlasUp   = new MH2D(Form("mc_MatSelection2D_%s_TruePlasUp",   label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TruePlasBetw = new MH2D(Form("mc_MatSelection2D_%s_TruePlasBetw", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TruePlasDown = new MH2D(Form("mc_MatSelection2D_%s_TruePlasDown", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_MatSelection2D_TrueOtherMat = new MH2D(Form("mc_MatSelection2D_%s_TrueOtherMat", label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_MatSelection2D = CVH2DW(mc_MatSelection2D, systematic_univs, clear_bands);
    
    m_mc_MatSelection2D_TrueTgt4Pb   = CVH2DW(mc_MatSelection2D_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_MatSelection2D_TrueTgt5Pb   = CVH2DW(mc_MatSelection2D_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_MatSelection2D_TrueTgt5Fe   = CVH2DW(mc_MatSelection2D_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_MatSelection2D_TruePlasUp   = CVH2DW(mc_MatSelection2D_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_MatSelection2D_TruePlasBetw = CVH2DW(mc_MatSelection2D_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_MatSelection2D_TruePlasDown = CVH2DW(mc_MatSelection2D_TruePlasDown, systematic_univs, clear_bands);
    m_mc_MatSelection2D_TrueOtherMat = CVH2DW(mc_MatSelection2D_TrueOtherMat, systematic_univs, clear_bands);
    
    delete mc_MatSelection2D;
    delete mc_MatSelection2D_TrueTgt4Pb;
    delete mc_MatSelection2D_TrueTgt5Pb;
    delete mc_MatSelection2D_TrueTgt5Fe;
    delete mc_MatSelection2D_TruePlasUp;
    delete mc_MatSelection2D_TruePlasBetw;
    delete mc_MatSelection2D_TruePlasDown;
    delete mc_MatSelection2D_TrueOtherMat;
}



// Data
// ====

void Histograms2D::InitDataHists_MatSelection2D()
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    m_data_MatSelection2D = new MH2D(Form("data_MatSelection2D_%s", label), label, NBinsX(), binsx, NBinsY(), binsy);
}





// ==========================================================================
//  INITIALIZE RECO OBJECT HISTOGRAMS WITH PDG BREAKDOWN
// ==========================================================================

// Monte Carlo
// ===========

template<typename T>
void Histograms2D::InitMCHists_ObjectPdg2D(T systematic_univs)
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH2D* mc_ObjectPdg2D = new MH2D(Form("mc_ObjectPdg2D_%s", label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    MH2D* mc_ObjectPdg2D_Pi0     = new MH2D(Form("mc_ObjectPdg2D_%s_Pi0",     label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_Proton  = new MH2D(Form("mc_ObjectPdg2D_%s_Proton",  label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_Neutron = new MH2D(Form("mc_ObjectPdg2D_%s_Neutron", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_Pion    = new MH2D(Form("mc_ObjectPdg2D_%s_Pion",    label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_EM      = new MH2D(Form("mc_ObjectPdg2D_%s_EM",      label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_Muon    = new MH2D(Form("mc_ObjectPdg2D_%s_Muon",    label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_OthPdg  = new MH2D(Form("mc_ObjectPdg2D_%s_OthPdg",  label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_MCXtalk = new MH2D(Form("mc_ObjectPdg2D_%s_MCXtalk", label), label, NBinsX(), binsx, NBinsY(), binsy);
    MH2D* mc_ObjectPdg2D_Overlay = new MH2D(Form("mc_ObjectPdg2D_%s_Overlay", label), label, NBinsX(), binsx, NBinsY(), binsy);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_ObjectPdg2D = CVH2DW(mc_ObjectPdg2D, systematic_univs, clear_bands);
    
    m_mc_ObjectPdg2D_Pi0     = CVH2DW(mc_ObjectPdg2D_Pi0,     systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_Proton  = CVH2DW(mc_ObjectPdg2D_Proton,  systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_Neutron = CVH2DW(mc_ObjectPdg2D_Neutron, systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_Pion    = CVH2DW(mc_ObjectPdg2D_Pion,    systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_EM      = CVH2DW(mc_ObjectPdg2D_EM,      systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_Muon    = CVH2DW(mc_ObjectPdg2D_Muon,    systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_OthPdg  = CVH2DW(mc_ObjectPdg2D_OthPdg,  systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_MCXtalk = CVH2DW(mc_ObjectPdg2D_MCXtalk, systematic_univs, clear_bands);
    m_mc_ObjectPdg2D_Overlay = CVH2DW(mc_ObjectPdg2D_Overlay, systematic_univs, clear_bands);
    
    delete mc_ObjectPdg2D;
    delete mc_ObjectPdg2D_Pi0;
    delete mc_ObjectPdg2D_Proton;
    delete mc_ObjectPdg2D_Neutron;
    delete mc_ObjectPdg2D_Pion;
    delete mc_ObjectPdg2D_Muon;
    delete mc_ObjectPdg2D_OthPdg;
    delete mc_ObjectPdg2D_MCXtalk;
    delete mc_ObjectPdg2D_Overlay;
}



// Data
// ====

void Histograms2D::InitDataHists_ObjectPdg2D()
{
    const Double_t* binsx = m_binsx_array.GetArray();
    const Double_t* binsy = m_binsy_array.GetArray();
    const char* label     = m_label.c_str();
    
    m_data_ObjectPdg2D = new MH2D(Form("data_ObjectPdg2D_%s", label), label, NBinsX(), binsx, NBinsY(), binsy);
}





// ==========================================================================
//  SYNCHRONIZE MC CV HISTOGRAMS
// ==========================================================================

// Event selection
// ===============

void Histograms2D::SyncMCHists_Selection2D() {
    m_mc_Selection2D.SyncCVHistos();
    m_mc_Selection2D_Signal.SyncCVHistos();
    m_mc_Selection2D_Backgr.SyncCVHistos();
    
    m_mc_Selection2D_BackgrPi0HighW.SyncCVHistos();
    m_mc_Selection2D_BackgrQElike.SyncCVHistos();
    m_mc_Selection2D_BackgrPionProd.SyncCVHistos();
    m_mc_Selection2D_BackgrPlasUp.SyncCVHistos();
    m_mc_Selection2D_BackgrPlasBetw.SyncCVHistos();
    m_mc_Selection2D_BackgrPlasDown.SyncCVHistos();
    m_mc_Selection2D_BackgrOther.SyncCVHistos();
}



// Event selection with material breakdown
// =======================================

void Histograms2D::SyncMCHists_MatSelection2D() {
    m_mc_MatSelection2D.SyncCVHistos();
    m_mc_MatSelection2D_TrueTgt4Pb.SyncCVHistos();
    m_mc_MatSelection2D_TrueTgt5Pb.SyncCVHistos();
    m_mc_MatSelection2D_TrueTgt5Fe.SyncCVHistos();
    m_mc_MatSelection2D_TruePlasUp.SyncCVHistos();
    m_mc_MatSelection2D_TruePlasBetw.SyncCVHistos();
    m_mc_MatSelection2D_TruePlasDown.SyncCVHistos();
    m_mc_MatSelection2D_TrueOtherMat.SyncCVHistos();
}



// Reco objects with PDG breakdown
// ===============================

void Histograms2D::SyncMCHists_ObjectPdg2D() {
    m_mc_ObjectPdg2D.SyncCVHistos();
    m_mc_ObjectPdg2D_Pi0.SyncCVHistos();
    m_mc_ObjectPdg2D_Proton.SyncCVHistos();
    m_mc_ObjectPdg2D_Neutron.SyncCVHistos();
    m_mc_ObjectPdg2D_Pion.SyncCVHistos();
    m_mc_ObjectPdg2D_EM.SyncCVHistos();
    m_mc_ObjectPdg2D_Muon.SyncCVHistos();
    m_mc_ObjectPdg2D_OthPdg.SyncCVHistos();
    m_mc_ObjectPdg2D_MCXtalk.SyncCVHistos();
    m_mc_ObjectPdg2D_Overlay.SyncCVHistos();
}





// =======================================================================================
//  BIN WIDTH NORMALIZE HISTOGRAMS
// =======================================================================================

// Event selection
// ===============

// Monte Carlo
void Histograms2D::BinWidthNormMCHists_Selection2D() {
    m_mc_Selection2D.hist        -> Scale(m_mc_Selection2D.hist->GetNormBinWidthX() *
                                          m_mc_Selection2D.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_Signal.hist -> Scale(m_mc_Selection2D_Signal.hist->GetNormBinWidthX() *
                                          m_mc_Selection2D_Signal.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_Backgr.hist -> Scale(m_mc_Selection2D_Backgr.hist->GetNormBinWidthX() *
                                          m_mc_Selection2D_Backgr.hist->GetNormBinWidthY(), "width");
    
    m_mc_Selection2D_BackgrPi0HighW.hist -> Scale(m_mc_Selection2D_BackgrPi0HighW.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrPi0HighW.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrQElike.hist   -> Scale(m_mc_Selection2D_BackgrQElike.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrQElike.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrPionProd.hist -> Scale(m_mc_Selection2D_BackgrPionProd.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrPionProd.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrPlasUp.hist   -> Scale(m_mc_Selection2D_BackgrPlasUp.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrPlasUp.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrPlasBetw.hist -> Scale(m_mc_Selection2D_BackgrPlasBetw.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrPlasBetw.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrPlasDown.hist -> Scale(m_mc_Selection2D_BackgrPlasDown.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrPlasDown.hist->GetNormBinWidthY(), "width");
    m_mc_Selection2D_BackgrOther.hist    -> Scale(m_mc_Selection2D_BackgrOther.hist->GetNormBinWidthX() *
                                                  m_mc_Selection2D_BackgrOther.hist->GetNormBinWidthY(), "width");
}


// Data
void Histograms2D::BinWidthNormDataHists_Selection2D() {
    m_data_Selection2D -> Scale(m_data_Selection2D->GetNormBinWidthX() *
                                m_data_Selection2D->GetNormBinWidthY(), "width");
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms2D::BinWidthNormMCHists_MatSelection2D() {
    m_mc_MatSelection2D.hist              -> Scale(m_mc_MatSelection2D.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TrueTgt4Pb.hist   -> Scale(m_mc_MatSelection2D_TrueTgt4Pb.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TrueTgt4Pb.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TrueTgt5Pb.hist   -> Scale(m_mc_MatSelection2D_TrueTgt5Pb.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TrueTgt5Pb.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TrueTgt5Fe.hist   -> Scale(m_mc_MatSelection2D_TrueTgt5Fe.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TrueTgt5Fe.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TruePlasUp.hist   -> Scale(m_mc_MatSelection2D_TruePlasUp.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TruePlasUp.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TruePlasBetw.hist -> Scale(m_mc_MatSelection2D_TruePlasBetw.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TruePlasBetw.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TruePlasDown.hist -> Scale(m_mc_MatSelection2D_TruePlasDown.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TruePlasDown.hist->GetNormBinWidthY(), "width");
    m_mc_MatSelection2D_TrueOtherMat.hist -> Scale(m_mc_MatSelection2D_TrueOtherMat.hist->GetNormBinWidthX() *
                                                   m_mc_MatSelection2D_TrueOtherMat.hist->GetNormBinWidthY(), "width");
}


// Data
void Histograms2D::BinWidthNormDataHists_MatSelection2D() {
    m_data_MatSelection2D -> Scale(m_data_MatSelection2D->GetNormBinWidthX() *
                                   m_data_MatSelection2D->GetNormBinWidthY(), "width");
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms2D::BinWidthNormMCHists_ObjectPdg2D() {
    m_mc_ObjectPdg2D.hist         -> Scale(m_mc_ObjectPdg2D.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Pi0.hist     -> Scale(m_mc_ObjectPdg2D_Pi0.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Pi0.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Proton.hist  -> Scale(m_mc_ObjectPdg2D_Proton.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Proton.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Neutron.hist -> Scale(m_mc_ObjectPdg2D_Neutron.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Neutron.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Pion.hist    -> Scale(m_mc_ObjectPdg2D_Pion.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Pion.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_EM.hist      -> Scale(m_mc_ObjectPdg2D_EM.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_EM.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Muon.hist    -> Scale(m_mc_ObjectPdg2D_Muon.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Muon.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_OthPdg.hist  -> Scale(m_mc_ObjectPdg2D_OthPdg.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_OthPdg.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_MCXtalk.hist -> Scale(m_mc_ObjectPdg2D_MCXtalk.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_MCXtalk.hist->GetNormBinWidthY(), "width");
    m_mc_ObjectPdg2D_Overlay.hist -> Scale(m_mc_ObjectPdg2D_Overlay.hist->GetNormBinWidthX() *
                                           m_mc_ObjectPdg2D_Overlay.hist->GetNormBinWidthY(), "width");
}


// Data
void Histograms2D::BinWidthNormDataHists_ObjectPdg2D() {
    m_data_Selection2D -> Scale(m_data_Selection2D->GetNormBinWidthX() *
                                m_data_Selection2D->GetNormBinWidthY(), "width");
}





// ==========================================================================
//  SCALE MC HISTOGRAMS
// ==========================================================================

// Event selection
// ===============

void Histograms2D::ScaleMCHists_Selection2D(const double mc_pot, const double data_pot) {
    m_mc_Selection2D.hist        -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_Signal.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_Backgr.hist -> Scale(data_pot/mc_pot);
    
    m_mc_Selection2D_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection2D_BackgrOther.hist    -> Scale(data_pot/mc_pot);
}



// Event selection with material breakdown
// =======================================

void Histograms2D::ScaleMCHists_MatSelection2D(const double mc_pot, const double data_pot) {
    m_mc_MatSelection2D.hist              -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_MatSelection2D_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
}



// Reco objects with PDG breakdown
// ===============================

void Histograms2D::ScaleMCHists_ObjectPdg2D(const double mc_pot, const double data_pot) {
    m_mc_ObjectPdg2D.hist         -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Pi0.hist     -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Proton.hist  -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Neutron.hist -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Pion.hist    -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_EM.hist      -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Muon.hist    -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_OthPdg.hist  -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_MCXtalk.hist -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg2D_Overlay.hist -> Scale(data_pot/mc_pot);
}





// ==========================================================================
//  WRITE HISTOGRAMS TO FILE
// ==========================================================================

// Event selection
// ===============

// Monte Carlo
void Histograms2D::WriteMCHists_Selection2D(TFile& fout) const {
    fout.cd();
    
    m_mc_Selection2D.hist        -> Write();
    m_mc_Selection2D_Signal.hist -> Write();
    m_mc_Selection2D_Backgr.hist -> Write();
    
    m_mc_Selection2D_BackgrPi0HighW.hist -> Write();
    m_mc_Selection2D_BackgrQElike.hist   -> Write();
    m_mc_Selection2D_BackgrPionProd.hist -> Write();
    m_mc_Selection2D_BackgrPlasUp.hist   -> Write();
    m_mc_Selection2D_BackgrPlasBetw.hist -> Write();
    m_mc_Selection2D_BackgrPlasDown.hist -> Write();
    m_mc_Selection2D_BackgrOther.hist    -> Write();
}


// Data
void Histograms2D::WriteDataHists_Selection2D(TFile& fout) const {
    fout.cd();
    m_data_Selection2D -> Write();
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms2D::WriteMCHists_MatSelection2D(TFile& fout) const {
    fout.cd();
    
    m_mc_MatSelection2D.hist              -> Write();
    m_mc_MatSelection2D_TrueTgt4Pb.hist   -> Write();
    m_mc_MatSelection2D_TrueTgt5Pb.hist   -> Write();
    m_mc_MatSelection2D_TrueTgt5Fe.hist   -> Write();
    m_mc_MatSelection2D_TruePlasUp.hist   -> Write();
    m_mc_MatSelection2D_TruePlasBetw.hist -> Write();
    m_mc_MatSelection2D_TruePlasDown.hist -> Write();
    m_mc_MatSelection2D_TrueOtherMat.hist -> Write();
}


// Data
void Histograms2D::WriteDataHists_MatSelection2D(TFile& fout) const {
    fout.cd();
    m_data_MatSelection2D -> Write();
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms2D::WriteMCHists_ObjectPdg2D(TFile& fout) const {
    fout.cd();
    
    m_mc_ObjectPdg2D.hist         -> Write();
    m_mc_ObjectPdg2D_Pi0.hist     -> Write();
    m_mc_ObjectPdg2D_Proton.hist  -> Write();
    m_mc_ObjectPdg2D_Neutron.hist -> Write();
    m_mc_ObjectPdg2D_Pion.hist    -> Write();
    m_mc_ObjectPdg2D_EM.hist      -> Write();
    m_mc_ObjectPdg2D_Muon.hist    -> Write();
    m_mc_ObjectPdg2D_OthPdg.hist  -> Write();
    m_mc_ObjectPdg2D_MCXtalk.hist -> Write();
    m_mc_ObjectPdg2D_Overlay.hist -> Write();
}


// Data
void Histograms2D::WriteDataHists_ObjectPdg2D(TFile& fout) const {
    fout.cd();
    m_data_ObjectPdg2D -> Write();
}





// ==========================================================================
//  LOAD HISTOGRAMS FROM FILE
// ==========================================================================

// Helper functions
// ================

// Load 1D HistWrapper
CVHW Histograms2D::LoadHWFromFile(TFile& fin, UniverseMap& error_bands,
                                  std::string prefix,
                                  std::string suffix)
{
    const bool do_erase_bands = false;
    MH1D* hist = (MH1D*)fin.Get(Form("%s%s%s", prefix.c_str(), m_label.c_str(), suffix.c_str()));
    
    if ( hist == 0 ) {
        std::cout << " ERROR LOADING 2D HISTWRAPPER: " << m_label << " | RETURNING EMPTY OBJECT!!! " << std::endl;
        return CVHW();
    }
    
    else {
        TArrayD binsx_array = *(hist->GetXaxis()->GetXbins());
        
        if ( binsx_array.GetSize() == 0 ) {
            binsx_array.Reset();
            binsx_array = MakeUniformBinArray(hist->GetXaxis()->GetNbins(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
            hist = dynamic_cast<MH1D*>(hist->Rebin(binsx_array.GetSize()-1, hist->GetName(), binsx_array.GetArray()));
        }
        
        for ( int i = 0; i < NBinsX(); ++i ) {
            if ( m_binsx_array[i] != binsx_array[i] ) {
                std::cout << " WARNING WITH A BINNING MISMATCH FOR A HISTOGRAM " << prefix << m_label << suffix << std::endl;
                m_binsx_array = binsx_array;
                break;
            }
        }
        
        return CVHW(hist, error_bands, do_erase_bands);
    }
}


// Load 2D HistWrapper
CVH2DW Histograms2D::LoadH2DWFromFile(TFile& fin, UniverseMap& error_bands,
                                      std::string prefix, std::string suffix)
{
    const bool do_erase_bands = false;
    MH2D* hist = (MH2D*)fin.Get(Form("%s%s%s", prefix.c_str(), m_label.c_str(), suffix.c_str()));
    assert(hist);
    
    return CVH2DW(hist, error_bands, do_erase_bands);
}



// Event selection
// ===============

// Monte Carlo
void Histograms2D::LoadMCHists_Selection2D(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Selection2D        = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "");
    m_mc_Selection2D_Signal = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_Signal");
    m_mc_Selection2D_Backgr = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_Backgr");
    
    m_mc_Selection2D_BackgrPi0HighW = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrPi0HighW");
    m_mc_Selection2D_BackgrQElike   = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrQElike");
    m_mc_Selection2D_BackgrPionProd = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrPionProd");
    m_mc_Selection2D_BackgrPlasUp   = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrPlasUp");
    m_mc_Selection2D_BackgrPlasBetw = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrPlasBetw");
    m_mc_Selection2D_BackgrPlasDown = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrPlasDown");
    m_mc_Selection2D_BackgrOther    = LoadH2DWFromFile(fin, error_bands, "mc_Selection2D_", "_BackgrOther");
}


// Data
void Histograms2D::LoadDataHists_Selection2D(TFile& fin) {
    m_data_Selection2D = (MH2D*)fin.Get(Form("data_Selection2D_%s", m_label.c_str()));
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms2D::LoadMCHists_MatSelection2D(TFile& fin, UniverseMap& error_bands)
{
    m_mc_MatSelection2D              = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "");
    m_mc_MatSelection2D_TrueTgt4Pb   = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TrueTgt4Pb");
    m_mc_MatSelection2D_TrueTgt5Pb   = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TrueTgt5Pb");
    m_mc_MatSelection2D_TrueTgt5Fe   = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TrueTgt5Fe");
    m_mc_MatSelection2D_TruePlasUp   = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TruePlasUp");
    m_mc_MatSelection2D_TruePlasBetw = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TruePlasBetw");
    m_mc_MatSelection2D_TruePlasDown = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TruePlasDown");
    m_mc_MatSelection2D_TrueOtherMat = LoadH2DWFromFile(fin, error_bands, "mc_MatSelection2D_", "_TrueOtherMat");
}


// Data
void Histograms2D::LoadDataHists_MatSelection2D(TFile& fin) {
    m_data_MatSelection2D = (MH2D*)fin.Get(Form("data_MatSelection2D_%s", m_label.c_str()));
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms2D::LoadMCHists_ObjectPdg2D(TFile& fin, UniverseMap& error_bands)
{
    m_mc_ObjectPdg2D         = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "");
    m_mc_ObjectPdg2D_Pi0     = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Pi0");
    m_mc_ObjectPdg2D_Proton  = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Proton");
    m_mc_ObjectPdg2D_Neutron = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Neutron");
    m_mc_ObjectPdg2D_Pion    = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Pion");
    m_mc_ObjectPdg2D_EM      = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_EM");
    m_mc_ObjectPdg2D_Muon    = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Muon");
    m_mc_ObjectPdg2D_OthPdg  = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_OthPdg");
    m_mc_ObjectPdg2D_MCXtalk = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_MCXtalk");
    m_mc_ObjectPdg2D_Overlay = LoadH2DWFromFile(fin, error_bands, "mc_ObjectPdg2D_", "_Overlay");
}


// Data
void Histograms2D::LoadDataHists_ObjectPdg2D(TFile& fin) {
    m_data_ObjectPdg2D = (MH2D*)fin.Get(Form("data_ObjectPdg2D_%s", m_label.c_str()));
}


#endif  // Histograms2D_cxx