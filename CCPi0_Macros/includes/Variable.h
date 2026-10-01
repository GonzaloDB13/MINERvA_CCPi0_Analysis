#ifndef Variable_h
#define Variable_h

#include <functional>

#include "CVUniverse.h"
#include "Histograms.h"
#include "Constants.h"  // typedefs MH1D, CVHW

#include "TArrayD.h"
#include "TFile.h"



#ifndef __CINT__  // CINT doesn't know about std::function


class Variable
{
    private:
        
        typedef std::function<double(const CVUniverse&)> PointerToCVUniverseFunction;
        PointerToCVUniverseFunction m_pointer_to_GetValue;
        
        typedef std::function<double(const CVUniverse&, int)> PointerToCVUniverseFunctionWithIndex;
        PointerToCVUniverseFunctionWithIndex m_pointer_to_GetValue_with_index;
        
        
        
    public:
        
        // =======================================================================================
        //  CONSTRUCTORS
        // =======================================================================================
        
        // Default
        Variable();
        
        
        // Uniform bin size
        Variable(const std::string label,
                 const std::string xaxis,
                 const std::string units,
                 const int nbins, const double xmin, const double xmax,
                 PointerToCVUniverseFunction p,
                 const bool is_true = false);
        
        Variable(const std::string label,
                 const std::string xaxis,
                 const std::string units,
                 const int nbins, const double xmin, const double xmax,
                 PointerToCVUniverseFunctionWithIndex p,
                 const bool is_true = false);
        
        
        // Variable bin size
        Variable(const std::string label,
                 const std::string xaxis,
                 const std::string units,
                 const TArrayD& bins_array,
                 PointerToCVUniverseFunction p,
                 const bool is_true = false);
        
        Variable(const std::string label,
                 const std::string xaxis,
                 const std::string units,
                 const TArrayD& bins_array,
                 PointerToCVUniverseFunctionWithIndex p,
                 const bool is_true = false);
        
        
        
        // =======================================================================================
        //  DATA MEMBERS
        // =======================================================================================
        
        std::string m_label;
        
        std::string m_units;
        
        Histograms m_hists;
        
        bool m_is_true;
        
        
        
        // =======================================================================================
        //  BASIC FUNCTIONS
        // =======================================================================================
        
        // Access properties
        std::string Name() const  { return m_label; }
        std::string Units() const { return m_units; }
        
        int NBins() const   { return m_hists.NBins(); }
        double XMin() const { return m_hists.XMin();  }
        double XMax() const { return m_hists.XMax();  }
        
        
        // Variable's value
        virtual double GetValue(const CVUniverse& universe) const;
        virtual double GetValue(const CVUniverse& universe, const int index) const;
        
        
        
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


#endif  // __CINT__


// Template member functions need to be available in the header.
#include "Variable.cxx"


#endif  // Variable_h