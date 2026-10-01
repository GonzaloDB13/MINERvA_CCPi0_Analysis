#ifndef Variable2D_cxx
#define Variable2D_cxx

#include <stdlib.h>  // exit()

#include "Variable2D.h"



#ifndef __CINT__  // CINT doesn't know about std::function


// ==========================================================================
//  DEFAULT CONSTRUCTOR
// ==========================================================================

Variable2D::Variable2D()
    : m_label(),
      m_xunits(),
      m_pointer_to_GetValueX(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueX_with_index(&CVUniverse::GetDummyVar1Arg),
      m_yunits(),
      m_pointer_to_GetValueY(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueY_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists2D(),
      m_is_true(false)
{}





// ==========================================================================
//  UNIFORM BIN SIZE CONSTRUCTOR
// ==========================================================================

// With no index in X and Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const int nbinsx, const double xmin, const double xmax,
                       PointerToCVUniverseFunction px,
                       const std::string ylabel,
                       const std::string yunits,
                       const int nbinsy, const double ymin, const double ymax,
                       PointerToCVUniverseFunction py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(px),
      m_pointer_to_GetValueX_with_index(&CVUniverse::GetDummyVar1Arg),
      m_yunits(yunits),
      m_pointer_to_GetValueY(py),
      m_pointer_to_GetValueY_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists2D(m_label, xlabel, nbinsx, xmin, xmax, ylabel, nbinsy, ymin, ymax),
      m_is_true(is_true)
{}


// With index in X
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const int nbinsx, const double xmin, const double xmax,
                       PointerToCVUniverseFunctionWithIndex px,
                       const std::string ylabel,
                       const std::string yunits,
                       const int nbinsy, const double ymin, const double ymax,
                       PointerToCVUniverseFunction py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueX_with_index(px),
      m_yunits(yunits),
      m_pointer_to_GetValueY(py),
      m_pointer_to_GetValueY_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists2D(m_label, xlabel, nbinsx, xmin, xmax, ylabel, nbinsy, ymin, ymax),
      m_is_true(is_true)
{}


// With index in Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const int nbinsx, const double xmin, const double xmax,
                       PointerToCVUniverseFunction px,
                       const std::string ylabel,
                       const std::string yunits,
                       const int nbinsy, const double ymin, const double ymax,
                       PointerToCVUniverseFunctionWithIndex py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(px),
      m_pointer_to_GetValueX_with_index(&CVUniverse::GetDummyVar1Arg),
      m_yunits(yunits),
      m_pointer_to_GetValueY(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueY_with_index(py),
      m_hists2D(m_label, xlabel, nbinsx, xmin, xmax, ylabel, nbinsy, ymin, ymax),
      m_is_true(is_true)
{}


// With index in X and Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const int nbinsx, const double xmin, const double xmax,
                       PointerToCVUniverseFunctionWithIndex px,
                       const std::string ylabel,
                       const std::string yunits,
                       const int nbinsy, const double ymin, const double ymax,
                       PointerToCVUniverseFunctionWithIndex py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueX_with_index(px),
      m_yunits(yunits),
      m_pointer_to_GetValueY(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueY_with_index(py),
      m_hists2D(m_label, xlabel, nbinsx, xmin, xmax, ylabel, nbinsy, ymin, ymax),
      m_is_true(is_true)
{}





// ==========================================================================
//  VARIABLE BIN SIZE CONSTRUCTOR
// ==========================================================================

// With no index in X and Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const TArrayD& binsx_array,
                       PointerToCVUniverseFunction px,
                       const std::string ylabel,
                       const std::string yunits,
                       const TArrayD& binsy_array,
                       PointerToCVUniverseFunction py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(px),
      m_pointer_to_GetValueX_with_index(&CVUniverse::GetDummyVar1Arg),
      m_yunits(yunits),
      m_pointer_to_GetValueY(py),
      m_pointer_to_GetValueY_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists2D(m_label, xlabel, binsx_array, ylabel, binsy_array),
      m_is_true(is_true)
{}


// With index in X
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const TArrayD& binsx_array,
                       PointerToCVUniverseFunctionWithIndex px,
                       const std::string ylabel,
                       const std::string yunits,
                       const TArrayD& binsy_array,
                       PointerToCVUniverseFunction py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueX_with_index(px),
      m_yunits(yunits),
      m_pointer_to_GetValueY(py),
      m_pointer_to_GetValueY_with_index(&CVUniverse::GetDummyVar1Arg),
      m_hists2D(m_label, xlabel, binsx_array, ylabel, binsy_array),
      m_is_true(is_true)
{}


// With index in Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const TArrayD& binsx_array,
                       PointerToCVUniverseFunction px,
                       const std::string ylabel,
                       const std::string yunits,
                       const TArrayD& binsy_array,
                       PointerToCVUniverseFunctionWithIndex py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(px),
      m_pointer_to_GetValueX_with_index(&CVUniverse::GetDummyVar1Arg),
      m_yunits(yunits),
      m_pointer_to_GetValueY(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueY_with_index(py),
      m_hists2D(m_label, xlabel, binsx_array, ylabel, binsy_array),
      m_is_true(is_true)
{}


// With index in X and Y
Variable2D::Variable2D(const std::string label,
                       const std::string xlabel,
                       const std::string xunits,
                       const TArrayD& binsx_array,
                       PointerToCVUniverseFunctionWithIndex px,
                       const std::string ylabel,
                       const std::string yunits,
                       const TArrayD& binsy_array,
                       PointerToCVUniverseFunctionWithIndex py,
                       const bool is_true)
    : m_label(label),
      m_xunits(xunits),
      m_pointer_to_GetValueX(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueX_with_index(px),
      m_yunits(yunits),
      m_pointer_to_GetValueY(&CVUniverse::GetDummyVar),
      m_pointer_to_GetValueY_with_index(py),
      m_hists2D(m_label, xlabel, binsx_array, ylabel, binsy_array),
      m_is_true(is_true)
{}





// ==========================================================================
//  GET VARIABLE VALUE
// ==========================================================================

// With no index
double Variable2D::GetValueX(const CVUniverse& universe) const {
    return m_pointer_to_GetValueX(universe);
}

double Variable2D::GetValueY(const CVUniverse& universe) const {
    return m_pointer_to_GetValueY(universe);
}


// Using index
double Variable2D::GetValueX(const CVUniverse& universe, const int index) const {
    return m_pointer_to_GetValueX_with_index(universe, index);
}

double Variable2D::GetValueY(const CVUniverse& universe, const int index) const {
    return m_pointer_to_GetValueY_with_index(universe, index);
}





// =======================================================================================
//  INITIALIZE HISTOGRAMS
// =======================================================================================

// Event selection
// ===============

// Monte Carlo
template<typename T>
void Variable2D::InitMCHists_Selection2D(T systematic_univs) {
    m_hists2D.InitMCHists_Selection2D(systematic_univs);
}


// Data
void Variable2D::InitDataHists_Selection2D() {
    m_hists2D.InitDataHists_Selection2D();
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
template<typename T>
void Variable2D::InitMCHists_MatSelection2D(T systematic_univs) {
    m_hists2D.InitMCHists_MatSelection2D(systematic_univs);
}


// Data
void Variable2D::InitDataHists_MatSelection2D() {
    m_hists2D.InitDataHists_MatSelection2D();
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
template<typename T>
void Variable2D::InitMCHists_ObjectPdg2D(T systematic_univs) {
    m_hists2D.InitMCHists_ObjectPdg2D(systematic_univs);
}


// Data
void Variable2D::InitDataHists_ObjectPdg2D() {
    m_hists2D.InitDataHists_ObjectPdg2D();
}





// =======================================================================================
//  SYNCHRONIZE MC CV HISTOGRAMS
// =======================================================================================

// Event selection
void Variable2D::SyncMCHists_Selection2D() { m_hists2D.SyncMCHists_Selection2D(); }


// Event selection with material breakdown
void Variable2D::SyncMCHists_MatSelection2D() { m_hists2D.SyncMCHists_MatSelection2D(); }


// Reco objects with PDG breakdown
void Variable2D::SyncMCHists_ObjectPdg2D() { m_hists2D.SyncMCHists_ObjectPdg2D(); }





// =======================================================================================
//  BIN WIDTH NORMALIZE HISTOGRAMS
// =======================================================================================

// Event selection
void Variable2D::BinWidthNormMCHists_Selection2D()   { m_hists2D.BinWidthNormMCHists_Selection2D();   }
void Variable2D::BinWidthNormDataHists_Selection2D() { m_hists2D.BinWidthNormDataHists_Selection2D(); }


// Event selection with material breakdown
void Variable2D::BinWidthNormMCHists_MatSelection2D()   { m_hists2D.BinWidthNormMCHists_MatSelection2D();   }
void Variable2D::BinWidthNormDataHists_MatSelection2D() { m_hists2D.BinWidthNormDataHists_MatSelection2D(); }


// Reco objects with PDG breakdown
void Variable2D::BinWidthNormMCHists_ObjectPdg2D()   { m_hists2D.BinWidthNormMCHists_ObjectPdg2D();   }
void Variable2D::BinWidthNormDataHists_ObjectPdg2D() { m_hists2D.BinWidthNormDataHists_ObjectPdg2D(); }





// =======================================================================================
//  SCALE MC HISTOGRAMS
// =======================================================================================

// Event selection
void Variable2D::ScaleMCHists_Selection2D(const double mc_pot, const double data_pot) {
    m_hists2D.ScaleMCHists_Selection2D(mc_pot, data_pot);
}


// Event selection with material breakdown
void Variable2D::ScaleMCHists_MatSelection2D(const double mc_pot, const double data_pot) {
    m_hists2D.ScaleMCHists_MatSelection2D(mc_pot, data_pot);
}


// Reco objects with PDG breakdown
void Variable2D::ScaleMCHists_ObjectPdg2D(const double mc_pot, const double data_pot) {
    m_hists2D.ScaleMCHists_ObjectPdg2D(mc_pot, data_pot);
}





// =======================================================================================
//  WRITE HISTOGRAMS TO FILE
// =======================================================================================

// Event selection
void Variable2D::WriteMCHists_Selection2D(TFile& fout) const   { m_hists2D.WriteMCHists_Selection2D(fout);   }
void Variable2D::WriteDataHists_Selection2D(TFile& fout) const { m_hists2D.WriteDataHists_Selection2D(fout); }


// Event selection with material breakdown
void Variable2D::WriteMCHists_MatSelection2D(TFile& fout) const   { m_hists2D.WriteMCHists_MatSelection2D(fout);   }
void Variable2D::WriteDataHists_MatSelection2D(TFile& fout) const { m_hists2D.WriteDataHists_MatSelection2D(fout); }


// Reco objects with PDG breakdown
void Variable2D::WriteMCHists_ObjectPdg2D(TFile& fout) const   { m_hists2D.WriteMCHists_ObjectPdg2D(fout);   }
void Variable2D::WriteDataHists_ObjectPdg2D(TFile& fout) const { m_hists2D.WriteDataHists_ObjectPdg2D(fout); }





// =======================================================================================
//  LOAD HISTOGRAMS FROM FILE
// =======================================================================================

// Event selection
void Variable2D::LoadMCHists_Selection2D(TFile& fin, UniverseMap& error_bands) {
    m_hists2D.LoadMCHists_Selection2D(fin, error_bands);
}

void Variable2D::LoadDataHists_Selection2D(TFile& fin) {
    m_hists2D.LoadDataHists_Selection2D(fin);
}


// Event selection with material breakdown
void Variable2D::LoadMCHists_MatSelection2D(TFile& fin, UniverseMap& error_bands) {
    m_hists2D.LoadMCHists_MatSelection2D(fin, error_bands);
}

void Variable2D::LoadDataHists_MatSelection2D(TFile& fin) {
    m_hists2D.LoadDataHists_MatSelection2D(fin);
}


// Reco objects with PDG breakdown
void Variable2D::LoadMCHists_ObjectPdg2D(TFile& fin, UniverseMap& error_bands) {
    m_hists2D.LoadMCHists_ObjectPdg2D(fin, error_bands);
}

void Variable2D::LoadDataHists_ObjectPdg2D(TFile& fin) {
    m_hists2D.LoadDataHists_ObjectPdg2D(fin);
}


#endif  // __CINT__


#endif  // Variable2D_cxx