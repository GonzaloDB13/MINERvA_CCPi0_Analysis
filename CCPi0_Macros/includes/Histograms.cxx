#ifndef Histograms_cxx
#define Histograms_cxx

#include <algorithm>

#include "Histograms.h"



// ==========================================================================
//  DEFAULT CONSTRUCTOR
// ==========================================================================

Histograms::Histograms()
    : m_label(),
      m_xlabel(),
      m_bins_array(0),
      m_bins_vector(0),
      
      // Event selection
      m_data_Selection(), m_mc_Selection(),
      m_mc_Selection_Signal(), m_mc_Selection_Backgr(),
      m_mc_Selection_BackgrPi0HighW(), m_mc_Selection_BackgrQElike(), m_mc_Selection_BackgrPionProd(),
      m_mc_Selection_BackgrPlasUp(), m_mc_Selection_BackgrPlasBetw(), m_mc_Selection_BackgrPlasDown(), m_mc_Selection_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection(), m_mc_MatSelection(),
      m_mc_MatSelection_TrueTgt4Pb(), m_mc_MatSelection_TrueTgt5Pb(), m_mc_MatSelection_TrueTgt5Fe(),
      m_mc_MatSelection_TruePlasUp(), m_mc_MatSelection_TruePlasBetw(), m_mc_MatSelection_TruePlasDown(), m_mc_MatSelection_TrueOtherMat(),
      
      // Event selection with interaction type breakdown
      m_data_IntTypeSelection(), m_mc_IntTypeSelection(),
      m_mc_IntTypeSelection_QE(), m_mc_IntTypeSelection_MEC(), m_mc_IntTypeSelection_DeltaRES(), m_mc_IntTypeSelection_OtherRES(),
      m_mc_IntTypeSelection_SoftDIS(), m_mc_IntTypeSelection_TrueDIS(), m_mc_IntTypeSelection_Other(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg(), m_mc_ObjectPdg(),
      m_mc_ObjectPdg_Pi0(), m_mc_ObjectPdg_Proton(), m_mc_ObjectPdg_Neutron(), m_mc_ObjectPdg_Pion(), m_mc_ObjectPdg_EM(),
      m_mc_ObjectPdg_Muon(), m_mc_ObjectPdg_OthPdg(), m_mc_ObjectPdg_MCXtalk(),m_mc_ObjectPdg_Overlay(),
      
      // Efficiency components
      m_mc_EffNumerator(),
      m_mc_EffNumerator_QE(), m_mc_EffNumerator_MEC(), m_mc_EffNumerator_DeltaRES(), m_mc_EffNumerator_OtherRES(),
      m_mc_EffNumerator_SoftDIS(), m_mc_EffNumerator_TrueDIS(), m_mc_EffNumerator_Other(),
      m_mc_EffDenominator(),
      m_mc_EffDenominator_QE(), m_mc_EffDenominator_MEC(), m_mc_EffDenominator_DeltaRES(), m_mc_EffDenominator_OtherRES(),
      m_mc_EffDenominator_SoftDIS(), m_mc_EffDenominator_TrueDIS(), m_mc_EffDenominator_Other(),
      
      // Migration matrices
      m_mc_Migration(),
      
      // Plastic sidebands in physics signal region
      /* RecoPb: */m_data_RecoPb_In_SigReg(), m_mc_RecoPb_In_SigReg(),
      m_mc_RecoPb_In_SigReg_TrueTgt4Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoPb_In_SigReg_TruePlasUp(), m_mc_RecoPb_In_SigReg_TruePlasBetw(), m_mc_RecoPb_In_SigReg_TruePlasDown(),
      m_mc_RecoPb_In_SigReg_TrueOtherMat(),
      /* RecoFe: */m_data_RecoFe_In_SigReg(), m_mc_RecoFe_In_SigReg(),
      m_mc_RecoFe_In_SigReg_TrueTgt4Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoFe_In_SigReg_TruePlasUp(), m_mc_RecoFe_In_SigReg_TruePlasBetw(), m_mc_RecoFe_In_SigReg_TruePlasDown(),
      m_mc_RecoFe_In_SigReg_TrueOtherMat(),
      /* PlasUpSB: */m_data_PlasUpSB_In_SigReg(), m_mc_PlasUpSB_In_SigReg(),
      m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_SigReg_TruePlasUp(), m_mc_PlasUpSB_In_SigReg_TruePlasBetw(), m_mc_PlasUpSB_In_SigReg_TruePlasDown(),
      m_mc_PlasUpSB_In_SigReg_TrueOtherMat(),
      /* PlasBetwSB: */m_data_PlasBetwSB_In_SigReg(), m_mc_PlasBetwSB_In_SigReg(),
      m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_SigReg_TruePlasUp(), m_mc_PlasBetwSB_In_SigReg_TruePlasBetw(), m_mc_PlasBetwSB_In_SigReg_TruePlasDown(),
      m_mc_PlasBetwSB_In_SigReg_TrueOtherMat(),
      /* PlasDownSB: */m_data_PlasDownSB_In_SigReg(), m_mc_PlasDownSB_In_SigReg(),
      m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_SigReg_TruePlasUp(), m_mc_PlasDownSB_In_SigReg_TruePlasBetw(), m_mc_PlasDownSB_In_SigReg_TruePlasDown(),
      m_mc_PlasDownSB_In_SigReg_TrueOtherMat(),
      
      // Plastic sidebands in physics sidebands
      /* PionBlobSB: */m_data_PlasUpSB_In_PionBlobSB(), m_mc_PlasUpSB_In_PionBlobSB(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_PionBlobSB(), m_mc_PlasBetwSB_In_PionBlobSB(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_PionBlobSB(), m_mc_PlasDownSB_In_PionBlobSB(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat(),
      /* ProtonBlobSB: */m_data_PlasUpSB_In_ProtonBlobSB(), m_mc_PlasUpSB_In_ProtonBlobSB(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_ProtonBlobSB(), m_mc_PlasBetwSB_In_ProtonBlobSB(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_ProtonBlobSB(), m_mc_PlasDownSB_In_ProtonBlobSB(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat(),
      /* HighWSB: */m_data_PlasUpSB_In_HighWSB(), m_mc_PlasUpSB_In_HighWSB(),
      m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_HighWSB_TruePlasUp(), m_mc_PlasUpSB_In_HighWSB_TruePlasBetw(), m_mc_PlasUpSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasUpSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_HighWSB(), m_mc_PlasBetwSB_In_HighWSB(),
      m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_HighWSB_TruePlasUp(), m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw(), m_mc_PlasBetwSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasDownSB_In_HighWSB(), m_mc_PlasDownSB_In_HighWSB(),
      m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_HighWSB_TruePlasUp(), m_mc_PlasDownSB_In_HighWSB_TruePlasBetw(), m_mc_PlasDownSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasDownSB_In_HighWSB_TrueOtherMat(),
      
      // Physics sidebands
      /* SigReg: */m_data_SigReg(), m_mc_SigReg(),
      m_mc_SigReg_Signal(), m_mc_SigReg_BackgrPi0HighW(), m_mc_SigReg_BackgrQElike(), m_mc_SigReg_BackgrPionProd(),
      m_mc_SigReg_BackgrPlasUp(), m_mc_SigReg_BackgrPlasBetw(), m_mc_SigReg_BackgrPlasDown(),
      m_mc_SigReg_BackgrOther(),
      /* PionBlobSB: */m_data_PionBlobSB(), m_mc_PionBlobSB(),
      m_mc_PionBlobSB_Signal(), m_mc_PionBlobSB_BackgrPi0HighW(), m_mc_PionBlobSB_BackgrQElike(), m_mc_PionBlobSB_BackgrPionProd(),
      m_mc_PionBlobSB_BackgrPlasUp(), m_mc_PionBlobSB_BackgrPlasBetw(), m_mc_PionBlobSB_BackgrPlasDown(),
      m_mc_PionBlobSB_BackgrOther(),
      /* ProtonBlobSB: */m_data_ProtonBlobSB(), m_mc_ProtonBlobSB(),
      m_mc_ProtonBlobSB_Signal(), m_mc_ProtonBlobSB_BackgrPi0HighW(), m_mc_ProtonBlobSB_BackgrQElike(), m_mc_ProtonBlobSB_BackgrPionProd(),
      m_mc_ProtonBlobSB_BackgrPlasUp(), m_mc_ProtonBlobSB_BackgrPlasBetw(), m_mc_ProtonBlobSB_BackgrPlasDown(),
      m_mc_ProtonBlobSB_BackgrOther(),
      /* HighWSB: */m_data_HighWSB(), m_mc_HighWSB(),
      m_mc_HighWSB_Signal(), m_mc_HighWSB_BackgrPi0HighW(), m_mc_HighWSB_BackgrQElike(), m_mc_HighWSB_BackgrPionProd(),
      m_mc_HighWSB_BackgrPlasUp(), m_mc_HighWSB_BackgrPlasBetw(), m_mc_HighWSB_BackgrPlasDown(),
      m_mc_HighWSB_BackgrOther(),
      
      // MC weights
      m_mc_Weight_SigReg_TruePlasUp(), m_mc_Weight_SigReg_TruePlasBetw(), m_mc_Weight_SigReg_TruePlasDown(),
      m_mc_Weight_PionBlobSB_TruePlasUp(), m_mc_Weight_PionBlobSB_TruePlasBetw(), m_mc_Weight_PionBlobSB_TruePlasDown(),
      m_mc_Weight_HighWSB_TruePlasUp(), m_mc_Weight_HighWSB_TruePlasBetw(), m_mc_Weight_HighWSB_TruePlasDown(),
      m_mc_Weight_ProtonBlobSB_TruePlasUp(), m_mc_Weight_ProtonBlobSB_TruePlasBetw(), m_mc_Weight_ProtonBlobSB_TruePlasDown(),
      m_mc_Weight_Signal(), m_mc_Weight_BackgrPi0HighW(), m_mc_Weight_BackgrQElike(), m_mc_Weight_BackgrPionProd(),
      
      // Before cross-section extraction
      m_data(),
      m_mc_Signal(),
      m_mc_BackgrNonTuned(), m_mc_BackgrNonTuned_Pi0HighW(), m_mc_BackgrNonTuned_QElike(), m_mc_BackgrNonTuned_PionProd(),
      m_mc_BackgrNonTuned_PlasUp(), m_mc_BackgrNonTuned_PlasBetw(), m_mc_BackgrNonTuned_PlasDown(), m_mc_BackgrNonTuned_Other(),
      m_mc_BackgrTuned(), m_mc_BackgrTuned_Pi0HighW(), m_mc_BackgrTuned_QElike(), m_mc_BackgrTuned_PionProd(),
      m_mc_BackgrTuned_PlasUp(), m_mc_BackgrTuned_PlasBetw(), m_mc_BackgrTuned_PlasDown(), m_mc_BackgrTuned_Other(),
      m_EffNumerator(), m_EffDenominator(), m_Efficiency(),
      m_MigrationMatrix(),
      
      // Later-stage cross section calculation
      m_mc_BackgrSubtr(), m_data_BackgrSubtr(),
      m_mc_Folded(), m_data_Folded(), m_mc_Unfolded(), m_data_Unfolded(),
      m_mc_EffCorrected(), m_data_EffCorrected(),
      m_mc_CrossSection(), m_data_CrossSection()
{}





// ==========================================================================
//  UNIFORM BIN SIZE CONSTRUCTOR
// ==========================================================================

Histograms::Histograms(const std::string label,
                       const std::string xlabel,
                       const int nbins, const double xmin, const double xmax)
    : m_label(label),
      m_xlabel(xlabel),
      m_bins_array(MakeUniformBinArray(nbins, xmin, xmax)),
      m_bins_vector(GetVecFromArray(m_bins_array)),
      
      // Event selection
      m_data_Selection(), m_mc_Selection(),
      m_mc_Selection_Signal(), m_mc_Selection_Backgr(),
      m_mc_Selection_BackgrPi0HighW(), m_mc_Selection_BackgrQElike(), m_mc_Selection_BackgrPionProd(),
      m_mc_Selection_BackgrPlasUp(), m_mc_Selection_BackgrPlasBetw(), m_mc_Selection_BackgrPlasDown(), m_mc_Selection_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection(), m_mc_MatSelection(),
      m_mc_MatSelection_TrueTgt4Pb(), m_mc_MatSelection_TrueTgt5Pb(), m_mc_MatSelection_TrueTgt5Fe(),
      m_mc_MatSelection_TruePlasUp(), m_mc_MatSelection_TruePlasBetw(), m_mc_MatSelection_TruePlasDown(), m_mc_MatSelection_TrueOtherMat(),
      
      // Event selection with interaction type breakdown
      m_data_IntTypeSelection(), m_mc_IntTypeSelection(),
      m_mc_IntTypeSelection_QE(), m_mc_IntTypeSelection_MEC(), m_mc_IntTypeSelection_DeltaRES(), m_mc_IntTypeSelection_OtherRES(),
      m_mc_IntTypeSelection_SoftDIS(), m_mc_IntTypeSelection_TrueDIS(), m_mc_IntTypeSelection_Other(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg(), m_mc_ObjectPdg(),
      m_mc_ObjectPdg_Pi0(), m_mc_ObjectPdg_Proton(), m_mc_ObjectPdg_Neutron(), m_mc_ObjectPdg_Pion(), m_mc_ObjectPdg_EM(),
      m_mc_ObjectPdg_Muon(), m_mc_ObjectPdg_OthPdg(), m_mc_ObjectPdg_MCXtalk(),m_mc_ObjectPdg_Overlay(),
      
      // Efficiency components
      m_mc_EffNumerator(),
      m_mc_EffNumerator_QE(), m_mc_EffNumerator_MEC(), m_mc_EffNumerator_DeltaRES(), m_mc_EffNumerator_OtherRES(),
      m_mc_EffNumerator_SoftDIS(), m_mc_EffNumerator_TrueDIS(), m_mc_EffNumerator_Other(),
      m_mc_EffDenominator(),
      m_mc_EffDenominator_QE(), m_mc_EffDenominator_MEC(), m_mc_EffDenominator_DeltaRES(), m_mc_EffDenominator_OtherRES(),
      m_mc_EffDenominator_SoftDIS(), m_mc_EffDenominator_TrueDIS(), m_mc_EffDenominator_Other(),
      
      // Migration matrices
      m_mc_Migration(),
      
      // Plastic sidebands in physics signal region
      /* RecoPb: */m_data_RecoPb_In_SigReg(), m_mc_RecoPb_In_SigReg(),
      m_mc_RecoPb_In_SigReg_TrueTgt4Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoPb_In_SigReg_TruePlasUp(), m_mc_RecoPb_In_SigReg_TruePlasBetw(), m_mc_RecoPb_In_SigReg_TruePlasDown(),
      m_mc_RecoPb_In_SigReg_TrueOtherMat(),
      /* RecoFe: */m_data_RecoFe_In_SigReg(), m_mc_RecoFe_In_SigReg(),
      m_mc_RecoFe_In_SigReg_TrueTgt4Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoFe_In_SigReg_TruePlasUp(), m_mc_RecoFe_In_SigReg_TruePlasBetw(), m_mc_RecoFe_In_SigReg_TruePlasDown(),
      m_mc_RecoFe_In_SigReg_TrueOtherMat(),
      /* PlasUpSB: */m_data_PlasUpSB_In_SigReg(), m_mc_PlasUpSB_In_SigReg(),
      m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_SigReg_TruePlasUp(), m_mc_PlasUpSB_In_SigReg_TruePlasBetw(), m_mc_PlasUpSB_In_SigReg_TruePlasDown(),
      m_mc_PlasUpSB_In_SigReg_TrueOtherMat(),
      /* PlasBetwSB: */m_data_PlasBetwSB_In_SigReg(), m_mc_PlasBetwSB_In_SigReg(),
      m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_SigReg_TruePlasUp(), m_mc_PlasBetwSB_In_SigReg_TruePlasBetw(), m_mc_PlasBetwSB_In_SigReg_TruePlasDown(),
      m_mc_PlasBetwSB_In_SigReg_TrueOtherMat(),
      /* PlasDownSB: */m_data_PlasDownSB_In_SigReg(), m_mc_PlasDownSB_In_SigReg(),
      m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_SigReg_TruePlasUp(), m_mc_PlasDownSB_In_SigReg_TruePlasBetw(), m_mc_PlasDownSB_In_SigReg_TruePlasDown(),
      m_mc_PlasDownSB_In_SigReg_TrueOtherMat(),
      
      // Plastic sidebands in physics sidebands
      /* PionBlobSB: */m_data_PlasUpSB_In_PionBlobSB(), m_mc_PlasUpSB_In_PionBlobSB(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_PionBlobSB(), m_mc_PlasBetwSB_In_PionBlobSB(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_PionBlobSB(), m_mc_PlasDownSB_In_PionBlobSB(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat(),
      /* ProtonBlobSB: */m_data_PlasUpSB_In_ProtonBlobSB(), m_mc_PlasUpSB_In_ProtonBlobSB(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_ProtonBlobSB(), m_mc_PlasBetwSB_In_ProtonBlobSB(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_ProtonBlobSB(), m_mc_PlasDownSB_In_ProtonBlobSB(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat(),
      /* HighWSB: */m_data_PlasUpSB_In_HighWSB(), m_mc_PlasUpSB_In_HighWSB(),
      m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_HighWSB_TruePlasUp(), m_mc_PlasUpSB_In_HighWSB_TruePlasBetw(), m_mc_PlasUpSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasUpSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_HighWSB(), m_mc_PlasBetwSB_In_HighWSB(),
      m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_HighWSB_TruePlasUp(), m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw(), m_mc_PlasBetwSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasDownSB_In_HighWSB(), m_mc_PlasDownSB_In_HighWSB(),
      m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_HighWSB_TruePlasUp(), m_mc_PlasDownSB_In_HighWSB_TruePlasBetw(), m_mc_PlasDownSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasDownSB_In_HighWSB_TrueOtherMat(),
      
      // Physics sidebands
      /* SigReg: */m_data_SigReg(), m_mc_SigReg(),
      m_mc_SigReg_Signal(), m_mc_SigReg_BackgrPi0HighW(), m_mc_SigReg_BackgrQElike(), m_mc_SigReg_BackgrPionProd(),
      m_mc_SigReg_BackgrPlasUp(), m_mc_SigReg_BackgrPlasBetw(), m_mc_SigReg_BackgrPlasDown(),
      m_mc_SigReg_BackgrOther(),
      /* PionBlobSB: */m_data_PionBlobSB(), m_mc_PionBlobSB(),
      m_mc_PionBlobSB_Signal(), m_mc_PionBlobSB_BackgrPi0HighW(), m_mc_PionBlobSB_BackgrQElike(), m_mc_PionBlobSB_BackgrPionProd(),
      m_mc_PionBlobSB_BackgrPlasUp(), m_mc_PionBlobSB_BackgrPlasBetw(), m_mc_PionBlobSB_BackgrPlasDown(),
      m_mc_PionBlobSB_BackgrOther(),
      /* ProtonBlobSB: */m_data_ProtonBlobSB(), m_mc_ProtonBlobSB(),
      m_mc_ProtonBlobSB_Signal(), m_mc_ProtonBlobSB_BackgrPi0HighW(), m_mc_ProtonBlobSB_BackgrQElike(), m_mc_ProtonBlobSB_BackgrPionProd(),
      m_mc_ProtonBlobSB_BackgrPlasUp(), m_mc_ProtonBlobSB_BackgrPlasBetw(), m_mc_ProtonBlobSB_BackgrPlasDown(),
      m_mc_ProtonBlobSB_BackgrOther(),
      /* HighWSB: */m_data_HighWSB(), m_mc_HighWSB(),
      m_mc_HighWSB_Signal(), m_mc_HighWSB_BackgrPi0HighW(), m_mc_HighWSB_BackgrQElike(), m_mc_HighWSB_BackgrPionProd(),
      m_mc_HighWSB_BackgrPlasUp(), m_mc_HighWSB_BackgrPlasBetw(), m_mc_HighWSB_BackgrPlasDown(),
      m_mc_HighWSB_BackgrOther(),
      
      // MC weights
      m_mc_Weight_SigReg_TruePlasUp(), m_mc_Weight_SigReg_TruePlasBetw(), m_mc_Weight_SigReg_TruePlasDown(),
      m_mc_Weight_PionBlobSB_TruePlasUp(), m_mc_Weight_PionBlobSB_TruePlasBetw(), m_mc_Weight_PionBlobSB_TruePlasDown(),
      m_mc_Weight_HighWSB_TruePlasUp(), m_mc_Weight_HighWSB_TruePlasBetw(), m_mc_Weight_HighWSB_TruePlasDown(),
      m_mc_Weight_ProtonBlobSB_TruePlasUp(), m_mc_Weight_ProtonBlobSB_TruePlasBetw(), m_mc_Weight_ProtonBlobSB_TruePlasDown(),
      m_mc_Weight_Signal(), m_mc_Weight_BackgrPi0HighW(), m_mc_Weight_BackgrQElike(), m_mc_Weight_BackgrPionProd(),
      
      // Before cross-section extraction
      m_data(),
      m_mc_Signal(),
      m_mc_BackgrNonTuned(), m_mc_BackgrNonTuned_Pi0HighW(), m_mc_BackgrNonTuned_QElike(), m_mc_BackgrNonTuned_PionProd(),
      m_mc_BackgrNonTuned_PlasUp(), m_mc_BackgrNonTuned_PlasBetw(), m_mc_BackgrNonTuned_PlasDown(), m_mc_BackgrNonTuned_Other(),
      m_mc_BackgrTuned(), m_mc_BackgrTuned_Pi0HighW(), m_mc_BackgrTuned_QElike(), m_mc_BackgrTuned_PionProd(),
      m_mc_BackgrTuned_PlasUp(), m_mc_BackgrTuned_PlasBetw(), m_mc_BackgrTuned_PlasDown(), m_mc_BackgrTuned_Other(),
      m_EffNumerator(), m_EffDenominator(), m_Efficiency(),
      m_MigrationMatrix(),
      
      // Later-stage cross section calculation
      m_mc_BackgrSubtr(), m_data_BackgrSubtr(),
      m_mc_Folded(), m_data_Folded(), m_mc_Unfolded(), m_data_Unfolded(),
      m_mc_EffCorrected(), m_data_EffCorrected(),
      m_mc_CrossSection(), m_data_CrossSection()
{}





// ==========================================================================
//  VARIABLE BIN SIZE CONSTRUCTOR
// ==========================================================================

Histograms::Histograms(const std::string label,
                       const std::string xlabel,
                       const TArrayD& bins_array)
    : m_label(label),
      m_xlabel(xlabel),
      m_bins_array(GetSortedArray(bins_array)),
      m_bins_vector(GetVecFromArray(m_bins_array)),
      
      // Event selection
      m_data_Selection(), m_mc_Selection(),
      m_mc_Selection_Signal(), m_mc_Selection_Backgr(),
      m_mc_Selection_BackgrPi0HighW(), m_mc_Selection_BackgrQElike(), m_mc_Selection_BackgrPionProd(),
      m_mc_Selection_BackgrPlasUp(), m_mc_Selection_BackgrPlasBetw(), m_mc_Selection_BackgrPlasDown(), m_mc_Selection_BackgrOther(),
      
      // Event selection with material breakdown
      m_data_MatSelection(), m_mc_MatSelection(),
      m_mc_MatSelection_TrueTgt4Pb(), m_mc_MatSelection_TrueTgt5Pb(), m_mc_MatSelection_TrueTgt5Fe(),
      m_mc_MatSelection_TruePlasUp(), m_mc_MatSelection_TruePlasBetw(), m_mc_MatSelection_TruePlasDown(), m_mc_MatSelection_TrueOtherMat(),
      
      // Event selection with interaction type breakdown
      m_data_IntTypeSelection(), m_mc_IntTypeSelection(),
      m_mc_IntTypeSelection_QE(), m_mc_IntTypeSelection_MEC(), m_mc_IntTypeSelection_DeltaRES(), m_mc_IntTypeSelection_OtherRES(),
      m_mc_IntTypeSelection_SoftDIS(), m_mc_IntTypeSelection_TrueDIS(), m_mc_IntTypeSelection_Other(),
      
      // Reconstruction objects with PDG breakdown
      m_data_ObjectPdg(), m_mc_ObjectPdg(),
      m_mc_ObjectPdg_Pi0(), m_mc_ObjectPdg_Proton(), m_mc_ObjectPdg_Neutron(), m_mc_ObjectPdg_Pion(), m_mc_ObjectPdg_EM(),
      m_mc_ObjectPdg_Muon(), m_mc_ObjectPdg_OthPdg(), m_mc_ObjectPdg_MCXtalk(),m_mc_ObjectPdg_Overlay(),
      
      // Efficiency components
      m_mc_EffNumerator(),
      m_mc_EffNumerator_QE(), m_mc_EffNumerator_MEC(), m_mc_EffNumerator_DeltaRES(), m_mc_EffNumerator_OtherRES(),
      m_mc_EffNumerator_SoftDIS(), m_mc_EffNumerator_TrueDIS(), m_mc_EffNumerator_Other(),
      m_mc_EffDenominator(),
      m_mc_EffDenominator_QE(), m_mc_EffDenominator_MEC(), m_mc_EffDenominator_DeltaRES(), m_mc_EffDenominator_OtherRES(),
      m_mc_EffDenominator_SoftDIS(), m_mc_EffDenominator_TrueDIS(), m_mc_EffDenominator_Other(),
      
      // Migration matrices
      m_mc_Migration(),
      
      // Plastic sidebands in physics signal region
      /* RecoPb: */m_data_RecoPb_In_SigReg(), m_mc_RecoPb_In_SigReg(),
      m_mc_RecoPb_In_SigReg_TrueTgt4Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Pb(), m_mc_RecoPb_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoPb_In_SigReg_TruePlasUp(), m_mc_RecoPb_In_SigReg_TruePlasBetw(), m_mc_RecoPb_In_SigReg_TruePlasDown(),
      m_mc_RecoPb_In_SigReg_TrueOtherMat(),
      /* RecoFe: */m_data_RecoFe_In_SigReg(), m_mc_RecoFe_In_SigReg(),
      m_mc_RecoFe_In_SigReg_TrueTgt4Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Pb(), m_mc_RecoFe_In_SigReg_TrueTgt5Fe(),
      m_mc_RecoFe_In_SigReg_TruePlasUp(), m_mc_RecoFe_In_SigReg_TruePlasBetw(), m_mc_RecoFe_In_SigReg_TruePlasDown(),
      m_mc_RecoFe_In_SigReg_TrueOtherMat(),
      /* PlasUpSB: */m_data_PlasUpSB_In_SigReg(), m_mc_PlasUpSB_In_SigReg(),
      m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_SigReg_TruePlasUp(), m_mc_PlasUpSB_In_SigReg_TruePlasBetw(), m_mc_PlasUpSB_In_SigReg_TruePlasDown(),
      m_mc_PlasUpSB_In_SigReg_TrueOtherMat(),
      /* PlasBetwSB: */m_data_PlasBetwSB_In_SigReg(), m_mc_PlasBetwSB_In_SigReg(),
      m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_SigReg_TruePlasUp(), m_mc_PlasBetwSB_In_SigReg_TruePlasBetw(), m_mc_PlasBetwSB_In_SigReg_TruePlasDown(),
      m_mc_PlasBetwSB_In_SigReg_TrueOtherMat(),
      /* PlasDownSB: */m_data_PlasDownSB_In_SigReg(), m_mc_PlasDownSB_In_SigReg(),
      m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb(), m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_SigReg_TruePlasUp(), m_mc_PlasDownSB_In_SigReg_TruePlasBetw(), m_mc_PlasDownSB_In_SigReg_TruePlasDown(),
      m_mc_PlasDownSB_In_SigReg_TrueOtherMat(),
      
      // Plastic sidebands in physics sidebands
      /* PionBlobSB: */m_data_PlasUpSB_In_PionBlobSB(), m_mc_PlasUpSB_In_PionBlobSB(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_PionBlobSB(), m_mc_PlasBetwSB_In_PionBlobSB(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_PionBlobSB(), m_mc_PlasDownSB_In_PionBlobSB(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat(),
      /* ProtonBlobSB: */m_data_PlasUpSB_In_ProtonBlobSB(), m_mc_PlasUpSB_In_ProtonBlobSB(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_ProtonBlobSB(), m_mc_PlasBetwSB_In_ProtonBlobSB(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat(),
      m_data_PlasDownSB_In_ProtonBlobSB(), m_mc_PlasDownSB_In_ProtonBlobSB(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw(), m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown(),
      m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat(),
      /* HighWSB: */m_data_PlasUpSB_In_HighWSB(), m_mc_PlasUpSB_In_HighWSB(),
      m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasUpSB_In_HighWSB_TruePlasUp(), m_mc_PlasUpSB_In_HighWSB_TruePlasBetw(), m_mc_PlasUpSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasUpSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasBetwSB_In_HighWSB(), m_mc_PlasBetwSB_In_HighWSB(),
      m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasBetwSB_In_HighWSB_TruePlasUp(), m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw(), m_mc_PlasBetwSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat(),
      m_data_PlasDownSB_In_HighWSB(), m_mc_PlasDownSB_In_HighWSB(),
      m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb(), m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe(),
      m_mc_PlasDownSB_In_HighWSB_TruePlasUp(), m_mc_PlasDownSB_In_HighWSB_TruePlasBetw(), m_mc_PlasDownSB_In_HighWSB_TruePlasDown(),
      m_mc_PlasDownSB_In_HighWSB_TrueOtherMat(),
      
      // Physics sidebands
      /* SigReg: */m_data_SigReg(), m_mc_SigReg(),
      m_mc_SigReg_Signal(), m_mc_SigReg_BackgrPi0HighW(), m_mc_SigReg_BackgrQElike(), m_mc_SigReg_BackgrPionProd(),
      m_mc_SigReg_BackgrPlasUp(), m_mc_SigReg_BackgrPlasBetw(), m_mc_SigReg_BackgrPlasDown(),
      m_mc_SigReg_BackgrOther(),
      /* PionBlobSB: */m_data_PionBlobSB(), m_mc_PionBlobSB(),
      m_mc_PionBlobSB_Signal(), m_mc_PionBlobSB_BackgrPi0HighW(), m_mc_PionBlobSB_BackgrQElike(), m_mc_PionBlobSB_BackgrPionProd(),
      m_mc_PionBlobSB_BackgrPlasUp(), m_mc_PionBlobSB_BackgrPlasBetw(), m_mc_PionBlobSB_BackgrPlasDown(),
      m_mc_PionBlobSB_BackgrOther(),
      /* ProtonBlobSB: */m_data_ProtonBlobSB(), m_mc_ProtonBlobSB(),
      m_mc_ProtonBlobSB_Signal(), m_mc_ProtonBlobSB_BackgrPi0HighW(), m_mc_ProtonBlobSB_BackgrQElike(), m_mc_ProtonBlobSB_BackgrPionProd(),
      m_mc_ProtonBlobSB_BackgrPlasUp(), m_mc_ProtonBlobSB_BackgrPlasBetw(), m_mc_ProtonBlobSB_BackgrPlasDown(),
      m_mc_ProtonBlobSB_BackgrOther(),
      /* HighWSB: */m_data_HighWSB(), m_mc_HighWSB(),
      m_mc_HighWSB_Signal(), m_mc_HighWSB_BackgrPi0HighW(), m_mc_HighWSB_BackgrQElike(), m_mc_HighWSB_BackgrPionProd(),
      m_mc_HighWSB_BackgrPlasUp(), m_mc_HighWSB_BackgrPlasBetw(), m_mc_HighWSB_BackgrPlasDown(),
      m_mc_HighWSB_BackgrOther(),
      
      // MC weights
      m_mc_Weight_SigReg_TruePlasUp(), m_mc_Weight_SigReg_TruePlasBetw(), m_mc_Weight_SigReg_TruePlasDown(),
      m_mc_Weight_PionBlobSB_TruePlasUp(), m_mc_Weight_PionBlobSB_TruePlasBetw(), m_mc_Weight_PionBlobSB_TruePlasDown(),
      m_mc_Weight_HighWSB_TruePlasUp(), m_mc_Weight_HighWSB_TruePlasBetw(), m_mc_Weight_HighWSB_TruePlasDown(),
      m_mc_Weight_ProtonBlobSB_TruePlasUp(), m_mc_Weight_ProtonBlobSB_TruePlasBetw(), m_mc_Weight_ProtonBlobSB_TruePlasDown(),
      m_mc_Weight_Signal(), m_mc_Weight_BackgrPi0HighW(), m_mc_Weight_BackgrQElike(), m_mc_Weight_BackgrPionProd(),
      
      // Before cross-section extraction
      m_data(),
      m_mc_Signal(),
      m_mc_BackgrNonTuned(), m_mc_BackgrNonTuned_Pi0HighW(), m_mc_BackgrNonTuned_QElike(), m_mc_BackgrNonTuned_PionProd(),
      m_mc_BackgrNonTuned_PlasUp(), m_mc_BackgrNonTuned_PlasBetw(), m_mc_BackgrNonTuned_PlasDown(), m_mc_BackgrNonTuned_Other(),
      m_mc_BackgrTuned(), m_mc_BackgrTuned_Pi0HighW(), m_mc_BackgrTuned_QElike(), m_mc_BackgrTuned_PionProd(),
      m_mc_BackgrTuned_PlasUp(), m_mc_BackgrTuned_PlasBetw(), m_mc_BackgrTuned_PlasDown(), m_mc_BackgrTuned_Other(),
      m_EffNumerator(), m_EffDenominator(), m_Efficiency(),
      m_MigrationMatrix(),
      
      // Later-stage cross section calculation
      m_mc_BackgrSubtr(), m_data_BackgrSubtr(),
      m_mc_Folded(), m_data_Folded(), m_mc_Unfolded(), m_data_Unfolded(),
      m_mc_EffCorrected(), m_data_EffCorrected(),
      m_mc_CrossSection(), m_data_CrossSection()
{}





// ==========================================================================
//  INITIALIZE SELECTION HISTOGRAMS
// ==========================================================================

// Monte Carlo
// ===========
template<typename T>
void Histograms::InitMCHists_Selection(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_Selection        = new MH1D(Form("mc_Selection_%s",        label), label, NBins(), bins);
    MH1D* mc_Selection_Signal = new MH1D(Form("mc_Selection_%s_Signal", label), label, NBins(), bins);
    MH1D* mc_Selection_Backgr = new MH1D(Form("mc_Selection_%s_Backgr", label), label, NBins(), bins);
    
    MH1D* mc_Selection_BackgrPi0HighW = new MH1D(Form("mc_Selection_%s_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrQElike   = new MH1D(Form("mc_Selection_%s_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrPionProd = new MH1D(Form("mc_Selection_%s_BackgrPionProd", label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrPlasUp   = new MH1D(Form("mc_Selection_%s_BackgrPlasUp",   label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrPlasBetw = new MH1D(Form("mc_Selection_%s_BackgrPlasBetw", label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrPlasDown = new MH1D(Form("mc_Selection_%s_BackgrPlasDown", label), label, NBins(), bins);
    MH1D* mc_Selection_BackgrOther    = new MH1D(Form("mc_Selection_%s_BackgrOther",    label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Selection        = CVHW(mc_Selection,        systematic_univs, clear_bands);
    m_mc_Selection_Signal = CVHW(mc_Selection_Signal, systematic_univs, clear_bands);
    m_mc_Selection_Backgr = CVHW(mc_Selection_Backgr, systematic_univs, clear_bands);
    
    m_mc_Selection_BackgrPi0HighW = CVHW(mc_Selection_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_Selection_BackgrQElike   = CVHW(mc_Selection_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_Selection_BackgrPionProd = CVHW(mc_Selection_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_Selection_BackgrPlasUp   = CVHW(mc_Selection_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_Selection_BackgrPlasBetw = CVHW(mc_Selection_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_Selection_BackgrPlasDown = CVHW(mc_Selection_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_Selection_BackgrOther    = CVHW(mc_Selection_BackgrOther,    systematic_univs, clear_bands);
    
    delete mc_Selection;
    delete mc_Selection_Signal;
    delete mc_Selection_Backgr;
    delete mc_Selection_BackgrPi0HighW;
    delete mc_Selection_BackgrQElike;
    delete mc_Selection_BackgrPionProd;
    delete mc_Selection_BackgrPlasUp;
    delete mc_Selection_BackgrPlasBetw;
    delete mc_Selection_BackgrPlasDown;
    delete mc_Selection_BackgrOther;
}



// Data
// ====
void Histograms::InitDataHists_Selection()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_Selection = new MH1D(Form("data_Selection_%s", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE SELECTION HISTOGRAMS WITH MATERIAL BREAKDOWN
// ==========================================================================

// Monte Carlo
// ===========
template<typename T>
void Histograms::InitMCHists_MatSelection(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_MatSelection = new MH1D(Form("mc_MatSelection_%s", label), label, NBins(), bins);
    
    MH1D* mc_MatSelection_TrueTgt4Pb   = new MH1D(Form("mc_MatSelection_%s_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_MatSelection_TrueTgt5Pb   = new MH1D(Form("mc_MatSelection_%s_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_MatSelection_TrueTgt5Fe   = new MH1D(Form("mc_MatSelection_%s_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_MatSelection_TruePlasUp   = new MH1D(Form("mc_MatSelection_%s_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_MatSelection_TruePlasBetw = new MH1D(Form("mc_MatSelection_%s_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_MatSelection_TruePlasDown = new MH1D(Form("mc_MatSelection_%s_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_MatSelection_TrueOtherMat = new MH1D(Form("mc_MatSelection_%s_TrueOtherMat", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_MatSelection = CVHW(mc_MatSelection, systematic_univs, clear_bands);
    
    m_mc_MatSelection_TrueTgt4Pb   = CVHW(mc_MatSelection_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_MatSelection_TrueTgt5Pb   = CVHW(mc_MatSelection_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_MatSelection_TrueTgt5Fe   = CVHW(mc_MatSelection_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_MatSelection_TruePlasUp   = CVHW(mc_MatSelection_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_MatSelection_TruePlasBetw = CVHW(mc_MatSelection_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_MatSelection_TruePlasDown = CVHW(mc_MatSelection_TruePlasDown, systematic_univs, clear_bands);
    m_mc_MatSelection_TrueOtherMat = CVHW(mc_MatSelection_TrueOtherMat, systematic_univs, clear_bands);
    
    delete mc_MatSelection;
    delete mc_MatSelection_TrueTgt4Pb;
    delete mc_MatSelection_TrueTgt5Pb;
    delete mc_MatSelection_TrueTgt5Fe;
    delete mc_MatSelection_TruePlasUp;
    delete mc_MatSelection_TruePlasBetw;
    delete mc_MatSelection_TruePlasDown;
    delete mc_MatSelection_TrueOtherMat;
}



// Data
// ====
void Histograms::InitDataHists_MatSelection()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_MatSelection = new MH1D(Form("data_MatSelection_%s", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE SELECTION HISTOGRAMS WITH INTERACTION TYPE BREAKDOWN
// ==========================================================================

// Monte Carlo
// ===========
template<typename T>
void Histograms::InitMCHists_IntTypeSelection(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_IntTypeSelection = new MH1D(Form("mc_IntTypeSelection_%s", label), label, NBins(), bins);
    
    MH1D* mc_IntTypeSelection_QE       = new MH1D(Form("mc_IntTypeSelection_%s_QE",       label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_MEC      = new MH1D(Form("mc_IntTypeSelection_%s_MEC",      label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_DeltaRES = new MH1D(Form("mc_IntTypeSelection_%s_DeltaRES", label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_OtherRES = new MH1D(Form("mc_IntTypeSelection_%s_OtherRES", label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_SoftDIS  = new MH1D(Form("mc_IntTypeSelection_%s_SoftDIS",  label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_TrueDIS  = new MH1D(Form("mc_IntTypeSelection_%s_TrueDIS",  label), label, NBins(), bins);
    MH1D* mc_IntTypeSelection_Other    = new MH1D(Form("mc_IntTypeSelection_%s_Other",    label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_IntTypeSelection = CVHW(mc_IntTypeSelection, systematic_univs, clear_bands);
    
    m_mc_IntTypeSelection_QE       = CVHW(mc_IntTypeSelection_QE,       systematic_univs, clear_bands);
    m_mc_IntTypeSelection_MEC      = CVHW(mc_IntTypeSelection_MEC,      systematic_univs, clear_bands);
    m_mc_IntTypeSelection_DeltaRES = CVHW(mc_IntTypeSelection_DeltaRES, systematic_univs, clear_bands);
    m_mc_IntTypeSelection_OtherRES = CVHW(mc_IntTypeSelection_OtherRES, systematic_univs, clear_bands);
    m_mc_IntTypeSelection_SoftDIS  = CVHW(mc_IntTypeSelection_SoftDIS,  systematic_univs, clear_bands);
    m_mc_IntTypeSelection_TrueDIS  = CVHW(mc_IntTypeSelection_TrueDIS,  systematic_univs, clear_bands);
    m_mc_IntTypeSelection_Other    = CVHW(mc_IntTypeSelection_Other,    systematic_univs, clear_bands);
    
    delete mc_IntTypeSelection;
    delete mc_IntTypeSelection_QE;
    delete mc_IntTypeSelection_MEC;
    delete mc_IntTypeSelection_DeltaRES;
    delete mc_IntTypeSelection_OtherRES;
    delete mc_IntTypeSelection_SoftDIS;
    delete mc_IntTypeSelection_TrueDIS;
    delete mc_IntTypeSelection_Other;
}



// Data
// ====
void Histograms::InitDataHists_IntTypeSelection()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_IntTypeSelection = new MH1D(Form("data_IntTypeSelection_%s", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE RECO OBJECT HISTOGRAMS WITH PDG BREAKDOWN
// ==========================================================================

// Monte Carlo
// ===========
template<typename T>
void Histograms::InitMCHists_ObjectPdg(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_ObjectPdg = new MH1D(Form("mc_ObjectPdg_%s", label), label, NBins(), bins);
    
    MH1D* mc_ObjectPdg_Pi0     = new MH1D(Form("mc_ObjectPdg_%s_Pi0",     label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_Proton  = new MH1D(Form("mc_ObjectPdg_%s_Proton",  label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_Neutron = new MH1D(Form("mc_ObjectPdg_%s_Neutron", label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_Pion    = new MH1D(Form("mc_ObjectPdg_%s_Pion",    label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_EM      = new MH1D(Form("mc_ObjectPdg_%s_EM",      label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_Muon    = new MH1D(Form("mc_ObjectPdg_%s_Muon",    label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_OthPdg  = new MH1D(Form("mc_ObjectPdg_%s_OthPdg",  label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_MCXtalk = new MH1D(Form("mc_ObjectPdg_%s_MCXtalk", label), label, NBins(), bins);
    MH1D* mc_ObjectPdg_Overlay = new MH1D(Form("mc_ObjectPdg_%s_Overlay", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_ObjectPdg = CVHW(mc_ObjectPdg, systematic_univs, clear_bands);
    
    m_mc_ObjectPdg_Pi0     = CVHW(mc_ObjectPdg_Pi0,     systematic_univs, clear_bands);
    m_mc_ObjectPdg_Proton  = CVHW(mc_ObjectPdg_Proton,  systematic_univs, clear_bands);
    m_mc_ObjectPdg_Neutron = CVHW(mc_ObjectPdg_Neutron, systematic_univs, clear_bands);
    m_mc_ObjectPdg_Pion    = CVHW(mc_ObjectPdg_Pion,    systematic_univs, clear_bands);
    m_mc_ObjectPdg_EM      = CVHW(mc_ObjectPdg_EM,      systematic_univs, clear_bands);
    m_mc_ObjectPdg_Muon    = CVHW(mc_ObjectPdg_Muon,    systematic_univs, clear_bands);
    m_mc_ObjectPdg_OthPdg  = CVHW(mc_ObjectPdg_OthPdg,  systematic_univs, clear_bands);
    m_mc_ObjectPdg_MCXtalk = CVHW(mc_ObjectPdg_MCXtalk, systematic_univs, clear_bands);
    m_mc_ObjectPdg_Overlay = CVHW(mc_ObjectPdg_Overlay, systematic_univs, clear_bands);
    
    delete mc_ObjectPdg;
    delete mc_ObjectPdg_Pi0;
    delete mc_ObjectPdg_Proton;
    delete mc_ObjectPdg_Neutron;
    delete mc_ObjectPdg_Pion;
    delete mc_ObjectPdg_EM;
    delete mc_ObjectPdg_Muon;
    delete mc_ObjectPdg_OthPdg;
    delete mc_ObjectPdg_MCXtalk;
    delete mc_ObjectPdg_Overlay;
}



// Data
// ====
void Histograms::InitDataHists_ObjectPdg()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_ObjectPdg = new MH1D(Form("data_ObjectPdg_%s", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE EFFICIENCY COMPONENT HISTOGRAMS
// ==========================================================================

// Numerator
// =========
template<typename T>
void Histograms::InitEffNumerator(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histogram
    MH1D* mc_EffNumerator          = new MH1D(Form("mc_EffNumerator_%s",          label), label, NBins(), bins);
    MH1D* mc_EffNumerator_QE       = new MH1D(Form("mc_EffNumerator_%s_QE",       label), label, NBins(), bins);
    MH1D* mc_EffNumerator_MEC      = new MH1D(Form("mc_EffNumerator_%s_MEC",      label), label, NBins(), bins);
    MH1D* mc_EffNumerator_DeltaRES = new MH1D(Form("mc_EffNumerator_%s_DeltaRES", label), label, NBins(), bins);
    MH1D* mc_EffNumerator_OtherRES = new MH1D(Form("mc_EffNumerator_%s_OtherRES", label), label, NBins(), bins);
    MH1D* mc_EffNumerator_SoftDIS  = new MH1D(Form("mc_EffNumerator_%s_SoftDIS",  label), label, NBins(), bins);
    MH1D* mc_EffNumerator_TrueDIS  = new MH1D(Form("mc_EffNumerator_%s_TrueDIS",  label), label, NBins(), bins);
    MH1D* mc_EffNumerator_Other    = new MH1D(Form("mc_EffNumerator_%s_Other",    label), label, NBins(), bins);
    
    
    // Assign member histogram and delete dummy
    const bool clear_bands = true;
    m_mc_EffNumerator          = CVHW(mc_EffNumerator,          systematic_univs, clear_bands);
    m_mc_EffNumerator_QE       = CVHW(mc_EffNumerator_QE,       systematic_univs, clear_bands);
    m_mc_EffNumerator_MEC      = CVHW(mc_EffNumerator_MEC,      systematic_univs, clear_bands);
    m_mc_EffNumerator_DeltaRES = CVHW(mc_EffNumerator_DeltaRES, systematic_univs, clear_bands);
    m_mc_EffNumerator_OtherRES = CVHW(mc_EffNumerator_OtherRES, systematic_univs, clear_bands);
    m_mc_EffNumerator_SoftDIS  = CVHW(mc_EffNumerator_SoftDIS,  systematic_univs, clear_bands);
    m_mc_EffNumerator_TrueDIS  = CVHW(mc_EffNumerator_TrueDIS,  systematic_univs, clear_bands);
    m_mc_EffNumerator_Other    = CVHW(mc_EffNumerator_Other,    systematic_univs, clear_bands);
    
    delete mc_EffNumerator;
    delete mc_EffNumerator_QE;
    delete mc_EffNumerator_MEC;
    delete mc_EffNumerator_DeltaRES;
    delete mc_EffNumerator_OtherRES;
    delete mc_EffNumerator_SoftDIS;
    delete mc_EffNumerator_TrueDIS;
    delete mc_EffNumerator_Other;
}



// Denominator
// ===========
template<typename T>
void Histograms::InitEffDenominator(T systematic_univs_truth)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histogram
    MH1D* mc_EffDenominator          = new MH1D(Form("mc_EffDenominator_%s",          label), label, NBins(), bins);
    MH1D* mc_EffDenominator_QE       = new MH1D(Form("mc_EffDenominator_%s_QE",       label), label, NBins(), bins);
    MH1D* mc_EffDenominator_MEC      = new MH1D(Form("mc_EffDenominator_%s_MEC",      label), label, NBins(), bins);
    MH1D* mc_EffDenominator_DeltaRES = new MH1D(Form("mc_EffDenominator_%s_DeltaRES", label), label, NBins(), bins);
    MH1D* mc_EffDenominator_OtherRES = new MH1D(Form("mc_EffDenominator_%s_OtherRES", label), label, NBins(), bins);
    MH1D* mc_EffDenominator_SoftDIS  = new MH1D(Form("mc_EffDenominator_%s_SoftDIS",  label), label, NBins(), bins);
    MH1D* mc_EffDenominator_TrueDIS  = new MH1D(Form("mc_EffDenominator_%s_TrueDIS",  label), label, NBins(), bins);
    MH1D* mc_EffDenominator_Other    = new MH1D(Form("mc_EffDenominator_%s_Other",    label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    m_mc_EffDenominator          = CVHW(mc_EffDenominator,          systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_QE       = CVHW(mc_EffDenominator_QE,       systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_MEC      = CVHW(mc_EffDenominator_MEC,      systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_DeltaRES = CVHW(mc_EffDenominator_DeltaRES, systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_OtherRES = CVHW(mc_EffDenominator_OtherRES, systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_SoftDIS  = CVHW(mc_EffDenominator_SoftDIS,  systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_TrueDIS  = CVHW(mc_EffDenominator_TrueDIS,  systematic_univs_truth, clear_bands);
    m_mc_EffDenominator_Other    = CVHW(mc_EffDenominator_Other,    systematic_univs_truth, clear_bands);
    
    delete mc_EffDenominator;
    delete mc_EffDenominator_QE;
    delete mc_EffDenominator_MEC;
    delete mc_EffDenominator_DeltaRES;
    delete mc_EffDenominator_OtherRES;
    delete mc_EffDenominator_SoftDIS;
    delete mc_EffDenominator_TrueDIS;
    delete mc_EffDenominator_Other;
}





// ==========================================================================
//  INITIALIZE MIGRATION HISTOGRAMS
// ==========================================================================

template<typename T>
void Histograms::InitMigrationHists(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH2D* migration = new MH2D(Form("mc_Migration_%s", label), label, NBins(), bins, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Migration = CVH2DW(migration, systematic_univs, clear_bands);
    
    delete migration;
}





// ==========================================================================
//  INITIALIZE PLASTIC SIDEBAND HISTOGRAMS IN SIGNAL REGION
// ==========================================================================

// Monte Carlo
// ===========
template <typename T>
void Histograms::InitMCHists_PlasSB_In_SigReg(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_RecoPb_In_SigReg              = new MH1D(Form("mc_%s_RecoPb_In_SigReg",              label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TrueTgt4Pb   = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TrueTgt5Pb   = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TrueTgt5Fe   = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TruePlasUp   = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TruePlasBetw = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TruePlasDown = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_RecoPb_In_SigReg_TrueOtherMat = new MH1D(Form("mc_%s_RecoPb_In_SigReg_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_RecoFe_In_SigReg              = new MH1D(Form("mc_%s_RecoFe_In_SigReg",              label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TrueTgt4Pb   = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TrueTgt5Pb   = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TrueTgt5Fe   = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TruePlasUp   = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TruePlasBetw = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TruePlasDown = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_RecoFe_In_SigReg_TrueOtherMat = new MH1D(Form("mc_%s_RecoFe_In_SigReg_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasUpSB_In_SigReg              = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg",              label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TruePlasUp   = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TruePlasBetw = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TruePlasDown = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_SigReg_TrueOtherMat = new MH1D(Form("mc_%s_PlasUpSB_In_SigReg_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasBetwSB_In_SigReg              = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg",              label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TruePlasUp   = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TruePlasBetw = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TruePlasDown = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_SigReg_TrueOtherMat = new MH1D(Form("mc_%s_PlasBetwSB_In_SigReg_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasDownSB_In_SigReg              = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg",              label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TruePlasUp   = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TruePlasBetw = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TruePlasDown = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_SigReg_TrueOtherMat = new MH1D(Form("mc_%s_PlasDownSB_In_SigReg_TrueOtherMat", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_RecoPb_In_SigReg              = CVHW(mc_RecoPb_In_SigReg,              systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb   = CVHW(mc_RecoPb_In_SigReg_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb   = CVHW(mc_RecoPb_In_SigReg_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe   = CVHW(mc_RecoPb_In_SigReg_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TruePlasUp   = CVHW(mc_RecoPb_In_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TruePlasBetw = CVHW(mc_RecoPb_In_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TruePlasDown = CVHW(mc_RecoPb_In_SigReg_TruePlasDown, systematic_univs, clear_bands);
    m_mc_RecoPb_In_SigReg_TrueOtherMat = CVHW(mc_RecoPb_In_SigReg_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_RecoFe_In_SigReg              = CVHW(mc_RecoFe_In_SigReg,              systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb   = CVHW(mc_RecoFe_In_SigReg_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb   = CVHW(mc_RecoFe_In_SigReg_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe   = CVHW(mc_RecoFe_In_SigReg_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TruePlasUp   = CVHW(mc_RecoFe_In_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TruePlasBetw = CVHW(mc_RecoFe_In_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TruePlasDown = CVHW(mc_RecoFe_In_SigReg_TruePlasDown, systematic_univs, clear_bands);
    m_mc_RecoFe_In_SigReg_TrueOtherMat = CVHW(mc_RecoFe_In_SigReg_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasUpSB_In_SigReg              = CVHW(mc_PlasUpSB_In_SigReg,              systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb   = CVHW(mc_PlasUpSB_In_SigReg_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb   = CVHW(mc_PlasUpSB_In_SigReg_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe   = CVHW(mc_PlasUpSB_In_SigReg_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TruePlasUp   = CVHW(mc_PlasUpSB_In_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw = CVHW(mc_PlasUpSB_In_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TruePlasDown = CVHW(mc_PlasUpSB_In_SigReg_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat = CVHW(mc_PlasUpSB_In_SigReg_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasBetwSB_In_SigReg              = CVHW(mc_PlasBetwSB_In_SigReg,              systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb   = CVHW(mc_PlasBetwSB_In_SigReg_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb   = CVHW(mc_PlasBetwSB_In_SigReg_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe   = CVHW(mc_PlasBetwSB_In_SigReg_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp   = CVHW(mc_PlasBetwSB_In_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw = CVHW(mc_PlasBetwSB_In_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown = CVHW(mc_PlasBetwSB_In_SigReg_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat = CVHW(mc_PlasBetwSB_In_SigReg_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasDownSB_In_SigReg              = CVHW(mc_PlasDownSB_In_SigReg,              systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb   = CVHW(mc_PlasDownSB_In_SigReg_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb   = CVHW(mc_PlasDownSB_In_SigReg_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe   = CVHW(mc_PlasDownSB_In_SigReg_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TruePlasUp   = CVHW(mc_PlasDownSB_In_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw = CVHW(mc_PlasDownSB_In_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TruePlasDown = CVHW(mc_PlasDownSB_In_SigReg_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat = CVHW(mc_PlasDownSB_In_SigReg_TrueOtherMat, systematic_univs, clear_bands);
    
    delete mc_RecoPb_In_SigReg;
    delete mc_RecoPb_In_SigReg_TrueTgt4Pb;
    delete mc_RecoPb_In_SigReg_TrueTgt5Pb;
    delete mc_RecoPb_In_SigReg_TrueTgt5Fe;
    delete mc_RecoPb_In_SigReg_TruePlasUp;
    delete mc_RecoPb_In_SigReg_TruePlasBetw;
    delete mc_RecoPb_In_SigReg_TruePlasDown;
    delete mc_RecoPb_In_SigReg_TrueOtherMat;
    
    delete mc_RecoFe_In_SigReg;
    delete mc_RecoFe_In_SigReg_TrueTgt4Pb;
    delete mc_RecoFe_In_SigReg_TrueTgt5Pb;
    delete mc_RecoFe_In_SigReg_TrueTgt5Fe;
    delete mc_RecoFe_In_SigReg_TruePlasUp;
    delete mc_RecoFe_In_SigReg_TruePlasBetw;
    delete mc_RecoFe_In_SigReg_TruePlasDown;
    delete mc_RecoFe_In_SigReg_TrueOtherMat;
    
    delete mc_PlasUpSB_In_SigReg;
    delete mc_PlasUpSB_In_SigReg_TrueTgt4Pb;
    delete mc_PlasUpSB_In_SigReg_TrueTgt5Pb;
    delete mc_PlasUpSB_In_SigReg_TrueTgt5Fe;
    delete mc_PlasUpSB_In_SigReg_TruePlasUp;
    delete mc_PlasUpSB_In_SigReg_TruePlasBetw;
    delete mc_PlasUpSB_In_SigReg_TruePlasDown;
    delete mc_PlasUpSB_In_SigReg_TrueOtherMat;
    
    delete mc_PlasBetwSB_In_SigReg;
    delete mc_PlasBetwSB_In_SigReg_TrueTgt4Pb;
    delete mc_PlasBetwSB_In_SigReg_TrueTgt5Pb;
    delete mc_PlasBetwSB_In_SigReg_TrueTgt5Fe;
    delete mc_PlasBetwSB_In_SigReg_TruePlasUp;
    delete mc_PlasBetwSB_In_SigReg_TruePlasBetw;
    delete mc_PlasBetwSB_In_SigReg_TruePlasDown;
    delete mc_PlasBetwSB_In_SigReg_TrueOtherMat;
    
    delete mc_PlasDownSB_In_SigReg;
    delete mc_PlasDownSB_In_SigReg_TrueTgt4Pb;
    delete mc_PlasDownSB_In_SigReg_TrueTgt5Pb;
    delete mc_PlasDownSB_In_SigReg_TrueTgt5Fe;
    delete mc_PlasDownSB_In_SigReg_TruePlasUp;
    delete mc_PlasDownSB_In_SigReg_TruePlasBetw;
    delete mc_PlasDownSB_In_SigReg_TruePlasDown;
    delete mc_PlasDownSB_In_SigReg_TrueOtherMat;
}



// Data
// ====
void Histograms::InitDataHists_PlasSB_In_SigReg()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_RecoPb_In_SigReg     = new MH1D(Form("data_%s_RecoPb_In_SigReg",     label), label, NBins(), bins);
    m_data_RecoFe_In_SigReg     = new MH1D(Form("data_%s_RecoFe_In_SigReg",     label), label, NBins(), bins);
    m_data_PlasUpSB_In_SigReg   = new MH1D(Form("data_%s_PlasUpSB_In_SigReg",   label), label, NBins(), bins);
    m_data_PlasBetwSB_In_SigReg = new MH1D(Form("data_%s_PlasBetwSB_In_SigReg", label), label, NBins(), bins);
    m_data_PlasDownSB_In_SigReg = new MH1D(Form("data_%s_PlasDownSB_In_SigReg", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE PLASTIC SIDEBAND HISTOGRAMS IN PHYSICS SIDEBANDS
// ==========================================================================

// Monte Carlo
// ===========
template <typename T>
void Histograms::InitMCHists_PlasSB_In_PhysSB(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_PlasUpSB_In_PionBlobSB              = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_PionBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasUpSB_In_PionBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasBetwSB_In_PionBlobSB              = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasBetwSB_In_PionBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasDownSB_In_PionBlobSB              = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_PionBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasDownSB_In_PionBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasUpSB_In_ProtonBlobSB              = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasUpSB_In_ProtonBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB              = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasDownSB_In_ProtonBlobSB              = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB",              label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasDownSB_In_ProtonBlobSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasUpSB_In_HighWSB              = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB",              label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TruePlasDown = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasUpSB_In_HighWSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasUpSB_In_HighWSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasBetwSB_In_HighWSB              = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB",              label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TruePlasDown = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasBetwSB_In_HighWSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasBetwSB_In_HighWSB_TrueOtherMat", label), label, NBins(), bins);
    
    MH1D* mc_PlasDownSB_In_HighWSB              = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB",              label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TrueTgt4Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TrueTgt4Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TrueTgt5Pb   = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TrueTgt5Pb",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TrueTgt5Fe   = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TrueTgt5Fe",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TruePlasUp   = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TruePlasBetw = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TruePlasDown = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TruePlasDown", label), label, NBins(), bins);
    MH1D* mc_PlasDownSB_In_HighWSB_TrueOtherMat = new MH1D(Form("mc_%s_PlasDownSB_In_HighWSB_TrueOtherMat", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_PlasUpSB_In_PionBlobSB              = CVHW(mc_PlasUpSB_In_PionBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb   = CVHW(mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb   = CVHW(mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe   = CVHW(mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp   = CVHW(mc_PlasUpSB_In_PionBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw = CVHW(mc_PlasUpSB_In_PionBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown = CVHW(mc_PlasUpSB_In_PionBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat = CVHW(mc_PlasUpSB_In_PionBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasBetwSB_In_PionBlobSB              = CVHW(mc_PlasBetwSB_In_PionBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb   = CVHW(mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb   = CVHW(mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe   = CVHW(mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp   = CVHW(mc_PlasBetwSB_In_PionBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw = CVHW(mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown = CVHW(mc_PlasBetwSB_In_PionBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat = CVHW(mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasDownSB_In_PionBlobSB              = CVHW(mc_PlasDownSB_In_PionBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb   = CVHW(mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb   = CVHW(mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe   = CVHW(mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp   = CVHW(mc_PlasDownSB_In_PionBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw = CVHW(mc_PlasDownSB_In_PionBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown = CVHW(mc_PlasDownSB_In_PionBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat = CVHW(mc_PlasDownSB_In_PionBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasUpSB_In_ProtonBlobSB              = CVHW(mc_PlasUpSB_In_ProtonBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb   = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb   = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe   = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp   = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat = CVHW(mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasBetwSB_In_ProtonBlobSB              = CVHW(mc_PlasBetwSB_In_ProtonBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb   = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb   = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe   = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp   = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat = CVHW(mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasDownSB_In_ProtonBlobSB              = CVHW(mc_PlasDownSB_In_ProtonBlobSB,              systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb   = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb   = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe   = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp   = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat = CVHW(mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasUpSB_In_HighWSB              = CVHW(mc_PlasUpSB_In_HighWSB,              systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb   = CVHW(mc_PlasUpSB_In_HighWSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb   = CVHW(mc_PlasUpSB_In_HighWSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe   = CVHW(mc_PlasUpSB_In_HighWSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp   = CVHW(mc_PlasUpSB_In_HighWSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw = CVHW(mc_PlasUpSB_In_HighWSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown = CVHW(mc_PlasUpSB_In_HighWSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat = CVHW(mc_PlasUpSB_In_HighWSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasBetwSB_In_HighWSB              = CVHW(mc_PlasBetwSB_In_HighWSB,              systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb   = CVHW(mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb   = CVHW(mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe   = CVHW(mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp   = CVHW(mc_PlasBetwSB_In_HighWSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw = CVHW(mc_PlasBetwSB_In_HighWSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown = CVHW(mc_PlasBetwSB_In_HighWSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat = CVHW(mc_PlasBetwSB_In_HighWSB_TrueOtherMat, systematic_univs, clear_bands);
    
    m_mc_PlasDownSB_In_HighWSB              = CVHW(mc_PlasDownSB_In_HighWSB,              systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb   = CVHW(mc_PlasDownSB_In_HighWSB_TrueTgt4Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb   = CVHW(mc_PlasDownSB_In_HighWSB_TrueTgt5Pb,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe   = CVHW(mc_PlasDownSB_In_HighWSB_TrueTgt5Fe,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp   = CVHW(mc_PlasDownSB_In_HighWSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw = CVHW(mc_PlasDownSB_In_HighWSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown = CVHW(mc_PlasDownSB_In_HighWSB_TruePlasDown, systematic_univs, clear_bands);
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat = CVHW(mc_PlasDownSB_In_HighWSB_TrueOtherMat, systematic_univs, clear_bands);
    
    delete mc_PlasUpSB_In_PionBlobSB;
    delete mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb;
    delete mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb;
    delete mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe;
    delete mc_PlasUpSB_In_PionBlobSB_TruePlasUp;
    delete mc_PlasUpSB_In_PionBlobSB_TruePlasBetw;
    delete mc_PlasUpSB_In_PionBlobSB_TruePlasDown;
    delete mc_PlasUpSB_In_PionBlobSB_TrueOtherMat;
    
    delete mc_PlasBetwSB_In_PionBlobSB;
    delete mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb;
    delete mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb;
    delete mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe;
    delete mc_PlasBetwSB_In_PionBlobSB_TruePlasUp;
    delete mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw;
    delete mc_PlasBetwSB_In_PionBlobSB_TruePlasDown;
    delete mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat;
    
    delete mc_PlasDownSB_In_PionBlobSB;
    delete mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb;
    delete mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb;
    delete mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe;
    delete mc_PlasDownSB_In_PionBlobSB_TruePlasUp;
    delete mc_PlasDownSB_In_PionBlobSB_TruePlasBetw;
    delete mc_PlasDownSB_In_PionBlobSB_TruePlasDown;
    delete mc_PlasDownSB_In_PionBlobSB_TrueOtherMat;
    
    delete mc_PlasUpSB_In_ProtonBlobSB;
    delete mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb;
    delete mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb;
    delete mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe;
    delete mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp;
    delete mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw;
    delete mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown;
    delete mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat;
    
    delete mc_PlasBetwSB_In_ProtonBlobSB;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown;
    delete mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat;
    
    delete mc_PlasDownSB_In_ProtonBlobSB;
    delete mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb;
    delete mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb;
    delete mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe;
    delete mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp;
    delete mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw;
    delete mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown;
    delete mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat;
    
    delete mc_PlasUpSB_In_HighWSB;
    delete mc_PlasUpSB_In_HighWSB_TrueTgt4Pb;
    delete mc_PlasUpSB_In_HighWSB_TrueTgt5Pb;
    delete mc_PlasUpSB_In_HighWSB_TrueTgt5Fe;
    delete mc_PlasUpSB_In_HighWSB_TruePlasUp;
    delete mc_PlasUpSB_In_HighWSB_TruePlasBetw;
    delete mc_PlasUpSB_In_HighWSB_TruePlasDown;
    delete mc_PlasUpSB_In_HighWSB_TrueOtherMat;
    
    delete mc_PlasBetwSB_In_HighWSB;
    delete mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb;
    delete mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb;
    delete mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe;
    delete mc_PlasBetwSB_In_HighWSB_TruePlasUp;
    delete mc_PlasBetwSB_In_HighWSB_TruePlasBetw;
    delete mc_PlasBetwSB_In_HighWSB_TruePlasDown;
    delete mc_PlasBetwSB_In_HighWSB_TrueOtherMat;
    
    delete mc_PlasDownSB_In_HighWSB;
    delete mc_PlasDownSB_In_HighWSB_TrueTgt4Pb;
    delete mc_PlasDownSB_In_HighWSB_TrueTgt5Pb;
    delete mc_PlasDownSB_In_HighWSB_TrueTgt5Fe;
    delete mc_PlasDownSB_In_HighWSB_TruePlasUp;
    delete mc_PlasDownSB_In_HighWSB_TruePlasBetw;
    delete mc_PlasDownSB_In_HighWSB_TruePlasDown;
    delete mc_PlasDownSB_In_HighWSB_TrueOtherMat;
}



// Data
// ====
void Histograms::InitDataHists_PlasSB_In_PhysSB()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Initialize data histograms
    m_data_PlasUpSB_In_PionBlobSB   = new MH1D(Form("data_%s_PlasUpSB_In_PionBlobSB",   label), label, NBins(), bins);
    m_data_PlasBetwSB_In_PionBlobSB = new MH1D(Form("data_%s_PlasBetwSB_In_PionBlobSB", label), label, NBins(), bins);
    m_data_PlasDownSB_In_PionBlobSB = new MH1D(Form("data_%s_PlasDownSB_In_PionBlobSB", label), label, NBins(), bins);
    
    m_data_PlasUpSB_In_ProtonBlobSB   = new MH1D(Form("data_%s_PlasUpSB_In_ProtonBlobSB",   label), label, NBins(), bins);
    m_data_PlasBetwSB_In_ProtonBlobSB = new MH1D(Form("data_%s_PlasBetwSB_In_ProtonBlobSB", label), label, NBins(), bins);
    m_data_PlasDownSB_In_ProtonBlobSB = new MH1D(Form("data_%s_PlasDownSB_In_ProtonBlobSB", label), label, NBins(), bins);
    
    m_data_PlasUpSB_In_HighWSB   = new MH1D(Form("data_%s_PlasUpSB_In_HighWSB",   label), label, NBins(), bins);
    m_data_PlasBetwSB_In_HighWSB = new MH1D(Form("data_%s_PlasBetwSB_In_HighWSB", label), label, NBins(), bins);
    m_data_PlasDownSB_In_HighWSB = new MH1D(Form("data_%s_PlasDownSB_In_HighWSB", label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE PHYSICS SIDEBAND HISTOGRAMS
// ==========================================================================

// Monte Carlo
// ===========
template <typename T>
void Histograms::InitMCHists_PhysSB(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_SigReg                = new MH1D(Form("mc_%s_SigReg",                label), label, NBins(), bins);
    MH1D* mc_SigReg_Signal         = new MH1D(Form("mc_%s_SigReg_Signal",         label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrPi0HighW = new MH1D(Form("mc_%s_SigReg_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrQElike   = new MH1D(Form("mc_%s_SigReg_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrPionProd = new MH1D(Form("mc_%s_SigReg_BackgrPionProd", label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrPlasUp   = new MH1D(Form("mc_%s_SigReg_BackgrPlasUp",   label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrPlasBetw = new MH1D(Form("mc_%s_SigReg_BackgrPlasBetw", label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrPlasDown = new MH1D(Form("mc_%s_SigReg_BackgrPlasDown", label), label, NBins(), bins);
    MH1D* mc_SigReg_BackgrOther    = new MH1D(Form("mc_%s_SigReg_BackgrOther",    label), label, NBins(), bins);
    
    MH1D* mc_PionBlobSB                = new MH1D(Form("mc_%s_PionBlobSB",                label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_Signal         = new MH1D(Form("mc_%s_PionBlobSB_Signal",         label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrPi0HighW = new MH1D(Form("mc_%s_PionBlobSB_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrQElike   = new MH1D(Form("mc_%s_PionBlobSB_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrPionProd = new MH1D(Form("mc_%s_PionBlobSB_BackgrPionProd", label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrPlasUp   = new MH1D(Form("mc_%s_PionBlobSB_BackgrPlasUp",   label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrPlasBetw = new MH1D(Form("mc_%s_PionBlobSB_BackgrPlasBetw", label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrPlasDown = new MH1D(Form("mc_%s_PionBlobSB_BackgrPlasDown", label), label, NBins(), bins);
    MH1D* mc_PionBlobSB_BackgrOther    = new MH1D(Form("mc_%s_PionBlobSB_BackgrOther",    label), label, NBins(), bins);
    
    MH1D* mc_ProtonBlobSB                = new MH1D(Form("mc_%s_ProtonBlobSB",                label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_Signal         = new MH1D(Form("mc_%s_ProtonBlobSB_Signal",         label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrPi0HighW = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrQElike   = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrPionProd = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrPionProd", label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrPlasUp   = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrPlasUp",   label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrPlasBetw = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrPlasBetw", label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrPlasDown = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrPlasDown", label), label, NBins(), bins);
    MH1D* mc_ProtonBlobSB_BackgrOther    = new MH1D(Form("mc_%s_ProtonBlobSB_BackgrOther",    label), label, NBins(), bins);
    
    MH1D* mc_HighWSB                = new MH1D(Form("mc_%s_HighWSB",                label), label, NBins(), bins);
    MH1D* mc_HighWSB_Signal         = new MH1D(Form("mc_%s_HighWSB_Signal",         label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrPi0HighW = new MH1D(Form("mc_%s_HighWSB_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrQElike   = new MH1D(Form("mc_%s_HighWSB_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrPionProd = new MH1D(Form("mc_%s_HighWSB_BackgrPionProd", label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrPlasUp   = new MH1D(Form("mc_%s_HighWSB_BackgrPlasUp",   label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrPlasBetw = new MH1D(Form("mc_%s_HighWSB_BackgrPlasBetw", label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrPlasDown = new MH1D(Form("mc_%s_HighWSB_BackgrPlasDown", label), label, NBins(), bins);
    MH1D* mc_HighWSB_BackgrOther    = new MH1D(Form("mc_%s_HighWSB_BackgrOther",    label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_SigReg                = CVHW(mc_SigReg,                systematic_univs, clear_bands);
    m_mc_SigReg_Signal         = CVHW(mc_SigReg_Signal,         systematic_univs, clear_bands);
    m_mc_SigReg_BackgrPi0HighW = CVHW(mc_SigReg_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_SigReg_BackgrQElike   = CVHW(mc_SigReg_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_SigReg_BackgrPionProd = CVHW(mc_SigReg_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_SigReg_BackgrPlasUp   = CVHW(mc_SigReg_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_SigReg_BackgrPlasBetw = CVHW(mc_SigReg_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_SigReg_BackgrPlasDown = CVHW(mc_SigReg_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_SigReg_BackgrOther    = CVHW(mc_SigReg_BackgrOther,    systematic_univs, clear_bands);
    
    m_mc_PionBlobSB                = CVHW(mc_PionBlobSB,                systematic_univs, clear_bands);
    m_mc_PionBlobSB_Signal         = CVHW(mc_PionBlobSB_Signal,         systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrPi0HighW = CVHW(mc_PionBlobSB_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrQElike   = CVHW(mc_PionBlobSB_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrPionProd = CVHW(mc_PionBlobSB_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrPlasUp   = CVHW(mc_PionBlobSB_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrPlasBetw = CVHW(mc_PionBlobSB_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrPlasDown = CVHW(mc_PionBlobSB_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_PionBlobSB_BackgrOther    = CVHW(mc_PionBlobSB_BackgrOther,    systematic_univs, clear_bands);
    
    m_mc_ProtonBlobSB                = CVHW(mc_ProtonBlobSB,                systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_Signal         = CVHW(mc_ProtonBlobSB_Signal,         systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrPi0HighW = CVHW(mc_ProtonBlobSB_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrQElike   = CVHW(mc_ProtonBlobSB_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrPionProd = CVHW(mc_ProtonBlobSB_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrPlasUp   = CVHW(mc_ProtonBlobSB_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrPlasBetw = CVHW(mc_ProtonBlobSB_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrPlasDown = CVHW(mc_ProtonBlobSB_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_ProtonBlobSB_BackgrOther    = CVHW(mc_ProtonBlobSB_BackgrOther,    systematic_univs, clear_bands);
    
    m_mc_HighWSB                = CVHW(mc_HighWSB,                systematic_univs, clear_bands);
    m_mc_HighWSB_Signal         = CVHW(mc_HighWSB_Signal,         systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrPi0HighW = CVHW(mc_HighWSB_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrQElike   = CVHW(mc_HighWSB_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrPionProd = CVHW(mc_HighWSB_BackgrPionProd, systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrPlasUp   = CVHW(mc_HighWSB_BackgrPlasUp,   systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrPlasBetw = CVHW(mc_HighWSB_BackgrPlasBetw, systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrPlasDown = CVHW(mc_HighWSB_BackgrPlasDown, systematic_univs, clear_bands);
    m_mc_HighWSB_BackgrOther    = CVHW(mc_HighWSB_BackgrOther,    systematic_univs, clear_bands);
    
    delete mc_SigReg;
    delete mc_SigReg_Signal;
    delete mc_SigReg_BackgrPi0HighW;
    delete mc_SigReg_BackgrQElike;
    delete mc_SigReg_BackgrPionProd;
    delete mc_SigReg_BackgrPlasUp;
    delete mc_SigReg_BackgrPlasBetw;
    delete mc_SigReg_BackgrPlasDown;
    delete mc_SigReg_BackgrOther;
    
    delete mc_PionBlobSB;
    delete mc_PionBlobSB_Signal;
    delete mc_PionBlobSB_BackgrPi0HighW;
    delete mc_PionBlobSB_BackgrQElike;
    delete mc_PionBlobSB_BackgrPionProd;
    delete mc_PionBlobSB_BackgrPlasUp;
    delete mc_PionBlobSB_BackgrPlasBetw;
    delete mc_PionBlobSB_BackgrPlasDown;
    delete mc_PionBlobSB_BackgrOther;
    
    delete mc_ProtonBlobSB;
    delete mc_ProtonBlobSB_Signal;
    delete mc_ProtonBlobSB_BackgrPi0HighW;
    delete mc_ProtonBlobSB_BackgrQElike;
    delete mc_ProtonBlobSB_BackgrPionProd;
    delete mc_ProtonBlobSB_BackgrPlasUp;
    delete mc_ProtonBlobSB_BackgrPlasBetw;
    delete mc_ProtonBlobSB_BackgrPlasDown;
    delete mc_ProtonBlobSB_BackgrOther;
    
    delete mc_HighWSB;
    delete mc_HighWSB_Signal;
    delete mc_HighWSB_BackgrPi0HighW;
    delete mc_HighWSB_BackgrQElike;
    delete mc_HighWSB_BackgrPionProd;
    delete mc_HighWSB_BackgrPlasUp;
    delete mc_HighWSB_BackgrPlasBetw;
    delete mc_HighWSB_BackgrPlasDown;
    delete mc_HighWSB_BackgrOther;
}



// Data
// ====
void Histograms::InitDataHists_PhysSB()
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    m_data_SigReg       = new MH1D(Form("data_%s_SigReg",       label), label, NBins(), bins);
    m_data_PionBlobSB   = new MH1D(Form("data_%s_PionBlobSB",   label), label, NBins(), bins);
    m_data_ProtonBlobSB = new MH1D(Form("data_%s_ProtonBlobSB", label), label, NBins(), bins);
    m_data_HighWSB      = new MH1D(Form("data_%s_HighWSB",      label), label, NBins(), bins);
}





// ==========================================================================
//  INITIALIZE MC TUNING WEIGHTS
// ==========================================================================

// MC plastic weights in signal region
// ===================================

template <typename T>
void Histograms::InitMCWeights_PlasBackgr_In_SigReg(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_Weight_SigReg_TruePlasUp   = new MH1D(Form("mc_Weight_%s_SigReg_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_Weight_SigReg_TruePlasBetw = new MH1D(Form("mc_Weight_%s_SigReg_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_Weight_SigReg_TruePlasDown = new MH1D(Form("mc_Weight_%s_SigReg_TruePlasDown", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Weight_SigReg_TruePlasUp   = CVHW(mc_Weight_SigReg_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_Weight_SigReg_TruePlasBetw = CVHW(mc_Weight_SigReg_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_Weight_SigReg_TruePlasDown = CVHW(mc_Weight_SigReg_TruePlasDown, systematic_univs, clear_bands);
}



// MC plastic weights in physics sidebands
// =======================================

template <typename T>
void Histograms::InitMCWeights_PlasBackgr_In_PhysSB(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_Weight_PionBlobSB_TruePlasUp   = new MH1D(Form("mc_Weight_%s_PionBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_Weight_PionBlobSB_TruePlasBetw = new MH1D(Form("mc_Weight_%s_PionBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_Weight_PionBlobSB_TruePlasDown = new MH1D(Form("mc_Weight_%s_PionBlobSB_TruePlasDown", label), label, NBins(), bins);
    
    MH1D* mc_Weight_ProtonBlobSB_TruePlasUp   = new MH1D(Form("mc_Weight_%s_ProtonBlobSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_Weight_ProtonBlobSB_TruePlasBetw = new MH1D(Form("mc_Weight_%s_ProtonBlobSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_Weight_ProtonBlobSB_TruePlasDown = new MH1D(Form("mc_Weight_%s_ProtonBlobSB_TruePlasDown", label), label, NBins(), bins);
    
    MH1D* mc_Weight_HighWSB_TruePlasUp   = new MH1D(Form("mc_Weight_%s_HighWSB_TruePlasUp",   label), label, NBins(), bins);
    MH1D* mc_Weight_HighWSB_TruePlasBetw = new MH1D(Form("mc_Weight_%s_HighWSB_TruePlasBetw", label), label, NBins(), bins);
    MH1D* mc_Weight_HighWSB_TruePlasDown = new MH1D(Form("mc_Weight_%s_HighWSB_TruePlasDown", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Weight_PionBlobSB_TruePlasUp   = CVHW(mc_Weight_PionBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_Weight_PionBlobSB_TruePlasBetw = CVHW(mc_Weight_PionBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_Weight_PionBlobSB_TruePlasDown = CVHW(mc_Weight_PionBlobSB_TruePlasDown, systematic_univs, clear_bands);
    
    m_mc_Weight_ProtonBlobSB_TruePlasUp   = CVHW(mc_Weight_ProtonBlobSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_Weight_ProtonBlobSB_TruePlasBetw = CVHW(mc_Weight_ProtonBlobSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_Weight_ProtonBlobSB_TruePlasDown = CVHW(mc_Weight_ProtonBlobSB_TruePlasDown, systematic_univs, clear_bands);
    
    m_mc_Weight_HighWSB_TruePlasUp   = CVHW(mc_Weight_HighWSB_TruePlasUp,   systematic_univs, clear_bands);
    m_mc_Weight_HighWSB_TruePlasBetw = CVHW(mc_Weight_HighWSB_TruePlasBetw, systematic_univs, clear_bands);
    m_mc_Weight_HighWSB_TruePlasDown = CVHW(mc_Weight_HighWSB_TruePlasDown, systematic_univs, clear_bands);
}



// MC physics weights
// ==================

template <typename T>
void Histograms::InitMCWeights_PhysBackgr(T systematic_univs)
{
    const Double_t* bins = m_bins_array.GetArray();
    const char* label    = m_label.c_str();
    
    
    // Define MC dummy histograms
    MH1D* mc_Weight_Signal         = new MH1D(Form("mc_Weight_%s_Signal",         label), label, NBins(), bins);
    MH1D* mc_Weight_BackgrPi0HighW = new MH1D(Form("mc_Weight_%s_BackgrPi0HighW", label), label, NBins(), bins);
    MH1D* mc_Weight_BackgrQElike   = new MH1D(Form("mc_Weight_%s_BackgrQElike",   label), label, NBins(), bins);
    MH1D* mc_Weight_BackgrPionProd = new MH1D(Form("mc_Weight_%s_BackgrPionProd", label), label, NBins(), bins);
    
    
    // Assign member histograms and delete dummies
    const bool clear_bands = true;
    
    m_mc_Weight_Signal         = CVHW(mc_Weight_Signal,         systematic_univs, clear_bands);
    m_mc_Weight_BackgrPi0HighW = CVHW(mc_Weight_BackgrPi0HighW, systematic_univs, clear_bands);
    m_mc_Weight_BackgrQElike   = CVHW(mc_Weight_BackgrQElike,   systematic_univs, clear_bands);
    m_mc_Weight_BackgrPionProd = CVHW(mc_Weight_BackgrPionProd, systematic_univs, clear_bands);
}





// ==========================================================================
//  SYNCHRONIZE MC CV HISTOGRAMS
// ==========================================================================

// Event selection
// ===============

void Histograms::SyncMCHists_Selection() {
    m_mc_Selection.SyncCVHistos();
    m_mc_Selection_Signal.SyncCVHistos();
    m_mc_Selection_Backgr.SyncCVHistos();
    
    m_mc_Selection_BackgrPi0HighW.SyncCVHistos();
    m_mc_Selection_BackgrQElike.SyncCVHistos();
    m_mc_Selection_BackgrPionProd.SyncCVHistos();
    m_mc_Selection_BackgrPlasUp.SyncCVHistos();
    m_mc_Selection_BackgrPlasBetw.SyncCVHistos();
    m_mc_Selection_BackgrPlasDown.SyncCVHistos();
    m_mc_Selection_BackgrOther.SyncCVHistos();
}



// Event selection with material breakdown
// =======================================

void Histograms::SyncMCHists_MatSelection() {
    m_mc_MatSelection.SyncCVHistos();
    m_mc_MatSelection_TrueTgt4Pb.SyncCVHistos();
    m_mc_MatSelection_TrueTgt5Pb.SyncCVHistos();
    m_mc_MatSelection_TrueTgt5Fe.SyncCVHistos();
    m_mc_MatSelection_TruePlasUp.SyncCVHistos();
    m_mc_MatSelection_TruePlasBetw.SyncCVHistos();
    m_mc_MatSelection_TruePlasDown.SyncCVHistos();
    m_mc_MatSelection_TrueOtherMat.SyncCVHistos();
}



// Event selection with interaction type breakdown
// ===============================================

void Histograms::SyncMCHists_IntTypeSelection() {
    m_mc_IntTypeSelection.SyncCVHistos();
    m_mc_IntTypeSelection_QE.SyncCVHistos();
    m_mc_IntTypeSelection_MEC.SyncCVHistos();
    m_mc_IntTypeSelection_DeltaRES.SyncCVHistos();
    m_mc_IntTypeSelection_OtherRES.SyncCVHistos();
    m_mc_IntTypeSelection_SoftDIS.SyncCVHistos();
    m_mc_IntTypeSelection_TrueDIS.SyncCVHistos();
    m_mc_IntTypeSelection_Other.SyncCVHistos();
}



// Reco objects with PDG breakdown
// ===============================

void Histograms::SyncMCHists_ObjectPdg() {
    m_mc_ObjectPdg.SyncCVHistos();
    m_mc_ObjectPdg_Pi0.SyncCVHistos();
    m_mc_ObjectPdg_Proton.SyncCVHistos();
    m_mc_ObjectPdg_Neutron.SyncCVHistos();
    m_mc_ObjectPdg_Pion.SyncCVHistos();
    m_mc_ObjectPdg_EM.SyncCVHistos();
    m_mc_ObjectPdg_Muon.SyncCVHistos();
    m_mc_ObjectPdg_OthPdg.SyncCVHistos();
    m_mc_ObjectPdg_MCXtalk.SyncCVHistos();
    m_mc_ObjectPdg_Overlay.SyncCVHistos();
}



// Efficiency components
// =====================

void Histograms::SyncEffNumerator() {
    m_mc_EffNumerator.SyncCVHistos();
    m_mc_EffNumerator_QE.SyncCVHistos();
    m_mc_EffNumerator_MEC.SyncCVHistos();
    m_mc_EffNumerator_DeltaRES.SyncCVHistos();
    m_mc_EffNumerator_OtherRES.SyncCVHistos();
    m_mc_EffNumerator_SoftDIS.SyncCVHistos();
    m_mc_EffNumerator_TrueDIS.SyncCVHistos();
    m_mc_EffNumerator_Other.SyncCVHistos();
}


void Histograms::SyncEffDenominator() {
    m_mc_EffDenominator.SyncCVHistos();
    m_mc_EffDenominator_QE.SyncCVHistos();
    m_mc_EffDenominator_MEC.SyncCVHistos();
    m_mc_EffDenominator_DeltaRES.SyncCVHistos();
    m_mc_EffDenominator_OtherRES.SyncCVHistos();
    m_mc_EffDenominator_SoftDIS.SyncCVHistos();
    m_mc_EffDenominator_TrueDIS.SyncCVHistos();
    m_mc_EffDenominator_Other.SyncCVHistos();
}



// Migration matrix
// ================

void Histograms::SyncMigrationHists() {
    m_mc_Migration.SyncCVHistos();
}



// Plastic sidebands in signal region
// ==================================

void Histograms::SyncMCHists_PlasSB_In_SigReg() {
    m_mc_RecoPb_In_SigReg.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TruePlasDown.SyncCVHistos();
    m_mc_RecoPb_In_SigReg_TrueOtherMat.SyncCVHistos();
    
    m_mc_RecoFe_In_SigReg.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TruePlasDown.SyncCVHistos();
    m_mc_RecoFe_In_SigReg_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasUpSB_In_SigReg.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TruePlasDown.SyncCVHistos();
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasBetwSB_In_SigReg.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown.SyncCVHistos();
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasDownSB_In_SigReg.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TruePlasDown.SyncCVHistos();
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat.SyncCVHistos();
}



// Plastic sidebands in physics sidebands
// ======================================

void Histograms::SyncMCHists_PlasSB_In_PhysSB() {
    m_mc_PlasUpSB_In_PionBlobSB.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasBetwSB_In_PionBlobSB.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasDownSB_In_PionBlobSB.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasUpSB_In_ProtonBlobSB.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasBetwSB_In_ProtonBlobSB.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasDownSB_In_ProtonBlobSB.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasUpSB_In_HighWSB.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasBetwSB_In_HighWSB.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.SyncCVHistos();
    
    m_mc_PlasDownSB_In_HighWSB.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown.SyncCVHistos();
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.SyncCVHistos();
}



// Physics sidebands
// =================

void Histograms::SyncMCHists_PhysSB() {
    m_mc_SigReg.SyncCVHistos();
    m_mc_SigReg_Signal.SyncCVHistos();
    m_mc_SigReg_BackgrPi0HighW.SyncCVHistos();
    m_mc_SigReg_BackgrQElike.SyncCVHistos();
    m_mc_SigReg_BackgrPionProd.SyncCVHistos();
    m_mc_SigReg_BackgrPlasUp.SyncCVHistos();
    m_mc_SigReg_BackgrPlasBetw.SyncCVHistos();
    m_mc_SigReg_BackgrPlasDown.SyncCVHistos();
    m_mc_SigReg_BackgrOther.SyncCVHistos();
    
    m_mc_PionBlobSB.SyncCVHistos();
    m_mc_PionBlobSB_Signal.SyncCVHistos();
    m_mc_PionBlobSB_BackgrPi0HighW.SyncCVHistos();
    m_mc_PionBlobSB_BackgrQElike.SyncCVHistos();
    m_mc_PionBlobSB_BackgrPionProd.SyncCVHistos();
    m_mc_PionBlobSB_BackgrPlasUp.SyncCVHistos();
    m_mc_PionBlobSB_BackgrPlasBetw.SyncCVHistos();
    m_mc_PionBlobSB_BackgrPlasDown.SyncCVHistos();
    m_mc_PionBlobSB_BackgrOther.SyncCVHistos();
    
    m_mc_ProtonBlobSB.SyncCVHistos();
    m_mc_ProtonBlobSB_Signal.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrPi0HighW.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrQElike.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrPionProd.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrPlasUp.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrPlasBetw.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrPlasDown.SyncCVHistos();
    m_mc_ProtonBlobSB_BackgrOther.SyncCVHistos();
    
    m_mc_HighWSB.SyncCVHistos();
    m_mc_HighWSB_Signal.SyncCVHistos();
    m_mc_HighWSB_BackgrPi0HighW.SyncCVHistos();
    m_mc_HighWSB_BackgrQElike.SyncCVHistos();
    m_mc_HighWSB_BackgrPionProd.SyncCVHistos();
    m_mc_HighWSB_BackgrPlasUp.SyncCVHistos();
    m_mc_HighWSB_BackgrPlasBetw.SyncCVHistos();
    m_mc_HighWSB_BackgrPlasDown.SyncCVHistos();
    m_mc_HighWSB_BackgrOther.SyncCVHistos();
}



// MC plastic weights in signal region
// ===================================

void Histograms::SyncMCWeights_PlasBackgr_In_SigReg() {
    m_mc_Weight_SigReg_TruePlasUp.SyncCVHistos();
    m_mc_Weight_SigReg_TruePlasBetw.SyncCVHistos();
    m_mc_Weight_SigReg_TruePlasDown.SyncCVHistos();
}



// MC plastic weights in physics sidebands
// =======================================
void Histograms::SyncMCWeights_PlasBackgr_In_PhysSB() {
    m_mc_Weight_PionBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_Weight_PionBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_Weight_PionBlobSB_TruePlasDown.SyncCVHistos();
    
    m_mc_Weight_HighWSB_TruePlasUp.SyncCVHistos();
    m_mc_Weight_HighWSB_TruePlasBetw.SyncCVHistos();
    m_mc_Weight_HighWSB_TruePlasDown.SyncCVHistos();
    
    m_mc_Weight_ProtonBlobSB_TruePlasUp.SyncCVHistos();
    m_mc_Weight_ProtonBlobSB_TruePlasBetw.SyncCVHistos();
    m_mc_Weight_ProtonBlobSB_TruePlasDown.SyncCVHistos();
}



// MC physics weights
// ==================

void Histograms::SyncMCWeights_PhysBackgr() {
    m_mc_Weight_Signal.SyncCVHistos();
    m_mc_Weight_BackgrPi0HighW.SyncCVHistos();
    m_mc_Weight_BackgrQElike.SyncCVHistos();
    m_mc_Weight_BackgrPionProd.SyncCVHistos();
}





// =======================================================================================
//  BIN WIDTH NORMALIZE HISTOGRAMS
// =======================================================================================

// Event selection
// ===============

// Monte Carlo
void Histograms::BinWidthNormMCHists_Selection() {
    m_mc_Selection.hist        -> Scale(m_mc_Selection.hist->GetNormBinWidth(),        "width");
    m_mc_Selection_Signal.hist -> Scale(m_mc_Selection_Signal.hist->GetNormBinWidth(), "width");
    m_mc_Selection_Backgr.hist -> Scale(m_mc_Selection_Backgr.hist->GetNormBinWidth(), "width");
    
    m_mc_Selection_BackgrPi0HighW.hist -> Scale(m_mc_Selection_BackgrPi0HighW.hist->GetNormBinWidth(), "width");
    m_mc_Selection_BackgrQElike.hist   -> Scale(m_mc_Selection_BackgrQElike.hist->GetNormBinWidth(),   "width");
    m_mc_Selection_BackgrPionProd.hist -> Scale(m_mc_Selection_BackgrPionProd.hist->GetNormBinWidth(), "width");
    m_mc_Selection_BackgrPlasUp.hist   -> Scale(m_mc_Selection_BackgrPlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_Selection_BackgrPlasBetw.hist -> Scale(m_mc_Selection_BackgrPlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_Selection_BackgrPlasDown.hist -> Scale(m_mc_Selection_BackgrPlasDown.hist->GetNormBinWidth(), "width");
    m_mc_Selection_BackgrOther.hist    -> Scale(m_mc_Selection_BackgrOther.hist->GetNormBinWidth(),    "width");
}


// Data
void Histograms::BinWidthNormDataHists_Selection() {
    m_data_Selection -> Scale(m_data_Selection->GetNormBinWidth(), "width");
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms::BinWidthNormMCHists_MatSelection() {
    m_mc_MatSelection.hist              -> Scale(m_mc_MatSelection.hist->GetNormBinWidth(),              "width");
    m_mc_MatSelection_TrueTgt4Pb.hist   -> Scale(m_mc_MatSelection_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_MatSelection_TrueTgt5Pb.hist   -> Scale(m_mc_MatSelection_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_MatSelection_TrueTgt5Fe.hist   -> Scale(m_mc_MatSelection_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_MatSelection_TruePlasUp.hist   -> Scale(m_mc_MatSelection_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_MatSelection_TruePlasBetw.hist -> Scale(m_mc_MatSelection_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_MatSelection_TruePlasDown.hist -> Scale(m_mc_MatSelection_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_MatSelection_TrueOtherMat.hist -> Scale(m_mc_MatSelection_TrueOtherMat.hist->GetNormBinWidth(), "width");
}


// Data
void Histograms::BinWidthNormDataHists_MatSelection() {
    m_data_MatSelection -> Scale(m_data_MatSelection->GetNormBinWidth(), "width");
}



// Event selection with interaction type breakdown
// ===============================================

// Monte Carlo
void Histograms::BinWidthNormMCHists_IntTypeSelection() {
    m_mc_IntTypeSelection.hist          -> Scale(m_mc_IntTypeSelection.hist->GetNormBinWidth(),          "width");
    m_mc_IntTypeSelection_QE.hist       -> Scale(m_mc_IntTypeSelection_QE.hist->GetNormBinWidth(),       "width");
    m_mc_IntTypeSelection_MEC.hist      -> Scale(m_mc_IntTypeSelection_MEC.hist->GetNormBinWidth(),      "width");
    m_mc_IntTypeSelection_DeltaRES.hist -> Scale(m_mc_IntTypeSelection_DeltaRES.hist->GetNormBinWidth(), "width");
    m_mc_IntTypeSelection_OtherRES.hist -> Scale(m_mc_IntTypeSelection_OtherRES.hist->GetNormBinWidth(), "width");
    m_mc_IntTypeSelection_SoftDIS.hist  -> Scale(m_mc_IntTypeSelection_SoftDIS.hist->GetNormBinWidth(),  "width");
    m_mc_IntTypeSelection_TrueDIS.hist  -> Scale(m_mc_IntTypeSelection_TrueDIS.hist->GetNormBinWidth(),  "width");
    m_mc_IntTypeSelection_Other.hist    -> Scale(m_mc_IntTypeSelection_Other.hist->GetNormBinWidth(),    "width");
}


// Data
void Histograms::BinWidthNormDataHists_IntTypeSelection() {
    m_data_IntTypeSelection -> Scale(m_data_IntTypeSelection->GetNormBinWidth(), "width");
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms::BinWidthNormMCHists_ObjectPdg() {
    m_mc_ObjectPdg.hist         -> Scale(m_mc_ObjectPdg.hist->GetNormBinWidth(),         "width");
    m_mc_ObjectPdg_Pi0.hist     -> Scale(m_mc_ObjectPdg_Pi0.hist->GetNormBinWidth(),     "width");
    m_mc_ObjectPdg_Proton.hist  -> Scale(m_mc_ObjectPdg_Proton.hist->GetNormBinWidth(),  "width");
    m_mc_ObjectPdg_Neutron.hist -> Scale(m_mc_ObjectPdg_Neutron.hist->GetNormBinWidth(), "width");
    m_mc_ObjectPdg_Pion.hist    -> Scale(m_mc_ObjectPdg_Pion.hist->GetNormBinWidth(),    "width");
    m_mc_ObjectPdg_EM.hist      -> Scale(m_mc_ObjectPdg_EM.hist->GetNormBinWidth(),      "width");
    m_mc_ObjectPdg_Muon.hist    -> Scale(m_mc_ObjectPdg_Muon.hist->GetNormBinWidth(),    "width");
    m_mc_ObjectPdg_OthPdg.hist  -> Scale(m_mc_ObjectPdg_OthPdg.hist->GetNormBinWidth(),  "width");
    m_mc_ObjectPdg_MCXtalk.hist -> Scale(m_mc_ObjectPdg_MCXtalk.hist->GetNormBinWidth(), "width");
    m_mc_ObjectPdg_Overlay.hist -> Scale(m_mc_ObjectPdg_Overlay.hist->GetNormBinWidth(), "width");
}


// Data
void Histograms::BinWidthNormDataHists_ObjectPdg() {
    m_data_ObjectPdg -> Scale(m_data_ObjectPdg->GetNormBinWidth(), "width");
}



// Efficiency components
// =====================

// Numerator
void Histograms::BinWidthNormEffNumerator() {
    m_mc_EffNumerator.hist          -> Scale(m_mc_EffNumerator.hist->GetNormBinWidth(),          "width");
    m_mc_EffNumerator_QE.hist       -> Scale(m_mc_EffNumerator_QE.hist->GetNormBinWidth(),       "width");
    m_mc_EffNumerator_MEC.hist      -> Scale(m_mc_EffNumerator_MEC.hist->GetNormBinWidth(),      "width");
    m_mc_EffNumerator_DeltaRES.hist -> Scale(m_mc_EffNumerator_DeltaRES.hist->GetNormBinWidth(), "width");
    m_mc_EffNumerator_OtherRES.hist -> Scale(m_mc_EffNumerator_OtherRES.hist->GetNormBinWidth(), "width");
    m_mc_EffNumerator_SoftDIS.hist  -> Scale(m_mc_EffNumerator_SoftDIS.hist->GetNormBinWidth(),  "width");
    m_mc_EffNumerator_TrueDIS.hist  -> Scale(m_mc_EffNumerator_TrueDIS.hist->GetNormBinWidth(),  "width");
    m_mc_EffNumerator_Other.hist    -> Scale(m_mc_EffNumerator_Other.hist->GetNormBinWidth(),    "width");
}


// Denominator
void Histograms::BinWidthNormEffDenominator() {
    m_mc_EffDenominator.hist          -> Scale(m_mc_EffDenominator.hist->GetNormBinWidth(),          "width");
    m_mc_EffDenominator_QE.hist       -> Scale(m_mc_EffDenominator_QE.hist->GetNormBinWidth(),       "width");
    m_mc_EffDenominator_MEC.hist      -> Scale(m_mc_EffDenominator_MEC.hist->GetNormBinWidth(),      "width");
    m_mc_EffDenominator_DeltaRES.hist -> Scale(m_mc_EffDenominator_DeltaRES.hist->GetNormBinWidth(), "width");
    m_mc_EffDenominator_OtherRES.hist -> Scale(m_mc_EffDenominator_OtherRES.hist->GetNormBinWidth(), "width");
    m_mc_EffDenominator_SoftDIS.hist  -> Scale(m_mc_EffDenominator_SoftDIS.hist->GetNormBinWidth(),  "width");
    m_mc_EffDenominator_TrueDIS.hist  -> Scale(m_mc_EffDenominator_TrueDIS.hist->GetNormBinWidth(),  "width");
    m_mc_EffDenominator_Other.hist    -> Scale(m_mc_EffDenominator_Other.hist->GetNormBinWidth(),    "width");
}



// Migration matrix
// ================

void Histograms::BinWidthNormMigrationHists() {
    m_mc_Migration.hist -> Scale(m_mc_Migration.hist->GetNormBinWidthX() *
                                 m_mc_Migration.hist->GetNormBinWidthY(), "width");
}



// Plastic sidebands in signal region
// ==================================

// Monte Carlo
void Histograms::BinWidthNormMCHists_PlasSB_In_SigReg() {
    m_mc_RecoPb_In_SigReg.hist              -> Scale(m_mc_RecoPb_In_SigReg.hist->GetNormBinWidth(),              "width");
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb.hist   -> Scale(m_mc_RecoPb_In_SigReg_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb.hist   -> Scale(m_mc_RecoPb_In_SigReg_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe.hist   -> Scale(m_mc_RecoPb_In_SigReg_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_RecoPb_In_SigReg_TruePlasUp.hist   -> Scale(m_mc_RecoPb_In_SigReg_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_RecoPb_In_SigReg_TruePlasBetw.hist -> Scale(m_mc_RecoPb_In_SigReg_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_RecoPb_In_SigReg_TruePlasDown.hist -> Scale(m_mc_RecoPb_In_SigReg_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_RecoPb_In_SigReg_TrueOtherMat.hist -> Scale(m_mc_RecoPb_In_SigReg_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_RecoFe_In_SigReg.hist              -> Scale(m_mc_RecoFe_In_SigReg.hist->GetNormBinWidth(),              "width");
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb.hist   -> Scale(m_mc_RecoFe_In_SigReg_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb.hist   -> Scale(m_mc_RecoFe_In_SigReg_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe.hist   -> Scale(m_mc_RecoFe_In_SigReg_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_RecoFe_In_SigReg_TruePlasUp.hist   -> Scale(m_mc_RecoFe_In_SigReg_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_RecoFe_In_SigReg_TruePlasBetw.hist -> Scale(m_mc_RecoFe_In_SigReg_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_RecoFe_In_SigReg_TruePlasDown.hist -> Scale(m_mc_RecoFe_In_SigReg_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_RecoFe_In_SigReg_TrueOtherMat.hist -> Scale(m_mc_RecoFe_In_SigReg_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasUpSB_In_SigReg.hist              -> Scale(m_mc_PlasUpSB_In_SigReg.hist->GetNormBinWidth(),              "width");
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist   -> Scale(m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist -> Scale(m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist -> Scale(m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat.hist -> Scale(m_mc_PlasUpSB_In_SigReg_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasBetwSB_In_SigReg.hist              -> Scale(m_mc_PlasBetwSB_In_SigReg.hist->GetNormBinWidth(),              "width");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist   -> Scale(m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist -> Scale(m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist -> Scale(m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.hist -> Scale(m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasDownSB_In_SigReg.hist              -> Scale(m_mc_PlasDownSB_In_SigReg.hist->GetNormBinWidth(),              "width");
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist   -> Scale(m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist -> Scale(m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist -> Scale(m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat.hist -> Scale(m_mc_PlasDownSB_In_SigReg_TrueOtherMat.hist->GetNormBinWidth(), "width");
}


// Data
void Histograms::BinWidthNormDataHists_PlasSB_In_SigReg() {
    m_data_RecoPb_In_SigReg     -> Scale(m_data_RecoPb_In_SigReg->GetNormBinWidth(),     "width");
    m_data_RecoFe_In_SigReg     -> Scale(m_data_RecoFe_In_SigReg->GetNormBinWidth(),     "width");
    m_data_PlasUpSB_In_SigReg   -> Scale(m_data_PlasUpSB_In_SigReg->GetNormBinWidth(),   "width");
    m_data_PlasBetwSB_In_SigReg -> Scale(m_data_PlasBetwSB_In_SigReg->GetNormBinWidth(), "width");
    m_data_PlasDownSB_In_SigReg -> Scale(m_data_PlasDownSB_In_SigReg->GetNormBinWidth(), "width");
}



// Plastic sidebands in physics sidebands
// ======================================

// Monte Carlo
void Histograms::BinWidthNormMCHists_PlasSB_In_PhysSB() {
    m_mc_PlasUpSB_In_PionBlobSB.hist              -> Scale(m_mc_PlasUpSB_In_PionBlobSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasBetwSB_In_PionBlobSB.hist              -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasDownSB_In_PionBlobSB.hist              -> Scale(m_mc_PlasDownSB_In_PionBlobSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasUpSB_In_ProtonBlobSB.hist              -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasBetwSB_In_ProtonBlobSB.hist              -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasDownSB_In_ProtonBlobSB.hist              -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasUpSB_In_HighWSB.hist              -> Scale(m_mc_PlasUpSB_In_HighWSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist   -> Scale(m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist -> Scale(m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist -> Scale(m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.hist -> Scale(m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasBetwSB_In_HighWSB.hist              -> Scale(m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist   -> Scale(m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist -> Scale(m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist -> Scale(m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.hist -> Scale(m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
    
    m_mc_PlasDownSB_In_HighWSB.hist              -> Scale(m_mc_PlasDownSB_In_HighWSB.hist->GetNormBinWidth(),              "width");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist   -> Scale(m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist -> Scale(m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist -> Scale(m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.hist -> Scale(m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.hist->GetNormBinWidth(), "width");
}


// Data
void Histograms::BinWidthNormDataHists_PlasSB_In_PhysSB() {
    m_data_PlasUpSB_In_PionBlobSB   -> Scale(m_data_PlasUpSB_In_PionBlobSB->GetNormBinWidth(),   "width");
    m_data_PlasBetwSB_In_PionBlobSB -> Scale(m_data_PlasBetwSB_In_PionBlobSB->GetNormBinWidth(), "width");
    m_data_PlasDownSB_In_PionBlobSB -> Scale(m_data_PlasDownSB_In_PionBlobSB->GetNormBinWidth(), "width");
    
    m_data_PlasUpSB_In_ProtonBlobSB   -> Scale(m_data_PlasUpSB_In_ProtonBlobSB->GetNormBinWidth(),   "width");
    m_data_PlasBetwSB_In_ProtonBlobSB -> Scale(m_data_PlasBetwSB_In_ProtonBlobSB->GetNormBinWidth(), "width");
    m_data_PlasDownSB_In_ProtonBlobSB -> Scale(m_data_PlasDownSB_In_ProtonBlobSB->GetNormBinWidth(), "width");
    
    m_data_PlasUpSB_In_HighWSB   -> Scale(m_data_PlasUpSB_In_HighWSB->GetNormBinWidth(),   "width");
    m_data_PlasBetwSB_In_HighWSB -> Scale(m_data_PlasBetwSB_In_HighWSB->GetNormBinWidth(), "width");
    m_data_PlasDownSB_In_HighWSB -> Scale(m_data_PlasDownSB_In_HighWSB->GetNormBinWidth(), "width");
}



// Physics sidebands
// =================

// Monte Carlo
void Histograms::BinWidthNormMCHists_PhysSB() {
    m_mc_SigReg.hist                -> Scale(m_mc_SigReg.hist->GetNormBinWidth(),                "width");
    m_mc_SigReg_Signal.hist         -> Scale(m_mc_SigReg_Signal.hist->GetNormBinWidth(),         "width");
    m_mc_SigReg_BackgrPi0HighW.hist -> Scale(m_mc_SigReg_BackgrPi0HighW.hist->GetNormBinWidth(), "width");
    m_mc_SigReg_BackgrQElike.hist   -> Scale(m_mc_SigReg_BackgrQElike.hist->GetNormBinWidth(),   "width");
    m_mc_SigReg_BackgrPionProd.hist -> Scale(m_mc_SigReg_BackgrPionProd.hist->GetNormBinWidth(), "width");
    m_mc_SigReg_BackgrPlasUp.hist   -> Scale(m_mc_SigReg_BackgrPlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_SigReg_BackgrPlasBetw.hist -> Scale(m_mc_SigReg_BackgrPlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_SigReg_BackgrPlasDown.hist -> Scale(m_mc_SigReg_BackgrPlasDown.hist->GetNormBinWidth(), "width");
    m_mc_SigReg_BackgrOther.hist    -> Scale(m_mc_SigReg_BackgrOther.hist->GetNormBinWidth(),    "width");
    
    m_mc_PionBlobSB.hist                -> Scale(m_mc_PionBlobSB.hist->GetNormBinWidth(),                "width");
    m_mc_PionBlobSB_Signal.hist         -> Scale(m_mc_PionBlobSB_Signal.hist->GetNormBinWidth(),         "width");
    m_mc_PionBlobSB_BackgrPi0HighW.hist -> Scale(m_mc_PionBlobSB_BackgrPi0HighW.hist->GetNormBinWidth(), "width");
    m_mc_PionBlobSB_BackgrQElike.hist   -> Scale(m_mc_PionBlobSB_BackgrQElike.hist->GetNormBinWidth(),   "width");
    m_mc_PionBlobSB_BackgrPionProd.hist -> Scale(m_mc_PionBlobSB_BackgrPionProd.hist->GetNormBinWidth(), "width");
    m_mc_PionBlobSB_BackgrPlasUp.hist   -> Scale(m_mc_PionBlobSB_BackgrPlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_PionBlobSB_BackgrPlasBetw.hist -> Scale(m_mc_PionBlobSB_BackgrPlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_PionBlobSB_BackgrPlasDown.hist -> Scale(m_mc_PionBlobSB_BackgrPlasDown.hist->GetNormBinWidth(), "width");
    m_mc_PionBlobSB_BackgrOther.hist    -> Scale(m_mc_PionBlobSB_BackgrOther.hist->GetNormBinWidth(),    "width");
    
    m_mc_ProtonBlobSB.hist                -> Scale(m_mc_ProtonBlobSB.hist->GetNormBinWidth(),                "width");
    m_mc_ProtonBlobSB_Signal.hist         -> Scale(m_mc_ProtonBlobSB_Signal.hist->GetNormBinWidth(),         "width");
    m_mc_ProtonBlobSB_BackgrPi0HighW.hist -> Scale(m_mc_ProtonBlobSB_BackgrPi0HighW.hist->GetNormBinWidth(), "width");
    m_mc_ProtonBlobSB_BackgrQElike.hist   -> Scale(m_mc_ProtonBlobSB_BackgrQElike.hist->GetNormBinWidth(),   "width");
    m_mc_ProtonBlobSB_BackgrPionProd.hist -> Scale(m_mc_ProtonBlobSB_BackgrPionProd.hist->GetNormBinWidth(), "width");
    m_mc_ProtonBlobSB_BackgrPlasUp.hist   -> Scale(m_mc_ProtonBlobSB_BackgrPlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_ProtonBlobSB_BackgrPlasBetw.hist -> Scale(m_mc_ProtonBlobSB_BackgrPlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_ProtonBlobSB_BackgrPlasDown.hist -> Scale(m_mc_ProtonBlobSB_BackgrPlasDown.hist->GetNormBinWidth(), "width");
    m_mc_ProtonBlobSB_BackgrOther.hist    -> Scale(m_mc_ProtonBlobSB_BackgrOther.hist->GetNormBinWidth(),    "width");
    
    m_mc_HighWSB.hist                -> Scale(m_mc_HighWSB.hist->GetNormBinWidth(),                "width");
    m_mc_HighWSB_Signal.hist         -> Scale(m_mc_HighWSB_Signal.hist->GetNormBinWidth(),         "width");
    m_mc_HighWSB_BackgrPi0HighW.hist -> Scale(m_mc_HighWSB_BackgrPi0HighW.hist->GetNormBinWidth(), "width");
    m_mc_HighWSB_BackgrQElike.hist   -> Scale(m_mc_HighWSB_BackgrQElike.hist->GetNormBinWidth(),   "width");
    m_mc_HighWSB_BackgrPionProd.hist -> Scale(m_mc_HighWSB_BackgrPionProd.hist->GetNormBinWidth(), "width");
    m_mc_HighWSB_BackgrPlasUp.hist   -> Scale(m_mc_HighWSB_BackgrPlasUp.hist->GetNormBinWidth(),   "width");
    m_mc_HighWSB_BackgrPlasBetw.hist -> Scale(m_mc_HighWSB_BackgrPlasBetw.hist->GetNormBinWidth(), "width");
    m_mc_HighWSB_BackgrPlasDown.hist -> Scale(m_mc_HighWSB_BackgrPlasDown.hist->GetNormBinWidth(), "width");
    m_mc_HighWSB_BackgrOther.hist    -> Scale(m_mc_HighWSB_BackgrOther.hist->GetNormBinWidth(),    "width");
}


// Data
void Histograms::BinWidthNormDataHists_PhysSB() {
    m_data_SigReg       -> Scale(m_data_SigReg->GetNormBinWidth(),       "width");
    m_data_PionBlobSB   -> Scale(m_data_PionBlobSB->GetNormBinWidth(),   "width");
    m_data_ProtonBlobSB -> Scale(m_data_ProtonBlobSB->GetNormBinWidth(), "width");
    m_data_HighWSB      -> Scale(m_data_HighWSB->GetNormBinWidth(),      "width");
}





// ==========================================================================
//  SCALE MC HISTOGRAMS
// ==========================================================================

// Event selection
// ===============

void Histograms::ScaleMCHists_Selection(const double mc_pot, const double data_pot) {
    m_mc_Selection.hist        -> Scale(data_pot/mc_pot);
    m_mc_Selection_Signal.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection_Backgr.hist -> Scale(data_pot/mc_pot);
    
    m_mc_Selection_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_Selection_BackgrOther.hist    -> Scale(data_pot/mc_pot);
}



// Event selection with material breakdown
// =======================================

void Histograms::ScaleMCHists_MatSelection(const double mc_pot, const double data_pot) {
    m_mc_MatSelection.hist              -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_MatSelection_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
}



// Event selection with interaction type breakdown
// ===============================================

void Histograms::ScaleMCHists_IntTypeSelection(const double mc_pot, const double data_pot) {
    m_mc_IntTypeSelection.hist          -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_QE.hist       -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_MEC.hist      -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_DeltaRES.hist -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_OtherRES.hist -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_SoftDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_TrueDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_IntTypeSelection_Other.hist    -> Scale(data_pot/mc_pot);
}



// Reco objects with PDG breakdown
// ===============================

void Histograms::ScaleMCHists_ObjectPdg(const double mc_pot, const double data_pot) {
    m_mc_ObjectPdg.hist         -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Pi0.hist     -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Proton.hist  -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Neutron.hist -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Pion.hist    -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_EM.hist      -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Muon.hist    -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_OthPdg.hist  -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_MCXtalk.hist -> Scale(data_pot/mc_pot);
    m_mc_ObjectPdg_Overlay.hist -> Scale(data_pot/mc_pot);
}



// Efficiency components
// =====================

void Histograms::ScaleEffNumerator(const double mc_pot, const double data_pot) {
    m_mc_EffNumerator.hist          -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_QE.hist       -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_MEC.hist      -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_DeltaRES.hist -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_OtherRES.hist -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_SoftDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_TrueDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_EffNumerator_Other.hist    -> Scale(data_pot/mc_pot);
}


void Histograms::ScaleEffDenominator(const double mc_pot, const double data_pot) {
    m_mc_EffDenominator.hist          -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_QE.hist       -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_MEC.hist      -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_DeltaRES.hist -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_OtherRES.hist -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_SoftDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_TrueDIS.hist  -> Scale(data_pot/mc_pot);
    m_mc_EffDenominator_Other.hist    -> Scale(data_pot/mc_pot);
}



// Migration matrix
// ================

void Histograms::ScaleMigrationHists(const double mc_pot, const double data_pot) {
    m_mc_Migration.hist -> Scale(data_pot/mc_pot);
}



// Plastic sidebands in signal region
// ==================================

void Histograms::ScaleMCHists_PlasSB_In_SigReg(const double mc_pot, const double data_pot) {
    m_mc_RecoPb_In_SigReg.hist              -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_RecoPb_In_SigReg_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_RecoFe_In_SigReg.hist              -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_RecoFe_In_SigReg_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasUpSB_In_SigReg.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasBetwSB_In_SigReg.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasDownSB_In_SigReg.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
}



// Plastic sidebands in physics sidebands
// ======================================

void Histograms::ScaleMCHists_PlasSB_In_PhysSB(const double mc_pot, const double data_pot) {
    m_mc_PlasUpSB_In_PionBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasBetwSB_In_PionBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasDownSB_In_PionBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasUpSB_In_ProtonBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasBetwSB_In_ProtonBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasDownSB_In_ProtonBlobSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasUpSB_In_HighWSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasBetwSB_In_HighWSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
    
    m_mc_PlasDownSB_In_HighWSB.hist              -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.hist -> Scale(data_pot/mc_pot);
}



// Physics sidebands
// =================

void Histograms::ScaleMCHists_PhysSB(const double mc_pot, const double data_pot) {
    m_mc_SigReg.hist                -> Scale(data_pot/mc_pot);
    m_mc_SigReg_Signal.hist         -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_SigReg_BackgrOther.hist    -> Scale(data_pot/mc_pot);
    
    m_mc_PionBlobSB.hist                -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_Signal.hist         -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_PionBlobSB_BackgrOther.hist    -> Scale(data_pot/mc_pot);
    
    m_mc_ProtonBlobSB.hist                -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_Signal.hist         -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_ProtonBlobSB_BackgrOther.hist    -> Scale(data_pot/mc_pot);
    
    m_mc_HighWSB.hist                -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_Signal.hist         -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrPi0HighW.hist -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrQElike.hist   -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrPionProd.hist -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrPlasUp.hist   -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrPlasBetw.hist -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrPlasDown.hist -> Scale(data_pot/mc_pot);
    m_mc_HighWSB_BackgrOther.hist    -> Scale(data_pot/mc_pot);
}





// ==========================================================================
//  WRITE HISTOGRAMS TO FILE
// ==========================================================================

// Event selection
// ===============

// Monte Carlo
void Histograms::WriteMCHists_Selection(TFile& fout) const {
    fout.cd();
    
    m_mc_Selection.hist        -> Write();
    m_mc_Selection_Signal.hist -> Write();
    m_mc_Selection_Backgr.hist -> Write();
    
    m_mc_Selection_BackgrPi0HighW.hist -> Write();
    m_mc_Selection_BackgrQElike.hist   -> Write();
    m_mc_Selection_BackgrPionProd.hist -> Write();
    m_mc_Selection_BackgrPlasUp.hist   -> Write();
    m_mc_Selection_BackgrPlasBetw.hist -> Write();
    m_mc_Selection_BackgrPlasDown.hist -> Write();
    m_mc_Selection_BackgrOther.hist    -> Write();
}


// Data
void Histograms::WriteDataHists_Selection(TFile& fout) const {
    fout.cd();
    m_data_Selection -> Write();
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms::WriteMCHists_MatSelection(TFile& fout) const {
    fout.cd();
    
    m_mc_MatSelection.hist              -> Write();
    m_mc_MatSelection_TrueTgt4Pb.hist   -> Write();
    m_mc_MatSelection_TrueTgt5Pb.hist   -> Write();
    m_mc_MatSelection_TrueTgt5Fe.hist   -> Write();
    m_mc_MatSelection_TruePlasUp.hist   -> Write();
    m_mc_MatSelection_TruePlasBetw.hist -> Write();
    m_mc_MatSelection_TruePlasDown.hist -> Write();
    m_mc_MatSelection_TrueOtherMat.hist -> Write();
}


// Data
void Histograms::WriteDataHists_MatSelection(TFile& fout) const {
    fout.cd();
    m_data_MatSelection -> Write();
}



// Event selection with interaction type breakdown
// ===============================================

// Monte Carlo
void Histograms::WriteMCHists_IntTypeSelection(TFile& fout) const {
    fout.cd();
    
    m_mc_IntTypeSelection.hist          -> Write();
    m_mc_IntTypeSelection_QE.hist       -> Write();
    m_mc_IntTypeSelection_MEC.hist      -> Write();
    m_mc_IntTypeSelection_DeltaRES.hist -> Write();
    m_mc_IntTypeSelection_OtherRES.hist -> Write();
    m_mc_IntTypeSelection_SoftDIS.hist  -> Write();
    m_mc_IntTypeSelection_TrueDIS.hist  -> Write();
    m_mc_IntTypeSelection_Other.hist    -> Write();
}


// Data
void Histograms::WriteDataHists_IntTypeSelection(TFile& fout) const {
    fout.cd();
    m_data_IntTypeSelection -> Write();
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms::WriteMCHists_ObjectPdg(TFile& fout) const {
    fout.cd();
    
    m_mc_ObjectPdg.hist         -> Write();
    m_mc_ObjectPdg_Pi0.hist     -> Write();
    m_mc_ObjectPdg_Proton.hist  -> Write();
    m_mc_ObjectPdg_Neutron.hist -> Write();
    m_mc_ObjectPdg_Pion.hist    -> Write();
    m_mc_ObjectPdg_EM.hist      -> Write();
    m_mc_ObjectPdg_Muon.hist    -> Write();
    m_mc_ObjectPdg_OthPdg.hist  -> Write();
    m_mc_ObjectPdg_MCXtalk.hist -> Write();
    m_mc_ObjectPdg_Overlay.hist -> Write();
}


// Data
void Histograms::WriteDataHists_ObjectPdg(TFile& fout) const {
    fout.cd();
    m_data_ObjectPdg -> Write();
}



// Efficiency components
// =====================

// Numerator
void Histograms::WriteEffNumerator(TFile& fout) const {
    fout.cd();
    m_mc_EffNumerator.hist          -> Write();
    m_mc_EffNumerator_QE.hist       -> Write();
    m_mc_EffNumerator_MEC.hist      -> Write();
    m_mc_EffNumerator_DeltaRES.hist -> Write();
    m_mc_EffNumerator_OtherRES.hist -> Write();
    m_mc_EffNumerator_SoftDIS.hist  -> Write();
    m_mc_EffNumerator_TrueDIS.hist  -> Write();
    m_mc_EffNumerator_Other.hist    -> Write();
}


// Denominator
void Histograms::WriteEffDenominator(TFile& fout) const {
    fout.cd();
    m_mc_EffDenominator.hist -> Write();
    m_mc_EffDenominator_QE.hist       -> Write();
    m_mc_EffDenominator_MEC.hist      -> Write();
    m_mc_EffDenominator_DeltaRES.hist -> Write();
    m_mc_EffDenominator_OtherRES.hist -> Write();
    m_mc_EffDenominator_SoftDIS.hist  -> Write();
    m_mc_EffDenominator_TrueDIS.hist  -> Write();
    m_mc_EffDenominator_Other.hist    -> Write();
}



// Migration matrix
// ================

void Histograms::WriteMigrationHists(TFile& fout) const {
    fout.cd();
    m_mc_Migration.hist -> Write();
}



// Plastic sidebands in signal region
// ==================================

// Monte Carlo
void Histograms::WriteMCHists_PlasSB_In_SigReg(TFile& fout) const {
    fout.cd();
    
    m_mc_RecoPb_In_SigReg.hist              -> Write();
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb.hist   -> Write();
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb.hist   -> Write();
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe.hist   -> Write();
    m_mc_RecoPb_In_SigReg_TruePlasUp.hist   -> Write();
    m_mc_RecoPb_In_SigReg_TruePlasBetw.hist -> Write();
    m_mc_RecoPb_In_SigReg_TruePlasDown.hist -> Write();
    m_mc_RecoPb_In_SigReg_TrueOtherMat.hist -> Write();
    
    m_mc_RecoFe_In_SigReg.hist              -> Write();
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb.hist   -> Write();
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb.hist   -> Write();
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe.hist   -> Write();
    m_mc_RecoFe_In_SigReg_TruePlasUp.hist   -> Write();
    m_mc_RecoFe_In_SigReg_TruePlasBetw.hist -> Write();
    m_mc_RecoFe_In_SigReg_TruePlasDown.hist -> Write();
    m_mc_RecoFe_In_SigReg_TrueOtherMat.hist -> Write();
    
    m_mc_PlasUpSB_In_SigReg.hist              -> Write();
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasUpSB_In_SigReg_TruePlasUp.hist   -> Write();
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw.hist -> Write();
    m_mc_PlasUpSB_In_SigReg_TruePlasDown.hist -> Write();
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat.hist -> Write();
    
    m_mc_PlasBetwSB_In_SigReg.hist              -> Write();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp.hist   -> Write();
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.hist -> Write();
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown.hist -> Write();
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.hist -> Write();
    
    m_mc_PlasDownSB_In_SigReg.hist              -> Write();
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasDownSB_In_SigReg_TruePlasUp.hist   -> Write();
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw.hist -> Write();
    m_mc_PlasDownSB_In_SigReg_TruePlasDown.hist -> Write();
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat.hist -> Write();
}


// Data
void Histograms::WriteDataHists_PlasSB_In_SigReg(TFile& fout) const {
    fout.cd();
    
    m_data_RecoPb_In_SigReg     -> Write();
    m_data_RecoFe_In_SigReg     -> Write();
    m_data_PlasUpSB_In_SigReg   -> Write();
    m_data_PlasBetwSB_In_SigReg -> Write();
    m_data_PlasDownSB_In_SigReg -> Write();
}



// Plastic sidebands in physics sidebands
// ======================================

// Monte Carlo
void Histograms::WriteMCHists_PlasSB_In_PhysSB(TFile& fout) const {
    fout.cd();
    
    m_mc_PlasUpSB_In_PionBlobSB.hist              -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasBetwSB_In_PionBlobSB.hist              -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasDownSB_In_PionBlobSB.hist              -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasUpSB_In_HighWSB.hist              -> Write();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp.hist   -> Write();
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.hist -> Write();
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown.hist -> Write();
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasBetwSB_In_HighWSB.hist              -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.hist   -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.hist -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.hist -> Write();
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasDownSB_In_HighWSB.hist              -> Write();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp.hist   -> Write();
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.hist -> Write();
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown.hist -> Write();
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasUpSB_In_ProtonBlobSB.hist              -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasBetwSB_In_ProtonBlobSB.hist              -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.hist -> Write();
    
    m_mc_PlasDownSB_In_ProtonBlobSB.hist              -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.hist   -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.hist   -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.hist   -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.hist   -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.hist -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.hist -> Write();
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.hist -> Write();
}


// Data
void Histograms::WriteDataHists_PlasSB_In_PhysSB(TFile& fout) const {
    fout.cd();
    
    m_data_PlasUpSB_In_PionBlobSB   -> Write();
    m_data_PlasBetwSB_In_PionBlobSB -> Write();
    m_data_PlasDownSB_In_PionBlobSB -> Write();
    
    m_data_PlasUpSB_In_ProtonBlobSB   -> Write();
    m_data_PlasBetwSB_In_ProtonBlobSB -> Write();
    m_data_PlasDownSB_In_ProtonBlobSB -> Write();
    
    m_data_PlasUpSB_In_HighWSB   -> Write();
    m_data_PlasBetwSB_In_HighWSB -> Write();
    m_data_PlasDownSB_In_HighWSB -> Write();
}



// Physics sidebands
// =================

// Monte Carlo
void Histograms::WriteMCHists_PhysSB(TFile& fout) const {
    fout.cd();
    
    m_mc_SigReg.hist                -> Write();
    m_mc_SigReg_Signal.hist         -> Write();
    m_mc_SigReg_BackgrPi0HighW.hist -> Write();
    m_mc_SigReg_BackgrQElike.hist   -> Write();
    m_mc_SigReg_BackgrPionProd.hist -> Write();
    m_mc_SigReg_BackgrPlasUp.hist   -> Write();
    m_mc_SigReg_BackgrPlasBetw.hist -> Write();
    m_mc_SigReg_BackgrPlasDown.hist -> Write();
    m_mc_SigReg_BackgrOther.hist    -> Write();
    
    m_mc_PionBlobSB.hist                -> Write();
    m_mc_PionBlobSB_Signal.hist         -> Write();
    m_mc_PionBlobSB_BackgrPi0HighW.hist -> Write();
    m_mc_PionBlobSB_BackgrQElike.hist   -> Write();
    m_mc_PionBlobSB_BackgrPionProd.hist -> Write();
    m_mc_PionBlobSB_BackgrPlasUp.hist   -> Write();
    m_mc_PionBlobSB_BackgrPlasBetw.hist -> Write();
    m_mc_PionBlobSB_BackgrPlasDown.hist -> Write();
    m_mc_PionBlobSB_BackgrOther.hist    -> Write();
    
    m_mc_ProtonBlobSB.hist                -> Write();
    m_mc_ProtonBlobSB_Signal.hist         -> Write();
    m_mc_ProtonBlobSB_BackgrPi0HighW.hist -> Write();
    m_mc_ProtonBlobSB_BackgrQElike.hist   -> Write();
    m_mc_ProtonBlobSB_BackgrPionProd.hist -> Write();
    m_mc_ProtonBlobSB_BackgrPlasUp.hist   -> Write();
    m_mc_ProtonBlobSB_BackgrPlasBetw.hist -> Write();
    m_mc_ProtonBlobSB_BackgrPlasDown.hist -> Write();
    m_mc_ProtonBlobSB_BackgrOther.hist    -> Write();
    
    m_mc_HighWSB.hist                -> Write();
    m_mc_HighWSB_Signal.hist         -> Write();
    m_mc_HighWSB_BackgrPi0HighW.hist -> Write();
    m_mc_HighWSB_BackgrQElike.hist   -> Write();
    m_mc_HighWSB_BackgrPionProd.hist -> Write();
    m_mc_HighWSB_BackgrPlasUp.hist   -> Write();
    m_mc_HighWSB_BackgrPlasBetw.hist -> Write();
    m_mc_HighWSB_BackgrPlasDown.hist -> Write();
    m_mc_HighWSB_BackgrOther.hist    -> Write();
}


// Data
void Histograms::WriteDataHists_PhysSB(TFile& fout) const {
    fout.cd();
    
    m_data_SigReg       -> Write();
    m_data_PionBlobSB   -> Write();
    m_data_ProtonBlobSB -> Write();
    m_data_HighWSB      -> Write();
}



// MC plastic weights in signal region
// ===================================

void Histograms::WriteMCWeights_PlasBackgr_In_SigReg(TFile& fout) const {
    fout.cd();
    
    m_mc_Weight_SigReg_TruePlasUp.hist   -> Write();
    m_mc_Weight_SigReg_TruePlasBetw.hist -> Write();
    m_mc_Weight_SigReg_TruePlasDown.hist -> Write();
}



// MC plastic weights in physics sidebands
// =======================================

void Histograms::WriteMCWeights_PlasBackgr_In_PhysSB(TFile& fout) const {
    fout.cd();
    
    m_mc_Weight_PionBlobSB_TruePlasUp.hist   -> Write();
    m_mc_Weight_PionBlobSB_TruePlasBetw.hist -> Write();
    m_mc_Weight_PionBlobSB_TruePlasDown.hist -> Write();
    
    m_mc_Weight_ProtonBlobSB_TruePlasUp.hist   -> Write();
    m_mc_Weight_ProtonBlobSB_TruePlasBetw.hist -> Write();
    m_mc_Weight_ProtonBlobSB_TruePlasDown.hist -> Write();
    
    m_mc_Weight_HighWSB_TruePlasUp.hist   -> Write();
    m_mc_Weight_HighWSB_TruePlasBetw.hist -> Write();
    m_mc_Weight_HighWSB_TruePlasDown.hist -> Write();
}



// MC physics weights
// ==================

void Histograms::WriteMCWeights_PhysBackgr(TFile& fout) const {
    fout.cd();
    
    m_mc_Weight_Signal.hist         -> Write();
    m_mc_Weight_BackgrPi0HighW.hist -> Write();
    m_mc_Weight_BackgrQElike.hist   -> Write();
    m_mc_Weight_BackgrPionProd.hist -> Write();
}





// ==========================================================================
//  LOAD HISTOGRAMS FROM FILE
// ==========================================================================

// Helper functions
// ================

// Load 1D HistWrapper
CVHW Histograms::LoadHWFromFile(TFile& fin, UniverseMap& error_bands,
                                std::string prefix,
                                std::string suffix)
{
    const bool do_erase_bands = false;
    MH1D* hist = (MH1D*)fin.Get(Form("%s%s%s", prefix.c_str(), m_label.c_str(), suffix.c_str()));
    
    if ( hist == 0 ) {
        std::cout << " ERROR LOADING HISTWRAPPER: " << m_label << " | RETURNING EMPTY OBJECT!!! " << std::endl;
        return CVHW();
    }
    
    else {
        TArrayD bins_array = *(hist->GetXaxis()->GetXbins());
        
        if ( bins_array.GetSize() == 0 ) {
            bins_array.Reset();
            bins_array = MakeUniformBinArray(hist->GetXaxis()->GetNbins(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
            hist = dynamic_cast<MH1D*>(hist->Rebin(bins_array.GetSize()-1, hist->GetName(), bins_array.GetArray()));
        }
        
        for ( int i = 0; i < NBins(); ++i ) {
            if ( m_bins_array[i] != bins_array[i] ) {
                std::cout << " WARNING WITH A BINNING MISMATCH FOR A HISTOGRAM " << prefix << m_label << suffix << std::endl;
                m_bins_array = bins_array;
                break;
            }
        }
        
        return CVHW(hist, error_bands, do_erase_bands);
    }
}


// Load 2D HistWrapper
CVH2DW Histograms::LoadH2DWFromFile(TFile& fin, UniverseMap& error_bands,
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
void Histograms::LoadMCHists_Selection(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Selection        = LoadHWFromFile(fin, error_bands, "mc_Selection_", "");
    m_mc_Selection_Signal = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_Signal");
    m_mc_Selection_Backgr = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_Backgr");
    
    m_mc_Selection_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrPi0HighW");
    m_mc_Selection_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrQElike");
    m_mc_Selection_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrPionProd");
    m_mc_Selection_BackgrPlasUp   = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrPlasUp");
    m_mc_Selection_BackgrPlasBetw = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrPlasBetw");
    m_mc_Selection_BackgrPlasDown = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrPlasDown");
    m_mc_Selection_BackgrOther    = LoadHWFromFile(fin, error_bands, "mc_Selection_", "_BackgrOther");
}


// Data
void Histograms::LoadDataHists_Selection(TFile& fin) {
    m_data_Selection = (MH1D*)fin.Get(Form("data_Selection_%s", m_label.c_str()));
}



// Event selection with material breakdown
// =======================================

// Monte Carlo
void Histograms::LoadMCHists_MatSelection(TFile& fin, UniverseMap& error_bands)
{
    m_mc_MatSelection              = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "");
    m_mc_MatSelection_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TrueTgt4Pb");
    m_mc_MatSelection_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TrueTgt5Pb");
    m_mc_MatSelection_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TrueTgt5Fe");
    m_mc_MatSelection_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TruePlasUp");
    m_mc_MatSelection_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TruePlasBetw");
    m_mc_MatSelection_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TruePlasDown");
    m_mc_MatSelection_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_MatSelection_", "_TrueOtherMat");
}


// Data
void Histograms::LoadDataHists_MatSelection(TFile& fin) {
    m_data_MatSelection = (MH1D*)fin.Get(Form("data_MatSelection_%s", m_label.c_str()));
}



// Event selection with interaction type breakdown
// ===============================================

// Monte Carlo
void Histograms::LoadMCHists_IntTypeSelection(TFile& fin, UniverseMap& error_bands)
{
    m_mc_IntTypeSelection          = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "");
    m_mc_IntTypeSelection_QE       = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_QE");
    m_mc_IntTypeSelection_MEC      = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_MEC");
    m_mc_IntTypeSelection_DeltaRES = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_DeltaRES");
    m_mc_IntTypeSelection_OtherRES = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_OtherRES");
    m_mc_IntTypeSelection_SoftDIS  = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_SoftDIS");
    m_mc_IntTypeSelection_TrueDIS  = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_TrueDIS");
    m_mc_IntTypeSelection_Other    = LoadHWFromFile(fin, error_bands, "mc_IntTypeSelection_", "_Other");
}


// Data
void Histograms::LoadDataHists_IntTypeSelection(TFile& fin) {
    m_data_IntTypeSelection = (MH1D*)fin.Get(Form("data_IntTypeSelection_%s", m_label.c_str()));
}



// Reco objects with PDG breakdown
// ===============================

// Monte Carlo
void Histograms::LoadMCHists_ObjectPdg(TFile& fin, UniverseMap& error_bands)
{
    m_mc_ObjectPdg         = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "");
    m_mc_ObjectPdg_Pi0     = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Pi0");
    m_mc_ObjectPdg_Proton  = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Proton");
    m_mc_ObjectPdg_Neutron = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Neutron");
    m_mc_ObjectPdg_Pion    = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Pion");
    m_mc_ObjectPdg_EM      = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_EM");
    m_mc_ObjectPdg_Muon    = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Muon");
    m_mc_ObjectPdg_OthPdg  = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_OthPdg");
    m_mc_ObjectPdg_MCXtalk = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_MCXtalk");
    m_mc_ObjectPdg_Overlay = LoadHWFromFile(fin, error_bands, "mc_ObjectPdg_", "_Overlay");
}


// Data
void Histograms::LoadDataHists_ObjectPdg(TFile& fin) {
    m_data_ObjectPdg = (MH1D*)fin.Get(Form("data_ObjectPdg_%s", m_label.c_str()));
}



// Efficiency components
// =====================

// Numerator
void Histograms::LoadEffNumerator(TFile& fin, UniverseMap& error_bands) {
    m_mc_EffNumerator          = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "");
    m_mc_EffNumerator_QE       = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_QE");
    m_mc_EffNumerator_MEC      = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_MEC");
    m_mc_EffNumerator_DeltaRES = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_DeltaRES");
    m_mc_EffNumerator_OtherRES = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_OtherRES");
    m_mc_EffNumerator_SoftDIS  = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_SoftDIS");
    m_mc_EffNumerator_TrueDIS  = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_TrueDIS");
    m_mc_EffNumerator_Other    = LoadHWFromFile(fin, error_bands, "mc_EffNumerator_", "_Other");
}


// Denominator
void Histograms::LoadEffDenominator(TFile& fin, UniverseMap& error_bands) {
    m_mc_EffDenominator          = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "");
    m_mc_EffDenominator_QE       = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_QE");
    m_mc_EffDenominator_MEC      = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_MEC");
    m_mc_EffDenominator_DeltaRES = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_DeltaRES");
    m_mc_EffDenominator_OtherRES = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_OtherRES");
    m_mc_EffDenominator_SoftDIS  = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_SoftDIS");
    m_mc_EffDenominator_TrueDIS  = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_TrueDIS");
    m_mc_EffDenominator_Other    = LoadHWFromFile(fin, error_bands, "mc_EffDenominator_", "_Other");
}



// Migration matrix
// ================

void Histograms::LoadMigrationHists(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Migration = LoadH2DWFromFile(fin, error_bands, "mc_Migration_", "");
}



// Plastic sidebands in signal region
// ==================================

// Monte Carlo
void Histograms::LoadMCHists_PlasSB_In_SigReg(TFile& fin, UniverseMap& error_bands)
{
    m_mc_RecoPb_In_SigReg              = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg");
    m_mc_RecoPb_In_SigReg_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TrueTgt4Pb");
    m_mc_RecoPb_In_SigReg_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TrueTgt5Pb");
    m_mc_RecoPb_In_SigReg_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TrueTgt5Fe");
    m_mc_RecoPb_In_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TruePlasUp");
    m_mc_RecoPb_In_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TruePlasBetw");
    m_mc_RecoPb_In_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TruePlasDown");
    m_mc_RecoPb_In_SigReg_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_RecoPb_In_SigReg_TrueOtherMat");
    
    m_mc_RecoFe_In_SigReg              = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg");
    m_mc_RecoFe_In_SigReg_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TrueTgt4Pb");
    m_mc_RecoFe_In_SigReg_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TrueTgt5Pb");
    m_mc_RecoFe_In_SigReg_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TrueTgt5Fe");
    m_mc_RecoFe_In_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TruePlasUp");
    m_mc_RecoFe_In_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TruePlasBetw");
    m_mc_RecoFe_In_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TruePlasDown");
    m_mc_RecoFe_In_SigReg_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_RecoFe_In_SigReg_TrueOtherMat");
    
    m_mc_PlasUpSB_In_SigReg              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg");
    m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TrueTgt4Pb");
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TrueTgt5Pb");
    m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TrueTgt5Fe");
    m_mc_PlasUpSB_In_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TruePlasUp");
    m_mc_PlasUpSB_In_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TruePlasBetw");
    m_mc_PlasUpSB_In_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TruePlasDown");
    m_mc_PlasUpSB_In_SigReg_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_SigReg_TrueOtherMat");
    
    m_mc_PlasBetwSB_In_SigReg              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TrueTgt4Pb");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TrueTgt5Pb");
    m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TrueTgt5Fe");
    m_mc_PlasBetwSB_In_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TruePlasUp");
    m_mc_PlasBetwSB_In_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TruePlasBetw");
    m_mc_PlasBetwSB_In_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TruePlasDown");
    m_mc_PlasBetwSB_In_SigReg_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_SigReg_TrueOtherMat");
    
    m_mc_PlasDownSB_In_SigReg              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg");
    m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TrueTgt4Pb");
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TrueTgt5Pb");
    m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TrueTgt5Fe");
    m_mc_PlasDownSB_In_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TruePlasUp");
    m_mc_PlasDownSB_In_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TruePlasBetw");
    m_mc_PlasDownSB_In_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TruePlasDown");
    m_mc_PlasDownSB_In_SigReg_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_SigReg_TrueOtherMat");
}


// Data
void Histograms::LoadDataHists_PlasSB_In_SigReg(TFile& fin)
{
    m_data_RecoPb_In_SigReg     = (MH1D*)fin.Get(Form("data_%s_RecoPb_In_SigReg",     m_label.c_str()));
    m_data_RecoFe_In_SigReg     = (MH1D*)fin.Get(Form("data_%s_RecoFe_In_SigReg",     m_label.c_str()));
    m_data_PlasUpSB_In_SigReg   = (MH1D*)fin.Get(Form("data_%s_PlasUpSB_In_SigReg",   m_label.c_str()));
    m_data_PlasBetwSB_In_SigReg = (MH1D*)fin.Get(Form("data_%s_PlasBetwSB_In_SigReg", m_label.c_str()));
    m_data_PlasDownSB_In_SigReg = (MH1D*)fin.Get(Form("data_%s_PlasDownSB_In_SigReg", m_label.c_str()));
}



// Plastic sidebands in physics sidebands
// ======================================

// Monte Carlo
void Histograms::LoadMCHists_PlasSB_In_PhysSB(TFile& fin, UniverseMap& error_bands)
{
    m_mc_PlasUpSB_In_PionBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TrueTgt4Pb");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TrueTgt5Pb");
    m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TrueTgt5Fe");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TruePlasUp");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TruePlasBetw");
    m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TruePlasDown");
    m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_PionBlobSB_TrueOtherMat");
    
    m_mc_PlasBetwSB_In_PionBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TruePlasUp");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TruePlasBetw");
    m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TruePlasDown");
    m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_PionBlobSB_TrueOtherMat");
    
    m_mc_PlasDownSB_In_PionBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TrueTgt4Pb");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TrueTgt5Pb");
    m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TrueTgt5Fe");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TruePlasUp");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TruePlasBetw");
    m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TruePlasDown");
    m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_PionBlobSB_TrueOtherMat");
    
    m_mc_PlasUpSB_In_ProtonBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TruePlasUp");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TruePlasBetw");
    m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TruePlasDown");
    m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_ProtonBlobSB_TrueOtherMat");
    
    m_mc_PlasBetwSB_In_ProtonBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TruePlasUp");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TruePlasDown");
    m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat");
    
    m_mc_PlasDownSB_In_ProtonBlobSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TruePlasUp");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TruePlasBetw");
    m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TruePlasDown");
    m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_ProtonBlobSB_TrueOtherMat");
    
    m_mc_PlasUpSB_In_HighWSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TrueTgt4Pb");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TrueTgt5Pb");
    m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TrueTgt5Fe");
    m_mc_PlasUpSB_In_HighWSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TruePlasUp");
    m_mc_PlasUpSB_In_HighWSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TruePlasBetw");
    m_mc_PlasUpSB_In_HighWSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TruePlasDown");
    m_mc_PlasUpSB_In_HighWSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasUpSB_In_HighWSB_TrueOtherMat");
    
    m_mc_PlasBetwSB_In_HighWSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TrueTgt4Pb");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TrueTgt5Pb");
    m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TrueTgt5Fe");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TruePlasUp");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TruePlasBetw");
    m_mc_PlasBetwSB_In_HighWSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TruePlasDown");
    m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasBetwSB_In_HighWSB_TrueOtherMat");
    
    m_mc_PlasDownSB_In_HighWSB              = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TrueTgt4Pb");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TrueTgt5Pb");
    m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TrueTgt5Fe");
    m_mc_PlasDownSB_In_HighWSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TruePlasUp");
    m_mc_PlasDownSB_In_HighWSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TruePlasBetw");
    m_mc_PlasDownSB_In_HighWSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TruePlasDown");
    m_mc_PlasDownSB_In_HighWSB_TrueOtherMat = LoadHWFromFile(fin, error_bands, "mc_", "_PlasDownSB_In_HighWSB_TrueOtherMat");
}


// Data
void Histograms::LoadDataHists_PlasSB_In_PhysSB(TFile& fin)
{
    m_data_PlasUpSB_In_PionBlobSB   = (MH1D*)fin.Get(Form("data_%s_PlasUpSB_In_PionBlobSB",   m_label.c_str()));
    m_data_PlasBetwSB_In_PionBlobSB = (MH1D*)fin.Get(Form("data_%s_PlasBetwSB_In_PionBlobSB", m_label.c_str()));
    m_data_PlasDownSB_In_PionBlobSB = (MH1D*)fin.Get(Form("data_%s_PlasDownSB_In_PionBlobSB", m_label.c_str()));
    
    m_data_PlasUpSB_In_ProtonBlobSB   = (MH1D*)fin.Get(Form("data_%s_PlasUpSB_In_ProtonBlobSB",   m_label.c_str()));
    m_data_PlasBetwSB_In_ProtonBlobSB = (MH1D*)fin.Get(Form("data_%s_PlasBetwSB_In_ProtonBlobSB", m_label.c_str()));
    m_data_PlasDownSB_In_ProtonBlobSB = (MH1D*)fin.Get(Form("data_%s_PlasDownSB_In_ProtonBlobSB", m_label.c_str()));
    
    m_data_PlasUpSB_In_HighWSB   = (MH1D*)fin.Get(Form("data_%s_PlasUpSB_In_HighWSB",   m_label.c_str()));
    m_data_PlasBetwSB_In_HighWSB = (MH1D*)fin.Get(Form("data_%s_PlasBetwSB_In_HighWSB", m_label.c_str()));
    m_data_PlasDownSB_In_HighWSB = (MH1D*)fin.Get(Form("data_%s_PlasDownSB_In_HighWSB", m_label.c_str()));
}



// Physics sidebands
// =================

// Monte Carlo
void Histograms::LoadMCHists_PhysSB(TFile& fin, UniverseMap& error_bands)
{
    m_mc_SigReg                = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg");
    m_mc_SigReg_Signal         = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_Signal");
    m_mc_SigReg_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrPi0HighW");
    m_mc_SigReg_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrQElike");
    m_mc_SigReg_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrPionProd");
    m_mc_SigReg_BackgrPlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrPlasUp");
    m_mc_SigReg_BackgrPlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrPlasBetw");
    m_mc_SigReg_BackgrPlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrPlasDown");
    m_mc_SigReg_BackgrOther    = LoadHWFromFile(fin, error_bands, "mc_", "_SigReg_BackgrOther");
    
    m_mc_PionBlobSB                = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB");
    m_mc_PionBlobSB_Signal         = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_Signal");
    m_mc_PionBlobSB_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrPi0HighW");
    m_mc_PionBlobSB_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrQElike");
    m_mc_PionBlobSB_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrPionProd");
    m_mc_PionBlobSB_BackgrPlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrPlasUp");
    m_mc_PionBlobSB_BackgrPlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrPlasBetw");
    m_mc_PionBlobSB_BackgrPlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrPlasDown");
    m_mc_PionBlobSB_BackgrOther    = LoadHWFromFile(fin, error_bands, "mc_", "_PionBlobSB_BackgrOther");
    
    m_mc_ProtonBlobSB                = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB");
    m_mc_ProtonBlobSB_Signal         = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_Signal");
    m_mc_ProtonBlobSB_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrPi0HighW");
    m_mc_ProtonBlobSB_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrQElike");
    m_mc_ProtonBlobSB_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrPionProd");
    m_mc_ProtonBlobSB_BackgrPlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrPlasUp");
    m_mc_ProtonBlobSB_BackgrPlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrPlasBetw");
    m_mc_ProtonBlobSB_BackgrPlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrPlasDown");
    m_mc_ProtonBlobSB_BackgrOther    = LoadHWFromFile(fin, error_bands, "mc_", "_ProtonBlobSB_BackgrOther");
    
    m_mc_HighWSB                = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB");
    m_mc_HighWSB_Signal         = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_Signal");
    m_mc_HighWSB_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrPi0HighW");
    m_mc_HighWSB_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrQElike");
    m_mc_HighWSB_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrPionProd");
    m_mc_HighWSB_BackgrPlasUp   = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrPlasUp");
    m_mc_HighWSB_BackgrPlasBetw = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrPlasBetw");
    m_mc_HighWSB_BackgrPlasDown = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrPlasDown");
    m_mc_HighWSB_BackgrOther    = LoadHWFromFile(fin, error_bands, "mc_", "_HighWSB_BackgrOther");
}


// Data
void Histograms::LoadDataHists_PhysSB(TFile& fin)
{
    m_data_SigReg       = (MH1D*)fin.Get(Form("data_%s_SigReg",       m_label.c_str()));
    m_data_PionBlobSB   = (MH1D*)fin.Get(Form("data_%s_PionBlobSB",   m_label.c_str()));
    m_data_ProtonBlobSB = (MH1D*)fin.Get(Form("data_%s_ProtonBlobSB", m_label.c_str()));
    m_data_HighWSB      = (MH1D*)fin.Get(Form("data_%s_HighWSB",      m_label.c_str()));
}



// MC plastic weights in signal region
// ===================================

void Histograms::LoadMCWeights_PlasBackgr_In_SigReg(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Weight_SigReg_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_SigReg_TruePlasUp");
    m_mc_Weight_SigReg_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_SigReg_TruePlasBetw");
    m_mc_Weight_SigReg_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_SigReg_TruePlasDown");
}



// MC plastic weights in physics sidebands
// =======================================

void Histograms::LoadMCWeights_PlasBackgr_In_PhysSB(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Weight_PionBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_PionBlobSB_TruePlasUp");
    m_mc_Weight_PionBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_PionBlobSB_TruePlasBetw");
    m_mc_Weight_PionBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_PionBlobSB_TruePlasDown");
    
    m_mc_Weight_ProtonBlobSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_ProtonBlobSB_TruePlasUp");
    m_mc_Weight_ProtonBlobSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_ProtonBlobSB_TruePlasBetw");
    m_mc_Weight_ProtonBlobSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_ProtonBlobSB_TruePlasDown");
    
    m_mc_Weight_HighWSB_TruePlasUp   = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_HighWSB_TruePlasUp");
    m_mc_Weight_HighWSB_TruePlasBetw = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_HighWSB_TruePlasBetw");
    m_mc_Weight_HighWSB_TruePlasDown = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_HighWSB_TruePlasDown");
}



// MC physics weights
// ==================

void Histograms::LoadMCWeights_PhysBackgr(TFile& fin, UniverseMap& error_bands)
{
    m_mc_Weight_Signal         = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_Signal");
    m_mc_Weight_BackgrPi0HighW = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_BackgrPi0HighW");
    m_mc_Weight_BackgrQElike   = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_BackgrQElike");
    m_mc_Weight_BackgrPionProd = LoadHWFromFile(fin, error_bands, "mc_Weight_", "_BackgrPionProd");
}


#endif  // Histograms_cxx