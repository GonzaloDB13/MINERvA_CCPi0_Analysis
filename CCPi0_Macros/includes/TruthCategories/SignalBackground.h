#ifndef SignalBackground_h
#define SignalBackground_h

#include <string>

#include "../CVUniverse.h"



// Signal and background breakdown
enum SignalBackgrType {
    kSignal,             // 0
    kBackgrPi0HighW,     // 1
    kBackgrQElike,       // 2
    kBackgrPionProd,     // 3
    kBackgrPlasUp,       // 4
    kBackgrPlasBetw,     // 5
    kBackgrPlasDown,     // 6
    kBackgrOther,        // 7
    kNSignalBackgrTypes  // 8
};



// ==============================================================================
//  Signal-background with breakdown
// ==============================================================================

SignalBackgrType GetSignalBackgrType(const CVUniverse& universe,
                                     std::string option_material)
{
    if ( universe.IsSignal(option_material) ) {
        return kSignal;
    }
    else if ( universe.IsBackgr_Pi0HighW(option_material) ) {
        return kBackgrPi0HighW;
    }
    else if ( universe.IsBackgr_QElike(option_material) ) {
        return kBackgrQElike;
    }
    else if ( universe.IsBackgr_PionProd(option_material) ) {
        return kBackgrPionProd;
    }
    else if ( universe.IsBackgr_PlasUp() ) {
        return kBackgrPlasUp;
    }
    else if ( universe.IsBackgr_PlasBetw() ) {
        return kBackgrPlasBetw;
    }
    else if ( universe.IsBackgr_PlasDown() ) {
        return kBackgrPlasDown;
    }
    else {
        return kBackgrOther;
    }
}



std::string GetTruthClassification_LegendLabel(SignalBackgrType category)
{
    switch ( category ) {
        case kSignal :
            return "CC 1#pi^{0} signal";
        
        case kBackgrPi0HighW :
            return "High-W CC #pi^{0}";
        
        case kBackgrQElike :
            return "CCQE-like";
        
        case kBackgrPionProd :
            return "CC #pi^{#pm} prod.";
        
        case kBackgrPlasUp :
            return "Plastic up.";
        
        case kBackgrPlasBetw :
            return "Plastic betw.";
        
        case kBackgrPlasDown :
            return "Plastic down.";
        
        case kBackgrOther :
            return "Other";
        
        default : {
            std::cout << " GetTruthClassification_LegendLabel() ERROR: PICK CORRECT TRUTH CATEGORY!!! " << std::endl;
            exit(1);
        }
    }
}



std::string GetTruthClassification_Name(SignalBackgrType category)
{
    switch ( category ) {
        case kSignal :
            return "Signal";
        
        case kBackgrPi0HighW :
            return "BackgrPi0HighW";
        
        case kBackgrQElike :
            return "BackgrQElike";
        
        case kBackgrPionProd :
            return "BackgrPionProd";
        
        case kBackgrPlasUp :
            return "BackgrPlasUp";
        
        case kBackgrPlasBetw :
            return "BackgrPlasBetw";
        
        case kBackgrPlasDown :
            return "BackgrPlasDown";
        
        case kBackgrOther :
            return "BackgrOther";
        
        default : {
            std::cout << " GetTruthClassification_Name() ERROR: PICK CORRECT TRUTH CATEGORY!!! " << std::endl;
            exit(1);
        }
    }
}


#endif  // SignalBackground_h