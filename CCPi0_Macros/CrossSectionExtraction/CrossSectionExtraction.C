#ifndef CrossSectionExtraction_C
#define CrossSectionExtraction_C

#include <iostream>
#include <vector>

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

#include "MinervaUnfold/MnvUnfold.h"
#include "PlotUtils/FluxReweighter.h"
#include "PlotUtils/TargetUtils.h"

#include "TFile.h"





// ========================================================================================================================
//  HELPER FUNCTIONS
// ========================================================================================================================

// // =====================================================
// //  Unfold using method with fakes
// // =====================================================

// void UnfoldingWithFakes(Variable* var,
//                         PlotUtils::MnvH1D* &h_unfolded,
//                         PlotUtils::MnvH1D* h_folded,
//                         int n_iterations, bool add_systematics, bool use_sys_variated_migrations)
// {
//     // Get variable name
//     std::string var_name = var->Name();
    
    
//     // Perform unfolding
//     // =================
    
//     // Signal-only in reco (X) vs. true (Y) kinematics
//     PlotUtils::MnvH2D* h_migration = (PlotUtils::MnvH2D*)var->m_hists.m_MigrationMatrix->Clone(Form("migration_%s", var_name.c_str()));
    
//     // Not-background-subtracted MC reco
//     PlotUtils::MnvH1D* h_mc_reco = (PlotUtils::MnvH1D*)var->m_hists.m_mc_Tuned->Clone(Form("mc_reco_%s", var_name.c_str()));
    
//     // Signal-only MC truth
//     PlotUtils::MnvH1D* h_mc_truth = (PlotUtils::MnvH1D*)var->m_hists.m_EffNumerator->Clone(Form("mc_truth_%s", var_name.c_str()));
    
//     // Background-only MC reco
//     PlotUtils::MnvH1D* h_mc_backgr = nullptr;
    
//     // Covariance matrix (before fixing bug on diagonal elements)
//     TMatrixD cov_matrix;
    
//     // Unfold
//     MinervaUnfold::MnvUnfold mnv_unfold;
    
//     mnv_unfold.UnfoldHistoWithFakes(h_unfolded, cov_matrix,
//                                     h_migration, h_folded, h_mc_reco, h_mc_truth, h_mc_backgr,
//                                     n_iterations, add_systematics, use_sys_variated_migrations);
    
    
//     // Fix covariant matrix bug
//     // ========================
    
//     // Remove covariance matrix from unfolded distribution
//     TMatrixD* cov_dummy = h_unfolded->PopSysErrorMatrix("unfoldingCov");
    
//     // Perform "dummy" unfolding
//     TMatrixD cov_matrix_corrected;
    
//     TH1D* h_unfolded_dummy  = new TH1D(h_unfolded->GetCVHistoWithStatError());
//     TH2D* h_migration_dummy = new TH2D(h_migration->GetCVHistoWithStatError());
//     TH1D* h_folded_dummy    = new TH1D(h_folded->GetCVHistoWithStatError());
//     TH1D* h_mc_reco_dummy   = new TH1D(h_mc_reco->GetCVHistoWithStatError());
//     TH1D* h_mc_truth_dummy  = new TH1D(h_mc_truth->GetCVHistoWithStatError());
//     TH1D* h_mc_backgr_dummy = nullptr;
    
//     PlotUtils::MnvH1D* mnvh_unfolded_dummy  = new PlotUtils::MnvH1D(*h_unfolded_dummy);
//     PlotUtils::MnvH2D* mnvh_migration_dummy = new PlotUtils::MnvH2D(*h_migration_dummy);
//     PlotUtils::MnvH1D* mnvh_folded_dummy    = new PlotUtils::MnvH1D(*h_folded_dummy);
//     PlotUtils::MnvH1D* mnvh_mc_reco_dummy   = new PlotUtils::MnvH1D(*h_mc_reco_dummy);
//     PlotUtils::MnvH1D* mnvh_mc_truth_dummy  = new PlotUtils::MnvH1D(*h_mc_truth_dummy);
//     PlotUtils::MnvH1D* mnvh_mc_backgr_dummy = nullptr;
    
//     mnv_unfold.UnfoldHistoWithFakes(mnvh_unfolded_dummy, cov_matrix_corrected,
//                                     mnvh_migration_dummy, mnvh_folded_dummy, mnvh_mc_reco_dummy, mnvh_mc_truth_dummy, mnvh_mc_backgr_dummy,
//                                     n_iterations, add_systematics, use_sys_variated_migrations);
    
//     int correct_Nbins = h_unfolded_dummy->fN;
//     int matrix_rows   = cov_matrix_corrected.GetNrows();
    
//     // Resize correct covariance matrix (if needed)
//     if ( correct_Nbins != matrix_rows ) {
//         std::cout << std::endl;
//         std::cout << " Fixing unfolding covariance matrix size because of RooUnfold bug: from " << matrix_rows << " to " << correct_Nbins << endl;
//         std::cout << std::endl;
        
//         cov_matrix_corrected.ResizeTo(correct_Nbins, correct_Nbins);
//     }
    
//     // Set diagonal elements of covariance matrix to 0, and push them into unfolded distribution
//     for ( int i = 0; i < cov_matrix_corrected.GetNrows(); ++i )
//         cov_matrix_corrected(i, i) = 0.0;
    
//     h_unfolded -> PushCovMatrix("unfoldingCov", cov_matrix_corrected);
    
//     // Delete dummy objects
//     delete cov_dummy;
//     delete h_unfolded_dummy;
//     delete h_migration_dummy;
//     delete h_folded_dummy;
//     delete h_mc_reco_dummy;
//     delete h_mc_truth_dummy;
//     delete h_mc_backgr_dummy;
//     delete mnvh_unfolded_dummy;
//     delete mnvh_migration_dummy;
//     delete mnvh_folded_dummy;
//     delete mnvh_mc_reco_dummy;
//     delete mnvh_mc_truth_dummy;
//     delete mnvh_mc_backgr_dummy;
// }



// =====================================================
//  Unfold using standard method
// =====================================================

void UnfoldingStandard(Variable* var,
                       PlotUtils::MnvH1D* &h_unfolded,
                       PlotUtils::MnvH1D* h_folded,
                       int n_iterations, bool add_systematics, bool use_sys_variated_migrations, double cov_uncfactor)
{
    // Get variable name
    std::string var_name = var->Name();
    
    
    // Perform unfolding
    // =================
    
    // Signal-only in reco (X) vs. true (Y) kinematics
    PlotUtils::MnvH2D* h_migration = (PlotUtils::MnvH2D*)var->m_hists.m_MigrationMatrix->Clone(Form("migration_%s", var_name.c_str()));
    
    // Define covariance matrix (before fixing bug on diagonal elements)
    TMatrixD cov_matrix;
    
    // Unfold
    MinervaUnfold::MnvUnfold mnv_unfold;
    
    mnv_unfold.UnfoldHisto(h_unfolded, cov_matrix,
                           h_migration, h_folded,
                           RooUnfold::kBayes, n_iterations, add_systematics, use_sys_variated_migrations);
    
    
    // Fix covariant matrix bug
    // ========================
    
    // Remove covariance matrix from unfolded distribution
    TMatrixD* cov_dummy = h_unfolded->PopSysErrorMatrix("unfoldingCov");
    
    // Perform "dummy" unfolding
    TMatrixD cov_matrix_corrected;
    
    TH1D* h_unfolded_dummy  = new TH1D(h_unfolded->GetCVHistoWithStatError());
    TH2D* h_migration_dummy = new TH2D(h_migration->GetCVHistoWithStatError());
    TH1D* h_mc_reco_dummy   = new TH1D(h_migration->ProjectionX()->GetCVHistoWithStatError());
    TH1D* h_mc_truth_dummy  = new TH1D(h_migration->ProjectionY()->GetCVHistoWithStatError());
    TH1D* h_folded_dummy    = new TH1D(h_folded->GetCVHistoWithStatError());
    
    mnv_unfold.UnfoldHisto(h_unfolded_dummy, cov_matrix_corrected,
                           h_migration_dummy, h_mc_reco_dummy, h_mc_truth_dummy, h_folded_dummy,
                           RooUnfold::kBayes, n_iterations);
    
    int correct_Nbins = h_unfolded_dummy->fN;
    int matrix_rows   = cov_matrix_corrected.GetNrows();
    
    // Resize correct covariance matrix (if needed)
    if ( correct_Nbins != matrix_rows ) {
        std::cout << std::endl;
        std::cout << " Fixing unfolding covariance matrix size because of RooUnfold bug: from " << matrix_rows << " to " << correct_Nbins << endl;
        std::cout << std::endl;
        
        cov_matrix_corrected.ResizeTo(correct_Nbins, correct_Nbins);
    }
    
    // Set diagonal elements of covariance matrix to 0, and push them into unfolded distribution
    for ( int i = 0; i < cov_matrix_corrected.GetNrows(); ++i )
        cov_matrix_corrected(i, i) = 0.0;
    
    h_unfolded -> PushCovMatrix("unfoldingCov", cov_matrix_corrected, false);
    
    // Modify statistical uncertainty of unfolding matrix
    h_unfolded -> ModifyStatisticalUnc(cov_uncfactor, "unfoldingCov");
    
    // Delete dummy objects
    delete cov_dummy;
    delete h_unfolded_dummy;
    delete h_migration_dummy;
    delete h_mc_reco_dummy;
    delete h_mc_truth_dummy;
    delete h_folded_dummy;
}



// =====================================================
//  Write covariance and correlation marices
// =====================================================

void WriteCovAndCorrMatrices(Variable* var,
                             PlotUtils::MnvH1D* histo,
                             TFile& fout,
                             bool is_mc,
                             bool do_cross_check)
{
    fout.cd();
    
    // Get variable name
    std::string var_name = var->Name();
    
    // Get MC or data string
    const std::string mc_data_str = is_mc ? "mc" : "data";
    
    // Get names of all systematic error matrices names (including 'unfoldingCov')
    std::vector<std::string> error_names = histo->GetSysErrorMatricesNames();
    
    // Get diagonal stat. error matrix
    // 1st bool: Show as fractional error?
    TMatrixD diag_stat_err_matrix = histo->GetStatErrorMatrix(false);
    const int size = diag_stat_err_matrix.GetNrows();
    
    
    // Define error matrices to write
    // ==============================
    
    TMatrixD flux_cov_matrix(size, size);
    TMatrixD xsec_ccqe_2p2h_cov_matrix(size, size);
    TMatrixD xsec_respi_cov_matrix(size, size);
    TMatrixD xsec_nonrespi_cov_matrix(size, size);
    TMatrixD xsec_dis_cov_matrix(size, size);
    TMatrixD fsi_nucl_cov_matrix(size, size);
    TMatrixD fsi_pion_cov_matrix(size, size);
    TMatrixD muon_cov_matrix(size, size);
    TMatrixD part_resp_cov_matrix(size, size);
    TMatrixD other_cov_matrix(size, size);
    
    TMatrixD total_stat_cov_matrix(size, size);
    TMatrixD total_syst_cov_matrix(size, size);
    TMatrixD total_cov_matrix(size, size);
    
    // Add diagonal errors to total statistical matrix
    total_stat_cov_matrix += diag_stat_err_matrix;
    
    
    // Define containers of error band names
    // =====================================
    
    // Flux
    std::vector<std::string> flux_errors = {"Flux"};
    
    // Cross-section: EL + CCQE + RPA + 2p2h
    std::vector<std::string> xsec_ccqe_2p2h_errors = {"GENIE_MaNCEL", "GENIE_EtaNCEL",
                                                      "GENIE_MaCCQE", "GENIE_CCQEPauliSupViaKF", "GENIE_VecFFCCQEshape",
                                                      "Low_Recoil_2p2h_Tune", "RPA_HighQ2", "RPA_LowQ2"};
    
    // Cross-section: Resonant pion
    std::vector<std::string> xsec_respi_errors = {"GENIE_MaRES", "GENIE_MvRES", "GENIE_NormNCRES", "GENIE_RDecBR1gamma", "GENIE_Theta_Delta2Npi",
                                                  "GENIE_D2_MaRES", "GENIE_D2_NormCCRES", "GENIE_EP_MvRES", "LowQ2Pi"};
    
    // Cross-setion: Non-resonant pion
    std::vector<std::string> xsec_nonrespi_errors = {"GENIE_Rvn1pi", "GENIE_Rvn2pi", "GENIE_Rvp1pi", "GENIE_Rvp2pi"};
    
    // Cross-section: DIS/hadronization
    std::vector<std::string> xsec_dis_errors = {"GENIE_AhtBY", "GENIE_BhtBY", "GENIE_CV1uBY", "GENIE_CV2uBY", "GENIE_AGKYxF1pi", "GENIE_NormDISCC"};
    
    // FSI: nucleons
    std::vector<std::string> fsi_nucl_errors = {"GENIE_MFP_N", "GENIE_FrElas_N", "GENIE_FrInel_N", "GENIE_FrAbs_N", "GENIE_FrCEx_N", "GENIE_FrPiProd_N"};
    
    // FSI: pions
    std::vector<std::string> fsi_pion_errors = {"GENIE_MFP_pi", "GENIE_FrElas_pi", "GENIE_FrAbs_pi", "GENIE_FrCEx_pi", "GENIE_FrPiProd_pi"};
    
    // Muon reconstruction
    std::vector<std::string> muon_errors = {"Muon_Energy_MINERvA", "Muon_Energy_MINOS", "Muon_Energy_Resolution",
                                            "MuonAngleXResolution", "MuonAngleYResolution", "BeamAngleX", "BeamAngleY",
                                            "MINOS_Reconstruction_Efficiency"};
    
    // Particle response
    std::vector<std::string> part_resp_errors = {"response_low_proton", "response_mid_proton", "response_high_proton",
                                                 "response_low_neutron", "response_mid_neutron", "response_high_neutron",
                                                 "response_meson", "response_em", "response_other"};
    
    // Other
    std::vector<std::string> other_errors = {"GEANT_Proton", "GEANT_Neutron", "GEANT_Pion",
                                             "Target_Mass_CH", "Target_Mass_C", "Target_Mass_H2O", "Target_Mass_Fe", "Target_Mass_Pb",
                                             "MichelEfficiency"};
    
    
    // Get covariance matrices of systematic groups
    // ============================================
    
    // Loop over error bands
    for ( unsigned int i = 0; i < error_names.size(); ++i )
    {
        // Get individual syst. error matrix of each error band
        // 1st bool: Show as fractional error?
        // 2nd bool: Area-norm covariance?
        TMatrixD indiv_syst_cov_matrix = histo->GetSysErrorMatrix(error_names[i], false, false);
        
        // If error band is 'unfoldingCov', add it to total stat error matrix instead than a systematic
        if ( error_names[i] == "unfoldingCov" ) {
            total_stat_cov_matrix += indiv_syst_cov_matrix;
        }
        
        // If error band is NOT 'unfoldingCov', treatment is different
        else {
            // Find which systematic group the individual syst. matrix belongs, and add it to that syst. group matrix
            if ( std::find(flux_errors.begin(), flux_errors.end(), error_names[i]) != flux_errors.end() ) {
                flux_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(xsec_ccqe_2p2h_errors.begin(), xsec_ccqe_2p2h_errors.end(), error_names[i]) != xsec_ccqe_2p2h_errors.end() ) {
                xsec_ccqe_2p2h_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(xsec_respi_errors.begin(), xsec_respi_errors.end(), error_names[i]) != xsec_respi_errors.end() ) {
                xsec_respi_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(xsec_nonrespi_errors.begin(), xsec_nonrespi_errors.end(), error_names[i]) != xsec_nonrespi_errors.end() ) {
                xsec_nonrespi_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(xsec_dis_errors.begin(), xsec_dis_errors.end(), error_names[i]) != xsec_dis_errors.end() ) {
                xsec_dis_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(fsi_nucl_errors.begin(), fsi_nucl_errors.end(), error_names[i]) != fsi_nucl_errors.end() ) {
                fsi_nucl_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(fsi_pion_errors.begin(), fsi_pion_errors.end(), error_names[i]) != fsi_pion_errors.end() ) {
                fsi_pion_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(muon_errors.begin(), muon_errors.end(), error_names[i]) != muon_errors.end() ) {
                muon_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(part_resp_errors.begin(), part_resp_errors.end(), error_names[i]) != part_resp_errors.end() ) {
                part_resp_cov_matrix += indiv_syst_cov_matrix;
            }
            else if ( std::find(other_errors.begin(), other_errors.end(), error_names[i]) != other_errors.end() ) {
                other_cov_matrix += indiv_syst_cov_matrix;
            }
            else {
                std::cout << " ERROR WITH SYSTEMATIC ERROR BAND THAT DOESN'T BELONG TO ANY GROUP!!! " << std::endl;
                std::cout << error_names[i] << std::endl;
                std::exit(1);
            }
        }
    }
    
    
    // Write total covariance matrices
    // ===============================
    
    // Write total stat. error matrix
    TH2D* h_total_stat_cov_matrix = new TH2D(total_stat_cov_matrix);
    h_total_stat_cov_matrix -> Write(Form("%s_TotalStatCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_stat_cov_matrix;
    
    // Write covariance matrices of error groups
    TH2D* h_flux_cov_matrix = new TH2D(flux_cov_matrix);
    h_flux_cov_matrix -> Write(Form("%s_FluxCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_flux_cov_matrix;
    
    TH2D* h_xsec_ccqe_2p2h_cov_matrix = new TH2D(xsec_ccqe_2p2h_cov_matrix);
    h_xsec_ccqe_2p2h_cov_matrix -> Write(Form("%s_XsecCCQE2p2hCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_ccqe_2p2h_cov_matrix;
    
    TH2D* h_xsec_respi_cov_matrix = new TH2D(xsec_respi_cov_matrix);
    h_xsec_respi_cov_matrix -> Write(Form("%s_XsecResPiCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_respi_cov_matrix;
    
    TH2D* h_xxsec_nonrespi_cov_matrix = new TH2D(xsec_nonrespi_cov_matrix);
    h_xxsec_nonrespi_cov_matrix -> Write(Form("%s_XsecNonResPiCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xxsec_nonrespi_cov_matrix;
    
    TH2D* h_xsec_dis_cov_matrix = new TH2D(xsec_dis_cov_matrix);
    h_xsec_dis_cov_matrix -> Write(Form("%s_XsecDISCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_dis_cov_matrix;
    
    TH2D* h_fsi_nucl_cov_matrix = new TH2D(fsi_nucl_cov_matrix);
    h_fsi_nucl_cov_matrix -> Write(Form("%s_FSINuclCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_fsi_nucl_cov_matrix;
    
    TH2D* h_fsi_pion_cov_matrix = new TH2D(fsi_pion_cov_matrix);
    h_fsi_pion_cov_matrix -> Write(Form("%s_FSIPionCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_fsi_pion_cov_matrix;
    
    TH2D* h_muon_cov_matrix = new TH2D(muon_cov_matrix);
    h_muon_cov_matrix -> Write(Form("%s_MuonCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_muon_cov_matrix;
    
    TH2D* h_part_resp_cov_matrix = new TH2D(part_resp_cov_matrix);
    h_part_resp_cov_matrix -> Write(Form("%s_PartRespCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_part_resp_cov_matrix;
    
    TH2D* h_other_cov_matrix = new TH2D(other_cov_matrix);
    h_other_cov_matrix -> Write(Form("%s_OtherCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_other_cov_matrix;
    
    // Add syst. error group matrices to total syst. error matrix, and write it
    total_syst_cov_matrix += flux_cov_matrix;
    total_syst_cov_matrix += xsec_ccqe_2p2h_cov_matrix;
    total_syst_cov_matrix += xsec_respi_cov_matrix;
    total_syst_cov_matrix += xsec_nonrespi_cov_matrix;
    total_syst_cov_matrix += xsec_dis_cov_matrix;
    total_syst_cov_matrix += fsi_nucl_cov_matrix;
    total_syst_cov_matrix += fsi_pion_cov_matrix;
    total_syst_cov_matrix += muon_cov_matrix;
    total_syst_cov_matrix += part_resp_cov_matrix;
    total_syst_cov_matrix += other_cov_matrix;
    
    TH2D* h_total_syst_cov_matrix = new TH2D(total_syst_cov_matrix);
    h_total_syst_cov_matrix -> Write(Form("%s_TotalSystCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_syst_cov_matrix;
    
    // Add stat. and syst. error matrices to total error matrix, and write it
    total_cov_matrix += total_stat_cov_matrix;
    total_cov_matrix += total_syst_cov_matrix;
    
    TH2D* h_total_cov_matrix = new TH2D(total_cov_matrix);
    h_total_cov_matrix -> Write(Form("%s_TotalCovMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_cov_matrix;
    
    
    // Cross-check total covariance matrix
    // ===================================
    
    if ( do_cross_check ) {
        // 1st bool: Include statistical matrix?
        // 2nd bool: Show as fractional error?
        // 3rd bool: Covariance area-normalized?
        TMatrixD dummy_total_cov_matrix = histo->GetTotalErrorMatrix(true, false, false);
        
        for ( int x = 0; x < size; ++x ) {
            for ( int y = 0; y < size; ++y ) {
                if ( std::fabs(dummy_total_cov_matrix[x][y] - total_cov_matrix[x][y]) > std::numeric_limits<double>::epsilon() ) {
                    std::cout << " \tTotal covariance matrix discrepancy at [x][y] = [" << x << "][" << y << "] " << std::endl;
                    std::cout << " \t\tMy covariance matrix:        " << total_cov_matrix[x][y] << std::endl;
                    std::cout << " \t\tPlotUtils covariance matrix: " << dummy_total_cov_matrix[x][y] << std::endl;
                    std::cout << " \t\tAbs. difference : " << std::fabs(dummy_total_cov_matrix[x][y] - total_cov_matrix[x][y]) << std::endl;
                }
            }
        }
    }
    
    
    // Write correlation matrices (the following methods are adapted from 'MnvH1D::GetSysCorrelationMatrix')
    // ==========================
    
    // Total stat. correlation matrix
    TMatrixD total_stat_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            total_stat_corr_matrix[x][y] = (total_stat_cov_matrix[x][x] == 0.0 || total_stat_cov_matrix[y][y] == 0.0 ) ?
                                           0.0 : total_stat_cov_matrix[x][y] / std::sqrt(total_stat_cov_matrix[x][x]*total_stat_cov_matrix[y][y]);
        }
    }
    TH2D* h_total_stat_corr_matrix = new TH2D(total_stat_corr_matrix);
    h_total_stat_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_total_stat_corr_matrix -> Write(Form("%s_TotalStatCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_stat_corr_matrix;
    
    
    // Flux correlation matrix
    TMatrixD flux_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            flux_corr_matrix[x][y] = (flux_cov_matrix[x][x] == 0.0 || flux_cov_matrix[y][y] == 0.0 ) ?
                                     0.0 : flux_cov_matrix[x][y] / std::sqrt(flux_cov_matrix[x][x]*flux_cov_matrix[y][y]);
        }
    }
    TH2D* h_flux_corr_matrix = new TH2D(flux_corr_matrix);
    h_flux_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_flux_corr_matrix -> Write(Form("%s_FluxCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_flux_corr_matrix;
    
    
    // Xsection CCQE + 2p2h correlation matrix
    TMatrixD xsec_ccqe_2p2h_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            xsec_ccqe_2p2h_corr_matrix[x][y] = (xsec_ccqe_2p2h_cov_matrix[x][x] == 0.0 || xsec_ccqe_2p2h_cov_matrix[y][y] == 0.0 ) ?
                                               0.0 : xsec_ccqe_2p2h_cov_matrix[x][y] / std::sqrt(xsec_ccqe_2p2h_cov_matrix[x][x]*xsec_ccqe_2p2h_cov_matrix[y][y]);
        }
    }
    TH2D* h_xsec_ccqe_2p2h_corr_matrix = new TH2D(xsec_ccqe_2p2h_corr_matrix);
    h_xsec_ccqe_2p2h_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_xsec_ccqe_2p2h_corr_matrix -> Write(Form("%s_XsecCCQE2p2hCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_ccqe_2p2h_corr_matrix;
    
    
    // Xsection resonant pion correlation matrix
    TMatrixD xsec_respi_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            xsec_respi_corr_matrix[x][y] = (xsec_respi_cov_matrix[x][x] == 0.0 || xsec_respi_cov_matrix[y][y] == 0.0 ) ?
                                           0.0 : xsec_respi_cov_matrix[x][y] / std::sqrt(xsec_respi_cov_matrix[x][x]*xsec_respi_cov_matrix[y][y]);
        }
    }
    TH2D* h_xsec_respi_corr_matrix = new TH2D(xsec_respi_corr_matrix);
    h_xsec_respi_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_xsec_respi_corr_matrix -> Write(Form("%s_XsecResPiCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_respi_corr_matrix;
    
    
    // Xsection non-responant pion correlation matrix
    TMatrixD xsec_nonrespi_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            xsec_nonrespi_corr_matrix[x][y] = (xsec_nonrespi_cov_matrix[x][x] == 0.0 || xsec_nonrespi_cov_matrix[y][y] == 0.0 ) ?
                                              0.0 : xsec_nonrespi_cov_matrix[x][y] / std::sqrt(xsec_nonrespi_cov_matrix[x][x]*xsec_nonrespi_cov_matrix[y][y]);
        }
    }
    TH2D* h_xsec_nonrespi_corr_matrix = new TH2D(xsec_nonrespi_corr_matrix);
    h_xsec_nonrespi_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_xsec_nonrespi_corr_matrix -> Write(Form("%s_XsecNonResPiCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_nonrespi_corr_matrix;
    
    
    // Xsection DIS correlation matrix
    TMatrixD xsec_dis_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            xsec_dis_corr_matrix[x][y] = (xsec_dis_cov_matrix[x][x] == 0.0 || xsec_dis_cov_matrix[y][y] == 0.0 ) ?
                                         0.0 : xsec_dis_cov_matrix[x][y] / std::sqrt(xsec_dis_cov_matrix[x][x]*xsec_dis_cov_matrix[y][y]);
        }
    }
    TH2D* h_xsec_dis_corr_matrix = new TH2D(xsec_dis_corr_matrix);
    h_xsec_dis_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_xsec_dis_corr_matrix -> Write(Form("%s_XsecDISCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_xsec_dis_corr_matrix;
    
    
    // FSI nucleon correlation matrix
    TMatrixD fsi_nucl_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            fsi_nucl_corr_matrix[x][y] = (fsi_nucl_cov_matrix[x][x] == 0.0 || fsi_nucl_cov_matrix[y][y] == 0.0 ) ?
                                         0.0 : fsi_nucl_cov_matrix[x][y] / std::sqrt(fsi_nucl_cov_matrix[x][x]*fsi_nucl_cov_matrix[y][y]);
        }
    }
    TH2D* h_fsi_nucl_corr_matrix = new TH2D(fsi_nucl_corr_matrix);
    h_fsi_nucl_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_fsi_nucl_corr_matrix -> Write(Form("%s_FSINuclCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_fsi_nucl_corr_matrix;
    
    
    // FSI pion correlation matrix
    TMatrixD fsi_pion_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            fsi_pion_corr_matrix[x][y] = (fsi_pion_cov_matrix[x][x] == 0.0 || fsi_pion_cov_matrix[y][y] == 0.0 ) ?
                                         0.0 : fsi_pion_cov_matrix[x][y] / std::sqrt(fsi_pion_cov_matrix[x][x]*fsi_pion_cov_matrix[y][y]);
        }
    }
    TH2D* h_fsi_pion_corr_matrix = new TH2D(fsi_pion_corr_matrix);
    h_fsi_pion_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_fsi_pion_corr_matrix -> Write(Form("%s_FSIPionCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_fsi_pion_corr_matrix;
    
    
    // Muon reconstruction correlation matrix
    TMatrixD muon_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            muon_corr_matrix[x][y] = (muon_cov_matrix[x][x] == 0.0 || muon_cov_matrix[y][y] == 0.0 ) ?
                                     0.0 : muon_cov_matrix[x][y] / std::sqrt(muon_cov_matrix[x][x]*muon_cov_matrix[y][y]);
        }
    }
    TH2D* h_muon_corr_matrix = new TH2D(muon_corr_matrix);
    h_muon_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_muon_corr_matrix -> Write(Form("%s_MuonCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_muon_corr_matrix;
    
    
    // Particle response correlation matrix
    TMatrixD part_resp_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            part_resp_corr_matrix[x][y] = (part_resp_cov_matrix[x][x] == 0.0 || part_resp_cov_matrix[y][y] == 0.0 ) ?
                                          0.0 : part_resp_cov_matrix[x][y] / std::sqrt(part_resp_cov_matrix[x][x]*part_resp_cov_matrix[y][y]);
        }
    }
    TH2D* h_part_resp_corr_matrix = new TH2D(part_resp_corr_matrix);
    h_part_resp_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_part_resp_corr_matrix -> Write(Form("%s_PartRespCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_part_resp_corr_matrix;
    
    
    // Other correlation matrix
    TMatrixD other_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            other_corr_matrix[x][y] = (other_cov_matrix[x][x] == 0.0 || other_cov_matrix[y][y] == 0.0 ) ?
                                      0.0 : other_cov_matrix[x][y] / std::sqrt(other_cov_matrix[x][x]*other_cov_matrix[y][y]);
        }
    }
    TH2D* h_other_corr_matrix = new TH2D(other_corr_matrix);
    h_other_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_other_corr_matrix -> Write(Form("%s_OtherCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_other_corr_matrix;
    
    
    // Total syst. correlation matrix
    TMatrixD total_syst_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            total_syst_corr_matrix[x][y] = (total_syst_cov_matrix[x][x] == 0.0 || total_syst_cov_matrix[y][y] == 0.0 ) ?
                                           0.0 : total_syst_cov_matrix[x][y] / std::sqrt(total_syst_cov_matrix[x][x]*total_syst_cov_matrix[y][y]);
        }
    }
    TH2D* h_total_syst_corr_matrix = new TH2D(total_syst_corr_matrix);
    h_total_syst_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_total_syst_corr_matrix -> Write(Form("%s_TotalSystCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_syst_corr_matrix;
    
    
    // Total correlation matrix
    TMatrixD total_corr_matrix(size, size);
    for ( int x = 0; x < size; ++x ) {
        for ( int y = 0; y < size; ++y ) {
            total_corr_matrix[x][y] = (total_cov_matrix[x][x] == 0.0 || total_cov_matrix[y][y] == 0.0 ) ?
                                      0.0 : total_cov_matrix[x][y] / std::sqrt(total_cov_matrix[x][x]*total_cov_matrix[y][y]);
        }
    }
    TH2D* h_total_corr_matrix = new TH2D(total_corr_matrix);
    h_total_corr_matrix -> GetZaxis() -> SetRangeUser(-1.0, 1.0);
    h_total_corr_matrix -> Write(Form("%s_TotalCorrMatrix_%s", mc_data_str.c_str(), var_name.c_str()));
    delete h_total_corr_matrix;
}





// ========================================================================================================================
//  PRE-CROSS-SECTION HISTOGRAMS
// ========================================================================================================================

void GetPreXsectionHistos(CCPi0::MacroUtil util,
                          Variable* var,
                          TFile& mc_fin_notuned,
                          TFile& mc_fin_tuned,
                          TFile& data_fin,
                          TFile& fout_prexsec,
                          std::string option_material)
{
    std::cout << " \tPreparing pre-cross-section histograms... " << std::endl;
    std::cout << std::endl;
    
    
    // Get variable name
    std::string var_name = var->Name();
    
    
    // Define "dummy" variables
    std::vector<Variable*> dummy_variables = GetXsecVariables(false);  // Include true variables
    
    
    // Loop over "dummy" variables
    for ( auto dummy_var : dummy_variables )
    {
        std::string dummy_var_name = dummy_var->Name();
        
        
        if ( var_name == dummy_var_name )
        {
            // Load histograms
            // ===============
            
            // Load data
            PlotUtils::MnvH1D* h_data = (PlotUtils::MnvH1D*)data_fin.Get(Form("data_Selection_%s", dummy_var_name.c_str()));
            h_data -> SetName(Form("data_%s", var_name.c_str()));
            
            
            // Load signal
            PlotUtils::MnvH1D* h_mc_Signal = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_Signal", dummy_var_name.c_str()));
            h_mc_Signal -> SetName(Form("mc_%s_Signal", var_name.c_str()));
            
            
            // Load non-tuned MC
            PlotUtils::MnvH1D* h_mc_NonTuned                = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s",                dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned          = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_Backgr",         dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_Pi0HighW = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrPi0HighW", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_QElike   = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrQElike",   dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PionProd = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrPionProd", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasUp   = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrPlasUp",   dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasBetw = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrPlasBetw", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_PlasDown = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrPlasDown", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrNonTuned_Other    = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_Selection_%s_BackgrOther",    dummy_var_name.c_str()));
            
            h_mc_NonTuned                -> SetName(Form("mc_%s_NonTuned",                var_name.c_str()));
            h_mc_BackgrNonTuned          -> SetName(Form("mc_%s_BackgrNonTuned",          var_name.c_str()));
            h_mc_BackgrNonTuned_Pi0HighW -> SetName(Form("mc_%s_BackgrNonTuned_Pi0HighW", var_name.c_str()));
            h_mc_BackgrNonTuned_QElike   -> SetName(Form("mc_%s_BackgrNonTuned_QElike",   var_name.c_str()));
            h_mc_BackgrNonTuned_PionProd -> SetName(Form("mc_%s_BackgrNonTuned_PionProd", var_name.c_str()));
            h_mc_BackgrNonTuned_PlasUp   -> SetName(Form("mc_%s_BackgrNonTuned_PlasUp",   var_name.c_str()));
            h_mc_BackgrNonTuned_PlasBetw -> SetName(Form("mc_%s_BackgrNonTuned_PlasBetw", var_name.c_str()));
            h_mc_BackgrNonTuned_PlasDown -> SetName(Form("mc_%s_BackgrNonTuned_PlasDown", var_name.c_str()));
            h_mc_BackgrNonTuned_Other    -> SetName(Form("mc_%s_BackgrNonTuned_Other",    var_name.c_str()));
            
            
            // Load tuned MC
            PlotUtils::MnvH1D* h_mc_BackgrTuned_Pi0HighW = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrPi0HighW", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_QElike   = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrQElike",   dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_PionProd = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrPionProd", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasUp   = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrPlasUp",   dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasBetw = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrPlasBetw", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_PlasDown = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrPlasDown", dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_mc_BackgrTuned_Other    = (PlotUtils::MnvH1D*)mc_fin_tuned.Get(Form("mc_%s_SigReg_BackgrOther",    dummy_var_name.c_str()));
            
            h_mc_BackgrTuned_Pi0HighW -> SetName(Form("mc_%s_BackgrTuned_Pi0HighW", var_name.c_str()));
            h_mc_BackgrTuned_QElike   -> SetName(Form("mc_%s_BackgrTuned_QElike",   var_name.c_str()));
            h_mc_BackgrTuned_PionProd -> SetName(Form("mc_%s_BackgrTuned_PionProd", var_name.c_str()));
            h_mc_BackgrTuned_PlasUp   -> SetName(Form("mc_%s_BackgrTuned_PlasUp",   var_name.c_str()));
            h_mc_BackgrTuned_PlasBetw -> SetName(Form("mc_%s_BackgrTuned_PlasBetw", var_name.c_str()));
            h_mc_BackgrTuned_PlasDown -> SetName(Form("mc_%s_BackgrTuned_PlasDown", var_name.c_str()));
            h_mc_BackgrTuned_Other    -> SetName(Form("mc_%s_BackgrTuned_Other",    var_name.c_str()));
            
            PlotUtils::MnvH1D* h_mc_BackgrTuned = h_mc_BackgrTuned_Pi0HighW->Clone(Form("mc_%s_BackgrTuned", var_name.c_str()));
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_QElike);
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_PionProd);
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_PlasUp);
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_PlasBetw);
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_PlasDown);
            h_mc_BackgrTuned -> Add(h_mc_BackgrTuned_Other);
            
            PlotUtils::MnvH1D* h_mc_Tuned = h_mc_Signal->Clone(Form("mc_%s_Tuned", var_name.c_str()));
            h_mc_Tuned -> Add(h_mc_BackgrTuned);
            
            
            // Load efficiency
            PlotUtils::MnvH1D* h_EffNumerator   = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_EffNumerator_%s_True",   dummy_var_name.c_str()));
            PlotUtils::MnvH1D* h_EffDenominator = (PlotUtils::MnvH1D*)mc_fin_notuned.Get(Form("mc_EffDenominator_%s_True", dummy_var_name.c_str()));
            h_EffDenominator -> AddMissingErrorBandsAndFillWithCV(*h_EffNumerator);
            
            h_EffNumerator   -> SetName(Form("EffNumerator_%s",   var_name.c_str()));
            h_EffDenominator -> SetName(Form("EffDenominator_%s", var_name.c_str()));
            
            PlotUtils::MnvH1D* h_Efficiency = h_EffNumerator->Clone(Form("Efficiency_%s", var_name.c_str()));
            h_Efficiency -> Divide(h_EffNumerator, h_EffDenominator);
            
            
            // Load migration matrix
            PlotUtils::MnvH2D* h_MigrationMatrix = (PlotUtils::MnvH2D*)mc_fin_notuned.Get(Form("mc_Migration_%s", dummy_var_name.c_str()));
            h_MigrationMatrix -> SetName(Form("MigrationMatrix_%s", var_name.c_str()));
        
        
            // Assign histograms to variable and write them on file
            // ====================================================
            
            fout_prexsec.cd();
            
            // Signal
            var -> m_hists.m_mc_Signal = h_mc_Signal;
            var -> m_hists.m_mc_Signal -> Write();
            
            
            // Non-tuned MC
            var -> m_hists.m_mc_NonTuned                = h_mc_NonTuned;
            var -> m_hists.m_mc_BackgrNonTuned          = h_mc_BackgrNonTuned;
            var -> m_hists.m_mc_BackgrNonTuned_Pi0HighW = h_mc_BackgrNonTuned_Pi0HighW;
            var -> m_hists.m_mc_BackgrNonTuned_QElike   = h_mc_BackgrNonTuned_QElike;
            var -> m_hists.m_mc_BackgrNonTuned_PionProd = h_mc_BackgrNonTuned_PionProd;
            var -> m_hists.m_mc_BackgrNonTuned_PlasUp   = h_mc_BackgrNonTuned_PlasUp;
            var -> m_hists.m_mc_BackgrNonTuned_PlasBetw = h_mc_BackgrNonTuned_PlasBetw;
            var -> m_hists.m_mc_BackgrNonTuned_PlasDown = h_mc_BackgrNonTuned_PlasDown;
            var -> m_hists.m_mc_BackgrNonTuned_Other    = h_mc_BackgrNonTuned_Other;
            
            var -> m_hists.m_mc_NonTuned                -> Write();
            var -> m_hists.m_mc_BackgrNonTuned          -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_Pi0HighW -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_QElike   -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_PionProd -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_PlasUp   -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_PlasBetw -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_PlasDown -> Write();
            var -> m_hists.m_mc_BackgrNonTuned_Other    -> Write();
            
            
            // Tuned MC
            var -> m_hists.m_mc_Tuned                = h_mc_Tuned;
            var -> m_hists.m_mc_BackgrTuned          = h_mc_BackgrTuned;
            var -> m_hists.m_mc_BackgrTuned_Pi0HighW = h_mc_BackgrTuned_Pi0HighW;
            var -> m_hists.m_mc_BackgrTuned_QElike   = h_mc_BackgrTuned_QElike;
            var -> m_hists.m_mc_BackgrTuned_PionProd = h_mc_BackgrTuned_PionProd;
            var -> m_hists.m_mc_BackgrTuned_PlasUp   = h_mc_BackgrTuned_PlasUp;
            var -> m_hists.m_mc_BackgrTuned_PlasBetw = h_mc_BackgrTuned_PlasBetw;
            var -> m_hists.m_mc_BackgrTuned_PlasDown = h_mc_BackgrTuned_PlasDown;
            var -> m_hists.m_mc_BackgrTuned_Other    = h_mc_BackgrTuned_Other;
            
            var -> m_hists.m_mc_Tuned                -> Write();
            var -> m_hists.m_mc_BackgrTuned          -> Write();
            var -> m_hists.m_mc_BackgrTuned_Pi0HighW -> Write();
            var -> m_hists.m_mc_BackgrTuned_QElike   -> Write();
            var -> m_hists.m_mc_BackgrTuned_PionProd -> Write();
            var -> m_hists.m_mc_BackgrTuned_PlasUp   -> Write();
            var -> m_hists.m_mc_BackgrTuned_PlasBetw -> Write();
            var -> m_hists.m_mc_BackgrTuned_PlasDown -> Write();
            var -> m_hists.m_mc_BackgrTuned_Other    -> Write();
            
            
            // Data
            var -> m_hists.m_data = h_data;
            var -> m_hists.m_data -> Write();
            
            
            // Efficiency
            var -> m_hists.m_EffNumerator   = h_EffNumerator;
            var -> m_hists.m_EffDenominator = h_EffDenominator;
            var -> m_hists.m_Efficiency     = h_Efficiency;
            
            var -> m_hists.m_EffNumerator   -> Write();
            var -> m_hists.m_EffDenominator -> Write();
            var -> m_hists.m_Efficiency     -> Write();
            
            
            // Migration matrix
            var -> m_hists.m_MigrationMatrix = h_MigrationMatrix;
            var -> m_hists.m_MigrationMatrix -> Write();
            
            
            // Covariance and correlation matrices
            WriteCovAndCorrMatrices(var, h_mc_Tuned, fout_prexsec, true,  false);
            WriteCovAndCorrMatrices(var, h_data,     fout_prexsec, false, false);
        }
    }
}





// ========================================================================================================================
//  BACKGROUND SUBTRACTION HISTOGRAMS
// ========================================================================================================================

void GetBackgrSubtrHistos(CCPi0::MacroUtil util,
                          Variable* var,
                          TFile& fout_backgrsubtr,
                          std::string option_material)
{
    std::cout << " \tSubtracting background... " << std::endl;
    std::cout << std::endl;
    
    
    // Get variable name
    std::string var_name = var->Name();
    
    
    // Subtract background
    // ===================
    
    // Get MC and data histograms
    PlotUtils::MnvH1D* h_mc_BackgrSubtr   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_Signal->Clone(Form("mc_BackgrSubtr_%s", var_name.c_str()));
    PlotUtils::MnvH1D* h_data_BackgrSubtr = (PlotUtils::MnvH1D*)var->m_hists.m_data->Clone(Form("data_BackgrSubtr_%s",   var_name.c_str()));
    
    
    // Assign error bands to data histogram
    h_data_BackgrSubtr -> ClearAllErrorBands();
    h_data_BackgrSubtr -> AddMissingErrorBandsAndFillWithCV(*h_mc_BackgrSubtr);
    
    
    // Subtract tuned background from data
    h_data_BackgrSubtr -> Add(var->m_hists.m_mc_BackgrTuned, -1);
    
    
    // Write histograms on file
    // ========================
    
    fout_backgrsubtr.cd();
    
    var->m_hists.m_mc_BackgrSubtr   = h_mc_BackgrSubtr;
    var->m_hists.m_data_BackgrSubtr = h_data_BackgrSubtr;
    
    var->m_hists.m_mc_BackgrSubtr   -> Write();
    var->m_hists.m_data_BackgrSubtr -> Write();
    
    // Covariance and correlation matrices
    WriteCovAndCorrMatrices(var, h_mc_BackgrSubtr,   fout_backgrsubtr, true,  false);
    WriteCovAndCorrMatrices(var, h_data_BackgrSubtr, fout_backgrsubtr, false, false);
}





// ========================================================================================================================
//  UNFOLDING HISTOGRAMS
// ========================================================================================================================

void GetUnfoldingHistos(CCPi0::MacroUtil util,
                        Variable* var,
                        TFile& fout_unfolding,
                        int n_iterations,
                        double cov_uncfactor,
                        std::string option_material)
{
    // Get variable name
    std::string var_name = var->Name();
    
    
    // General unfolding parameters
    bool add_systematics = true;
    bool use_sys_variated_migrations = true;
    
    
    // Define folded and unfolded histograms
    PlotUtils::MnvH1D* h_mc_folded;
    PlotUtils::MnvH1D* h_mc_unfolded;
    
    PlotUtils::MnvH1D* h_data_folded;
    PlotUtils::MnvH1D* h_data_unfolded;
    
    CVHW cvhw_data_folded;  // To use only with standard unfolding
    
    
    // Initialize histograms for standard unfolding
    // ============================================
    
    std::cout << " \tInitializing histograms for standard unfolding... " << std::endl;
    std::cout << std::endl;
    
    // When unfolding MC, the folded distribution is background-subtracted MC,
    // and the unfolded distribution is a clone with their error bands cleared
    h_mc_folded = (PlotUtils::MnvH1D*)var->m_hists.m_mc_BackgrSubtr->Clone(Form("mc_Folded_%s", var_name.c_str()));
    
    h_mc_unfolded = (PlotUtils::MnvH1D*)var->m_hists.m_mc_BackgrSubtr->Clone(Form("mc_Unfolded_%s", var_name.c_str()));
    h_mc_unfolded -> ClearAllErrorBands();
    h_mc_unfolded -> Reset();
    
    // When unfolding data, the folded distribution is background-subtracted data,
    // but it's necessary to make sure that the bin content of those never go negative
    h_data_folded = (PlotUtils::MnvH1D*)var->m_hists.m_data_BackgrSubtr->Clone(Form("data_Folded_%s", var_name.c_str()));
    
    // This is a quite convoluted process, but first need to define a HistWrapper from the MnvH1D*
    cvhw_data_folded = CVHW(h_data_folded, util.m_error_bands, false);  // Don't clear error bands
    
    // Loop over all error bands
    for ( auto error_band : util.m_error_bands )
    {
        // Get universes
        std::vector<CVUniverse*> universes = error_band.second;
        
        // Loop over all universes
        for ( auto universe : universes )
        {
            // Loop over histogram bins
            for ( int bin = 0; bin <= cvhw_data_folded.univHist(universe)->GetNbinsX()+1; ++bin )
            {
                double bin_content = cvhw_data_folded.univHist(universe)->GetBinContent(bin);
                double bin_error   = cvhw_data_folded.univHist(universe)->GetBinError(bin);
                
                // If bin content is negative, set it as zero, but keep the error same as before
                if ( bin_content < 0.0 ) {
                    cvhw_data_folded.univHist(universe)->SetBinContent(bin, 0.0);
                    cvhw_data_folded.univHist(universe)->SetBinError(bin, bin_error);
                }
            }  // End of loop over bins
            
        }  // End of loop over universes
        
    }  // End of loop over error bands
    
    // The data unfolded distribution is a clone of the data folded with their error bands cleared
    h_data_unfolded = (PlotUtils::MnvH1D*)var->m_hists.m_data_BackgrSubtr->Clone(Form("data_Unfolded_%s", var_name.c_str()));
    h_data_unfolded -> ClearAllErrorBands();
    h_data_unfolded -> Reset();
    
    var->m_hists.m_mc_Folded   = h_mc_folded;
    var->m_hists.m_data_Folded = cvhw_data_folded.hist;
    
    
    // // Initialize histograms for unfolding with fakes
    // // ==============================================
    
    // if ( use_unfold_fakes )
    // {
    //     std::cout << " \tInitializing histograms for unfolding with fakes... " << std::endl;
    //     std::cout << std::endl;
        
    //     // When unfolding MC, the folded distribution is MC WITHOUT background subtraction,
    //     // and the unfolded distribution is a clone of it
    //     h_mc_folded   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_Tuned->Clone(Form("mc_Folded_%s",   var_name.c_str()));
    //     h_mc_unfolded = (PlotUtils::MnvH1D*)var->m_hists.m_mc_Tuned->Clone(Form("mc_Unfolded_%s", var_name.c_str()));
        
    //     // When unfolding data, the folded distribution is data WITHOUT background subtraction,
    //     // and the unfolded distribution is a clone of it.
    //     // Error bars should be added for both distributions and being filled with the CV
    //     h_data_folded   = (PlotUtils::MnvH1D*)var->m_hists.m_data->Clone(Form("data_Folded_%s",   var_name.c_str()));
    //     h_data_unfolded = (PlotUtils::MnvH1D*)var->m_hists.m_data->Clone(Form("data_Unfolded_%s", var_name.c_str()));
        
    //     h_data_folded   -> ClearAllErrorBands();
    //     h_data_unfolded -> ClearAllErrorBands();
        
    //     h_data_folded   -> AddMissingErrorBandsAndFillWithCV(*h_mc_folded);
    //     h_data_unfolded -> AddMissingErrorBandsAndFillWithCV(*h_mc_unfolded);
        
    //     var->m_hists.m_mc_Folded   = h_mc_folded;
    //     var->m_hists.m_data_Folded = h_data_folded;
    // }
    
    
    // Perform unfolding
    // =================
    
    std::cout << " \tPerforming standard unfolding... " << std::endl;
    std::cout << std::endl;
    
    UnfoldingStandard(var, h_mc_unfolded,   h_mc_folded,           n_iterations, add_systematics, use_sys_variated_migrations, cov_uncfactor);
    UnfoldingStandard(var, h_data_unfolded, cvhw_data_folded.hist, n_iterations, add_systematics, use_sys_variated_migrations, cov_uncfactor);
    
    var->m_hists.m_mc_Unfolded   = h_mc_unfolded;
    var->m_hists.m_data_Unfolded = h_data_unfolded;
    
    // if ( use_unfold_fakes ) {
    //     std::cout << " \tPerforming unfolding with fakes... " << std::endl;
    //     std::cout << std::endl;
        
    //     UnfoldingWithFakes(var, h_mc_unfolded,   h_mc_folded,   n_iterations, add_systematics, use_sys_variated_migrations);
    //     UnfoldingWithFakes(var, h_data_unfolded, h_data_folded, n_iterations, add_systematics, use_sys_variated_migrations);
    // }
    
    // Unfolding internal closure
    PlotUtils::MnvH1D* h_mc_UnfoldClosure = (PlotUtils::MnvH1D*)h_mc_unfolded->Clone(Form("mc_UnfoldClosure_%s", var_name.c_str()));
    PlotUtils::MnvH1D* h_EffNumerator     = (PlotUtils::MnvH1D*)var->m_hists.m_EffNumerator->Clone("");
    
    h_EffNumerator     -> AddMissingErrorBandsAndFillWithCV(*h_mc_UnfoldClosure);
    h_mc_UnfoldClosure -> Divide(h_mc_UnfoldClosure, h_EffNumerator);
    
    double max_diff = 0.0;
    for ( int bin = 1; bin <= h_mc_UnfoldClosure->GetNbinsX(); ++bin ) {
        double bin_content = h_mc_UnfoldClosure->GetBinContent(bin);
        bin_content = 1.0 - bin_content;
        h_mc_UnfoldClosure->SetBinContent(bin, bin_content);
        
        if ( std::fabs(bin_content) > std::fabs(max_diff) ) max_diff = std::fabs(bin_content);
    }
    h_mc_UnfoldClosure -> SetMaximum(2.0*max_diff);
    h_mc_UnfoldClosure -> SetMinimum(-2.0*max_diff);
    
    
    // Write distributions to file
    // ===========================
    
    fout_unfolding.cd();
    
    var->m_hists.m_mc_Folded   -> Write();
    var->m_hists.m_data_Folded -> Write();
    
    var->m_hists.m_mc_Unfolded   -> Write();
    var->m_hists.m_data_Unfolded -> Write();
    
    h_mc_UnfoldClosure -> Write();
    
    WriteCovAndCorrMatrices(var, h_mc_unfolded,   fout_unfolding, true , false);
    WriteCovAndCorrMatrices(var, h_data_unfolded, fout_unfolding, false, false);
}





// ========================================================================================================================
//  EFFICIENCY CORRECTION HISTOGRAMS
// ========================================================================================================================

void GetEffCorrectedHistos(CCPi0::MacroUtil util,
                           Variable* var,
                           TFile& fout_effcorr,
                           std::string option_material)
{
    std::cout << " \tCorrecting for efficiency... " << std::endl;
    std::cout << std::endl;
    
    
    // Get variable name
    std::string var_name = var->Name();
    
    
    // Correct by efficiency
    // =====================
    
    PlotUtils::MnvH1D* h_mc_EffCorrected   = var->m_hists.m_mc_Unfolded->Clone(Form("mc_EffCorrected_%s",     var_name.c_str()));
    PlotUtils::MnvH1D* h_data_EffCorrected = var->m_hists.m_data_Unfolded->Clone(Form("data_EffCorrected_%s", var_name.c_str()));
    
    h_mc_EffCorrected   -> Divide(h_mc_EffCorrected,   var->m_hists.m_Efficiency);
    h_data_EffCorrected -> Divide(h_data_EffCorrected, var->m_hists.m_Efficiency);
    
    // Efficiency correction internal closure
    PlotUtils::MnvH1D* h_mc_EffCorrClosure = (PlotUtils::MnvH1D*)h_mc_EffCorrected->Clone(Form("mc_EffCorrClosure_%s", var_name.c_str()));
    PlotUtils::MnvH1D* h_EffDenominator    = (PlotUtils::MnvH1D*)var->m_hists.m_EffDenominator->Clone("");
    
    h_EffDenominator    -> AddMissingErrorBandsAndFillWithCV(*h_mc_EffCorrClosure);
    h_mc_EffCorrClosure -> Divide(h_mc_EffCorrClosure, h_EffDenominator);
    
    double max_diff = 0.0;
    for ( int bin = 1; bin <= h_mc_EffCorrClosure->GetNbinsX(); ++bin ) {
        double bin_content = h_mc_EffCorrClosure->GetBinContent(bin);
        bin_content = 1.0 - bin_content;
        h_mc_EffCorrClosure->SetBinContent(bin, bin_content);
        
        if ( std::fabs(bin_content) > std::fabs(max_diff) ) max_diff = std::fabs(bin_content);
    }
    h_mc_EffCorrClosure -> SetMaximum(2.0*max_diff);
    h_mc_EffCorrClosure -> SetMinimum(-2.0*max_diff);
    
    
    // Write distributions to file
    // ===========================
    
    fout_effcorr.cd();
    
    var->m_hists.m_mc_EffCorrected   = h_mc_EffCorrected;
    var->m_hists.m_data_EffCorrected = h_data_EffCorrected;
    
    var->m_hists.m_mc_EffCorrected   -> Write();
    var->m_hists.m_data_EffCorrected -> Write();
    
    h_mc_EffCorrClosure -> Write();
    
    WriteCovAndCorrMatrices(var, h_mc_EffCorrected,   fout_effcorr, true,  false);
    WriteCovAndCorrMatrices(var, h_data_EffCorrected, fout_effcorr, false, false);
}





// ========================================================================================================================
//  CROSS-SECTION HISTOGRAMS
// ========================================================================================================================

void GetXsectionHistos(CCPi0::MacroUtil util,
                       Variable* var,
                       TFile& fout_xsec,
                       std::string option_material,
                       std::ofstream& flux_text,
                       std::ofstream& target_text)
{
    std::cout << " \tNormalizing by flux and targets... " << std::endl;
    std::cout << std::endl;
    
    
    // Get variable name
    std::string var_name = var->Name();
    
    
    // Set decimal precision
    flux_text   << std::setprecision(3) << std::scientific;
    target_text << std::setprecision(3) << std::scientific;
    
    
    // Initialize cross-section histograms from eff. corr. histograms
    PlotUtils::MnvH1D* h_mc_CrossSection   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone(Form("mc_CrossSection_%s",     var_name.c_str()));
    PlotUtils::MnvH1D* h_data_CrossSection = (PlotUtils::MnvH1D*)var->m_hists.m_data_EffCorrected->Clone(Form("data_CrossSection_%s", var_name.c_str()));
    
    PlotUtils::MnvH1D* h_mc_CrossSection_oldFlux   = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone(Form("mc_CrossSection_OldFlux_%s",     var_name.c_str()));
    PlotUtils::MnvH1D* h_data_CrossSection_oldFlux = (PlotUtils::MnvH1D*)var->m_hists.m_data_EffCorrected->Clone(Form("data_CrossSection_OldFlux_%s", var_name.c_str()));
    
    
    // FluxReweighter ingredients
    const std::string playlist = "minervame1d1m1nweightedave";
    const int nu_pdg = 14;
    const bool use_nue_constraint = CCPi0AnaConstants::kUseNueConstraint;
    const int Nflux_universes = CCPi0AnaConstants::kNFluxUniverses;
    
    const double min_energy = 0.0;
    const double max_energy = 100.0;
    const bool use_muon_correlations = true;
    const std::string project_dir = "targets_2345_temp";
    
    
    // Get flux histograms (units: neutrinos/m2/GeV/POT)
    // =================================================
    
    // Lead
    PlotUtils::MnvH1D* h_flux_lead = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).  // Full constraint
                                     GetTargetFluxMnvH1D(nu_pdg, "lead", project_dir, true);
    
    PlotUtils::MnvH1D* h_flux_old_lead = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).  // FHC nu-e constraint only
                                         GetTargetFluxMnvH1D(nu_pdg, "lead", project_dir, false);
    
    h_flux_lead     -> SetName("Flux");
    h_flux_old_lead -> SetName("Flux_Old");
    
    
    // Iron
    PlotUtils::MnvH1D* h_flux_iron = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                                     GetTargetFluxMnvH1D(nu_pdg, "iron", project_dir);
    
    PlotUtils::MnvH1D* h_flux_old_iron = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                                         GetTargetFluxMnvH1D(nu_pdg, "iron", project_dir, false);
    
    h_flux_iron     -> SetName("Flux");
    h_flux_old_iron -> SetName("Flux_Old");
    
    
    // Define flux ratio histograms
    PlotUtils::MnvH1D* h_flux_ratio_lead = (PlotUtils::MnvH1D*)h_flux_lead->Clone("");
    h_flux_ratio_lead -> SetName("FluxRatio");
    h_flux_ratio_lead -> Divide(h_flux_lead, h_flux_old_lead);
    
    PlotUtils::MnvH1D* h_flux_ratio_iron = (PlotUtils::MnvH1D*)h_flux_iron->Clone("");
    h_flux_ratio_iron -> SetName("FluxRatio");
    h_flux_ratio_iron -> Divide(h_flux_iron, h_flux_old_iron);
    
    
    // Define total flux histograms (units: neutrinos/cm2/GeV)
    // =======================================================
    
    PlotUtils::MnvH1D* h_flux_total_lead = (PlotUtils::MnvH1D*)h_flux_lead->Clone("");
    h_flux_total_lead -> SetName("FluxTotal");
    h_flux_total_lead -> Scale(util.m_data_pot * 1.0e-4);
    
    PlotUtils::MnvH1D* h_flux_total_old_lead = (PlotUtils::MnvH1D*)h_flux_old_lead->Clone("");
    h_flux_total_old_lead -> SetName("FluxTotal_Old");
    h_flux_total_old_lead -> Scale(util.m_data_pot * 1.0e-4);
    
    PlotUtils::MnvH1D* h_flux_total_iron = (PlotUtils::MnvH1D*)h_flux_iron->Clone("");
    h_flux_total_iron -> SetName("FluxTotal");
    h_flux_total_iron -> Scale(util.m_data_pot * 1.0e-4);
    
    PlotUtils::MnvH1D* h_flux_total_old_iron = (PlotUtils::MnvH1D*)h_flux_old_iron->Clone("");
    h_flux_total_old_iron -> SetName("FluxTotal_Old");
    h_flux_total_old_iron -> Scale(util.m_data_pot * 1.0e-4);
    
    
    // Get integrated flux histograms (units: neutrinos/m2/POT)
    // ========================================================
    
    // Initialize integrated flux histograms from efficiency-corrected
    PlotUtils::MnvH1D* h_integrflux_lead = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone("");
    h_integrflux_lead -> ClearAllErrorBands();
    h_integrflux_lead -> Reset();
    
    PlotUtils::MnvH1D* h_integrflux_old_lead = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone("");
    h_integrflux_old_lead -> ClearAllErrorBands();
    h_integrflux_old_lead -> Reset();
    
    PlotUtils::MnvH1D* h_integrflux_iron = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone("");
    h_integrflux_iron -> ClearAllErrorBands();
    h_integrflux_iron -> Reset();
    
    PlotUtils::MnvH1D* h_integrflux_old_iron = (PlotUtils::MnvH1D*)var->m_hists.m_mc_EffCorrected->Clone("");
    h_integrflux_old_iron -> ClearAllErrorBands();
    h_integrflux_old_iron -> Reset();
    
    
    // Lead
    h_integrflux_lead = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                        GetIntegratedTargetFlux(nu_pdg, "lead", h_mc_CrossSection, min_energy, max_energy, project_dir, true);
    
    h_integrflux_old_lead = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                            GetIntegratedTargetFlux(nu_pdg, "lead", h_mc_CrossSection_oldFlux, min_energy, max_energy, project_dir, false);
    
    
    // Iron
    h_integrflux_iron = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                        GetIntegratedTargetFlux(nu_pdg, "iron", h_mc_CrossSection, min_energy, max_energy, project_dir, true);
    
    h_integrflux_old_iron = PlotUtils::flux_reweighter(playlist, nu_pdg, use_nue_constraint, Nflux_universes).
                            GetIntegratedTargetFlux(nu_pdg, "iron", h_mc_CrossSection_oldFlux, min_energy, max_energy, project_dir, false);
    
    
    // Convert flux units: neutrinos/m2/POT -> neutrinos/cm2/POT
    h_integrflux_lead     -> Scale(1.0e-4);
    h_integrflux_old_lead -> Scale(1.0e-4);
    
    h_integrflux_iron     -> Scale(1.0e-4);
    h_integrflux_old_iron -> Scale(1.0e-4);
    
    
    // Write integrated flux info to text file
    double integrflux     = 0.0;
    double integrflux_old = 0.0;
    
    if ( option_material == "lead" ) {
        flux_text << std::endl;
        flux_text << " =========================== " << std::endl;
        flux_text << "  LEAD INTEGRATED FLUX INFO  " << std::endl;
        flux_text << " =========================== " << std::endl;
        flux_text << std::endl;
        
        integrflux     = h_integrflux_lead->GetBinContent(1);
        integrflux_old = h_integrflux_old_lead->GetBinContent(1);
    }
    
    else if ( option_material == "iron" )
    {
        flux_text << std::endl;
        flux_text << " =========================== " << std::endl;
        flux_text << "  IRON INTEGRATED FLUX INFO  " << std::endl;
        flux_text << " =========================== " << std::endl;
        flux_text << std::endl;
        
        integrflux     = h_integrflux_iron->GetBinContent(1);
        integrflux_old = h_integrflux_old_iron->GetBinContent(1);
    }
    
    flux_text << " Full constrained (used in this analysis): " << integrflux << " nu/cm2/POT " << std::endl;
    flux_text << std::endl;
    flux_text << " Aaron's flux (only FHC nu-e constraint):  " << integrflux_old << " nu/cm2/POT " << std::endl;
    flux_text << std::endl;
    flux_text.close();
    
    
    // Nucleon targets
    // ===============
    
    // Get number of nucleons and target mass
    double mc_Nnucleons   = 0.0;
    double data_Nnucleons = 0.0;
    
    if ( option_material == "lead" ) {
        mc_Nnucleons += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(4, 82, true, CCPi0AnaConstants::kApothem);
        mc_Nnucleons += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(5, 82, true, CCPi0AnaConstants::kApothem);
        
        data_Nnucleons += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(4, 82, false, CCPi0AnaConstants::kApothem);
        data_Nnucleons += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(5, 82, false, CCPi0AnaConstants::kApothem);
    }
    
    else if ( option_material == "iron" ) {
        mc_Nnucleons   += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(5, 26, true,  CCPi0AnaConstants::kApothem);
        data_Nnucleons += PlotUtils::TargetUtils::Get().GetPassiveTargetNNucleons(5, 26, false, CCPi0AnaConstants::kApothem);
    }
    
    
    // Save target info in histograms
    PlotUtils::MnvH1D* h_mc_Nnucleons   = new PlotUtils::MnvH1D("mc_Nnucleons", "mc_Nnucleons",   1, 0., 1.);
    PlotUtils::MnvH1D* h_data_Nnucleons = new PlotUtils::MnvH1D("data_Nnucleons", "data_Nnucleons", 1, 0., 1.);
    
    h_mc_Nnucleons   -> Fill(0.5, mc_Nnucleons);
    h_data_Nnucleons -> Fill(0.5, data_Nnucleons);
    
    
    // Save target info in text file
    if ( option_material == "lead" ) {
        target_text << std::endl;
        target_text << " ================== " << std::endl;
        target_text << "  LEAD TARGET INFO  " << std::endl;
        target_text << " ================== " << std::endl;
        target_text << std::endl;
    }
    
    else if ( option_material == "iron" ) {
        target_text << std::endl;
        target_text << " ================== " << std::endl;
        target_text << "  IRON TARGET INFO  " << std::endl;
        target_text << " ================== " << std::endl;
        target_text << std::endl;
    }
    
    target_text << " Number of nucleons: " << std::endl;
    target_text << " ------------------  " << std::endl;
    target_text << " \tMC:   " << mc_Nnucleons   << std::endl;
    target_text << " \tData: " << data_Nnucleons << std::endl;
    target_text << std::endl;
    target_text.close();
    
    
    // Get cross section histograms and normalize
    // ==========================================
    
    // Divide by flux
    if ( option_material == "lead" ) {
        h_mc_CrossSection   -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_lead);
        h_data_CrossSection -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_lead);
        
        h_mc_CrossSection   -> Divide(h_mc_CrossSection,   h_integrflux_lead);
        h_data_CrossSection -> Divide(h_data_CrossSection, h_integrflux_lead);
        
        h_mc_CrossSection_oldFlux   -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_old_lead);
        h_data_CrossSection_oldFlux -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_old_lead);
        
        h_mc_CrossSection_oldFlux   -> Divide(h_mc_CrossSection_oldFlux,   h_integrflux_old_lead);
        h_data_CrossSection_oldFlux -> Divide(h_data_CrossSection_oldFlux, h_integrflux_old_lead);
    }
    
    else if ( option_material == "iron" ) {
        h_mc_CrossSection   -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_iron);
        h_data_CrossSection -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_iron);
        
        h_mc_CrossSection   -> Divide(h_mc_CrossSection,   h_integrflux_iron);
        h_data_CrossSection -> Divide(h_data_CrossSection, h_integrflux_iron);
        
        h_mc_CrossSection_oldFlux   -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_old_iron);
        h_data_CrossSection_oldFlux -> AddMissingErrorBandsAndFillWithCV(*h_integrflux_old_iron);
        
        h_mc_CrossSection_oldFlux   -> Divide(h_mc_CrossSection_oldFlux,   h_integrflux_old_iron);
        h_data_CrossSection_oldFlux -> Divide(h_data_CrossSection_oldFlux, h_integrflux_old_iron);
    }
    
    
    // Divide by number of targets and data POT
    h_mc_CrossSection   -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
    h_data_CrossSection -> Scale(1.0 / (data_Nnucleons * util.m_data_pot));
    
    h_mc_CrossSection_oldFlux   -> Scale(1.0 / (mc_Nnucleons * util.m_data_pot));
    h_data_CrossSection_oldFlux -> Scale(1.0 / (data_Nnucleons * util.m_data_pot));
    
    
    // Write histograms to file
    // ========================
    
    fout_xsec.cd();
    
    // Cross-section
    var->m_hists.m_mc_CrossSection   = h_mc_CrossSection;
    var->m_hists.m_data_CrossSection = h_data_CrossSection;
    
    var->m_hists.m_mc_CrossSection   -> Write();
    var->m_hists.m_data_CrossSection -> Write();
    
    h_mc_CrossSection_oldFlux   -> Write(Form("mc_CrossSection_OldFlux_%s", var_name.c_str()));
    h_data_CrossSection_oldFlux -> Write(Form("data_CrossSection_OldFlux_%s", var_name.c_str()));
    
    // Flux
    if ( option_material == "lead" ) {
        h_flux_lead           -> Write();
        h_flux_old_lead       -> Write();
        h_flux_total_lead     -> Write();
        h_flux_total_old_lead -> Write();
        h_flux_ratio_lead     -> Write();
        h_integrflux_lead     -> Write("IntegratedFlux");
        h_integrflux_old_lead -> Write("IntegratedFlux_Old");
    }
    else if ( option_material == "iron" ) {
        h_flux_iron           -> Write();
        h_flux_old_iron       -> Write();
        h_flux_total_iron     -> Write();
        h_flux_total_old_iron -> Write();
        h_flux_ratio_iron     -> Write();
        h_integrflux_iron     -> Write("IntegratedFlux");
        h_integrflux_old_iron -> Write("IntegratedFlux_Old");
    }
    
    // Number of targets
    h_mc_Nnucleons   -> Write();
    h_data_Nnucleons -> Write();
    
    // Covariance and correlation matrices
    WriteCovAndCorrMatrices(var, h_mc_CrossSection,   fout_xsec, true,  true);
    WriteCovAndCorrMatrices(var, h_data_CrossSection, fout_xsec, false, true);
}





// ================================================================================================================================================================
// 
//  ********** MAIN FUNCTION **********
// 
// ================================================================================================================================================================

void CrossSectionExtraction(std::string option_date,
                            std::string option_model,
                            std::string option_material,
                            std::string option_bkg_fit_function = "Bilinear",
                            bool do_unfolding                   = true,
                            int n_iterations                    = 2)
{
    // Get MC model based on input
    EnumModels type_model;
    GetModel(option_model, type_model);
    
    
    // Options for input/output files
    const std::string option_date_mc   = option_date + "_" + option_model;
    const std::string option_date_data = option_date;
    
    
    
    // =========================================
    //  Input files
    // =========================================
    
    // Top directories
    // ===============
    
    // Non-tuned MC, efficiency components, and migration
    std::string mc_fin_notuned_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/mc/%s/%s", option_date_mc.c_str(),
                                                                                                                   option_material.c_str());
    
    // Tuned MC
    std::string mc_fin_tuned_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/PhysicsTuning/mc/%s/%s/AfterTuning", option_date_mc.c_str(),
                                                                                                                            option_material.c_str());
    
    // Data
    std::string data_fin_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/EventSelection/data/%s/%s", option_date_data.c_str(),
                                                                                                               option_material.c_str());
    
    
    // Define input files
    // ==================
    
    // Non-tuned MC, efficiency components, and migration
    TFile mc_fin_notuned(Form("%s/MC_EventSelection_MnvGENIE%s_WithSyst_POTScaled_AllPlaylists_%s.root", mc_fin_notuned_topdir.c_str(),
                                                                                                         option_model.c_str(),
                                                                                                         option_material.c_str()), "READ");
    
    // Tuned MC
    TFile mc_fin_tuned(Form("%s/MC_AfterPhysicsTuning_MnvGENIE%s_WithSyst_POTScaled_AllPlaylists_%sFit_%s.root", mc_fin_tuned_topdir.c_str(),
                                                                                                                 option_model.c_str(),
                                                                                                                 option_bkg_fit_function.c_str(),
                                                                                                                 option_material.c_str()), "READ");
    
    // Data
    TFile data_fin(Form("%s/Data_EventSelection_AllPlaylists_%s.root", data_fin_topdir.c_str(),
                                                                       option_material.c_str()), "READ");
    
    
    
    // =========================================
    //  MacroUtil and variables
    // =========================================
    
    // Playlist string
    // (Use only to load MC chain and access systematics)
    const std::string plist_string = "minervame1A";
    
    
    // Set playlists MC and data input
    // (Similarly, only to load MC and access systematics)
    const std::string mc_file_list   = GetPlaylistFile(true,  plist_string, "test");
    const std::string data_file_list = GetPlaylistFile(false, plist_string, "test");
    
    
    // Set MacroUtil
    // (SYSTEMATICS and TRUTH options set as 'true')
    CCPi0::MacroUtil util(mc_file_list, data_file_list, plist_string, true, true, type_model);
    
    
    // Set MacroUtil POT
    double mc_pot_notuned = GetPOT(mc_fin_notuned, true);
    double mc_pot_tuned   = GetPOT(mc_fin_tuned,   true);
    double data_pot       = GetPOT(data_fin,       false);
    
    if ( std::fabs(mc_pot_notuned - mc_pot_tuned) > std::numeric_limits<double>::epsilon() ) {
        std::cout << std::endl;
        std::cout << " WARNING: MC POT before and after tuning are not the same. Setting MacroUtil POT using MC tuned file. " << std::endl;
        std::cout << std::endl;
    }
    
    SetMacroUtilPOT(mc_fin_notuned, data_fin, util);
    
    std::cout << std::endl;
    std::cout << " \tMC POT:   " << util.m_mc_pot   << std::endl;
    std::cout << " \tData POT: " << util.m_data_pot << std::endl;
    std::cout << std::endl;
    
    
    // Get variables
    std::vector<Variable*> variables = GetXsecVariables(false);  // Don't include true variables
    
    TH1::AddDirectory(false);
    TH2::AddDirectory(false);
    
    
    
    // =========================================
    //  Do steps before unfolding
    // =========================================
    
    // Output files
    // ============
    
    // Top directory
    std::string fout_topdir = Form("/pnfs/minerva/persistent/users/gonzalo/MAT/CrossSectionExtraction/%s/%s", option_date_mc.c_str(),
                                                                                                              option_material.c_str());
    
    // Pre-cross-section extraction output
    TFile fout_prexsec(Form("%s/PreCrossSectionExtraction_MnvGENIE%s_Bkg%sFit_%s.root", fout_topdir.c_str(),
                                                                                        option_model.c_str(),
                                                                                        option_bkg_fit_function.c_str(),
                                                                                        option_material.c_str()), "RECREATE");
    
    // Background subtraction output
    TFile fout_backgrsubtr(Form("%s/BackgroundSubtraction_MnvGENIE%s_Bkg%sFit_%s.root", fout_topdir.c_str(),
                                                                                        option_model.c_str(),
                                                                                        option_bkg_fit_function.c_str(),
                                                                                        option_material.c_str()), "RECREATE");
    
    
    // Write POT to output files
    WritePOT(fout_prexsec,     util.m_mc_pot, util.m_data_pot);
    WritePOT(fout_backgrsubtr, util.m_mc_pot, util.m_data_pot);
    
    
    // Do cross section steps
    // ======================
    
    std::cout << " Performing cross-section extraction steps... " << std:: endl;
    std::cout << std::endl;
    
    for ( auto var : variables )
    {
        // Get all pre-cross-section distributions
        GetPreXsectionHistos(util, var, mc_fin_notuned, mc_fin_tuned, data_fin, fout_prexsec, option_material);
        
        // Get background-subtracted distributions
        GetBackgrSubtrHistos(util, var, fout_backgrsubtr, option_material);
    }
    
    
    // Close output files
    fout_prexsec.Close();
    fout_backgrsubtr.Close();
    
    
    // Exit function if requested
    if ( !do_unfolding ) return;
    
    
    
    // =========================================
    //  Do unfolding and following steps
    // =========================================
    
    // Output files
    // ============
    
    // Unfolding output
    TFile fout_unfolding(Form("%s/Unfolding_MnvGENIE%s_Bkg%sFit_%s.root", fout_topdir.c_str(),
                                                                          option_model.c_str(),
                                                                          option_bkg_fit_function.c_str(),
                                                                          option_material.c_str()), "RECREATE");
    
    // Efficiency correction output
    TFile fout_effcorr(Form("%s/EfficiencyCorrection_MnvGENIE%s_Bkg%sFit_%s.root", fout_topdir.c_str(),
                                                                                   option_model.c_str(),
                                                                                   option_bkg_fit_function.c_str(),
                                                                                   option_material.c_str()), "RECREATE");
    
    // Cross-section output
    TFile fout_xsec(Form("%s/CrossSection_MnvGENIE%s_Bkg%sFit_%s.root", fout_topdir.c_str(),
                                                                        option_model.c_str(),
                                                                        option_bkg_fit_function.c_str(),
                                                                        option_material.c_str()), "RECREATE");
    
    
    // Write POT to output files
    WritePOT(fout_unfolding, util.m_mc_pot, util.m_data_pot);
    WritePOT(fout_effcorr,   util.m_mc_pot, util.m_data_pot);
    WritePOT(fout_xsec,      util.m_mc_pot, util.m_data_pot);
    
    
    // Extraction info text files
    std::string text_topdir = Form("/minerva/data/users/gonzalo/MAT/ExtractionInfo/%s/%s", option_date_mc.c_str(),
                                                                                           option_material.c_str());
    
    std::ofstream flux_text(Form("%s/FluxInfo_%s.txt", text_topdir.c_str(),
                                                       option_material.c_str()));
    
    std::ofstream target_text(Form("%s/TargetInfo_%s.txt", text_topdir.c_str(),
                                                           option_material.c_str()));
    
    
    // Do cross section steps
    // ======================
    
    for ( auto var : variables )
    {
        // Determine factor used to modify covariance matrix:
        // [1 + 1/cov_uncfactor]
        double cov_uncfactor = 1.0;
        
        if ( option_material == "lead" ) {
            cov_uncfactor = 8.2;
        }
        else if ( option_material == "iron" ) {
            cov_uncfactor = 3.3;
        }
        else {
            std::cout << " ERROR: PICK RIGHT MATERIAL!! " << std::endl;
            exit(1);
        }
        
        // Get unfolded distributions
        GetUnfoldingHistos(util, var, fout_unfolding, n_iterations, cov_uncfactor, option_material);
        
        // Get efficiency-corrected histograms
        GetEffCorrectedHistos(util, var, fout_effcorr, option_material);
        
        // Get cross-section histograms
        GetXsectionHistos(util, var, fout_xsec, option_material, flux_text, target_text);
    }
    
    
    // Close ROOT files
    fout_unfolding.Close();
    fout_effcorr.Close();
    fout_xsec.Close();
}


#endif  // CrossSectionExtraction_C