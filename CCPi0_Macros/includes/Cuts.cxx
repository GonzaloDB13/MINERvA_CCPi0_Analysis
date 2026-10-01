#ifndef Cuts_cxx
#define Cuts_cxx

#include "Cuts.h"
#include "util.h"  // ContainerEraser()



// ==============================================================================
//  CUT FUNCTIONS
// ==============================================================================

// CUT 1: Interaction vertex
// =========================
bool RecoCut_InteractionVertex(const CVUniverse& univ) {
    return univ.Survive_InteractionVertex();
}



// CUT 2: Neutrino helicity
// ========================
bool RecoCut_NeutrinoHelicity(const CVUniverse& univ) {
    return univ.Survive_NeutrinoHelicity();
}



// CUT 3: MINOS match
// ==================
bool RecoCut_MinosMatch(const CVUniverse& univ) {
    return univ.Survive_MinosMatch();
}



// CUT 4: Muon charge
// ==================
bool RecoCut_MuonCharge(const CVUniverse& univ) {
    return univ.Survive_MuonCharge();
}



// CUT 5: Muon track angle w.r.t. beam
// ===================================
bool RecoCut_MuonTrackAngle(const CVUniverse& univ) {
    return univ.Survive_MuonTrackAngle();
}



// CUT 6: Dead time discriminators
// ===============================
bool RecoCut_DeadTime(const CVUniverse& univ) {
    return univ.Survive_DeadTime();
}



// CUT 7: Fiducial volume
// ======================
bool RecoCut_FiducialVolume(const CVUniverse& univ,
                            std::string option_material)
{
    // Lead of targets 4 and 5
    // -----------------------
    if ( option_material == "lead" ) {
        bool tgt4_pb = univ.Survive_FiducialReco_Tgt4Pb();
        bool tgt5_pb = univ.Survive_FiducialReco_Tgt5Pb();
        return (tgt4_pb || tgt5_pb);
    }
    
    // Iron of target 5
    // ----------------
    else if ( option_material == "iron" ) {
        bool tgt5_fe = univ.Survive_FiducialReco_Tgt5Fe();
        return tgt5_fe;
    }
    
    // Bad material
    // ------------
    else {
        std::cout << " RecoCut_FiducialVolume() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
        exit(1);
    }
}



// CUT 8: Michel electrons
// =======================
bool RecoCut_MichelElectrons(const CVUniverse& univ)
{
    // Start point vertices
    if ( univ.NstartPointVertexMichels() > 0 )
        return false;
    
    // Non-muon stop point vertices
    if ( univ.NstopPointVertexMichels() > 0 )
        return false;
    
    // Kinked vertices
    if ( univ.NkinkedVertexMichels() > 0 )
        return false;
    
    return true;
}



// CUT 9: Long tracks
// ==================
bool RecoCut_LongTracks(const CVUniverse& univ)
{
    bool prim_track_good = false;
    bool sec_track_good  = false;
    
    // Primary tracks
    if ( univ.NprimTracks() == 0 )
        prim_track_good = true;
    
    else if ( univ.NprimTracks() == 1 ) {
        int track_contained = univ.PrimTrackIsContained();
        int track_kinked    = univ.PrimTrackIsKinked();
        double track_score  = univ.PrimTrackPionScore();
        
        if ( (track_contained == 1) && (track_kinked == 0) && (track_score <= 4.0) )
            prim_track_good = true;
    }
    
    // Secondary tracks
    if ( univ.NsecTracks() == 0 )
        sec_track_good = true;
    
    return (prim_track_good && sec_track_good);
}



// CUT 10: Event has blobs
// =======================
bool RecoCut_EventHasBlobs(const CVUniverse& univ)
{
    // AngleScan blobs
    if ( univ.AngleScanNblobs() <= 0 )
        return false;
    
    // Basic quality blobs
    if ( univ.NblobsPassBasicQuality() <= 0 )
        return false;
    
    // Fit quality blobs
    int Nblobs = univ.NblobCandidates();
    if ( Nblobs <= 0 ) return false;
    
    // Check that all good-quality blobs are not bad fit type
    int Nblobs_good = 0;
    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
        if ( blob_index > 5 ) break;
        
        bool start_end_good = univ.GetBlobIsGoodStartAndEndPoints(blob_index);
        bool bad_fit_type1  = univ.GetBlobIsBadFitType1(blob_index);
        bool bad_fit_type2  = univ.GetBlobIsBadFitType2(blob_index);
        bool bad_fit_type3  = univ.GetBlobIsBadFitType3(blob_index);
        
        if ( start_end_good && !bad_fit_type1 && !bad_fit_type2 && !bad_fit_type3 )
            ++Nblobs_good;
    }
    if ( Nblobs != Nblobs_good )
        return false;
    
    // Return true if all is okay
    return true;
}



// CUT 11: Blob angle w.r.t. muon track
// ====================================
bool RecoCut_BlobAngleWRTMuon(const CVUniverse& univ,
                              std::string option_material)
{
    int Nblobs = univ.NblobCandidates();
    
    // Loop over blobs
    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index )
    {
        if ( blob_index > 5 ) break;
        
        double blob_angle = univ.GetBlobAxisAngleWRTMuon(blob_index);  // [deg]
        
        // Lead of targets 4 and 5
        // -----------------------
        if ( option_material == "lead" )
        {
            // Single-blob events
            if ( Nblobs == 1 && blob_index == 1 ) {
                if ( blob_angle < 12.0 ) return false;
            }
            
            // Multi-blob events
            else if ( Nblobs > 1 && blob_index >= 1 ) {
                if ( blob_angle < 6.0 ) return false;
            }
        }
        
        // Iron of target 5
        // ----------------
        else if ( option_material == "iron" )
        {
            // Single-blob events
            if ( Nblobs == 1 && blob_index == 1 ) {
                if ( blob_angle < 12.0 ) return false;
            }
            
            // Multi-blob events
            else if ( Nblobs > 1 && blob_index >= 1 ) {
                if ( blob_angle < 6.0 ) return false;
            }
        }
        
        // Bad material
        // ------------
        else {
            std::cout << " RecoCut_BlobAngleWRTMuon() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
            exit(1);
        }
    }
    
    // Return true if no bad blobs
    return true;
}



// CUT 12: Blob deviation
// ======================
bool RecoCut_BlobDeviation(const CVUniverse& univ,
                           std::string option_material)
{
    int Nblobs = univ.NblobCandidates();
    
    // Single-blob events
    // ------------------
    if ( Nblobs == 1 ) {
        bool pi0_like = univ.IsPi0Like_SingleBlob("BlobDeviation", option_material, 1);
        if ( !pi0_like ) return false;
    }
    
    // Multi-blob events
    // -----------------
    else if ( Nblobs >= 1 )
    {
        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
            if ( blob_index > 5 ) return false;
            
            bool pi0_like  = univ.IsPi0Like_MultiBlob("BlobDeviation", option_material, blob_index);
            if ( !pi0_like ) return false;
        }
    }
    
    // Return true if all blobs are pi0-like
    return true;
}



// CUT 13: Blob energy vs. dx
// ==========================
bool RecoCut_BlobEnergyVSdx(const CVUniverse& univ,
                            std::string option_material)
{
    int Nblobs = univ.NblobCandidates();
    
    // Single-blob events
    // ------------------
    if ( Nblobs == 1 ) {
        bool pi0_like = univ.IsPi0Like_SingleBlob("BlobEnergyVSdx", option_material, 1);
        if ( !pi0_like ) return false;
    }
    
    // Multi-blob events
    // -----------------
    else if ( Nblobs >= 1 )
    {
        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
            if ( blob_index > 5 ) return false;
            
            bool pi0_like  = univ.IsPi0Like_MultiBlob("BlobEnergyVSdx", option_material, blob_index);
            if ( !pi0_like ) return false;
        }
    }
    
    // Return true if all blobs are pi0-like
    return true;
}



// CUT 14: Blob end dE/dx
// ======================
bool RecoCut_BlobdEdxEnd(const CVUniverse& univ,
                         std::string option_material)
{
    int Nblobs = univ.NblobCandidates();
    
    // Single-blob events
    // ------------------
    if ( Nblobs == 1 ) {
        bool pi0_like = univ.IsPi0Like_SingleBlob("BlobdEdxEnd", option_material, 1);
        if ( !pi0_like ) return false;
    }
    
    // Multi-blob events
    // -----------------
    else if ( Nblobs >= 1 )
    {
        for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
            if ( blob_index > 5 ) return false;
            
            bool pi0_like  = univ.IsPi0Like_MultiBlob("BlobdEdxEnd", option_material, blob_index);
            if ( !pi0_like ) return false;
        }
    }
    
    // Return true if all blobs are pi0-like
    return true;
}



// CUT 15: Blob end-point Michel
// =============================
bool RecoCut_BlobHasMichel(const CVUniverse& univ)
{
    int Nblobs = univ.NblobCandidates();
    
    // Loop over blobs
    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index )
    {
        if ( blob_index > 5 ) break;
        
        int match = univ.GetBlobEndPointMichelMatch(blob_index);
        
        // Single-blob events
        if ( Nblobs == 1 && blob_index == 1 ) {
            if ( match > 0 ) return false;
        }
        
        // Multi-blob events
        else if ( Nblobs > 1 && blob_index >= 1 ) {
            if ( match > 0 ) return false;
        }
    }
    
    // Return true if no bad blobs
    return true;
}



// CUT 16: No-pi0 recoil energy
// ============================
bool RecoCut_NoPi0RecoilEnergy(const CVUniverse& univ,
                               std::string option_material)
{
    int Nblobs = univ.NblobCandidates();
    
    double nopi0_recoil = univ.GetNoPi0RecoilE();  // [GeV]
    double recoil = univ.GetRecoilE();  // [GeV]
    double w2 = univ.GetW2();  // [GeV^2/c^4]
    double w  = univ.GetW();   // [GeV/c^2]
    
    // Lead of targets 4 and 5
    // -----------------------
    if ( option_material == "lead" )
    {
        // Single-blob events
        if ( Nblobs == 1 ) {
            if ( nopi0_recoil > 0.8 ) return false;
        }
        
        // Multi-blob events
        else if ( Nblobs > 1 ) {
            if ( nopi0_recoil > 0.5 ) return false;
        }
    }
    
    // Iron of target 5
    // ----------------
    else if ( option_material == "iron" )
    {
        // Single-blob events
        if ( Nblobs == 1 ) {
            if ( nopi0_recoil > 0.8 ) return false;
        }
        
        // Multi-blob events
        else if ( Nblobs > 1 ) {
            if ( nopi0_recoil > 0.5 ) return false;
        }
    }
    
    // Bad material
    // ------------
    else {
        std::cout << " RecoCut_NoPi0RecoilEnergy() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
        exit(1);
    }
    
    // Return true if all is okay
    return true;
}





// ==============================================================================
//  HELPER FUNCTIONS
// ==============================================================================

// Check that universe passes individual cut
// =========================================

bool PassesCut(const CVUniverse& univ,
               EnumCuts cut,
               std::string option_material,
               bool use_fiducial_cut)
{
#ifndef __CINT__
    switch ( cut )
    {
        // No reco cuts
        case kNoRecoCuts :
            return true;
        
        // CUT 1: Interaction vertex
        case kRecoCut_InteractionVertex :
            return RecoCut_InteractionVertex(univ);
        
        // CUT 2: Neutrino helicity
        case kRecoCut_NeutrinoHelicity :
            return RecoCut_NeutrinoHelicity(univ);
        
        // CUT 3: MINOS match
        case kRecoCut_MinosMatch :
            return RecoCut_MinosMatch(univ);
        
        // CUT 4: Muon charge
        case kRecoCut_MuonCharge :
            return RecoCut_MuonCharge(univ);
        
        // CUT 5: Muon track angle
        case kRecoCut_MuonTrackAngle :
            return RecoCut_MuonTrackAngle(univ);
        
        // CUT 6: Dead time discriminators
        case kRecoCut_DeadTime :
            return RecoCut_DeadTime(univ);
        
        // CUT 7: Fiducial volume
        case kRecoCut_FiducialVolume : {
            if ( use_fiducial_cut ) return RecoCut_FiducialVolume(univ, option_material);
            else return true;
        }
        
        // CUT 8: Michel electrons
        case kRecoCut_MichelElectrons :
            return RecoCut_MichelElectrons(univ);
        
        // CUT 9: Long tracks
        case kRecoCut_LongTracks :
            return RecoCut_LongTracks(univ);
        
        // CUT 10: Event has blobs
        case kRecoCut_EventHasBlobs :
            return RecoCut_EventHasBlobs(univ);
        
        // CUT 11: Blob angle w.r.t. muon
        case kRecoCut_BlobAngleWRTMuon : {
            if ( option_material == "lead" )      return RecoCut_BlobAngleWRTMuon(univ, "lead");
            else if ( option_material == "iron" ) return RecoCut_BlobAngleWRTMuon(univ, "iron");
            else {
                std::cout << " PassesCut() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
                exit(1);
            }
        }
        
        // CUT 12: Blob deviation
        case kRecoCut_BlobDeviation : {
            if ( option_material == "lead" )      return RecoCut_BlobDeviation(univ, "lead");
            else if ( option_material == "iron" ) return RecoCut_BlobDeviation(univ, "iron");
            else {
                std::cout << " PassesCut() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
                exit(1);
            }
        }
        
        // CUT 13: Blob energy VS dx
        case kRecoCut_BlobEnergyVSdx : {
            if ( option_material == "lead" )      return RecoCut_BlobEnergyVSdx(univ, "lead");
            else if ( option_material == "iron" ) return RecoCut_BlobEnergyVSdx(univ, "iron");
            else {
                std::cout << " PassesCut() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
                exit(1);
            }
        }
        
        // CUT 14: Blob end dE/dx
        case kRecoCut_BlobdEdxEnd : {
            if ( option_material == "lead" )      return RecoCut_BlobdEdxEnd(univ, "lead");
            else if ( option_material == "iron" ) return RecoCut_BlobdEdxEnd(univ, "iron");
            else {
                std::cout << " PassesCut() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
                exit(1);
            }
        }
        
        // CUT 15: Blob end-point Michel
        case kRecoCut_BlobHasMichel :
            return RecoCut_BlobHasMichel(univ);
        
        // CUT 16: No-pi0 recoil energy
        case kRecoCut_NoPi0RecoilEnergy : {
            if ( option_material == "lead" )      return RecoCut_NoPi0RecoilEnergy(univ, "lead");
            else if ( option_material == "iron" ) return RecoCut_NoPi0RecoilEnergy(univ, "iron");
            else {
                std::cout << " PassesCut() ERROR: PICK RIGHT MATERIAL!!! " << std::endl;
                exit(1);
            }
        }
        
        // Cut not specified
        default: {
            std::cout << " PassesCut() ERROR: UNKNOWN CUT = " << cut << "!!! " << std::endl;
            exit(1);
        }
    };
#endif  // __CINT__
}



// Check that universe passes a vector of cuts
// ===========================================

bool PassesCuts(const CVUniverse& univ,
                std::vector<EnumCuts> cuts,
                std::string option_material,
                bool use_fiducial_cut)
{
#ifndef __CINT__

    for ( auto cut : cuts ) {
        bool pass_cut = PassesCut(univ, cut, option_material, use_fiducial_cut);
        if ( !pass_cut ) return false;
    }
    return true;
    
#endif  // __CINT__
}



// Check that universe is part of a sideband
// =========================================

bool IsSideband(const CVUniverse& univ,
                std::vector<EnumCuts> sb_cuts_singleblob,
                std::vector<EnumCuts> sb_cuts_multiblob,
                std::string option_material,
                bool use_fiducial_cut)
{
#ifndef __CINT__

    // Get number of blobs in the event
    int Nblobs = univ.NblobCandidates();
    
    // Sidebands must have at least one of more blobs
    if ( Nblobs <= 0 ) return false;
    
    // Get sideband cuts (depending on the number of blobs)
    std::vector<EnumCuts> sideband_cuts;
    if ( Nblobs == 1 )     sideband_cuts = sb_cuts_singleblob;
    else if ( Nblobs > 1 ) sideband_cuts = sb_cuts_multiblob;
    
    // Check that all cuts from sideband are passed
    for ( auto sb_cut : sideband_cuts ) {
        bool pass_cut = PassesCut(univ, sb_cut, option_material, use_fiducial_cut);
        if ( !pass_cut ) return false;
    }
    
    // Get signal region cuts
    std::vector<EnumCuts> sigreg_cuts = GetCutsVector_SigReg();
    
    // Get sideband "anti-cuts" by finding cuts that are part of signal region, but are missing from the sideband cuts
    std::vector<EnumCuts> anti_cuts;
    for ( auto sigreg_cut : sigreg_cuts ) {
        if ( std::find(sideband_cuts.begin(), sideband_cuts.end(), sigreg_cut) == sideband_cuts.end() )
            anti_cuts.push_back(sigreg_cut);
    }
    
    // Event is part of sideband if it fails at least one "anti-cut"
    for ( auto anti_cut : anti_cuts ) {
        bool pass_cut = PassesCut(univ, anti_cut, option_material, use_fiducial_cut);
        if ( !pass_cut ) return true;
    }
    
    // If event doesn't fail any "anti-cut", is not part of sideband
    return false;
    
#endif  // __CINT__
}





// ==============================================================================
//  CHECK SIGNAL REGION AND PHYSICS SIDEBANDS
// ==============================================================================

// Signal region
// =============

bool IsSigReg(const CVUniverse& univ,
              std::string option_material,
              bool use_fiducial_cut)
{
#ifndef __CINT__

    std::vector<EnumCuts> sigreg_cuts = GetCutsVector_SigReg();
    
    // Check that all signal region cuts are passed
    for ( auto sigreg_cut : sigreg_cuts ) {
        bool pass_cut = PassesCut(univ, sigreg_cut, option_material, use_fiducial_cut);
        if ( !pass_cut ) return false;
    }
    return true;
    
#endif  // __CINT__
}



// Pion-like shower sideband
// =========================

bool IsPionBlobSB(const CVUniverse& univ,
                  std::string option_material,
                  bool use_fiducial_cut)
{
#ifndef __CINT__

    std::vector<EnumCuts> sideband_cuts_singleblob = GetCutsVector_PionBlobSB_SingleBlob();
    std::vector<EnumCuts> sideband_cuts_multiblob  = GetCutsVector_PionBlobSB_MultiBlob();
    
    bool is_sideband = IsSideband(univ,
                                  sideband_cuts_singleblob,
                                  sideband_cuts_multiblob,
                                  option_material, use_fiducial_cut);
    return is_sideband;
    
#endif  // __CINT__
}



// Proton-like shower sideband
// ===========================

bool IsProtonBlobSB(const CVUniverse& univ,
                    std::string option_material,
                    bool use_fiducial_cut)
{
#ifndef __CINT__

    std::vector<EnumCuts> sideband_cuts_singleblob = GetCutsVector_ProtonBlobSB_SingleBlob();
    std::vector<EnumCuts> sideband_cuts_multiblob  = GetCutsVector_ProtonBlobSB_MultiBlob();
    
    bool is_sideband = IsSideband(univ,
                                  sideband_cuts_singleblob,
                                  sideband_cuts_multiblob,
                                  option_material, use_fiducial_cut);
    return is_sideband;
    
#endif  // __CINT__
}



// High-W sideband
// ===============

bool IsHighWSB(const CVUniverse& univ,
               std::string option_material,
               bool use_fiducial_cut)
{
#ifndef __CINT__

    std::vector<EnumCuts> sideband_cuts_singleblob = GetCutsVector_HighWSB_SingleBlob();
    std::vector<EnumCuts> sideband_cuts_multiblob  = GetCutsVector_HighWSB_MultiBlob();
    
    bool is_sideband = IsSideband(univ,
                                  sideband_cuts_singleblob,
                                  sideband_cuts_multiblob,
                                  option_material, use_fiducial_cut);
    return is_sideband;
    
#endif  // __CINT__
}





// ==============================================================================
//  CHECK RECO MATERIALS AND PLASTIC SIDEBANDS
// ==============================================================================

// Reconstructed Pb
// ================

bool IsRecoPb(const CVUniverse& univ)
{
#ifndef __CINT__

    bool tgt4_pb = univ.Survive_FiducialReco_Tgt4Pb();
    bool tgt5_pb = univ.Survive_FiducialReco_Tgt5Pb();
    return (tgt4_pb || tgt5_pb);
    
#endif  // __CINT__
}



// Reconstructed Fe
// ================

bool IsRecoFe(const CVUniverse& univ)
{
#ifndef __CINT__

    bool tgt5_fe = univ.Survive_FiducialReco_Tgt5Fe();
    return tgt5_fe;
    
#endif  // __CINT__
}



// Plastic sideband upstream target 4
// ==================================

bool IsPlasUpSB(const CVUniverse& univ)
{
#ifndef __CINT__

    bool sideband = univ.Sideband_PlasUp();
    return sideband;
    
#endif  // __CINT__
}



// Plastic sideband between
// ========================

bool IsPlasBetwSB(const CVUniverse& univ)
{
#ifndef __CINT__

    bool sideband = univ.Sideband_PlasBetw();
    return sideband;
    
#endif  // __CINT__
}



// Plastic sideband downstream target 5
// ====================================

bool IsPlasDownSB(const CVUniverse& univ)
{
#ifndef __CINT__

    bool sideband = univ.Sideband_PlasDown();
    return sideband;
    
#endif  // __CINT__
}


#endif  // Cuts_cxx