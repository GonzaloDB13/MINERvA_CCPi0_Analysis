#ifndef Variable2D_h
#define Variable2D_h

#include <functional>

#include "CVUniverse.h"
#include "Histograms2D.h"
#include "Constants.h"  // typedefs MH2D, CVH2DW

#include "TArrayD.h"
#include "TFile.h"



#ifndef __CINT__  // CINT doesn't know about std::function


class Variable2D
{
    private:
        
        typedef std::function<double(const CVUniverse&)> PointerToCVUniverseFunction;
        PointerToCVUniverseFunction m_pointer_to_GetValueX;
        PointerToCVUniverseFunction m_pointer_to_GetValueY;
        
        
        typedef std::function<double(const CVUniverse&, int)> PointerToCVUniverseFunctionWithIndex;
        PointerToCVUniverseFunctionWithIndex m_pointer_to_GetValueX_with_index;
        PointerToCVUniverseFunctionWithIndex m_pointer_to_GetValueY_with_index;
        
        
        
    public:
        
        // =======================================================================================
        //  CONSTRUCTORS
        // =======================================================================================
        
        // Default
        Variable2D();
        
        
        // Uniform bin size
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const int nbinsx, const double xmin, const double xmax,
                   PointerToCVUniverseFunction px,
                   const std::string ylabel,
                   const std::string yunits,
                   const int nbinsy, const double ymin, const double ymax,
                   PointerToCVUniverseFunction py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const int nbinsx, const double xmin, const double xmax,
                   PointerToCVUniverseFunctionWithIndex px,
                   const std::string ylabel,
                   const std::string yunits,
                   const int nbinsy, const double ymin, const double ymax,
                   PointerToCVUniverseFunction py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const int nbinsx, const double xmin, const double xmax,
                   PointerToCVUniverseFunction px,
                   const std::string ylabel,
                   const std::string yunits,
                   const int nbinsy, const double ymin, const double ymax,
                   PointerToCVUniverseFunctionWithIndex py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const int nbinsx, const double xmin, const double xmax,
                   PointerToCVUniverseFunctionWithIndex px,
                   const std::string ylabel,
                   const std::string yunits,
                   const int nbinsy, const double ymin, const double ymax,
                   PointerToCVUniverseFunctionWithIndex py,
                   const bool is_true = false);
        
        
        // Variable bin size
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const TArrayD& xbins_array,
                   PointerToCVUniverseFunction px,
                   const std::string ylabel,
                   const std::string yunits,
                   const TArrayD& ybins_array,
                   PointerToCVUniverseFunction py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const TArrayD& xbins_array,
                   PointerToCVUniverseFunctionWithIndex px,
                   const std::string ylabel,
                   const std::string yunits,
                   const TArrayD& ybins_array,
                   PointerToCVUniverseFunction py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const TArrayD& xbins_array,
                   PointerToCVUniverseFunction px,
                   const std::string ylabel,
                   const std::string yunits,
                   const TArrayD& ybins_array,
                   PointerToCVUniverseFunctionWithIndex py,
                   const bool is_true = false);
        
        Variable2D(const std::string label,
                   const std::string xlabel,
                   const std::string xunits,
                   const TArrayD& xbins_array,
                   PointerToCVUniverseFunctionWithIndex px,
                   const std::string ylabel,
                   const std::string yunits,
                   const TArrayD& ybins_array,
                   PointerToCVUniverseFunctionWithIndex py,
                   const bool is_true = false);
        
        
        
        // =======================================================================================
        //  DATA MEMBERS
        // =======================================================================================
        
        std::string m_label;
        
        std::string m_xunits;
        std::string m_yunits;
        
        Histograms2D m_hists2D;
        
        bool m_is_true;
        
        
        
        // =======================================================================================
        //  BASIC FUNCTIONS
        // =======================================================================================
        
        // Access properties
        std::string Name() const   { return m_label; }
        std::string Xunits() const { return m_xunits; }
        std::string Yunits() const { return m_yunits; }
        
        int NBinsX() const  { return m_hists2D.NBinsX(); }
        double XMin() const { return m_hists2D.XMin();   }
        double XMax() const { return m_hists2D.XMax();   }
        
        int NBinsY() const  { return m_hists2D.NBinsY(); }
        double YMin() const { return m_hists2D.YMin();   }
        double YMax() const { return m_hists2D.YMax();   }
        
        
        // Variable's value
        virtual double GetValueX(const CVUniverse& universe) const;
        virtual double GetValueY(const CVUniverse& universe) const;
        
        virtual double GetValueX(const CVUniverse& universe, const int index) const;
        virtual double GetValueY(const CVUniverse& universe, const int index) const;
        
        
        
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


#endif  // __CINT__


// Template member functions need to be available in the header.
#include "Variable2D.cxx"


#endif  // Variable2D_h