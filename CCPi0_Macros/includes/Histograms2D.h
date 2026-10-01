#ifndef Histograms2D_h
#define Histograms2D_h

#include "CVUniverse.h"
#include "Constants.h"   // typedefs MH2D, CVH2DW
#include "Binning.h"     // MakeUniformBinArray()
#include "util.h"        // uniq()

#include "TArrayD.h"
#include "TFile.h"



class Histograms2D
{
    public:
        
        // ==========================================================================
        //  CONSTRUCTORS
        // ==========================================================================
        
        // Default
        Histograms2D();
        
        
        // Uniform bin size
        Histograms2D(const std::string label,
                     const std::string xlabel,
                     const int nbinsx, const double xmin, const double xmax,
                     const std::string ylabel,
                     const int nbinsy, const double ymin, const double ymax);
        
        
        // Variable bin size
        Histograms2D(const std::string label,
                     const std::string xlabel,
                     const TArrayD& binsx_array,
                     const std::string ylabel,
                     const TArrayD& binsy_array);
        
        
        
        // ==========================================================================
        //  DATA MEMBERS
        // ==========================================================================
        
        // Basic data members
        std::string m_label;
        
        std::string m_xlabel;
        std::string m_ylabel;
        
        TArrayD m_binsx_array;
        TArrayD m_binsy_array;
        
        std::vector<double> m_binsx_vector;
        std::vector<double> m_binsy_vector;
        
        
        // Histograms -- Event selection
        // =============================
        
        MH2D*   m_data_Selection2D;  // Data
        CVH2DW  m_mc_Selection2D;    // MC
        
        CVH2DW m_mc_Selection2D_Signal;  // Signal
        CVH2DW m_mc_Selection2D_Backgr;  // Background
        
        CVH2DW m_mc_Selection2D_BackgrPi0HighW;  // High-W pi0 background
        CVH2DW m_mc_Selection2D_BackgrQElike;    // QE-like background
        CVH2DW m_mc_Selection2D_BackgrPionProd;  // Pion production background
        CVH2DW m_mc_Selection2D_BackgrPlasUp;    // Up. plastic background
        CVH2DW m_mc_Selection2D_BackgrPlasBetw;  // Betw. plastic background
        CVH2DW m_mc_Selection2D_BackgrPlasDown;  // Down. plastic background
        CVH2DW m_mc_Selection2D_BackgrOther;     // Other background
        
        
        // Histograms -- Reconstructed objects w/PDG breakdown
        // ===================================================
        
        MH2D*   m_data_MatSelection2D;  // Data
        CVH2DW  m_mc_MatSelection2D;    // MC
        
        CVH2DW m_mc_MatSelection2D_TrueTgt4Pb;    // Pb of target 4
        CVH2DW m_mc_MatSelection2D_TrueTgt5Pb;    // Pb of target 5
        CVH2DW m_mc_MatSelection2D_TrueTgt5Fe;    // Fe of target 5
        CVH2DW m_mc_MatSelection2D_TruePlasUp;    // Up. plastic
        CVH2DW m_mc_MatSelection2D_TruePlasBetw;  // Betw. plastic
        CVH2DW m_mc_MatSelection2D_TruePlasDown;  // Down. plastic
        CVH2DW m_mc_MatSelection2D_TrueOtherMat;  // Other material
        
        
        // Histograms -- Reconstructed objects w/PDG breakdown
        // ===================================================
        
        MH2D*   m_data_ObjectPdg2D;  // Data
        CVH2DW  m_mc_ObjectPdg2D;    // MC
        
        CVH2DW m_mc_ObjectPdg2D_Pi0;      // True pi0
        CVH2DW m_mc_ObjectPdg2D_Proton;   // True proton
        CVH2DW m_mc_ObjectPdg2D_Neutron;  // True neutron
        CVH2DW m_mc_ObjectPdg2D_Pion;     // True charged pion
        CVH2DW m_mc_ObjectPdg2D_EM;       // True electron/photon
        CVH2DW m_mc_ObjectPdg2D_Muon;     // True muon
        CVH2DW m_mc_ObjectPdg2D_OthPdg;   // True other PDG
        CVH2DW m_mc_ObjectPdg2D_MCXtalk;  // MC X-talk
        CVH2DW m_mc_ObjectPdg2D_Overlay;  // Data overlay
        
        
        
        // ==========================================================================
        //  FUNCTIONS
        // ==========================================================================
        
        // Basic functions
        int NBinsX()  const { return m_binsx_array.GetSize()-1; }
        double XMin() const { return m_binsx_array[0]; }
        double XMax() const { return m_binsx_array[NBinsX()]; }
        
        int NBinsY()  const { return m_binsy_array.GetSize()-1; }
        double YMin() const { return m_binsy_array[0]; }
        double YMax() const { return m_binsy_array[NBinsY()]; }
        
        void PrintBinningX() const {
            for( int i = 0; i <= NBinsX(); ++i ) std::cout << m_binsx_array[i] << " ";
            std::cout << std::endl;
        }
        
        void PrintBinningY() const {
            for( int i = 0; i <= NBinsY(); ++i ) std::cout << m_binsy_array[i] << " ";
            std::cout << std::endl;
        }
        
        
        
        // =======================================================================================
        //  INITIALIZE HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        template<typename T>
        void InitMCHists_Selection2D(T systematic_univs);
        
        void InitDataHists_Selection2D();
        
        
        // Event selection with material breakdown
        template<typename T>
        void InitMCHists_MatSelection2D(T systematic_univs);
        
        void InitDataHists_MatSelection2D();
        
        
        // Reco objects with PDG breakdown
        template<typename T>
        void InitMCHists_ObjectPdg2D(T systematic_univs);
        
        void InitDataHists_ObjectPdg2D();
        
        
        
        // =======================================================================================
        //  SYNCHRONIZE MC CV HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void SyncMCHists_Selection2D();
        
        
        // Event selection with material breakdown
        void SyncMCHists_MatSelection2D();
        
        
        // Reco objects with PDG breakdown
        void SyncMCHists_ObjectPdg2D();
        
        
        
        // =======================================================================================
        //  BIN WIDTH NORMALIZE HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void BinWidthNormMCHists_Selection2D();
        void BinWidthNormDataHists_Selection2D();
        
        
        // Event selection with material breakdown
        void BinWidthNormMCHists_MatSelection2D();
        void BinWidthNormDataHists_MatSelection2D();
        
        
        // Reco objects with PDG breakdown
        void BinWidthNormMCHists_ObjectPdg2D();
        void BinWidthNormDataHists_ObjectPdg2D();
        
        
        
        // =======================================================================================
        //  SCALE MC HISTOGRAMS
        // =======================================================================================
        
        // Event selection
        void ScaleMCHists_Selection2D(const double mc_pot, const double data_pot);
        
        
        // Event selection with material breakdown
        void ScaleMCHists_MatSelection2D(const double mc_pot, const double data_pot);
        
        
        // Reco objects with PDG breakdown
        void ScaleMCHists_ObjectPdg2D(const double mc_pot, const double data_pot);
        
        
        
        // =======================================================================================
        //  WRITE HISTOGRAMS TO FILE
        // =======================================================================================
        
        // Event selection
        void WriteMCHists_Selection2D(TFile& fout) const;
        void WriteDataHists_Selection2D(TFile& fout) const;
        
        
        // Event selection with material breakdown
        void WriteMCHists_MatSelection2D(TFile& fout) const;
        void WriteDataHists_MatSelection2D(TFile& fout) const;
        
        
        // Reco objects with PDG breakdown
        void WriteMCHists_ObjectPdg2D(TFile& fout) const;
        void WriteDataHists_ObjectPdg2D(TFile& fout) const;
        
        
        
        // =======================================================================================
        //  LOAD HISTOGRAMS FROM FILE
        // =======================================================================================
        
        // Helper functions
        CVHW LoadHWFromFile(TFile& fin, UniverseMap& error_bands,
                            std::string prefix, std::string suffix);
        
        CVH2DW LoadH2DWFromFile(TFile& fin, UniverseMap& error_bands,
                                std::string prefix, std::string suffix);
        
        
        // Event selection
        void LoadMCHists_Selection2D(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_Selection2D(TFile& fin);
        
        
        // Event selection with material breakdown
        void LoadMCHists_MatSelection2D(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_MatSelection2D(TFile& fin);
        
        
        // Reco objects with PDG breakdown
        void LoadMCHists_ObjectPdg2D(TFile& fin, UniverseMap& error_bands);
        void LoadDataHists_ObjectPdg2D(TFile& fin);
        
};


// Template member functions need to be available in the header.
#include "Histograms2D.cxx"


#endif  // Histograms2D_h