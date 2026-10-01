#ifndef SidebandStudies_C
#define SidebandStudies_C

#include <iostream>
#include <vector>

#include "../includes/Cuts.h"
#include "../includes/CVUniverse.h"
#include "../includes/MacroUtil.h"
#include "../includes/CCPi0Event.h"
#include "../includes/TruthMatching.h"
#include "../includes/Constants.h"
#include "../includes/Binning.h"
#include "../includes/GetVariables.h"
#include "../includes/common_functions.h"
#include "../includes/util.h"

#ifndef __CINT__
#include "../includes/Variable.h"
#endif  // __CINT__

#include "TFile.h"



// ========================================================================================================================
//  SIDEBAND CONFIGURATION 1
// ========================================================================================================================

// Pion sideband
// =============
std::vector<EnumCuts> PionSBCuts_Conf1_SingleBlob() {
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
}

std::vector<EnumCuts> PionSBCuts_Conf1_MultiBlob() {
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
}


// Proton sideband
// ===============
std::vector<EnumCuts> ProtonSBCuts_Conf1_SingleBlob() {
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
}

std::vector<EnumCuts> ProtonSBCuts_Conf1_MultiBlob() {
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
}


// High-W sideband
// ===============
std::vector<EnumCuts> HighWSBCuts_Conf1_SingleBlob() {
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
}

std::vector<EnumCuts> HighWSBCuts_Conf1_MultiBlob() {
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
}





// ========================================================================================================================
//  SIDEBAND CONFIGURATION 2
// ========================================================================================================================

// Pion sideband
// =============
std::vector<EnumCuts> PionSBCuts_Conf2_SingleBlob() {
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
}

std::vector<EnumCuts> PionSBCuts_Conf2_MultiBlob() {
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
    // cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}


// Proton sideband
// ===============
std::vector<EnumCuts> ProtonSBCuts_Conf2_SingleBlob() {
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
}

std::vector<EnumCuts> ProtonSBCuts_Conf2_MultiBlob() {
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
}


// High-W sideband
// ===============
std::vector<EnumCuts> HighWSBCuts_Conf2_SingleBlob() {
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
}

std::vector<EnumCuts> HighWSBCuts_Conf2_MultiBlob() {
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
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}





// ========================================================================================================================
//  SIDEBAND CONFIGURATION 3
// ========================================================================================================================

// Pion sideband
// =============
std::vector<EnumCuts> PionSBCuts_Conf3_SingleBlob() {
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
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}

std::vector<EnumCuts> PionSBCuts_Conf3_MultiBlob() {
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
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}


// Proton sideband
// ===============
std::vector<EnumCuts> ProtonSBCuts_Conf3_SingleBlob() {
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
}

std::vector<EnumCuts> ProtonSBCuts_Conf3_MultiBlob() {
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
}


// High-W sideband
// ===============
std::vector<EnumCuts> HighWSBCuts_Conf3_SingleBlob() {
    std::vector<EnumCuts> cuts_vec;
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}

std::vector<EnumCuts> HighWSBCuts_Conf3_MultiBlob() {
    std::vector<EnumCuts> cuts_vec;
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    // cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}





// ========================================================================================================================
//  SIDEBAND CONFIGURATION 4
// ========================================================================================================================

// Pion sideband
// =============
std::vector<EnumCuts> PionSBCuts_Conf4_SingleBlob() {
    std::vector<EnumCuts> cuts_vec;
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}

std::vector<EnumCuts> PionSBCuts_Conf4_MultiBlob() {
    std::vector<EnumCuts> cuts_vec;
    cuts_vec.push_back(kRecoCut_InteractionVertex);
    cuts_vec.push_back(kRecoCut_NeutrinoHelicity);
    cuts_vec.push_back(kRecoCut_MinosMatch);
    cuts_vec.push_back(kRecoCut_MuonCharge);
    cuts_vec.push_back(kRecoCut_MuonTrackAngle);
    cuts_vec.push_back(kRecoCut_DeadTime);
    cuts_vec.push_back(kRecoCut_FiducialVolume);
    // cuts_vec.push_back(kRecoCut_MichelElectrons);
    // cuts_vec.push_back(kRecoCut_LongTracks);
    cuts_vec.push_back(kRecoCut_EventHasBlobs);
    cuts_vec.push_back(kRecoCut_BlobAngleWRTMuon);
    cuts_vec.push_back(kRecoCut_BlobDeviation);
    cuts_vec.push_back(kRecoCut_BlobEnergyVSdx);
    cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    cuts_vec.push_back(kRecoCut_BlobHasMichel);
    cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}


// Proton sideband
// ===============
std::vector<EnumCuts> ProtonSBCuts_Conf4_SingleBlob() {
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
}

std::vector<EnumCuts> ProtonSBCuts_Conf4_MultiBlob() {
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
}


// High-W sideband
// ===============
std::vector<EnumCuts> HighWSBCuts_Conf4_SingleBlob() {
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
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}

std::vector<EnumCuts> HighWSBCuts_Conf4_MultiBlob() {
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
    // cuts_vec.push_back(kRecoCut_BlobdEdxEnd);
    // cuts_vec.push_back(kRecoCut_BlobHasMichel);
    // cuts_vec.push_back(kRecoCut_NoPi0RecoilEnergy);
    return cuts_vec;
}





// ========================================================================================================================
//  LOOP OVER ERROR BANDS AND FILL HISTOGRAMS
// ========================================================================================================================

#ifndef __CINT__

void LoopAndFillHistograms(const CCPi0::MacroUtil& util,
                           const EnumDataMCTruth& type_DataMCTruth,
                           const EnumModels& type_model,
                           std::string option_material,
                           std::vector<Variable*>& variables)
{
    // Setup loop
    bool is_mc, is_truth;
    Long64_t n_entries;
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " \tStarting loop to fill distributions on " << option_material << "... " << std::endl;
    SetupLoop(type_DataMCTruth, type_model, util, is_mc, is_truth, n_entries);
    
    
    // Get error bands
    UniverseMap error_bands;
    
    if ( type_DataMCTruth == kMC || type_DataMCTruth == kTruth ) {
        if ( is_truth ) error_bands = util.m_error_bands_truth;
        else            error_bands = util.m_error_bands;
    }
    
    else if ( type_DataMCTruth == kData ) {
        CVUniverse* data_universe = util.m_data_universe;
        std::vector<CVUniverse*> data_band = {data_universe};
        error_bands["cv_data"] = data_band;
    }
    
    
    // Define TStopwatch
    TStopwatch sw;
    
    
    // Get sideband cuts
    std::vector<EnumCuts> cuts_PionSB_Conf1_SingleBlob   = PionSBCuts_Conf1_SingleBlob();
    std::vector<EnumCuts> cuts_PionSB_Conf1_MultiBlob    = PionSBCuts_Conf1_MultiBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf1_SingleBlob = ProtonSBCuts_Conf1_SingleBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf1_MultiBlob  = ProtonSBCuts_Conf1_MultiBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf1_SingleBlob  = HighWSBCuts_Conf1_SingleBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf1_MultiBlob   = HighWSBCuts_Conf1_MultiBlob();
    
    std::vector<EnumCuts> cuts_PionSB_Conf2_SingleBlob   = PionSBCuts_Conf2_SingleBlob();
    std::vector<EnumCuts> cuts_PionSB_Conf2_MultiBlob    = PionSBCuts_Conf2_MultiBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf2_SingleBlob = ProtonSBCuts_Conf2_SingleBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf2_MultiBlob  = ProtonSBCuts_Conf2_MultiBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf2_SingleBlob  = HighWSBCuts_Conf2_SingleBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf2_MultiBlob   = HighWSBCuts_Conf2_MultiBlob();
    
    std::vector<EnumCuts> cuts_PionSB_Conf3_SingleBlob   = PionSBCuts_Conf3_SingleBlob();
    std::vector<EnumCuts> cuts_PionSB_Conf3_MultiBlob    = PionSBCuts_Conf3_MultiBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf3_SingleBlob = ProtonSBCuts_Conf3_SingleBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf3_MultiBlob  = ProtonSBCuts_Conf3_MultiBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf3_SingleBlob  = HighWSBCuts_Conf3_SingleBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf3_MultiBlob   = HighWSBCuts_Conf3_MultiBlob();
    
    std::vector<EnumCuts> cuts_PionSB_Conf4_SingleBlob   = PionSBCuts_Conf4_SingleBlob();
    std::vector<EnumCuts> cuts_PionSB_Conf4_MultiBlob    = PionSBCuts_Conf4_MultiBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf4_SingleBlob = ProtonSBCuts_Conf4_SingleBlob();
    std::vector<EnumCuts> cuts_ProtonSB_Conf4_MultiBlob  = ProtonSBCuts_Conf4_MultiBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf4_SingleBlob  = HighWSBCuts_Conf4_SingleBlob();
    std::vector<EnumCuts> cuts_HighWSB_Conf4_MultiBlob   = HighWSBCuts_Conf4_MultiBlob();
    
    
    // Loop over entries
    // =================
    
    for ( Long64_t i_event = 0; i_event < n_entries; ++i_event )
    {
        // Report progress
        if ( i_event%10000 == 0 ) reportProgress(double(i_event)/n_entries, sw);
        
        
        // Boolean to check if vertical-only universe has been checked by cuts, by default it's false.
        // Once the first time a vertical-only universe is evaluated, it becomes true and
        // there won't be a need to check again until the next entry
        bool vert_universe_checked_SigReg = false;
        
        bool vert_universe_checked_PionSB_Conf1   = false;
        bool vert_universe_checked_ProtonSB_Conf1 = false;
        bool vert_universe_checked_HighWSB_Conf1  = false;
        
        bool vert_universe_checked_PionSB_Conf2   = false;
        bool vert_universe_checked_ProtonSB_Conf2 = false;
        bool vert_universe_checked_HighWSB_Conf2  = false;
        
        bool vert_universe_checked_PionSB_Conf3   = false;
        bool vert_universe_checked_ProtonSB_Conf3 = false;
        bool vert_universe_checked_HighWSB_Conf3  = false;
        
        bool vert_universe_checked_PionSB_Conf4   = false;
        bool vert_universe_checked_ProtonSB_Conf4 = false;
        bool vert_universe_checked_HighWSB_Conf4  = false;
        
        
        // Booleans to check if vertical-only universe fulfills reconstructed cuts.
        // Similarly, it only needs to be evaluated once for a vertical-only universe
        bool vert_universe_passes_SigReg = false;
        
        bool vert_universe_passes_PionSB_Conf1   = false;
        bool vert_universe_passes_ProtonSB_Conf1 = false;
        bool vert_universe_passes_HighWSB_Conf1  = false;
        
        bool vert_universe_passes_PionSB_Conf2   = false;
        bool vert_universe_passes_ProtonSB_Conf2 = false;
        bool vert_universe_passes_HighWSB_Conf2  = false;
        
        bool vert_universe_passes_PionSB_Conf3   = false;
        bool vert_universe_passes_ProtonSB_Conf3 = false;
        bool vert_universe_passes_HighWSB_Conf3  = false;
        
        bool vert_universe_passes_PionSB_Conf4   = false;
        bool vert_universe_passes_ProtonSB_Conf4 = false;
        bool vert_universe_passes_HighWSB_Conf4  = false;
        
        
        // Loop over error bands
        // =====================
        
        for ( auto error_band : error_bands )
        {
            // Get universes of this error band
            std::vector<CVUniverse*> universes = error_band.second;
            
            
            // Loop over universes
            // ===================
            
            for ( auto universe : universes )
            {
                // Set entry of this universe
                universe -> SetEntry(i_event);
                
                
                // Define CCPi0 event
                // (Set material option and include cut on fiducial volume)
                CCPi0Event event(is_mc, is_truth, universe, type_model, option_material, true);
                
                
                // Check cuts
                // ==========
                
                bool event_passes_SigReg = false;
                
                bool event_passes_PionSB_Conf1   = false;
                bool event_passes_ProtonSB_Conf1 = false;
                bool event_passes_HighWSB_Conf1  = false;
                
                bool event_passes_PionSB_Conf2   = false;
                bool event_passes_ProtonSB_Conf2 = false;
                bool event_passes_HighWSB_Conf2  = false;
                
                bool event_passes_PionSB_Conf3   = false;
                bool event_passes_ProtonSB_Conf3 = false;
                bool event_passes_HighWSB_Conf3  = false;
                
                bool event_passes_PionSB_Conf4   = false;
                bool event_passes_ProtonSB_Conf4 = false;
                bool event_passes_HighWSB_Conf4  = false;
                
                
                // Check if universe of this error band is vertical-only.
                // If it is vertical-only, cuts would need to be checked only once
                if ( universe->IsVerticalOnly() )
                {
                    // If cuts haven't been checked for this set of vertical-only universes, check cuts for the first time
                    // Once universe has been checked for the first time, be sure that other vertical-only universes within this error band are not checked
                    
                    // Signal region
                    if ( !vert_universe_checked_SigReg ) vert_universe_passes_SigReg = IsSigReg(event);
                    
                    // Coniguration 1
                    if ( !vert_universe_checked_PionSB_Conf1 ) {
                        vert_universe_passes_PionSB_Conf1 = IsSideband(*universe, cuts_PionSB_Conf1_SingleBlob, cuts_PionSB_Conf1_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_ProtonSB_Conf1 ) {
                        vert_universe_passes_ProtonSB_Conf1 = IsSideband(*universe, cuts_ProtonSB_Conf1_SingleBlob, cuts_ProtonSB_Conf1_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_HighWSB_Conf1 ) {
                        vert_universe_passes_HighWSB_Conf1 = IsSideband(*universe, cuts_HighWSB_Conf1_SingleBlob, cuts_HighWSB_Conf1_MultiBlob, option_material, true);
                    }
                    
                    // Configuration 2
                    if ( !vert_universe_checked_PionSB_Conf2 ) {
                        vert_universe_passes_PionSB_Conf2 = IsSideband(*universe, cuts_PionSB_Conf2_SingleBlob, cuts_PionSB_Conf2_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_ProtonSB_Conf2 ) {
                        vert_universe_passes_ProtonSB_Conf2 = IsSideband(*universe, cuts_ProtonSB_Conf2_SingleBlob, cuts_ProtonSB_Conf2_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_HighWSB_Conf2 ) {
                        vert_universe_passes_HighWSB_Conf2 = IsSideband(*universe, cuts_HighWSB_Conf2_SingleBlob, cuts_HighWSB_Conf2_MultiBlob, option_material, true);
                    }
                    
                    // Configuration 3
                    if ( !vert_universe_checked_PionSB_Conf3 ) {
                        vert_universe_passes_PionSB_Conf3 = IsSideband(*universe, cuts_PionSB_Conf3_SingleBlob, cuts_PionSB_Conf3_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_ProtonSB_Conf3 ) {
                        vert_universe_passes_ProtonSB_Conf3 = IsSideband(*universe, cuts_ProtonSB_Conf3_SingleBlob, cuts_ProtonSB_Conf3_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_HighWSB_Conf3 ) {
                        vert_universe_passes_HighWSB_Conf3 = IsSideband(*universe, cuts_HighWSB_Conf3_SingleBlob, cuts_HighWSB_Conf3_MultiBlob, option_material, true);
                    }
                    
                    // Configuration 4
                    if ( !vert_universe_checked_PionSB_Conf4 ) {
                        vert_universe_passes_PionSB_Conf4 = IsSideband(*universe, cuts_PionSB_Conf4_SingleBlob, cuts_PionSB_Conf4_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_ProtonSB_Conf4 ) {
                        vert_universe_passes_ProtonSB_Conf4 = IsSideband(*universe, cuts_ProtonSB_Conf4_SingleBlob, cuts_ProtonSB_Conf4_MultiBlob, option_material, true);
                    }
                    if ( !vert_universe_checked_HighWSB_Conf4 ) {
                        vert_universe_passes_HighWSB_Conf4 = IsSideband(*universe, cuts_HighWSB_Conf4_SingleBlob, cuts_HighWSB_Conf4_MultiBlob, option_material, true);
                    }
                    
                    
                    // If cuts have already been checked either in this vertical-only universe or on another one,
                    // there's no need to evaluate them again and this universe's data members can be assigned
                    if ( vert_universe_checked_SigReg ) event_passes_SigReg = vert_universe_passes_SigReg;
                    
                    if ( vert_universe_checked_PionSB_Conf1 )   event_passes_PionSB_Conf1   = vert_universe_passes_PionSB_Conf1;
                    if ( vert_universe_checked_ProtonSB_Conf1 ) event_passes_ProtonSB_Conf1 = vert_universe_passes_ProtonSB_Conf1;
                    if ( vert_universe_checked_HighWSB_Conf1 )  event_passes_HighWSB_Conf1  = vert_universe_passes_HighWSB_Conf1;
                    
                    if ( vert_universe_checked_PionSB_Conf2 )   event_passes_PionSB_Conf2   = vert_universe_passes_PionSB_Conf2;
                    if ( vert_universe_checked_ProtonSB_Conf2 ) event_passes_ProtonSB_Conf2 = vert_universe_passes_ProtonSB_Conf2;
                    if ( vert_universe_checked_HighWSB_Conf2 )  event_passes_HighWSB_Conf2  = vert_universe_passes_HighWSB_Conf2;
                    
                    if ( vert_universe_checked_PionSB_Conf3 )   event_passes_PionSB_Conf3   = vert_universe_passes_PionSB_Conf3;
                    if ( vert_universe_checked_ProtonSB_Conf3 ) event_passes_ProtonSB_Conf3 = vert_universe_passes_ProtonSB_Conf3;
                    if ( vert_universe_checked_HighWSB_Conf3 )  event_passes_HighWSB_Conf3  = vert_universe_passes_HighWSB_Conf3;
                    
                    if ( vert_universe_checked_PionSB_Conf4 )   event_passes_PionSB_Conf4   = vert_universe_passes_PionSB_Conf4;
                    if ( vert_universe_checked_ProtonSB_Conf4 ) event_passes_ProtonSB_Conf4 = vert_universe_passes_ProtonSB_Conf4;
                    if ( vert_universe_checked_HighWSB_Conf4 )  event_passes_HighWSB_Conf4  = vert_universe_passes_HighWSB_Conf4;
                }
                
                
                // If universe is not vertical-only, check cuts universe-by-universe
                // since in some universes cuts may or may not be satisfied
                else {
                    // Signal region
                    event_passes_SigReg = IsSigReg(event);
                    
                    // Configuration 1
                    event_passes_PionSB_Conf1   = IsSideband(*universe, cuts_PionSB_Conf1_SingleBlob, cuts_PionSB_Conf1_MultiBlob, option_material, true);
                    event_passes_ProtonSB_Conf1 = IsSideband(*universe, cuts_ProtonSB_Conf1_SingleBlob, cuts_ProtonSB_Conf1_MultiBlob, option_material, true);
                    event_passes_HighWSB_Conf1  = IsSideband(*universe, cuts_HighWSB_Conf1_SingleBlob, cuts_HighWSB_Conf1_MultiBlob, option_material, true);
                    
                    // Configuration 2
                    event_passes_PionSB_Conf2   = IsSideband(*universe, cuts_PionSB_Conf2_SingleBlob, cuts_PionSB_Conf2_MultiBlob, option_material, true);
                    event_passes_ProtonSB_Conf2 = IsSideband(*universe, cuts_ProtonSB_Conf2_SingleBlob, cuts_ProtonSB_Conf2_MultiBlob, option_material, true);
                    event_passes_HighWSB_Conf2  = IsSideband(*universe, cuts_HighWSB_Conf2_SingleBlob, cuts_HighWSB_Conf2_MultiBlob, option_material, true);
                    
                    // Configuration 3
                    event_passes_PionSB_Conf3   = IsSideband(*universe, cuts_PionSB_Conf3_SingleBlob, cuts_PionSB_Conf3_MultiBlob, option_material, true);
                    event_passes_ProtonSB_Conf3 = IsSideband(*universe, cuts_ProtonSB_Conf3_SingleBlob, cuts_ProtonSB_Conf3_MultiBlob, option_material, true);
                    event_passes_HighWSB_Conf3  = IsSideband(*universe, cuts_HighWSB_Conf3_SingleBlob, cuts_HighWSB_Conf3_MultiBlob, option_material, true);
                    
                    // Configuration 4
                    event_passes_PionSB_Conf4   = IsSideband(*universe, cuts_PionSB_Conf4_SingleBlob, cuts_PionSB_Conf4_MultiBlob, option_material, true);
                    event_passes_ProtonSB_Conf4 = IsSideband(*universe, cuts_ProtonSB_Conf4_SingleBlob, cuts_ProtonSB_Conf4_MultiBlob, option_material, true);
                    event_passes_HighWSB_Conf4  = IsSideband(*universe, cuts_HighWSB_Conf4_SingleBlob, cuts_HighWSB_Conf4_MultiBlob, option_material, true);
                }
                
                
                // Fill histograms in MC reco and data
                // ===================================
                
                for ( auto var : variables )
                {
                    if ( (event_passes_SigReg  && (var->Name() == "SigReg_Conf1" || var->Name() == "SigReg_Conf2" || var->Name() == "SigReg_Conf3" || var->Name() == "SigReg_Conf4") ) ||
                         (event_passes_PionSB_Conf1   && var->Name() == "PionSB_Conf1"  ) ||
                         (event_passes_ProtonSB_Conf1 && var->Name() == "ProtonSB_Conf1") ||
                         (event_passes_HighWSB_Conf1  && var->Name() == "HighWSB_Conf1" ) ||
                         (event_passes_PionSB_Conf2   && var->Name() == "PionSB_Conf2"  ) ||
                         (event_passes_ProtonSB_Conf2 && var->Name() == "ProtonSB_Conf2") ||
                         (event_passes_HighWSB_Conf2  && var->Name() == "HighWSB_Conf2" ) ||
                         (event_passes_PionSB_Conf3   && var->Name() == "PionSB_Conf3"  ) ||
                         (event_passes_ProtonSB_Conf3 && var->Name() == "ProtonSB_Conf3") ||
                         (event_passes_HighWSB_Conf3  && var->Name() == "HighWSB_Conf3" ) ||
                         (event_passes_PionSB_Conf4   && var->Name() == "PionSB_Conf4"  ) ||
                         (event_passes_ProtonSB_Conf4 && var->Name() == "ProtonSB_Conf4") ||
                         (event_passes_HighWSB_Conf4  && var->Name() == "HighWSB_Conf4" ) )
                    {
                        if ( type_DataMCTruth == kMC ) {
                            ccpi0_event::FillMCHists_Selection(event, var);
                        }
                        else if ( type_DataMCTruth == kData ) {
                            ccpi0_event::FillDataHists_Selection(event, var);
                        }
                    }
                }  // End of loop over variables
                
            }  // End of loop over universes
            
        }  // End of loop over error bands
        
    }  // End of loop over entries
}

#endif  // __CINT__





// ========================================================================================================================
//  PROCESS MC TUPLES
// ========================================================================================================================

void ProcessMCTuples(const CCPi0::MacroUtil& util,
                     TFile& fout,
                     const EnumModels& type_model,
                     std::string option_material)
{
    // Get variables
    std::vector<Variable*> variables = GetSidebandStudyVariables();
    
    
    // Initialize histograms
    for ( auto var : variables ) {
        var -> InitMCHists_Selection(util.m_error_bands);
    }
    
    
    // Loop and fill histograms
    MinervaUniverse::SetTruth(false);
    LoopAndFillHistograms(util, kMC, type_model, option_material, variables);
    
    
    // Write POT
    WritePOT(fout, true, util.m_mc_pot);
    
    
    // Sync and write histograms
    for ( auto var : variables ) {
        var -> SyncMCHists_Selection();
        var -> WriteMCHists_Selection(fout);
    }
}





// ========================================================================================================================
//  PROCESS DATA TUPLES
// ========================================================================================================================

void ProcessDataTuples(const CCPi0::MacroUtil& util,
                       TFile& fout,
                       std::string option_material)
{
    // Get variables
    std::vector<Variable*> variables = GetSidebandStudyVariables();
    
    
    // Initialize histograms
    for ( auto var : variables ) {
        var -> InitDataHists_Selection();
    }
    
    
    // Loop and fill histograms
    LoopAndFillHistograms(util, kData, kDataNoModel, option_material, variables);
    
    
    // Write POT
    WritePOT(fout, false, util.m_data_pot);
    
    
    // Write histograms
    for ( auto var : variables ) {
        var -> WriteDataHists_Selection(fout);
    }
}





// ========================================================================================================================
//  SET MACROUTIL
// ========================================================================================================================

// =============
//  Monte Carlo
// =============
void SetMacroUtilMC(TFile& fout_lead, TFile& fout_iron,
                    std::string plist_string,
                    std::string file_list,
                    const EnumModels& type_model)
{
    // Set MacroUtil
    // (Set 'Truth' and 'Systematics' options as FALSE)
    CCPi0::MacroUtil util(file_list, plist_string, false, false, type_model);
    util.PrintMacroConfiguration("SidebandStudies");
    
    
    // Process event selection
    ProcessMCTuples(util, fout_lead, type_model, "lead");
    ProcessMCTuples(util, fout_iron, type_model, "iron");
}



// ======
//  Data
// ======
void SetMacroUtilData(TFile& fout_lead, TFile& fout_iron,
                      std::string plist_string,
                      std::string file_list)
{
    // Set MacroUtil
    CCPi0::MacroUtil util(file_list, plist_string);
    util.PrintMacroConfiguration("SidebandStudies");
    
    
    // Process event selection
    ProcessDataTuples(util, fout_lead, "lead");
    ProcessDataTuples(util, fout_iron, "iron");
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void SidebandStudies(bool is_mc,
                     std::string plist_string,
                     std::string option_model = "data",
                     bool is_grid             = false,
                     std::string input_file   = "",
                     int run                  = 0)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Stop program if running over data and MC-only options are on
    assert(!(!is_mc && type_model != kDataNoModel) && " When running data, 'option_model' must be set as 'data'!!! ");
    assert(!(is_mc  && type_model == kDataNoModel) && " When running MC, 'option_model' must not be set as 'data'!!! ");
    
    
    // Stop program if input file is not specified when running in the grid
    assert(!(is_grid && input_file.empty()) && " Input file must be specified when running on grid!!! ");
    
    
    // File list:
    // -> For grid jobs, uses input parameter
    // -> Interactively, uses 'test' sample as default (to change this, add a 3rd argument = "full")
    const std::string file_list = is_grid ? input_file : GetPlaylistFile(is_mc, plist_string);
    
    
    
    // =========================================
    //  Output file
    // =========================================
    
    // Output file names
    // =================
    
    // General option for output file names
    const std::string option_mc_data = is_mc ? "MC" : "Data";
    
    
    // Construct output file name on LEAD
    std::string fout_name_lead = is_mc ?
        Form("%s_SidebandStudies_MnvGENIE%s_%s_lead", option_mc_data.c_str(),
                                                      option_model.c_str(),
                                                      plist_string.c_str()) :
        Form("%s_SidebandStudies_%s_lead", option_mc_data.c_str(),
                                           plist_string.c_str());
    
    // Construct output file name on IRON
    std::string fout_name_iron = is_mc ?
        Form("%s_SidebandStudies_MnvGENIE%s_%s_iron", option_mc_data.c_str(),
                                                      option_model.c_str(),
                                                      plist_string.c_str()) :
        Form("%s_SidebandStudies_%s_iron", option_mc_data.c_str(),
                                           plist_string.c_str());
    
    
    // Define output file names for interactive or grid
    // ================================================
    
    // If run number is different than 0, add it to output file names
    if ( run != 0 ) {
        fout_name_lead = fout_name_lead + Form("_%d", run);
        fout_name_iron = fout_name_iron + Form("_%d", run);
    }
    
    
    // If macro is not in grid, add top output directories manually
    if ( !is_grid ) {
        const std::string fout_topdir = "/minerva/data/users/gonzalo/MAT/SidebandStudies/root_files_interactive_test/";  // Test sample
        const std::string is_mc_dir   = is_mc ? "mc/" : "data/";
        
        fout_name_lead = fout_topdir + fout_name_lead;
        fout_name_iron = fout_topdir + fout_name_iron;
    }
    
    
    // Add ROOT extension
    fout_name_lead = fout_name_lead + ".root";
    fout_name_iron = fout_name_iron + ".root";
    
    
    // Define output files
    TFile fout_lead(fout_name_lead.c_str(), "RECREATE");
    TFile fout_iron(fout_name_iron.c_str(), "RECREATE");
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  Set MacroUtil and fill histograms
    // =========================================
    
    // Monte Carlo
    if ( is_mc )
        SetMacroUtilMC(fout_lead, fout_iron,
                       plist_string, file_list, type_model);
    
    // Data
    else
        SetMacroUtilData(fout_lead, fout_iron,
                         plist_string, file_list);
    
    
    // Close ROOT files
    fout_lead.Close();
    fout_iron.Close();
    
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << " Success!! " << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


#endif  // SidebandStudies_C