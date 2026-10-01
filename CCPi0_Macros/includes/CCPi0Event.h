#ifndef CCPi0Event_h
#define CCPi0Event_h

#include "CVUniverse.h"
#include "TruthCategories/SignalBackground.h"
#include "TruthCategories/Material.h"
#include "TruthCategories/InteractionType.h"
#include "Constants.h"

#ifndef __CINT__
#include "Variable.h"
#include "Variable2D.h"
#endif  // __CINT__


// Forward declare Variable classes
class Variable;
class Variable2D;



// ==============================================================================
//  CONSTRUCTOR
// ==============================================================================

// CCPi0 event class
class CCPi0Event
{
    public:
        CCPi0Event(const bool is_mc,
                   const bool is_truth,
                   const CVUniverse* universe,
                   const EnumModels& type_model,
                   const std::string option_material,
                   const bool use_fiducial_cut);
        
        
        // Basic data members (fixed by constructor)
        const bool m_is_mc;
        const bool m_is_truth;
        const CVUniverse* m_universe;
        const EnumModels& m_model_type;
        const std::string m_option_material;
        const bool m_use_fiducial_cut;
        
        bool m_is_signal;
        double m_weight;
        
        SignalBackgrType m_signal_backgr_type;
        MaterialType     m_material_type;
        InteractionType  m_interaction_type;
        
        
        // Other data members (fixed by other functions)
        bool m_passes_cuts;
        
        std::vector<int> m_passes_cut;
        
        bool m_is_SigReg;
        bool m_is_PionBlobSB;
        bool m_is_HighWSB;
        bool m_is_ProtonBlobSB;
        
        bool m_is_RecoPb;
        bool m_is_RecoFe;
        bool m_is_PlasUpSB;
        bool m_is_PlasBetwSB;
        bool m_is_PlasDownSB;
};



// ==============================================================================
//  HELPER FUNCTIONS -- CHECK CUTS
// ==============================================================================

// Individual cut
bool PassesCut(const CCPi0Event&, EnumCuts cut);


// Group of cuts (arbitrary)
bool PassesCuts(const CCPi0Event&, std::vector<EnumCuts> cuts);



// ==============================================================================
//  HELPER FUNCTIONS -- CHECK SIGNAL REGION AND PHYSICS SIDEBANDS
// ==============================================================================

// Signal region
bool IsSigReg(const CCPi0Event&);


// Pion-like shower sideband
bool IsPionBlobSB(const CCPi0Event&);


// Proton-like shower sideband
bool IsProtonBlobSB(const CCPi0Event&);


// High-W sideband
bool IsHighWSB(const CCPi0Event&);



// ==============================================================================
//  HELPER FUNCTIONS -- CHECK RECO MATERIAL AND PLASTIC SIDEBANDS
// ==============================================================================

// Reconstructed Pb
bool IsRecoPb(const CCPi0Event&);


// Reconstructed Fe
bool IsRecoFe(const CCPi0Event&);


// Plastic sideband upstream
bool IsPlasUpSB(const CCPi0Event&);


// Plastic sideband between
bool IsPlasBetwSB(const CCPi0Event&);


// Plastic sideband downstream
bool IsPlasDownSB(const CCPi0Event&);



// ==============================================================================
//  HELPER FUNCTIONS -- VALIDATE VARIABLES
// ==============================================================================

// MC variable
bool ValidateMCVariable(const CCPi0Event&, Variable*);
bool ValidateMCVariable2D(const CCPi0Event&, Variable2D*);


// MC Truth variable
bool ValidateMCTruthVariable(const CCPi0Event&, Variable*);
bool ValidateMCTruthVariable2D(const CCPi0Event&, Variable2D*);


// Data variable
bool ValidateDataVariable(const CCPi0Event&, Variable*);
bool ValidateDataVariable2D(const CCPi0Event&, Variable2D*);



// ==============================================================================
//  FILL HISTOGRAMS
// ==============================================================================

namespace ccpi0_event
{
    // Event selection
    void FillMCHists_Selection(const CCPi0Event&, Variable*);
    void FillDataHists_Selection(const CCPi0Event&, Variable*);
    
    void FillMCHists_Selection2D(const CCPi0Event&, Variable2D*);
    void FillDataHists_Selection2D(const CCPi0Event&, Variable2D*);
    
    
    // Event selection with material breakdown
    void FillMCHists_MatSelection(const CCPi0Event&, Variable*);
    void FillDataHists_MatSelection(const CCPi0Event&, Variable*);
    
    void FillMCHists_MatSelection2D(const CCPi0Event&, Variable2D*);
    void FillDataHists_MatSelection2D(const CCPi0Event&, Variable2D*);
    
    
    // Event selection with interaction type breakdown
    void FillMCHists_IntTypeSelection(const CCPi0Event&, Variable*);
    void FillDataHists_IntTypeSelection(const CCPi0Event&, Variable*);
    
    
    // Efficiency components
    void FillEffNumerator(const CCPi0Event&, Variable*);
    void FillEffDenominator(const CCPi0Event&, Variable*);
    
    
    // Migration matrix
    void FillMigrationHists(const CCPi0Event&, Variable*, const std::vector<Variable*>&);
    
    
    // Plastic sidebands in signal region
    void FillMCHists_RecoPb_In_SigReg(const CCPi0Event&, Variable*);
    void FillMCHists_RecoFe_In_SigReg(const CCPi0Event&, Variable*);
    void FillMCHists_PlasUpSB_In_SigReg(const CCPi0Event&, Variable*);
    void FillMCHists_PlasBetwSB_In_SigReg(const CCPi0Event&, Variable*);
    void FillMCHists_PlasDownSB_In_SigReg(const CCPi0Event&, Variable*);
    
    void FillDataHists_RecoPb_In_SigReg(const CCPi0Event&, Variable*);
    void FillDataHists_RecoFe_In_SigReg(const CCPi0Event&, Variable*);
    void FillDataHists_PlasUpSB_In_SigReg(const CCPi0Event&, Variable*);
    void FillDataHists_PlasBetwSB_In_SigReg(const CCPi0Event&, Variable*);
    void FillDataHists_PlasDownSB_In_SigReg(const CCPi0Event&, Variable*);
    
    
    // Plastic sidebands in physics sidebands
    void FillMCHists_PlasUpSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasBetwSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasDownSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasUpSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasBetwSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasDownSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasUpSB_In_HighWSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasBetwSB_In_HighWSB(const CCPi0Event&, Variable*);
    void FillMCHists_PlasDownSB_In_HighWSB(const CCPi0Event&, Variable*);
    
    void FillDataHists_PlasUpSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasBetwSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasDownSB_In_PionBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasUpSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasBetwSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasDownSB_In_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasUpSB_In_HighWSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasBetwSB_In_HighWSB(const CCPi0Event&, Variable*);
    void FillDataHists_PlasDownSB_In_HighWSB(const CCPi0Event&, Variable*);
    
    
    // Physics sidebands
    void FillMCHists_SigReg(const CCPi0Event&, Variable*);
    void FillMCHists_PionBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillMCHists_HighWSB(const CCPi0Event&, Variable*);
    
    void FillDataHists_SigReg(const CCPi0Event&, Variable*);
    void FillDataHists_PionBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_ProtonBlobSB(const CCPi0Event&, Variable*);
    void FillDataHists_HighWSB(const CCPi0Event&, Variable*);
}


#endif  // CCPi0Event_h