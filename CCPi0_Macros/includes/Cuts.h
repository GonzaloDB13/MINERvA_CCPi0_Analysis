#ifndef Cuts_h
#define Cuts_h

#include <string>
#include <vector>

#include "CVUniverse.h"
#include "Constants.h"  // EnumCuts



// ==============================================================================
//  VECTOR OF CUTS -- SIGNAL REGION
// ==============================================================================

std::vector<EnumCuts> GetCutsVector_SigReg()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}





// ==============================================================================
//  VECTOR OF CUTS -- PION-LIKE SHOWER SIDEBAND
// ==============================================================================

// Single-blob events
std::vector<EnumCuts> GetCutsVector_PionBlobSB_SingleBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}


// Multi-blob events
std::vector<EnumCuts> GetCutsVector_PionBlobSB_MultiBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    // cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}





// ==============================================================================
//  VECTOR OF CUTS -- PROTON-LIKE SHOWER SIDEBAND
// ==============================================================================

// Single-blob events
std::vector<EnumCuts> GetCutsVector_ProtonBlobSB_SingleBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    // cuts_vec.push_back(kRecoCut_BlobDeviation);
    // cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}


// Multi-blob events
std::vector<EnumCuts> GetCutsVector_ProtonBlobSB_MultiBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}





// ==============================================================================
//  VECTOR OF CUTS -- HIGH-W SIDEBAND
// ==============================================================================

// Single-blob events
std::vector<EnumCuts> GetCutsVector_HighWSB_SingleBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}


// Multi-blob events
std::vector<EnumCuts> GetCutsVector_HighWSB_MultiBlob()
{
#ifndef __CINT__
    std::vector<EnumCuts> cuts_vec;
    
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    // cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    
    return cuts_vec;
#endif  // __CINT__
}





// ==============================================================================
//  CUT FUNCTIONS
// ==============================================================================

bool RecoCut_InteractionVertex(const CVUniverse&);
bool RecoCut_NeutrinoHelicity (const CVUniverse&);
bool RecoCut_MinosMatch       (const CVUniverse&);
bool RecoCut_MuonCharge       (const CVUniverse&);
bool RecoCut_MuonTrackAngle   (const CVUniverse&);
bool RecoCut_DeadTime         (const CVUniverse&);
bool RecoCut_FiducialVolume   (const CVUniverse&, std::string option_material);
bool RecoCut_MichelElectrons  (const CVUniverse&);
bool RecoCut_LongTracks       (const CVUniverse&);
bool RecoCut_EventHasBlobs    (const CVUniverse&);
bool RecoCut_BlobAngleWRTMuon (const CVUniverse&, std::string option_material);
bool RecoCut_BlobDeviation    (const CVUniverse&, std::string option_material);
bool RecoCut_BlobEnergyVSdx   (const CVUniverse&, std::string option_material);
bool RecoCut_BlobdEdxEnd      (const CVUniverse&, std::string option_material);
bool RecoCut_BlobHasMichel    (const CVUniverse&);
bool RecoCut_NoPi0RecoilEnergy(const CVUniverse&, std::string option_material);





// ==============================================================================
//  HELPER FUNCTIONS
// ==============================================================================

// Check that universe passes individual cut
bool PassesCut(const CVUniverse&,
               EnumCuts cut,
               std::string option_material,
               bool use_fiducial_cut = true);


// Check that universe passes a vector of cuts
bool PassesCuts(const CVUniverse&,
                std::vector<EnumCuts> cuts,
                std::string option_material,
                bool use_fiducial_cut = true);


// Check that universe is part of a sideband
bool IsSideband(const CVUniverse&,
                std::vector<EnumCuts> sb_cuts_singleblob,
                std::vector<EnumCuts> sb_cuts_multiblob,
                std::string option_material,
                bool use_fiducial_cut = true);





// ==============================================================================
//  CHECK SIGNAL REGION AND PHYSICS SIDEBANDS
// ==============================================================================

// Signal region
bool IsSigReg(const CVUniverse&,
              std::string option_material,
              bool use_fiducial_cut = true);


// Pion-like shower sideband
bool IsPionBlobSB(const CVUniverse&,
                  std::string option_material,
                  bool use_fiducial_cut = true);


// Proton-like shower sideband
bool IsProtonBlobSB(const CVUniverse&,
                    std::string option_material,
                    bool use_fiducial_cut = true);


// High-W sideband
bool IsHighWSB(const CVUniverse&,
               std::string option_material,
               bool use_fiducial_cut = true);





// ==============================================================================
//  CHECK RECO MATERIALS AND PLASTIC SIDEBANDS
// ==============================================================================

// Reconstructed Pb
bool IsRecoPb(const CVUniverse& univ);


// Reconstructed Fe
bool IsRecoFe(const CVUniverse& univ);


// Plastic sideband upstream target 4
bool IsPlasUpSB(const CVUniverse& univ);


// Plastic sideband between
bool IsPlasBetwSB(const CVUniverse& univ);


// Plastic sideband downstream target 5
bool IsPlasDownSB(const CVUniverse& univ);


#endif  // Cuts_h