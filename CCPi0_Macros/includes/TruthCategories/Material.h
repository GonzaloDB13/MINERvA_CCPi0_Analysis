#ifndef Material_h
#define Material_h

#include <string>

#include "../CVUniverse.h"



// True interaction material
enum MaterialType {
    kTrueTgt4Pb,     // 0
    kTrueTgt5Pb,     // 1
    kTrueTgt5Fe,     // 2
    kTruePlasUp,     // 3
    kTruePlasBetw,   // 4
    kTruePlasDown,   // 5
    kTrueOtherMat,   // 6
    kNMaterialTypes  // 7
};



// ==============================================================================
//  Material categorization
// ==============================================================================

MaterialType GetMaterialType(const CVUniverse& universe)
{
    if ( universe.IsTrueTgt4Pb() ) {
        return kTrueTgt4Pb;
    }
    else if ( universe.IsTrueTgt5Pb() ) {
        return kTrueTgt5Pb;
    }
    else if ( universe.IsTrueTgt5Fe() ) {
        return kTrueTgt5Fe;
    }
    else if ( universe.IsTruePlasUp() ) {
        return kTruePlasUp;
    }
    else if ( universe.IsTruePlasBetw() ) {
        return kTruePlasBetw;
    }
    else if ( universe.IsTruePlasDown() ) {
        return kTruePlasDown;
    }
    else {
        return kTrueOtherMat;
    }
}



std::string GetTruthClassification_LegendLabel(MaterialType category)
{
    switch ( category ) {
        case kTrueTgt4Pb :
            return "Pb of target 4";
        
        case kTrueTgt5Pb :
            return "Pb of target 5";
        
        case kTrueTgt5Fe :
            return "Fe of target 5";
        
        case kTruePlasUp :
            return "Plastic up.";
        
        case kTruePlasBetw :
            return "Plastic betw.";
        
        case kTruePlasDown :
            return "Plastic down.";
        
        case kTrueOtherMat :
            return "Other material";
        
        default : {
            std::cout << " GetTruthClassification_LegendLabel() ERROR: PICK CORRECT MATERIAL!!! " << std::endl;
            exit(1);
        }
    }
}



std::string GetTruthClassification_Name(MaterialType category)
{
    switch ( category ) {
        case kTrueTgt4Pb :
            return "TrueTgt4Pb";
        
        case kTrueTgt5Pb :
            return "TrueTgt5Pb";
        
        case kTrueTgt5Fe :
            return "TrueTgt5Fe";
        
        case kTruePlasUp :
            return "TruePlasUp";
        
        case kTruePlasBetw :
            return "TruePlasBetw";
        
        case kTruePlasDown :
            return "TruePlasDown";
        
        case kTrueOtherMat :
            return "TrueOtherMat";
        
        default : {
            std::cout << " GetTruthClassification_LegendLabel() ERROR: PICK CORRECT MATERIAL!!! " << std::endl;
            exit(1);
        }
    }
}


#endif  // Material_h