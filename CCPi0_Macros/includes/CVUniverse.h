#ifndef CVUniverse_h
#define CVUniverse_h

#include "Constants.h"
#include "PlotUtils/ChainWrapper.h"
#include "PlotUtils/MinervaUniverse.h"
#include "PlotUtils/TargetUtils.h"



class CVUniverse : public PlotUtils::MinervaUniverse
{
    public:
        
        #include "PlotUtils/TruthFunctions.h"
        #include "PlotUtils/WeightFunctions.h"
        #include "PlotUtils/MuonFunctions.h"
        #include "PlotUtils/RecoilEnergyFunctions.h"
        #include "PlotUtils/MichelFunctions.h"
        
        
        
        // ========================================================================
        //  CONSTRUCTOR AND DESTRUCTOR
        // ========================================================================
        
        // Constructor
        CVUniverse(PlotUtils::ChainWrapper* chw,
                   double nsigma = 0);
        
        
        // Destructor
        virtual ~CVUniverse() {}
        
        
        
        // ========================================================================
        //  TRUE INTERACTION INFORMATION
        // ========================================================================
        
        // GENIE basic info
        virtual int MC_Incoming() const;
        virtual int MC_Current() const;
        
        virtual int MC_NFSPart() const;
        virtual std::vector<int> MC_FSPartPDG() const;
        virtual std::vector<double> MC_FSPartE() const;
        virtual std::vector<double> MC_FSPartPx() const;
        virtual std::vector<double> MC_FSPartPy() const;
        virtual std::vector<double> MC_FSPartPz() const;
        
        virtual void MC_NFSPartRelevant(int& n_muon, int& n_pi0, int& n_pion, int& n_other_meson, int& n_heavy_baryon) const;
        
        
        // Interaction type info
        virtual bool IsTrueCC() const;
        virtual bool IsTrueCC_1Pi0() const;
        virtual bool IsTrueCC_Pi0HighW() const;
        virtual bool IsTrueCC_QElike() const;
        virtual bool IsTrueCC_PionProd() const;
        
        
        // GENIE interaction type info
        virtual int MC_IntType() const;
        virtual int MC_Charm() const;
        virtual int MC_ResID() const;
        
        virtual bool IsTrueDISKinematics() const;
        
        virtual bool IsTrueGenieCC_QE() const;
        virtual bool IsTrueGenieCC_MEC() const;
        virtual bool IsTrueGenieCC_COH() const;
        virtual bool IsTrueGenieCC_DeltaRES() const;
        virtual bool IsTrueGenieCC_OtherRES() const;
        virtual bool IsTrueGenieCC_SoftDIS() const;
        virtual bool IsTrueGenieCC_TrueDIS() const;
        
        
        
        // ========================================================================
        //  TRUE KINEMATICS
        // ========================================================================
        
        // Neutrino and muon kinematics
        virtual double GetNeutrinoE_True() const;
        
        virtual double GetMuonE_True() const;
        virtual double GetMuonP_True() const;
        virtual double GetMuonPt_True() const;
        virtual double GetMuonPz_True() const;
        virtual double GetMuonTheta_True() const;
        
        virtual double GetQ2_True() const;
        
        
        // Energy kinematics
        virtual double GetRecoilE_True() const;
        virtual double GetW2_True() const;
        virtual double GetW_True() const;
        
        virtual double GetAvailableE_True() const;
        virtual double GetW2available_True() const;
        virtual double GetWavailable_True() const;
        
        virtual double Getq3_True() const;
        
        
        // Pi0 kinematics
        virtual void GetPi0Kinematics_True(double& pi0_energy, double& pi0_kinetic, double& pi0_momentum, double& pi0_theta) const;
        
        virtual double GetPi0E_True() const;
        virtual double GetPi0KE_True() const;
        virtual double GetPi0P_True() const;
        virtual double GetPi0Theta_True() const;
        
        
        // Photon kinematics
        virtual double GetGamma1E_True() const;
        virtual double GetGamma2E_True() const;
        
        virtual double GetGamma1Ehit() const;
        virtual double GetGamma2Ehit() const;
        
        
        // Charged pion kinematics (for coherent weight calculation)
        virtual void GetPionEnergyAndTheta_True(double& pion_energy, double& pion_theta) const;
        
        
        
        // ========================================================================
        //  TRUE MATERIAL
        // ========================================================================
        
        // True vertex position
        virtual double MC_VtxX() const;
        virtual double MC_VtxY() const;
        virtual double MC_VtxZ() const;
        
        virtual bool IsTrueVertexInsidePlastic() const;
        virtual int TrueVertexPlane() const;
        
        
        // True vertex in targets
        virtual bool IsTrueTgt4Pb() const;
        virtual bool IsTrueTgt5Pb() const;
        virtual bool IsTrueTgt5Fe() const;
        
        
        // True vertex in plastic
        virtual bool IsTruePlasUp() const;
        virtual bool IsTruePlasBetw() const;
        virtual bool IsTruePlasDown() const;
        
        
        
        // ========================================================================
        //  SIGNAL AND BACKGROUND
        // ========================================================================
        
        // Signal
        virtual bool IsSignal(std::string option_material) const;
        
        
        // Physics background
        virtual bool IsBackgr_Pi0HighW(std::string option_material) const;
        virtual bool IsBackgr_QElike(std::string option_material) const;
        virtual bool IsBackgr_PionProd(std::string option_material) const;
        
        
        // Plastic background
        virtual bool IsBackgr_PlasUp() const;
        virtual bool IsBackgr_PlasBetw() const;
        virtual bool IsBackgr_PlasDown() const;
        
        
        
        // ========================================================================
        //  WEIGHTS
        // ========================================================================
        
        virtual double GetWeight(const EnumModels& type_model) const;
        
        
        
        // ========================================================================
        //  BASIC CUTS
        // ========================================================================
        
        virtual bool Survive_InteractionVertex() const;
        virtual bool Survive_NeutrinoHelicity() const;
        virtual bool Survive_MinosMatch() const;
        virtual bool Survive_MuonCharge() const;
        virtual bool Survive_MuonTrackAngle() const;
        virtual bool Survive_DeadTime() const;
        
        
        
        // ========================================================================
        //  RECO CUTS AND FIDUCIAL VOLUME
        // ========================================================================
        
        // Default vertex
        virtual double VtxX() const;
        virtual double VtxY() const;
        virtual double VtxZ() const;
        
        
        // Muon vertex
        virtual double MuonVertexX() const;
        virtual double MuonVertexY() const;
        virtual double MuonVertexZ() const;
        
        
        // Target vertex
        virtual double TargetVertexX() const;
        virtual double TargetVertexY() const;
        virtual double TargetVertexZ() const;
        
        
        // Muon vertex plane
        virtual int MuonVertexPlane() const;
        
        
        // Fiducial volume
        virtual bool Survive_FiducialReco_Tgt4Pb() const;
        virtual bool Survive_FiducialReco_Tgt5Pb() const;
        virtual bool Survive_FiducialReco_Tgt5Fe() const;
        
        
        // Plastic sidebands
        virtual bool Sideband_PlasUp() const;
        virtual bool Sideband_PlasBetw() const;
        virtual bool Sideband_PlasDown() const;
        
        
        
        // ========================================================================
        //  MUON FUNCTIONS
        // ========================================================================
        
        virtual double GetMuonE() const;
        virtual double GetMuonP() const;
        virtual double GetMuonPt() const;
        virtual double GetMuonTheta() const;
        
        
        
        // ========================================================================
        //  MICHEL FUNCTIONS
        // ========================================================================
        
        // Start point vertices
        virtual int NstartPointVertexMichels() const;
        virtual int StartPointVertexMichelTruePDG(const int michel_index) const;
        
        
        // Stop point vertices
        virtual int NstopPointVertexMichels() const;
        virtual int StopPointVertexMichelTruePDG(const int michel_index) const;
        
        
        // Kinked vertices
        virtual int NkinkedVertexMichels() const;
        virtual int KinkedVertexMichelTruePDG(const int michel_index) const;
        
        
        
        // ========================================================================
        //  LONG TRACK FUNCTIONS
        // ========================================================================
        
        // Primary non-muon tracks
        virtual int NprimTracks() const;
        virtual int PrimTrackIsContained() const;
        virtual int PrimTrackIsKinked() const;
        virtual double PrimTrackPionScore() const;
        virtual int PrimTrackTruePDG() const;
        
        
        // Secondary tracks
        virtual int NsecTracks() const;
        virtual int SecTrackIsContained() const;
        virtual int SecTrackIsKinked() const;
        virtual double SecTrackPionScore() const;
        virtual int SecTrackTruePDG() const;
        
        
        
        // ========================================================================
        //  BLOB BASIC FUNCTIONS
        // ========================================================================
        
        // Blob multiplicity
        virtual int AngleScanNblobs() const;
        virtual int NblobsPassBasicQuality() const;
        virtual int NblobCandidates() const;
        
        
        // Blob true PDG
        virtual int GetBlobTruePDG(const int blob_index) const;
        
        
        // Blob start point
        virtual double GetBlobStartPointX(const int blob_index) const;
        virtual double GetBlobStartPointY(const int blob_index) const;
        virtual double GetBlobStartPointZ(const int blob_index) const;
        
        
        // Blob end point
        virtual double GetBlobEndPointX(const int blob_index) const;
        virtual double GetBlobEndPointY(const int blob_index) const;
        virtual double GetBlobEndPointZ(const int blob_index) const;
        
        
        // Blob midpoint
        virtual double GetBlobMidPointX(const int blob_index) const;
        virtual double GetBlobMidPointY(const int blob_index) const;
        virtual double GetBlobMidPointZ(const int blob_index) const;
        
        
        // Blob start and end points cross check
        virtual bool GetBlobIsGoodStartAndEndPoints(const int blob_index) const;
        
        
        // Blob bad fit types
        virtual bool GetBlobIsBadFitType1(const int blob_index) const;
        virtual bool GetBlobIsBadFitType2(const int blob_index) const;
        virtual bool GetBlobIsBadFitType3(const int blob_index) const;
        
        
        // Blob start and end point Michel
        virtual int GetBlobStartPointMichelMatch(const int blob_index) const;
        virtual int GetBlobEndPointMichelMatch(const int blob_index) const;
        
        
        
        // ========================================================================
        //  BLOB GEOMETRY FUNCTIONS
        // ========================================================================
        
        // Blob axis vector cosine direction
        virtual double GetBlobAxisCosDirX(const int blob_index) const;
        virtual double GetBlobAxisCosDirY(const int blob_index) const;
        virtual double GetBlobAxisCosDirZ(const int blob_index) const;
        
        
        // Blob angle w.r.t. muon track
        virtual double GetBlobAxisAngleWRTMuon(const int blob_index) const;
        virtual double GetBlobMidpointAngleWRTMuon(const int blob_index) const;
        
        
        // Blob theta
        virtual double GetBlobAxisTheta(const int blob_index) const;
        virtual double GetBlobMidpointTheta(const int blob_index) const;
        
        
        // Blob axis projected deviation
        virtual double GetBlobProjDeviation(const int blob_index) const;
        
        
        // Blob axis angle deviation
        virtual double GetBlobAngleDeviation(const int blob_index) const;
        
        
        
        // ========================================================================
        //  BLOB ENERGY FUNCTIONS
        // ========================================================================
        
        // Blob plane vector
        virtual std::vector<int> GetBlobPlaneVector(const int blob_index) const;
        
        
        // Blob calorimetric energy-per-plane vector
        virtual std::vector<double> GetBlobEcaloPerPlane(const int blob_index) const;
        
        
        // Blob accumulated calorimetric energy-per-plane vector
        virtual std::vector<double> GetBlobEcaloAccumPerPlane(const int blob_index) const;
        
        
        // Blob total calorimetric energy
        virtual double GetBlobEcalo(const int blob_index) const;
        
        
        // Blob number of planes
        virtual int GetBlobNplanes(const int blob_index) const;
        
        
        // Blob dx
        virtual double GetBlobdx(const int blob_index) const;
        
        
        // Blob mean dE/dx
        virtual double GetBlobdEdxMean(const int blob_index) const;
        
        
        // Blob front dE/dx
        virtual double GetBlobdEdxFront(const int blob_index) const;
        
        
        // Blob end dE/dx
        virtual double GetBlobdEdxEnd(const int blob_index) const;
        
        
        
        // ========================================================================
        //  BLOB PARTICLE ID FUNCTIONS
        // ========================================================================
        
        // Single-blob
        virtual bool IsPi0Like_SingleBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsPionLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsProtonLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsNeutronLike_SingleBlob(std::string cut, std::string option_material, const int blob_index) const;
        
        
        // Multi-blob
        virtual bool IsPi0Like_MultiBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsPionLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsProtonLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const;
        // virtual bool IsNeutronLike_MultiBlob(std::string cut, std::string option_material, const int blob_index) const;
        
        
        
        // ========================================================================
        //  RECONSTRUCTED KINEMATIC FUNCTIONS
        // ========================================================================
        
        virtual double GetCalRecoilEnergy() const;
        virtual double GetNonCalRecoilEnergy() const;
        virtual double GetRecoilE() const;
        
        virtual double GetPseudoPi0E() const;
        virtual double GetNoPi0RecoilE() const;
        
        virtual double GetNeutrinoE() const;
        virtual double GetQ2() const;
        
        virtual double GetW2() const;
        virtual double GetW() const;
        
};


#endif  // CVUniverse_h