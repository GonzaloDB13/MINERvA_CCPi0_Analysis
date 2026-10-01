#ifndef Variable_cxx
#define Variable_cxx

#include <stdlib.h>  // exit()

#include "Variable.h"



#ifndef __CINT__  // CINT doesn't know about std::function


// ==========================================================================
//  DEFAULT CONSTRUCTOR
// ==========================================================================

Variable::Variable()
    : m_label(),
      m_units(),
      m_pointer_to_GetValue(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValue_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists(),
      m_is_true(false)
{}





// ==========================================================================
//  UNIFORM BIN SIZE CONSTRUCTOR
// ==========================================================================

// With no index
Variable::Variable(const std::string label,
                   const std::string xlabel,
                   const std::string units,
                   const int nbins, const double xmin, const double xmax,
                   PointerToCVUniverseFunction p,
                   const bool is_true)
    : m_label(label),
      m_units(units),
      m_pointer_to_GetValue(p),
      m_pointer_to_GetValue_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists(m_label, xlabel, nbins, xmin, xmax),
      m_is_true(is_true)
{}


// Using index
Variable::Variable(const std::string label,
                   const std::string xlabel,
                   const std::string units,
                   const int nbins, const double xmin, const double xmax,
                   PointerToCVUniverseFunctionWithIndex p,
                   const bool is_true)
    : m_label(label),
      m_units(units),
      m_pointer_to_GetValue(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValue_with_index(p),
      m_hists(m_label, xlabel, nbins, xmin, xmax),
      m_is_true(is_true)
{}





// ==========================================================================
//  VARIABLE BIN SIZE CONSTRUCTOR
// ==========================================================================

// With no index
Variable::Variable(const std::string label,
                   const std::string xlabel,
                   const std::string units,
                   const TArrayD& bins_array,
                   PointerToCVUniverseFunction p,
                   const bool is_true)
    : m_label(label),
      m_units(units),
      m_pointer_to_GetValue(p),
      m_pointer_to_GetValue_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists(m_label, xlabel, bins_array),
      m_is_true(is_true)
{}


// Using index
Variable::Variable(const std::string label,
                   const std::string xlabel,
                   const std::string units,
                   const TArrayD& bins_array,
                   PointerToCVUniverseFunctionWithIndex p,
                   const bool is_true)
    : m_label(label),
      m_units(units),
      m_pointer_to_GetValue(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValue_with_index(p),
      m_hists(m_label, xlabel, bins_array),
      m_is_true(is_true)
{}





// ==========================================================================
//  GET VARIABLE VALUE
// ==========================================================================

// With no index
double Variable::GetValue(const CVUniverse& universe) const {
    return m_pointer_to_GetValue(universe);
}


// Using index
double Variable::GetValue(const CVUniverse& universe, const int index) const {
    return m_pointer_to_GetValue_with_index(universe, index);
}





// =======================================================================================
//  INITIALIZE HISTOGRAMS
// =======================================================================================

// Event selection
// ===============

// Monte Carlo
template<typename T>
void Variable::InitMCHists_Selection(T systematic_univs) {
    m_hists.InitMCHists_Selection(systematic_univs);
}


// Data
void Variable::InitDataHists_Selection() {
    m_hists.InitDataHists_Selection();
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_MatSelection(T systematic_univs) {
    m_hists.InitMCHists_MatSelection(systematic_univs);
}


// Data
void Variable::InitDataHists_MatSelection() {
    m_hists.InitDataHists_MatSelection();
}



// Event selection with interaction type breakdown
// ===============================================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_IntTypeSelection(T systematic_univs) {
    m_hists.InitMCHists_IntTypeSelection(systematic_univs);
}


// Data
void Variable::InitDataHists_IntTypeSelection() {
    m_hists.InitDataHists_IntTypeSelection();
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_ObjectPdg(T systematic_univs) {
    m_hists.InitMCHists_ObjectPdg(systematic_univs);
}


// Data
void Variable::InitDataHists_ObjectPdg() {
    m_hists.InitDataHists_ObjectPdg();
}



// Efficiency components
// =====================

// Efficiency numerator
template<typename T>
void Variable::InitEffNumerator(T systematic_univs) {
    m_hists.InitEffNumerator(systematic_univs);
}


// Efficiency denominator
template<typename T>
void Variable::InitEffDenominator(T systematic_univs_truth) {
    m_hists.InitEffDenominator(systematic_univs_truth);
}



// Migration matrix
// ================

template<typename T>
void Variable::InitMigrationHists(T systematic_univs) {
    m_hists.InitMigrationHists(systematic_univs);
}



// Plastic sidebands in signal region
// ==================================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_PlasSB_In_SigReg(T systematic_univs) {
    m_hists.InitMCHists_PlasSB_In_SigReg(systematic_univs);
}


// Data
void Variable::InitDataHists_PlasSB_In_SigReg() {
    m_hists.InitDataHists_PlasSB_In_SigReg();
}



// Plastic sidebands in physics sidebands
// ======================================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_PlasSB_In_PhysSB(T systematic_univs) {
    m_hists.InitMCHists_PlasSB_In_PhysSB(systematic_univs);
}


// Data
void Variable::InitDataHists_PlasSB_In_PhysSB() {
    m_hists.InitDataHists_PlasSB_In_PhysSB();
}



// Physics sidebands
// =================

// Monte Carlo
template<typename T>
void Variable::InitMCHists_PhysSB(T systematic_univs) {
    m_hists.InitMCHists_PhysSB(systematic_univs);
}


// Data
void Variable::InitDataHists_PhysSB() {
    m_hists.InitDataHists_PhysSB();
}



// Initialize MC tuning weights
// ============================

// MC plastic weights in signal region
template<typename T>
void Variable::InitMCWeights_PlasBackgr_In_SigReg(T systematic_univs) {
    m_hists.InitMCWeights_PlasBackgr_In_SigReg(systematic_univs);
}


// MC plastic weights in physics sidebands
template<typename T>
void Variable::InitMCWeights_PlasBackgr_In_PhysSB(T systematic_univs) {
    m_hists.InitMCWeights_PlasBackgr_In_PhysSB(systematic_univs);
}


// MC physics weights
template<typename T>
void Variable::InitMCWeights_PhysBackgr(T systematic_univs) {
    m_hists.InitMCWeights_PhysBackgr(systematic_univs);
}





// =======================================================================================
//  SYNCHRONIZE MC CV HISTOGRAMS
// =======================================================================================

// Event selection
void Variable::SyncMCHists_Selection() { m_hists.SyncMCHists_Selection(); }


// Event selection with material breakdown
void Variable::SyncMCHists_MatSelection() { m_hists.SyncMCHists_MatSelection(); }


// Event selection with interaction type breakdown
void Variable::SyncMCHists_IntTypeSelection() { m_hists.SyncMCHists_IntTypeSelection(); }


// Reco objects with PDG breakdown
void Variable::SyncMCHists_ObjectPdg() { m_hists.SyncMCHists_ObjectPdg(); }


// Efficiency components
void Variable::SyncEffNumerator()   { m_hists.SyncEffNumerator();   }
void Variable::SyncEffDenominator() { m_hists.SyncEffDenominator(); }


// Migration matrix
void Variable::SyncMigrationHists() { m_hists.SyncMigrationHists(); }


// Plastic sidebands in signal region
void Variable::SyncMCHists_PlasSB_In_SigReg() { m_hists.SyncMCHists_PlasSB_In_SigReg(); }


// Plastic sidebands in physics sidebands
void Variable::SyncMCHists_PlasSB_In_PhysSB() { m_hists.SyncMCHists_PlasSB_In_PhysSB(); }


// Physics sidebands
void Variable::SyncMCHists_PhysSB() { m_hists.SyncMCHists_PhysSB(); }


// MC tuning weights
void Variable::SyncMCWeights_PlasBackgr_In_SigReg() { m_hists.SyncMCWeights_PlasBackgr_In_SigReg(); }
void Variable::SyncMCWeights_PlasBackgr_In_PhysSB() { m_hists.SyncMCWeights_PlasBackgr_In_PhysSB(); }
void Variable::SyncMCWeights_PhysBackgr()           { m_hists.SyncMCWeights_PhysBackgr();           }





// =======================================================================================
//  BIN WIDTH NORMALIZE HISTOGRAMS
// =======================================================================================

// Event selection
void Variable::BinWidthNormMCHists_Selection()   { m_hists.BinWidthNormMCHists_Selection();   }
void Variable::BinWidthNormDataHists_Selection() { m_hists.BinWidthNormDataHists_Selection(); }


// Event selection with material breakdown
void Variable::BinWidthNormMCHists_MatSelection()   { m_hists.BinWidthNormMCHists_MatSelection();   }
void Variable::BinWidthNormDataHists_MatSelection() { m_hists.BinWidthNormDataHists_MatSelection(); }


// Event selection with interaction type breakdown
void Variable::BinWidthNormMCHists_IntTypeSelection()   { m_hists.BinWidthNormMCHists_IntTypeSelection();   }
void Variable::BinWidthNormDataHists_IntTypeSelection() { m_hists.BinWidthNormDataHists_IntTypeSelection(); }


// Reco objects with PDG breakdown
void Variable::BinWidthNormMCHists_ObjectPdg()   { m_hists.BinWidthNormMCHists_ObjectPdg();   }
void Variable::BinWidthNormDataHists_ObjectPdg() { m_hists.BinWidthNormDataHists_ObjectPdg(); }


// Efficiency components
void Variable::BinWidthNormEffNumerator()   { m_hists.BinWidthNormEffNumerator();   }
void Variable::BinWidthNormEffDenominator() { m_hists.BinWidthNormEffDenominator(); }


// Migration matrix
void Variable::BinWidthNormMigrationHists() { m_hists.BinWidthNormMigrationHists(); }


// Plastic sidebands in signal region
void Variable::BinWidthNormMCHists_PlasSB_In_SigReg()   { m_hists.BinWidthNormMCHists_PlasSB_In_SigReg();   }
void Variable::BinWidthNormDataHists_PlasSB_In_SigReg() { m_hists.BinWidthNormDataHists_PlasSB_In_SigReg(); }


// Plastic sidebands in physics sidebands
void Variable::BinWidthNormMCHists_PlasSB_In_PhysSB()   { m_hists.BinWidthNormMCHists_PlasSB_In_PhysSB();   }
void Variable::BinWidthNormDataHists_PlasSB_In_PhysSB() { m_hists.BinWidthNormDataHists_PlasSB_In_PhysSB(); }


// Physics sidebands
void Variable::BinWidthNormMCHists_PhysSB()   { m_hists.BinWidthNormMCHists_PhysSB();   }
void Variable::BinWidthNormDataHists_PhysSB() { m_hists.BinWidthNormDataHists_PhysSB(); }





// =======================================================================================
//  SCALE MC HISTOGRAMS
// =======================================================================================

// Event selection
void Variable::ScaleMCHists_Selection(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_Selection(mc_pot, data_pot);
}


// Event selection with material breakdown
void Variable::ScaleMCHists_MatSelection(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_MatSelection(mc_pot, data_pot);
}


// Event selection with interaction type breakdown
void Variable::ScaleMCHists_IntTypeSelection(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_IntTypeSelection(mc_pot, data_pot);
}


// Reco objects with PDG breakdown
void Variable::ScaleMCHists_ObjectPdg(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_ObjectPdg(mc_pot, data_pot);
}


// Efficiency components
void Variable::ScaleEffNumerator(const double mc_pot, const double data_pot) {
    m_hists.ScaleEffNumerator(mc_pot, data_pot);
}

void Variable::ScaleEffDenominator(const double mc_pot, const double data_pot) {
    m_hists.ScaleEffDenominator(mc_pot, data_pot);
}


// Migration matrix
void Variable::ScaleMigrationHists(const double mc_pot, const double data_pot) {
    m_hists.ScaleMigrationHists(mc_pot, data_pot);
}


// Plastic sidebands in signal region
void Variable::ScaleMCHists_PlasSB_In_SigReg(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_PlasSB_In_SigReg(mc_pot, data_pot);
}


// Plastic sidebands in physics sidebands
void Variable::ScaleMCHists_PlasSB_In_PhysSB(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_PlasSB_In_PhysSB(mc_pot, data_pot);
}


// Physics sidebands
void Variable::ScaleMCHists_PhysSB(const double mc_pot, const double data_pot) {
    m_hists.ScaleMCHists_PhysSB(mc_pot, data_pot);
}





// =======================================================================================
//  WRITE HISTOGRAMS TO FILE
// =======================================================================================

// Event selection
void Variable::WriteMCHists_Selection(TFile& fout) const   { m_hists.WriteMCHists_Selection(fout);   }
void Variable::WriteDataHists_Selection(TFile& fout) const { m_hists.WriteDataHists_Selection(fout); }


// Event selection with material breakdown
void Variable::WriteMCHists_MatSelection(TFile& fout) const   { m_hists.WriteMCHists_MatSelection(fout);   }
void Variable::WriteDataHists_MatSelection(TFile& fout) const { m_hists.WriteDataHists_MatSelection(fout); }


// Event selection with interaction type breakdown
void Variable::WriteMCHists_IntTypeSelection(TFile& fout) const   { m_hists.WriteMCHists_IntTypeSelection(fout);   }
void Variable::WriteDataHists_IntTypeSelection(TFile& fout) const { m_hists.WriteDataHists_IntTypeSelection(fout); }


// Reco objects with PDG breakdown
void Variable::WriteMCHists_ObjectPdg(TFile& fout) const   { m_hists.WriteMCHists_ObjectPdg(fout);   }
void Variable::WriteDataHists_ObjectPdg(TFile& fout) const { m_hists.WriteDataHists_ObjectPdg(fout); }


// Efficiency components
void Variable::WriteEffNumerator(TFile& fout) const   { m_hists.WriteEffNumerator(fout);   }
void Variable::WriteEffDenominator(TFile& fout) const { m_hists.WriteEffDenominator(fout); }


// Migration matrix
void Variable::WriteMigrationHists(TFile& fout) const { m_hists.WriteMigrationHists(fout); }


// Plastic sidebands in signal region
void Variable::WriteMCHists_PlasSB_In_SigReg(TFile& fout) const   { m_hists.WriteMCHists_PlasSB_In_SigReg(fout);   }
void Variable::WriteDataHists_PlasSB_In_SigReg(TFile& fout) const { m_hists.WriteDataHists_PlasSB_In_SigReg(fout); }


// Plastic sidebands in physics sidebands
void Variable::WriteMCHists_PlasSB_In_PhysSB(TFile& fout) const   { m_hists.WriteMCHists_PlasSB_In_PhysSB(fout);   }
void Variable::WriteDataHists_PlasSB_In_PhysSB(TFile& fout) const { m_hists.WriteDataHists_PlasSB_In_PhysSB(fout); }


// Physics sidebands
void Variable::WriteMCHists_PhysSB(TFile& fout) const   { m_hists.WriteMCHists_PhysSB(fout);   }
void Variable::WriteDataHists_PhysSB(TFile& fout) const { m_hists.WriteDataHists_PhysSB(fout); }


// MC tuning weights
void Variable::WriteMCWeights_PlasBackgr_In_SigReg(TFile& fout) const { m_hists.WriteMCWeights_PlasBackgr_In_SigReg(fout); }
void Variable::WriteMCWeights_PlasBackgr_In_PhysSB(TFile& fout) const { m_hists.WriteMCWeights_PlasBackgr_In_PhysSB(fout); }
void Variable::WriteMCWeights_PhysBackgr(TFile& fout) const           { m_hists.WriteMCWeights_PhysBackgr(fout);           }





// =======================================================================================
//  LOAD HISTOGRAMS FROM FILE
// =======================================================================================

// Event selection
void Variable::LoadMCHists_Selection(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_Selection(fin, error_bands);
}

void Variable::LoadDataHists_Selection(TFile& fin) {
    m_hists.LoadDataHists_Selection(fin);
}


// Event selection with material breakdown
void Variable::LoadMCHists_MatSelection(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_MatSelection(fin, error_bands);
}

void Variable::LoadDataHists_MatSelection(TFile& fin) {
    m_hists.LoadDataHists_MatSelection(fin);
}


// Event selection with interaction type breakdown
void Variable::LoadMCHists_IntTypeSelection(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_IntTypeSelection(fin, error_bands);
}

void Variable::LoadDataHists_IntTypeSelection(TFile& fin) {
    m_hists.LoadDataHists_IntTypeSelection(fin);
}


// Reco objects with PDG breakdown
void Variable::LoadMCHists_ObjectPdg(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_ObjectPdg(fin, error_bands);
}

void Variable::LoadDataHists_ObjectPdg(TFile& fin) {
    m_hists.LoadDataHists_ObjectPdg(fin);
}


// Efficiency components
void Variable::LoadEffNumerator(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadEffNumerator(fin, error_bands);
}

void Variable::LoadEffDenominator(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadEffDenominator(fin, error_bands);
}


// Migration matrix
void Variable::LoadMigrationHists(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMigrationHists(fin, error_bands);
}


// Plastic sidebands in signal region
void Variable::LoadMCHists_PlasSB_In_SigReg(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_PlasSB_In_SigReg(fin, error_bands);
}

void Variable::LoadDataHists_PlasSB_In_SigReg(TFile& fin) {
    m_hists.LoadDataHists_PlasSB_In_SigReg(fin);
}


// Plastic sidebands in physics sidebands
void Variable::LoadMCHists_PlasSB_In_PhysSB(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_PlasSB_In_PhysSB(fin, error_bands);
}

void Variable::LoadDataHists_PlasSB_In_PhysSB(TFile& fin) {
    m_hists.LoadDataHists_PlasSB_In_PhysSB(fin);
}


// Physics sidebands
void Variable::LoadMCHists_PhysSB(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCHists_PhysSB(fin, error_bands);
}

void Variable::LoadDataHists_PhysSB(TFile& fin) {
    m_hists.LoadDataHists_PhysSB(fin);
}


// MC tuning weights
void Variable::LoadMCWeights_PlasBackgr_In_SigReg(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCWeights_PlasBackgr_In_SigReg(fin, error_bands);
}

void Variable::LoadMCWeights_PlasBackgr_In_PhysSB(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCWeights_PlasBackgr_In_PhysSB(fin, error_bands);
}

void Variable::LoadMCWeights_PhysBackgr(TFile& fin, UniverseMap& error_bands) {
    m_hists.LoadMCWeights_PhysBackgr(fin, error_bands);
}


#endif  // __CINT__


#endif  // Variable_cxx