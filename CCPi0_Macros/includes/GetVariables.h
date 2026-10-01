#ifndef GetVariables_h
#define GetVariables_h

#include <vector>

#ifndef __CINT__
#include "../includes/Variable.h"
#include "../includes/Variable2D.h"
#endif  // __CINT__



// Forward declare Variable classes
class Variable;
class Variable2D;



// ==========================================================================
//  CROSS-SECTION VARIABLES
// ==========================================================================

std::vector<Variable*> GetXsecVariables(bool include_truth_vars = false)
{
    // Reco variables
    Variable* MuonPt = new Variable("MuonPt",
                                    "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // True variables
    bool is_true = true;
    Variable* MuonPt_True = new Variable("MuonPt_True",
                                         "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    // Vector of variables
    std::vector<Variable*> variables = {MuonPt};
    if ( include_truth_vars ) variables.push_back(MuonPt_True);
    return variables;
}





// ==========================================================================
//  EVENT COUNT VARIABLES
// ==========================================================================

// Event count in Truth
// ====================
std::vector<Variable*> GetEventCountVariablesTruth()
{
    // Event count
    Variable* EventCount_True = new Variable("EventCount_True",
                                             "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar, true);
    
    std::vector<Variable*> variables = {EventCount_True};
    return variables;
}


// Event count in MC Reco and Data
// ===============================
std::vector<Variable*> GetEventCountVariables()
{
    // Event count
    Variable* EventCount = new Variable("EventCount",
                                        "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* EventCount_Single = new Variable("EventCount_Single",
                                               "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* EventCount_Multi = new Variable("EventCount_Multi",
                                              "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    // Blob PDG
    Variable* BlobPdg_Single = new Variable("BlobPdg_Single",
                                            "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* BlobPdg_Multi = new Variable("BlobPdg_Multi",
                                           "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    std::vector<Variable*> variables = {EventCount, EventCount_Single, EventCount_Multi,
                                        BlobPdg_Single, BlobPdg_Multi};
    return variables;
}


// Rejected events
// ===============
std::vector<Variable*> GetRejectedEventVariables()
{
    // Event count
    Variable* RejectedEvent = new Variable("RejectedEvent",
                                           "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* RejectedEvent_Single = new Variable("RejectedEvent_Single",
                                                  "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* RejectedEvent_Multi = new Variable("RejectedEvent_Multi",
                                                 "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    // Blob PDG
    Variable* RejectedBlobPdg_Single = new Variable("RejectedBlobPdg_Single",
                                                    "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    Variable* RejectedBlobPdg_Multi = new Variable("RejectedBlobPdg_Multi",
                                                   "Cut number", "", 17, 0., 17., &CVUniverse::GetDummyVar);
    
    std::vector<Variable*> variables = {RejectedEvent, RejectedEvent_Single, RejectedEvent_Multi,
                                        RejectedBlobPdg_Single, RejectedBlobPdg_Multi};
    return variables;
}





// ==========================================================================
//  MICHEL VARIABLES
// ==========================================================================

std::vector<Variable*> GetMichelVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeMichel = new Variable("MuonPt_BeforeMichel",
                                                 "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Start point vertices
    Variable* NstartPointVertexMichels = new Variable("NstartPointVertexMichels",
                                                      "Number of Michel candidates", "", 4, 0., 4., &CVUniverse::NstartPointVertexMichels);
    
    Variable* StartPointVertexMichelPdg = new Variable("StartPointVertexMichelPdg",
                                                       "Start point vertex Michel PDG", "", 9, 1., 10., &CVUniverse::StartPointVertexMichelTruePDG);
    
    // Stop point vertices
    Variable* NstopPointVertexMichels = new Variable("NstopPointVertexMichels",
                                                     "Number of Michel candidates", "", 4, 0., 4., &CVUniverse::NstopPointVertexMichels);
    
    Variable* StopPointVertexMichelPdg = new Variable("StopPointVertexMichelPdg",
                                                      "Stop point vertex Michel PDG", "", 9, 1., 10., &CVUniverse::StopPointVertexMichelTruePDG);
    
    // Kinked vertices
    Variable* NkinkedVertexMichels = new Variable("NkinkedVertexMichels",
                                                  "Number of Michel candidates", "", 4, 0., 4., &CVUniverse::NkinkedVertexMichels);
    
    Variable* KinkedVertexMichelPdg = new Variable("KinkedVertexMichelPdg",
                                                   "Kinked vertex Michel PDG", "", 9, 1., 10., &CVUniverse::KinkedVertexMichelTruePDG);
    
    std::vector<Variable*> variables = {MuonPt_BeforeMichel,
                                        NstartPointVertexMichels, StartPointVertexMichelPdg,
                                        NstopPointVertexMichels,  StopPointVertexMichelPdg,
                                        NkinkedVertexMichels,     KinkedVertexMichelPdg};
    return variables;
}





// ==========================================================================
//  TRACK VARIABLES
// ==========================================================================

std::vector<Variable*> GetTrackVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeTrack = new Variable("MuonPt_BeforeTrack",
                                                "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Primary track
    Variable* NprimTracks = new Variable("NprimTracks",
                                         "Number of primary tracks", "", 4, 0., 4., &CVUniverse::NprimTracks);
    
    Variable* PrimTrackPionScore = new Variable("PrimTrackPionScore",
                                                "Pion LLR score", "", CCPi0::GetBinning("TrackPionScore"), &CVUniverse::PrimTrackPionScore);
    
    // Secondary track
    Variable* NsecTracks = new Variable("NsecTracks",
                                        "Number of secondary tracks", "", 4, 0., 4., &CVUniverse::NsecTracks);
    
    // Variable* SecTrackPionScore = new Variable("SecTrackPionScore",
    //                                            "Pion log-likelihood ratio score", "", CCPi0::GetBinning("TrackPionScore"), &CVUniverse::SecTrackPionScore);
    
    std::vector<Variable*> variables = {MuonPt_BeforeTrack,
                                        NprimTracks, PrimTrackPionScore,
                                        NsecTracks/*,  SecTrackPionScore*/};
    return variables;
}





// ==========================================================================
//  ANGLESCAN VARIABLES
// ==========================================================================

std::vector<Variable*> GetAngleScanVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeAngleScan = new Variable("MuonPt_BeforeAngleScan",
                                                    "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // AngleScan
    Variable* AngleScanNblobs = new Variable("AngleScanNblobs",
                                             "Number of shower candidates", "", 6, 0., 6., &CVUniverse::AngleScanNblobs);
    
    Variable* NblobsPassBasicQuality = new Variable("NblobsPassBasicQuality",
                                                    "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobsPassBasicQuality);
    
    Variable* NblobCandidates = new Variable("NblobCandidates",
                                             "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    std::vector<Variable*> variables = {MuonPt_BeforeAngleScan,
                                        AngleScanNblobs,
                                        NblobsPassBasicQuality,
                                        NblobCandidates};
    return variables;
}





// ==========================================================================
//  BLOB ANGLE W.R.T. MUON VARIABLES
// ==========================================================================

std::vector<Variable*> GetBlobAngleWRTMuonVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeBlobAngleWRTMuon = new Variable("MuonPt_BeforeBlobAngleWRTMuon",
                                                           "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_BeforeBlobAngleWRTMuon = new Variable("Nblobs_BeforeBlobAngleWRTMuon",
                                                           "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobAngleWRTMuon_Single = new Variable("BlobAngleWRTMuon_Single",
                                                     "Shower angle w.r.t. muon #xi", "deg", CCPi0::GetBinning("BlobAngleWRTMuon"), &CVUniverse::GetBlobAxisAngleWRTMuon);
    
    // Multi-blob
    Variable* BlobAngleWRTMuon_Multi = new Variable("BlobAngleWRTMuon_Multi",
                                                    "Shower angle w.r.t. muon #xi", "deg", CCPi0::GetBinning("BlobAngleWRTMuon"), &CVUniverse::GetBlobAxisAngleWRTMuon);
    
    std::vector<Variable*> variables = {MuonPt_BeforeBlobAngleWRTMuon,
                                        Nblobs_BeforeBlobAngleWRTMuon,
                                        BlobAngleWRTMuon_Single,
                                        BlobAngleWRTMuon_Multi};
    return variables;
}





// ==========================================================================
//  BLOB DEVIATION VARIABLES
// ==========================================================================

// 1D variables
// ============
std::vector<Variable*> GetBlobDeviationVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeBlobDeviation = new Variable("MuonPt_BeforeBlobDeviation",
                                                        "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_BeforeBlobDeviation = new Variable("Nblobs_BeforeBlobDeviation",
                                                        "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobProjDeviation_Single = new Variable("BlobProjDeviation_Single",
                                                      "Shower projected deviation #font[12]{D}", "mm", CCPi0::GetBinning("BlobProjDeviation"), &CVUniverse::GetBlobProjDeviation);
    
    Variable* BlobAngleDeviation_Single = new Variable("BlobAngleDeviation_Single",
                                                       "Shower angle deviation #alpha", "deg", CCPi0::GetBinning("BlobAngleDeviation"), &CVUniverse::GetBlobAngleDeviation);
    
    // Multi-blob
    Variable* BlobProjDeviation_Multi = new Variable("BlobProjDeviation_Multi",
                                                     "Shower projected deviation #font[12]{D}", "mm", CCPi0::GetBinning("BlobProjDeviation"), &CVUniverse::GetBlobProjDeviation);
    
    Variable* BlobAngleDeviation_Multi = new Variable("BlobAngleDeviation_Multi",
                                                      "Shower angle deviation #alpha", "deg", CCPi0::GetBinning("BlobAngleDeviation"), &CVUniverse::GetBlobAngleDeviation);
    
    std::vector<Variable*> variables = {MuonPt_BeforeBlobDeviation,
                                        Nblobs_BeforeBlobDeviation,
                                        BlobProjDeviation_Single, BlobAngleDeviation_Single,
                                        BlobProjDeviation_Multi,  BlobAngleDeviation_Multi};
    return variables;
}


// 2D variables
// ============
std::vector<Variable2D*> GetBlobDeviationVariables2D()
{
    // Single-blob
    Variable2D* BlobAngleDeviationVSDeviation_Single = new Variable2D("BlobAngleDeviationVSDeviation_Single",
                                                                      "Shower projected deviation #font[12]{D}", "mm", CCPi0::GetBinning("BlobProjDeviation"), &CVUniverse::GetBlobProjDeviation,
                                                                      "Shower angle deviation #alpha", "deg", CCPi0::GetBinning("BlobAngleDeviation"), &CVUniverse::GetBlobAngleDeviation);
    
    // Multi-blob leading
    Variable2D* BlobAngleDeviationVSDeviation_Multi = new Variable2D("BlobAngleDeviationVSDeviation_Multi",
                                                                     "Shower projected deviation #font[12]{D}", "mm", CCPi0::GetBinning("BlobProjDeviation"), &CVUniverse::GetBlobProjDeviation,
                                                                     "Shower angle deviation #alpha", "deg", CCPi0::GetBinning("BlobAngleDeviation"), &CVUniverse::GetBlobAngleDeviation);
    
    std::vector<Variable2D*> variables2D = {BlobAngleDeviationVSDeviation_Single,
                                            BlobAngleDeviationVSDeviation_Multi};
    return variables2D;
}





// ==========================================================================
//  BLOB ENERGY VS dx VARIABLES
// ==========================================================================

// 1D variables
// ============
std::vector<Variable*> GetBlobEnergyVSdxVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeBlobEnergyVSdx = new Variable("MuonPt_BeforeBlobEnergyVSdx",
                                                         "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_BeforeBlobEnergyVSdx = new Variable("Nblobs_BeforeBlobEnergyVSdx",
                                                         "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobEcalo_Single_Part1 = new Variable("BlobEcalo_Single_Part1",
                                                    "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Single_Part1 = new Variable("Blobdx_Single_Part1",
                                                 "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Single_Part1 = new Variable("BlobdEdxMean_Single_Part1",
                                                       "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Single_Part1 = new Variable("BlobdEdxFront_Single_Part1",
                                                        "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Single_Part1 = new Variable("BlobdEdxEnd_Single_Part1",
                                                      "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable* BlobEcalo_Multi_Part1 = new Variable("BlobEcalo_Multi_Part1",
                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Multi_Part1 = new Variable("Blobdx_Multi_Part1",
                                                "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Multi_Part1 = new Variable("BlobdEdxMean_Multi_Part1",
                                                      "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Multi_Part1 = new Variable("BlobdEdxFront_Multi_Part1",
                                                       "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Multi_Part1 = new Variable("BlobdEdxEnd_Multi_Part1",
                                                     "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable*> variables = {MuonPt_BeforeBlobEnergyVSdx,
                                        Nblobs_BeforeBlobEnergyVSdx,
                                        BlobEcalo_Single_Part1, Blobdx_Single_Part1, BlobdEdxMean_Single_Part1, BlobdEdxFront_Single_Part1, BlobdEdxEnd_Single_Part1,
                                        BlobEcalo_Multi_Part1,  Blobdx_Multi_Part1,  BlobdEdxMean_Multi_Part1,  BlobdEdxFront_Multi_Part1,  BlobdEdxEnd_Multi_Part1};
    return variables;
}


// 2D variables
// ============
std::vector<Variable2D*> GetBlobEnergyVSdxVariables2D()
{
    // Single-blob
    Variable2D* BlobEcaloVSdx_Single_Part1 = new Variable2D("BlobEcaloVSdx_Single_Part1",
                                                            "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                            "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Single_Part1 = new Variable2D("BlobdEdxMeanVSEcalo_Single_Part1",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Single_Part1 = new Variable2D("BlobdEdxFrontVSEcalo_Single_Part1",
                                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                   "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Single_Part1 = new Variable2D("BlobdEdxEndVSEcalo_Single_Part1",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable2D* BlobEcaloVSdx_Multi_Part1 = new Variable2D("BlobEcaloVSdx_Multi_Part1",
                                                           "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                           "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Multi_Part1 = new Variable2D("BlobdEdxMeanVSEcalo_Multi_Part1",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Multi_Part1 = new Variable2D("BlobdEdxFrontVSEcalo_Multi_Part1",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Multi_Part1 = new Variable2D("BlobdEdxEndVSEcalo_Multi_Part1",
                                                                "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable2D*> variables2D = {BlobEcaloVSdx_Single_Part1, BlobdEdxMeanVSEcalo_Single_Part1, BlobdEdxFrontVSEcalo_Single_Part1, BlobdEdxEndVSEcalo_Single_Part1,
                                            BlobEcaloVSdx_Multi_Part1,  BlobdEdxMeanVSEcalo_Multi_Part1,  BlobdEdxFrontVSEcalo_Multi_Part1,  BlobdEdxEndVSEcalo_Multi_Part1};
    return variables2D;
}





// ==========================================================================
//  BLOB END dE/dx VARIABLES
// ==========================================================================

// 1D variables
// ============
std::vector<Variable*> GetBlobdEdxEndVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeBlobdEdxEnd = new Variable("MuonPt_BeforeBlobdEdxEnd",
                                                      "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_BeforeBlobdEdxEnd = new Variable("Nblobs_BeforeBlobdEdxEnd",
                                                      "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobEcalo_Single_Part2 = new Variable("BlobEcalo_Single_Part2",
                                                    "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Single_Part2 = new Variable("Blobdx_Single_Part2",
                                                 "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Single_Part2 = new Variable("BlobdEdxMean_Single_Part2",
                                                       "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Single_Part2 = new Variable("BlobdEdxFront_Single_Part2",
                                                        "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Single_Part2 = new Variable("BlobdEdxEnd_Single_Part2",
                                                      "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable* BlobEcalo_Multi_Part2 = new Variable("BlobEcalo_Multi_Part2",
                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Multi_Part2 = new Variable("Blobdx_Multi_Part2",
                                                "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Multi_Part2 = new Variable("BlobdEdxMean_Multi_Part2",
                                                      "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Multi_Part2 = new Variable("BlobdEdxFront_Multi_Part2",
                                                       "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Multi_Part2 = new Variable("BlobdEdxEnd_Multi_Part2",
                                                     "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable*> variables = {MuonPt_BeforeBlobdEdxEnd,
                                        Nblobs_BeforeBlobdEdxEnd,
                                        BlobEcalo_Single_Part2, Blobdx_Single_Part2, BlobdEdxMean_Single_Part2, BlobdEdxFront_Single_Part2, BlobdEdxEnd_Single_Part2,
                                        BlobEcalo_Multi_Part2,  Blobdx_Multi_Part2,  BlobdEdxMean_Multi_Part2,  BlobdEdxFront_Multi_Part2,  BlobdEdxEnd_Multi_Part2};
    return variables;
}


// 2D variables
// ============
std::vector<Variable2D*> GetBlobdEdxEndVariables2D()
{
    // Single-blob
    Variable2D* BlobEcaloVSdx_Single_Part2 = new Variable2D("BlobEcaloVSdx_Single_Part2",
                                                            "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                            "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Single_Part2 = new Variable2D("BlobdEdxMeanVSEcalo_Single_Part2",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Single_Part2 = new Variable2D("BlobdEdxFrontVSEcalo_Single_Part2",
                                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Single_Part2 = new Variable2D("BlobdEdxEndVSEcalo_Single_Part2",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable2D* BlobEcaloVSdx_Multi_Part2 = new Variable2D("BlobEcaloVSdx_Multi_Part2",
                                                           "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                           "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Multi_Part2 = new Variable2D("BlobdEdxMeanVSEcalo_Multi_Part2",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Multi_Part2 = new Variable2D("BlobdEdxFrontVSEcalo_Multi_Part2",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Multi_Part2 = new Variable2D("BlobdEdxEndVSEcalo_Multi_Part2",
                                                                "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable2D*> variables2D = {BlobEcaloVSdx_Single_Part2, BlobdEdxMeanVSEcalo_Single_Part2, BlobdEdxFrontVSEcalo_Single_Part2, BlobdEdxEndVSEcalo_Single_Part2,
                                            BlobEcaloVSdx_Multi_Part2,  BlobdEdxMeanVSEcalo_Multi_Part2,  BlobdEdxFrontVSEcalo_Multi_Part2,  BlobdEdxEndVSEcalo_Multi_Part2};
    return variables2D;
}





// ==========================================================================
//  AFTER BLOB ENERGY VARIABLES
// ==========================================================================

// 1D variables
// ============
std::vector<Variable*> GetAfterBlobEnergyVariables()
{
    // Muon Pt
    Variable* MuonPt_AfterBlobEnergy = new Variable("MuonPt_AfterBlobEnergy",
                                                    "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_AfterBlobEnergy = new Variable("Nblobs_AfterBlobEnergy",
                                                    "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobEcalo_Single_Part3 = new Variable("BlobEcalo_Single_Part3",
                                                    "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Single_Part3 = new Variable("Blobdx_Single_Part3",
                                                 "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Single_Part3 = new Variable("BlobdEdxMean_Single_Part3",
                                                       "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Single_Part3 = new Variable("BlobdEdxFront_Single_Part3",
                                                        "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Single_Part3 = new Variable("BlobdEdxEnd_Single_Part3",
                                                      "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable* BlobEcalo_Multi_Part3 = new Variable("BlobEcalo_Multi_Part3",
                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable* Blobdx_Multi_Part3 = new Variable("Blobdx_Multi_Part3",
                                                "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx);
    
    Variable* BlobdEdxMean_Multi_Part3 = new Variable("BlobdEdxMean_Multi_Part3",
                                                      "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable* BlobdEdxFront_Multi_Part3 = new Variable("BlobdEdxFront_Multi_Part3",
                                                       "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable* BlobdEdxEnd_Multi_Part3 = new Variable("BlobdEdxEnd_Multi_Part3",
                                                     "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable*> variables = {MuonPt_AfterBlobEnergy,
                                        Nblobs_AfterBlobEnergy,
                                        BlobEcalo_Single_Part3, Blobdx_Single_Part3, BlobdEdxMean_Single_Part3, BlobdEdxFront_Single_Part3, BlobdEdxEnd_Single_Part3,
                                        BlobEcalo_Multi_Part3,  Blobdx_Multi_Part3,  BlobdEdxMean_Multi_Part3,  BlobdEdxFront_Multi_Part3,  BlobdEdxEnd_Multi_Part3};
    return variables;
}


// 2D variables
// ============
std::vector<Variable2D*> GetAfterBlobEnergyVariables2D()
{
    // Single-blob
    Variable2D* BlobEcaloVSdx_Single_Part3 = new Variable2D("BlobEcaloVSdx_Single_Part3",
                                                            "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                            "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Single_Part3 = new Variable2D("BlobdEdxMeanVSEcalo_Single_Part3",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Single_Part3 = new Variable2D("BlobdEdxFrontVSEcalo_Single_Part3",
                                                                   "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Single_Part3 = new Variable2D("BlobdEdxEndVSEcalo_Single_Part3",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    // Multi-blob
    Variable2D* BlobEcaloVSdx_Multi_Part3 = new Variable2D("BlobEcaloVSdx_Multi_Part3",
                                                           "Shower length", "cm", CCPi0::GetBinning("Blobdx"), &CVUniverse::GetBlobdx,
                                                           "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo);
    
    Variable2D* BlobdEdxMeanVSEcalo_Multi_Part3 = new Variable2D("BlobdEdxMeanVSEcalo_Multi_Part3",
                                                                 "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                 "Shower mean #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxMean"), &CVUniverse::GetBlobdEdxMean);
    
    Variable2D* BlobdEdxFrontVSEcalo_Multi_Part3 = new Variable2D("BlobdEdxFrontVSEcalo_Multi_Part3",
                                                                  "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                  "Shower front #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxFront"), &CVUniverse::GetBlobdEdxFront);
    
    Variable2D* BlobdEdxEndVSEcalo_Multi_Part3 = new Variable2D("BlobdEdxEndVSEcalo_Multi_Part3",
                                                                "Shower calorimetric energy", "MeV", CCPi0::GetBinning("BlobEcalo"), &CVUniverse::GetBlobEcalo,
                                                                "Shower end #font[12]{dE/dx}", "MeV/cm", CCPi0::GetBinning("BlobdEdxEnd"), &CVUniverse::GetBlobdEdxEnd);
    
    std::vector<Variable2D*> variables2D = {BlobEcaloVSdx_Single_Part3, BlobdEdxMeanVSEcalo_Single_Part3, BlobdEdxFrontVSEcalo_Single_Part3, BlobdEdxEndVSEcalo_Single_Part3,
                                            BlobEcaloVSdx_Multi_Part3,  BlobdEdxMeanVSEcalo_Multi_Part3,  BlobdEdxFrontVSEcalo_Multi_Part3,  BlobdEdxEndVSEcalo_Multi_Part3};
    return variables2D;
}





// ==========================================================================
//  BLOB MICHEL VARIABLES
// ==========================================================================

std::vector<Variable*> GetBlobMichelVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeBlobMichel = new Variable("MuonPt_BeforeBlobMichel",
                                                     "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Nblobs
    Variable* Nblobs_BeforeBlobMichel = new Variable("Nblobs_BeforeBlobMichel",
                                                     "Number of shower candidates", "", 6, 0., 6., &CVUniverse::NblobCandidates);
    
    // Single-blob
    Variable* BlobStartPointMichel_Single = new Variable("BlobStartPointMichel_Single",
                                                         "Number of Michels around shower start point", "", 2, 0., 2., &CVUniverse::GetBlobStartPointMichelMatch);
    
    Variable* BlobEndPointMichel_Single = new Variable("BlobEndPointMichel_Single",
                                                       "Number of Michels around shower end point", "", 2, 0., 2., &CVUniverse::GetBlobEndPointMichelMatch);
    
    Variable* BlobPdg_Single = new Variable("BlobPdg_Single",
                                            "Shower true PDG", "", 9, 1., 10., &CVUniverse::GetBlobTruePDG);
    
    // Multi-blob
    Variable* BlobStartPointMichel_Multi = new Variable("BlobStartPointMichel_Multi",
                                                          "Number of Michels around shower start point", "", 2, 0., 2., &CVUniverse::GetBlobStartPointMichelMatch);
    
    Variable* BlobEndPointMichel_Multi = new Variable("BlobEndPointMichel_Multi",
                                                        "Number of Michels around shower end point", "", 2, 0., 2., &CVUniverse::GetBlobEndPointMichelMatch);
    
    Variable* BlobPdg_Multi = new Variable("BlobPdg_Multi",
                                             "Shower true PDG", "", 9, 1., 10., &CVUniverse::GetBlobTruePDG);
    
    std::vector<Variable*> variables = {MuonPt_BeforeBlobMichel,
                                        Nblobs_BeforeBlobMichel,
                                        BlobStartPointMichel_Single, BlobEndPointMichel_Single, BlobPdg_Single,
                                        BlobStartPointMichel_Multi,  BlobEndPointMichel_Multi,  BlobPdg_Multi};
    return variables;
}





// ==========================================================================
//  ENERGY VARIABLES
// ==========================================================================

std::vector<Variable*> GetEnergyVariables()
{
    // Muon Pt
    Variable* MuonPt_BeforeEnergy = new Variable("MuonPt_BeforeEnergy",
                                                 "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Single-blob
    Variable* NoPi0RecoilE_Single = new Variable("NoPi0RecoilE_Single",
                                                 "No-#pi^{0} recoil energy", "GeV", CCPi0::GetBinning("NoPi0RecoilE"), &CVUniverse::GetNoPi0RecoilE);
    
    Variable* RecoilE_Single = new Variable("RecoilE_Single",
                                            "Recoil energy", "GeV", CCPi0::GetBinning("RecoilE"), &CVUniverse::GetRecoilE);
    
    Variable* Q2_Single = new Variable("Q2_Single",
                                       "Transferred four-momentum #font[12]{Q^{2}}", "GeV^{2}/c^{2}", CCPi0::GetBinning("Q2"), &CVUniverse::GetQ2);
    
    Variable* W2_Single = new Variable("W2_Single",
                                       "Hadronic mass squared #font[12]{W^{2}}", "GeV^{2}/c^{4}", CCPi0::GetBinning("W2"), &CVUniverse::GetW2);
    
    Variable* W_Single = new Variable("W_Single",
                                      "Hadronic mass #font[12]{W}", "GeV/c^{2}", CCPi0::GetBinning("W"), &CVUniverse::GetW);
    
    // Multi-blob
    Variable* NoPi0RecoilE_Multi = new Variable("NoPi0RecoilE_Multi",
                                                "No-#pi^{0} recoil energy", "GeV", CCPi0::GetBinning("NoPi0RecoilE"), &CVUniverse::GetNoPi0RecoilE);
    
    Variable* RecoilE_Multi = new Variable("RecoilE_Multi",
                                           "Recoil energy", "GeV", CCPi0::GetBinning("RecoilE"), &CVUniverse::GetRecoilE);
    
    Variable* Q2_Multi = new Variable("Q2_Multi",
                                      "Transferred four-momentum #font[12]{Q^{2}}", "GeV^{2}/c^{2}", CCPi0::GetBinning("Q2"), &CVUniverse::GetQ2);
    
    Variable* W2_Multi = new Variable("W2_Multi",
                                      "Hadronic mass squared #font[12]{W^{2}}", "GeV^{2}/c^{4}", CCPi0::GetBinning("W2"), &CVUniverse::GetW2);
    
    Variable* W_Multi = new Variable("W_Multi",
                                     "Hadronic mass #font[12]{W}", "GeV/c^{2}", CCPi0::GetBinning("W"), &CVUniverse::GetW);
    
    std::vector<Variable*> variables = {MuonPt_BeforeEnergy,
                                        NoPi0RecoilE_Single, RecoilE_Single, Q2_Single, W2_Single, W_Single,
                                        NoPi0RecoilE_Multi,  RecoilE_Multi,  Q2_Multi,  W2_Multi,  W_Multi};
    return variables;
}





// ==========================================================================
//  SIDEBAND STUDY VARIABLES
// ==========================================================================

std::vector<Variable*> GetSidebandStudyVariables()
{
    // Configuration #1
    Variable* SigReg_Conf1 = new Variable("SigReg_Conf1",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* PionSB_Conf1 = new Variable("PionSB_Conf1",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* ProtonSB_Conf1 = new Variable("ProtonSB_Conf1",
                                            "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* HighWSB_Conf1 = new Variable("HighWSB_Conf1",
                                           "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Configuration #2
    Variable* SigReg_Conf2 = new Variable("SigReg_Conf2",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* PionSB_Conf2 = new Variable("PionSB_Conf2",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* ProtonSB_Conf2 = new Variable("ProtonSB_Conf2",
                                            "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* HighWSB_Conf2 = new Variable("HighWSB_Conf2",
                                           "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Configuration #3
    Variable* SigReg_Conf3 = new Variable("SigReg_Conf3",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* PionSB_Conf3 = new Variable("PionSB_Conf3",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* ProtonSB_Conf3 = new Variable("ProtonSB_Conf3",
                                            "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* HighWSB_Conf3 = new Variable("HighWSB_Conf3",
                                           "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Configuration #4
    Variable* SigReg_Conf4 = new Variable("SigReg_Conf4",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* PionSB_Conf4 = new Variable("PionSB_Conf4",
                                          "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* ProtonSB_Conf4 = new Variable("ProtonSB_Conf4",
                                            "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    Variable* HighWSB_Conf4 = new Variable("HighWSB_Conf4",
                                           "Reconstructed muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt);
    
    // Vector of variables
    std::vector<Variable*> variables = {SigReg_Conf1, PionSB_Conf1, ProtonSB_Conf1, HighWSB_Conf1,
                                        SigReg_Conf2, PionSB_Conf2, ProtonSB_Conf2, HighWSB_Conf2,
                                        SigReg_Conf3, PionSB_Conf3, ProtonSB_Conf3, HighWSB_Conf3,
                                        SigReg_Conf4, PionSB_Conf4, ProtonSB_Conf4, HighWSB_Conf4};
    return variables;
}





// ==========================================================================
//  EFFICIENCY STUDY VARIABLES
// ==========================================================================

std::vector<Variable*> GetEffStudyVariables(bool include_truth_vars = true)
{
    bool is_true = true;
    
    Variable* Eff_ThetaMu10 = new Variable("Eff_ThetaMu10",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu11 = new Variable("Eff_ThetaMu11",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu12 = new Variable("Eff_ThetaMu12",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu13 = new Variable("Eff_ThetaMu13",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu14 = new Variable("Eff_ThetaMu14",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu15 = new Variable("Eff_ThetaMu15",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu16 = new Variable("Eff_ThetaMu16",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu17 = new Variable("Eff_ThetaMu17",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu18 = new Variable("Eff_ThetaMu18",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu19 = new Variable("Eff_ThetaMu19",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu20 = new Variable("Eff_ThetaMu20",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu21 = new Variable("Eff_ThetaMu21",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu22 = new Variable("Eff_ThetaMu22",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu23 = new Variable("Eff_ThetaMu23",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu24 = new Variable("Eff_ThetaMu24",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    Variable* Eff_ThetaMu25 = new Variable("Eff_ThetaMu25",
                                           "True muon #font[12]{p}_{#font[132]{T}}", "GeV/c", CCPi0::GetBinning("MuonPt"), &CVUniverse::GetMuonPt_True, is_true);
    
    std::vector<Variable*> variables = {Eff_ThetaMu10, Eff_ThetaMu11, Eff_ThetaMu12, Eff_ThetaMu13, Eff_ThetaMu14, Eff_ThetaMu15,
                                        Eff_ThetaMu16, Eff_ThetaMu17, Eff_ThetaMu18, Eff_ThetaMu19, Eff_ThetaMu20, Eff_ThetaMu21,
                                        Eff_ThetaMu22, Eff_ThetaMu23, Eff_ThetaMu24, Eff_ThetaMu25};
    return variables;
}





// ==========================================================================
//  Alternative efficiency variables
// ==========================================================================

std::vector<Variable*> GetAlternativeEffVariables(bool include_truth_vars = true)
{
    bool is_true = true;
    
    Variable* MuonPz_True = new Variable("MuonPz_True",
                                         "True muon #font[12]{p}_{#font[132]{||}}", "GeV/c", CCPi0::GetBinning("MuonPz"), &CVUniverse::GetMuonPz_True, is_true);
    
    Variable* Pi0E_True = new Variable("Pi0E_True",
                                       "True #pi^{0} energy", "GeV", CCPi0::GetBinning("Pi0E"), &CVUniverse::GetPi0E_True, is_true);
    
    Variable* Pi0KE_True = new Variable("Pi0KE_True",
                                        "True #pi^{0} kinetic energy", "GeV", CCPi0::GetBinning("Pi0KE"), &CVUniverse::GetPi0KE_True, is_true);
    
    Variable* Pi0P_True = new Variable("Pi0P_True",
                                       "True #pi^{0} momentum", "GeV/c", CCPi0::GetBinning("Pi0P"), &CVUniverse::GetPi0P_True, is_true);
    
    Variable* Pi0Theta_True = new Variable("Pi0Theta_True",
                                           "True #pi^{0} polar angle", "deg", CCPi0::GetBinning("Pi0Theta"), &CVUniverse::GetPi0Theta_True, is_true);
    
    std::vector<Variable*> variables = {Pi0E_True, Pi0KE_True, Pi0P_True, Pi0Theta_True};
    return variables;
}





// ==========================================================================
//  Other supporting variables
// ==========================================================================

// 1D variables
// ============
std::vector<Variable*> GetSupportVariables()
{
    bool is_true = true;
    
    Variable* MuonVertexPlane = new Variable("MuonVertexPlane",
                                             "Muon vertex plane", "", 24, 31., 55., &CVUniverse::MuonVertexPlane);
    
    Variable* Gamma1TrueE = new Variable("Gamma1TrueE",
                                         "True photon energy", "GeV", 50, 0., 2.5, &CVUniverse::GetGamma1E_True);
    
    Variable* Gamma2TrueE = new Variable("Gamma2TrueE",
                                         "True photon energy", "GeV", 30, 0., 1.5, &CVUniverse::GetGamma2E_True);
    
    std::vector<Variable*> variables = {MuonVertexPlane,
                                        Gamma1TrueE, Gamma2TrueE};
    return variables;
}


// 2D variables
// ============
std::vector<Variable2D*> GetSupportVariables2D()
{
    Variable2D* Gamma1EnergyLoss = new Variable2D("Gamma1EnergyLoss",
                                                  "Energy deposited", "MeV", 100, 0., 2.5, &CVUniverse::GetGamma1Ehit,
                                                  "True photon energy", "MeV", 100, 0., 2.5, &CVUniverse::GetGamma1E_True);
    
    Variable2D* Gamma2EnergyLoss = new Variable2D("Gamma2EnergyLoss",
                                                  "Energy deposited", "MeV", 60, 0., 1.5, &CVUniverse::GetGamma2Ehit,
                                                  "True photon energy", "MeV", 60, 0., 1.5, &CVUniverse::GetGamma2E_True);
    
    std::vector<Variable2D*> variables = {Gamma1EnergyLoss, Gamma2EnergyLoss};
    return variables;
}





// // ==========================================================================
// //  Vertex study variables
// // ==========================================================================

// // 1D variables
// // ============

// std::vector<Variable*> GetVertexStudyVariables(bool include_truth_vars = false)
// {
//     // Reco variables
//     Variable* VertexX = new Variable("VertexX",
//                                      "Reco interaction vertex in X", "mm", 200, -1000., 1000., &CVUniverse::TargetVertexX);
    
//     Variable* VertexY = new Variable("VertexY",
//                                      "Reco interaction vertex in Y", "mm", 200, -1000., 1000., &CVUniverse::TargetVertexY);
    
//     Variable* VertexZ = new Variable("VertexZ",
//                                      "Reco interaction vertex in Z", "mm", 220, 5600., 5820., &CVUniverse::TargetVertexZ);
    
//     // True variables
//     bool is_true = true;
    
//     Variable* VertexX_True = new Variable("VertexX_True",
//                                           "True interaction vertex in X", "mm", 200, -1000., 1000., &CVUniverse::MC_VtxX, is_true);
    
//     Variable* VertexY_True = new Variable("VertexY_True",
//                                           "True interaction vertex in Y", "mm", 200, -1000., 1000., &CVUniverse::MC_VtxZ, is_true);
    
//     Variable* VertexZ_True = new Variable("VertexZ_True",
//                                           "True interaction vertex in Z", "mm", 220, 5600., 5820., &CVUniverse::MC_VtxZ, is_true);
    
//     // Vector of variables
//     std::vector<Variable*> variables = {VertexX,
//                                         VertexY,
//                                         VertexZ};
//     if ( include_truth_vars ) {
//         variables.push_back(VertexX_True);
//         variables.push_back(VertexY_True);
//         variables.push_back(VertexZ_True);
//     }
    
//     return variables;
// }


// // 2D variables
// // ============

// std::vector<Variable2D*> GetVertexStudyVariables2D(bool include_truth_vars = false)
// {
//     // Reco variables
//     Variable2D* VertexXY = new Variable2D("VertexXY",
//                                           "Reco interaction vertex in X", "mm", 200, -1000., 1000., &CVUniverse::TargetVertexX,
//                                           "Reco interaction vertex in Y", "mm", 200, -1000., 1000., &CVUniverse::TargetVertexY);
    
//     // True variables
//     bool is_true = true;
//     Variable2D* VertexXY_True = new Variable2D("VertexXY_True",
//                                                "True interaction vertex in X", "mm", 200, -1000., 1000., &CVUniverse::MC_VtxX,
//                                                "True interaction vertex in Y", "mm", 200, -1000., 1000., &CVUniverse::MC_VtxY, is_true);
    
//     // Vector of variables
//     std::vector<Variable2D*> variables2D = {VertexXY};
//     if ( include_truth_vars ) variables2D.push_back(VertexXY_True);
    
//     return variables2D;
// }

#endif  // GetVariables_h