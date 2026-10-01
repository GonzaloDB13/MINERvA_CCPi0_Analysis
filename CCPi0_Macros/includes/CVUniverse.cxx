#ifndef CVUniverse_cxx
#define CVUniverse_cxx

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "CVUniverse.h"

#include "TVector3.h"



// ======================================================================================================================================
//  CONSTRUCTOR
// ======================================================================================================================================

CVUniverse::CVUniverse(PlotUtils::ChainWrapper* chw,
                       double nsigma)
    : PlotUtils::MinervaUniverse(chw, nsigma) {}





// ======================================================================================================================================
//  TRUE INTERACTION INFORMATION
// ======================================================================================================================================

// GENIE basic info
// ================
/* Adapted from the schematics used in CCQENuCuts.cxx. */

// Incoming particle
int CVUniverse::MC_Incoming() const {
    return GetInt("mc_incoming");
}


// Interaction current
int CVUniverse::MC_Current() const {
    return GetInt("mc_current");
}


// Number of FS particles
int CVUniverse::MC_NFSPart() const {
    return GetInt("mc_nFSPart");
}


// PDG of FS particles
std::vector<int> CVUniverse::MC_FSPartPDG() const {
    return GetVecInt("mc_FSPartPDG");
}


// Energy of FS particles
std::vector<double> CVUniverse::MC_FSPartE() const {
    return GetVecDouble("mc_FSPartE");
}


// Momentum of FS particles
std::vector<double> CVUniverse::MC_FSPartPx() const {
    return GetVecDouble("mc_FSPartPx");
}

std::vector<double> CVUniverse::MC_FSPartPy() const {
    return GetVecDouble("mc_FSPartPy");
}

std::vector<double> CVUniverse::MC_FSPartPz() const {
    return GetVecDouble("mc_FSPartPz");
}


// Number of most relevant FS particles
void CVUniverse::MC_NFSPartRelevant(int& n_muon, int& n_pi0, int& n_pion, int& n_other_meson, int& n_heavy_baryon) const
{
    int n_fspart = MC_NFSPart();
    std::vector<int> fspart_pdg = MC_FSPartPDG();
    
    for ( int i = 0; i < n_fspart; ++i ) {
        int pdg = fspart_pdg[i];
        
        if ( pdg == 13 )                 ++n_muon;
        else if ( pdg == 111 )           ++n_pi0;
        else if ( std::abs(pdg) == 211 ) ++n_pion;
        
        else if ( std::abs(pdg) == 130 /* K0 long */ || std::abs(pdg) == 310 /* K0 short */ || std::abs(pdg) == 311 /* K0 */   ||
                  std::abs(pdg) == 321 /* K+ */      || std::abs(pdg) == 313 /* K*0 */      || std::abs(pdg) == 323 /* K*+ */  ||
                  std::abs(pdg) == 411 /* D+ */      || std::abs(pdg) == 421 /* D0 */ )
            ++n_other_meson;
        
        else if ( std::abs(pdg) == 3122 /* Lambda */     ||
                  std::abs(pdg) == 3222 /* Sigma+ */     || std::abs(pdg) == 3212 /* Sigma0 */    || std::abs(pdg) == 3112 /* Sigma- */ ||
                  std::abs(pdg) == 4122 /* Lambda+ ch */ ||
                  std::abs(pdg) == 4222 /* Sigma++ ch */ || std::abs(pdg) == 4212 /* Sigma+ ch */ || std::abs(pdg) == 4112 /* Sigma0 ch */ )
            ++n_heavy_baryon;
    }
}


// Interaction type info
// =====================
/* Adapted from the schematics used in CCQENuCuts.cxx. */

// Is true charged-current?
bool CVUniverse::IsTrueCC() const {
    int pdg     = MC_Incoming();
    int current = MC_Current();
    return (pdg == 14 && current == 1);
}


// Is true CC single-pi0?
bool CVUniverse::IsTrueCC_1Pi0() const {
    if ( !IsTrueCC() ) return false;
    
    int n_muon         = 0;
    int n_pi0          = 0;
    int n_pion         = 0;
    int n_other_meson  = 0;
    int n_heavy_baryon = 0;
    
    MC_NFSPartRelevant(n_muon, n_pi0, n_pion, n_other_meson, n_heavy_baryon);
    return (n_muon == 1 && n_pi0 == 1 && n_pion == 0 && n_other_meson == 0 && n_heavy_baryon == 0);
}


// Is true CC high-W pi0?
bool CVUniverse::IsTrueCC_Pi0HighW() const {
    if ( !IsTrueCC() ) return false;
    
    int n_muon         = 0;
    int n_pi0          = 0;
    int n_pion         = 0;
    int n_other_meson  = 0;
    int n_heavy_baryon = 0;
    
    MC_NFSPartRelevant(n_muon, n_pi0, n_pion, n_other_meson, n_heavy_baryon);
    bool case_1 = (n_pi0 == 1 && n_pion >= 1);  // Single-pi0 with single/multi-pion
    bool case_2 = (n_pi0 >  1 && n_pion >= 1);  // Multi-pi0 with single/multi-pion
    bool case_3 = (n_pi0 >  1 && n_pion == 0);  // Multi-pi0 with no pion
    
    return (n_muon == 1 && (case_1 || case_2 || case_3) && n_other_meson == 0 && n_heavy_baryon == 0);
}


// Is true CC QE-like?
bool CVUniverse::IsTrueCC_QElike() const {
    if ( !IsTrueCC() ) return false;
    
    int n_muon         = 0;
    int n_pi0          = 0;
    int n_pion         = 0;
    int n_other_meson  = 0;
    int n_heavy_baryon = 0;
    
    MC_NFSPartRelevant(n_muon, n_pi0, n_pion, n_other_meson, n_heavy_baryon);
    return (n_muon == 1 && n_pi0 == 0 && n_pion == 0 && n_other_meson == 0 && n_heavy_baryon == 0);
}


// Is true CC charged-pion production?
bool CVUniverse::IsTrueCC_PionProd() const {
    if ( !IsTrueCC() ) return false;
    
    int n_muon         = 0;
    int n_pi0          = 0;
    int n_pion         = 0;
    int n_other_meson  = 0;
    int n_heavy_baryon = 0;
    
    MC_NFSPartRelevant(n_muon, n_pi0, n_pion, n_other_meson, n_heavy_baryon);
    return (n_muon == 1 && n_pi0 == 0 && n_pion >= 1 && n_other_meson == 0 && n_heavy_baryon == 0);
}


// GENIE interaction type info
// ===========================
/* Adapted from the schematics used in CCQENuCuts.cxx. */

// GENIE interaction type
int CVUniverse::MC_IntType() const {
    return GetInt("mc_intType");
}


// GENIE charm type
int CVUniverse::MC_Charm() const {
    return GetInt("mc_charm");
}


// GENIE resonance type
int CVUniverse::MC_ResID() const {
    return GetInt("mc_resID");
}


// GENIE "true" DIS kinematics
bool CVUniverse::IsTrueDISKinematics() const {
    double W  = GetW_True();
    double Q2 = GetQ2_True();
    
    return (W > 2.0 && Q2 > 1.0);
}


// Is true GENIE charged-current QE?
bool CVUniverse::IsTrueGenieCC_QE() const {
    if ( !IsTrueCC() ) return false;
    
    int type  = MC_IntType();
    int charm = MC_Charm();
    return (type == 1 && charm == 0);
}


// Is true charged-current MEC? (Not an out-of-the-box GENIE category)
bool CVUniverse::IsTrueGenieCC_MEC() const {
    if ( !IsTrueCC() ) return false;
    
    int type  = MC_IntType();
    int charm = MC_Charm();
    return (type == 8 && charm == 0);
}


// Is true charged-current COH? (Not an out-of-the-box GENIE category)
bool CVUniverse::IsTrueGenieCC_COH() const {
    if ( !IsTrueCC() ) return false;
    
    int type = MC_IntType();
    return (type == 4);
}


// Is true GENIE charged-current delta RES?
bool CVUniverse::IsTrueGenieCC_DeltaRES() const {
    if ( !IsTrueCC() ) return false;
    
    int type  = MC_IntType();
    int resID = MC_ResID();
    return (type == 2 && resID == 0);
}


// Is true GENIE charged-current non-delta RES?
bool CVUniverse::IsTrueGenieCC_OtherRES() const {
    if ( !IsTrueCC() ) return false;
    
    int type  = MC_IntType();
    int resID = MC_ResID();
    return (type == 2 && resID != 0);
}


// Is true GENIE charged-current "soft" DIS?
bool CVUniverse::IsTrueGenieCC_SoftDIS() const {
    if ( !IsTrueCC() ) return false;
    
    int type = MC_IntType();
    bool trueDIS = IsTrueDISKinematics();
    return (type == 3 && !trueDIS);
}


// Is true GENIE charged-current "true" DIS?
bool CVUniverse::IsTrueGenieCC_TrueDIS() const {
    if ( !IsTrueCC() ) return false;
    
    int type = MC_IntType();
    bool trueDIS = IsTrueDISKinematics();
    return (type == 3 && trueDIS);
}





// ======================================================================================================================================
//  TRUE KINEMATICS
// ======================================================================================================================================

// Neutrino and muon kinematics
// ============================

// Neutrino energy [GeV]
double CVUniverse::GetNeutrinoE_True() const {
    return GetEnuTrue() / 1000.0;
}


// Muon energy [GeV]
double CVUniverse::GetMuonE_True() const {
    return GetElepTrue() / 1000.0;
}


// Muon momentum [GeV]
double CVUniverse::GetMuonP_True() const {
    return GetPlepTrue() / 1000.0;
}


// Muon transverse momentum [GeV]
double CVUniverse::GetMuonPt_True() const {
    return GetPlepTrue() * std::sin(GetThetalepTrue()) / 1000.0;
}


// Muon longitudinal momentum [GeV]
double CVUniverse::GetMuonPz_True() const {
    return GetPlepTrue() * std::cos(GetThetalepTrue()) / 1000.0;
}


// Muon polar angle w.r.t. beam [deg]
double CVUniverse::GetMuonTheta_True() const {
    return GetThetalepTrue() * CCPi0AnaConstants::RADTODEG;
}


// Transferred four-momentum Q2 [GeV^2]
double CVUniverse::GetQ2_True() const {
    double neutrino_E = GetNeutrinoE_True();                                // [GeV]
    double muon_mass  = CCPi0AnaConstants::MUON_MASS / 1000.0;              // [GeV]
    double muon_E     = GetMuonE_True();                                    // [GeV]
    double muon_P     = GetMuonP_True();                                    // [GeV]
    double muon_theta = GetMuonTheta_True() * CCPi0AnaConstants::DEGTORAD;  // [rad]
    
    double q2 = 2.0 * neutrino_E * (muon_E - (muon_P * std::cos(muon_theta))) - std::pow(muon_mass,2.0);
    return q2;
}


// Hadronic kinematics
// ===================

// Recoil energy [GeV]
double CVUniverse::GetRecoilE_True() const {
    return (GetNeutrinoE_True() - GetMuonE_True());
}


// Hadronic mass squared W2 [GeV^2/c^4]
double CVUniverse::GetW2_True() const {
    double nucl_mass = CCPi0AnaConstants::NUCLEON_MASS / 1000.0;  // [GeV]
    double Q2        = GetQ2_True();                              // [GeV]
    double E_recoil  = GetRecoilE_True();                         // [GeV]
    
    double W2 = std::pow(nucl_mass,2.0) + (2.0 * nucl_mass * E_recoil) - Q2;
    return W2;
}


// Hadronic mass W [GeV/c^2]
double CVUniverse::GetW_True() const {
    double W2 = GetW2_True();  // [GeV]
    
    if ( W2 > 0.0 ) return std::sqrt(W2);
    else return (-0.1);
}


// Available energy [GeV]
double CVUniverse::GetAvailableE_True() const {
    double E_avail = 0.0;
    
    int n_FSpart = MC_NFSPart();
    std::vector<int> pdg_vector       = MC_FSPartPDG();
    std::vector<double> energy_vector = MC_FSPartE();
    
    for ( int part = 0; part < n_FSpart; ++part ) {
        double energy = energy_vector[part] / 1000.0;  // [GeV]
        
        // Exclude muons and neutrons
        if ( std::abs(pdg_vector[part]) == 13 || pdg_vector[part] == 2112 ) continue;
        
        // Add kinetic energy of protons, charged pions, and charged kaons (DocDB 29921)
        else if ( pdg_vector[part] == 2212 )          E_avail += energy - (CCPi0AnaConstants::PROTON_MASS/1000.0);
        else if ( std::abs(pdg_vector[part]) == 211 ) E_avail += energy - (CCPi0AnaConstants::PION_MASS/1000.0);
        else if ( std::abs(pdg_vector[part]) == 321 ) E_avail += energy - (CCPi0AnaConstants::KAON_MASS/1000.0);
        
        // Add total energy of pi0 and photons
        else if ( pdg_vector[part] == 22 )  E_avail += energy;
        else if ( pdg_vector[part] == 111 ) E_avail += energy;
    }
    
    return E_avail;
}


// Hadronic mass squared W2 (with available energy) [GeV^2]
double CVUniverse::GetW2available_True() const {
    double nucl_mass = CCPi0AnaConstants::NUCLEON_MASS / 1000.0;  // [GeV]
    double Q2       = GetQ2_True();                               // [GeV]
    double E_avail  = GetAvailableE_True();                       // [GeV]
    
    double W2 = std::pow(nucl_mass,2.0) + (2.0 * nucl_mass * E_avail) - Q2;
    return W2;
}


// Hadronic mass W (with available energy) [GeV]
double CVUniverse::GetWavailable_True() const {
    double W2 = GetW2available_True();  // [GeV^2]
    
    if ( W2 > 0.0 ) return std::sqrt(W2);
    else return (-0.1);
}


// Norm of 3-momentum transferred q3 [GeV]
double CVUniverse::Getq3_True() const {
    double q0 = GetRecoilE_True();  // [GeV]
    double Q2 = GetQ2_True();       // [GeV^2]
    
    return std::sqrt(std::pow(q0,2.0) + Q2);
}


// Pi0 kinematics
// ==============

// Full pi0 kinematics
void CVUniverse::GetPi0Kinematics_True(double& pi0_energy, double& pi0_kinetic, double& pi0_momentum, double& pi0_theta) const {
    pi0_energy   = -9999.0;
    pi0_kinetic  = -9999.0;
    pi0_momentum = -9999.0;
    pi0_theta    = -9999.0;
    
    int n_FSpart = MC_NFSPart();
    std::vector<int> pdg_vector       = MC_FSPartPDG();
    std::vector<double> energy_vector = MC_FSPartE();
    std::vector<double> px_vector     = MC_FSPartPx();
    std::vector<double> py_vector     = MC_FSPartPy();
    std::vector<double> pz_vector     = MC_FSPartPz();
    
    for ( int part = 0; part < n_FSpart; ++part )
    {
        if ( std::abs(pdg_vector[part]) == 111 ) {
            pi0_energy = energy_vector[part] / 1000.0;  // [GeV]
            
            pi0_kinetic = pi0_energy - (CCPi0AnaConstants::PI0_MASS / 1000.0);  // [GeV]
            
            double px    = px_vector[part] / 1000.0;  // [GeV]
            double py    = py_vector[part] / 1000.0;  // [GeV]
            double pz    = pz_vector[part] / 1000.0;  // [GeV]
            pi0_momentum = std::sqrt((px * px) + (py * py) + (px * px));  // [GeV]
            
            TVector3 pi0_p3(px, py, pz);
            pi0_p3.RotateX(MinervaUnits::numi_beam_angle_rad);
            pi0_theta = pi0_p3.Theta() * CCPi0AnaConstants::RADTODEG;  // [deg]
            break;
        }
    }
}


// Pi0 energy [GeV]
double CVUniverse::GetPi0E_True() const{
    double pi0_energy   = -9999.0;
    double pi0_kinetic  = -9999.0;
    double pi0_momentum = -9999.0;
    double pi0_theta    = -9999.0;
    
    GetPi0Kinematics_True(pi0_energy, pi0_kinetic, pi0_momentum, pi0_theta);
    return pi0_energy;
}


// Pi0 kinetic energy [GeV]
double CVUniverse::GetPi0KE_True() const{
    double pi0_energy   = -9999.0;
    double pi0_kinetic  = -9999.0;
    double pi0_momentum = -9999.0;
    double pi0_theta    = -9999.0;
    
    GetPi0Kinematics_True(pi0_energy, pi0_kinetic, pi0_momentum, pi0_theta);
    return pi0_kinetic;
}


// Pi0 momentum [GeV]
double CVUniverse::GetPi0P_True() const{
    double pi0_energy   = -9999.0;
    double pi0_kinetic  = -9999.0;
    double pi0_momentum = -9999.0;
    double pi0_theta    = -9999.0;
    
    GetPi0Kinematics_True(pi0_energy, pi0_kinetic, pi0_momentum, pi0_theta);
    return pi0_momentum;
}


// Pi0 polar angle w.r.t. beam [deg]
double CVUniverse::GetPi0Theta_True() const{
    double pi0_energy   = -9999.0;
    double pi0_kinetic  = -9999.0;
    double pi0_momentum = -9999.0;
    double pi0_theta    = -9999.0;
    
    GetPi0Kinematics_True(pi0_energy, pi0_kinetic, pi0_momentum, pi0_theta);
    return pi0_theta;
}


// Photon kinematics
// =================

// Leading photon energy from Geant4 [GeV]
double CVUniverse::GetGamma1E_True() const{
    return GetDouble("G4_Gamma1_E") / 1000.0;
}


// Secondary photon energy from Geant4 [GeV]
double CVUniverse::GetGamma2E_True() const{
    return GetDouble("G4_Gamma2_E") / 1000.0;
}


// Leading photon energy hits [GeV]
double CVUniverse::GetGamma1Ehit() const{
    return GetDouble("G4_Gamma1_Ehit") / 1000.0;
}


// Secondary photon energy hits [GeV]
double CVUniverse::GetGamma2Ehit() const{
    return GetDouble("G4_Gamma2_Ehit") / 1000.0;
}


// Charged pion kinematics (for coherent weight calculation)
// =======================

void CVUniverse::GetPionEnergyAndTheta_True(double& pion_energy, double& pion_theta) const {
    pion_energy = -9999.0;
    pion_theta  = -9999.0;
    
    int n_FSpart = MC_NFSPart();
    std::vector<int> pdg_vector       = MC_FSPartPDG();
    std::vector<double> energy_vector = MC_FSPartE();
    std::vector<double> px_vector     = MC_FSPartPx();
    std::vector<double> py_vector     = MC_FSPartPy();
    std::vector<double> pz_vector     = MC_FSPartPz();
    
    for ( int part = 0; part < n_FSpart; ++part )
    {
        if ( std::abs(pdg_vector[part]) == 211 ) {
            pion_energy = energy_vector[part] / 1000.0;  // [GeV]
            double px   = px_vector[part] / 1000.0;      // [GeV]
            double py   = py_vector[part] / 1000.0;      // [GeV]
            double pz   = pz_vector[part] / 1000.0;      // [GeV]
            
            TVector3 pion_p3(px, py, pz);
            pion_p3.RotateX(MinervaUnits::numi_beam_angle_rad);
            pion_theta = pion_p3.Theta() * CCPi0AnaConstants::RADTODEG;  // [deg]
            break;
        }
    }
}





// ======================================================================================================================================
//  TRUE MATERIAL
// ======================================================================================================================================

// True vertex position  // [mm]
double CVUniverse::MC_VtxX() const { return GetVecElem("mc_vtx", 0); }
double CVUniverse::MC_VtxY() const { return GetVecElem("mc_vtx", 1); }
double CVUniverse::MC_VtxZ() const { return GetVecElem("mc_vtx", 2); }


// True vertex inside target? or plastic?
bool CVUniverse::IsTrueVertexInsidePlastic() const { return GetBool("truth_Vertex_InsidePlastic"); }


// True plane number
int CVUniverse::TrueVertexPlane() const { return GetInt("truth_Vertex_Plane"); }


// True vertex in targets
// ======================
/* The following functions from TargetUtils account for:
   -> Correct target number
   -> Correct nucleus atomic number Z
   -> Correct location of vertex (x,y,z) within target width */

// True vertex in Pb of target 4?
bool CVUniverse::IsTrueTgt4Pb() const {
    double vtx_x = MC_VtxX();
    double vtx_y = MC_VtxY();
    double vtx_z = MC_VtxZ();
    
    TargetUtils tgt_utils;
    return tgt_utils.InLead4VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);
}


// True vertex in Pb of target 5?
bool CVUniverse::IsTrueTgt5Pb() const {
    double vtx_x = MC_VtxX();
    double vtx_y = MC_VtxY();
    double vtx_z = MC_VtxZ();
    
    TargetUtils tgt_utils;
    return tgt_utils.InLead5VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);  // Keep 'excludeBuffer' as FALSE (default)
}


// True vertex in Fe of target 5?
bool CVUniverse::IsTrueTgt5Fe() const {
    double vtx_x = MC_VtxX();
    double vtx_y = MC_VtxY();
    double vtx_z = MC_VtxZ();
    
    TargetUtils tgt_utils;
    return tgt_utils.InIron5VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);  // Keep 'excludeBuffer' as FALSE (default)
}


// True vertex in plastic
// ======================

// True vertex in plastic upstream target 4?
bool CVUniverse::IsTruePlasUp() const {
    bool true_tg4pb     = IsTrueTgt4Pb();
    bool true_tg5pb     = IsTrueTgt5Pb();
    bool true_tg5fe     = IsTrueTgt5Fe();
    bool inside_plastic = IsTrueVertexInsidePlastic();
    int vertex_plane    = TrueVertexPlane();
    
    return (!true_tg4pb && !true_tg5pb && !true_tg5fe &&
            inside_plastic && (vertex_plane > -99999) && (vertex_plane <= 38));
}


// True vertex in plastic between targets 4 and 5?
bool CVUniverse::IsTruePlasBetw() const {
    bool true_tg4pb     = IsTrueTgt4Pb();
    bool true_tg5pb     = IsTrueTgt5Pb();
    bool true_tg5fe     = IsTrueTgt5Fe();
    bool inside_plastic = IsTrueVertexInsidePlastic();
    int vertex_plane    = TrueVertexPlane();
    
    return (!true_tg4pb && !true_tg5pb && !true_tg5fe &&
            inside_plastic && (vertex_plane >= 41) && (vertex_plane <= 44));
}


// True vertex in plastic downstream target 5?
bool CVUniverse::IsTruePlasDown() const {
    bool true_tg4pb     = IsTrueTgt4Pb();
    bool true_tg5pb     = IsTrueTgt5Pb();
    bool true_tg5fe     = IsTrueTgt5Fe();
    bool inside_plastic = IsTrueVertexInsidePlastic();
    int vertex_plane    = TrueVertexPlane();
    
    return (!true_tg4pb && !true_tg5pb && !true_tg5fe &&
            inside_plastic && (vertex_plane >= 47));
}





// ======================================================================================================================================
//  SIGNAL AND BACKGROUND
// ======================================================================================================================================

// Signal
// ======

bool CVUniverse::IsSignal(std::string option_material) const {
    bool signal = false;
    
    if ( option_material == "lead" ) {
        signal = (IsTrueCC_1Pi0() && (IsTrueTgt4Pb() || IsTrueTgt5Pb()) && (GetMuonTheta_True() <= 17.0));
    }
    else if ( option_material == "iron" ) {
        signal = (IsTrueCC_1Pi0() && IsTrueTgt5Fe() && (GetMuonTheta_True() <= 17.0));
    }
    return signal;
}


// Physics background
// ==================

// High-W pi0 production
bool CVUniverse::IsBackgr_Pi0HighW(std::string option_material) const {
    bool backgr = false;
    
    if ( option_material == "lead" ) {
        backgr = (IsTrueCC_Pi0HighW() && (IsTrueTgt4Pb() || IsTrueTgt5Pb()));
    }
    else if ( option_material == "iron" ) {
        backgr = (IsTrueCC_Pi0HighW() && IsTrueTgt5Fe());
    }
    return backgr;
}


// QE-like
bool CVUniverse::IsBackgr_QElike(std::string option_material) const {
    bool backgr = false;
    
    if ( option_material == "lead" ) {
        backgr = (IsTrueCC_QElike() && (IsTrueTgt4Pb() || IsTrueTgt5Pb()));
    }
    else if ( option_material == "iron" ) {
        backgr = (IsTrueCC_QElike() && IsTrueTgt5Fe());
    }
    return backgr;
}


// Pion production
bool CVUniverse::IsBackgr_PionProd(std::string option_material) const {
    bool backgr = false;
    
    if ( option_material == "lead" ) {
        backgr = (IsTrueCC_PionProd() && (IsTrueTgt4Pb() || IsTrueTgt5Pb()));
    }
    else if ( option_material == "iron" ) {
        backgr = (IsTrueCC_PionProd() && IsTrueTgt5Fe());
    }
    return backgr;
}


// Plastic background
// ==================

// Plastic upstream target 4
bool CVUniverse::IsBackgr_PlasUp() const {
    return IsTruePlasUp();
}


// Plastic between targets 4 and 5
bool CVUniverse::IsBackgr_PlasBetw() const {
    return IsTruePlasBetw();
}


// Plastic downstream target 5
bool CVUniverse::IsBackgr_PlasDown() const {
    return IsTruePlasDown();
}





// ======================================================================================================================================
//  WEIGHTS
// ======================================================================================================================================

double CVUniverse::GetWeight(const EnumModels& type_model) const
{
    // Return weight = 1.0 if event is data
    if ( type_model == kDataNoModel ) return 1.0;
    
    
    // Create weights and initialize them to 1.0
    double wgt_flux_and_cv = 1.0;
    double wgt_fsi         = 1.0;
    double wgt_genie       = 1.0;
    double wgt_2p2h        = 1.0;
    double wgt_rpa         = 1.0;
    // double wgt_coh_pi      = 1.0;
    // double wgt_mk          = 1.0;
    double wgt_lowq2pi     = 1.0;
    double wgt_geant       = 1.0;
    double wgt_tarmass     = 1.0;
    double wgt_michel_eff  = 1.0;
    double wgt_minos_eff   = 1.0;
    
    
    // Weights applicable to Truth and MC reco
    // =======================================
    
    // Flux + CV (requires using nu-e + IMD constraints)
    wgt_flux_and_cv = GetFluxAndCVWeight();
    
    
    // GENIE FSI bug fix
    if ( type_model != kGENIE ) wgt_fsi = GetFSIWeight(0);
    
    
    // Low recoil 2p2h tune
    if ( type_model != kGENIE ) wgt_2p2h = GetLowRecoil2p2hWeight();
    
    
    // RPA tune
    if ( type_model != kGENIE ) wgt_rpa = GetRPAWeight();
    
    
    // Non-resonant pion and deuterium reweightings:
    // Each of these two tunes are set independently on MacroUtil.h/.cxx
    if ( type_model != kGENIE ) wgt_genie = GetGenieWeight();
    
    
    // Pion production low-Q2 suppression
    if ( type_model == kMnvGENIEv2_MINOS )        wgt_lowq2pi = GetLowQ2PiWeight("MINOS");
    else if ( type_model == kMnvGENIEv2_JOINT )   wgt_lowq2pi = GetLowQ2PiWeight("JOINT");
    else if ( type_model == kMnvGENIEv2_NU1PI )   wgt_lowq2pi = GetLowQ2PiWeight("NU1PI");
    else if ( type_model == kMnvGENIEv2_NUNPI )   wgt_lowq2pi = GetLowQ2PiWeight("NUNPI");
    else if ( type_model == kMnvGENIEv2_NUPI0 )   wgt_lowq2pi = GetLowQ2PiWeight("NUPI0");
    else if ( type_model == kMnvGENIEv2_MENU1PI ) wgt_lowq2pi = GetLowQ2PiWeight("MENU1PI");
    
    
    // // Minoo Kabirnezhad (MK) weight
    // if ( type_model == kMnvGENIEv1_withMK ) wgt_mk = GetMKWeight();
    
    
    // // Coherent pion weight
    // if ( type_model == kMnvGENIEv1_withCohPi ) {
    //     double pion_energy = -9999.0;
    //     double pion_theta  = -9999.0;
    //     GetPionEnergyAndTheta_True(pion_energy, pion_theta);
        
    //     if ( pion_energy > 0.0 )
    //         wgt_coh_pi = GetCoherentPiWeight(pion_energy, pion_theta);
    //     else
    //         wgt_coh_pi = 1.0;
    // }
    
    
    // Weights applicable only to MC reco
    // ==================================
    
    if ( !IsTruth() )
    {
        // GEANT hadron
        wgt_geant = GetGeantHadronWeight();
        
        // Target mass
        wgt_tarmass = GetTargetMassWeight();
        
        // Michel tag efficiency
        wgt_michel_eff = GetMichelEfficiencyWeight();
        
        // MINOS muon tracking efficiency
        if ( Survive_MinosMatch() ) wgt_minos_eff = GetMinosEfficiencyWeight();
    }
    
    return (wgt_flux_and_cv * wgt_fsi * wgt_genie * wgt_2p2h * wgt_rpa * wgt_lowq2pi *
            wgt_geant * wgt_tarmass * wgt_michel_eff * wgt_minos_eff);
}





// ======================================================================================================================================
//  BASIC CUTS AND RECO VERTEX
// ======================================================================================================================================

// Basic cuts
// ==========

// Interaction vertex
bool CVUniverse::Survive_InteractionVertex() const {
    return GetBool("Survive_HasInteractionVertex");
}


// Neutrino helicity
bool CVUniverse::Survive_NeutrinoHelicity() const {
    int helicity = GetInt("CCPi0AnaTool_nuHelicity");
    return helicity == 1;
}


// MINOS match
bool CVUniverse::Survive_MinosMatch() const {
    return GetBool("Survive_HasMinosMatch");
}


// Muon charge
bool CVUniverse::Survive_MuonCharge() const {
    return GetBool("Survive_MuonNegative");
}


// Muon track angle w.r.t. beam
bool CVUniverse::Survive_MuonTrackAngle() const {
    double theta = GetMuonTheta();  // [deg]
    return (theta <= 17.0);
}


// Dead time discriminators
bool CVUniverse::Survive_DeadTime() const {
    int n_dead_discr = GetInt("phys_n_dead_discr_pair_upstream_prim_track_proj");
    return (n_dead_discr <= 1);
}





// ======================================================================================================================================
//  RECO VERTICES AND FIDUCIAL VOLUME
// ======================================================================================================================================

// Reco vertices
// =============

// Default vertex [mm]
double CVUniverse::VtxX() const { return GetVecElem("vtx", 0); }
double CVUniverse::VtxY() const { return GetVecElem("vtx", 1); }
double CVUniverse::VtxZ() const { return GetVertexZ(); }  // Keep it this way for MAT shifts


// Muon vertex [mm]
double CVUniverse::MuonVertexX() const { return GetDouble("MuonVertex_X"); }
double CVUniverse::MuonVertexY() const { return GetDouble("MuonVertex_Y"); }
double CVUniverse::MuonVertexZ() const { return GetDouble("MuonVertex_Z"); }


// Target vertex [mm]  -- TO DO: VARIABLE TO APPLY SYSTEMATIC SHIFTS
double CVUniverse::TargetVertexX() const {
    double vtx_x = GetDouble("TargetVertex_X");
    return vtx_x;
}

double CVUniverse::TargetVertexY() const {
    double vtx_y = GetDouble("TargetVertex_Y");
    return vtx_y;
}

double CVUniverse::TargetVertexZ() const {
    double vtx_z = GetDouble("TargetVertex_Z");
    return vtx_z;
}


// Fiducial volume
// ===============

// Muon vertex plane
int CVUniverse::MuonVertexPlane() const  { return GetInt("MuonVertex_Plane"); }


// Target vertex in Pb of target 4?
bool CVUniverse::Survive_FiducialReco_Tgt4Pb() const {
    int muon_vertex_plane = MuonVertexPlane();
    double vtx_x = TargetVertexX();
    double vtx_y = TargetVertexY();
    double vtx_z = TargetVertexZ();
    
    TargetUtils tgt_utils;
    bool tgt4_pb = tgt_utils.InLead4VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);
    
    return (tgt4_pb && (muon_vertex_plane == 41));
}


// Target vertex in Pb of target 5?
bool CVUniverse::Survive_FiducialReco_Tgt5Pb() const {
    int muon_vertex_plane = MuonVertexPlane();
    double vtx_x = TargetVertexX();
    double vtx_y = TargetVertexY();
    double vtx_z = TargetVertexZ();
    
    TargetUtils tgt_utils;
    bool tgt5_pb = tgt_utils.InLead5VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);  // Keep 'excludeBuffer' as FALSE (default)
    
    return (tgt5_pb && (muon_vertex_plane == 47));
}


// Target vertex in Fe of target 5?
bool CVUniverse::Survive_FiducialReco_Tgt5Fe() const {
    int muon_vertex_plane = MuonVertexPlane();
    
    double vtx_x = TargetVertexX();
    double vtx_y = TargetVertexY();
    double vtx_z = TargetVertexZ();
    
    TargetUtils tgt_utils;
    bool tgt5_fe = tgt_utils.InIron5VolMC(vtx_x, vtx_y, vtx_z, CCPi0AnaConstants::kApothem);  // Keep 'excludeBuffer' as FALSE (default)
    
    return (tgt5_fe && (muon_vertex_plane == 47));
}


// Plastic sidebands
// =================

// Plastic upstream target 4
bool CVUniverse::Sideband_PlasUp() const {
    int muon_vertex_plane = MuonVertexPlane();
    double vtx_x = MuonVertexX();
    double vtx_y = MuonVertexY();
    
    TargetUtils tgt_utils;
    bool inside_hexagon = tgt_utils.IsInHexagon(vtx_x, vtx_y, CCPi0AnaConstants::kApothem);
    
    return (inside_hexagon && (muon_vertex_plane >= 32) && (muon_vertex_plane <= 36));
}


// Plastic between targets 4 and 5
bool CVUniverse::Sideband_PlasBetw() const {
    int muon_vertex_plane = MuonVertexPlane();
    double vtx_x = MuonVertexX();
    double vtx_y = MuonVertexY();
    
    TargetUtils tgt_utils;
    bool inside_hexagon = tgt_utils.IsInHexagon(vtx_x, vtx_y, CCPi0AnaConstants::kApothem);
    
    return (inside_hexagon && (muon_vertex_plane >= 42) && (muon_vertex_plane <= 43));
}


// Plastic downstream target 5
bool CVUniverse::Sideband_PlasDown() const {
    int muon_vertex_plane = MuonVertexPlane();
    double vtx_x = MuonVertexX();
    double vtx_y = MuonVertexY();
    
    TargetUtils tgt_utils;
    bool inside_hexagon = tgt_utils.IsInHexagon(vtx_x, vtx_y, CCPi0AnaConstants::kApothem);
    
    return (inside_hexagon && (muon_vertex_plane >= 48) && (muon_vertex_plane <= 54));
}





// ======================================================================================================================================
//  MUON FUNCTIONS
// ======================================================================================================================================

// Muon energy [GeV]
double CVUniverse::GetMuonE() const {
    return GetEmu() / 1000.0;
}


// Muon momentum [GeV]
double CVUniverse::GetMuonP() const {
    return GetPmu() / 1000.0;
}


// Muon transverse momentum [GeV]
double CVUniverse::GetMuonPt() const {
    return GetPmu() * std::sin(GetThetamu()) / 1000.0;
}


// Muon polar angle [deg]
double CVUniverse::GetMuonTheta() const {
    return GetThetamu() * CCPi0AnaConstants::RADTODEG;
}





// ======================================================================================================================================
//  MICHEL FUNCTIONS
// ======================================================================================================================================

// Start point vertices
// ====================

int CVUniverse::NstartPointVertexMichels() const {
    int Nmichels = GetInt("NstartPointVertexMichels");
    if ( Nmichels > 0 ) return Nmichels;
    else return 0;
}


int CVUniverse::StartPointVertexMichelTruePDG(const int michel_index) const {
    int pdg_index = -99999;
    
    double pi0_fraction     = GetVecElem("StartPointVertexMichel_Pi0FractionVector",     michel_index);
    double proton_fraction  = GetVecElem("StartPointVertexMichel_ProtonFractionVector",  michel_index);
    double neutron_fraction = GetVecElem("StartPointVertexMichel_NeutronFractionVector", michel_index);
    double pion_fraction    = GetVecElem("StartPointVertexMichel_PiplusFractionVector",  michel_index) +
                              GetVecElem("StartPointVertexMichel_PiminusFractionVector", michel_index);
    double em_fraction      = GetVecElem("StartPointVertexMichel_EMFractionVector",      michel_index);
    double muon_fraction    = GetVecElem("StartPointVertexMichel_MuonFractionVector",    michel_index);
    double othPdg_fraction  = GetVecElem("StartPointVertexMichel_OtherFractionVector",   michel_index);
    double mcXtalk_fraction = GetVecElem("StartPointVertexMichel_MCXtalkFractionVector", michel_index);
    double overlay_fraction = GetVecElem("StartPointVertexMichel_OverlayFractionVector", michel_index);
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fraction);
    pdg_fraction.push_back(proton_fraction);
    pdg_fraction.push_back(neutron_fraction);
    pdg_fraction.push_back(pion_fraction);
    pdg_fraction.push_back(em_fraction);
    pdg_fraction.push_back(muon_fraction);
    pdg_fraction.push_back(othPdg_fraction);
    pdg_fraction.push_back(mcXtalk_fraction);
    pdg_fraction.push_back(overlay_fraction);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}


// Stop point vertices
// ===================

int CVUniverse::NstopPointVertexMichels() const {
    int Nmichels = GetInt("NstopPointVertexMichels");
    if ( Nmichels > 0 ) return Nmichels;
    else return 0;
}


int CVUniverse::StopPointVertexMichelTruePDG(const int michel_index) const {
    int pdg_index = -99999;
    
    double pi0_fraction     = GetVecElem("StopPointVertexMichel_Pi0FractionVector",     michel_index);
    double proton_fraction  = GetVecElem("StopPointVertexMichel_ProtonFractionVector",  michel_index);
    double neutron_fraction = GetVecElem("StopPointVertexMichel_NeutronFractionVector", michel_index);
    double pion_fraction    = GetVecElem("StopPointVertexMichel_PiplusFractionVector",  michel_index) +
                              GetVecElem("StopPointVertexMichel_PiminusFractionVector", michel_index);
    double em_fraction      = GetVecElem("StopPointVertexMichel_EMFractionVector",      michel_index);
    double muon_fraction    = GetVecElem("StopPointVertexMichel_MuonFractionVector",    michel_index);
    double othPdg_fraction  = GetVecElem("StopPointVertexMichel_OtherFractionVector",   michel_index);
    double mcXtalk_fraction = GetVecElem("StopPointVertexMichel_MCXtalkFractionVector", michel_index);
    double overlay_fraction = GetVecElem("StopPointVertexMichel_OverlayFractionVector", michel_index);
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fraction);
    pdg_fraction.push_back(proton_fraction);
    pdg_fraction.push_back(neutron_fraction);
    pdg_fraction.push_back(pion_fraction);
    pdg_fraction.push_back(em_fraction);
    pdg_fraction.push_back(muon_fraction);
    pdg_fraction.push_back(othPdg_fraction);
    pdg_fraction.push_back(mcXtalk_fraction);
    pdg_fraction.push_back(overlay_fraction);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}


// Kinked vertices
// ===============

int CVUniverse::NkinkedVertexMichels() const {
    int Nmichels = GetInt("NkinkedVertexMichels");
    if ( Nmichels > 0 ) return Nmichels;
    else return 0;
}


int CVUniverse::KinkedVertexMichelTruePDG(const int michel_index) const {
    int pdg_index = -99999;
    
    double pi0_fraction     = GetVecElem("KinkedVertexMichel_Pi0FractionVector",     michel_index);
    double proton_fraction  = GetVecElem("KinkedVertexMichel_ProtonFractionVector",  michel_index);
    double neutron_fraction = GetVecElem("KinkedVertexMichel_NeutronFractionVector", michel_index);
    double pion_fraction    = GetVecElem("KinkedVertexMichel_PiplusFractionVector",  michel_index) +
                              GetVecElem("KinkedVertexMichel_PiminusFractionVector", michel_index);
    double em_fraction      = GetVecElem("KinkedVertexMichel_EMFractionVector",      michel_index);
    double muon_fraction    = GetVecElem("KinkedVertexMichel_MuonFractionVector",    michel_index);
    double othPdg_fraction  = GetVecElem("KinkedVertexMichel_OtherFractionVector",   michel_index);
    double mcXtalk_fraction = GetVecElem("KinkedVertexMichel_MCXtalkFractionVector", michel_index);
    double overlay_fraction = GetVecElem("KinkedVertexMichel_OverlayFractionVector", michel_index);
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fraction);
    pdg_fraction.push_back(proton_fraction);
    pdg_fraction.push_back(neutron_fraction);
    pdg_fraction.push_back(pion_fraction);
    pdg_fraction.push_back(em_fraction);
    pdg_fraction.push_back(muon_fraction);
    pdg_fraction.push_back(othPdg_fraction);
    pdg_fraction.push_back(mcXtalk_fraction);
    pdg_fraction.push_back(overlay_fraction);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}





// ======================================================================================================================================
//  LONG TRACK FUNCTIONS
// ======================================================================================================================================

// Primary non-muon tracks
// =======================

int CVUniverse::NprimTracks() const {
    int Ntracks = GetInt("NprimTracks");
    if ( Ntracks > 0 ) return Ntracks;
    else return 0;
}


int CVUniverse::PrimTrackIsContained() const {
    return GetInt("PrimTrack1_IsContained");
}


int CVUniverse::PrimTrackIsKinked() const {
    return GetInt("PrimTrack1_IsKinked");
}


double CVUniverse::PrimTrackPionScore() const {
    return GetDouble("PrimTrack1_PionScore");
}


int CVUniverse::PrimTrackTruePDG() const {
    int pdg_index = -99999;
    
    double pi0_fraction     = GetDouble("PrimTrack1_Pi0Fraction");
    double proton_fraction  = GetDouble("PrimTrack1_ProtonFraction");
    double neutron_fraction = GetDouble("PrimTrack1_NeutronFraction");
    double pion_fraction    = GetDouble("PrimTrack1_PiplusFraction") +
                              GetDouble("PrimTrack1_PiminusFraction");
    double em_fraction      = GetDouble("PrimTrack1_EMFraction");
    double muon_fraction    = GetDouble("PrimTrack1_MuonFraction");
    double othPdg_fraction  = GetDouble("PrimTrack1_OtherFraction");
    double mcXtalk_fraction = GetDouble("PrimTrack1_MCXtalkFraction");
    double overlay_fraction = GetDouble("PrimTrack1_OverlayFraction");
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fraction);
    pdg_fraction.push_back(proton_fraction);
    pdg_fraction.push_back(neutron_fraction);
    pdg_fraction.push_back(pion_fraction);
    pdg_fraction.push_back(em_fraction);
    pdg_fraction.push_back(muon_fraction);
    pdg_fraction.push_back(othPdg_fraction);
    pdg_fraction.push_back(mcXtalk_fraction);
    pdg_fraction.push_back(overlay_fraction);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}


// Secondary tracks
// ================

int CVUniverse::NsecTracks() const {
    int Ntracks = GetInt("NsecTracks");
    if ( Ntracks > 0 ) return Ntracks;
    else return 0;
}


int CVUniverse::SecTrackIsContained() const {
    return GetInt("SecTrack1_IsContained");
}


int CVUniverse::SecTrackIsKinked() const {
    return GetInt("SecTrack1_IsKinked");
}


double CVUniverse::SecTrackPionScore() const {
    return GetDouble("SecTrack1_PionScore");
}


int CVUniverse::SecTrackTruePDG() const {
    int pdg_index = -99999;
    
    double pi0_fraction     = GetDouble("SecTrack1_Pi0Fraction");
    double proton_fraction  = GetDouble("SecTrack1_ProtonFraction");
    double neutron_fraction = GetDouble("SecTrack1_NeutronFraction");
    double pion_fraction    = GetDouble("SecTrack1_PiplusFraction") +
                              GetDouble("SecTrack1_PiminusFraction");
    double em_fraction      = GetDouble("SecTrack1_EMFraction");
    double muon_fraction    = GetDouble("SecTrack1_MuonFraction");
    double othPdg_fraction  = GetDouble("SecTrack1_OtherFraction");
    double mcXtalk_fraction = GetDouble("SecTrack1_MCXtalkFraction");
    double overlay_fraction = GetDouble("SecTrack1_OverlayFraction");
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fraction);
    pdg_fraction.push_back(proton_fraction);
    pdg_fraction.push_back(neutron_fraction);
    pdg_fraction.push_back(pion_fraction);
    pdg_fraction.push_back(em_fraction);
    pdg_fraction.push_back(muon_fraction);
    pdg_fraction.push_back(othPdg_fraction);
    pdg_fraction.push_back(mcXtalk_fraction);
    pdg_fraction.push_back(overlay_fraction);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}





// ======================================================================================================================================
//  BLOB BASIC FUNCTIONS
// ======================================================================================================================================

// Blob multiplicity
// =================

int CVUniverse::AngleScanNblobs() const {
    int Nblobs = GetInt("AngleScan_Nblobs");
    if ( Nblobs > 0 ) return Nblobs;
    else return 0;
}


int CVUniverse::NblobsPassBasicQuality() const {
    int Nblobs = GetInt("NblobsPassBasicQuality");
    if ( Nblobs > 0 ) return Nblobs;
    else return 0;
}


int CVUniverse::NblobCandidates() const {
    int Nblobs = GetInt("NblobCandidates");
    if ( Nblobs > 0 ) return Nblobs;
    else return 0;
}


// Blob true PDG
// =============
/* Based on the calorimetric energy and the TG4Trajectories of each cluster within the blob. */

int CVUniverse::GetBlobTruePDG(const int blob_index) const {
    int pdg_index = -99999;
    
    std::string pi0_branch     = "Blob" + std::to_string(blob_index) + "_Pi0Fraction";
    std::string proton_branch  = "Blob" + std::to_string(blob_index) + "_ProtonFraction";
    std::string neutron_branch = "Blob" + std::to_string(blob_index) + "_NeutronFraction";
    std::string piplus_branch  = "Blob" + std::to_string(blob_index) + "_PiplusFraction";
    std::string piminus_branch = "Blob" + std::to_string(blob_index) + "_PiminusFraction";
    std::string em_branch      = "Blob" + std::to_string(blob_index) + "_EMFraction";
    std::string muon_branch    = "Blob" + std::to_string(blob_index) + "_MuonFraction";
    std::string othPdg_branch  = "Blob" + std::to_string(blob_index) + "_OtherFraction";
    std::string mcXtalk_branch = "Blob" + std::to_string(blob_index) + "_MCXtalkFraction";
    std::string overlay_branch = "Blob" + std::to_string(blob_index) + "_OverlayFraction";
    
    double pi0_fracion     = GetDouble(pi0_branch.c_str());
    double proton_fracion  = GetDouble(proton_branch.c_str());
    double neutron_fracion = GetDouble(neutron_branch.c_str());
    double pion_fracion    = GetDouble(piplus_branch.c_str()) +
                             GetDouble(piminus_branch.c_str());
    double em_fracion      = GetDouble(em_branch.c_str());
    double muon_fracion    = GetDouble(muon_branch.c_str());
    double othPdg_fracion  = GetDouble(othPdg_branch.c_str());
    double mcXtalk_fracion = GetDouble(mcXtalk_branch.c_str());
    double overlay_fracion = GetDouble(overlay_branch.c_str());
    
    std::vector<double> pdg_fraction;
    pdg_fraction.push_back(pi0_fracion);
    pdg_fraction.push_back(proton_fracion);
    pdg_fraction.push_back(neutron_fracion);
    pdg_fraction.push_back(pion_fracion);
    pdg_fraction.push_back(em_fracion);
    pdg_fraction.push_back(muon_fracion);
    pdg_fraction.push_back(othPdg_fracion);
    pdg_fraction.push_back(mcXtalk_fracion);
    pdg_fraction.push_back(overlay_fracion);
    
    int max_index = std::max_element(pdg_fraction.begin(), pdg_fraction.end()) - pdg_fraction.begin();
    if ( max_index == 0 )      pdg_index = 1;  // Pi0
    else if ( max_index == 1 ) pdg_index = 2;  // Proton
    else if ( max_index == 2 ) pdg_index = 3;  // Neutron
    else if ( max_index == 3 ) pdg_index = 4;  // Pion
    else if ( max_index == 4 ) pdg_index = 5;  // EM
    else if ( max_index == 5 ) pdg_index = 6;  // Muon
    else if ( max_index == 6 ) pdg_index = 7;  // Other PDG
    else if ( max_index == 7 ) pdg_index = 8;  // MC X-talk
    else if ( max_index == 8 ) pdg_index = 9;  // Overlay
    
    return pdg_index;
}


// Blob start point [mm]
// =====================

double CVUniverse::GetBlobStartPointX(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_StartPoint";
    return GetVecElem(branch.c_str(), 0);
}


double CVUniverse::GetBlobStartPointY(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_StartPoint";
    return GetVecElem(branch.c_str(), 1);
}


double CVUniverse::GetBlobStartPointZ(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_StartPoint";
    return GetVecElem(branch.c_str(), 2);
}


// Blob end point [mm]
// ===================

double CVUniverse::GetBlobEndPointX(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_EndPoint";
    return GetVecElem(branch.c_str(), 0);
}


double CVUniverse::GetBlobEndPointY(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_EndPoint";
    return GetVecElem(branch.c_str(), 1);
}


double CVUniverse::GetBlobEndPointZ(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_EndPoint";
    return GetVecElem(branch.c_str(), 2);
}


// Blob midpoint [mm]
// ==================

double CVUniverse::GetBlobMidPointX(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_MidPoint";
    return GetVecElem(branch.c_str(), 0);
}


double CVUniverse::GetBlobMidPointY(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_MidPoint";
    return GetVecElem(branch.c_str(), 1);
}


double CVUniverse::GetBlobMidPointZ(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_MidPoint";
    return GetVecElem(branch.c_str(), 2);
}


// Blob start and end points cross check
// =====================================
/* This function returns TRUE if the follwing two conditions are satisfied simultaneously:
 * 1) Blob start and end point 3D distances from the target vertex must be different.
 * 2) Blob start point must be closer in 3D distance to the target vertex than end point. */

bool CVUniverse::GetBlobIsGoodStartAndEndPoints(const int blob_index) const
{
    double double_epsilon = std::numeric_limits<double>::epsilon();
    
    double target_vertexX = TargetVertexX();
    double target_vertexY = TargetVertexY();
    double target_vertexZ = TargetVertexZ();
    
    double start_pointX = GetBlobStartPointX(blob_index);
    double start_pointY = GetBlobStartPointY(blob_index);
    double start_pointZ = GetBlobStartPointZ(blob_index);
    double start_dist   = std::sqrt(std::pow(start_pointX - target_vertexX,2.0) +
                                    std::pow(start_pointY - target_vertexY,2.0) +
                                    std::pow(start_pointZ - target_vertexZ,2.0));
    
    double end_pointX = GetBlobEndPointX(blob_index);
    double end_pointY = GetBlobEndPointY(blob_index);
    double end_pointZ = GetBlobEndPointZ(blob_index);
    double end_dist   = std::sqrt(std::pow(end_pointX - target_vertexX,2.0) +
                                  std::pow(end_pointY - target_vertexY,2.0) +
                                  std::pow(end_pointZ - target_vertexZ,2.0));
    
    bool start_end_different   = false;
    bool start_closer_than_end = false;
    
    if ( std::fabs(start_dist-end_dist) > double_epsilon ) start_end_different = true;
    if ( end_dist > start_dist ) start_closer_than_end = true;
    
    return (start_end_different && start_closer_than_end);
}


// Blob bad fit type-1
// ===================
/* Bad fit type-1 means when both fit start and end points are in one side of the target vertex in Z,
 * either upstream or downstream, but the start point is farther in Z than the end point. */

bool CVUniverse::GetBlobIsBadFitType1(const int blob_index) const {
    bool bad_fit = false;
    
    double target_vertexZ = TargetVertexZ();
    double start_pointZ   = GetBlobStartPointZ(blob_index);
    double end_pointZ     = GetBlobEndPointZ(blob_index);
    
    // If both start and end points are downstream vertex
    if ( (start_pointZ >= target_vertexZ) && (end_pointZ >= target_vertexZ) ) {
        // If startZ > endZ, is bad fit type-1
        if ( start_pointZ >= end_pointZ ) bad_fit = true;
    }
    
    // If both start and end points are upstream vertex    
    else if ( (start_pointZ < target_vertexZ) && (end_pointZ < target_vertexZ) ) {
        // If startZ < endZ, is bad fit type-1
        if ( start_pointZ < end_pointZ ) bad_fit = true;
    }
    
    return bad_fit;
}


// Blob bad fit type-2
// ===================
/* Bad fit type-2 means when either start or end point is upstream the target vertex in Z
 * while the other is downstream, and the start point is indeed closer in Z to the vertex. */

bool CVUniverse::GetBlobIsBadFitType2(const int blob_index) const {
    bool bad_fit = false;
    
    double target_vertexZ = TargetVertexZ();
    double start_pointZ   = GetBlobStartPointZ(blob_index);
    double end_pointZ     = GetBlobEndPointZ(blob_index);
    
    // If start point is upstream vertex and end point is downstream vertex
    if ( (start_pointZ < target_vertexZ) && (end_pointZ >= target_vertexZ) ) {
        // If startZ is closer to vertex, is bad fit type-2
        if ( std::fabs(start_pointZ - target_vertexZ) < std::fabs(end_pointZ - target_vertexZ) ) bad_fit = true;
    }
    
    // If start point is downstream vertex and end point is upstream vertex
    else if ( (start_pointZ >= target_vertexZ) && (end_pointZ < target_vertexZ) ) {
        // If startZ is closer to vertex, is bad fit type-2
        if ( std::fabs(start_pointZ - target_vertexZ) < std::fabs(end_pointZ - target_vertexZ) ) bad_fit = true;
    }
    
    return bad_fit;
}


// Blob bad fit type-3
// ===================
/* Bad fit type-3 means when either start or end point is upstream the vertex in Z
 * while the other is downstream, similarly to type-2, except that this time
 * the start point is farther in Z to the vertex than the end point. */

bool CVUniverse::GetBlobIsBadFitType3(const int blob_index) const {
    bool bad_fit = false;
    
    double target_vertexZ = TargetVertexZ();
    double start_pointZ   = GetBlobStartPointZ(blob_index);
    double end_pointZ     = GetBlobEndPointZ(blob_index);
    
    // If start point is upstream vertex and end point is downstream vertex
    if ( (start_pointZ < target_vertexZ) && (end_pointZ >= target_vertexZ) ) {
        // If startZ is farther to vertex, is bad fit type-3
        if ( std::fabs(start_pointZ - target_vertexZ) >= std::fabs(end_pointZ - target_vertexZ) ) bad_fit = true;
    }
    
    // If start point is downstream vertex and end point is upstream vertex
    else if ( (start_pointZ >= target_vertexZ) && (end_pointZ < target_vertexZ) ) {
        // If startZ is farther to vertex, is bad fit type-3
        if ( std::fabs(start_pointZ - target_vertexZ) >= std::fabs(end_pointZ - target_vertexZ) ) bad_fit = true;
    }
    
    return bad_fit;
}


// Blob start-point Michel
// =======================

int CVUniverse::GetBlobStartPointMichelMatch(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_StartPointMichel_Match";
    int Nmichels = GetInt(branch.c_str());
    
    if ( Nmichels > 0 ) return Nmichels;
    else return 0;
}


// Blob end-point Michel
// =====================

int CVUniverse::GetBlobEndPointMichelMatch(const int blob_index) const {
    std::string branch = "Blob" + std::to_string(blob_index) + "_EndPointMichel_Match";
    int Nmichels = GetInt(branch.c_str());
    
    if ( Nmichels > 0 ) return Nmichels;
    else return 0;
}





// ======================================================================================================================================
//  BLOB GEOMETRY FUNCTIONS
// ======================================================================================================================================

// Blob axis vector cosine direction  -- TO DO: VARIABLE TO APPLY SYSTEMATIC SHIFTS
// =================================
/* Cosine directions of the axis vector (from the blob start to the end point) w.r.t. detector coordinates. */

// Cosine direction in X
double CVUniverse::GetBlobAxisCosDirX(const int blob_index) const {
    double blob_start_x = GetBlobStartPointX(blob_index);  // [mm]
    double blob_start_y = GetBlobStartPointY(blob_index);  // [mm]
    double blob_start_z = GetBlobStartPointZ(blob_index);  // [mm]
    
    double blob_end_x = GetBlobEndPointX(blob_index);  // [mm]
    double blob_end_y = GetBlobEndPointY(blob_index);  // [mm]
    double blob_end_z = GetBlobEndPointZ(blob_index);  // [mm]
    
    double blob_vec_x = blob_end_x - blob_start_x;  // [mm]
    double blob_vec_y = blob_end_y - blob_start_y;  // [mm]
    double blob_vec_z = blob_end_z - blob_start_z;  // [mm]
    
    TVector3 blob_vec(blob_vec_x, blob_vec_y, blob_vec_z);  // [mm]
    double blob_cosdir_x = (blob_vec.Unit()).X();
    return blob_cosdir_x;
}


// Cosine direction in Y
double CVUniverse::GetBlobAxisCosDirY(const int blob_index) const {
    double blob_start_x = GetBlobStartPointX(blob_index);  // [mm]
    double blob_start_y = GetBlobStartPointY(blob_index);  // [mm]
    double blob_start_z = GetBlobStartPointZ(blob_index);  // [mm]
    
    double blob_end_x = GetBlobEndPointX(blob_index);  // [mm]
    double blob_end_y = GetBlobEndPointY(blob_index);  // [mm]
    double blob_end_z = GetBlobEndPointZ(blob_index);  // [mm]
    
    double blob_vec_x = blob_end_x - blob_start_x;  // [mm]
    double blob_vec_y = blob_end_y - blob_start_y;  // [mm]
    double blob_vec_z = blob_end_z - blob_start_z;  // [mm]
    
    TVector3 blob_vec(blob_vec_x, blob_vec_y, blob_vec_z);  // [mm]
    double blob_cosdir_y = (blob_vec.Unit()).Y();
    return blob_cosdir_y;
}


// Cosine direction in Z
double CVUniverse::GetBlobAxisCosDirZ(const int blob_index) const {
    double blob_start_x = GetBlobStartPointX(blob_index);  // [mm]
    double blob_start_y = GetBlobStartPointY(blob_index);  // [mm]
    double blob_start_z = GetBlobStartPointZ(blob_index);  // [mm]
    
    double blob_end_x = GetBlobEndPointX(blob_index);  // [mm]
    double blob_end_y = GetBlobEndPointY(blob_index);  // [mm]
    double blob_end_z = GetBlobEndPointZ(blob_index);  // [mm]
    
    double blob_vec_x = blob_end_x - blob_start_x;  // [mm]
    double blob_vec_y = blob_end_y - blob_start_y;  // [mm]
    double blob_vec_z = blob_end_z - blob_start_z;  // [mm]
    
    TVector3 blob_vec(blob_vec_x, blob_vec_y, blob_vec_z);  // [mm]
    double blob_cosdir_z = (blob_vec.Unit()).Z();
    return blob_cosdir_z;
}


// Blob axis angle w.r.t. muon track [deg]
// =======================================
/* Angle between the blob axis vector (from start to end points) and the muon track momentum.
 * Muon track momentum is calculated w.r.t. beam direction, needs to be rotated back to detector coordinates. */

double CVUniverse::GetBlobAxisAngleWRTMuon(const int blob_index) const {
    double muon_p       = GetPmu();       // [MeV]
    double muon_theta_x = GetThetaXmu();  // [rad]
    double muon_theta_y = GetThetaYmu();  // [rad]
    double muon_theta   = GetThetamu();   // [rad]
    
    double muon_px = muon_p * std::sin(muon_theta_x);  // [MeV]
    double muon_py = muon_p * std::sin(muon_theta_y);  // [MeV]
    double muon_pz = muon_p * std::cos(muon_theta);    // [MeV]
    
    TVector3 muon_3p(muon_px, muon_py, muon_pz);  // [MeV]
    muon_3p.RotateX(-1.0 * CCPi0AnaConstants::NUMI_BEAM_ANGLE_RAD);  // Rotate muon momentum to detector coordinates
    
    double blob_cosdir_x = GetBlobAxisCosDirX(blob_index);
    double blob_cosdir_y = GetBlobAxisCosDirY(blob_index);
    double blob_cosdir_z = GetBlobAxisCosDirZ(blob_index);
    
    TVector3 blob_axis(blob_cosdir_x, blob_cosdir_y, blob_cosdir_z);
    double angle = blob_axis.Angle(muon_3p);     // [rad]
    return angle * CCPi0AnaConstants::RADTODEG;  // [rad] -> [deg]
}


// Blob midpoint angle w.r.t. muon track [deg]
// ===========================================
/* Angle between the blob midpoint vector (from target vertex to midpoint) and the muon track momentum.
 * Muon track momentum is calculated w.r.t. beam direction, needs to be rotated back to detector coordinates. */

double CVUniverse::GetBlobMidpointAngleWRTMuon(const int blob_index) const {
    double muon_p       = GetPmu();       // [MeV]
    double muon_theta_x = GetThetaXmu();  // [rad]
    double muon_theta_y = GetThetaYmu();  // [rad]
    double muon_theta   = GetThetamu();   // [rad]
    
    double muon_px = muon_p * std::sin(muon_theta_x);  // [MeV]
    double muon_py = muon_p * std::sin(muon_theta_y);  // [MeV]
    double muon_pz = muon_p * std::cos(muon_theta);    // [MeV]
    
    TVector3 muon_3p(muon_px, muon_py, muon_pz);  // [MeV]
    muon_3p.RotateX(-1.0 * CCPi0AnaConstants::NUMI_BEAM_ANGLE_RAD);  // Rotate muon momentum to detector coordinates
    
    double vtx_x = TargetVertexX();  // [mm]
    double vtx_y = TargetVertexY();  // [mm]
    double vtx_z = TargetVertexZ();  // [mm]
    
    double blob_mid_x = GetBlobMidPointX(blob_index);  // [mm]
    double blob_mid_y = GetBlobMidPointY(blob_index);  // [mm]
    double blob_mid_z = GetBlobMidPointZ(blob_index);  // [mm]
    
    TVector3 blob_mid_dir(blob_mid_x - vtx_x, blob_mid_y - vtx_y, blob_mid_z - vtx_z);  // [mm]
    double angle = (blob_mid_dir.Unit()).Angle(muon_3p);  // [rad]
    return angle * CCPi0AnaConstants::RADTODEG;           // [rad] -> [deg]
}


// Blob axis theta [deg]
// =====================
/* Angle of the blob axis vector (from start to end points) w.r.t. Z-axis in detector coordinates. */

double CVUniverse::GetBlobAxisTheta(const int blob_index) const {
    double blob_cosdir_x = GetBlobAxisCosDirX(blob_index);
    double blob_cosdir_y = GetBlobAxisCosDirY(blob_index);
    double blob_cosdir_z = GetBlobAxisCosDirZ(blob_index);
    
    TVector3 blob_axis(blob_cosdir_x, blob_cosdir_y, blob_cosdir_z);
    double axis_theta = (blob_axis.Unit()).Theta();   // [rad]
    return axis_theta * CCPi0AnaConstants::RADTODEG;  // [rad] -> [deg]
}


// Blob midpoint theta [deg]
// =========================
/* Angle of the blob midpoint vector (from target vertex to midpoint) w.r.t. Z-axis in detector coordinates. */

double CVUniverse::GetBlobMidpointTheta(const int blob_index) const {
    double vtx_x = TargetVertexX();  // [mm]
    double vtx_y = TargetVertexY();  // [mm]
    double vtx_z = TargetVertexZ();  // [mm]
    
    double blob_mid_x = GetBlobMidPointX(blob_index);  // [mm]
    double blob_mid_y = GetBlobMidPointY(blob_index);  // [mm]
    double blob_mid_z = GetBlobMidPointZ(blob_index);  // [mm]
    
    TVector3 blob_mid_dir(blob_mid_x - vtx_x, blob_mid_y - vtx_y, blob_mid_z - vtx_z);  // [mm]
    double blob_theta = (blob_mid_dir.Unit()).Theta();  // [rad]
    return blob_theta * CCPi0AnaConstants::RADTODEG;    // [rad] -> [deg]
}


// Blob axis projected deviation [mm]
// ==================================
/* Distance from the target vertex (vtx_x, vtx_y, vtx_z) to the blob axis vector projected onto the plane Z = vtx_z. */

double CVUniverse::GetBlobProjDeviation(const int blob_index) const {
    double vtx_x = TargetVertexX();  // [mm]
    double vtx_y = TargetVertexY();  // [mm]
    double vtx_z = TargetVertexZ();  // [mm]
    
    double blob_start_x = GetBlobStartPointX(blob_index);  // [mm]
    double blob_start_y = GetBlobStartPointY(blob_index);  // [mm]
    double blob_start_z = GetBlobStartPointZ(blob_index);  // [mm]
    
    double blob_cosdir_x = GetBlobAxisCosDirX(blob_index);
    double blob_cosdir_y = GetBlobAxisCosDirY(blob_index);
    double blob_cosdir_z = GetBlobAxisCosDirZ(blob_index);
    
    double alpha       = (vtx_z - blob_start_z) / blob_cosdir_z;  // [mm]
    double axis_proj_x = blob_start_x + (alpha * blob_cosdir_x);  // [mm]
    double axis_proj_y = blob_start_y + (alpha * blob_cosdir_y);  // [mm]
    
    double dev_x = axis_proj_x - vtx_x;  // [mm]
    double dev_y = axis_proj_y - vtx_y;  // [mm]
    double deviation = std::sqrt((dev_x * dev_x) + (dev_y * dev_y));  // [mm]
    return deviation;
}


// Blob axis angle deviation [deg]
// ===============================
/* Angle between the blob axis vector and the vector from the target vertex to the blob start point. */

double CVUniverse::GetBlobAngleDeviation(const int blob_index) const {
    double vtx_x = TargetVertexX();  // [mm]
    double vtx_y = TargetVertexY();  // [mm]
    double vtx_z = TargetVertexZ();  // [mm]
    
    double blob_start_x = GetBlobStartPointX(blob_index);  // [mm]
    double blob_start_y = GetBlobStartPointY(blob_index);  // [mm]
    double blob_start_z = GetBlobStartPointZ(blob_index);  // [mm]
    
    double blob_cosdir_x = GetBlobAxisCosDirX(blob_index);  // [mm]
    double blob_cosdir_y = GetBlobAxisCosDirY(blob_index);  // [mm]
    double blob_cosdir_z = GetBlobAxisCosDirZ(blob_index);  // [mm]
    
    TVector3 blob_closest(blob_start_x - vtx_x, blob_start_y - vtx_y, blob_start_z - vtx_z);  // [mm]
    TVector3 blob_axis(blob_cosdir_x, blob_cosdir_y, blob_cosdir_z);
    
    double blob_angle_dev = blob_axis.Angle(blob_closest.Unit());  // [rad]
    return blob_angle_dev * CCPi0AnaConstants::RADTODEG;           // [rad] -> [deg]
}





// ======================================================================================================================================
//  BLOB ENERGY FUNCTIONS
// ======================================================================================================================================

// Blob plane vector
// =================
/* Vector containing the planes which the blob extends to.
 * If blob goes backwards, vector order is switched, in such a way that the order of the planes go from blob start to end points.
 * Doesn't count "empty" planes (or gaps), only active planes. */

std::vector<int> CVUniverse::GetBlobPlaneVector(const int blob_index) const
{
    std::string branch = "Blob" + std::to_string(blob_index) + "_PlaneVector";
    std::vector<int> plane_vector = GetVecInt(branch.c_str());
    
    // If blob axis angle > 90 deg, reverse order of planes so it goes from blob start to end points
    double axis_theta = GetBlobAxisTheta(blob_index);
    if ( axis_theta > 90.0 ) {
        std::reverse(plane_vector.begin(), plane_vector.end());
    }
    return plane_vector;
}


// Blob calorimetric energy-per-plane vector [MeV]  -- TO DO: VARIABLE TO APPLY SYSTEMATIC SHIFTS
// ===============================================
/* Vector containing the calorimetric energy left by the blob on each plane that it extends to.
 * If blob goes backwards, vector order is switched, in such a way that the order of the energy on each plane go from blob start to end points.
 * Doesn't count "empty" planes (or gaps), only active planes. */

std::vector<double> CVUniverse::GetBlobEcaloPerPlane(const int blob_index) const
{
    std::string branch = "Blob" + std::to_string(blob_index) + "_EcaloPerPlaneVector";
    std::vector<double> ecalo_per_plane = GetVecDouble(branch.c_str());  // [MeV]
    
    // If blob axis angle > 90 deg, reverse order of energy on each plane so it goes from blob start to end points
    double axis_theta = GetBlobAxisTheta(blob_index);
    if ( axis_theta > 90.0 ) {
        std::reverse(ecalo_per_plane.begin(), ecalo_per_plane.end());
    }
    return ecalo_per_plane;
}


// Blob accumulated calorimetric energy-per-plane vector [MeV]
// ===========================================================
/* Vector containing the calorimetric energy left by the blob, but this time it's accumulated over each plane.
 * This means, 1st entry has energy of plane 1, 2nd entry has energy of planes 1+2, and so on... Last entry has blob total energy.
 * Since this cumulative is calculated from the energy-per-plane vector, it's ordered from blob start to end points.
 * Doesn't count "empty" planes (or gaps), only active planes. */

std::vector<double> CVUniverse::GetBlobEcaloAccumPerPlane(const int blob_index) const
{
    std::vector<double> ecalo_per_plane = GetBlobEcaloPerPlane(blob_index);  // [MeV]
    std::vector<double> ecalo_accum_per_plane;
    
    double ecalo_accum = 0.0;
    for ( unsigned int plane = 0; plane < ecalo_per_plane.size(); ++plane ) {
        double plane_ecalo = ecalo_per_plane[plane];  // [MeV]
        ecalo_accum += plane_ecalo;
        ecalo_accum_per_plane.push_back(ecalo_accum);
    }
    
    return ecalo_accum_per_plane;
}


// Blob total calorimetric energy [MeV]
// ====================================
/* Total calorimetric energy of the blob, calculated from the energy-per-plane vector. */

double CVUniverse::GetBlobEcalo(const int blob_index) const
{
    std::vector<double> ecalo_per_plane = GetBlobEcaloPerPlane(blob_index);  // [MeV]
    
    double ecalo = 0.0;
    for ( unsigned int plane = 0; plane < ecalo_per_plane.size(); ++plane ) {
        double plane_ecalo = ecalo_per_plane[plane];  // [MeV]
        ecalo += plane_ecalo;
    }
    return ecalo;
}


// Blob number of planes
// =====================
/* Number of blob planes that have deposited energy.
 * Doesn't count "empty" planes (or gaps), only active planes. */

int CVUniverse::GetBlobNplanes(const int blob_index) const {
    std::vector<double> ecalo_per_plane = GetBlobEcaloPerPlane(blob_index);  // [MeV]
    int n_planes = ecalo_per_plane.size();
    return n_planes;
}


// Blob dx [cm]
// ============
/* Blob length in cm along the blob axis.
 * Doesn't count "empty" planes (or gaps), only active planes. */

double CVUniverse::GetBlobdx(const int blob_index) const {
    double axis_theta = GetBlobAxisTheta(blob_index) * CCPi0AnaConstants::DEGTORAD;  // [deg]
    int n_planes = GetBlobNplanes(blob_index);
    
    double dx = 0.0;
    for ( int plane = 0; plane < n_planes; ++plane ) {
        dx += std::fabs(1.7 / std::cos(axis_theta));  // [cm]
    }
    return dx;
}


// Blob mean dE/dx [MeV/cm]
// ========================
/* Total calorimetric energy divided by the blob length.
 * Doesn't count "empty" planes (or gaps), only active planes. */

double CVUniverse::GetBlobdEdxMean(const int blob_index) const {
    double dE = GetBlobEcalo(blob_index);  // [MeV]
    double dx = GetBlobdx(blob_index);     // [cm]
    return (dE/dx);
}


// Blob front dE/dx [MeV/cm]
// =========================
/* Mean dE/dx among the first 15 cm at the front of the blob.
 * Doesn't count "empty" planes (or gaps), only active planes. */

double CVUniverse::GetBlobdEdxFront(const int blob_index) const
{
    // Helpful variables
    // -----------------
    double dE_front;
    double dx_front;
    double dEdx_front = -1.0;
    
    std::vector<double> dEaccum_per_plane = GetBlobEcaloAccumPerPlane(blob_index);  // Blob energy accumulated per plane [MeV]
    int n_total_planes = GetBlobNplanes(blob_index);                                // Number of blob total planes
    
    double axis_theta   = GetBlobAxisTheta(blob_index) * CCPi0AnaConstants::DEGTORAD;  // Blob axis theta [rad]
    double dx_per_plane = std::fabs(1.7 / std::cos(axis_theta));                       // Blob dx per plane [cm]
    
    
    // Find discrete (integer) number of planes of 15 cm "box"
    // -------------------------------------------------------
    int n_planes_15cm_int;
    double n_planes_15cm_double = 15.0/dx_per_plane;
    
    if ( n_planes_15cm_double > 1.0 ) {
        int n_planes_up   = std::ceil(n_planes_15cm_double);
        int n_planes_down = std::floor(n_planes_15cm_double);
        
        if ( std::fabs(double(n_planes_up)-n_planes_15cm_double) > std::fabs(double(n_planes_down)-n_planes_15cm_double) )
            n_planes_15cm_int = n_planes_down;
        else
            n_planes_15cm_int = n_planes_up;
    }
    else n_planes_15cm_int = 1;
    
    
    // Find front dE/dx when total number of planes is > number of planes of 15 cm "box"
    // ---------------------------------------------------------------------------------
    if ( n_total_planes > n_planes_15cm_int ) {
        // Front dE is the energy accumulated up the last plane of the 15 cm "box"
        dE_front = dEaccum_per_plane[n_planes_15cm_int-1];
        
        // End dx is just the number of planes of the box times dx per plane
        dx_front = dx_per_plane * n_planes_15cm_int;
        
        // Front dE/dx is the division
        dEdx_front = dE_front / dx_front;
    }
    
    
    // Find front dE/dx when total number of planes is < number of planes of 15 cm "box"
    // ---------------------------------------------------------------------------------
    else {
        // Front dE/dx in this case is just the mean dE/dx
        dEdx_front = GetBlobdEdxMean(blob_index);
    }
    
    
    // Return front dE/dx
    // -----------------------
    return dEdx_front;
}


// /* Lowest mean dE/dx among "blocks" of 10 cm within the first 50 cm at the front of the blob.
//  * Doesn't count "empty" planes (or gaps), only active planes. */

// double CVUniverse::GetBlobdEdxFront(const int blob_index) const
// {
//     // Helpful variables
//     // -----------------
//     double dEdx_front = -1.0;               // Front dE/dx
//     std::vector<double> dEdx_front_vector;  // dE/dx vector with all dE/dx of different 10 cm "blocks"
    
//     std::vector<double> dE_per_plane = GetBlobEcaloPerPlane(blob_index);  // Blob energy per plane [MeV]
//     int n_total_planes = GetBlobNplanes(blob_index);                      // Number of blob total planes
    
//     double axis_theta   = GetBlobAxisTheta(blob_index) * CCPi0AnaConstants::DEGTORAD;  // Blob axis theta [rad]
//     double dx_per_plane = std::fabs(1.7 / std::cos(axis_theta));                       // Blob dx per plane [cm]
    
    
//     // Find discrete (integer) number of planes per 10 cm "block"
//     // ----------------------------------------------------------
//     int n_planes_10cm_int;
//     double n_planes_10cm_double = 10.0/dx_per_plane;
    
//     // If floating (double) number of planes needed to get 10 cm is > 1,
//     // find a discrete (integer) number of planes closest to those 10 cm
//     if ( n_planes_10cm_double > 1.0 )
//     {
//         // Round floating number of planes to an integer, both up and down
//         int n_planes_up   = std::ceil(n_planes_10cm_double);
//         int n_planes_down = std::floor(n_planes_10cm_double);
        
//         // Decide which rounded integer is closer to the floating number of planes of the 10 cm "block",
//         // and assign that integer to the discrete number of planes of the 10 cm "block"
//         if ( std::fabs(double(n_planes_up)-n_planes_10cm_double) > std::fabs(double(n_planes_down)-n_planes_10cm_double) )
//             n_planes_10cm_int = n_planes_down;
//         else
//             n_planes_10cm_int = n_planes_up;
//     }
//     // If floating number of planes needed to get 10 cm is < 1, set discrete number of planes of the "block" to 1
//     else n_planes_10cm_int = 1;
    
    
//     // Find front dE/dx when total number of planes is > number of planes of 10 cm "block"
//     // -----------------------------------------------------------------------------------
//     if ( n_total_planes > n_planes_10cm_int )
//     {
//         // Repeat the same procedure done to find discrete number of planes of a "block" of 10 cm,
//         // but this time for a "box" of 50 cm
//         int n_planes_50cm_int;
//         double n_planes_50cm_double = 50.0/dx_per_plane;
        
//         if ( n_planes_50cm_double > 1.0 ) {
//             int n_planes_up   = std::ceil(n_planes_50cm_double);
//             int n_planes_down = std::floor(n_planes_50cm_double);
            
//             if ( std::fabs(double(n_planes_up)-n_planes_50cm_double) > std::fabs(double(n_planes_down)-n_planes_50cm_double) )
//                 n_planes_50cm_int = n_planes_down;
//             else
//                 n_planes_50cm_int = n_planes_up;
//         }
//         else n_planes_50cm_int = 1;
        
//         // If number of planes in "box" of 50 cm is > blob total number of planes, use that total instead
//         if ( n_planes_50cm_int > n_total_planes ) n_planes_50cm_int = n_total_planes;
        
//         // Get number of 10 cm "block" iterations within the 50 cm "box"
//         int n_iterations = (n_planes_50cm_int - n_planes_10cm_int) + 1;
        
//         // Loop over "blocks" and store their dE/dx
//         int block_first_plane = 0;
        
//         for ( int iteration = 0; iteration < n_iterations; ++iteration ) {
//             double dE_block = 0.0;
//             double dx_block = 0.0;
            
//             // Iterate over planes of current "block"
//             for ( int plane = 0; plane < n_planes_10cm_int; ++plane ) {
//                 dE_block += dE_per_plane[block_first_plane + plane];
//                 dx_block += dx_per_plane;
//             }
            
//             // Get dE/dx of current "block" and if it's > 0, store it
//             double dEdx_block = dE_block/dx_block;
//             if ( dEdx_block > 0.0 )
//                 dEdx_front_vector.push_back(dEdx_block);
            
//             // Move first plane of next "block" one plane up for next iteration
//             ++block_first_plane;
//         }
        
//         // Find minimum dE/dx among all 10 cm "block" dE/dx of the 50 cm "box"
//         if ( dEdx_front_vector.size() > 0 ) {
//             int min_index = std::min_element(dEdx_front_vector.begin(), dEdx_front_vector.end()) - dEdx_front_vector.begin();
//             dEdx_front = dEdx_front_vector[min_index];
//         }
//         else
//             dEdx_front = 0.0;
//     }
    
    
//     // Find front dE/dx when total number of planes is < number of planes of 10 cm "block"
//     // -----------------------------------------------------------------------------------
//     else {
//         // Front dE/dx in this case is just the mean dE/dx
//         dEdx_front = GetBlobdEdxMean(blob_index);
//     }
    
    
//     // Return front dE/dx
//     // ------------------
//     return dEdx_front;
// }


// Blob end dE/dx [MeV]
// ====================
/* Mean dE/dx in the last 12 cm of the blob.
 * Doesn't count "empty" planes (or gaps), only active planes. */

double CVUniverse::GetBlobdEdxEnd(const int blob_index) const
{
    // Helpful variables
    // -----------------
    double dE_end;
    double dx_end;
    double dEdx_end = -1.0;
    
    std::vector<double> dEaccum_per_plane = GetBlobEcaloAccumPerPlane(blob_index);  // Blob energy accumulated per plane [MeV]
    int n_total_planes = GetBlobNplanes(blob_index);                                // Number of blob total planes
    
    double axis_theta   = GetBlobAxisTheta(blob_index) * CCPi0AnaConstants::DEGTORAD;  // Blob axis theta [rad]
    double dx_per_plane = std::fabs(1.7 / std::cos(axis_theta));                       // Blob dx per plane [cm]
    
    
    // Find discrete (integer) number of planes of 12 cm "box"
    // -------------------------------------------------------
    int n_planes_12cm_int;
    double n_planes_12cm_double = 12.0/dx_per_plane;
    
    if ( n_planes_12cm_double > 1.0 ) {
        int n_planes_up   = std::ceil(n_planes_12cm_double);
        int n_planes_down = std::floor(n_planes_12cm_double);
        
        if ( std::fabs(double(n_planes_up)-n_planes_12cm_double) > std::fabs(double(n_planes_down)-n_planes_12cm_double) )
            n_planes_12cm_int = n_planes_down;
        else
            n_planes_12cm_int = n_planes_up;
    }
    else n_planes_12cm_int = 1;
    
    
    // Find end dE/dx when total number of planes is > number of planes of 12 cm "box"
    // -------------------------------------------------------------------------------
    if ( n_total_planes > n_planes_12cm_int ) {
        // End dE is the energy accumulated up the last plane (i.e. total energy) minus
        // the energy accumulated up to the plane before the start of the 12 cm "box"
        dE_end = dEaccum_per_plane.back() - dEaccum_per_plane[n_total_planes - (n_planes_12cm_int+1)];
        
        // End dx is just the number of planes of the box times dx per plane
        dx_end = dx_per_plane * n_planes_12cm_int;
        
        // End dE/dx is the division
        dEdx_end = dE_end / dx_end;
    }
    
    
    // Find end dE/dx when total number of planes is < number of planes of 12 cm "box"
    // -------------------------------------------------------------------------------
    else {
        // End dE/dx in this case is just the mean dE/dx
        dEdx_end = GetBlobdEdxMean(blob_index);
    }
    
    
    // Return End dE/dx
    // -----------------------
    return dEdx_end;
}





// ======================================================================================================================================
//  BLOB PARTICLE ID FUNCTIONS
// ======================================================================================================================================

// Analyze blob PDG-likelihood for single-blob events
// ==================================================
/* These functions analyze the likelihood of a blob to be either pi0-, pion-, proton-, or neutron-like depending on the region they are located
   evaluating a 2D observable. The functions receive the name of a cut and perform the analysis based on that. */

// Pi0-like
// --------
bool CVUniverse::IsPi0Like_SingleBlob(std::string cut, std::string option_material, const int blob_index) const {
    bool pi0_like = false;
    if ( blob_index != 1 ) return false;
    
    if ( cut == "BlobDeviation" ) {
        double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
        double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
        if ( option_material == "lead" ) {
            bool region1 = (proj_dev >= 0.0   && proj_dev <  125.0) && (angle_dev >= 0.0 && angle_dev <= 90.0);
            bool region2 = (proj_dev >= 125.0 && proj_dev <= 350.0) && (angle_dev >= 0.0 && angle_dev <= 18.0);
            pi0_like = (region1 || region2);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (proj_dev >= 0.0   && proj_dev <  100.0) && (angle_dev >= 0.0 && angle_dev <= 50.0);
            bool region2 = (proj_dev >= 100.0 && proj_dev <= 400.0) && (angle_dev >= 0.0 && angle_dev <= 21.0);
            pi0_like = (region1 || region2);
        }
        return pi0_like;
    }
    
    if ( cut == "BlobEnergyVSdx" ) {
        double dx    = GetBlobdx(blob_index);     // [cm]
        double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
        if ( option_material == "lead" ) {
            bool region1 = (dx >= 0.0 && dx <= 50.0) && (Ecalo >= 0.0 && Ecalo <= (0.0375 * dx * dx + 2.85 * dx));
            bool region2 = (dx >= 0.0 && dx <= 50.0) && (Ecalo >= 360.0);
            bool region3 = (dx >= 50.0);
            pi0_like = (region1 || region2 || region3);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (dx >= 0.0 && dx <= 55.0) && (Ecalo >= 0.0 && Ecalo <= (4.54545 * dx));
            bool region2 = (dx >= 0.0 && dx <= 55.0) && (Ecalo >= 340.0);
            bool region3 = (dx >= 55.0);
            pi0_like = (region1 || region2 || region3);
        }
        return pi0_like;
    }
    
    // if ( cut == "BlobdEdxFront" ) {
    //     double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
    //     double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
    //     if ( option_material == "lead" ) {
    //         bool region1 = (Ecalo >= 0.0 && Ecalo < 80.0) && (dEdx_front >= 0.0 && dEdx_front <= 7.0);
    //         bool region2 = (Ecalo >= 80.0 && Ecalo < 200.0) && (dEdx_front >= 3.0 && dEdx_front <= 7.0);
    //         bool region3 = (Ecalo >= 200.0) && (dEdx_front >= 3.0);
    //         pi0_like = (region1 || region2 || region3);
    //     }
    //     else if ( option_material == "iron" ) {
    //         bool region1 = (Ecalo >= 0.0 && Ecalo < 80.0) && (dEdx_front >= 0.0 && dEdx_front <= 7.0);
    //         bool region2 = (Ecalo >= 80.0 && Ecalo < 200.0) && (dEdx_front >= 3.0 && dEdx_front <= 7.0);
    //         bool region3 = (Ecalo >= 200.0) && (dEdx_front >= 3.0);
    //         pi0_like = (region1 || region2 || region3);
    //     }
    //     return pi0_like;
    // }
    
    if ( cut == "BlobdEdxEnd" ) {
        double Ecalo    = GetBlobEcalo(blob_index);    // [MeV]
        double dEdx_end = GetBlobdEdxEnd(blob_index);  // [MeV/cm]
        
        if ( option_material == "lead" ) {
            bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_end >= 0.0 && dEdx_end <= 4.0);
            bool region2 = (Ecalo >= 100.0 && Ecalo < 400.0) && (dEdx_end >= 0.0 && dEdx_end <= (3.0 + 0.01 * Ecalo));
            bool region3 = (Ecalo >= 400.0) && (dEdx_end >= 0.0);
            pi0_like = (region1 || region2 || region3);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_end >= 0.0 && dEdx_end <= 4.0);
            bool region2 = (Ecalo >= 100.0 && Ecalo < 400.0) && (dEdx_end >= 0.0 && dEdx_end <= (2.67 + 0.0133 * Ecalo));
            bool region3 = (Ecalo >= 400.0) && (dEdx_end >= 0.0);
            pi0_like = (region1 || region2 || region3);
        }
        return pi0_like;
    }
    
    else return false;
}


// // Pion-like
// // ---------
// bool CVUniverse::IsPionLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool pion_like = false;
//     if ( blob_index != 1 ) return false;
//     if ( IsPi0Like_SingleBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
//         double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (proj_dev >= 0.0 && proj_dev < 125.0) && (angle_dev >= 90.0);
//             bool region2 = (proj_dev >= 125.0 && proj_dev < 700.0) && (angle_dev >= (0.173913 + 0.142609 * proj_dev));
//             bool region3 = (proj_dev >= 700.0) && (angle_dev >= 100.0);
//             pion_like = (region1 || region2 || region3);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (proj_dev >= 0.0 && proj_dev < 100.0) && (angle_dev >= 50.0);
//             bool region2 = (proj_dev >= 100.0 && proj_dev < 1000.0) && (angle_dev >= (12.2222 + 0.087778 * proj_dev));
//             bool region3 = (proj_dev >= 1000.0) && (angle_dev >= 100.0);
//             pion_like = (region1 || region2 || region3);
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobEnergyVSdx" ) {
//         double dx    = GetBlobdx(blob_index);     // [cm]
//         double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (dx >= 55.0) && (Ecalo <= (30.7692 + 3.07692 * dx));
//             pion_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (dx >= 50.0) && (Ecalo <= (57.1429 + 2.85714 * dx));
//             pion_like = region1;
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobdEdxFront" ) {
//         double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
//         double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 80.0) && (dEdx_front >= 0.0 && dEdx_front <= 3.0);
//             pion_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 80.0) && (dEdx_front >= 0.0 && dEdx_front <= 3.0);
//             pion_like = region1;
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobdEdxEnd" ) {
//         if ( option_material == "lead" ) {
//             pion_like = false;
//         }
//         else if ( option_material == "iron" ) {
//             pion_like = false;
//         }
//         return pion_like;
//     }
    
//     else return false;
// }


// // Proton-like
// // -----------
// bool CVUniverse::IsProtonLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool proton_like = false;
//     if ( blob_index != 1 ) return false;
//     if ( IsPi0Like_SingleBlob(cut, option_material, blob_index) )  return false;
//     if ( IsPionLike_SingleBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
//         double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
//         if ( option_material == "lead" ) {
//             proton_like = false;  // Pi0- and proton-like showers share the same region
//         }
//         else if ( option_material == "iron" ) {
//             proton_like = false;  // Pi0- and proton-like showers share the same region
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobEnergyVSdx" ) {
//         double dx    = GetBlobdx(blob_index);     // [cm]
//         double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (dx >= 0.0 && dx <= 55.0) && (Ecalo >= (5.09091 * dx) && Ecalo <= (300.0 + 1.81818 * dx));
//             proton_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (dx >= 0.0 && dx <= 55.0) && (Ecalo >= (4.54545 * dx) && Ecalo <= (250.0 + 2.72727 * dx));
//             proton_like = region1;
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobdEdxFront" ) {
//         double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
//         double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo <= 200.0) && (dEdx_front >= 7.0);
//             proton_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo <= 200.0) && (dEdx_front >= 7.0);
//             proton_like = region1;
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobdEdxEnd" ) {
//         double Ecalo    = GetBlobEcalo(blob_index);    // [MeV]
//         double dEdx_end = GetBlobdEdxEnd(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 150.0) && (dEdx_end <= 5.0);
//             bool region2 = (Ecalo >= 150.0 && Ecalo < 400.0) && (dEdx_end >= (2.0 + 0.02 * Ecalo));
//             proton_like = (region1 || region2);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 200.0) && (dEdx_end >= 5.0);
//             bool region2 = (Ecalo >= 200.0 && Ecalo < 400.0) && (dEdx_end >= 0.025 * Ecalo);
//             proton_like = (region1 || region2);
//         }
//         return proton_like;
//     }
    
//     else return false;
// }


// // Neutron-like
// // ------------
// bool CVUniverse::IsNeutronLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool neutron_like = false;
//     if ( blob_index != 1 ) return false;
//     if ( IsPi0Like_SingleBlob(cut, option_material, blob_index) )  return false;
//     if ( IsPionLike_SingleBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
//         double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (proj_dev >= 125.0 && proj_dev < 350.0) && (angle_dev >= 18.0 && angle_dev <= (0.173913 + 0.142609 * proj_dev));
//             bool region2 = (proj_dev >= 325.0 && proj_dev < 700.0) && (angle_dev <= (0.173913 + 0.142609 * proj_dev));
//             bool region3 = (proj_dev >= 700.0) && (angle_dev <= 100.0);
//             neutron_like = (region1 || region2 || region3);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (proj_dev >= 100.0 && proj_dev < 400.0) && (angle_dev >= 21.0 && angle_dev <= (12.2222 + 0.087778 * proj_dev));
//             bool region2 = (proj_dev >= 400.0 && proj_dev < 1000.0) && (angle_dev <= (12.2222 + 0.087778 * proj_dev));
//             bool region3 = (proj_dev >= 1000.0) && (angle_dev <= 100.0);
//             neutron_like = (region1 || region2 || region3);
//         }
//         return neutron_like;
//     }
    
//     else return false;
// }



// Analyze blob PDG-likelihood for multi-blob events
// =================================================
/* Similar as above, but for multi-blob events. */

// Pi0-like
// --------
bool CVUniverse::IsPi0Like_MultiBlob(std::string cut, std::string option_material, const int blob_index) const {
    bool pi0_like = false;
    
    if ( cut == "BlobDeviation" ) {
        double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
        double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
        if ( option_material == "lead" ) {
            bool region1 = (proj_dev >= 0.0   && proj_dev <  180.0) && (angle_dev >= 0.0 && angle_dev <= 90.0);
            bool region2 = (proj_dev >= 180.0 && proj_dev <= 400.0) && (angle_dev >= 0.0 && angle_dev <= 21.0);
            pi0_like = (region1 || region2);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (proj_dev >= 0.0   && proj_dev <  150.0) && (angle_dev >= 0.0 && angle_dev <= 60.0);
            bool region2 = (proj_dev >= 150.0 && proj_dev <= 400.0) && (angle_dev >= 0.0 && angle_dev <= 25.0);
            pi0_like = (region1 || region2);
        }
        return pi0_like;
    }
    
    if ( cut == "BlobEnergyVSdx" ) {
        double dx    = GetBlobdx(blob_index);     // [cm]
        double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
        if ( option_material == "lead" ) {
            bool region1 = (dx >= 0.0 && dx <= 40.0) && (Ecalo >= 0.0 && Ecalo <= (5.0 * dx));
            bool region2 = (dx >= 0.0 && dx <= 40.0) && (Ecalo >= (300.0 + 2.5 * dx));
            bool region3 = (dx >= 40.0);
            pi0_like = (region1 || region2 || region3);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (dx >= 0.0 && dx <= 34.0) && (Ecalo >= 0.0 && Ecalo <= (5.0 * dx));
            bool region2 = (dx >= 0.0 && dx <= 34.0) && (Ecalo <= (5.0 * dx));
            bool region3 = (dx >= 34.0);
            pi0_like = (region1 || region2 || region3);
        }
        return pi0_like;
    }
    
    // if ( cut == "BlobdEdxFront" ) {
    //     double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
    //     double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
    //     if ( option_material == "lead" ) {
    //         bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_front >= 0.0 && dEdx_front <= 7.0);
    //         bool region2 = (Ecalo >= 100.0 && Ecalo < 200.0) && (dEdx_front >= 3.0 && dEdx_front <= (4.0 + 0.03 * Ecalo));
    //         bool region3 = (Ecalo >= 200.0) && (dEdx_front >= 3.0);
    //         pi0_like = (region1 || region2 || region3);
    //     }
    //     else if ( option_material == "iron" ) {
    //         bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_front >= 0.0 && dEdx_front <= 7.0);
    //         bool region2 = (Ecalo >= 100.0 && Ecalo < 200.0) && (dEdx_front >= 3.0 && dEdx_front <= (4.0 + 0.03 * Ecalo));
    //         bool region3 = (Ecalo >= 200.0) && (dEdx_front >= 3.0);
    //         pi0_like = (region1 || region2 || region3);
    //     }
    //     return pi0_like;
    // }
    
    if ( cut == "BlobdEdxEnd" ) {
        double Ecalo    = GetBlobEcalo(blob_index);    // [MeV]
        double dEdx_end = GetBlobdEdxEnd(blob_index);  // [MeV/cm]
        
        if ( option_material == "lead" ) {
            bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_end >= 0.0 && dEdx_end <= 6.0);
            bool region2 = (Ecalo >= 100.0 && Ecalo < 400.0) && (dEdx_end >= 0.0 && dEdx_end <= (5.33 + 0.00667 * Ecalo));
            bool region3 = (Ecalo >= 400.0) && (dEdx_end >= 0.0);
            pi0_like = (region1 || region2 || region3);
        }
        else if ( option_material == "iron" ) {
            bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_end >= 0.0 && dEdx_end <= 5.0);
            bool region2 = (Ecalo >= 100.0 && Ecalo < 260.0) && (dEdx_end >= 0.0 && dEdx_end <= (3.75 + 0.0125 * Ecalo));
            bool region3 = (Ecalo >= 260.0) && (dEdx_end >= 0.0);
            pi0_like = (region1 || region2 || region3);
        }
        return pi0_like;
    }
    
    else return false;
}


// // Pion-like
// // ---------
// bool CVUniverse::IsPionLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool pion_like = false;
//     if ( IsPi0Like_MultiBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
//         double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (proj_dev >= 0.0 && proj_dev < 180.0) && (angle_dev >= 90.0);
//             bool region2 = (proj_dev >= 180.0 && proj_dev < 400.0) && (angle_dev >= 21.0);
//             bool region3 = (proj_dev >= 400.0) && (angle_dev >= (-4.09091 + 0.0627273 * proj_dev));
//             pion_like = (region1 || region2 || region3);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (proj_dev >= 0.0 && proj_dev < 150.0) && (angle_dev >= 60.0);
//             bool region2 = (proj_dev >= 150.0) && (angle_dev >= (17.0588 + 0.0529412 * proj_dev));
//             pion_like = (region1 || region2);
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobEnergyVSdx" ) {
//         double dx    = GetBlobdx(blob_index);     // [cm]
//         double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (dx >= 50.0) && (Ecalo <= (-28.5714 + 3.57143 * dx));
//             pion_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (dx >= 55.0) && (Ecalo <= (-61.5385 + 3.84615 * dx));
//             pion_like = region1;
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobdEdxFront" ) {
//         double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
//         double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 100.0) && (dEdx_front >= 0.0 && dEdx_front <= 3.0);
//             pion_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 100.0) && (dEdx_front >= 0.0 && dEdx_front <= 3.0);
//             pion_like = region1;
//         }
//         return pion_like;
//     }
    
//     if ( cut == "BlobdEdxEnd" ) {
//         if ( option_material == "lead" ) {
//             pion_like = false;
//         }
//         else if ( option_material == "iron" ) {
//             pion_like = false;
//         }
//         return pion_like;
//     }
    
//     else return false;
// }


// // Proton-like
// // -----------
// bool CVUniverse::IsProtonLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool proton_like = false;
//     if ( IsPi0Like_MultiBlob(cut, option_material, blob_index) )  return false;
//     if ( IsPionLike_MultiBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         if ( option_material == "lead" ) {
//             proton_like = false;  // Pi0- and proton-like showers share the same region
//         }
//         else if ( option_material == "iron" ) {
//             proton_like = false;  // Pi0- and proton-like showers share the same region
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobEnergyVSdx" ) {
//         double dx    = GetBlobdx(blob_index);     // [cm]
//         double Ecalo = GetBlobEcalo(blob_index);  // [MeV]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (dx >= 0.0 && dx <= 45.0) && (Ecalo >= (5.0 * dx) && Ecalo <= (300.0 + 2.5 * dx));
//             proton_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (dx >= 0.0 && dx <= 40.0) && (Ecalo >= (5.0 * dx) && Ecalo <= (200.0 + 2.5 * dx));
//             proton_like = region1;
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobdEdxFront" ) {
//         double Ecalo      = GetBlobEcalo(blob_index);      // [MeV]
//         double dEdx_front = GetBlobdEdxFront(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_front >= 7.0);
//             bool region2 = (Ecalo >= 100.0 && Ecalo <= 200.0) && (dEdx_front >= (4.0 + 0.03 * Ecalo));
//             proton_like = (region1 || region2);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 100.0) && (dEdx_front >= 7.0);
//             bool region2 = (Ecalo >= 100.0 && Ecalo <= 200.0) && (dEdx_front >= (4.0 + 0.03 * Ecalo));
//             proton_like = (region1 || region2);
//         }
//         return proton_like;
//     }
    
//     if ( cut == "BlobdEdxEnd" ) {
//         double Ecalo    = GetBlobEcalo(blob_index);    // [MeV]
//         double dEdx_end = GetBlobdEdxEnd(blob_index);  // [MeV/cm]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 200.0) && (dEdx_end >= 8.0);
//             bool region2 = (Ecalo >= 200.0 && Ecalo <= 400.0) && (dEdx_end >= (6.0 + 0.01 * Ecalo));
//             proton_like = (region1 || region2);
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (Ecalo >= 0.0 && Ecalo < 200.0) && (dEdx_end >= 7.0);
//             bool region2 = (Ecalo >= 200.0 && Ecalo <= 300.0) && (dEdx_end >= (1.0 + 0.03 * Ecalo));
//             proton_like = (region1 || region2);
//         }
//         return proton_like;
//     }
    
//     else return false;
// }


// // Neutron-like
// // ------------
// bool CVUniverse::IsNeutronLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const {
//     bool neutron_like = false;
//     if ( IsPi0Like_MultiBlob(cut, option_material, blob_index) )  return false;
//     if ( IsPionLike_MultiBlob(cut, option_material, blob_index) ) return false;
    
//     if ( cut == "BlobDeviation" ) {
//         double proj_dev  = GetBlobProjDeviation(blob_index);   // [mm]
//         double angle_dev = GetBlobAngleDeviation(blob_index);  // [deg]
        
//         if ( option_material == "lead" ) {
//             bool region1 = (proj_dev <= 400.0) && (angle_dev <= (-4.09091 + 0.0627273 * proj_dev));
//             neutron_like = region1;
//         }
//         else if ( option_material == "iron" ) {
//             bool region1 = (proj_dev >= 150.0 && proj_dev < 400.0) && (angle_dev <= (17.0588 + 0.0529412 * proj_dev));
//             bool region2 = (proj_dev >= 400.0) && (angle_dev <= (17.0588 + 0.0529412 * proj_dev));
//             neutron_like = (region1 || region2);
//         }
//         return neutron_like;
//     }
    
//     else return false;
// }





// ======================================================================================================================================
//  RECONSTRUCTED KINEMATIC FUNCTIONS
// ======================================================================================================================================

// Recoil energy [GeV]
// ===================
/* Calorimetric recoil energy directly from anatuples (i.e. not corrected by multiplicative factors nor splines).
 * This method of defining recoil energy is a bit ambiguous, but basically can be explained this in a way that
 * we can satisfy the functions that are in [MAT-MINERvA/calculators/RecoilEnergyFunctions.h]:
 * 
 * 'GetCalRecoilEnergy()' returns the calorimetric recoil of the event, in my case it's all calorimetry energy since
 * I don't use any non-calorimetric recoil reconstruction. For this, I use the "Recoil_Ecalo" branch of my anatool.
 * 
 * 'GetNonCalRecoilEnergy()' returns the non calorimetric recoil. In my case it will return zero
 * since I don't perform any hadron energy reconstruction via non-calorimetric methods (like proton/pion tracks). */

double CVUniverse::GetCalRecoilEnergy() const {
    return GetDouble("Recoil_Ecalo") / 1000.0;  // [GeV]
}

double CVUniverse::GetNonCalRecoilEnergy() const {
    return 0.0;
}

double CVUniverse::GetRecoilE() const {
    return GetRecoilEnergy();
}


// 'Pseudo-pi0' energy [GeV]
// =========================
/* Energy that comes from adding the calorimetric energy of all blobs of the event.
 * The name of the function is a little picky, since this is not actually the pi0 energy at all. */

double CVUniverse::GetPseudoPi0E() const {
    double energy = 0.0;
    int Nblobs = NblobCandidates();
    
    for ( int blob_index = 1; blob_index <= Nblobs; ++blob_index ) {
        if ( blob_index > 5 ) continue;
        energy += (GetBlobEcalo(blob_index)/1000.0);  // [MeV] -> [GeV]
    }
    return energy;
}


// No-pi0 recoil energy [GeV]
// ==========================
/* Energy that comes from subtracting the "pseudo-pi0" energy from the calorimetric recoil energy. */

double CVUniverse::GetNoPi0RecoilE() const {
    return (GetRecoilE() - GetPseudoPi0E());
}


// Neutrino energy [GeV]
// =====================
/* Reconstructed neutrino energy is the sum of the muon energy and the calorimetric recoil. */

double CVUniverse::GetNeutrinoE() const {
    return (GetMuonE() + GetRecoilE());
}


// Transferred four-momentum Q2 [GeV^2]
// ====================================

double CVUniverse::GetQ2() const {
    double neutrino_E = GetNeutrinoE();                                // [GeV]
    double muon_mass  = CCPi0AnaConstants::MUON_MASS / 1000.0;         // [GeV]
    double muon_E     = GetMuonE();                                    // [GeV]
    double muon_P     = GetMuonP();                                    // [GeV]
    double muon_theta = GetMuonTheta() * CCPi0AnaConstants::DEGTORAD;  // [rad]
    
    double Q2 = 2.0 * neutrino_E * (muon_E - (muon_P * std::cos(muon_theta))) - std::pow(muon_mass,2.0);
    return Q2;
}


// Hadronic mass squared W2 [GeV^2/c^4]
// ====================================

double CVUniverse::GetW2() const {
    double nucl_mass = CCPi0AnaConstants::NUCLEON_MASS / 1000.0;  // [GeV]
    double Q2        = GetQ2();       // [GeV]
    double E_had     = GetRecoilE();  // [GeV]
    
    double W2 = std::pow(nucl_mass,2.0) + (2.0 * nucl_mass * E_had) - Q2;
    return W2;
}


// Hadronic mass W [GeV/c^2]
// =========================

double CVUniverse::GetW() const {
    double W2 = GetW2();  // [GeV^2]
    
    if ( W2 > 0.0 ) return std::sqrt(W2);
    else return -0.1;
}


#endif  // CVUniverse_cxx