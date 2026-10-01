#ifndef InteractionType_h
#define InteractionType_h

#include <string>

#include "../CVUniverse.h"



// Interaction type breakdown
enum InteractionType {
    kCCQE,              // 0
    kCCMEC,             // 1
    kCCDeltaRES,        // 3
    kCCOtherRES,        // 4
    kCCSoftDIS,         // 5
    kCCTrueDIS,         // 6
    kOtherIntType,      // 7
    kNInteractionTypes  // 8
};



// ==============================================================================
//  Signal-background with breakdown
// ==============================================================================

InteractionType GetInteractionType(const CVUniverse& universe)
{
    if ( universe.IsTrueGenieCC_QE() ) {
        return kCCQE;
    }
    else if ( universe.IsTrueGenieCC_MEC() ) {
        return kCCMEC;
    }
    else if ( universe.IsTrueGenieCC_DeltaRES() ) {
        return kCCDeltaRES;
    }
    else if ( universe.IsTrueGenieCC_OtherRES() ) {
        return kCCOtherRES;
    }
    else if ( universe.IsTrueGenieCC_SoftDIS() ) {
        return kCCSoftDIS;
    }
    else if ( universe.IsTrueGenieCC_TrueDIS() ) {
        return kCCTrueDIS;
    }
    else {
        return kOtherIntType;
    }
}



std::string GetTruthClassification_LegendLabel(InteractionType category)
{
    switch ( category ) {
        case kCCQE :
            return "QE";
        
        case kCCMEC :
            return "MEC";
        
        case kCCDeltaRES :
            return "Delta RES";
        
        case kCCOtherRES :
            return "Other RES";
        
        case kCCSoftDIS :
            return "'Soft' DIS";
        
        case kCCTrueDIS :
            return "'True' DIS";
        
        case kOtherIntType :
            return "Other";
        
        default : {
            std::cout << " GetTruthClassification_LegendLabel() ERROR: PICK CORRECT INTERACTION TYPE!!! " << std::endl;
            exit(1);
        }
    }
}



std::string GetTruthClassification_Name(InteractionType category)
{
    switch ( category ) {
        case kCCQE :
            return "CC Quasi-elastic";
        
        case kCCMEC :
            return "CC MEC";
        
        case kCCDeltaRES :
            return "CC Delta resonance";
        
        case kCCOtherRES :
            return "CC other resonance";
        
        case kCCSoftDIS :
            return "CC soft DIS";
        
        case kCCTrueDIS :
            return "CC true DIS";
        
        case kOtherIntType :
            return "Other";
        
        default : {
            std::cout << " GetTruthClassification_Name() ERROR: PICK CORRECT INTERACTION TYPE!!! " << std::endl;
            exit(1);
        }
    }
}


#endif  // InteractionType_h