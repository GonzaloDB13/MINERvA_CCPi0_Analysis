#ifndef CCPi0Event_cxx 
#define CCPi0Event_cxx

#include "CCPi0Event.h"
#include "Cuts.h"
#include "Constants.h"         // EnumCuts
#include "common_functions.h"  // GetVar(), HasVar()



// ==============================================================================
//  CONSTRUCTOR
// ==============================================================================

CCPi0Event::CCPi0Event(const bool is_mc,
                       const bool is_truth,
                       const CVUniverse* universe,
                       const EnumModels& type_model,
                       const std::string option_material,
                       const bool use_fiducial_cut)
    : m_is_mc(is_mc),
      m_is_truth(is_truth),
      m_universe(universe),
      m_model_type(type_model),
      m_option_material(option_material),
      m_use_fiducial_cut(use_fiducial_cut)
{
    m_is_signal = is_mc ? (GetSignalBackgrType(*universe, option_material) == kSignal) :
                          false;
    
    m_weight = is_mc ? universe->GetWeight(type_model) :
                       1.0;
    
    m_signal_backgr_type = is_mc ? GetSignalBackgrType(*universe, option_material) :
                                   kNSignalBackgrTypes;
    
    m_material_type = is_mc ? GetMaterialType(*universe) :
                              kNMaterialTypes;
    
    m_interaction_type = is_mc ? GetInteractionType(*universe) :
                                 kNInteractionTypes;
}





// ==============================================================================
//  HELPER FUNCTIONS -- CHECK CUTS
// ==============================================================================

// Individual cut
bool PassesCut(const CCPi0Event& evt, EnumCuts cut) {
    return PassesCut(*evt.m_universe, cut, evt.m_option_material, evt.m_use_fiducial_cut);
}


// Group of cuts (arbitrary)
bool PassesCuts(const CCPi0Event& evt, std::vector<EnumCuts> cuts) {
    return PassesCuts(*evt.m_universe, cuts, evt.m_option_material, evt.m_use_fiducial_cut);
}





// ==============================================================================
//  HELPER FUNCTIONS -- CHECK SIGNAL REGION AND PHYSICS SIDEBANDS
// ==============================================================================

// Signal region
bool IsSigReg(const CCPi0Event& evt) {
    return IsSigReg(*evt.m_universe, evt.m_option_material, evt.m_use_fiducial_cut);
}


// Pion-like shower sideband
bool IsPionBlobSB(const CCPi0Event& evt) {
    return IsPionBlobSB(*evt.m_universe, evt.m_option_material, evt.m_use_fiducial_cut);
}


// Proton-like shower sideband
bool IsProtonBlobSB(const CCPi0Event& evt) {
    return IsProtonBlobSB(*evt.m_universe, evt.m_option_material, evt.m_use_fiducial_cut);
}


// High-W sideband
bool IsHighWSB(const CCPi0Event& evt) {
    return IsHighWSB(*evt.m_universe, evt.m_option_material, evt.m_use_fiducial_cut);
}





// ==============================================================================
//  HELPER FUNCTIONS -- CHECK RECO MATERIAL AND PLASTIC SIDEBANDS
// ==============================================================================

// Reconstructed Pb
bool IsRecoPb(const CCPi0Event& evt) { return IsRecoPb(*evt.m_universe); }


// Reconstructed Fe
bool IsRecoFe(const CCPi0Event& evt) { return IsRecoFe(*evt.m_universe); }


// Plastic sideband upstream target 4
bool IsPlasUpSB(const CCPi0Event& evt) { return IsPlasUpSB(*evt.m_universe); }


// Plastic sideband between
bool IsPlasBetwSB(const CCPi0Event& evt) { return IsPlasBetwSB(*evt.m_universe); }


// Plastic sideband downstream target 5
bool IsPlasDownSB(const CCPi0Event& evt) { return IsPlasDownSB(*evt.m_universe); }





// ==============================================================================
//  HELPER FUNCTIONS -- VALIDATE VARIABLES
// ==============================================================================

// Monte Carlo
// ===========

bool ValidateMCVariable(const CCPi0Event& evt, Variable* var) {
    // Don't fill variable if event is not MC
    if ( !evt.m_is_mc ) return false;
    
    // Don't fill if variable is true and event is not MC (weird case)
    if ( var->m_is_true && !evt.m_is_mc ) return false;
    
    return true;
}


bool ValidateMCVariable2D(const CCPi0Event& evt, Variable2D* var2D) {
    // Don't fill variable if event is not MC
    if ( !evt.m_is_mc ) return false;
    
    // Don't fill if variable is true and event is not MC (weird case)
    if ( var2D->m_is_true && !evt.m_is_mc ) return false;
    
    return true;
}



// MC Truth
// ========

bool ValidateMCTruthVariable(const CCPi0Event& evt, Variable* var) {
    // Don't fill variable if event is not MC
    if ( !evt.m_is_mc ) return false;
    
    // Don't fill if variable is true and event is not MC (weird case)
    if ( var->m_is_true && !evt.m_is_mc ) return false;
    
    // Don't fill if variable is not true
    if ( !(var->m_is_true) ) return false;
    
    return true;
}


bool ValidateMCTruthVariable2D(const CCPi0Event& evt, Variable2D* var2D) {
    // Don't fill variable if event is not MC
    if ( !evt.m_is_mc ) return false;
    
    // Don't fill if variable is true and event is not MC (weird case)
    if ( var2D->m_is_true && !evt.m_is_mc ) return false;
    
    // Don't fill if variable is not true
    if ( !(var2D->m_is_true) ) return false;
    
    return true;
}



// Data
// ====

bool ValidateDataVariable(const CCPi0Event& evt, Variable* var) {
    // Don't fill if variable is true or event is MC
    if ( var->m_is_true || evt.m_is_mc ) return false;
    
    return true;
}


bool ValidateDataVariable2D(const CCPi0Event& evt, Variable2D* var2D) {
    // Don't fill if variable is true or event is MC
    if ( var2D->m_is_true || evt.m_is_mc ) return false;
    
    return true;
}





// ==============================================================================
//  FILL EVENT SELECTION HISTOGRAMS
// ==============================================================================

// Monte Carlo
// ===========

void ccpi0_event::FillMCHists_Selection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill MC histogram
    var -> m_hists.m_mc_Selection.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    
    // Fill signal and background
    if ( evt.m_signal_backgr_type == kSignal )
        var -> m_hists.m_mc_Selection_Signal.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    
    else {
        var -> m_hists.m_mc_Selection_Backgr.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
        switch ( evt.m_signal_backgr_type ) {
            case kBackgrPi0HighW :
                var -> m_hists.m_mc_Selection_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrQElike :
                var -> m_hists.m_mc_Selection_BackgrQElike.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrPionProd :
                var -> m_hists.m_mc_Selection_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrPlasUp :
                var -> m_hists.m_mc_Selection_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrPlasBetw :
                var -> m_hists.m_mc_Selection_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrPlasDown :
                var -> m_hists.m_mc_Selection_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kBackgrOther :
                var -> m_hists.m_mc_Selection_BackgrOther.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            default :
                std::cerr << " FILL SELECTION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
                std::exit(2);
        }
    }
}



// Data
// ====

void ccpi0_event::FillDataHists_Selection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_Selection->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL EVENT SELECTION HISTOGRAMS 2D
// ==============================================================================

// Monte Carlo
// ===========

void ccpi0_event::FillMCHists_Selection2D(const CCPi0Event& evt, Variable2D* var2D)
{
    // Sanity check
    if ( !ValidateMCVariable2D(evt, var2D) ) return;
    
    // Get value
    double fill_val_x = var2D -> GetValueX(*evt.m_universe);
    double fill_val_y = var2D -> GetValueY(*evt.m_universe);
    
    // Fill MC histogram
    var2D -> m_hists2D.m_mc_Selection2D.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
    
    // Fill signal and background
    if ( evt.m_signal_backgr_type == kSignal )
        var2D -> m_hists2D.m_mc_Selection2D_Signal.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
    
    else {
        var2D -> m_hists2D.m_mc_Selection2D_Backgr.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
        switch ( evt.m_signal_backgr_type ) {
            case kBackgrPi0HighW :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrQElike :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrQElike.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrPionProd :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrPlasUp :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrPlasBetw :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrPlasDown :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            case kBackgrOther :
                var2D -> m_hists2D.m_mc_Selection2D_BackgrOther.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
                break;
            default :
                std::cerr << " FILL SELECTION 2D BACKGROUND CATEGORIES ERROR!!! " << std::endl;
                std::exit(2);
        }
    }
}



// Data
// ====

void ccpi0_event::FillDataHists_Selection2D(const CCPi0Event& evt, Variable2D* var2D)
{
    // Sanity check
    if ( !ValidateDataVariable2D(evt, var2D) ) return;
    
    // Get value
    double fill_val_x = var2D -> GetValueX(*evt.m_universe);
    double fill_val_y = var2D -> GetValueY(*evt.m_universe);
    
    // Fill data histogram
    var2D -> m_hists2D.m_data_Selection2D->Fill(fill_val_x, fill_val_y, evt.m_weight);
}





// ==============================================================================
//  FILL EVENT SELECTION HISTOGRAMS WITH MATERIAL BREAKDOWN
// ==============================================================================

// Monte Carlo
// ===========

void ccpi0_event::FillMCHists_MatSelection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_MatSelection.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_MatSelection_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_MatSelection_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_MatSelection_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_MatSelection_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_MatSelection_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_MatSelection_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_MatSelection_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL SELECTION W/MATERIAL BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

void ccpi0_event::FillDataHists_MatSelection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_MatSelection->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL EVENT SELECTION HISTOGRAMS 2D WITH MATERIAL BREAKDOWN
// ==============================================================================

// Monte Carlo
// ===========

void ccpi0_event::FillMCHists_MatSelection2D(const CCPi0Event& evt, Variable2D* var2D)
{
    // Sanity check
    if ( !ValidateMCVariable2D(evt, var2D) ) return;
    
    // Get value
    double fill_val_x = var2D -> GetValueX(*evt.m_universe);
    double fill_val_y = var2D -> GetValueY(*evt.m_universe);
    
    // Fill histograms
    var2D -> m_hists2D.m_mc_MatSelection2D.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
    
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var2D -> m_hists2D.m_mc_MatSelection2D_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var2D -> m_hists2D.m_mc_MatSelection2D_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var2D -> m_hists2D.m_mc_MatSelection2D_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTruePlasUp :
            var2D -> m_hists2D.m_mc_MatSelection2D_TruePlasUp.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTruePlasBetw :
            var2D -> m_hists2D.m_mc_MatSelection2D_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTruePlasDown :
            var2D -> m_hists2D.m_mc_MatSelection2D_TruePlasDown.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        case kTrueOtherMat :
            var2D -> m_hists2D.m_mc_MatSelection2D_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val_x, fill_val_y, evt.m_weight);
            break;
        default :
            std::cerr << " FILL SELECTION 2D W/MATERIAL BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

void ccpi0_event::FillDataHists_MatSelection2D(const CCPi0Event& evt, Variable2D* var2D)
{
    // Sanity check
    if ( !ValidateDataVariable2D(evt, var2D) ) return;
    
    // Get value
    double fill_val_x = var2D -> GetValueX(*evt.m_universe);
    double fill_val_y = var2D -> GetValueY(*evt.m_universe);
    
    // Fill data histogram
    var2D -> m_hists2D.m_data_MatSelection2D->Fill(fill_val_x, fill_val_y, evt.m_weight);
}





// ==============================================================================
//  FILL EVENT SELECTION HISTOGRAMS WITH INTERACTION TYPE BREAKDOWN
// ==============================================================================

// Monte Carlo
// ===========

void ccpi0_event::FillMCHists_IntTypeSelection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_IntTypeSelection.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    
    switch ( evt.m_interaction_type ) {
        case kCCQE :
            var -> m_hists.m_mc_IntTypeSelection_QE.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kCCMEC :
            var -> m_hists.m_mc_IntTypeSelection_MEC.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kCCDeltaRES :
            var -> m_hists.m_mc_IntTypeSelection_DeltaRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kCCOtherRES :
            var -> m_hists.m_mc_IntTypeSelection_OtherRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kCCSoftDIS :
            var -> m_hists.m_mc_IntTypeSelection_SoftDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kCCTrueDIS :
            var -> m_hists.m_mc_IntTypeSelection_TrueDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kOtherIntType :
            var -> m_hists.m_mc_IntTypeSelection_Other.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL SELECTION W/INTERACTION TYPE CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

void ccpi0_event::FillDataHists_IntTypeSelection(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_IntTypeSelection->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL EFFICIENCY COMPONENTS
// ==============================================================================

// Efficiency numerator
// ====================

void ccpi0_event::FillEffNumerator(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCTruthVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill efficiency numerator
    if ( evt.m_signal_backgr_type == kSignal ) {
        var -> m_hists.m_mc_EffNumerator.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
        
        switch ( evt.m_interaction_type ) {
            case kCCQE :
                var -> m_hists.m_mc_EffNumerator_QE.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCMEC :
                var -> m_hists.m_mc_EffNumerator_MEC.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCDeltaRES :
                var -> m_hists.m_mc_EffNumerator_DeltaRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCOtherRES :
                var -> m_hists.m_mc_EffNumerator_OtherRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCSoftDIS :
                var -> m_hists.m_mc_EffNumerator_SoftDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCTrueDIS :
                var -> m_hists.m_mc_EffNumerator_TrueDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kOtherIntType :
                var -> m_hists.m_mc_EffNumerator_Other.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            default :
                std::cerr << " FILL EFFICIENCY W/INTERACTION TYPE CATEGORIES ERROR!!! " << std::endl;
                std::exit(2);
        }
    }
}



// Efficiency denominator
// ======================

void ccpi0_event::FillEffDenominator(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCTruthVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill efficiency denominator
    if ( evt.m_signal_backgr_type == kSignal ) {
        var -> m_hists.m_mc_EffDenominator.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
        
        switch ( evt.m_interaction_type ) {
            case kCCQE :
                var -> m_hists.m_mc_EffDenominator_QE.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCMEC :
                var -> m_hists.m_mc_EffDenominator_MEC.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCDeltaRES :
                var -> m_hists.m_mc_EffDenominator_DeltaRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCOtherRES :
                var -> m_hists.m_mc_EffDenominator_OtherRES.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCSoftDIS :
                var -> m_hists.m_mc_EffDenominator_SoftDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kCCTrueDIS :
                var -> m_hists.m_mc_EffDenominator_TrueDIS.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            case kOtherIntType :
                var -> m_hists.m_mc_EffDenominator_Other.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
                break;
            default :
                std::cerr << " FILL EFFICIENCY W/INTERACTION TYPE CATEGORIES ERROR!!! " << std::endl;
                std::exit(2);
        }
    }
}





// ==============================================================================
//  FILL MIGRATION MATRIX
// ==============================================================================

void ccpi0_event::FillMigrationHists(const CCPi0Event& evt, Variable* var,
                                     const std::vector<Variable*>& variables)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // -----------------------------------------------------------------------------------------------------
    // NOTE:
    // We have to be sure to use fill migration matrix ONLY WHEN 'var' IS A RECO VARIABLE,
    // since the information of its corresponding true variable is automatically extracted by this function.
    // That being said, the vector 'variables' MUST INCLUDE the true analog of 'var'. It's super important
    // not to forget this and make sure that within the looping function there are both,
    // the reco 'var' and its corresponding true quantity.
    // -----------------------------------------------------------------------------------------------------
    
    // Get reco variable
    Variable* reco_var = var;
    std::string name = reco_var->m_label;
    
    // Get true variable
    Variable* true_var = GetVar(variables, name + std::string("_True"));
    if ( true_var == 0 ) return;
    
    // Sanity check
    if ( !ValidateMCVariable(evt, reco_var) )      return;
    if ( !ValidateMCTruthVariable(evt, true_var) ) return;
    
    // Get true and reco  values
    double reco_fill_val = reco_var -> GetValue(*evt.m_universe);
    double true_fill_val = true_var -> GetValue(*evt.m_universe);
    
    // Fill migration matrix (only for signal)
    if ( evt.m_signal_backgr_type == kSignal )
        var -> m_hists.m_mc_Migration.FillUniverse(*evt.m_universe, reco_fill_val, true_fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL PLASTIC SIDEBANDS IN SIGNAL REGION
// ==============================================================================

// Monte Carlo
// ===========

// Reconstructed Pb
// ----------------
void ccpi0_event::FillMCHists_RecoPb_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_RecoPb_In_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_RecoPb_In_SigReg_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Reconstructed Fe
// ----------------
void ccpi0_event::FillMCHists_RecoFe_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_RecoFe_In_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_RecoFe_In_SigReg_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillMCHists_PlasUpSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasUpSB_In_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasUpSB_In_SigReg_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband between
// ------------------------
void ccpi0_event::FillMCHists_PlasBetwSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasBetwSB_In_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasBetwSB_In_SigReg_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillMCHists_PlasDownSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasDownSB_In_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasDownSB_In_SigReg_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

// Reconstructed Pb
// ----------------
void ccpi0_event::FillDataHists_RecoPb_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_RecoPb_In_SigReg->Fill(fill_val, evt.m_weight);
}



// Reconstructed Fe
// ----------------
void ccpi0_event::FillDataHists_RecoFe_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_RecoFe_In_SigReg->Fill(fill_val, evt.m_weight);
}



// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillDataHists_PlasUpSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasUpSB_In_SigReg->Fill(fill_val, evt.m_weight);
}


// Plastic sideband between
// ------------------------
void ccpi0_event::FillDataHists_PlasBetwSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasBetwSB_In_SigReg->Fill(fill_val, evt.m_weight);
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillDataHists_PlasDownSB_In_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasDownSB_In_SigReg->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL PLASTIC SIDEBANDS IN PION-LIKE SHOWER SIDEBAND
// ==============================================================================

// Monte Carlo
// ===========

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillMCHists_PlasUpSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasUpSB_In_PionBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN PION SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband between
// ------------------------
void ccpi0_event::FillMCHists_PlasBetwSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasBetwSB_In_PionBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN PION SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillMCHists_PlasDownSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasDownSB_In_PionBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN PION SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillDataHists_PlasUpSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasUpSB_In_PionBlobSB->Fill(fill_val, evt.m_weight);
}


// Plastic sideband between
// ------------------------
void ccpi0_event::FillDataHists_PlasBetwSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasBetwSB_In_PionBlobSB->Fill(fill_val, evt.m_weight);
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillDataHists_PlasDownSB_In_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasDownSB_In_PionBlobSB->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL PLASTIC SIDEBANDS IN PROTON-LIKE BLOB SIDEBAND
// ==============================================================================

// Monte Carlo
// ===========

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillMCHists_PlasUpSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasUpSB_In_ProtonBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD BLOBS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband between
// ------------------------
void ccpi0_event::FillMCHists_PlasBetwSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasBetwSB_In_ProtonBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD BLOBS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillMCHists_PlasDownSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasDownSB_In_ProtonBlobSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD BLOBS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillDataHists_PlasUpSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasUpSB_In_ProtonBlobSB->Fill(fill_val, evt.m_weight);
}


// Plastic sideband between
// ------------------------
void ccpi0_event::FillDataHists_PlasBetwSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasBetwSB_In_ProtonBlobSB->Fill(fill_val, evt.m_weight);
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillDataHists_PlasDownSB_In_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasDownSB_In_ProtonBlobSB->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL PLASTIC SIDEBANDS IN HIGH-W SIDEBAND
// ==============================================================================

// Monte Carlo
// ===========

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillMCHists_PlasUpSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasUpSB_In_HighWSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasUpSB_In_HighWSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD MASS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband between
// ------------------------
void ccpi0_event::FillMCHists_PlasBetwSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasBetwSB_In_HighWSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasBetwSB_In_HighWSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD MASS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillMCHists_PlasDownSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PlasDownSB_In_HighWSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_material_type ) {
        case kTrueTgt4Pb :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt4Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Pb :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Pb.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueTgt5Fe :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TrueTgt5Fe.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasUp :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasBetw :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTruePlasDown :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TruePlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kTrueOtherMat :
            var -> m_hists.m_mc_PlasDownSB_In_HighWSB_TrueOtherMat.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PLASTIC SB IN HAD MASS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

// Plastic sideband upstream
// -------------------------
void ccpi0_event::FillDataHists_PlasUpSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasUpSB_In_HighWSB->Fill(fill_val, evt.m_weight);
}


// Plastic sideband between
// ------------------------
void ccpi0_event::FillDataHists_PlasBetwSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasBetwSB_In_HighWSB->Fill(fill_val, evt.m_weight);
}



// Plastic sideband downstream
// ---------------------------
void ccpi0_event::FillDataHists_PlasDownSB_In_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PlasDownSB_In_HighWSB->Fill(fill_val, evt.m_weight);
}





// ==============================================================================
//  FILL PHYSICS SIDEBANDS
// ==============================================================================

// Monte Carlo
// ===========

// Signal region
// -------------
void ccpi0_event::FillMCHists_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_SigReg.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_signal_backgr_type ) {
        case kSignal :
            var -> m_hists.m_mc_SigReg_Signal.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPi0HighW :
            var -> m_hists.m_mc_SigReg_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrQElike :
            var -> m_hists.m_mc_SigReg_BackgrQElike.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPionProd :
            var -> m_hists.m_mc_SigReg_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasUp :
            var -> m_hists.m_mc_SigReg_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasBetw :
            var -> m_hists.m_mc_SigReg_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasDown :
            var -> m_hists.m_mc_SigReg_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrOther :
            var -> m_hists.m_mc_SigReg_BackgrOther.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Pion-like shower sideband
// -------------------------
void ccpi0_event::FillMCHists_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_PionBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_signal_backgr_type ) {
        case kSignal :
            var -> m_hists.m_mc_PionBlobSB_Signal.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPi0HighW :
            var -> m_hists.m_mc_PionBlobSB_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrQElike :
            var -> m_hists.m_mc_PionBlobSB_BackgrQElike.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPionProd :
            var -> m_hists.m_mc_PionBlobSB_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasUp :
            var -> m_hists.m_mc_PionBlobSB_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasBetw :
            var -> m_hists.m_mc_PionBlobSB_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasDown :
            var -> m_hists.m_mc_PionBlobSB_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrOther :
            var -> m_hists.m_mc_PionBlobSB_BackgrOther.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL PION SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// High-W sideband
// ---------------
void ccpi0_event::FillMCHists_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_HighWSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_signal_backgr_type ) {
        case kSignal :
            var -> m_hists.m_mc_HighWSB_Signal.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPi0HighW :
            var -> m_hists.m_mc_HighWSB_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrQElike :
            var -> m_hists.m_mc_HighWSB_BackgrQElike.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPionProd :
            var -> m_hists.m_mc_HighWSB_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasUp :
            var -> m_hists.m_mc_HighWSB_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasBetw :
            var -> m_hists.m_mc_HighWSB_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasDown :
            var -> m_hists.m_mc_HighWSB_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrOther :
            var -> m_hists.m_mc_HighWSB_BackgrOther.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL HAD MASS SB BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Proton-like shower sideband
// ---------------------------
void ccpi0_event::FillMCHists_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateMCVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill histograms
    var -> m_hists.m_mc_ProtonBlobSB.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
    switch ( evt.m_signal_backgr_type ) {
        case kSignal :
            var -> m_hists.m_mc_ProtonBlobSB_Signal.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPi0HighW :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrPi0HighW.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrQElike :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrQElike.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPionProd :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrPionProd.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasUp :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrPlasUp.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasBetw :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrPlasBetw.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrPlasDown :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrPlasDown.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        case kBackgrOther :
            var -> m_hists.m_mc_ProtonBlobSB_BackgrOther.FillUniverse(*evt.m_universe, fill_val, evt.m_weight);
            break;
        default :
            std::cerr << " FILL SIGNAL REGION BACKGROUND CATEGORIES ERROR!!! " << std::endl;
            std::exit(2);
    }
}



// Data
// ====

// Signal region
// -------------
void ccpi0_event::FillDataHists_SigReg(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_SigReg->Fill(fill_val, evt.m_weight);
}



// Pion-like shower sideband
// -------------------------
void ccpi0_event::FillDataHists_PionBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_PionBlobSB->Fill(fill_val, evt.m_weight);
}



// High-W sideband
// ---------------
void ccpi0_event::FillDataHists_HighWSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_HighWSB->Fill(fill_val, evt.m_weight);
}



// Proton-like shower sideband
// ---------------------------
void ccpi0_event::FillDataHists_ProtonBlobSB(const CCPi0Event& evt, Variable* var)
{
    // Sanity check
    if ( !ValidateDataVariable(evt, var) ) return;
    
    // Get value
    double fill_val = var -> GetValue(*evt.m_universe);
    
    // Fill data histogram
    var -> m_hists.m_data_ProtonBlobSB->Fill(fill_val, evt.m_weight);
}


#endif  // CCPi0Event_cxx