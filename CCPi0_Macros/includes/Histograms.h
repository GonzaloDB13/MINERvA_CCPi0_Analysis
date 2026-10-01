#ifndef Histograms_h
#define Histograms_h

#include "CVUniverse.h"
#include "Constants.h"   // typedefs MH1D, MH2D, CVHW, CVH2DW
#include "Binning.h"     // MakeUniformBinArray()
#include "util.h"        // uniq()

#include "TArrayD.h"
#include "TFile.h"



class Histograms
{
    public:
        
        // ==========================================================================
        //  CONSTRUCTORS
        // ==========================================================================
        
        // Default
        Histograms();
        
        
        // Uniform bin size
        Histograms(const std::string label,
                   const std::string xlabel,
                   const int nbins, const double xmin, const double xmax);
        
        
        // Variable bin size
        Histograms(const std::string label,
                   const std::string xlabel,
                   const TArrayD& bins_array);
        
        
        
        // ==========================================================================
        //  DATA MEMBERS
        // ==========================================================================
        
        // Basic data members
        std::string m_label;
        
        std::string m_xlabel;
        
        TArrayD m_bins_array;
        
        std::vector<double> m_bins_vector;
        
        
        // Histograms -- Event selection
        // =============================
        
        MH1D* m_data_Selection;  // Data
        CVHW  m_mc_Selection;    // MC
        
        CVHW m_mc_Selection_Signal;  // Signal
        CVHW m_mc_Selection_Backgr;  // Background
        
        CVHW m_mc_Selection_BackgrPi0HighW;  // High-W pi0 background
        CVHW m_mc_Selection_BackgrQElike;    // QE-like background
        CVHW m_mc_Selection_BackgrPionProd;  // Pion production background
        CVHW m_mc_Selection_BackgrPlasUp;    // Up. plastic background
        CVHW m_mc_Selection_BackgrPlasBetw;  // Betw. plastic background
        CVHW m_mc_Selection_BackgrPlasDown;  // Down. plastic background
        CVHW m_mc_Selection_BackgrOther;     // Other background
        
        
        // Histograms -- Event selection w/material breakdown
        // ==================================================
        
        MH1D* m_data_MatSelection;  // Data
        CVHW  m_mc_MatSelection;    // MC
        
        CVHW m_mc_MatSelection_TrueTgt4Pb;    // Pb of target 4
        CVHW m_mc_MatSelection_TrueTgt5Pb;    // Pb of target 5
        CVHW m_mc_MatSelection_TrueTgt5Fe;    // Fe of target 5
        CVHW m_mc_MatSelection_TruePlasUp;    // Up. plastic
        CVHW m_mc_MatSelection_TruePlasBetw;  // Betw. plastic
        CVHW m_mc_MatSelection_TruePlasDown;  // Down. plastic
        CVHW m_mc_MatSelection_TrueOtherMat;  // Other material
        
        
        // Histograms -- Event selection w/interaction type breakdown
        // ==========================================================
        
        MH1D* m_data_IntTypeSelection;  // Data
        CVHW  m_mc_IntTypeSelection;    // MC
        
        CVHW m_mc_IntTypeSelection_QE;        // CC QE
        CVHW m_mc_IntTypeSelection_MEC;       // CC MEC
        CVHW m_mc_IntTypeSelection_DeltaRES;  // CC Delta RES
        CVHW m_mc_IntTypeSelection_OtherRES;  // CC other RES
        CVHW m_mc_IntTypeSelection_SoftDIS;   // CC "soft" DIS
        CVHW m_mc_IntTypeSelection_TrueDIS;   // CC "true" DIS (W > 2 and Q2 > 1)
        CVHW m_mc_IntTypeSelection_Other;     // Other
        
        
        // Histograms -- Reconstructed objects w/PDG breakdown
        // ===================================================
        
        MH1D* m_data_ObjectPdg;  // Data
        CVHW  m_mc_ObjectPdg;    // MC
        
        CVHW m_mc_ObjectPdg_Pi0;      // True pi0
        CVHW m_mc_ObjectPdg_Proton;   // True proton
        CVHW m_mc_ObjectPdg_Neutron;  // True neutron
        CVHW m_mc_ObjectPdg_Pion;     // True charged pion
        CVHW m_mc_ObjectPdg_EM;       // True electron/photon
        CVHW m_mc_ObjectPdg_Muon;     // True muon
        CVHW m_mc_ObjectPdg_OthPdg;   // True other PDG
        CVHW m_mc_ObjectPdg_MCXtalk;  // MC X-talk
        CVHW m_mc_ObjectPdg_Overlay;  // Data overlay
        
        
        // Histograms -- Efficiency components
        // ===================================
        
        CVHW m_mc_EffNumerator;           // Efficiency numerator
        CVHW m_mc_EffNumerator_QE;        // CC QE
        CVHW m_mc_EffNumerator_MEC;       // CC MEC
        CVHW m_mc_EffNumerator_DeltaRES;  // CC delta resonance
        CVHW m_mc_EffNumerator_OtherRES;  // CC other resonance
        CVHW m_mc_EffNumerator_SoftDIS;   // CC "soft" DIS
        CVHW m_mc_EffNumerator_TrueDIS;   // CC "true" DIS (W > 2 and Q2 > 1)
        CVHW m_mc_EffNumerator_Other;     // Other interaction type
        
        CVHW m_mc_EffDenominator;           // Efficiency denominator
        CVHW m_mc_EffDenominator_QE;        // CC QE
        CVHW m_mc_EffDenominator_MEC;       // CC MEC
        CVHW m_mc_EffDenominator_DeltaRES;  // CC delta resonance
        CVHW m_mc_EffDenominator_OtherRES;  // CC other resonance
        CVHW m_mc_EffDenominator_SoftDIS;   // CC "soft" DIS
        CVHW m_mc_EffDenominator_TrueDIS;   // CC "true" DIS (W > 2 and Q2 > 1)
        CVHW m_mc_EffDenominator_Other;     // Other interaction type
        
        
        // Histograms -- Migration matrix
        // ==============================
        
        CVH2DW m_mc_Migration;  // Signal events after selection
        
        
        // Histograms -- Plastic sidebands in physics signal region
        // ========================================================
        /* Data format is: m_data_<Reco material or plastic sideband>_In_<Signal region>
         * MC format is:   m_mc_<Reco material or plastic sideband>_In_<Signal region>_<True material> */
        
        // Reconstructed Pb
        MH1D* m_data_RecoPb_In_SigReg;
        CVHW  m_mc_RecoPb_In_SigReg;
        CVHW  m_mc_RecoPb_In_SigReg_TrueTgt4Pb;
        CVHW  m_mc_RecoPb_In_SigReg_TrueTgt5Pb;
        CVHW  m_mc_RecoPb_In_SigReg_TrueTgt5Fe;
        CVHW  m_mc_RecoPb_In_SigReg_TruePlasUp;
        CVHW  m_mc_RecoPb_In_SigReg_TruePlasBetw;
        CVHW  m_mc_RecoPb_In_SigReg_TruePlasDown;
        CVHW  m_mc_RecoPb_In_SigReg_TrueOtherMat;
        
        // Reconstructed Fe
        MH1D* m_data_RecoFe_In_SigReg;
        CVHW  m_mc_RecoFe_In_SigReg;
        CVHW  m_mc_RecoFe_In_SigReg_TrueTgt4Pb;
        CVHW  m_mc_RecoFe_In_SigReg_TrueTgt5Pb;
        CVHW  m_mc_RecoFe_In_SigReg_TrueTgt5Fe;
        CVHW  m_mc_RecoFe_In_SigReg_TruePlasUp;
        CVHW  m_mc_RecoFe_In_SigReg_TruePlasBetw;
        CVHW  m_mc_RecoFe_In_SigReg_TruePlasDown;
        CVHW  m_mc_RecoFe_In_SigReg_TrueOtherMat;
        
        // Plastic up. target 4
        MH1D* m_data_PlasUpSB_In_SigReg;
        CVHW  m_mc_PlasUpSB_In_SigReg;
        CVHW  m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb;
        CVHW  m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb;
        CVHW  m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe;
        CVHW  m_mc_PlasUpSB_In_SigReg_TruePlasUp;
        CVHW  m_mc_PlasUpSB_In_SigReg_TruePlasBetw;
        CVHW  m_mc_PlasUpSB_In_SigReg_TruePlasDown;
        CVHW  m_mc_PlasUpSB_In_SigReg_TrueOtherMat;
        
        // Plastic between
        MH1D* m_data_PlasBetwSB_In_SigReg;
        CVHW  m_mc_PlasBetwSB_In_SigReg;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TruePlasUp;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TruePlasBetw;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TruePlasDown;
        CVHW  m_mc_PlasBetwSB_In_SigReg_TrueOtherMat;
        
        // Plastic down. target 5
        MH1D* m_data_PlasDownSB_In_SigReg;
        CVHW  m_mc_PlasDownSB_In_SigReg;
        CVHW  m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb;
        CVHW  m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb;
        CVHW  m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe;
        CVHW  m_mc_PlasDownSB_In_SigReg_TruePlasUp;
        CVHW  m_mc_PlasDownSB_In_SigReg_TruePlasBetw;
        CVHW  m_mc_PlasDownSB_In_SigReg_TruePlasDown;
        CVHW  m_mc_PlasDownSB_In_SigReg_TrueOtherMat;
        
        
        // Histograms -- Plastic sidebands in physics sidebands
        // ====================================================
        /* Data format is: m_data_<Plastic sideband>_In_<Physics sideband>
         * MC format is:   m_mc_<Plastic sideband>_In_<Physics sideband>_<True material> */
        
        // Pion-like shower sideband
        MH1D* m_data_PlasUpSB_In_PionBlobSB;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown;
        CVHW  m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat;
        
        MH1D* m_data_PlasBetwSB_In_PionBlobSB;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown;
        CVHW  m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat;
        
        MH1D* m_data_PlasDownSB_In_PionBlobSB;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown;
        CVHW  m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat;
        
        // Proton-like shower sideband
        MH1D* m_data_PlasUpSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown;
        CVHW  m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat;
        
        MH1D* m_data_PlasBetwSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown;
        CVHW  m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat;
        
        MH1D* m_data_PlasDownSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown;
        CVHW  m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat;
        
        // High-W sideband
        MH1D* m_data_PlasUpSB_In_HighWSB;
        CVHW  m_mc_PlasUpSB_In_HighWSB;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TruePlasUp;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TruePlasBetw;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TruePlasDown;
        CVHW  m_mc_PlasUpSB_In_HighWSB_TrueOtherMat;
        
        MH1D* m_data_PlasBetwSB_In_HighWSB;
        CVHW  m_mc_PlasBetwSB_In_HighWSB;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TruePlasUp;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TruePlasDown;
        CVHW  m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat;
        
        MH1D* m_data_PlasDownSB_In_HighWSB;
        CVHW  m_mc_PlasDownSB_In_HighWSB;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TruePlasUp;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TruePlasBetw;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TruePlasDown;
        CVHW  m_mc_PlasDownSB_In_HighWSB_TrueOtherMat;
        
        
        // Histograms -- Physics sidebands
        // ===============================
        /* Data format is: m_data_<Signal region or physics sideband>
         * MC format is:   m_mc_<Signal region or physics sideband>_<True signal or background> */
        
        // Signal region
        MH1D* m_data_SigReg;
        CVHW  m_mc_SigReg;
        CVHW  m_mc_SigReg_Signal;
        CVHW  m_mc_SigReg_BackgrPi0HighW;
        CVHW  m_mc_SigReg_BackgrQElike;
        CVHW  m_mc_SigReg_BackgrPionProd;
        CVHW  m_mc_SigReg_BackgrPlasUp;
        CVHW  m_mc_SigReg_BackgrPlasBetw;
        CVHW  m_mc_SigReg_BackgrPlasDown;
        CVHW  m_mc_SigReg_BackgrOther;
        
        // Pion-like shower sideband
        MH1D* m_data_PionBlobSB;
        CVHW  m_mc_PionBlobSB;
        CVHW  m_mc_PionBlobSB_Signal;
        CVHW  m_mc_PionBlobSB_BackgrPi0HighW;
        CVHW  m_mc_PionBlobSB_BackgrQElike;
        CVHW  m_mc_PionBlobSB_BackgrPionProd;
        CVHW  m_mc_PionBlobSB_BackgrPlasUp;
        CVHW  m_mc_PionBlobSB_BackgrPlasBetw;
        CVHW  m_mc_PionBlobSB_BackgrPlasDown;
        CVHW  m_mc_PionBlobSB_BackgrOther;
        
        // Proton-like shower sideband
        MH1D* m_data_ProtonBlobSB;
        CVHW  m_mc_ProtonBlobSB;
        CVHW  m_mc_ProtonBlobSB_Signal;
        CVHW  m_mc_ProtonBlobSB_BackgrPi0HighW;
        CVHW  m_mc_ProtonBlobSB_BackgrQElike;
        CVHW  m_mc_ProtonBlobSB_BackgrPionProd;
        CVHW  m_mc_ProtonBlobSB_BackgrPlasUp;
        CVHW  m_mc_ProtonBlobSB_BackgrPlasBetw;
        CVHW  m_mc_ProtonBlobSB_BackgrPlasDown;
        CVHW  m_mc_ProtonBlobSB_BackgrOther;
        
        // High-W sideband
        MH1D* m_data_HighWSB;
        CVHW  m_mc_HighWSB;
        CVHW  m_mc_HighWSB_Signal;
        CVHW  m_mc_HighWSB_BackgrPi0HighW;
        CVHW  m_mc_HighWSB_BackgrQElike;
        CVHW  m_mc_HighWSB_BackgrPionProd;
        CVHW  m_mc_HighWSB_BackgrPlasUp;
        CVHW  m_mc_HighWSB_BackgrPlasBetw;
        CVHW  m_mc_HighWSB_BackgrPlasDown;
        CVHW  m_mc_HighWSB_BackgrOther;
        
        
        // Histograms -- MC weights
        // ========================
        /* Format is:   m_mc_Weight_<Signal region or physics sideband>_<True background> */
        
        // Plastic background in signal region
        CVHW m_mc_Weight_SigReg_TruePlasUp;
        CVHW m_mc_Weight_SigReg_TruePlasBetw;
        CVHW m_mc_Weight_SigReg_TruePlasDown;
        
        // Plastic background in pion-like shower sideband
        CVHW m_mc_Weight_PionBlobSB_TruePlasUp;
        CVHW m_mc_Weight_PionBlobSB_TruePlasBetw;
        CVHW m_mc_Weight_PionBlobSB_TruePlasDown;
        
        // Plastic background in proton-like shower sideband
        CVHW m_mc_Weight_ProtonBlobSB_TruePlasUp;
        CVHW m_mc_Weight_ProtonBlobSB_TruePlasBetw;
        CVHW m_mc_Weight_ProtonBlobSB_TruePlasDown;
        
        // Plastic background in high-W sideband
        CVHW m_mc_Weight_HighWSB_TruePlasUp;
        CVHW m_mc_Weight_HighWSB_TruePlasBetw;
        CVHW m_mc_Weight_HighWSB_TruePlasDown;
        
        // Signal + physics background
        CVHW m_mc_Weight_Signal;
        CVHW m_mc_Weight_BackgrPi0HighW;
        CVHW m_mc_Weight_BackgrQElike;
        CVHW m_mc_Weight_BackgrPionProd;
        
        
        // Histograms -- Before cross-section extraction
        // =============================================
        /* The reason I create this variables is to alleviate the fact tha most of the tuned and not tuned distributions
         * are in different files and with ambiguous names that I don't want to mess with.
         * That's why I save all that I need for cross-section extraction in one file using these variables. */
        
        MH1D* m_data;
        
        MH1D* m_mc_Signal;  // Signal (not tuned)
        
        MH1D* m_mc_NonTuned;                 // Signal (not tuned) + not tuned background
        MH1D* m_mc_BackgrNonTuned;           // Tuned background
        MH1D* m_mc_BackgrNonTuned_Pi0HighW;
        MH1D* m_mc_BackgrNonTuned_QElike;
        MH1D* m_mc_BackgrNonTuned_PionProd;
        MH1D* m_mc_BackgrNonTuned_PlasUp;
        MH1D* m_mc_BackgrNonTuned_PlasBetw;
        MH1D* m_mc_BackgrNonTuned_PlasDown;
        MH1D* m_mc_BackgrNonTuned_Other;
        
        MH1D* m_mc_Tuned;                 // Signal (not tuned) + tuned background
        MH1D* m_mc_BackgrTuned;           // Tuned background
        MH1D* m_mc_BackgrTuned_Pi0HighW;
        MH1D* m_mc_BackgrTuned_QElike;
        MH1D* m_mc_BackgrTuned_PionProd;
        MH1D* m_mc_BackgrTuned_PlasUp;
        MH1D* m_mc_BackgrTuned_PlasBetw;
        MH1D* m_mc_BackgrTuned_PlasDown;
        MH1D* m_mc_BackgrTuned_Other;
        
        MH1D* m_EffNumerator;    // Basically the same physics that is calculated before,
        MH1D* m_EffDenominator;  // but in MnvH1D form instead of ChainWrapper
        MH1D* m_Efficiency;
        
        MH2D* m_MigrationMatrix;  // Similar, same physics but in a different format
        
        
        // Histograms -- After cross section extraction
        // ============================================
        
        // Background subtraction
        MH1D* m_mc_BackgrSubtr;
        MH1D* m_data_BackgrSubtr;
        
        // Unfolding
        MH1D* m_mc_Folded;
        MH1D* m_data_Folded;
        
        MH1D* m_mc_Unfolded;
        MH1D* m_data_Unfolded;
        
        // Efficiency correction
        MH1D* m_mc_EffCorrected;
        MH1D* m_data_EffCorrected;
        
        // Cross section
        MH1D* m_mc_CrossSection;
        MH1D* m_data_CrossSection;
        
        
        
        // ==========================================================================
        //  FUNCTIONS
        // ==========================================================================
        
        // Basic functions
        int NBins()   const { return m_bins_array.GetSize()-1; }
        double XMin() const { return m_bins_array[0]; }
        double XMax() const { return m_bins_array[NBins()]; }
        
        void PrintBinning() const {
            for( int i = 0; i <= NBins(); ++i ) std::cout << m_bins_array[i] << " ";
            std::cout << std::endl;
        }
        
        
        
        // =======================================================================================
        //  INITIALIZE HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        template<typename T>
        void InitMCHists_Selection(T systematic_univs);
        
        void InitDataHists_Selection();
        
        
        // Event selection with material breakdown
        template<typename T>
        void InitMCHists_MatSelection(T systematic_univs);
        
        void InitDataHists_MatSelection();
        
        
        // Event selection with interaction type breakdown
        template<typename T>
        void InitMCHists_IntTypeSelection(T systematic_univs);
        
        void InitDataHists_IntTypeSelection();
        
        
        // Reco objects with PDG breakdown
        template<typename T>
        void InitMCHists_ObjectPdg(T systematic_univs);
        
        void InitDataHists_ObjectPdg();
        
        
        // Efficiency components
        template<typename T>
        void InitEffNumerator(T systematic_univs);
        
        template<typename T>
        void InitEffDenominator(T systematic_univs_truth);
        
        
        // Migration matrix
        template<typename T>
        void InitMigrationHists(T systematic_univs);
        
        
        // Plastic sidebands in signal region
        template<typename T>
        void InitMCHists_PlasSB_In_SigReg(T systematic_univs);
        
        void InitDataHists_PlasSB_In_SigReg();
        
        
        // Plastic sidebands in physics sidebands
        template<typename T>
        void InitMCHists_PlasSB_In_PhysSB(T systematic_univs);
        
        void InitDataHists_PlasSB_In_PhysSB();
        
        
        // Physics sidebands
        template<typename T>
        void InitMCHists_PhysSB(T systematic_univs);
        
        void InitDataHists_PhysSB();
        
        
        // MC tuning weights
        template<typename T>
        void InitMCWeights_PlasBackgr_In_SigReg(T systematic_univs);
        
        template<typename T>
        void InitMCWeights_PlasBackgr_In_PhysSB(T systematic_univs);
        
        template<typename T>
        void InitMCWeights_PhysBackgr(T systematic_univs);
        
        
        
        
        // =======================================================================================
        //  SYNCHRONIZE MC CV HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void SyncMCHists_Selection();
        
        
        // Event selection with material breakdown
        void SyncMCHists_MatSelection();
        
        
        // Event selection with interaction type breakdown
        void SyncMCHists_IntTypeSelection();
        
        
        // Reco objects with PDG breakdown
        void SyncMCHists_ObjectPdg();
        
        
        // Efficiency components
        void SyncEffNumerator();
        void SyncEffDenominator();
        
        
        // Migration matrix
        void SyncMigrationHists();
        
        
        // Plastic sidebands in signal region
        void SyncMCHists_PlasSB_In_SigReg();
        
        
        // Plastic sidebands in physics sidebands
        void SyncMCHists_PlasSB_In_PhysSB();
        
        
        // Physics sidebands
        void SyncMCHists_PhysSB();
        
        
        // MC tuning weights
        void SyncMCWeights_PlasBackgr_In_SigReg();
        void SyncMCWeights_PlasBackgr_In_PhysSB();
        void SyncMCWeights_PhysBackgr();
        
        
        
        // =======================================================================================
        //  BIN WIDTH NORMALIZE HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void BinWidthNormMCHists_Selection();
        void BinWidthNormDataHists_Selection();
        
        
        // Event selection with material breakdown
        void BinWidthNormMCHists_MatSelection();
        void BinWidthNormDataHists_MatSelection();
        
        
        // Event selection with interaction type breakdown
        void BinWidthNormMCHists_IntTypeSelection();
        void BinWidthNormDataHists_IntTypeSelection();
        
        
        // Reco objects with PDG breakdown
        void BinWidthNormMCHists_ObjectPdg();
        void BinWidthNormDataHists_ObjectPdg();
        
        
        // Efficiency components
        void BinWidthNormEffNumerator();
        void BinWidthNormEffDenominator();
        
        
        // Migration matrix
        void BinWidthNormMigrationHists();
        
        
        // Plastic sidebands in signal region
        void BinWidthNormMCHists_PlasSB_In_SigReg();
        void BinWidthNormDataHists_PlasSB_In_SigReg();
        
        
        // Plastic sidebands in physics sidebands
        void BinWidthNormMCHists_PlasSB_In_PhysSB();
        void BinWidthNormDataHists_PlasSB_In_PhysSB();
        
        
        // Physics sidebands
        void BinWidthNormMCHists_PhysSB();
        void BinWidthNormDataHists_PhysSB();
        
        
        
        // =======================================================================================
        //  SCALE MC HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void ScaleMCHists_Selection(const double mc_pot, const double data_pot);
        
        
        // Event selection with material breakdown
        void ScaleMCHists_MatSelection(const double mc_pot, const double data_pot);
        
        
        // Event selection with interaction type breakdown
        void ScaleMCHists_IntTypeSelection(const double mc_pot, const double data_pot);
        
        
        // Reco objects with PDG breakdown
        void ScaleMCHists_ObjectPdg(const double mc_pot, const double data_pot);
        
        
        // Efficiency components
        void ScaleEffNumerator(const double mc_pot, const double data_pot);
        void ScaleEffDenominator(const double mc_pot, const double data_pot);
        
        
        // Migration matrix
        void ScaleMigrationHists(const double mc_pot, const double data_pot);
        
        
        // Plastic sidebands in signal region
        void ScaleMCHists_PlasSB_In_SigReg(const double mc_pot, const double data_pot);
        
        
        // Plastic sidebands in physics sidebands
        void ScaleMCHists_PlasSB_In_PhysSB(const double mc_pot, const double data_pot);
        
        
        // Physics sidebands
        void ScaleMCHists_PhysSB(const double mc_pot, const double data_pot);
        
        
        
        // =======================================================================================
        //  WRITE HISTOGRAMS TO FILE
        // =======================================================================================
        
        // Event selection
        void WriteMCHists_Selection(TFile& fout) const;
        void WriteDataHists_Selection(TFile& fout) const;
        
        
        // Event selection with material breakdown
        void WriteMCHists_MatSelection(TFile& fout) const;
        void WriteDataHists_MatSelection(TFile& fout) const;
        
        
        // Event selection with interaction type breakdown
        void WriteMCHists_IntTypeSelection(TFile& fout) const;
        void WriteDataHists_IntTypeSelection(TFile& fout) const;
        
        
        // Reco objects with PDG breakdown
        void WriteMCHists_ObjectPdg(TFile& fout) const;
        void WriteDataHists_ObjectPdg(TFile& fout) const;
        
        
        // Efficiency components
        void WriteEffNumerator(TFile& fout) const;
        void WriteEffDenominator(TFile& fout) const;
        
        
        // Migration matrix
        void WriteMigrationHists(TFile& fout) const;
        
        
        // Plastic sidebands in signal region
        void WriteMCHists_PlasSB_In_SigReg(TFile& fout) const;
        void WriteDataHists_PlasSB_In_SigReg(TFile& fout) const;
        
        
        // Plastic sidebands in physics sidebands
        void WriteMCHists_PlasSB_In_PhysSB(TFile& fout) const;
        void WriteDataHists_PlasSB_In_PhysSB(TFile& fout) const;
        
        
        // Physics sidebands
        void WriteMCHists_PhysSB(TFile& fout) const;
        void WriteDataHists_PhysSB(TFile& fout) const;
        
        
        // MC tuning weights
        void WriteMCWeights_PlasBackgr_In_SigReg(TFile& fout) const;
        void WriteMCWeights_PlasBackgr_In_PhysSB(TFile& fout) const;
        void WriteMCWeights_PhysBackgr(TFile& fout) const;
        
        
        
        // =======================================================================================
        //  LOAD HISTOGRAMS FROM FILE
        // =======================================================================================
        
        // Helper functions
        CVHW LoadHWFromFile(TFile& fin, UniverseMap& error_bands,
                            std::string prefix, std::string suffix);
        
        CVH2DW LoadH2DWFromFile(TFile& fin, UniverseMap& error_bands,
                                std::string prefix, std::string suffix);
        
        
        // Event selection
        void LoadMCHists_Selection(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_Selection(TFile& fin);
        
        
        // Event selection with material breakdown
        void LoadMCHists_MatSelection(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_MatSelection(TFile& fin);
        
        
        // Event selection with interaction type breakdown
        void LoadMCHists_IntTypeSelection(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_IntTypeSelection(TFile& fin);
        
        
        // Reco objects with PDG breakdown
        void LoadMCHists_ObjectPdg(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_ObjectPdg(TFile& fin);
        
        
        // Efficiency components
        void LoadEffNumerator(TFile& fin, UniverseMap& error_bands);
        void LoadEffDenominator(TFile& fin, UniverseMap& error_bands);
        
        
        // Migration matrix
        void LoadMigrationHists(TFile& fin, UniverseMap& error_bands);
        
        
        // Plastic sidebands in signal region
        void LoadMCHists_PlasSB_In_SigReg(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_PlasSB_In_SigReg(TFile& fin);
        
        
        // Plastic sidebands in physics sidebands
        void LoadMCHists_PlasSB_In_PhysSB(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_PlasSB_In_PhysSB(TFile& fin);
        
        
        // Physics sidebands
        void LoadMCHists_PhysSB(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_PhysSB(TFile& fin);
        
        
        // MC tuning weights
        void LoadMCWeights_PlasBackgr_In_SigReg(TFile& fin, UniverseMap& error_bands);
        void LoadMCWeights_PlasBackgr_In_PhysSB(TFile& fin, UniverseMap& error_bands);
        void LoadMCWeights_PhysBackgr(TFile& fin, UniverseMap& error_bands);
        
};


// Template member functions need to be available in the header.
#include "Histograms.cxx"


#endif  // Histograms_h