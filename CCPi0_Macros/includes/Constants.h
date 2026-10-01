#ifndef Constants_h
#define Constants_h

#include <iostream>
#include <string>

#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"



// Forward-declare CVUniverse class
class CVUniverse;



// ==============================================================================
//  Typedefs and enums
// ==============================================================================

// Typedefs
typedef PlotUtils::MnvH1D MH1D;
typedef PlotUtils::MnvH2D MH2D;

typedef PlotUtils::HistWrapper<CVUniverse> CVHW;
typedef PlotUtils::Hist2DWrapper<CVUniverse> CVH2DW;

typedef std::map<std::string, std::vector<CVUniverse*> > UniverseMap;


// Data, MC, Truth enumerator
enum EnumDataMCTruth {
    kData,              // 0
    kMC,                // 1
    kTruth,             // 2
    kNDataMCTruthTypes  // 3
};


// Cuts enumerator
enum EnumCuts {
    kNoRecoCuts,                 // 0
    kRecoCut_InteractionVertex,  // 1
    kRecoCut_NeutrinoHelicity,   // 2
    kRecoCut_MinosMatch,         // 3
    kRecoCut_MuonCharge,         // 4
    kRecoCut_MuonTrackAngle,     // 5
    kRecoCut_DeadTime,           // 6
    kRecoCut_FiducialVolume,     // 7
    kRecoCut_MichelElectrons,    // 8
    kRecoCut_LongTracks,         // 9
    kRecoCut_EventHasBlobs,      // 10
    kRecoCut_BlobAngleWRTMuon,   // 11
    kRecoCut_BlobDeviation,      // 12
    kRecoCut_BlobEnergyVSdx,     // 13
    kRecoCut_BlobdEdxEnd,        // 14
    kRecoCut_BlobHasMichel,      // 15
    kRecoCut_NoPi0RecoilEnergy,  // 16
    kAllRecoCuts                 // 17
};





// ==============================================================================
//  MC model functions
// ==============================================================================

// MC models enumerator
enum EnumModels {
    kGENIE,                  // 0
    kMnvGENIEv1,             // 1
    kMnvGENIEv1_noNonResPi,  // 2
    kMnvGENIEv1_noD2,        // 3
    kMnvGENIEv1_noPionTune,  // 4
    kMnvGENIEv2_MINOS,       // 5
    kMnvGENIEv2_JOINT,       // 6
    kMnvGENIEv2_NU1PI,       // 7
    kMnvGENIEv2_NUNPI,       // 8
    kMnvGENIEv2_NUPI0,       // 9
    kMnvGENIEv2_MENU1PI,     // 10
    kDataNoModel             // 11
};


// Get model type from a string
void GetModel(std::string option_model, EnumModels& type_model)
{
    if ( option_model == "v0" )                type_model = kGENIE;
    else if ( option_model == "v1" )           type_model = kMnvGENIEv1;
    else if ( option_model == "v1noNonResPi" ) type_model = kMnvGENIEv1_noNonResPi;
    else if ( option_model == "v1noD2" )       type_model = kMnvGENIEv1_noD2;
    else if ( option_model == "v1noPionTune" ) type_model = kMnvGENIEv1_noPionTune;
    else if ( option_model == "v2MINOS" )      type_model = kMnvGENIEv2_MINOS;
    else if ( option_model == "v2JOINT" )      type_model = kMnvGENIEv2_JOINT;
    else if ( option_model == "v2NU1PI" )      type_model = kMnvGENIEv2_NU1PI;
    else if ( option_model == "v2NUNPI" )      type_model = kMnvGENIEv2_NUNPI;
    else if ( option_model == "v2NUPI0" )      type_model = kMnvGENIEv2_NUPI0;
    else if ( option_model == "v2MENU1PI" )    type_model = kMnvGENIEv2_MENU1PI;
    else if ( option_model == "data" )         type_model = kDataNoModel;
    else {
        std::cout << " GetModel() ERROR: MODEL STRING NOT SPECIFIED!!! " << std::endl;
        exit(1);
    }
}





// ==============================================================================
//  Playlists functions
// ==============================================================================

// Playlist enumerator
enum EnumPlaylists {
    minervame1A,  // 0
    minervame1B,  // 1
    minervame1C,  // 2
    minervame1D,  // 3
    minervame1E,  // 4
    minervame1F,  // 5
    minervame1G,  // 6
    minervame1L,  // 7
    minervame1M,  // 8
    minervame1N,  // 9
    minervame1O,  // 10
    minervame1P,  // 11
    AllPlaylists  // 12
};


// Get playlist .txt file
std::string GetPlaylistFile(bool is_mc,
                            std::string plist,
                            std::string sample = "test",
                            bool use_xrootd = true)
{
#ifndef __CINT__
    //const std::string date = "2021-01-01";
    const std::string date = "2022-02-01";
    const std::string is_mc_str = is_mc ? "mc" : "data";
    
    std::string top_dir = "/minerva/data/users/gonzalo/MAT/playlists_txt/" + date;
    
    std::string plist_file = use_xrootd ?
        Form("%s/%s/%s/%s_xrootd.txt", top_dir.c_str(), sample.c_str(), is_mc_str.c_str(), plist.c_str()) :
        Form("%s/%s/%s/%s.txt",        top_dir.c_str(), sample.c_str(), is_mc_str.c_str(), plist.c_str());
    
    return plist_file;
#endif  // __CINT__
}





// ==============================================================================
//  Analysis constants
// ==============================================================================

namespace CCPi0AnaConstants
{
    // Parameters for MinervaUniverse in MacroUtil
    const int  kAnalysisNuPDG = 14;       // Analysis PDG
    const int  kNFluxUniverses   = 500;   // Flux universes
    const bool kUseNueConstraint = true;  // (FHC+RHC) Nu-e + IMD flux constraints
    
    const std::string kMHRReadoutVolume       = "nuke";  // MnvHadronReweight options
    const bool kUseMHRWeightNeutronCVReweight = true;
    const bool kUseMHRWeightElastics          = true;
    
    
    // Fiducial hexagon apothem
    const double kApothem = 850.0;  // [mm]
    
    
    // Particle masses
    const double MUON_MASS    = 105.6583;    // [MeV/c^2]
    const double PROTON_MASS  = 938.272013;  // [MeV/c^2]
    const double NEUTRON_MASS = 939.56536;   // [MeV/c^2]
    const double PION_MASS    = 139.57039;   // [MeV/c^2]
    const double PI0_MASS     = 134.9768;    // [MeV/c^2]
    const double KAON_MASS    = 493.677;     // [MeV/c^2]
    const double NUCLEON_MASS = (PROTON_MASS + NEUTRON_MASS) / 2.0;
    
    
    // Numi beam angle [rad]
    const double NUMI_BEAM_ANGLE_RAD = -0.05887;  // [rad]
    
    
    // Numeric constants
    const double PI = 3.14159265358979323846;
    const double RADTODEG = 180.0/PI;
    const double DEGTORAD = PI/180.0;
}





// ==============================================================================
//  Stack histogram color schemes
// ==============================================================================

namespace CCPi0AnaStackColors
{
    // Signal and background color scheme
    const int Ncolors_phys = 8;
    
    const int line_colors_phys[Ncolors_phys] = {kAzure-4,   // Signal
                                                kRed,       // High-W pi0 prod
                                                kCyan+3,    // QE-like
                                                kRed+2,     // Pion production
                                                kYellow,    // Plastic upstream
                                                kOrange,    // Plastic between
                                                kOrange+2,  // Plastic downstream
                                                kGray};     // Other background
    
    const int fill_colors_phys[Ncolors_phys] = {kAzure-4,   // Signal
                                                kRed,       // High-W pi0 prod
                                                kCyan+3,    // QE-like
                                                kRed+2,     // Pion production
                                                kYellow,    // Plastic upstream
                                                kOrange,    // Plastic between
                                                kOrange+2,  // Plastic downstream
                                                kGray};     // Other background
    
    
    // Material color scheme
    const int Ncolors_mat = 7;
    
    const int line_colors_mat[Ncolors_mat] = {kGray,       // Target 4 Pb
                                              kGray+1,     // Target 5 Pb
                                              kGray+2,     // Target 5 Fe
                                              kSpring,     // Plastic upstream
                                              kMagenta,    // Plastic between
                                              kBlue-5,     // Plastic downstream
                                              kYellow-4};  // Other material
    
    const int fill_colors_mat[Ncolors_mat] = {kGray,       // Target 4 Pb
                                              kGray+1,     // Target 5 Pb
                                              kGray+2,     // Target 5 Fe
                                              kSpring,     // Plastic upstream
                                              kMagenta,    // Plastic between
                                              kBlue-5,     // Plastic downstream
                                              kYellow-4};  // Other material
    
    // PDG color scheme
    const int Ncolors_pdg = 8;
    
    const int line_colors_pdg[Ncolors_pdg] = {kAzure-4,   // Pi0 + EM
                                              kCyan+3,    // Proton
                                              kViolet-4,  // Neutron
                                              kRed+2,     // Charged pion
                                              kYellow+1,  // Muon
                                              kGreen+2,   // Other PDG
                                              kGray,      // MC X-talk
                                              kGray+1};   // Overlay
    
    const int fill_colors_pdg[Ncolors_pdg] = {kAzure-4,   // Pi0 + EM
                                              kCyan+3,    // Proton
                                              kViolet-4,  // Neutron
                                              kRed+2,     // Charged pion
                                              kYellow+1,  // Muon
                                              kGreen+2,   // Other PDG
                                              kGray,      // MC X-talk
                                              kGray+1};   // Overlay
    
    // Interaction type scheme
    const int Ncolors_inttype = 8;
    
    const int line_colors_inttype[Ncolors_inttype] = {kCyan+3,    // QE
                                                      kYellow+1,  // MEC
                                                      kAzure-4,   // Delta RES
                                                      kRed+2,     // Other RES
                                                      kOrange,    // Soft DIS
                                                      kViolet,    // True DIS
                                                      kGray+1};   // Other
    
    const int fill_colors_inttype[Ncolors_inttype] = {kCyan+3,    // QE
                                                      kYellow+1,  // MEC
                                                      kAzure-4,   // Delta RES
                                                      kRed+2,     // Other RES
                                                      kOrange,    // Soft DIS
                                                      kViolet,    // True DIS
                                                      kGray+1};   // Other
}  // namespace CCPi0AnaStackColors



void SetHistColorScheme(PlotUtils::MnvH1D* h,
                        const int type,
                        const int color_scheme)
{
    switch ( color_scheme ) {
        // Signal and background breakdown
        case 1 :
            h -> SetFillColor(CCPi0AnaStackColors::fill_colors_phys[type]);
            h -> SetLineColor(CCPi0AnaStackColors::line_colors_phys[type]);
            break;
        
        // Material breakdown
        case 2 :
            h -> SetFillColor(CCPi0AnaStackColors::fill_colors_mat[type]);
            h -> SetLineColor(CCPi0AnaStackColors::line_colors_mat[type]);
            break;
            
        // PDG breakdown
        case 3 :
            h -> SetFillColor(CCPi0AnaStackColors::fill_colors_pdg[type]);
            h -> SetLineColor(CCPi0AnaStackColors::line_colors_pdg[type]);
            break;
            
        // Interaction type breakdown
        case 4 :
            h -> SetFillColor(CCPi0AnaStackColors::line_colors_inttype[type]);
            h -> SetLineColor(CCPi0AnaStackColors::fill_colors_inttype[type]);
            break;
            
        default:
            std::cout << " NEED TO SET RIGHT COLOR SCHEME FOR STACKED HISTOS!! " << std::endl;
            return;
    }
}


#endif  // Constants_h