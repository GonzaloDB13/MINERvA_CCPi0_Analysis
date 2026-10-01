#ifndef plotting_functions_finalversion_h
#define plotting_functions_finalversion_h

#ifndef __CINT__
#include <iostream>
#include <vector>

#include "CVUniverse.h"
#include "MacroUtil.h"
#include "CCPi0Event.h"
#include "Variable.h"
#include "TruthMatching.h"
#include "Constants.h"
#include "Binning.h"
#include "GetVariables.h"
#include "common_functions.h"
#include "util.h"
#include "myPlotStyle.h"

#include "PlotUtils/MnvColors.h"
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"
#include "PlotUtils/MnvPlotter.h"
#include "PlotUtils/MnvVertErrorBand.h"

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TList.h"
#include "TPad.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TText.h"
#endif  // __CINT__



// Forward-define Variable class
class Variable;





// ================================================================================================================================================================
//  PLOT INFO CLASS
// ================================================================================================================================================================

class PlotInfo
{
    public :
        
        // Constructor with variable
        // =========================
        
        PlotInfo(Variable* variable,
                 double mc_pot, double data_pot,
                 bool do_frac_uncertainty,
                 bool do_cov_area_norm,
                 bool include_stat_error,
                 bool do_bin_width_norm,
                 std::string print_format)
        :
        m_mnv_plotter(kCCPi0AnaStyle),
        m_variable(variable),
        m_mc_pot(mc_pot),
        m_data_pot(data_pot),
        m_mc_pot_scale(1.0),
        m_do_frac_uncertainty(do_frac_uncertainty),
        m_do_cov_area_norm(do_cov_area_norm),
        m_include_stat_error(include_stat_error),
        m_print_format(print_format)
        {
            myPlotStyle();  // Implemented in 'includes/myPlotStyle.h'
            m_mnv_plotter.draw_normalized_to_bin_width = do_bin_width_norm;
        }
        
        
        // Constructor without variable
        // ============================
        
        PlotInfo(double mc_pot, double data_pot,
                 bool do_frac_uncertainty,
                 bool do_cov_area_norm,
                 bool include_stat_error,
                 bool do_bin_width_norm,
                 std::string print_format)
        :
        m_mnv_plotter(kCCPi0AnaStyle),
        m_variable(nullptr),
        m_mc_pot(mc_pot),
        m_data_pot(data_pot),
        m_mc_pot_scale(1.0),
        m_do_frac_uncertainty(do_frac_uncertainty),
        m_do_cov_area_norm(do_cov_area_norm),
        m_include_stat_error(include_stat_error),
        m_print_format(print_format)
        {
            myPlotStyle();  // Implemented in 'includes/myPlotStyle.h'
            m_mnv_plotter.draw_normalized_to_bin_width = do_bin_width_norm;
        }
        
        
        // Members
        // =======
        
        Variable* m_variable;
        MnvPlotter m_mnv_plotter;
        double m_mc_pot;
        double m_data_pot;
        double m_mc_pot_scale;
        bool m_do_frac_uncertainty;
        bool m_do_cov_area_norm;
        bool m_include_stat_error;
        std::string m_print_format;
        
        
        // Set title
        // =========
        
        void SetTitle(std::string title) {
            if ( !gPad )
                throw std::runtime_error(" Need a TCanvas. Please make one first!!! ");
            
            m_mnv_plotter.AddHistoTitle(title.c_str());
        }
        
        
        // Set X label
        // ===========
        
        // 1D histograms
        // -------------
        void SetXLabel(PlotUtils::MnvH1D* hist) {
            std::string label;
            
            if ( m_variable->m_units == "" )
                label = m_variable->m_hists.m_xlabel;
            else
                label = m_variable->m_hists.m_xlabel + " [" + m_variable->m_units + "]";
            
            if ( hist ) {
                hist->GetXaxis()->SetTitle(label.c_str());
                hist->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist->GetXaxis()->CenterTitle(kTRUE);
                hist->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetXLabel(PlotUtils::MnvH1D* hist, std::string label) {
            if ( hist ) {
                hist->GetXaxis()->SetTitle(label.c_str());
                hist->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist->GetXaxis()->CenterTitle(kTRUE);
                hist->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetXLabel(TH1* hist) {
            std::string label;
            
            if ( m_variable->m_units == "" )
                label = m_variable->m_hists.m_xlabel;
            else
                label = m_variable->m_hists.m_xlabel + " [" + m_variable->m_units + "]";
            
            if ( hist ) {
                hist->GetXaxis()->SetTitle(label.c_str());
                hist->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist->GetXaxis()->CenterTitle(kTRUE);
                hist->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetXLabel(TH1* hist, std::string label) {
            if ( hist ) {
                hist->GetXaxis()->SetTitle(label.c_str());
                hist->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist->GetXaxis()->CenterTitle(kTRUE);
                hist->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        
        // 2D histograms
        // -------------
        void SetXLabel(PlotUtils::MnvH2D* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetXaxis()->SetTitle(label.c_str());
                hist2D->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist2D->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist2D->GetXaxis()->CenterTitle(kTRUE);
                hist2D->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetXLabel(TH2* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetXaxis()->SetTitle(label.c_str());
                hist2D->GetXaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_x);
                hist2D->GetXaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_x);
                hist2D->GetXaxis()->CenterTitle(kTRUE);
                hist2D->GetXaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetXaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        
        // Set Y label
        // ===========
        
        // 1D histograms
        // -------------
        void SetYLabel(PlotUtils::MnvH1D* hist, double norm_bin_width) {
            std::string label;
            
            if ( norm_bin_width == (int)norm_bin_width )
                label = Form("Events / %d ", (int)norm_bin_width) + m_variable->m_units;
            else
                label = Form("Events / %.3f ", norm_bin_width) + m_variable->m_units;
            
            if ( hist ) {
                hist->GetYaxis()->SetTitle(label.c_str());
                hist->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist->GetYaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_y);
                hist->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetYLabel(PlotUtils::MnvH1D* hist, std::string label) {
            if ( hist ) {
                hist->GetYaxis()->SetTitle(label.c_str());
                hist->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist->GetYaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_y);
                hist->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetYLabel(TH1* hist, double norm_bin_width) {
            std::string label;
            
            if ( norm_bin_width == (int)norm_bin_width )
                label = Form("Events / %d ", (int)norm_bin_width) + m_variable->m_units;
            else
                label = Form("Events / %.3f ", norm_bin_width) + m_variable->m_units;
            
            if ( hist ) {
                hist->GetYaxis()->SetTitle(label.c_str());
                hist->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist->GetYaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_y);
                hist->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetYLabel(TH1* hist, std::string label) {
            if ( hist ) {
                hist->GetYaxis()->SetTitle(label.c_str());
                hist->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist->GetYaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_y);
                hist->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        
        // 2D histograms
        // -------------
        void SetYLabel(PlotUtils::MnvH2D* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetYaxis()->SetTitle(label.c_str());
                hist2D->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist2D->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist2D->GetYaxis()->CenterTitle(kTRUE);
                hist2D->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetYLabel(TH2* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetYaxis()->SetTitle(label.c_str());
                hist2D->GetYaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_y);
                hist2D->GetYaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_y);
                hist2D->GetYaxis()->CenterTitle(kTRUE);
                hist2D->GetYaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetYaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        
        // Set Z label
        // ===========
        
        void SetZLabel(PlotUtils::MnvH2D* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetZaxis()->SetTitle(label.c_str());
                hist2D->GetZaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_z);
                hist2D->GetZaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_z);
                hist2D->GetZaxis()->CenterTitle(kFALSE);
                hist2D->GetZaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetZaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        void SetZLabel(TH2* hist2D, std::string label) {
            if ( hist2D ) {
                hist2D->GetZaxis()->SetTitle(label.c_str());
                hist2D->GetZaxis()->SetTitleFont(m_mnv_plotter.axis_title_font_z);
                hist2D->GetZaxis()->SetTitleSize(m_mnv_plotter.axis_title_size_z);
                hist2D->GetZaxis()->CenterTitle(kFALSE);
                hist2D->GetZaxis()->SetLabelFont(m_mnv_plotter.axis_label_font);
                hist2D->GetZaxis()->SetLabelSize(m_mnv_plotter.axis_label_size);
            }
        }
        
        
        
        // Set 2D histogram margin style
        // =============================
        
        void Set2DHistoStyle(TCanvas* canv, PlotUtils::MnvH2D* hist2D) {
            if ( canv ) {
                canv->SetRightMargin(0.15);
            }
            if ( hist2D ) {
                hist2D->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist2D->GetYaxis()->SetTitleOffset(1.1);
                hist2D->GetZaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_z);
            }
        }
        
        void Set2DHistoStyle(TCanvas* canv, TH2* hist2D) {
            if ( canv ) {
                canv->SetRightMargin(0.15);
            }
            if ( hist2D ) {
                hist2D->GetXaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_x);
                hist2D->GetYaxis()->SetTitleOffset(1.1);
                hist2D->GetZaxis()->SetTitleOffset(m_mnv_plotter.axis_title_offset_z);
            }
        }
};





// ================================================================================================================================================================
//  PLOT DATA AND MC WITH STAT AND/OR SYST ERRORS
// ================================================================================================================================================================

void PlotDataMC(PlotInfo plot_info,
                PlotUtils::MnvH1D* h_input_data,  // Data histo
                PlotUtils::MnvH1D* h_input_mc,    // MC histo
                std::string output_str,           // Output location inside top directory
                std::string title_str,            // Histo title at header
                std::string xlabel_str  = "",     // X-axis label
                std::string ylabel_str  = "",     // Y-axis label
                std::string legend_pos  = "TR",   // Legend position
                double Ymin             = -1.0,   // Y-axis minimum: default is y=0
                double Ymax             = -1.0,   // Y-axis maximum: default is automatic size
                bool add_pot_info       = true,   // Add POT info?
                bool data_stat_err_only = false,  // Data histo with stat errors only?
                bool mc_stat_err_only   = true,   // MC histo with stat errors only?
                bool signal_tuned       = false,  // Write 'signal tuned'?
                bool backgr_no_tuned    = false,  // Write 'background not tuned'?
                bool plas_backgr_tuned  = false,  // Write 'plastic background tuned'?
                bool all_backgr_tuned   = false,  // Write 'background tuned'?
                bool write_preliminary  = false,  // Write 'MINERvA preliminary'?
                bool write_area_norm    = false,  // Write 'area normalized'?
                bool use_hist_titles    = false,  // Use histogram titles?
                bool add_chi2_info      = false,  // Add chi2 information?
                bool use_joint_chi2     = false,  // Use joint chi2 between many histograms?
                double chi2             = -1.0,
                int chi2_Npars          = -1,
                int chi2_ndf            = -1)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histograms
    PlotUtils::MnvH1D* mnvh_data_clone = (PlotUtils::MnvH1D*)h_input_data->Clone("mnvh_data_clone");
    PlotUtils::MnvH1D* mnvh_mc_clone   = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc_clone, mnvh_mc_clone->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc_clone, ylabel_str);
    
    // Define stat-only histograms
    TH1D* h_data_staterr = (TH1D*)mnvh_data_clone->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_staterr   = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    
    // Asign histograms to stat-only or full errors
    PlotUtils::MnvH1D* mnvh_data;
    if ( data_stat_err_only ) mnvh_data = new PlotUtils::MnvH1D(*h_data_staterr);
    else                      mnvh_data = mnvh_data_clone;
    
    PlotUtils::MnvH1D* mnvh_mc;
    if ( mc_stat_err_only ) mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    else                    mnvh_mc = mnvh_mc_clone;
    
    
    // Set Y-axis min and max
    // ======================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_data -> SetMinimum(Ymin);
        mnvh_mc   -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    plot_info.m_mnv_plotter.legend_text_size = 0.04;
    plot_info.m_mnv_plotter.DrawDataMCWithErrorBand(mnvh_data,                        // Data histo
                                                    mnvh_mc,                          // MC histo
                                                    plot_info.m_mc_pot_scale,         // MC already POT-normalized
                                                    legend_pos,                       // Legend position
                                                    use_hist_titles,                  // Use histogram titles?
                                                    NULL,                             // MC background histo
                                                    NULL,                             // Data background histo
                                                    plot_info.m_do_cov_area_norm,     // Area normalized?
                                                    plot_info.m_include_stat_error);  // Include stat errors?
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.75, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.75, 0.03, 1, 62);
    }
    
    // MC error labels
    if ( mc_stat_err_only ) {
        char* words = Form("MC: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.71, 0.03, 1, 62);
    }
    else {
        char* words = Form("MC: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.71, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.65, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.65, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.61, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.61, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.815, 0.61, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.81, 0.53, 0.03);
    
    // Chi2 info
    if ( add_chi2_info ) {
        // Calculate chi2 from data and MC
        if ( chi2 == -1.0 && chi2_Npars != -1 ) {
            int ndf;
            char* words;
            chi2 = plot_info.m_mnv_plotter.Chi2DataMC(mnvh_data, mnvh_mc, ndf, plot_info.m_mc_pot_scale);
            ndf = ndf - chi2_Npars;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
        // Print chi2 info directly from input
        if ( chi2 != -1.0 && chi2_ndf != -1 ) {
            char* words;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.legend_text_size = 0.35;
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
}





// ================================================================================================================================================================
//  PLOT DATA AND MC WITH STAT AND/OR SYST ERRORS, AND BACKGROUND
// ================================================================================================================================================================

void PlotDataMCWithBackgr(PlotInfo plot_info,
                          PlotUtils::MnvH1D* h_input_data,       // Data histo
                          PlotUtils::MnvH1D* h_input_mc,         // MC histo
                          PlotUtils::MnvH1D* h_input_mc_backgr,  // MC histo
                          std::string output_str,                // Output location inside top directory
                          std::string title_str,                 // Histo title at header
                          std::string xlabel_str  = "",          // X-axis label
                          std::string ylabel_str  = "",          // Y-axis label
                          std::string legend_pos  = "TR",        // Legend position
                          double Ymin             = -1.0,        // Y-axis minimum: default is y=0
                          double Ymax             = -1.0,        // Y-axis maximum: default is automatic size
                          bool add_pot_info       = true,        // Add POT info?
                          bool data_stat_err_only = false,       // Data histo with stat errors only?
                          bool mc_stat_err_only   = true,        // MC histo with stat errors only?
                          bool signal_tuned       = false,       // Write 'signal tuned'?
                          bool backgr_no_tuned    = false,       // Write 'background not tuned'?
                          bool plas_backgr_tuned  = false,       // Write 'plastic background tuned'?
                          bool all_backgr_tuned   = false,       // Write 'background tuned'?
                          bool write_preliminary  = false,       // Write 'MINERvA preliminary'?
                          bool write_area_norm    = false,       // Write 'area normalized'?
                          bool use_hist_titles    = false,       // Use histogram titles?
                          bool add_chi2_info      = false,       // Add chi2 information?
                          bool use_joint_chi2     = false,       // Use joint chi2 between many histograms?
                          double chi2             = -1.0,
                          int chi2_Npars          = -1,
                          int chi2_ndf            = -1)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histograms
    PlotUtils::MnvH1D* mnvh_data_clone = (PlotUtils::MnvH1D*)h_input_data->Clone("mnvh_data_clone");
    PlotUtils::MnvH1D* mnvh_mc_clone   = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    PlotUtils::MnvH1D* mnvh_mc_backgr  = (PlotUtils::MnvH1D*)h_input_mc_backgr->Clone("mnvh_mc_backgr");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc_clone, mnvh_mc_clone->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc_clone, ylabel_str);
    
    // Define stat-only histograms
    TH1D* h_data_staterr = (TH1D*)mnvh_data_clone->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_staterr   = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    
    // Asign histograms to stat-only or full errors
    PlotUtils::MnvH1D* mnvh_data;
    if ( data_stat_err_only ) mnvh_data = new PlotUtils::MnvH1D(*h_data_staterr);
    else                      mnvh_data = mnvh_data_clone;
    
    PlotUtils::MnvH1D* mnvh_mc;
    if ( mc_stat_err_only ) mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    else                    mnvh_mc = mnvh_mc_clone;
    
    
    // Set Y-axis min and max
    // ======================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_data -> SetMinimum(Ymin);
        mnvh_mc   -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    plot_info.m_mnv_plotter.legend_text_size = 0.038;
    plot_info.m_mnv_plotter.DrawDataMCWithErrorBand(mnvh_data,                        // Data histo
                                                    mnvh_mc,                          // MC histo
                                                    plot_info.m_mc_pot_scale,         // MC already POT-normalized
                                                    legend_pos,                       // Legend position
                                                    use_hist_titles,                  // Use histogram titles?
                                                    mnvh_mc_backgr,                   // MC background histo
                                                    NULL,                             // Data background histo
                                                    plot_info.m_do_cov_area_norm,     // Area normalized?
                                                    plot_info.m_include_stat_error);  // Include stat errors?
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.71, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.71, 0.03, 1, 62);
    }
    
    // MC error labels
    if ( mc_stat_err_only ) {
        char* words = Form("MC: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.67, 0.03, 1, 62);
    }
    else {
        char* words = Form("MC: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.67, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.61, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.61, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.57, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.57, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.57, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.78, 0.49, 0.03);
    
    // Chi2 info
    if ( add_chi2_info ) {
        // Calculate chi2 from data and MC
        if ( chi2 == -1.0 && chi2_Npars != -1 ) {
            int ndf;
            char* words;
            chi2 = plot_info.m_mnv_plotter.Chi2DataMC(mnvh_data, mnvh_mc, ndf, plot_info.m_mc_pot_scale);
            ndf = ndf - chi2_Npars;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
        // Print chi2 info directly from input
        if ( chi2 != -1.0 && chi2_ndf != -1 ) {
            char* words;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.legend_text_size = 0.035;
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_backgr;
}





// ================================================================================================================================================================
//  PLOT MC WITH STAT AND/OR SYST ERRORS
// ================================================================================================================================================================

void PlotMC(PlotInfo plot_info,
            PlotUtils::MnvH1D* h_input_mc,    // MC histo
            std::string output_str,           // Output location inside top directory
            std::string title_str,            // Histo title at header
            std::string xlabel_str  = "",     // X-axis label
            std::string ylabel_str  = "",     // Y-axis label
            double Ymin             = -1.0,   // Y-axis minimum
            double Ymax             = -1.0,   // Y-axis maximum
            double Yline            = -1.0,   // Y-axis horizontal line
            bool add_pot_info       = true,   // Add POT info?
            bool mc_stat_err_only   = true,   // MC histo with stat errors only?
            bool signal_tuned       = false,  // Write 'signal tuned'?
            bool backgr_no_tuned    = false,  // Write 'background not tuned'?
            bool plas_backgr_tuned  = false,  // Write 'plastic background tuned'?
            bool all_backgr_tuned   = false,  // Write 'background tuned'?
            bool write_preliminary  = false,  // Write 'MINERvA preliminary'?
            bool use_bin_width_norm = true)   // Use bin normalized to width?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histogram
    PlotUtils::MnvH1D* mnvh_mc_clone = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    if ( use_bin_width_norm ) mnvh_mc_clone -> Scale(mnvh_mc_clone->GetNormBinWidth(), "width");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc_clone, mnvh_mc_clone->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc_clone, ylabel_str);
    
    // Define stat-only and full error histograms
    TH1D* h_mc_staterr = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_fullerr = (TH1D*)mnvh_mc_clone->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    
    // Asign histogram to stat-only or full errors
    PlotUtils::MnvH1D* mnvh_mc;
    if ( mc_stat_err_only ) mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    else                    mnvh_mc = new PlotUtils::MnvH1D(*h_mc_fullerr);
    
    
    // Set Y-axis min and max
    // ======================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_mc_dummy = (PlotUtils::MnvH1D*)mnvh_mc->Clone();
        mnvh_mc_dummy -> Scale(mnvh_mc_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_mc_dummy->GetBinContent(mnvh_mc_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_mc_dummy;
    }
    
    
    // Draw
    // ====
    plot_info.m_mnv_plotter.DrawMCWithErrorBand(mnvh_mc,                    // MC histogram
                                                plot_info.m_mc_pot_scale);  // MC already POT-normalized
    
    
    // Add Y line
    // ==========
    if ( Yline != -1.0 ) {
        const TAxis* axis = mnvh_mc->GetXaxis();
        double lowX  = axis->GetBinLowEdge(axis->GetFirst());
        double highX = axis->GetBinUpEdge(axis->GetLast());
        
        TLine line;
        line.SetLineStyle(2);
        line.SetLineWidth(4);
        line.SetLineColor(36);
        line.DrawLine(lowX, Yline, highX, Yline);
    }
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // MC error labels
    if ( mc_stat_err_only ) {
        char* words = Form("Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.79, 0.87, 0.03, 1, 62);
    }
    else {
        char* words = Form("Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.79, 0.87, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.82, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.82, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.78, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.78, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.78, 0.03, 1, 52);
    }
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.78, 0.68, 0.03);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_mc_clone;
    delete mnvh_mc;
}





// ================================================================================================================================================================
//  PLOT DATA AND MC RATIO WITH ERROR ENVELOPE
// ================================================================================================================================================================

void PlotDataMCRatio(PlotInfo plot_info,
                     PlotUtils::MnvH1D* h_input_data,  // Data histo
                     PlotUtils::MnvH1D* h_input_mc,    // MC histo
                     std::string output_str,           // Output location inside top directory
                     std::string title_str,            // Histo title at header
                     std::string xlabel_str  = "",     // X-axis label
                     std::string ylabel_str  = "",     // Y-axis label
                     double Ymin             = 0.0,    // Y-axis minimum
                     double Ymax             = 2.0,    // Y-axis maximum
                     bool add_pot_info       = true,   // Add POT info?
                     bool data_stat_err_only = false,  // Data histo with stat errors only?
                     bool mc_stat_err_only   = true,   // MC histo with stat errors only?
                     bool signal_tuned       = false,  // Write 'signal tuned'?
                     bool backgr_no_tuned    = false,  // Write 'background not tuned'?
                     bool plas_backgr_tuned  = false,  // Write 'plastic background tuned'?
                     bool all_backgr_tuned   = false,  // Write 'background tuned'?
                     bool write_preliminary  = false,  // Write 'MINERvA preliminary'?
                     bool write_area_norm    = false)  // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histograms
    PlotUtils::MnvH1D* mnvh_data_clone = (PlotUtils::MnvH1D*)h_input_data->Clone("mnvh_data_clone");
    PlotUtils::MnvH1D* mnvh_mc_clone   = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    std::string ylabel = "Data / MC";
    if ( ylabel_str != "" ) ylabel = ylabel_str;
    
    // Define stat-only histograms
    TH1D* h_data_staterr = (TH1D*)mnvh_data_clone->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_staterr   = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    
    // Asign histograms to stat-only or full errors
    PlotUtils::MnvH1D* mnvh_data;
    if ( data_stat_err_only ) mnvh_data = new PlotUtils::MnvH1D(*h_data_staterr);
    else                      mnvh_data = mnvh_data_clone;
    
    PlotUtils::MnvH1D* mnvh_mc;
    if ( mc_stat_err_only ) mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    else                    mnvh_mc = mnvh_mc_clone;
    
    
    // Draw
    // ====
    
    bool draw_syst_envelope = true;
    if ( mc_stat_err_only ) draw_syst_envelope = false;
    
    plot_info.m_mnv_plotter.DrawDataMCRatio(mnvh_data,                 // Data histogram
                                            mnvh_mc,                   // MC histogram
                                            plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                            draw_syst_envelope,        // Draw systematic lines?
                                            true,                      // Draw ratio = 1.0 line?
                                            Ymin,                      // Y-axis minimum
                                            Ymax,                      // Y-axis maximum
                                            ylabel.c_str());           // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.88, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.88, 0.03, 1, 62);
    }
    
    // MC error labels
    if ( !mc_stat_err_only ) {
        char* words = Form("MC: Systematic envelope");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.84, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.78, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.78, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.74, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.74, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.74, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.81, 0.64, 0.03);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
}





// ================================================================================================================================================================
//  PLOT DATA AND STACKED MC OF SIGNAL AND BACKGROUND BREAKDOWN
// ================================================================================================================================================================

void PlotDataStackedMC(PlotInfo plot_info,
                       PlotUtils::MnvH1D* h_input_data,       // Data histo
                       PlotUtils::MnvH1D* h_input_mc,         // MC histo signal + background
                       PlotUtils::MnvH1D* h_input_mc_Signal,  // MC histos per category
                       PlotUtils::MnvH1D* h_input_mc_Pi0HighW, 
                       PlotUtils::MnvH1D* h_input_mc_QElike,
                       PlotUtils::MnvH1D* h_input_mc_PionProd,
                       PlotUtils::MnvH1D* h_input_mc_PlasUp,
                       PlotUtils::MnvH1D* h_input_mc_PlasBetw,
                       PlotUtils::MnvH1D* h_input_mc_PlasDown,
                       PlotUtils::MnvH1D* h_input_mc_Other,
                       std::string output_str,               // Output location inside top directory
                       std::string title_str,                // Histo title at header
                       std::string xlabel_str  = "",         // X-axis label
                       std::string ylabel_str  = "",         // Y-axis label
                       std::string legend_pos  = "TR",       // Legend position
                       double Ymin             = -1.0,       // Y-axis minimum: default is y=0
                       double Ymax             = -1.0,       // Y-axis maximum: default is automatic size
                       bool add_pot_info       = true,       // Add POT info?
                       bool data_stat_err_only = false,      // Data histo with stat errors only?
                       bool signal_tuned       = false,      // Write 'signal tuned'?
                       bool backgr_no_tuned    = false,      // Write 'background not tuned'?
                       bool plas_backgr_tuned  = false,      // Write 'plastic background tuned'?
                       bool all_backgr_tuned   = false,      // Write 'background tuned'?
                       bool write_preliminary  = false,      // Write 'MINERvA preliminary'?
                       bool write_area_norm    = false,      // Write 'area normalized'?
                       bool add_chi2_info      = false,      // Add chi2 information?
                       bool use_joint_chi2     = false,      // Use joint chi2 between many histograms?
                       double chi2             = -1.0,
                       int chi2_Npars          = -1,
                       int chi2_ndf            = -1)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_data        = (PlotUtils::MnvH1D*)h_input_data        -> Clone("mnvh_data");
    PlotUtils::MnvH1D* mnvh_mc          = (PlotUtils::MnvH1D*)h_input_mc          -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_Signal   = (PlotUtils::MnvH1D*)h_input_mc_Signal   -> Clone("mnvh_mc_Signal");
    PlotUtils::MnvH1D* mnvh_mc_Pi0HighW = (PlotUtils::MnvH1D*)h_input_mc_Pi0HighW -> Clone("mnvh_mc_Pi0HighW");
    PlotUtils::MnvH1D* mnvh_mc_QElike   = (PlotUtils::MnvH1D*)h_input_mc_QElike   -> Clone("mnvh_mc_QElike");
    PlotUtils::MnvH1D* mnvh_mc_PionProd = (PlotUtils::MnvH1D*)h_input_mc_PionProd -> Clone("mnvh_mc_PionProd");
    PlotUtils::MnvH1D* mnvh_mc_PlasUp   = (PlotUtils::MnvH1D*)h_input_mc_PlasUp   -> Clone("mnvh_mc_PlasUp");
    PlotUtils::MnvH1D* mnvh_mc_PlasBetw = (PlotUtils::MnvH1D*)h_input_mc_PlasBetw -> Clone("mnvh_mc_PlasBetw");
    PlotUtils::MnvH1D* mnvh_mc_PlasDown = (PlotUtils::MnvH1D*)h_input_mc_PlasDown -> Clone("mnvh_mc_PlasDown");
    PlotUtils::MnvH1D* mnvh_mc_Other    = (PlotUtils::MnvH1D*)h_input_mc_Other    -> Clone("mnvh_mc_Other");
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc          = mnvh_mc          -> Integral(0, mnvh_mc          -> GetNbinsX()+1);
    double area_mc_Signal   = mnvh_mc_Signal   -> Integral(0, mnvh_mc_Signal   -> GetNbinsX()+1);
    double area_mc_Pi0HighW = mnvh_mc_Pi0HighW -> Integral(0, mnvh_mc_Pi0HighW -> GetNbinsX()+1);
    double area_mc_QElike   = mnvh_mc_QElike   -> Integral(0, mnvh_mc_QElike   -> GetNbinsX()+1);
    double area_mc_PionProd = mnvh_mc_PionProd -> Integral(0, mnvh_mc_PionProd -> GetNbinsX()+1);
    double area_mc_PlasUp   = mnvh_mc_PlasUp   -> Integral(0, mnvh_mc_PlasUp   -> GetNbinsX()+1);
    double area_mc_PlasBetw = mnvh_mc_PlasBetw -> Integral(0, mnvh_mc_PlasBetw -> GetNbinsX()+1);
    double area_mc_PlasDown = mnvh_mc_PlasDown -> Integral(0, mnvh_mc_PlasDown -> GetNbinsX()+1);
    double area_mc_Other    = mnvh_mc_Other    -> Integral(0, mnvh_mc_Other    -> GetNbinsX()+1);
    
    legend_label = GetTruthClassification_LegendLabel(kSignal) + Form(" (%.1f%%)", 100.0*(area_mc_Signal/area_mc));
    mnvh_mc_Signal -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPi0HighW) + Form(" (%.1f%%)", 100.0*(area_mc_Pi0HighW/area_mc));
    mnvh_mc_Pi0HighW -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrQElike) + Form(" (%.1f%%)", 100.0*(area_mc_QElike/area_mc));
    mnvh_mc_QElike -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPionProd) + Form(" (%.1f%%)", 100.0*(area_mc_PionProd/area_mc));
    mnvh_mc_PionProd -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasUp) + Form(" (%.1f%%)", 100.0*(area_mc_PlasUp/area_mc));
    mnvh_mc_PlasUp -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasBetw) + Form(" (%.1f%%)", 100.0*(area_mc_PlasBetw/area_mc));
    mnvh_mc_PlasBetw -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasDown) + Form(" (%.1f%%)", 100.0*(area_mc_PlasDown/area_mc));
    mnvh_mc_PlasDown -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrOther) + Form(" (%.1f%%)", 100.0*(area_mc_Other/area_mc));
    mnvh_mc_Other -> SetTitle(legend_label.c_str());
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_Signal,   int(kSignal),         1);
    SetHistColorScheme(mnvh_mc_Pi0HighW, int(kBackgrPi0HighW), 1);
    SetHistColorScheme(mnvh_mc_QElike,   int(kBackgrQElike),   1);
    SetHistColorScheme(mnvh_mc_PionProd, int(kBackgrPionProd), 1);
    SetHistColorScheme(mnvh_mc_PlasUp,   int(kBackgrPlasUp),   1);
    SetHistColorScheme(mnvh_mc_PlasBetw, int(kBackgrPlasBetw), 1);
    SetHistColorScheme(mnvh_mc_PlasDown, int(kBackgrPlasDown), 1);
    SetHistColorScheme(mnvh_mc_Other,    int(kBackgrOther),    1);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Other);
    h_mc_array -> Add(mnvh_mc_PlasDown);
    h_mc_array -> Add(mnvh_mc_PlasBetw);
    h_mc_array -> Add(mnvh_mc_PlasUp);
    h_mc_array -> Add(mnvh_mc_PionProd);
    h_mc_array -> Add(mnvh_mc_QElike);
    h_mc_array -> Add(mnvh_mc_Pi0HighW);
    h_mc_array -> Add(mnvh_mc_Signal);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_data);
    else                    plot_info.SetXLabel(mnvh_data, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_data, mnvh_data->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_data, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    const char* x_label = mnvh_data->GetXaxis()->GetTitle();
    const char* y_label = mnvh_data->GetYaxis()->GetTitle();
    
    plot_info.m_mnv_plotter.DrawDataStackedMC(mnvh_data,                 // Data histogram
                                              h_mc_array,                // MC array histogram
                                              plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                              legend_pos,                // Legend position
                                              "Data",                    // Data histogram name
                                              -1, -1,                    // MC base color and color offset
                                              1001,                      // MC fill style
                                              x_label,                   // X-axis label
                                              y_label);                  // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.50, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.50, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.45, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.45, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.41, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.41, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.41, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.77, 0.39, 0.03);
    
    // Chi2 info
    if ( add_chi2_info ) {
        // Calculate chi2 from data and MC
        if ( chi2 == -1.0 && chi2_Npars != -1 ) {
            int ndf;
            char* words;
            chi2 = plot_info.m_mnv_plotter.Chi2DataMC(mnvh_data, mnvh_mc, ndf, plot_info.m_mc_pot_scale);
            ndf = ndf - chi2_Npars;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
        // Print chi2 info directly from input
        if ( chi2 != -1.0 && chi2_ndf != -1 ) {
            char* words;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_Signal;
    delete mnvh_mc_Pi0HighW;
    delete mnvh_mc_QElike;
    delete mnvh_mc_PionProd;
    delete mnvh_mc_PlasUp;
    delete mnvh_mc_PlasBetw;
    delete mnvh_mc_PlasDown;
    delete mnvh_mc_Other;
    delete h_mc_array;
}





// ================================================================================================================================================================
//  PLOT DATA AND STACKED MC OF TRUE MATERIAL BREAKDOWN
// ================================================================================================================================================================

void PlotDataStackedMC(PlotInfo plot_info,
                       PlotUtils::MnvH1D* h_input_data,           // Data histo
                       PlotUtils::MnvH1D* h_input_mc,             // MC histo of all materials
                       PlotUtils::MnvH1D* h_input_mc_TrueTgt4Pb,  // MC histos per category
                       PlotUtils::MnvH1D* h_input_mc_TrueTgt5Pb,
                       PlotUtils::MnvH1D* h_input_mc_TrueTgt5Fe,
                       PlotUtils::MnvH1D* h_input_mc_TruePlasUp,
                       PlotUtils::MnvH1D* h_input_mc_TruePlasBetw,
                       PlotUtils::MnvH1D* h_input_mc_TruePlasDown,
                       PlotUtils::MnvH1D* h_input_mc_TrueOtherMat,
                       std::string output_str,                      // Output location inside top directory
                       std::string title_str,                       // Histo title at header
                       std::string xlabel_str  = "",                // X-axis label
                       std::string ylabel_str  = "",                // Y-axis label
                       std::string legend_pos  = "TR",              // Legend position
                       double Ymin             = -1.0,              // Y-axis minimum: default is y=0
                       double Ymax             = -1.0,              // Y-axis maximum: default is automatic size
                       bool add_pot_info       = true,              // Add POT info?
                       bool data_stat_err_only = false,             // Data histo with stat errors only?
                       bool signal_tuned       = false,             // Write 'signal tuned'?
                       bool backgr_no_tuned    = false,             // Write 'background not tuned'?
                       bool plas_backgr_tuned  = false,             // Write 'plastic background tuned'?
                       bool all_backgr_tuned   = false,             // Write 'background tuned'?
                       bool write_preliminary  = false,             // Write 'MINERvA preliminary'?
                       bool write_area_norm    = false,             // Write 'area normalized'?
                       bool add_chi2_info      = false,             // Add chi2 information?
                       bool use_joint_chi2     = false,             // Use joint chi2 between many histograms?
                       double chi2             = -1.0,
                       int chi2_Npars          = -1,
                       int chi2_ndf            = -1)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_data            = (PlotUtils::MnvH1D*)h_input_data            -> Clone("mnvh_data");
    PlotUtils::MnvH1D* mnvh_mc              = (PlotUtils::MnvH1D*)h_input_mc              -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_TrueTgt4Pb   = (PlotUtils::MnvH1D*)h_input_mc_TrueTgt4Pb   -> Clone("mnvh_mc_TrueTgt4Pb");
    PlotUtils::MnvH1D* mnvh_mc_TrueTgt5Pb   = (PlotUtils::MnvH1D*)h_input_mc_TrueTgt5Pb   -> Clone("mnvh_mc_TrueTgt5Pb");
    PlotUtils::MnvH1D* mnvh_mc_TrueTgt5Fe   = (PlotUtils::MnvH1D*)h_input_mc_TrueTgt5Fe   -> Clone("mnvh_mc_TrueTgt5Fe");
    PlotUtils::MnvH1D* mnvh_mc_TruePlasUp   = (PlotUtils::MnvH1D*)h_input_mc_TruePlasUp   -> Clone("mnvh_mc_TruePlasUp");
    PlotUtils::MnvH1D* mnvh_mc_TruePlasBetw = (PlotUtils::MnvH1D*)h_input_mc_TruePlasBetw -> Clone("mnvh_mc_TruePlasBetw");
    PlotUtils::MnvH1D* mnvh_mc_TruePlasDown = (PlotUtils::MnvH1D*)h_input_mc_TruePlasDown -> Clone("mnvh_mc_TruePlasDown");
    PlotUtils::MnvH1D* mnvh_mc_TrueOtherMat = (PlotUtils::MnvH1D*)h_input_mc_TrueOtherMat -> Clone("mnvh_mc_TrueOtherMat");
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc              = mnvh_mc              -> Integral(0, mnvh_mc              -> GetNbinsX()+1);
    double area_mc_TrueTgt4Pb   = mnvh_mc_TrueTgt4Pb   -> Integral(0, mnvh_mc_TrueTgt4Pb   -> GetNbinsX()+1);
    double area_mc_TrueTgt5Pb   = mnvh_mc_TrueTgt5Pb   -> Integral(0, mnvh_mc_TrueTgt5Pb   -> GetNbinsX()+1);
    double area_mc_TrueTgt5Fe   = mnvh_mc_TrueTgt5Fe   -> Integral(0, mnvh_mc_TrueTgt5Fe   -> GetNbinsX()+1);
    double area_mc_TruePlasUp   = mnvh_mc_TruePlasUp   -> Integral(0, mnvh_mc_TruePlasUp   -> GetNbinsX()+1);
    double area_mc_TruePlasBetw = mnvh_mc_TruePlasBetw -> Integral(0, mnvh_mc_TruePlasBetw -> GetNbinsX()+1);
    double area_mc_TruePlasDown = mnvh_mc_TruePlasDown -> Integral(0, mnvh_mc_TruePlasDown -> GetNbinsX()+1);
    double area_mc_TrueOtherMat = mnvh_mc_TrueOtherMat -> Integral(0, mnvh_mc_TrueOtherMat -> GetNbinsX()+1);
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt4Pb) + Form(" (%.1f%%)", 100.0*(area_mc_TrueTgt4Pb/area_mc));
    mnvh_mc_TrueTgt4Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Pb) + Form(" (%.1f%%)", 100.0*(area_mc_TrueTgt5Pb/area_mc));
    mnvh_mc_TrueTgt5Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Fe) + Form(" (%.1f%%)", 100.0*(area_mc_TrueTgt5Fe/area_mc));
    mnvh_mc_TrueTgt5Fe -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasUp) + Form(" (%.1f%%)", 100.0*(area_mc_TruePlasUp/area_mc));
    mnvh_mc_TruePlasUp -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasBetw) + Form(" (%.1f%%)", 100.0*(area_mc_TruePlasBetw/area_mc));
    mnvh_mc_TruePlasBetw -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasDown) + Form(" (%.1f%%)", 100.0*(area_mc_TruePlasDown/area_mc));
    mnvh_mc_TruePlasDown -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueOtherMat) + Form(" (%.1f%%)", 100.0*(area_mc_TrueOtherMat/area_mc));
    mnvh_mc_TrueOtherMat -> SetTitle(legend_label.c_str());
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_TrueTgt4Pb,   int(kTrueTgt4Pb),   2);
    SetHistColorScheme(mnvh_mc_TrueTgt5Pb,   int(kTrueTgt5Pb),   2);
    SetHistColorScheme(mnvh_mc_TrueTgt5Fe,   int(kTrueTgt5Fe),   2);
    SetHistColorScheme(mnvh_mc_TruePlasUp,   int(kTruePlasUp),   2);
    SetHistColorScheme(mnvh_mc_TruePlasBetw, int(kTruePlasBetw), 2);
    SetHistColorScheme(mnvh_mc_TruePlasDown, int(kTruePlasDown), 2);
    SetHistColorScheme(mnvh_mc_TrueOtherMat, int(kTrueOtherMat), 2);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_TrueOtherMat);
    h_mc_array -> Add(mnvh_mc_TruePlasDown);
    h_mc_array -> Add(mnvh_mc_TruePlasBetw);
    h_mc_array -> Add(mnvh_mc_TruePlasUp);
    h_mc_array -> Add(mnvh_mc_TrueTgt5Fe);
    h_mc_array -> Add(mnvh_mc_TrueTgt5Pb);
    h_mc_array -> Add(mnvh_mc_TrueTgt4Pb);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_data);
    else                    plot_info.SetXLabel(mnvh_data, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_data, mnvh_data->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_data, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    const char* x_label = mnvh_data->GetXaxis()->GetTitle();
    const char* y_label = mnvh_data->GetYaxis()->GetTitle();
    
    plot_info.m_mnv_plotter.DrawDataStackedMC(mnvh_data,                 // Data histogram
                                              h_mc_array,                // MC array histogram
                                              plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                              legend_pos,                // Legend position
                                              "Data",                    // Data histogram name
                                              -1, -1,                    // MC base color and color offset
                                              1001,                      // MC fill style
                                              x_label,                   // X-axis label
                                              y_label);                  // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.54, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.54, 0.03, 1, 62);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.48, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.48, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.44, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.44, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.44, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.77, 0.41, 0.03);
    
    // Chi2 info
    if ( add_chi2_info ) {
        // Calculate chi2 from data and MC
        if ( chi2 == -1.0 && chi2_Npars != -1 ) {
            int ndf;
            char* words;
            chi2 = plot_info.m_mnv_plotter.Chi2DataMC(mnvh_data, mnvh_mc, ndf, plot_info.m_mc_pot_scale);
            ndf = ndf - chi2_Npars;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
        // Print chi2 info directly from input
        if ( chi2 != -1.0 && chi2_ndf != -1 ) {
            char* words;
            if ( use_joint_chi2 ) words = Form("Joint #chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            else                  words = Form("#chi^{2} / ndf = %3.2f / %d", chi2, chi2_ndf);
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.3, 0.78, 0.03, 1, 62);
        }
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_TrueTgt4Pb;
    delete mnvh_mc_TrueTgt5Pb;
    delete mnvh_mc_TrueTgt5Fe;
    delete mnvh_mc_TruePlasUp;
    delete mnvh_mc_TruePlasBetw;
    delete mnvh_mc_TruePlasDown;
    delete mnvh_mc_TrueOtherMat;
    delete h_mc_array;
}





// ================================================================================================================================================================
//  PLOT STACKED MC OF BACKGROUND BREAKDOWN
// ================================================================================================================================================================

void PlotStackedMC(PlotInfo plot_info,
                   PlotUtils::MnvH1D* h_input_mc,           // MC histo of total background
                   PlotUtils::MnvH1D* h_input_mc_Pi0HighW,  // MC histos of background categories
                   PlotUtils::MnvH1D* h_input_mc_QElike,
                   PlotUtils::MnvH1D* h_input_mc_PionProd,
                   PlotUtils::MnvH1D* h_input_mc_PlasUp,
                   PlotUtils::MnvH1D* h_input_mc_PlasBetw,
                   PlotUtils::MnvH1D* h_input_mc_PlasDown,
                   PlotUtils::MnvH1D* h_input_mc_Other,
                   std::string output_str,                 // Output location inside top directory
                   std::string title_str,                  // Histo title at header
                   std::string xlabel_str  = "",           // X-axis label
                   std::string ylabel_str  = "",           // Y-axis label
                   std::string legend_pos  = "TR",         // Legend position
                   double Ymin             = -1.0,         // Y-axis minimum: default is y=0
                   double Ymax             = -1.0,         // Y-axis maximum: default is automatic size
                   bool add_pot_info       = true,         // Add POT info?
                   bool signal_tuned       = false,        // Write 'signal tuned'?
                   bool backgr_no_tuned    = false,        // Write 'background not tuned'?
                   bool plas_backgr_tuned  = false,        // Write 'plastic background tuned'?
                   bool all_backgr_tuned   = false,        // Write 'background tuned'?
                   bool write_preliminary  = false,        // Write 'MINERvA preliminary'?
                   bool write_area_norm    = false)        // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_mc          = (PlotUtils::MnvH1D*)h_input_mc          -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_Pi0HighW = (PlotUtils::MnvH1D*)h_input_mc_Pi0HighW -> Clone("mnvh_mc_Pi0HighW");
    PlotUtils::MnvH1D* mnvh_mc_QElike   = (PlotUtils::MnvH1D*)h_input_mc_QElike   -> Clone("mnvh_mc_QElike");
    PlotUtils::MnvH1D* mnvh_mc_PionProd = (PlotUtils::MnvH1D*)h_input_mc_PionProd -> Clone("mnvh_mc_PionProd");
    PlotUtils::MnvH1D* mnvh_mc_PlasUp   = (PlotUtils::MnvH1D*)h_input_mc_PlasUp   -> Clone("mnvh_mc_PlasUp");
    PlotUtils::MnvH1D* mnvh_mc_PlasBetw = (PlotUtils::MnvH1D*)h_input_mc_PlasBetw -> Clone("mnvh_mc_PlasBetw");
    PlotUtils::MnvH1D* mnvh_mc_PlasDown = (PlotUtils::MnvH1D*)h_input_mc_PlasDown -> Clone("mnvh_mc_PlasDown");
    PlotUtils::MnvH1D* mnvh_mc_Other    = (PlotUtils::MnvH1D*)h_input_mc_Other    -> Clone("mnvh_mc_Other");
    
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc          = mnvh_mc          -> Integral(0, mnvh_mc          -> GetNbinsX()+1);
    double area_mc_Pi0HighW = mnvh_mc_Pi0HighW -> Integral(0, mnvh_mc_Pi0HighW -> GetNbinsX()+1);
    double area_mc_QElike   = mnvh_mc_QElike   -> Integral(0, mnvh_mc_QElike   -> GetNbinsX()+1);
    double area_mc_PionProd = mnvh_mc_PionProd -> Integral(0, mnvh_mc_PionProd -> GetNbinsX()+1);
    double area_mc_PlasUp   = mnvh_mc_PlasUp   -> Integral(0, mnvh_mc_PlasUp   -> GetNbinsX()+1);
    double area_mc_PlasBetw = mnvh_mc_PlasBetw -> Integral(0, mnvh_mc_PlasBetw -> GetNbinsX()+1);
    double area_mc_PlasDown = mnvh_mc_PlasDown -> Integral(0, mnvh_mc_PlasDown -> GetNbinsX()+1);
    double area_mc_Other    = mnvh_mc_Other    -> Integral(0, mnvh_mc_Other    -> GetNbinsX()+1);
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPi0HighW) + Form(" (%.1f%%)", 100.0*(area_mc_Pi0HighW/area_mc));
    mnvh_mc_Pi0HighW -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrQElike) + Form(" (%.1f%%)", 100.0*(area_mc_QElike/area_mc));
    mnvh_mc_QElike -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPionProd) + Form(" (%.1f%%)", 100.0*(area_mc_PionProd/area_mc));
    mnvh_mc_PionProd -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasUp) + Form(" (%.1f%%)", 100.0*(area_mc_PlasUp/area_mc));
    mnvh_mc_PlasUp -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasBetw) + Form(" (%.1f%%)", 100.0*(area_mc_PlasBetw/area_mc));
    mnvh_mc_PlasBetw -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrPlasDown) + Form(" (%.1f%%)", 100.0*(area_mc_PlasDown/area_mc));
    mnvh_mc_PlasDown -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kBackgrOther) + Form(" (%.1f%%)", 100.0*(area_mc_Other/area_mc));
    mnvh_mc_Other -> SetTitle(legend_label.c_str());
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_Pi0HighW, int(kBackgrPi0HighW), 1);
    SetHistColorScheme(mnvh_mc_QElike,   int(kBackgrQElike),   1);
    SetHistColorScheme(mnvh_mc_PionProd, int(kBackgrPionProd), 1);
    SetHistColorScheme(mnvh_mc_PlasUp,   int(kBackgrPlasUp),   1);
    SetHistColorScheme(mnvh_mc_PlasBetw, int(kBackgrPlasBetw), 1);
    SetHistColorScheme(mnvh_mc_PlasDown, int(kBackgrPlasDown), 1);
    SetHistColorScheme(mnvh_mc_Other,    int(kBackgrOther),    1);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Other);
    h_mc_array -> Add(mnvh_mc_PlasDown);
    h_mc_array -> Add(mnvh_mc_PlasBetw);
    h_mc_array -> Add(mnvh_mc_PlasUp);
    h_mc_array -> Add(mnvh_mc_PionProd);
    h_mc_array -> Add(mnvh_mc_QElike);
    h_mc_array -> Add(mnvh_mc_Pi0HighW);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc);
    else                    plot_info.SetXLabel(mnvh_mc, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc, mnvh_mc->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
    }
    
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc   -> SetMaximum(Ymax);
    }
    
    
    // Draw
    // ====
    const char* x_label = mnvh_mc->GetXaxis()->GetTitle();
    const char* y_label = mnvh_mc->GetYaxis()->GetTitle();
    
    plot_info.m_mnv_plotter.DrawStackedMC(h_mc_array,
                                          plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                          legend_pos,                // Legend position
                                          -1, -1,                    // MC base color and color offset
                                          1001,                      // MC fill style
                                          x_label,                   // X-axis label
                                          y_label);                  // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
        plot_info.m_mnv_plotter.WriteNorm(Form("MC POT: %.2E", plot_info.m_mc_pot), 0.3, 0.91-(2.0*0.03), 0.03);
    }
    
    // Signal or background tuned or not tuned
    if ( signal_tuned ) {
        char* words = Form("Signal tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.54, 0.03, 1, 52);
    }
    else {
        if ( backgr_no_tuned || plas_backgr_tuned || all_backgr_tuned ) {
            char* words = Form("Signal not tuned");
            plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.54, 0.03, 1, 52);
        }
    }
    if ( backgr_no_tuned ) {
        char* words = Form("Background not tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.50, 0.03, 1, 52);
    }
    if ( plas_backgr_tuned ) {
        char* words = Form("Plastic backgr. tuned only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.50, 0.03, 1, 52);
    }
    if ( all_backgr_tuned ) {
        char* words = Form("Background tuned");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.74, 0.50, 0.03, 1, 52);
    }
    
    // Area-normalized
    if ( write_area_norm )
        plot_info.m_mnv_plotter.WriteNorm("Area-normalized", 0.3, 0.91-(3.0*0.03), 0.03);
    
    // "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.76, 0.48, 0.03);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_mc;
    delete mnvh_mc_Pi0HighW;
    delete mnvh_mc_QElike;
    delete mnvh_mc_PionProd;
    delete mnvh_mc_PlasUp;
    delete mnvh_mc_PlasBetw;
    delete mnvh_mc_PlasDown;
    delete mnvh_mc_Other;
    delete h_mc_array;
}





// ================================================================================================================================================================
//  PLOT CLOSURE RATIO
// ================================================================================================================================================================

void PlotClosureRatio(PlotInfo plot_info,
                      PlotUtils::MnvH1D* mnvh_ratio,
                      std::string output_str,
                      std::string title_str,
                      std::string xlabel_str = "",
                      std::string ylabel_str = "",
                      double Ymin            = -1.0,
                      double Ymax            = -1.0)
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    // Get histogram
    TH1* h_ratio = (TH1*)mnvh_ratio->GetCVHistoWithStatError().Clone("");
    
    // Axis labels
    plot_info.SetXLabel(h_ratio, xlabel_str);
    plot_info.SetYLabel(h_ratio, ylabel_str);
    
    // Draw
    h_ratio -> SetMarkerStyle(0);
    h_ratio -> SetLineWidth(4);
    h_ratio -> SetLineColor(kBlue+1);
    h_ratio -> Draw("HIST");
    
    // Add Y = 0 line
    const TAxis* axis = h_ratio->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(2);
    line.SetLineWidth(3);
    line.SetLineColor(36);
    line.DrawLine(lowX, 0.0, highX, 0.0);
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h_ratio;
}





// ================================================================================================================================================================
//  FLUX
// ================================================================================================================================================================

// ==============================================================================================
//  Plot flux
// ==============================================================================================

void PlotFlux(PlotInfo plot_info,
              PlotUtils::MnvH1D* mnvh,
              std::string output_str,
              std::string title_str,
              std::string xlabel_str = "",
              std::string ylabel_str = "",
              double Xmin            = 0.0,
              double Xmax            = 20.0)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histogram
    TH1D* h = (TH1D*)mnvh->GetCVHistoWithStatError().Clone("h");
    
    // Set histograms
    h -> SetMarkerStyle(0);
    h -> SetLineWidth(3);
    h -> SetLineColor(kBlue+1);
    
    // Axis labels
    plot_info.SetXLabel(h, xlabel_str);
    plot_info.SetYLabel(h, ylabel_str);
    
    // Y-axis limits
    double Ymax = 1.15 * h->GetMaximum();
    h -> SetMaximum(Ymax);
    
    // X-axis limits
    h -> GetXaxis() -> SetRangeUser(Xmin, Xmax);
    
    // Draw
    h -> Draw("HIST");
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h;
}



// ==============================================================================================
//  Plot flux ratio
// ==============================================================================================

void PlotFluxRatio(PlotInfo plot_info,
                   PlotUtils::MnvH1D* mnvh_ratio,
                   std::string output_str,
                   std::string title_str,
                   std::string xlabel_str = "",
                   std::string ylabel_str = "",
                   double Xmax            = 20.0)
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    // Get histogram
    TH1* h_ratio = (TH1*)mnvh_ratio->GetCVHistoWithStatError().Clone("");
    
    // Axis labels
    plot_info.SetXLabel(h_ratio, xlabel_str);
    plot_info.SetYLabel(h_ratio, ylabel_str);
    
    // Set Y-axis limits
    double max_diff = 0.0;
    for ( int bin = 1; bin <= h_ratio->GetNbinsX(); ++bin ) {
        double bin_center  = h_ratio->GetXaxis()->GetBinCenter(bin);
        if ( bin_center > Xmax ) break;
        
        double bin_content = h_ratio->GetBinContent(bin);
        if ( std::fabs(bin_content-1.0) > max_diff ) max_diff = std::fabs(bin_content-1.0);
    }
    h_ratio -> SetMaximum(1.0 + 1.5*max_diff);
    h_ratio -> SetMinimum(1.0 - 1.5*max_diff);
    
    // Draw
    h_ratio -> GetXaxis() -> SetRangeUser(0.0, Xmax);
    h_ratio -> SetMarkerStyle(0);
    h_ratio -> SetLineWidth(3);
    h_ratio -> SetLineColor(kBlue+1);
    h_ratio -> Draw("HIST");
    
    // Add Y = 0 line
    const TAxis* axis = h_ratio->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(2);
    line.SetLineWidth(3);
    line.SetLineColor(36);
    line.DrawLine(0.0, 1.0, highX, 1.0);
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h_ratio;
}





// ================================================================================================================================================================
//  MIGRATION, COVARIANCE AND CORRELATION MATRICES
// ================================================================================================================================================================

// ==============================================================================================
//  Plot row-normalized migration matrix
// ==============================================================================================
/* Adapted from MnvPlotter::DrawNormalizedMigrationHistogram() */

void PlotRowNormMigration(PlotInfo plot_info,
                          PlotUtils::MnvH2D* mnvh_mig,
                          std::string output_str,
                          std::string title_str,
                          std::string xlabel_str = "",
                          std::string ylabel_str = "",
                          std::string zlabel_str = "",
                          bool include_flows     = true,
                          bool draw_as_matrix    = false)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histogram
    TH2D* h_mig = (TH2D*)mnvh_mig->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("h_mig");
    
    // Create migration matrix (partially copied from 'DrawNormalizedMigrationHistogram')
    int first_bin = include_flows ? 0 : 1;
    int last_bin  = include_flows ? h_mig->GetNbinsX()+1 : h_mig->GetNbinsX();
    int nbins     = include_flows ? h_mig->GetNbinsX()+2 : h_mig->GetNbinsX();
    
    TMatrixD matrix_mig(nbins, nbins);
    TH2D h_tmp(*h_mig);
    h_tmp.Reset();
    
    for ( int y = first_bin; y <= last_bin; ++y ) {
        double norm = 0.0;
        for ( int x = first_bin; x <= last_bin; ++x ) 
            norm += h_mig->GetBinContent(x, y);

        if ( fabs(norm) > 1e-8) {
            for ( int x = first_bin; x <= last_bin; ++x ) {
                double percentage = 100.0 * (h_mig->GetBinContent(x, y) / norm);
                if ( include_flows ) matrix_mig[y][x]     = percentage;
                else                 matrix_mig[y-1][x-1] = percentage;
                
                h_tmp.SetBinContent(x, y, percentage);
            }
        }
    }
    
    // Axis labels (partially copied from 'DrawNormalizedMigrationHistogram')
    if ( draw_as_matrix ) {
        h_tmp = TH2D(matrix_mig);
        plot_info.SetXLabel(&h_tmp, xlabel_str);
        plot_info.SetYLabel(&h_tmp, ylabel_str);
        plot_info.SetZLabel(&h_tmp, zlabel_str);
    }
    else {
        plot_info.SetXLabel(&h_tmp, xlabel_str);
        plot_info.SetYLabel(&h_tmp, ylabel_str);
        plot_info.SetZLabel(&h_tmp, zlabel_str);
    }
    
    // Set palette
    plot_info.m_mnv_plotter.SetRedHeatPalette();
    
    // Set Z-axis limits
    h_tmp.SetMinimum(0.0);
    h_tmp.SetMaximum(100.0);
    
    // Draw histogram
    plot_info.Set2DHistoStyle(&canvas, &h_tmp);
    
    if ( draw_as_matrix ) {
        gStyle -> SetPaintTextFormat("2.1f");
        h_tmp.GetXaxis()->SetRangeUser(first_bin, last_bin+1);
        h_tmp.GetYaxis()->SetRangeUser(first_bin, last_bin+1);
        h_tmp.SetMarkerSize(1.2);
        h_tmp.Draw("COLZ text");
    }
    else {
        h_tmp.Draw("COLZ");
    }
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Go back to previous style
    myPlotStyle();
    
    // Release
    delete h_mig;
}



// ==============================================================================================
//  Plot covariance matrix
// ==============================================================================================

void PlotCovarianceMatrix(PlotInfo plot_info,
                          TH2D* h_cov,
                          std::string output_str,
                          std::string title_str,
                          std::string xlabel_str = "",
                          std::string ylabel_str = "",
                          std::string zlabel_str = "",
                          bool hide_underflow    = true,
                          bool use_log_z         = false)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histogram
    TH2D* h = (TH2D*)h_cov->Clone("h");
    
    // Hide underflow bin (if needed),
    // but not overflow (OVERFLOW DOESN'T NEED TO BE HIDDEN)
    if ( hide_underflow ) {
        int Nbinsx  = h->GetNbinsX();
        int Nbinsy  = h->GetNbinsY();
        TH2D* h_tmp = new TH2D("", "", Nbinsx-1, 1.0, Nbinsx, Nbinsy-1, 1.0, Nbinsy);
        
        for ( int i = 2; i <= Nbinsx; ++i ) {
            for ( int j = 2; j <= Nbinsy; ++j ) {
                if ( i == Nbinsx && j == Nbinsy ) continue;
                double content = h->GetBinContent(i, j);
                double error   = h->GetBinError(i, j);
                h_tmp -> SetBinContent(i-1, j-1, content);
                h_tmp -> SetBinError(i-1, j-1, error);
            }
        }
        h = h_tmp;
    }
    
    // Axis labels
    plot_info.SetXLabel(h, xlabel_str);
    plot_info.SetYLabel(h, ylabel_str);
    plot_info.SetZLabel(h, zlabel_str);
    
    // Set log Z and palette
    if ( use_log_z ) {
        canvas.SetLogz();
        plot_info.m_mnv_plotter.SetROOT6Palette(55);  // kRainbow
    }
    else {
        double max = std::fabs(h->GetMaximum());
        double min = std::fabs(h->GetMinimum());
        double abs = (max >= min) ? max : min;
        
        h -> SetMaximum(max);
        h -> SetMinimum(-1.0*max);
        plot_info.m_mnv_plotter.SetROOT6Palette(87);  // kLightTemperature
    }
    
    // Draw histogram
    plot_info.Set2DHistoStyle(&canvas, h);
    h -> Draw("COLZ");
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Go back to previous style
    myPlotStyle();
    
    // Release
    delete h;
}



// ==============================================================================================
//  Plot correlation matrix
// ==============================================================================================

void PlotCorrelationMatrix(PlotInfo plot_info,
                           TH2D* h_corr,
                           std::string output_str,
                           std::string title_str,
                           std::string xlabel_str = "",
                           std::string ylabel_str = "",
                           std::string zlabel_str = "",
                           bool hide_underflow    = true)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histogram
    TH2D* h = (TH2D*)h_corr->Clone("h");
    
    // Hide underflow bin (if needed)
    if ( hide_underflow ) {
        int Nbinsx  = h->GetNbinsX();
        int Nbinsy  = h->GetNbinsY();
        TH2D* h_tmp = new TH2D("", "", Nbinsx-1, 1.0, Nbinsx, Nbinsy-1, 1.0, Nbinsy);
        
        for ( int i = 2; i <= Nbinsx; ++i ) {
            for ( int j = 2; j <= Nbinsy; ++j ) {
                if ( i == Nbinsx && j == Nbinsy ) continue;
                double content = h->GetBinContent(i, j);
                double error   = h->GetBinError(i, j);
                h_tmp -> SetBinContent(i-1, j-1, content);
                h_tmp -> SetBinError(i-1, j-1, error);
            }
        }
        h = h_tmp;
        h -> SetMaximum(1.0);
        h -> SetMinimum(-1.0);
    }
    
    // Axis labels
    plot_info.SetXLabel(h, xlabel_str);
    plot_info.SetYLabel(h, ylabel_str);
    plot_info.SetZLabel(h, zlabel_str);
    
    // Set palette
    plot_info.m_mnv_plotter.SetCorrelationPalette();
    
    // Draw histogram
    plot_info.Set2DHistoStyle(&canvas, h);
    h -> Draw("COLZ");
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Go back to previous style
    myPlotStyle();
    
    // Release
    delete h;
}





// ================================================================================================================================================================
//  BACKGROUND TUNING FUNCTIONS
// ================================================================================================================================================================

// ==============================================================================================
//  Data-MC chi2 using only histograms
// ==============================================================================================

double CalculateChi2DataMC(PlotInfo plot_info,
                           PlotUtils::MnvH1D* mnvh_data,
                           PlotUtils::MnvH1D* mnvh_mc,
                           int& ndf)
{
    // Get histograms
    PlotUtils::MnvH1D* h_data = (PlotUtils::MnvH1D*)mnvh_data->Clone("h_data");
    PlotUtils::MnvH1D* h_mc   = (PlotUtils::MnvH1D*)mnvh_mc->Clone("h_mc");
    
    // Get chi2
    double chi2 = plot_info.m_mnv_plotter.Chi2DataMC(h_data, h_mc, ndf, plot_info.m_mc_pot_scale);
    
    // Release
    delete h_data;
    delete h_mc;
    
    // Return chi2
    return chi2;
}



// ==============================================================================================
//  Plot plastic weights
// ==============================================================================================

void PlotPlasticWeights(PlotInfo plot_info,
                        PlotUtils::MnvH1D* mnvh_PlasUp,
                        PlotUtils::MnvH1D* mnvh_PlasBetw,
                        PlotUtils::MnvH1D* mnvh_PlasDown,
                        std::string output_str,
                        std::string title_str,
                        std::string xlabel_str = "",
                        std::string ylabel_str = "")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histograms
    TH1* h_PlasUp   = (TH1*)mnvh_PlasUp->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_PlasUp");
    TH1* h_PlasBetw = (TH1*)mnvh_PlasBetw->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_PlasBetw");
    TH1* h_PlasDown = (TH1*)mnvh_PlasDown->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_PlasDown");
    
    TH1* h_PlasUp_err   = (TH1*)h_PlasUp->Clone("h_PlasUp_err");
    TH1* h_PlasBetw_err = (TH1*)h_PlasBetw->Clone("h_PlasBetw_err");
    TH1* h_PlasDown_err = (TH1*)h_PlasDown->Clone("h_PlasDown_err");
    
    // Set histograms
    h_PlasUp -> SetMarkerStyle(0);
    h_PlasUp -> SetMarkerColor(kSpring);
    h_PlasUp -> SetLineWidth(5);
    h_PlasUp -> SetLineColor(kSpring);
    
    h_PlasBetw -> SetMarkerStyle(0);
    h_PlasBetw -> SetMarkerColor(kMagenta);
    h_PlasBetw -> SetLineWidth(5);
    h_PlasBetw -> SetLineColor(kMagenta);
    
    h_PlasDown -> SetMarkerStyle(0);
    h_PlasDown -> SetMarkerColor(kBlue-5);
    h_PlasDown -> SetLineWidth(5);
    h_PlasDown -> SetLineColor(kBlue-5);
    
    h_PlasUp_err -> SetMarkerStyle(0);
    h_PlasUp_err -> SetFillColor(kSpring);
    h_PlasUp_err -> SetFillStyle(3001);
    
    h_PlasBetw_err -> SetMarkerStyle(0);
    h_PlasBetw_err -> SetFillColor(kMagenta);
    h_PlasBetw_err -> SetFillStyle(3002);
    
    h_PlasDown_err -> SetMarkerStyle(0);
    h_PlasDown_err -> SetFillColor(kBlue-5);
    h_PlasDown_err -> SetFillStyle(3001);
    
    // Axis labels
    plot_info.SetXLabel(h_PlasUp_err, xlabel_str);
    plot_info.SetYLabel(h_PlasUp_err, ylabel_str);
    
    // Y-axis limits
    double Ymax_PlasUp   = h_PlasUp_err->GetMaximum()   + h_PlasUp_err->GetBinError(h_PlasUp_err->GetMaximumBin());
    double Ymax_PlasBetw = h_PlasBetw_err->GetMaximum() + h_PlasBetw_err->GetBinError(h_PlasBetw_err->GetMaximumBin());
    double Ymax_PlasDown = h_PlasDown_err->GetMaximum() + h_PlasDown_err->GetBinError(h_PlasDown_err->GetMaximumBin());
    
    std::vector<double> Ymax_vector;
    Ymax_vector.push_back(Ymax_PlasUp);
    Ymax_vector.push_back(Ymax_PlasBetw);
    Ymax_vector.push_back(Ymax_PlasDown);
    
    double Ymax = *std::max_element(Ymax_vector.begin(), Ymax_vector.end());
    h_PlasUp_err -> SetMinimum(0.0);
    h_PlasUp_err -> SetMaximum(Ymax * 1.3);
    
    // Draw
    h_PlasUp_err   -> Draw("E2");
    h_PlasBetw_err -> Draw("E2 sames");
    h_PlasDown_err -> Draw("E2 sames");
    h_PlasUp       -> Draw("HIST sames");
    h_PlasBetw     -> Draw("HIST sames");
    h_PlasDown     -> Draw("HIST sames");
    
    // Add Y = 1 line
    const TAxis* axis = h_PlasUp_err->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(2);
    line.SetLineWidth(3);
    line.SetLineColor(kBlack);
    line.DrawLine(lowX, 1.0, highX, 1.0);
    
    // Add legend
    std::vector<TH1*> h_vector;
    h_vector.push_back(h_PlasUp);
    h_vector.push_back(h_PlasBetw);
    h_vector.push_back(h_PlasDown);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Upstream");
    name_vector.push_back("Between");
    name_vector.push_back("Downstream");
    
    std::vector<std::string> opts_vector = {"l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.15, 0.75, 0.22, 0.16, 0.035);
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h_PlasUp;
    delete h_PlasBetw;
    delete h_PlasDown;
    delete h_PlasUp_err;
    delete h_PlasBetw_err;
    delete h_PlasDown_err;
}



// ==============================================================================================
//  Plot physics weights
// ==============================================================================================

void PlotPhysicsWeights(PlotInfo plot_info,
                        PlotUtils::MnvH1D* mnvh_Signal,
                        PlotUtils::MnvH1D* mnvh_Pi0HighW,
                        PlotUtils::MnvH1D* mnvh_QElike,
                        PlotUtils::MnvH1D* mnvh_PionProd,
                        std::string output_str,
                        std::string title_str,
                        std::string xlabel_str = "",
                        std::string ylabel_str = "")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Get histograms
    TH1* h_Signal    = (TH1*)mnvh_Signal->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_Signal");
    TH1* h_Pi0HighW  = (TH1*)mnvh_Pi0HighW->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_Pi0HighW");
    TH1* h_QElike    = (TH1*)mnvh_QElike->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_QElike");
    TH1* h_PionProd  = (TH1*)mnvh_PionProd->GetCVHistoWithError(false, plot_info.m_do_cov_area_norm).Clone("h_PionProd");
    
    TH1* h_Signal_err   = (TH1*)h_Signal->Clone("h_Signal_err");
    TH1* h_Pi0HighW_err = (TH1*)h_Pi0HighW->Clone("h_Pi0HighW_err");
    TH1* h_QElike_err   = (TH1*)h_QElike->Clone("h_QElike_err");
    TH1* h_PionProd_err = (TH1*)h_PionProd->Clone("h_PionProd_err");
    
    // Set histograms
    h_Signal -> SetMarkerStyle(0);
    h_Signal -> SetMarkerColor(kAzure-1);
    h_Signal -> SetLineWidth(5);
    h_Signal -> SetLineColor(kAzure-1);
    
    h_Pi0HighW -> SetMarkerStyle(0);
    h_Pi0HighW -> SetMarkerColor(kRed+1);
    h_Pi0HighW -> SetLineWidth(5);
    h_Pi0HighW -> SetLineColor(kRed+1);
    
    h_QElike -> SetMarkerStyle(0);
    h_QElike -> SetMarkerColor(kCyan+2);
    h_QElike -> SetLineWidth(5);
    h_QElike -> SetLineColor(kCyan+2);
    
    h_PionProd -> SetMarkerStyle(0);
    h_PionProd -> SetMarkerColor(kRed+3);
    h_PionProd -> SetLineWidth(5);
    h_PionProd -> SetLineColor(kRed+3);
    
    h_Signal_err -> SetMarkerStyle(0);
    h_Signal_err -> SetFillColor(kAzure-1);
    h_Signal_err -> SetFillStyle(3001);
    
    h_Pi0HighW_err -> SetMarkerStyle(0);
    h_Pi0HighW_err -> SetFillColor(kRed+1);
    h_Pi0HighW_err -> SetFillStyle(3002);
    
    h_QElike_err -> SetMarkerStyle(0);
    h_QElike_err -> SetFillColor(kCyan+2);
    h_QElike_err -> SetFillStyle(3001);
    
    h_PionProd_err -> SetMarkerStyle(0);
    h_PionProd_err -> SetFillColor(kRed+3);
    h_PionProd_err -> SetFillStyle(3002);
    
    // Axis labels
    plot_info.SetXLabel(h_Signal_err, xlabel_str);
    plot_info.SetYLabel(h_Signal_err, ylabel_str);
    
    // Y-axis limits
    double Ymax_Signal   = h_Signal_err->GetMaximum()   + h_Signal_err->GetBinError(h_Signal_err->GetMaximumBin());
    double Ymax_Pi0HighW = h_Pi0HighW_err->GetMaximum() + h_Pi0HighW_err->GetBinError(h_Pi0HighW_err->GetMaximumBin());
    double Ymax_QElike   = h_QElike_err->GetMaximum()   + h_QElike_err->GetBinError(h_QElike_err->GetMaximumBin());
    double Ymax_PionProd = h_PionProd_err->GetMaximum() + h_PionProd_err->GetBinError(h_PionProd_err->GetMaximumBin());
    
    std::vector<double> Ymax_vector;
    Ymax_vector.push_back(Ymax_Signal);
    Ymax_vector.push_back(Ymax_Pi0HighW);
    Ymax_vector.push_back(Ymax_QElike);
    Ymax_vector.push_back(Ymax_PionProd);
    
    double Ymax = *std::max_element(Ymax_vector.begin(), Ymax_vector.end());
    h_Signal_err -> SetMinimum(0.0);
    h_Signal_err -> SetMaximum(Ymax * 1.25);
    
    // Draw
    h_Signal_err   -> Draw("E2");
    h_Pi0HighW_err -> Draw("E2 sames");
    h_QElike_err   -> Draw("E2 sames");
    h_PionProd_err -> Draw("E2 sames");
    h_Signal       -> Draw("HIST sames");
    h_Pi0HighW     -> Draw("HIST sames");
    h_QElike       -> Draw("HIST sames");
    h_PionProd     -> Draw("HIST sames");
    
    // Add Y = 1 line
    const TAxis* axis = h_Signal_err->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(2);
    line.SetLineWidth(3);
    line.SetLineColor(kBlack);
    line.DrawLine(lowX, 1.0, highX, 1.0);
    
    // Add legend
    std::vector<TH1*> h_vector;
    h_vector.push_back(h_Signal);
    h_vector.push_back(h_Pi0HighW);
    h_vector.push_back(h_QElike);
    h_vector.push_back(h_PionProd);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("CC 1#pi^{0} signal");
    name_vector.push_back("High-#font[12]{W} CC #pi^{0}");
    name_vector.push_back("CCQE-like");
    name_vector.push_back("CC #pi^{#pm} prod.");
    
    std::vector<std::string> opts_vector = {"l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.15, 0.72, 0.24, 0.19, 0.035);
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h_Signal;
    delete h_Pi0HighW;
    delete h_QElike;
    delete h_PionProd;
    delete h_Signal_err;
    delete h_Pi0HighW_err;
    delete h_QElike_err;
    delete h_PionProd_err;
}





// ================================================================================================================================================================
//  WARPING STUDIES FUNCTIONS
// ================================================================================================================================================================

// ==============================================================================================
//  Plot chi2 full info
// ==============================================================================================

void PlotWarpingChi2FullInfo(PlotInfo plot_info,
                             PlotUtils::MnvH2D* mnvh2D,
                             PlotUtils::MnvH1D* mnvh_median,
                             TProfile* prof_avg,
                             std::string output_str,
                             std::string title_str,
                             std::string xlabel_str = "",
                             std::string ylabel_str = "",
                             std::string zlabel_str = "",
                             double ndf             = 13.0,
                             double Ymax            = 50.0,
                             double use_logy        = false)
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    // Use log scale
    if ( use_logy ) canvas.SetLogy();
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH2D* mnvh2D_clone = (PlotUtils::MnvH2D*)mnvh2D->Clone("mnvh2D_clone");
    TH2* h2D = (TH2*)mnvh2D_clone->GetCVHistoWithStatError().Clone("h2D");
    
    PlotUtils::MnvH1D* mnvh_median_clone = (PlotUtils::MnvH1D*)mnvh_median->Clone("mnvh_median_clone");
    TH1* h_median = (TH1*)mnvh_median->GetCVHistoWithStatError().Clone("h_median");
    
    TH1D* h_mean = h2D->ProfileX();
    
    
    // Set histograms and lines
    // ========================
    
    // Set 1D histograms
    h_median -> SetMarkerStyle(0);
    h_median -> SetMarkerColor(kBlue+1);
    h_median -> SetLineWidth(5);
    h_median -> SetLineColor(kBlue+1);
    
    h_mean -> SetMarkerStyle(0);
    h_mean -> SetMarkerColor(kCyan+1);
    h_mean -> SetLineWidth(5);
    h_mean -> SetLineColor(kCyan+1);
    
    // Add Y lines
    TLine line_ndf;
    line_ndf.SetLineStyle(9);
    line_ndf.SetLineWidth(4);
    line_ndf.SetLineColor(kBlack);
    
    TLine line_ndfX2;
    line_ndfX2.SetLineStyle(6);
    line_ndfX2.SetLineWidth(4);
    line_ndfX2.SetLineColor(kBlack);
    
    
    // Draw
    // ====
    
    // Y-axis limit
    h2D -> GetYaxis()->SetRangeUser(0.0, Ymax);
    
    // Axis labels
    plot_info.SetXLabel(h2D, xlabel_str);
    plot_info.SetYLabel(h2D, ylabel_str);
    plot_info.SetZLabel(h2D, zlabel_str);
    
    // Set palette
    plot_info.m_mnv_plotter.SetRedHeatPalette();
    
    // Draw
    plot_info.Set2DHistoStyle(&canvas, h2D);
    h2D -> Draw("COLZ");
    
    const TAxis* axis = h2D->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    line_ndf.DrawLine(lowX, ndf, highX, ndf);
    line_ndfX2.DrawLine(lowX, 2.0*ndf, highX, 2.0*ndf);
    
    h_mean   -> Draw("hist sames");
    h_median -> Draw("hist sames");
    
    
    // Add legend
    // ==========
    
    // Add dummy histograms for legend
    TH1D* h_ndf = new TH1D;
    h_ndf -> SetMarkerStyle(0);
    h_ndf -> SetMarkerColor(kBlack);
    h_ndf -> SetLineStyle(9);
    h_ndf -> SetLineWidth(4);
    h_ndf -> SetLineColor(kBlack);
    
    TH1D* h_ndfX2 = new TH1D;
    h_ndfX2 -> SetMarkerStyle(0);
    h_ndfX2 -> SetMarkerColor(kBlack);
    h_ndfX2 -> SetLineStyle(6);
    h_ndfX2 -> SetLineWidth(4);
    h_ndfX2 -> SetLineColor(kBlack);
    
    // Make legend
    std::vector<TH1*> h_vector;
    h_vector.push_back(h_mean);
    h_vector.push_back(h_median);
    h_vector.push_back(h_ndf);
    h_vector.push_back(h_ndfX2);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Mean #chi^{2}");
    name_vector.push_back("Median #chi^{2}");
    name_vector.push_back("N.d.f. = " + std::to_string((int)ndf));
    name_vector.push_back("N.d.f. #times 2");
    
    std::vector<std::string> opts_vector = {"l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.64, 0.74, 0.20, 0.16, 0.035);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h2D;
    delete h_median;
    delete h_mean;
    delete h_ndf;
    delete h_ndfX2;
}



// ==============================================================================================
//  Plot median chi2 vs. iterations
// ==============================================================================================

void PlotWarpingMedianChi2(PlotInfo plot_info,
                           PlotUtils::MnvH1D* mnvh_mc,
                           std::string output_str,
                           std::string title_str,
                           std::string xlabel_str = "",
                           std::string ylabel_str = "",
                           double ndf             = 13.0,
                           double use_logy        = false)
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    // Use log scale
    if ( use_logy ) canvas.SetLogy();
    
    // Get histogram
    PlotUtils::MnvH1D* mnvh_mc_clone = (PlotUtils::MnvH1D*)mnvh_mc->Clone("mnvh_mc_clone");
    TH1* h_mc = (TH1*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("h_mc");
    
    // Axis labels
    if ( xlabel_str == "" ) plot_info.SetXLabel(h_mc);
    else                    plot_info.SetXLabel(h_mc, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(h_mc, mnvh_mc->GetNormBinWidth());
    else                    plot_info.SetYLabel(h_mc, ylabel_str);
    
    
    // Add Y lines
    // ===========
    
    const TAxis* axis = h_mc->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line_ndf;
    line_ndf.SetLineStyle(9);
    line_ndf.SetLineWidth(4);
    line_ndf.SetLineColor(kBlack);
    
    TLine line_ndfX2;
    line_ndfX2.SetLineStyle(6);
    line_ndfX2.SetLineWidth(4);
    line_ndfX2.SetLineColor(kBlack);
    
    
    // Draw
    // ====
    
    gStyle -> SetEndErrorSize(4);
    h_mc -> SetMarkerStyle(20);
    h_mc -> SetMarkerColor(kBlue+1);
    h_mc -> SetMarkerSize(2);
    h_mc -> SetLineWidth(4);
    h_mc -> SetLineColor(kBlue+1);
    h_mc -> Draw("E1 X0");
    
    line_ndf.DrawLine(lowX, ndf, highX, ndf);
    line_ndfX2.DrawLine(lowX, 2.0*ndf, highX, 2.0*ndf);
    
    // Add legend
    // ==========
    
    // Add dummy histograms for legend
    TH1D* h_ndf = new TH1D;
    h_ndf -> SetMarkerStyle(0);
    h_ndf -> SetMarkerColor(kBlack);
    h_ndf -> SetLineStyle(9);
    h_ndf -> SetLineWidth(4);
    h_ndf -> SetLineColor(kBlack);
    
    TH1D* h_ndfX2 = new TH1D;
    h_ndfX2 -> SetMarkerStyle(0);
    h_ndfX2 -> SetMarkerColor(kBlack);
    h_ndfX2 -> SetLineStyle(6);
    h_ndfX2 -> SetLineWidth(4);
    h_ndfX2 -> SetLineColor(kBlack);
    
    // Make legend
    std::vector<TH1*> h_vector;
    h_vector.push_back(h_ndf);
    h_vector.push_back(h_ndfX2);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("N.d.f. = " + std::to_string((int)ndf));
    name_vector.push_back("N.d.f. #times 2");
    
    std::vector<std::string> opts_vector = {"l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.75, 0.78, 0.18, 0.12, 0.035);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete mnvh_mc_clone;
    delete h_mc;
    delete h_ndf;
    delete h_ndfX2;
}



// ==============================================================================================
//  Plot chi2 distribution at a given number of iterations
// ==============================================================================================

void PlotWarpingChi2AtNiter(PlotInfo plot_info,
                            PlotUtils::MnvH2D* mnvh2D,
                            PlotUtils::MnvH1D* mnvh_median,
                            TProfile* prof_avg,
                            std::string output_str,
                            std::string title_str,
                            std::string xlabel_str  = "",
                            std::string ylabel_str  = "",
                            double ndf              = 13.0,
                            int n_iterations        = 2)
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    
    // Get input histograms
    // ====================
    
    // Chi2 vs number of iterations
    PlotUtils::MnvH2D* mnvh2D_clone = (PlotUtils::MnvH2D*)mnvh2D->Clone("mnvh2D_clone");
    TH2* h2D = (TH2*)mnvh2D_clone->GetCVHistoWithStatError().Clone("h2D");
    
    // Chi2 median
    PlotUtils::MnvH1D* mnvh_median_clone = (PlotUtils::MnvH1D*)mnvh_median->Clone("mnvh_median_clone");
    TH1* h_median = (TH1*)mnvh_median->GetCVHistoWithStatError().Clone("h_median");
    
    // Chi2 mean
    TH1D* h_mean = h2D->ProfileX();
    
    
    // Make 1D output histogram
    // ========================
    
    int NbinsX  = h2D->GetNbinsX();
    int NbinsY  = h2D->GetNbinsY();
    double Ymin = h2D->GetYaxis()->GetBinLowEdge(1);
    double Ymax = h2D->GetYaxis()->GetBinLowEdge(NbinsY) + h2D->GetYaxis()->GetBinWidth(NbinsY);
    TH1D* h = new TH1D("histo", "", NbinsY, Ymin, Ymax);
    
    for ( int ybin = 1; ybin <= NbinsY; ++ybin ) {
        double bin_content = h2D->GetBinContent(n_iterations+1, ybin);  // Niterations starts at 0
        h -> SetBinContent(ybin, bin_content);
    }
    
    // Axis labels
    plot_info.SetXLabel(h, xlabel_str);
    plot_info.SetYLabel(h, ylabel_str);
    
    
    // Add X lines
    // ===========
    
    double median = h_median->GetBinContent(n_iterations+1);
    double mean   = h_mean->GetBinContent(n_iterations+1);
    
    TLine line_median;
    line_median.SetLineStyle(1);
    line_median.SetLineWidth(4);
    line_median.SetLineColor(kBlue+1);
    
    TLine line_mean;
    line_mean.SetLineStyle(1);
    line_mean.SetLineWidth(4);
    line_mean.SetLineColor(kCyan+1);
    
    TLine line_ndf;
    line_ndf.SetLineStyle(9);
    line_ndf.SetLineWidth(4);
    line_ndf.SetLineColor(kBlack);
    
    TLine line_ndfX2;
    line_ndfX2.SetLineStyle(6);
    line_ndfX2.SetLineWidth(4);
    line_ndfX2.SetLineColor(kBlack);
    
    
    // Draw
    // ====
    
    double Xmax = 1.2 * h->GetMaximum();
    h -> GetXaxis() -> SetRangeUser(0.0, 50.0);
    h -> SetMarkerStyle(0);
    h -> SetMarkerColor(kRed+1);
    h -> SetLineWidth(5);
    h -> SetLineColor(kRed+1);
    h -> SetMaximum(Xmax);
    h -> Draw("hist");
    
    line_ndf.DrawLine(ndf, 0.0, ndf, Xmax);
    line_ndfX2.DrawLine(2.0*ndf, 0.0, 2.0*ndf, Xmax);
    line_median.DrawLine(median, 0.0, median, Xmax);
    line_mean.DrawLine(mean, 0.0, mean, Xmax);
    
    
    // Add legend
    // ==========
    
    // Set dummy histograms for legend
    h_median -> SetMarkerStyle(0);
    h_median -> SetMarkerColor(kBlue+1);
    h_median -> SetLineStyle(1);
    h_median -> SetLineWidth(4);
    h_median -> SetLineColor(kBlue+1);
    
    h_mean -> SetMarkerStyle(0);
    h_mean -> SetMarkerColor(kCyan+1);
    h_mean -> SetLineStyle(1);
    h_mean -> SetLineWidth(4);
    h_mean -> SetLineColor(kCyan+1);
    
    TH1D* h_ndf = new TH1D;
    h_ndf -> SetMarkerStyle(0);
    h_ndf -> SetMarkerColor(kBlack);
    h_ndf -> SetLineStyle(9);
    h_ndf -> SetLineWidth(4);
    h_ndf -> SetLineColor(kBlack);
    
    TH1D* h_ndfX2 = new TH1D;
    h_ndfX2 -> SetMarkerStyle(0);
    h_ndfX2 -> SetMarkerColor(kBlack);
    h_ndfX2 -> SetLineStyle(6);
    h_ndfX2 -> SetLineWidth(4);
    h_ndfX2 -> SetLineColor(kBlack);
    
    // Add legend
    std::vector<TH1*> h_vector;
    h_vector.push_back(h_mean);
    h_vector.push_back(h_median);
    h_vector.push_back(h_ndf);
    h_vector.push_back(h_ndfX2);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Mean");
    name_vector.push_back("Median");
    name_vector.push_back("N.d.f. = " + std::to_string((int)ndf));
    name_vector.push_back("N.d.f. #times 2");
    
    std::vector<std::string> opts_vector = {"l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.72, 0.73, 0.21, 0.17, 0.035);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete h2D;
    delete h_median;
    delete h_mean;
}



// ==============================================================================================
//  Plot fake data and MC
// ==============================================================================================

void PlotFakeDataMC(PlotInfo plot_info,
                    PlotUtils::MnvH1D* h_input_data,  // Fake data histo
                    PlotUtils::MnvH1D* h_input_mc,    // MC histo
                    std::string output_str,           // Output location inside top directory
                    std::string title_str,            // Histo title at header
                    std::string xlabel_str  = "",     // X-axis label
                    std::string ylabel_str  = "",     // Y-axis label
                    std::string legend_pos  = "TR",   // Legend position
                    double Ymin             = -1.0,   // Y-axis minimum: default is y=0
                    double Ymax             = -1.0,   // Y-axis maximum: default is automatic size
                    bool use_hist_titles    = false)  // Use histogram titles?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histograms
    PlotUtils::MnvH1D* mnvh_data_clone = (PlotUtils::MnvH1D*)h_input_data->Clone("mnvh_data_clone");
    PlotUtils::MnvH1D* mnvh_mc_clone   = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc_clone, mnvh_mc_clone->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc_clone, ylabel_str);
    
    // Define stat-only histograms
    TH1D* h_data_staterr = (TH1D*)mnvh_data_clone->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data_staterr);
    
    TH1D* h_mc_staterr   = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    
    
    // Set Y-axis min and max
    // ======================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_data -> SetMinimum(Ymin);
        mnvh_mc   -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    plot_info.m_mnv_plotter.DrawDataMCWithErrorBand(mnvh_data,                        // Data histo
                                                    mnvh_mc,                          // MC histo
                                                    plot_info.m_mc_pot_scale,         // MC already POT-normalized
                                                    legend_pos,                       // Legend position
                                                    use_hist_titles,                  // Use histogram titles?
                                                    NULL,                             // MC background histo
                                                    NULL,                             // Data background histo
                                                    plot_info.m_do_cov_area_norm,     // Area normalized?
                                                    plot_info.m_include_stat_error);  // Include stat errors?
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
}



// ==============================================================================================
//  Plot fake data and MC ratio
// ==============================================================================================

void PlotFakeDataMCRatio(PlotInfo plot_info,
                         PlotUtils::MnvH1D* h_input_data,  // Data histo
                         PlotUtils::MnvH1D* h_input_mc,    // MC histo
                         std::string output_str,           // Output location inside top directory
                         std::string title_str,            // Histo title at header
                         std::string xlabel_str  = "",     // X-axis label
                         std::string ylabel_str  = "",     // Y-axis label
                         double Ymin             = 0.0,    // Y-axis minimum
                         double Ymax             = 2.0)    // Y-axis maximum
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Clone input histograms
    PlotUtils::MnvH1D* mnvh_data_clone = (PlotUtils::MnvH1D*)h_input_data->Clone("mnvh_data_clone");
    PlotUtils::MnvH1D* mnvh_mc_clone   = (PlotUtils::MnvH1D*)h_input_mc->Clone("mnvh_mc_clone");
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc_clone);
    else                    plot_info.SetXLabel(mnvh_mc_clone, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc_clone, mnvh_mc_clone->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc_clone, ylabel_str);
    
    // Define stat-only histograms
    TH1D* h_data_staterr = (TH1D*)mnvh_data_clone->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data_staterr);
    
    TH1D* h_mc_staterr   = (TH1D*)mnvh_mc_clone->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_mc = new PlotUtils::MnvH1D(*h_mc_staterr);
    
    
    // Draw
    // ====
    plot_info.m_mnv_plotter.DrawDataMCRatio(mnvh_data,                 // Data histogram
                                            mnvh_mc,                   // MC histogram
                                            plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                            true,                      // Draw systematic lines?
                                            true,                      // Draw ratio = 1.0 line?
                                            Ymin,                      // Y-axis minimum
                                            Ymax);                     // Y-axis maximum
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
}





// ================================================================================================================================================================
//  CROSS-SECTION MODELS FUNCTIONS
// ================================================================================================================================================================

// ==============================================================================================
//  Plot data-MC with interaction types
// ==============================================================================================

void PlotDataMC_XsecModels(PlotInfo plot_info,
                           PlotUtils::MnvH1D* h_input_data,      // Data histo
                           PlotUtils::MnvH1D* h_input_mc,        // MC histo
                           PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                           PlotUtils::MnvH1D* h_input_mc_MEC, 
                           PlotUtils::MnvH1D* h_input_mc_DeltaRES,
                           PlotUtils::MnvH1D* h_input_mc_OtherRES,
                           PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                           PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                           PlotUtils::MnvH1D* h_input_mc_Other,
                           std::string output_str,               // Output location inside top directory
                           std::string title_str,                // Histo title at header
                           std::string xlabel_str  = "",         // X-axis label
                           std::string ylabel_str  = "",         // Y-axis label
                           std::string legend_pos  = "TR",       // Legend position
                           double Ymin             = -1.0,       // Y-axis minimum: default is y=0
                           double Ymax             = -1.0,       // Y-axis maximum: default is automatic size
                           bool add_pot_info       = true,       // Add POT info?
                           bool data_stat_err_only = false,      // Data histo with stat errors only?
                           bool write_area_norm    = false)      // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    PlotUtils::MnvH1D* mnvh_mc          = (PlotUtils::MnvH1D*)h_input_mc          -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_QE       = (PlotUtils::MnvH1D*)h_input_mc_QE       -> Clone("mnvh_mc_QE");
    PlotUtils::MnvH1D* mnvh_mc_MEC      = (PlotUtils::MnvH1D*)h_input_mc_MEC      -> Clone("mnvh_mc_MEC");
    PlotUtils::MnvH1D* mnvh_mc_DeltaRES = (PlotUtils::MnvH1D*)h_input_mc_DeltaRES -> Clone("mnvh_mc_DeltaRES");
    PlotUtils::MnvH1D* mnvh_mc_OtherRES = (PlotUtils::MnvH1D*)h_input_mc_OtherRES -> Clone("mnvh_mc_OtherRES");
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS  = (PlotUtils::MnvH1D*)h_input_mc_SoftDIS  -> Clone("mnvh_mc_SoftDIS");
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS  = (PlotUtils::MnvH1D*)h_input_mc_TrueDIS  -> Clone("mnvh_mc_TrueDIS");
    PlotUtils::MnvH1D* mnvh_mc_Other    = (PlotUtils::MnvH1D*)h_input_mc_Other    -> Clone("mnvh_mc_Other");
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        double area_mc   = mnvh_mc->Integral(0, mnvh_mc->GetNbinsX()+1);
        mnvh_data        -> Scale(1.0/area_data);
        mnvh_data_stat   -> Scale(1.0/area_data);
        mnvh_mc          -> Scale(1.0/area_mc);
        mnvh_mc_QE       -> Scale(1.0/area_mc);
        mnvh_mc_MEC      -> Scale(1.0/area_mc);
        mnvh_mc_DeltaRES -> Scale(1.0/area_mc);
        mnvh_mc_OtherRES -> Scale(1.0/area_mc);
        mnvh_mc_SoftDIS  -> Scale(1.0/area_mc);
        mnvh_mc_TrueDIS  -> Scale(1.0/area_mc);
        mnvh_mc_Other    -> Scale(1.0/area_mc);
    }
    
    // Bin width normalize
    mnvh_data        -> Scale(mnvh_data        -> GetNormBinWidth(), "width");
    mnvh_data_stat   -> Scale(mnvh_data_stat   -> GetNormBinWidth(), "width");
    mnvh_mc          -> Scale(mnvh_mc          -> GetNormBinWidth(), "width");
    mnvh_mc_QE       -> Scale(mnvh_mc_QE       -> GetNormBinWidth(), "width");
    mnvh_mc_MEC      -> Scale(mnvh_mc_MEC      -> GetNormBinWidth(), "width");
    mnvh_mc_DeltaRES -> Scale(mnvh_mc_DeltaRES -> GetNormBinWidth(), "width");
    mnvh_mc_OtherRES -> Scale(mnvh_mc_OtherRES -> GetNormBinWidth(), "width");
    mnvh_mc_SoftDIS  -> Scale(mnvh_mc_SoftDIS  -> GetNormBinWidth(), "width");
    mnvh_mc_TrueDIS  -> Scale(mnvh_mc_TrueDIS  -> GetNormBinWidth(), "width");
    mnvh_mc_Other    -> Scale(mnvh_mc_Other    -> GetNormBinWidth(), "width");
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data_stat -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    // Total MC
    mnvh_mc -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc -> SetLineWidth(4);
    mnvh_mc -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // MC components
    mnvh_mc_QE       -> SetLineColor(kCyan+3);
    mnvh_mc_MEC      -> SetLineColor(kYellow+1);
    mnvh_mc_DeltaRES -> SetLineColor(kAzure-4);
    mnvh_mc_OtherRES -> SetLineColor(kRed+2);
    mnvh_mc_SoftDIS  -> SetLineColor(kOrange+1);
    mnvh_mc_TrueDIS  -> SetLineColor(kViolet);
    mnvh_mc_Other    -> SetLineColor(kGray+1);
    
    mnvh_mc_QE       -> SetLineWidth(4);
    mnvh_mc_MEC      -> SetLineWidth(4);
    mnvh_mc_DeltaRES -> SetLineWidth(4);
    mnvh_mc_OtherRES -> SetLineWidth(4);
    mnvh_mc_SoftDIS  -> SetLineWidth(4);
    mnvh_mc_TrueDIS  -> SetLineWidth(4);
    mnvh_mc_Other    -> SetLineWidth(4);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_Other, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_Other, ylabel_str);
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc_Other -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc_Other -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_Other -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_Other -> SetMaximum(Ymax);
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    mnvh_mc_Other    -> Draw("HIST L");
    mnvh_mc_MEC      -> DrawCopy("SAME HIST L");
    mnvh_mc_QE       -> DrawCopy("SAME HIST L");
    mnvh_mc_TrueDIS  -> DrawCopy("SAME HIST L");
    mnvh_mc_SoftDIS  -> DrawCopy("SAME HIST L");
    mnvh_mc_OtherRES -> DrawCopy("SAME HIST L");
    mnvh_mc_DeltaRES -> DrawCopy("SAME HIST L");
    mnvh_mc          -> DrawCopy("SAME HIST");
    mnvh_data        -> DrawCopy("SAME E1 X0");
    mnvh_data_stat   -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_DeltaRES);
    h_vector.push_back(mnvh_mc_OtherRES);
    h_vector.push_back(mnvh_mc_SoftDIS);
    h_vector.push_back(mnvh_mc_TrueDIS);
    h_vector.push_back(mnvh_mc_QE);
    h_vector.push_back(mnvh_mc_MEC);
    h_vector.push_back(mnvh_mc_Other);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("Delta RES");
    name_vector.push_back("Other RES");
    name_vector.push_back("'Soft' DIS");
    name_vector.push_back("'True' DIS");
    name_vector.push_back("QE");
    name_vector.push_back("MEC");
    name_vector.push_back("Other");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.70, 0.58, 0.22, 0.32, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.54, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.54, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_DeltaRES;
    delete mnvh_mc_OtherRES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
}



// ==============================================================================================
//  Plot data-MC with interaction types (from FlatTrees)
// ==============================================================================================

void PlotDataMC_XsecModels(PlotInfo plot_info,
                           PlotUtils::MnvH1D* h_input_data,      // Data histo
                           PlotUtils::MnvH1D* h_input_mc,        // MC histo
                           PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                           PlotUtils::MnvH1D* h_input_mc_MEC, 
                           PlotUtils::MnvH1D* h_input_mc_RES,
                           PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                           PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                           PlotUtils::MnvH1D* h_input_mc_Other,
                           std::string output_str,               // Output location inside top directory
                           std::string title_str,                // Histo title at header
                           std::string xlabel_str  = "",         // X-axis label
                           std::string ylabel_str  = "",         // Y-axis label
                           std::string legend_pos  = "TR",       // Legend position
                           double Ymin             = -1.0,       // Y-axis minimum: default is y=0
                           double Ymax             = -1.0,       // Y-axis maximum: default is automatic size
                           bool add_pot_info       = true,       // Add POT info?
                           bool data_stat_err_only = false,      // Data histo with stat errors only?
                           bool write_area_norm    = false)      // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    PlotUtils::MnvH1D* mnvh_mc         = (PlotUtils::MnvH1D*)h_input_mc         -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_QE      = (PlotUtils::MnvH1D*)h_input_mc_QE      -> Clone("mnvh_mc_QE");
    PlotUtils::MnvH1D* mnvh_mc_MEC     = (PlotUtils::MnvH1D*)h_input_mc_MEC     -> Clone("mnvh_mc_MEC");
    PlotUtils::MnvH1D* mnvh_mc_RES     = (PlotUtils::MnvH1D*)h_input_mc_RES     -> Clone("mnvh_mc_RES");
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS = (PlotUtils::MnvH1D*)h_input_mc_SoftDIS -> Clone("mnvh_mc_SoftDIS");
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS = (PlotUtils::MnvH1D*)h_input_mc_TrueDIS -> Clone("mnvh_mc_TrueDIS");
    PlotUtils::MnvH1D* mnvh_mc_Other   = (PlotUtils::MnvH1D*)h_input_mc_Other   -> Clone("mnvh_mc_Other");
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        double area_mc   = mnvh_mc->Integral(0, mnvh_mc->GetNbinsX()+1);
        mnvh_data       -> Scale(1.0/area_data);
        mnvh_data_stat  -> Scale(1.0/area_data);
        mnvh_mc         -> Scale(1.0/area_mc);
        mnvh_mc_QE      -> Scale(1.0/area_mc);
        mnvh_mc_MEC     -> Scale(1.0/area_mc);
        mnvh_mc_RES     -> Scale(1.0/area_mc);
        mnvh_mc_SoftDIS -> Scale(1.0/area_mc);
        mnvh_mc_TrueDIS -> Scale(1.0/area_mc);
        mnvh_mc_Other   -> Scale(1.0/area_mc);
    }
    
    // Bin width normalize
    mnvh_data       -> Scale(mnvh_data       -> GetNormBinWidth(), "width");
    mnvh_data_stat  -> Scale(mnvh_data_stat  -> GetNormBinWidth(), "width");
    mnvh_mc         -> Scale(mnvh_mc         -> GetNormBinWidth(), "width");
    mnvh_mc_QE      -> Scale(mnvh_mc_QE      -> GetNormBinWidth(), "width");
    mnvh_mc_MEC     -> Scale(mnvh_mc_MEC     -> GetNormBinWidth(), "width");
    mnvh_mc_RES     -> Scale(mnvh_mc_RES     -> GetNormBinWidth(), "width");
    mnvh_mc_SoftDIS -> Scale(mnvh_mc_SoftDIS -> GetNormBinWidth(), "width");
    mnvh_mc_TrueDIS -> Scale(mnvh_mc_TrueDIS -> GetNormBinWidth(), "width");
    mnvh_mc_Other   -> Scale(mnvh_mc_Other   -> GetNormBinWidth(), "width");
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data_stat -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    // Total MC
    mnvh_mc -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc -> SetLineWidth(4);
    mnvh_mc -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // MC components
    mnvh_mc_QE      -> SetLineColor(kCyan+3);
    mnvh_mc_MEC     -> SetLineColor(kYellow+1);
    mnvh_mc_RES     -> SetLineColor(kAzure-8);
    mnvh_mc_SoftDIS -> SetLineColor(kOrange+1);
    mnvh_mc_TrueDIS -> SetLineColor(kViolet);
    mnvh_mc_Other   -> SetLineColor(kGray+1);
    
    mnvh_mc_QE      -> SetLineWidth(4);
    mnvh_mc_MEC     -> SetLineWidth(4);
    mnvh_mc_RES     -> SetLineWidth(4);
    mnvh_mc_SoftDIS -> SetLineWidth(4);
    mnvh_mc_TrueDIS -> SetLineWidth(4);
    mnvh_mc_Other   -> SetLineWidth(4);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_Other, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_Other, ylabel_str);
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc_Other -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc_Other -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_Other -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_Other -> SetMaximum(Ymax);
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    mnvh_mc_Other   -> Draw("HIST L");
    mnvh_mc_MEC     -> DrawCopy("SAME HIST L");
    mnvh_mc_QE      -> DrawCopy("SAME HIST L");
    mnvh_mc_TrueDIS -> DrawCopy("SAME HIST L");
    mnvh_mc_SoftDIS -> DrawCopy("SAME HIST L");
    mnvh_mc_RES     -> DrawCopy("SAME HIST L");
    mnvh_mc         -> DrawCopy("SAME HIST");
    mnvh_data       -> DrawCopy("SAME E1 X0");
    mnvh_data_stat  -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_RES);
    h_vector.push_back(mnvh_mc_SoftDIS);
    h_vector.push_back(mnvh_mc_TrueDIS);
    h_vector.push_back(mnvh_mc_QE);
    h_vector.push_back(mnvh_mc_MEC);
    h_vector.push_back(mnvh_mc_Other);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("RES");
    name_vector.push_back("'Soft' DIS");
    name_vector.push_back("'True' DIS");
    name_vector.push_back("QE");
    name_vector.push_back("MEC");
    name_vector.push_back("Other");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.70, 0.58, 0.22, 0.32, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.54, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.81, 0.54, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_RES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
}



// ==============================================================================================
//  Plot data-MC model ratio with interaction types
// ==============================================================================================

void PlotDataMCRatio_XsecModels(PlotInfo plot_info,
                                PlotUtils::MnvH1D* h_input_data,      // Data histo
                                PlotUtils::MnvH1D* h_input_mc,        // MC histo
                                PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                                PlotUtils::MnvH1D* h_input_mc_MEC, 
                                PlotUtils::MnvH1D* h_input_mc_DeltaRES,
                                PlotUtils::MnvH1D* h_input_mc_OtherRES,
                                PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                                PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                                PlotUtils::MnvH1D* h_input_mc_Other,
                                std::string output_str,               // Output location inside top directory
                                std::string title_str,                // Histo title at header
                                std::string xlabel_str  = "",         // X-axis label
                                std::string ylabel_str  = "",         // Y-axis label
                                std::string legend_pos  = "TR",       // Legend position
                                double Ymin             = 0.0,        // Y-axis minimum: default is y=0
                                double Ymax             = 2.5,        // Y-axis maximum: default is automatic size
                                bool add_pot_info       = true,       // Add POT info?
                                bool data_stat_err_only = false)      // Data histo with stat errors only?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    TH1D* h_mc          = (TH1D*)h_input_mc->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_QE       = (TH1D*)h_input_mc_QE->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_MEC      = (TH1D*)h_input_mc_MEC->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_DeltaRES = (TH1D*)h_input_mc_DeltaRES->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_OtherRES = (TH1D*)h_input_mc_OtherRES->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_SoftDIS  = (TH1D*)h_input_mc_SoftDIS->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_TrueDIS  = (TH1D*)h_input_mc_TrueDIS->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_Other    = (TH1D*)h_input_mc_Other->GetCVHistoWithStatError().Clone("");
    
    PlotUtils::MnvH1D* mnvh_mc          = new PlotUtils::MnvH1D(*h_mc);
    PlotUtils::MnvH1D* mnvh_mc_QE       = new PlotUtils::MnvH1D(*h_mc_QE);
    PlotUtils::MnvH1D* mnvh_mc_MEC      = new PlotUtils::MnvH1D(*h_mc_MEC);
    PlotUtils::MnvH1D* mnvh_mc_DeltaRES = new PlotUtils::MnvH1D(*h_mc_DeltaRES);
    PlotUtils::MnvH1D* mnvh_mc_OtherRES = new PlotUtils::MnvH1D(*h_mc_OtherRES);
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS  = new PlotUtils::MnvH1D(*h_mc_SoftDIS);
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS  = new PlotUtils::MnvH1D(*h_mc_TrueDIS);
    PlotUtils::MnvH1D* mnvh_mc_Other    = new PlotUtils::MnvH1D(*h_mc_Other);
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    // Total MC
    mnvh_mc -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc -> SetLineWidth(5);
    mnvh_mc -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // MC components
    mnvh_mc_QE       -> SetLineColor(kCyan+3);
    mnvh_mc_MEC      -> SetLineColor(kYellow+1);
    mnvh_mc_DeltaRES -> SetLineColor(kAzure-4);
    mnvh_mc_OtherRES -> SetLineColor(kRed+2);
    mnvh_mc_SoftDIS  -> SetLineColor(kOrange+1);
    mnvh_mc_TrueDIS  -> SetLineColor(kViolet);
    mnvh_mc_Other    -> SetLineColor(kGray+1);
    
    mnvh_mc_QE       -> SetLineWidth(4);
    mnvh_mc_MEC      -> SetLineWidth(4);
    mnvh_mc_DeltaRES -> SetLineWidth(4);
    mnvh_mc_OtherRES -> SetLineWidth(4);
    mnvh_mc_SoftDIS  -> SetLineWidth(4);
    mnvh_mc_TrueDIS  -> SetLineWidth(4);
    mnvh_mc_Other    -> SetLineWidth(4);
    
    // Set MC error to zero
    for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
        mnvh_mc -> SetBinError(bin, 0.0);
    }
    
    // Bin width normalize
    mnvh_data        -> Scale(mnvh_data        -> GetNormBinWidth(), "width");
    mnvh_data_stat   -> Scale(mnvh_data_stat   -> GetNormBinWidth(), "width");
    mnvh_mc          -> Scale(mnvh_mc          -> GetNormBinWidth(), "width");
    mnvh_mc_QE       -> Scale(mnvh_mc_QE       -> GetNormBinWidth(), "width");
    mnvh_mc_MEC      -> Scale(mnvh_mc_MEC      -> GetNormBinWidth(), "width");
    mnvh_mc_DeltaRES -> Scale(mnvh_mc_DeltaRES -> GetNormBinWidth(), "width");
    mnvh_mc_OtherRES -> Scale(mnvh_mc_OtherRES -> GetNormBinWidth(), "width");
    mnvh_mc_SoftDIS  -> Scale(mnvh_mc_SoftDIS  -> GetNormBinWidth(), "width");
    mnvh_mc_TrueDIS  -> Scale(mnvh_mc_TrueDIS  -> GetNormBinWidth(), "width");
    mnvh_mc_Other    -> Scale(mnvh_mc_Other    -> GetNormBinWidth(), "width");
    
    // Get ratios
    mnvh_data        -> Divide(mnvh_data,        mnvh_mc);
    mnvh_data_stat   -> Divide(mnvh_data_stat,   mnvh_mc);
    mnvh_mc_QE       -> Divide(mnvh_mc_QE,       mnvh_mc);
    mnvh_mc_MEC      -> Divide(mnvh_mc_MEC,      mnvh_mc);
    mnvh_mc_DeltaRES -> Divide(mnvh_mc_DeltaRES, mnvh_mc);
    mnvh_mc_OtherRES -> Divide(mnvh_mc_OtherRES, mnvh_mc);
    mnvh_mc_SoftDIS  -> Divide(mnvh_mc_SoftDIS,  mnvh_mc);
    mnvh_mc_TrueDIS  -> Divide(mnvh_mc_TrueDIS,  mnvh_mc);
    mnvh_mc_Other    -> Divide(mnvh_mc_Other,    mnvh_mc);
    mnvh_mc          -> Divide(mnvh_mc,          mnvh_mc);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_Other, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_Other, ylabel_str);
    
    // Y-axis limits
    mnvh_mc_Other -> SetMinimum(Ymin);
    mnvh_mc_Other -> SetMaximum(Ymax);
    
    
    // Draw
    // ====
    
    const TAxis* axis = mnvh_mc_Other->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(1);
    line.SetLineWidth(5);
    line.SetLineColor(plot_info.m_mnv_plotter.mc_color);
    
    mnvh_mc_Other    -> Draw("HIST L");
    mnvh_mc_MEC      -> DrawCopy("SAME HIST L");
    mnvh_mc_QE       -> DrawCopy("SAME HIST L");
    mnvh_mc_TrueDIS  -> DrawCopy("SAME HIST L");
    mnvh_mc_SoftDIS  -> DrawCopy("SAME HIST L");
    mnvh_mc_OtherRES -> DrawCopy("SAME HIST L");
    mnvh_mc_DeltaRES -> DrawCopy("SAME HIST L");
    
    line.DrawLine(lowX, 1.0, highX, 1.0);
    mnvh_data        -> DrawCopy("SAME E1 X0");
    mnvh_data_stat   -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_DeltaRES);
    h_vector.push_back(mnvh_mc_OtherRES);
    h_vector.push_back(mnvh_mc_SoftDIS);
    h_vector.push_back(mnvh_mc_TrueDIS);
    h_vector.push_back(mnvh_mc_QE);
    h_vector.push_back(mnvh_mc_MEC);
    h_vector.push_back(mnvh_mc_Other);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("Delta RES");
    name_vector.push_back("Other RES");
    name_vector.push_back("'Soft' DIS");
    name_vector.push_back("'True' DIS");
    name_vector.push_back("QE");
    name_vector.push_back("MEC");
    name_vector.push_back("Other");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.legend_n_columns = 2;
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.52, 0.73, 0.40, 0.17, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    plot_info.m_mnv_plotter.legend_n_columns = 1;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_DeltaRES;
    delete mnvh_mc_OtherRES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
}



// ==============================================================================================
//  Plot data-MC model ratio with interaction types (from FlatTrees)
// ==============================================================================================

void PlotDataMCRatio_XsecModels(PlotInfo plot_info,
                                PlotUtils::MnvH1D* h_input_data,      // Data histo
                                PlotUtils::MnvH1D* h_input_mc,        // MC histo
                                PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                                PlotUtils::MnvH1D* h_input_mc_MEC, 
                                PlotUtils::MnvH1D* h_input_mc_RES,
                                PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                                PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                                PlotUtils::MnvH1D* h_input_mc_Other,
                                std::string output_str,               // Output location inside top directory
                                std::string title_str,                // Histo title at header
                                std::string xlabel_str  = "",         // X-axis label
                                std::string ylabel_str  = "",         // Y-axis label
                                std::string legend_pos  = "TR",       // Legend position
                                double Ymin             = 0.0,        // Y-axis minimum: default is y=0
                                double Ymax             = 2.5,        // Y-axis maximum: default is automatic size
                                bool add_pot_info       = true,       // Add POT info?
                                bool data_stat_err_only = false)      // Data histo with stat errors only?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    TH1D* h_mc         = (TH1D*)h_input_mc->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_QE      = (TH1D*)h_input_mc_QE->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_MEC     = (TH1D*)h_input_mc_MEC->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_RES     = (TH1D*)h_input_mc_RES->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_SoftDIS = (TH1D*)h_input_mc_SoftDIS->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_TrueDIS = (TH1D*)h_input_mc_TrueDIS->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_Other   = (TH1D*)h_input_mc_Other->GetCVHistoWithStatError().Clone("");
    
    PlotUtils::MnvH1D* mnvh_mc         = new PlotUtils::MnvH1D(*h_mc);
    PlotUtils::MnvH1D* mnvh_mc_QE      = new PlotUtils::MnvH1D(*h_mc_QE);
    PlotUtils::MnvH1D* mnvh_mc_MEC     = new PlotUtils::MnvH1D(*h_mc_MEC);
    PlotUtils::MnvH1D* mnvh_mc_RES     = new PlotUtils::MnvH1D(*h_mc_RES);
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS = new PlotUtils::MnvH1D(*h_mc_SoftDIS);
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS = new PlotUtils::MnvH1D(*h_mc_TrueDIS);
    PlotUtils::MnvH1D* mnvh_mc_Other   = new PlotUtils::MnvH1D(*h_mc_Other);
    
    // Set MC error to zero
    for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
        mnvh_mc -> SetBinError(bin, 0.0);
    }
    
    // Bin width normalize
    mnvh_data       -> Scale(mnvh_data       -> GetNormBinWidth(), "width");
    mnvh_data_stat  -> Scale(mnvh_data_stat  -> GetNormBinWidth(), "width");
    mnvh_mc         -> Scale(mnvh_mc         -> GetNormBinWidth(), "width");
    mnvh_mc_QE      -> Scale(mnvh_mc_QE      -> GetNormBinWidth(), "width");
    mnvh_mc_MEC     -> Scale(mnvh_mc_MEC     -> GetNormBinWidth(), "width");
    mnvh_mc_RES     -> Scale(mnvh_mc_RES     -> GetNormBinWidth(), "width");
    mnvh_mc_SoftDIS -> Scale(mnvh_mc_SoftDIS -> GetNormBinWidth(), "width");
    mnvh_mc_TrueDIS -> Scale(mnvh_mc_TrueDIS -> GetNormBinWidth(), "width");
    mnvh_mc_Other   -> Scale(mnvh_mc_Other   -> GetNormBinWidth(), "width");
    
    // Get ratios
    mnvh_data       -> Divide(mnvh_data,       mnvh_mc);
    mnvh_data_stat  -> Divide(mnvh_data_stat,  mnvh_mc);
    mnvh_mc_QE      -> Divide(mnvh_mc_QE,      mnvh_mc);
    mnvh_mc_MEC     -> Divide(mnvh_mc_MEC,     mnvh_mc);
    mnvh_mc_RES     -> Divide(mnvh_mc_RES,     mnvh_mc);
    mnvh_mc_SoftDIS -> Divide(mnvh_mc_SoftDIS, mnvh_mc);
    mnvh_mc_TrueDIS -> Divide(mnvh_mc_TrueDIS, mnvh_mc);
    mnvh_mc_Other   -> Divide(mnvh_mc_Other,   mnvh_mc);
    mnvh_mc         -> Divide(mnvh_mc,         mnvh_mc);
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    // Total MC
    mnvh_mc -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc -> SetLineWidth(5);
    mnvh_mc -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // MC components
    mnvh_mc_QE      -> SetLineColor(kCyan+3);
    mnvh_mc_MEC     -> SetLineColor(kYellow+1);
    mnvh_mc_RES     -> SetLineColor(kAzure-8);
    mnvh_mc_SoftDIS -> SetLineColor(kOrange+1);
    mnvh_mc_TrueDIS -> SetLineColor(kViolet);
    mnvh_mc_Other   -> SetLineColor(kGray+1);
    
    mnvh_mc_QE      -> SetLineWidth(4);
    mnvh_mc_MEC     -> SetLineWidth(4);
    mnvh_mc_RES     -> SetLineWidth(4);
    mnvh_mc_SoftDIS -> SetLineWidth(4);
    mnvh_mc_TrueDIS -> SetLineWidth(4);
    mnvh_mc_Other   -> SetLineWidth(4);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_Other, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_Other, ylabel_str);
    
    // Y-axis limits
    mnvh_mc_Other -> SetMinimum(Ymin);
    mnvh_mc_Other -> SetMaximum(Ymax);
    
    
    // Draw
    // ====
    
    const TAxis* axis = mnvh_mc_Other->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(1);
    line.SetLineWidth(5);
    line.SetLineColor(plot_info.m_mnv_plotter.mc_color);
    
    mnvh_mc_Other   -> Draw("HIST L");
    mnvh_mc_MEC     -> DrawCopy("SAME HIST L");
    mnvh_mc_QE      -> DrawCopy("SAME HIST L");
    mnvh_mc_TrueDIS -> DrawCopy("SAME HIST L");
    mnvh_mc_SoftDIS -> DrawCopy("SAME HIST L");
    mnvh_mc_RES     -> DrawCopy("SAME HIST L");
    
    line.DrawLine(lowX, 1.0, highX, 1.0);
    mnvh_data       -> DrawCopy("SAME E1 X0");
    mnvh_data_stat  -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_RES);
    h_vector.push_back(mnvh_mc_SoftDIS);
    h_vector.push_back(mnvh_mc_TrueDIS);
    h_vector.push_back(mnvh_mc_QE);
    h_vector.push_back(mnvh_mc_MEC);
    h_vector.push_back(mnvh_mc_Other);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("RES");
    name_vector.push_back("'Soft' DIS");
    name_vector.push_back("'True' DIS");
    name_vector.push_back("QE");
    name_vector.push_back("MEC");
    name_vector.push_back("Other");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.legend_n_columns = 2;
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.52, 0.73, 0.40, 0.17, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    plot_info.m_mnv_plotter.legend_n_columns = 1;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_RES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
}



// ==============================================================================================
//  Plot data-MC stacked with interaction types
// ==============================================================================================

void PlotDataStackedMC_XsecModels(PlotInfo plot_info,
                                  PlotUtils::MnvH1D* h_input_data,      // Data histo
                                  PlotUtils::MnvH1D* h_input_mc,        // MC histo of all models
                                  PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                                  PlotUtils::MnvH1D* h_input_mc_MEC, 
                                  PlotUtils::MnvH1D* h_input_mc_DeltaRES,
                                  PlotUtils::MnvH1D* h_input_mc_OtherRES,
                                  PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                                  PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                                  PlotUtils::MnvH1D* h_input_mc_Other,
                                  std::string output_str,               // Output location inside top directory
                                  std::string title_str,                // Histo title at header
                                  std::string xlabel_str  = "",         // X-axis label
                                  std::string ylabel_str  = "",         // Y-axis label
                                  std::string legend_pos  = "TR",       // Legend position
                                  double Ymin             = -1.0,       // Y-axis minimum: default is y=0
                                  double Ymax             = -1.0,       // Y-axis maximum: default is automatic size
                                  bool add_pot_info       = true,       // Add POT info?
                                  bool data_stat_err_only = false,      // Data histo with stat errors only?
                                  bool write_area_norm    = false)      // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_data        = (PlotUtils::MnvH1D*)h_input_data        -> Clone("mnvh_data");
    PlotUtils::MnvH1D* mnvh_mc          = (PlotUtils::MnvH1D*)h_input_mc          -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_QE       = (PlotUtils::MnvH1D*)h_input_mc_QE       -> Clone("mnvh_mc_QE");
    PlotUtils::MnvH1D* mnvh_mc_MEC      = (PlotUtils::MnvH1D*)h_input_mc_MEC      -> Clone("mnvh_mc_MEC");
    PlotUtils::MnvH1D* mnvh_mc_DeltaRES = (PlotUtils::MnvH1D*)h_input_mc_DeltaRES -> Clone("mnvh_mc_DeltaRES");
    PlotUtils::MnvH1D* mnvh_mc_OtherRES = (PlotUtils::MnvH1D*)h_input_mc_OtherRES -> Clone("mnvh_mc_OtherRES");
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS  = (PlotUtils::MnvH1D*)h_input_mc_SoftDIS  -> Clone("mnvh_mc_SoftDIS");
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS  = (PlotUtils::MnvH1D*)h_input_mc_TrueDIS  -> Clone("mnvh_mc_TrueDIS");
    PlotUtils::MnvH1D* mnvh_mc_Other    = (PlotUtils::MnvH1D*)h_input_mc_Other    -> Clone("mnvh_mc_Other");
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc          = mnvh_mc          -> Integral(0, mnvh_mc          -> GetNbinsX()+1);
    double area_mc_QE       = mnvh_mc_QE       -> Integral(0, mnvh_mc_QE       -> GetNbinsX()+1);
    double area_mc_MEC      = mnvh_mc_MEC      -> Integral(0, mnvh_mc_MEC      -> GetNbinsX()+1);
    double area_mc_DeltaRES = mnvh_mc_DeltaRES -> Integral(0, mnvh_mc_DeltaRES -> GetNbinsX()+1);
    double area_mc_OtherRES = mnvh_mc_OtherRES -> Integral(0, mnvh_mc_OtherRES -> GetNbinsX()+1);
    double area_mc_SoftDIS  = mnvh_mc_SoftDIS  -> Integral(0, mnvh_mc_SoftDIS  -> GetNbinsX()+1);
    double area_mc_TrueDIS  = mnvh_mc_TrueDIS  -> Integral(0, mnvh_mc_TrueDIS  -> GetNbinsX()+1);
    double area_mc_Other    = mnvh_mc_Other    -> Integral(0, mnvh_mc_Other    -> GetNbinsX()+1);
    
    legend_label = GetTruthClassification_LegendLabel(kCCQE) + Form(" (%.1f%%)", 100.0*(area_mc_QE/area_mc));
    mnvh_mc_QE -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kCCMEC) + Form(" (%.1f%%)", 100.0*(area_mc_MEC/area_mc));
    mnvh_mc_MEC -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kCCDeltaRES) + Form(" (%.1f%%)", 100.0*(area_mc_DeltaRES/area_mc));
    mnvh_mc_DeltaRES -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kCCOtherRES) + Form(" (%.1f%%)", 100.0*(area_mc_OtherRES/area_mc));
    mnvh_mc_OtherRES -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kCCSoftDIS) + Form(" (%.1f%%)", 100.0*(area_mc_SoftDIS/area_mc));
    mnvh_mc_SoftDIS -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kCCTrueDIS) + Form(" (%.1f%%)", 100.0*(area_mc_TrueDIS/area_mc));
    mnvh_mc_TrueDIS -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kOtherIntType) + Form(" (%.1f%%)", 100.0*(area_mc_Other/area_mc));
    mnvh_mc_Other -> SetTitle(legend_label.c_str());
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        mnvh_data        -> Scale(1.0/area_data);
        mnvh_mc          -> Scale(1.0/area_mc);
        mnvh_mc_QE       -> Scale(1.0/area_mc);
        mnvh_mc_MEC      -> Scale(1.0/area_mc);
        mnvh_mc_DeltaRES -> Scale(1.0/area_mc);
        mnvh_mc_OtherRES -> Scale(1.0/area_mc);
        mnvh_mc_SoftDIS  -> Scale(1.0/area_mc);
        mnvh_mc_TrueDIS  -> Scale(1.0/area_mc);
        mnvh_mc_Other    -> Scale(1.0/area_mc);
    }
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_QE,       int(kCCQE),         4);
    SetHistColorScheme(mnvh_mc_MEC,      int(kCCMEC),        4);
    SetHistColorScheme(mnvh_mc_DeltaRES, int(kCCDeltaRES),   4);
    SetHistColorScheme(mnvh_mc_OtherRES, int(kCCOtherRES),   4);
    SetHistColorScheme(mnvh_mc_SoftDIS,  int(kCCSoftDIS),    4);
    SetHistColorScheme(mnvh_mc_TrueDIS,  int(kCCTrueDIS),    4);
    SetHistColorScheme(mnvh_mc_Other,    int(kOtherIntType), 4);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Other);
    h_mc_array -> Add(mnvh_mc_MEC);
    h_mc_array -> Add(mnvh_mc_QE);
    h_mc_array -> Add(mnvh_mc_TrueDIS);
    h_mc_array -> Add(mnvh_mc_SoftDIS);
    h_mc_array -> Add(mnvh_mc_OtherRES);
    h_mc_array -> Add(mnvh_mc_DeltaRES);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_data);
    else                    plot_info.SetXLabel(mnvh_data, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_data, mnvh_data->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_data, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    const char* x_label = mnvh_data->GetXaxis()->GetTitle();
    const char* y_label = mnvh_data->GetYaxis()->GetTitle();
    
    plot_info.m_mnv_plotter.height_nspaces_per_hist = 1.2;
    plot_info.m_mnv_plotter.DrawDataStackedMC(mnvh_data,                 // Data histogram
                                              h_mc_array,                // MC array histogram
                                              plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                              legend_pos,                // Legend position
                                              "Data",                    // Data histogram name
                                              -1, -1,                    // MC base color and color offset
                                              1001,                      // MC fill style
                                              x_label,                   // X-axis label
                                              y_label);                  // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.51, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.775, 0.51, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.height_nspaces_per_hist = 1.1;
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_DeltaRES;
    delete mnvh_mc_OtherRES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
    delete h_mc_array;
}



// ==============================================================================================
//  Plot data-MC stacked with interaction types (from FlatTrees)
// ==============================================================================================

void PlotDataStackedMC_XsecModels(PlotInfo plot_info,
                                  PlotUtils::MnvH1D* h_input_data,      // Data histo
                                  PlotUtils::MnvH1D* h_input_mc,        // MC histo of all models
                                  PlotUtils::MnvH1D* h_input_mc_QE,     // MC histos per model
                                  PlotUtils::MnvH1D* h_input_mc_MEC, 
                                  PlotUtils::MnvH1D* h_input_mc_RES,
                                  PlotUtils::MnvH1D* h_input_mc_SoftDIS,
                                  PlotUtils::MnvH1D* h_input_mc_TrueDIS,
                                  PlotUtils::MnvH1D* h_input_mc_Other,
                                  std::string output_str,               // Output location inside top directory
                                  std::string title_str,                // Histo title at header
                                  std::string xlabel_str  = "",         // X-axis label
                                  std::string ylabel_str  = "",         // Y-axis label
                                  std::string legend_pos  = "TR",       // Legend position
                                  double Ymin             = -1.0,       // Y-axis minimum: default is y=0
                                  double Ymax             = -1.0,       // Y-axis maximum: default is automatic size
                                  bool add_pot_info       = true,       // Add POT info?
                                  bool data_stat_err_only = false,      // Data histo with stat errors only?
                                  bool write_area_norm    = false)      // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_data       = (PlotUtils::MnvH1D*)h_input_data       -> Clone("mnvh_data");
    PlotUtils::MnvH1D* mnvh_mc         = (PlotUtils::MnvH1D*)h_input_mc         -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_QE      = (PlotUtils::MnvH1D*)h_input_mc_QE      -> Clone("mnvh_mc_QE");
    PlotUtils::MnvH1D* mnvh_mc_MEC     = (PlotUtils::MnvH1D*)h_input_mc_MEC     -> Clone("mnvh_mc_MEC");
    PlotUtils::MnvH1D* mnvh_mc_RES     = (PlotUtils::MnvH1D*)h_input_mc_RES     -> Clone("mnvh_mc_RES");
    PlotUtils::MnvH1D* mnvh_mc_SoftDIS = (PlotUtils::MnvH1D*)h_input_mc_SoftDIS -> Clone("mnvh_mc_SoftDIS");
    PlotUtils::MnvH1D* mnvh_mc_TrueDIS = (PlotUtils::MnvH1D*)h_input_mc_TrueDIS -> Clone("mnvh_mc_TrueDIS");
    PlotUtils::MnvH1D* mnvh_mc_Other   = (PlotUtils::MnvH1D*)h_input_mc_Other   -> Clone("mnvh_mc_Other");
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc         = mnvh_mc         -> Integral(0, mnvh_mc         -> GetNbinsX()+1);
    double area_mc_QE      = mnvh_mc_QE      -> Integral(0, mnvh_mc_QE      -> GetNbinsX()+1);
    double area_mc_MEC     = mnvh_mc_MEC     -> Integral(0, mnvh_mc_MEC     -> GetNbinsX()+1);
    double area_mc_RES     = mnvh_mc_RES     -> Integral(0, mnvh_mc_RES     -> GetNbinsX()+1);
    double area_mc_SoftDIS = mnvh_mc_SoftDIS -> Integral(0, mnvh_mc_SoftDIS -> GetNbinsX()+1);
    double area_mc_TrueDIS = mnvh_mc_TrueDIS -> Integral(0, mnvh_mc_TrueDIS -> GetNbinsX()+1);
    double area_mc_Other   = mnvh_mc_Other   -> Integral(0, mnvh_mc_Other   -> GetNbinsX()+1);
    
    legend_label = Form("QE (%.1f%%)", 100.0*(area_mc_QE/area_mc));
    mnvh_mc_QE -> SetTitle(legend_label.c_str());
    
    legend_label = Form("MEC (%.1f%%)", 100.0*(area_mc_MEC/area_mc));
    mnvh_mc_MEC -> SetTitle(legend_label.c_str());
    
    legend_label = Form("RES (%.1f%%)", 100.0*(area_mc_RES/area_mc));
    mnvh_mc_RES -> SetTitle(legend_label.c_str());
    
    legend_label = Form("'Soft' DIS (%.1f%%)", 100.0*(area_mc_SoftDIS/area_mc));
    mnvh_mc_SoftDIS -> SetTitle(legend_label.c_str());
    
    legend_label = Form("'True' DIS (%.1f%%)", 100.0*(area_mc_TrueDIS/area_mc));
    mnvh_mc_TrueDIS -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Other (%.1f%%)", 100.0*(area_mc_Other/area_mc));
    mnvh_mc_Other -> SetTitle(legend_label.c_str());
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        mnvh_data       -> Scale(1.0/area_data);
        mnvh_mc         -> Scale(1.0/area_mc);
        mnvh_mc_QE      -> Scale(1.0/area_mc);
        mnvh_mc_MEC     -> Scale(1.0/area_mc);
        mnvh_mc_RES     -> Scale(1.0/area_mc);
        mnvh_mc_SoftDIS -> Scale(1.0/area_mc);
        mnvh_mc_TrueDIS -> Scale(1.0/area_mc);
        mnvh_mc_Other   -> Scale(1.0/area_mc);
    }
    
    // Set MC histogram colors
    mnvh_mc_QE      -> SetFillColor(kCyan+3);
    mnvh_mc_MEC     -> SetFillColor(kYellow+1);
    mnvh_mc_RES     -> SetFillColor(kAzure-8);
    mnvh_mc_SoftDIS -> SetFillColor(kOrange);
    mnvh_mc_TrueDIS -> SetFillColor(kViolet);
    mnvh_mc_Other   -> SetFillColor(kGray+1);
    
    mnvh_mc_QE      -> SetLineColor(kCyan+3);
    mnvh_mc_MEC     -> SetLineColor(kYellow+1);
    mnvh_mc_RES     -> SetLineColor(kAzure-8);
    mnvh_mc_SoftDIS -> SetLineColor(kOrange);
    mnvh_mc_TrueDIS -> SetLineColor(kViolet);
    mnvh_mc_Other   -> SetLineColor(kGray+1);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Other);
    h_mc_array -> Add(mnvh_mc_MEC);
    h_mc_array -> Add(mnvh_mc_QE);
    h_mc_array -> Add(mnvh_mc_TrueDIS);
    h_mc_array -> Add(mnvh_mc_SoftDIS);
    h_mc_array -> Add(mnvh_mc_RES);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_data);
    else                    plot_info.SetXLabel(mnvh_data, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_data, mnvh_data->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_data, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_mc->GetBinContent(bin);
            double bin_error   = mnvh_mc->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_data -> SetMinimum(Ymin_input);
            mnvh_mc   -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_data -> SetMaximum(Ymax);
        mnvh_mc   -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        mnvh_data_dummy -> Scale(mnvh_data_dummy->GetNormBinWidth(), "width");
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    const char* x_label = mnvh_data->GetXaxis()->GetTitle();
    const char* y_label = mnvh_data->GetYaxis()->GetTitle();
    
    plot_info.m_mnv_plotter.height_nspaces_per_hist = 1.2;
    plot_info.m_mnv_plotter.DrawDataStackedMC(mnvh_data,                 // Data histogram
                                              h_mc_array,                // MC array histogram
                                              plot_info.m_mc_pot_scale,  // MC already POT-normalized
                                              legend_pos,                // Legend position
                                              "Data",                    // Data histogram name
                                              -1, -1,                    // MC base color and color offset
                                              1001,                      // MC fill style
                                              x_label,                   // X-axis label
                                              y_label);                  // Y-axis label
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.77, 0.55, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.77, 0.55, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.height_nspaces_per_hist = 1.1;
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_QE;
    delete mnvh_mc_MEC;
    delete mnvh_mc_RES;
    delete mnvh_mc_SoftDIS;
    delete mnvh_mc_TrueDIS;
    delete mnvh_mc_Other;
    delete h_mc_array;
}



// ==============================================================================================
//  Plot data and multiple MC models -- VERSION 1
// ==============================================================================================

void PlotDataMC_FullXsecModels(PlotInfo plot_info,
                               PlotUtils::MnvH1D* h_input_data,            // Data histo
                               PlotUtils::MnvH1D* h_input_mc_v1,           // MC models
                               PlotUtils::MnvH1D* h_input_mc_v0,
                               PlotUtils::MnvH1D* h_input_mc_v1noNonResPi, 
                               PlotUtils::MnvH1D* h_input_mc_v1noD2,
                               PlotUtils::MnvH1D* h_input_mc_v1noPionTune,
                               std::string output_str,                     // Output location inside top directory
                               std::string title_str,                      // Histo title at header
                               std::string xlabel_str  = "",               // X-axis label
                               std::string ylabel_str  = "",               // Y-axis label
                               double Ymin             = -1.0,             // Y-axis minimum: default is y=0
                               double Ymax             = -1.0,             // Y-axis maximum: default is automatic size
                               bool add_pot_info       = true,             // Add POT info?
                               bool data_stat_err_only = false,            // Data histo with stat errors only?
                               bool write_area_norm    = false)            // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    PlotUtils::MnvH1D* mnvh_mc_v1           = (PlotUtils::MnvH1D*)h_input_mc_v1           -> Clone("mnvh_mc_v1");
    PlotUtils::MnvH1D* mnvh_mc_v0           = (PlotUtils::MnvH1D*)h_input_mc_v0           -> Clone("mnvh_mc_v0");
    PlotUtils::MnvH1D* mnvh_mc_v1noNonResPi = (PlotUtils::MnvH1D*)h_input_mc_v1noNonResPi -> Clone("mnvh_mc_v1noNonResPi");
    PlotUtils::MnvH1D* mnvh_mc_v1noD2       = (PlotUtils::MnvH1D*)h_input_mc_v1noD2       -> Clone("mnvh_mc_v1noD2");
    PlotUtils::MnvH1D* mnvh_mc_v1noPionTune = (PlotUtils::MnvH1D*)h_input_mc_v1noPionTune -> Clone("mnvh_mc_v1noPionTune");
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data             = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        double area_mc_v1            = mnvh_mc_v1->Integral(0, mnvh_mc_v1->GetNbinsX()+1);
        double area_mc_v0            = mnvh_mc_v0->Integral(0, mnvh_mc_v0->GetNbinsX()+1);
        double area_mc_v1nonNonResPi = mnvh_mc_v1noNonResPi->Integral(0, mnvh_mc_v1noNonResPi->GetNbinsX()+1);
        double area_mc_v1noD2        = mnvh_mc_v1noD2->Integral(0, mnvh_mc_v1noD2->GetNbinsX()+1);
        double area_mc_v1noPionTune  = mnvh_mc_v1noPionTune->Integral(0, mnvh_mc_v1noPionTune->GetNbinsX()+1);
        
        mnvh_data            -> Scale(1.0/area_data);
        mnvh_data_stat       -> Scale(1.0/area_data);
        mnvh_mc_v1           -> Scale(1.0/area_mc_v1);
        mnvh_mc_v0           -> Scale(1.0/area_mc_v0);
        mnvh_mc_v1noNonResPi -> Scale(1.0/area_mc_v1nonNonResPi);
        mnvh_mc_v1noD2       -> Scale(1.0/area_mc_v1noD2);
        mnvh_mc_v1noPionTune -> Scale(1.0/area_mc_v1noPionTune);
    }
    
    // Bin width normalize
    mnvh_data            -> Scale(mnvh_data            -> GetNormBinWidth(), "width");
    mnvh_data_stat       -> Scale(mnvh_data_stat       -> GetNormBinWidth(), "width");
    mnvh_mc_v1           -> Scale(mnvh_mc_v1           -> GetNormBinWidth(), "width");
    mnvh_mc_v0           -> Scale(mnvh_mc_v0           -> GetNormBinWidth(), "width");
    mnvh_mc_v1noNonResPi -> Scale(mnvh_mc_v1noNonResPi -> GetNormBinWidth(), "width");
    mnvh_mc_v1noD2       -> Scale(mnvh_mc_v1noD2       -> GetNormBinWidth(), "width");
    mnvh_mc_v1noPionTune -> Scale(mnvh_mc_v1noPionTune -> GetNormBinWidth(), "width");
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data_stat -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(4);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // Other models
    mnvh_mc_v0           -> SetLineColor(kCyan+2);
    mnvh_mc_v1noNonResPi -> SetLineColor(kOrange+1);
    mnvh_mc_v1noD2       -> SetLineColor(kBlue);
    mnvh_mc_v1noPionTune -> SetLineColor(kRed+2);
    
    mnvh_mc_v0           -> SetLineWidth(3);
    mnvh_mc_v1noNonResPi -> SetLineWidth(3);
    mnvh_mc_v1noD2       -> SetLineWidth(3);
    mnvh_mc_v1noPionTune -> SetLineWidth(3);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_v0, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_v0, ylabel_str);
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc_v0 -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc_v0 -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_v0 -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_v0 -> SetMaximum(Ymax);
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    mnvh_mc_v0           -> Draw("HIST L");
    mnvh_mc_v1noPionTune -> DrawCopy("SAME HIST L");
    mnvh_mc_v1noD2       -> DrawCopy("SAME HIST L");
    mnvh_mc_v1noNonResPi -> DrawCopy("SAME HIST L");
    mnvh_mc_v1           -> DrawCopy("SAME HIST L");
    mnvh_data            -> DrawCopy("SAME E1 X0");
    mnvh_data_stat       -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_v0);
    h_vector.push_back(mnvh_mc_v1);
    h_vector.push_back(mnvh_mc_v1noNonResPi);
    h_vector.push_back(mnvh_mc_v1noD2);
    h_vector.push_back(mnvh_mc_v1noPionTune);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("GENIE 2.12.6");
    name_vector.push_back("MINER#nuA Tune 4.0.1");
    name_vector.push_back("4.0.1 w/o non-RES #pi");
    name_vector.push_back("4.0.1 w/o deuterium #pi");
    name_vector.push_back("4.0.1 w/o any #pi tune");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.58, 0.61, 0.35, 0.29, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.55, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.55, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v0;
    delete mnvh_mc_v1;
    delete mnvh_mc_v1noNonResPi;
    delete mnvh_mc_v1noD2;
    delete mnvh_mc_v1noPionTune;
}



// ==============================================================================================
//  Plot data and multiple MC models -- VERSION 2
// ==============================================================================================

void PlotDataMC_FullXsecModels(PlotInfo plot_info,
                               PlotUtils::MnvH1D* h_input_data,         // Data histo
                               PlotUtils::MnvH1D* h_input_mc_v1,        // MC models
                               PlotUtils::MnvH1D* h_input_mc_v2MINOS,
                               PlotUtils::MnvH1D* h_input_mc_v2JOINT, 
                               PlotUtils::MnvH1D* h_input_mc_v2NU1PI,
                               PlotUtils::MnvH1D* h_input_mc_v2NUNPI,
                               PlotUtils::MnvH1D* h_input_mc_v2NUPI0,
                               PlotUtils::MnvH1D* h_input_mc_v2MENU1PI,
                               std::string output_str,                  // Output location inside top directory
                               std::string title_str,                   // Histo title at header
                               std::string xlabel_str  = "",            // X-axis label
                               std::string ylabel_str  = "",            // Y-axis label
                               double Ymin             = -1.0,          // Y-axis minimum: default is y=0
                               double Ymax             = -1.0,          // Y-axis maximum: default is automatic size
                               bool add_pot_info       = true,          // Add POT info?
                               bool data_stat_err_only = false,         // Data histo with stat errors only?
                               bool write_area_norm    = false)         // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    PlotUtils::MnvH1D* mnvh_mc_v1        = (PlotUtils::MnvH1D*)h_input_mc_v1        -> Clone("mnvh_mc_v1");
    PlotUtils::MnvH1D* mnvh_mc_v2MINOS   = (PlotUtils::MnvH1D*)h_input_mc_v2MINOS   -> Clone("mnvh_mc_v2MINOS");
    PlotUtils::MnvH1D* mnvh_mc_v2JOINT   = (PlotUtils::MnvH1D*)h_input_mc_v2JOINT   -> Clone("mnvh_mc_v2JOINT");
    PlotUtils::MnvH1D* mnvh_mc_v2NU1PI   = (PlotUtils::MnvH1D*)h_input_mc_v2NU1PI   -> Clone("mnvh_mc_v2NU1PI");
    PlotUtils::MnvH1D* mnvh_mc_v2NUNPI   = (PlotUtils::MnvH1D*)h_input_mc_v2NUNPI   -> Clone("mnvh_mc_v2NUNPI");
    PlotUtils::MnvH1D* mnvh_mc_v2NUPI0   = (PlotUtils::MnvH1D*)h_input_mc_v2NUPI0   -> Clone("mnvh_mc_v2NUPI0");
    PlotUtils::MnvH1D* mnvh_mc_v2MENU1PI = (PlotUtils::MnvH1D*)h_input_mc_v2MENU1PI -> Clone("mnvh_mc_v2MENU1PI");
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data         = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        double area_mc_v1        = mnvh_mc_v1->Integral(0, mnvh_mc_v1->GetNbinsX()+1);
        double area_mc_v2MINOS   = mnvh_mc_v2MINOS->Integral(0, mnvh_mc_v2MINOS->GetNbinsX()+1);
        double area_mc_v2JOINT   = mnvh_mc_v2JOINT->Integral(0, mnvh_mc_v2JOINT->GetNbinsX()+1);
        double area_mc_v2NU1PI   = mnvh_mc_v2NU1PI->Integral(0, mnvh_mc_v2NU1PI->GetNbinsX()+1);
        double area_mc_v2NUNPI   = mnvh_mc_v2NUNPI->Integral(0, mnvh_mc_v2NUNPI->GetNbinsX()+1);
        double area_mc_v2NUPI0   = mnvh_mc_v2NUPI0->Integral(0, mnvh_mc_v2NUPI0->GetNbinsX()+1);
        double area_mc_v2MENU1PI = mnvh_mc_v2MENU1PI->Integral(0, mnvh_mc_v2MENU1PI->GetNbinsX()+1);
        
        mnvh_data         -> Scale(1.0/area_data);
        mnvh_data_stat    -> Scale(1.0/area_data);
        mnvh_mc_v1        -> Scale(1.0/area_mc_v1);
        mnvh_mc_v2MINOS   -> Scale(1.0/area_mc_v2MINOS);
        mnvh_mc_v2JOINT   -> Scale(1.0/area_mc_v2JOINT);
        mnvh_mc_v2NU1PI   -> Scale(1.0/area_mc_v2NU1PI);
        mnvh_mc_v2NUNPI   -> Scale(1.0/area_mc_v2NUNPI);
        mnvh_mc_v2NUPI0   -> Scale(1.0/area_mc_v2NUPI0);
        mnvh_mc_v2MENU1PI -> Scale(1.0/area_mc_v2MENU1PI);
    }
    
    // Bin width normalize
    mnvh_data         -> Scale(mnvh_data         -> GetNormBinWidth(), "width");
    mnvh_data_stat    -> Scale(mnvh_data_stat    -> GetNormBinWidth(), "width");
    mnvh_mc_v1        -> Scale(mnvh_mc_v1        -> GetNormBinWidth(), "width");
    mnvh_mc_v2MINOS   -> Scale(mnvh_mc_v2MINOS   -> GetNormBinWidth(), "width");
    mnvh_mc_v2JOINT   -> Scale(mnvh_mc_v2JOINT   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NU1PI   -> Scale(mnvh_mc_v2NU1PI   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NUNPI   -> Scale(mnvh_mc_v2NUNPI   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NUPI0   -> Scale(mnvh_mc_v2NUPI0   -> GetNormBinWidth(), "width");
    mnvh_mc_v2MENU1PI -> Scale(mnvh_mc_v2MENU1PI -> GetNormBinWidth(), "width");
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data_stat -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(4);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // Other models
    mnvh_mc_v2MINOS   -> SetLineColor(kCyan+1);
    mnvh_mc_v2JOINT   -> SetLineColor(kRed+2);
    mnvh_mc_v2NU1PI   -> SetLineColor(kBlue);
    mnvh_mc_v2NUNPI   -> SetLineColor(kOrange+1);
    mnvh_mc_v2NUPI0   -> SetLineColor(kGreen+2);
    mnvh_mc_v2MENU1PI -> SetLineColor(kViolet-1);
    
    mnvh_mc_v2MINOS   -> SetLineWidth(3);
    mnvh_mc_v2JOINT   -> SetLineWidth(3);
    mnvh_mc_v2NU1PI   -> SetLineWidth(3);
    mnvh_mc_v2NUNPI   -> SetLineWidth(3);
    mnvh_mc_v2NUPI0   -> SetLineWidth(3);
    mnvh_mc_v2MENU1PI -> SetLineWidth(3);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_v2MINOS, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_v2MINOS, ylabel_str);
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc_v2MINOS -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc_v2MINOS -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_v2MINOS -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_v2MINOS -> SetMaximum(Ymax);
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    mnvh_mc_v2MINOS   -> Draw("HIST L");
    mnvh_mc_v2JOINT   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NU1PI   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NUNPI   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NUPI0   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2MENU1PI -> DrawCopy("SAME HIST L");
    mnvh_mc_v1        -> DrawCopy("SAME HIST L");
    mnvh_data         -> DrawCopy("SAME E1 X0");
    mnvh_data_stat    -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_v1);
    h_vector.push_back(mnvh_mc_v2MINOS);
    h_vector.push_back(mnvh_mc_v2JOINT);
    h_vector.push_back(mnvh_mc_v2NU1PI);
    h_vector.push_back(mnvh_mc_v2NUNPI);
    h_vector.push_back(mnvh_mc_v2NUPI0);
    h_vector.push_back(mnvh_mc_v2MENU1PI);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("MINER#nuA Tune 4.0.1");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'MINOS'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'JOINT'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NU1PI'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NUNPI'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NUPI0'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'MENU1PI'");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.54, 0.56, 0.40, 0.34, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.73, 0.51, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.73, 0.51, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v1;
    delete mnvh_mc_v2MINOS;
    delete mnvh_mc_v2JOINT;
    delete mnvh_mc_v2NU1PI;
    delete mnvh_mc_v2NUNPI;
    delete mnvh_mc_v2NUPI0;
    delete mnvh_mc_v2MENU1PI;
}



// ==============================================================================================
//  Plot data and multiple MC models -- VERSION 3
// ==============================================================================================

void PlotDataMC_FullXsecModels(PlotInfo plot_info,
                               PlotUtils::MnvH1D* h_input_data,         // Data histo
                               PlotUtils::MnvH1D* h_input_mc_v1,        // MC models
                               PlotUtils::MnvH1D* h_input_mc_GENIE3_02a,
                               PlotUtils::MnvH1D* h_input_mc_GENIE3_02b, 
                               PlotUtils::MnvH1D* h_input_mc_GENIE3_10a,
                               PlotUtils::MnvH1D* h_input_mc_GENIE3_10b,
                               PlotUtils::MnvH1D* h_input_mc_NEUT_LFG,
                               std::string output_str,                  // Output location inside top directory
                               std::string title_str,                   // Histo title at header
                               std::string xlabel_str  = "",            // X-axis label
                               std::string ylabel_str  = "",            // Y-axis label
                               double Ymin             = -1.0,          // Y-axis minimum: default is y=0
                               double Ymax             = -1.0,          // Y-axis maximum: default is automatic size
                               bool add_pot_info       = true,          // Add POT info?
                               bool data_stat_err_only = false,         // Data histo with stat errors only?
                               bool write_area_norm    = false)         // Write 'area normalized'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    PlotUtils::MnvH1D* mnvh_mc_v1         = (PlotUtils::MnvH1D*)h_input_mc_v1         -> Clone("mnvh_mc_v1");
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_02a = (PlotUtils::MnvH1D*)h_input_mc_GENIE3_02a -> Clone("mnvh_mc_GENIE3_02a");
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_02b = (PlotUtils::MnvH1D*)h_input_mc_GENIE3_02b -> Clone("mnvh_mc_GENIE3_02b");
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_10a = (PlotUtils::MnvH1D*)h_input_mc_GENIE3_10a -> Clone("mnvh_mc_GENIE3_10a");
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_10b = (PlotUtils::MnvH1D*)h_input_mc_GENIE3_10b -> Clone("mnvh_mc_GENIE3_10b");
    PlotUtils::MnvH1D* mnvh_mc_NEUT_LFG   = (PlotUtils::MnvH1D*)h_input_mc_NEUT_LFG   -> Clone("mnvh_mc_NEUT_LFG");
    
    // Area-normalize (if needed)
    if ( write_area_norm ) {
        double area_data          = mnvh_data->Integral(0, mnvh_data->GetNbinsX()+1);
        double area_mc_v1         = mnvh_mc_v1->Integral(0, mnvh_mc_v1->GetNbinsX()+1);
        double area_mc_GENIE3_02a = mnvh_mc_GENIE3_02a->Integral(0, mnvh_mc_GENIE3_02a->GetNbinsX()+1);
        double area_mc_GENIE3_02b = mnvh_mc_GENIE3_02b->Integral(0, mnvh_mc_GENIE3_02b->GetNbinsX()+1);
        double area_mc_GENIE3_10a = mnvh_mc_GENIE3_10a->Integral(0, mnvh_mc_GENIE3_10a->GetNbinsX()+1);
        double area_mc_GENIE3_10b = mnvh_mc_GENIE3_10b->Integral(0, mnvh_mc_GENIE3_10b->GetNbinsX()+1);
        double area_mc_NEUT_LFG   = mnvh_mc_NEUT_LFG->Integral(0, mnvh_mc_NEUT_LFG->GetNbinsX()+1);
        
        mnvh_data          -> Scale(1.0/area_data);
        mnvh_data_stat     -> Scale(1.0/area_data);
        mnvh_mc_v1         -> Scale(1.0/area_mc_v1);
        mnvh_mc_GENIE3_02a -> Scale(1.0/area_mc_GENIE3_02a);
        mnvh_mc_GENIE3_02b -> Scale(1.0/area_mc_GENIE3_02b);
        mnvh_mc_GENIE3_10a -> Scale(1.0/area_mc_GENIE3_10a);
        mnvh_mc_GENIE3_10b -> Scale(1.0/area_mc_GENIE3_10b);
        mnvh_mc_NEUT_LFG   -> Scale(1.0/area_mc_NEUT_LFG);
    }
    
    // Bin width normalize
    mnvh_data          -> Scale(mnvh_data          -> GetNormBinWidth(), "width");
    mnvh_data_stat     -> Scale(mnvh_data_stat     -> GetNormBinWidth(), "width");
    mnvh_mc_v1         -> Scale(mnvh_mc_v1         -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_02a -> Scale(mnvh_mc_GENIE3_02a -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_02b -> Scale(mnvh_mc_GENIE3_02b -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_10a -> Scale(mnvh_mc_GENIE3_10a -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_10b -> Scale(mnvh_mc_GENIE3_10b -> GetNormBinWidth(), "width");
    mnvh_mc_NEUT_LFG   -> Scale(mnvh_mc_NEUT_LFG   -> GetNormBinWidth(), "width");
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.data_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.data_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.data_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.data_line_width);
    mnvh_data_stat -> SetLineStyle(plot_info.m_mnv_plotter.data_line_style);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.data_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(4);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // Other models
    mnvh_mc_GENIE3_02a -> SetLineColor(kCyan+1);
    mnvh_mc_GENIE3_02b -> SetLineColor(kRed+2);
    mnvh_mc_GENIE3_10a -> SetLineColor(kBlue);
    mnvh_mc_GENIE3_10b -> SetLineColor(kOrange+1);
    mnvh_mc_NEUT_LFG   -> SetLineColor(kGreen+2);
    
    mnvh_mc_GENIE3_02a -> SetLineWidth(3);
    mnvh_mc_GENIE3_02b -> SetLineWidth(3);
    mnvh_mc_GENIE3_10a -> SetLineWidth(3);
    mnvh_mc_GENIE3_10b -> SetLineWidth(3);
    mnvh_mc_NEUT_LFG   -> SetLineWidth(3);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_NEUT_LFG, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_NEUT_LFG, ylabel_str);
    
    // Y-axis minimum
    if ( Ymin != -1.0 ) {
        plot_info.m_mnv_plotter.hist_min_zero = false;
        plot_info.m_mnv_plotter.axis_minimum  = Ymin;
        mnvh_mc_NEUT_LFG -> SetMinimum(Ymin);
    }
    else {
        double Ymin_input = 0.0;
        for ( int bin = 1; bin <= mnvh_data->GetNbinsX(); ++bin ) {
            double bin_content = mnvh_data->GetBinContent(bin);
            double bin_error   = mnvh_data->GetBinError(bin);
            if ( bin_content < 0.0 && (bin_content-bin_error) <= Ymin_input )
                Ymin_input = (bin_content-bin_error);
        }
        if ( Ymin_input < 0.0 ) {
            Ymin_input *= 2.0;
            plot_info.m_mnv_plotter.hist_min_zero = false;
            plot_info.m_mnv_plotter.axis_minimum  = Ymin_input;
            mnvh_mc_NEUT_LFG -> SetMinimum(Ymin_input);
        }
    }
    
    // Y-axis maximum
    if ( Ymax != -1.0 ) {
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_NEUT_LFG -> SetMaximum(Ymax);
    }
    else {
        PlotUtils::MnvH1D* mnvh_data_dummy = (PlotUtils::MnvH1D*)mnvh_data->Clone();
        double Ymax = plot_info.m_mnv_plotter.headroom * (mnvh_data_dummy->GetBinContent(mnvh_data_dummy->GetMaximumBin()));
        plot_info.m_mnv_plotter.axis_maximum = Ymax;
        mnvh_mc_NEUT_LFG -> SetMaximum(Ymax);
        delete mnvh_data_dummy;
    }
    
    
    // Draw
    // ====
    
    mnvh_mc_NEUT_LFG   -> Draw("HIST L");
    mnvh_mc_GENIE3_10b -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_10a -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_02b -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_02a -> DrawCopy("SAME HIST L");
    mnvh_mc_v1         -> DrawCopy("SAME HIST L");
    mnvh_data          -> DrawCopy("SAME E1 X0");
    mnvh_data_stat     -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_v1);
    h_vector.push_back(mnvh_mc_GENIE3_02a);
    h_vector.push_back(mnvh_mc_GENIE3_02b);
    h_vector.push_back(mnvh_mc_GENIE3_10a);
    h_vector.push_back(mnvh_mc_GENIE3_10b);
    h_vector.push_back(mnvh_mc_NEUT_LFG);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("MINER#nuA Tune 4.0.1");
    name_vector.push_back("GENIE 3.0.6 RFG #font[12]{hA}");
    name_vector.push_back("GENIE 3.0.6 RFG #font[12]{hN}");
    name_vector.push_back("GENIE 3.0.6 LFG #font[12]{hA}");
    name_vector.push_back("GENIE 3.0.6 LFG #font[12]{hN}");
    name_vector.push_back("NEUT 5.0.2 LFG");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.57, 0.58, 0.36, 0.32, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info && !write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.35, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.53, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.75, 0.53, 0.03, 1, 62);
    }
    
    // Area-normalized
    if ( write_area_norm ) {
        plot_info.m_mnv_plotter.WriteNorm("Prediction matched", 0.35, 0.91-0.05, 0.05);
        plot_info.m_mnv_plotter.WriteNorm("to data rate", 0.35, 0.91-0.10, 0.05);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v1;
    delete mnvh_mc_GENIE3_02a;
    delete mnvh_mc_GENIE3_02b;
    delete mnvh_mc_GENIE3_10a;
    delete mnvh_mc_GENIE3_10b;
    delete mnvh_mc_NEUT_LFG;
}



// ==============================================================================================
//  Plot data-MC ratio with multiple models - VERSION 1
// ==============================================================================================

void PlotDataMCRatio_FullXsecModels(PlotInfo plot_info,
                                    PlotUtils::MnvH1D* h_input_data,            // Data histo
                                    PlotUtils::MnvH1D* h_input_mc_v1,           // MC models
                                    PlotUtils::MnvH1D* h_input_mc_v0,
                                    PlotUtils::MnvH1D* h_input_mc_v1noNonResPi, 
                                    PlotUtils::MnvH1D* h_input_mc_v1noD2,
                                    PlotUtils::MnvH1D* h_input_mc_v1noPionTune,
                                    std::string output_str,                     // Output location inside top directory
                                    std::string title_str,                      // Histo title at header
                                    std::string xlabel_str  = "",               // X-axis label
                                    std::string ylabel_str  = "",               // Y-axis label
                                    double Ymin             = 0.0,              // Y-axis minimum: default is y=0
                                    double Ymax             = 2.5,              // Y-axis maximum: default is automatic size
                                    bool add_pot_info       = true,             // Add POT info?
                                    bool data_stat_err_only = false)            // Data histo with stat errors only?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    TH1D* h_mc_v1           = (TH1D*)h_input_mc_v1->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v0           = (TH1D*)h_input_mc_v0->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v1noNonResPi = (TH1D*)h_input_mc_v1noNonResPi->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v1noD2       = (TH1D*)h_input_mc_v1noD2->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v1noPionTune = (TH1D*)h_input_mc_v1noPionTune->GetCVHistoWithStatError().Clone("");
    
    PlotUtils::MnvH1D* mnvh_mc_v1           = new PlotUtils::MnvH1D(*h_mc_v1);
    PlotUtils::MnvH1D* mnvh_mc_v0           = new PlotUtils::MnvH1D(*h_mc_v0);
    PlotUtils::MnvH1D* mnvh_mc_v1noNonResPi = new PlotUtils::MnvH1D(*h_mc_v1noNonResPi);
    PlotUtils::MnvH1D* mnvh_mc_v1noD2       = new PlotUtils::MnvH1D(*h_mc_v1noD2);
    PlotUtils::MnvH1D* mnvh_mc_v1noPionTune = new PlotUtils::MnvH1D(*h_mc_v1noPionTune);
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(5);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    // Other models
    mnvh_mc_v0           -> SetLineColor(kCyan+2);
    mnvh_mc_v1noNonResPi -> SetLineColor(kOrange+1);
    mnvh_mc_v1noD2       -> SetLineColor(kBlue);
    mnvh_mc_v1noPionTune -> SetLineColor(kRed+2);
    
    mnvh_mc_v0           -> SetLineWidth(4);
    mnvh_mc_v1noNonResPi -> SetLineWidth(4);
    mnvh_mc_v1noD2       -> SetLineWidth(4);
    mnvh_mc_v1noPionTune -> SetLineWidth(4);
    
    // Set base model error to zero
    for ( int bin = 1; bin <= mnvh_mc_v1->GetNbinsX(); ++bin ) {
        mnvh_mc_v1 -> SetBinError(bin, 0.0);
    }
    
    // Bin width normalize
    mnvh_data            -> Scale(mnvh_data            -> GetNormBinWidth(), "width");
    mnvh_data_stat       -> Scale(mnvh_data_stat       -> GetNormBinWidth(), "width");
    mnvh_mc_v1           -> Scale(mnvh_mc_v1           -> GetNormBinWidth(), "width");
    mnvh_mc_v0           -> Scale(mnvh_mc_v0           -> GetNormBinWidth(), "width");
    mnvh_mc_v1noNonResPi -> Scale(mnvh_mc_v1noNonResPi -> GetNormBinWidth(), "width");
    mnvh_mc_v1noD2       -> Scale(mnvh_mc_v1noD2       -> GetNormBinWidth(), "width");
    mnvh_mc_v1noPionTune -> Scale(mnvh_mc_v1noPionTune -> GetNormBinWidth(), "width");
    
    // Get ratios
    mnvh_data            -> Divide(mnvh_data,            mnvh_mc_v1);
    mnvh_data_stat       -> Divide(mnvh_data_stat,       mnvh_mc_v1);
    mnvh_mc_v0           -> Divide(mnvh_mc_v0,           mnvh_mc_v1);
    mnvh_mc_v1noNonResPi -> Divide(mnvh_mc_v1noNonResPi, mnvh_mc_v1);
    mnvh_mc_v1noD2       -> Divide(mnvh_mc_v1noD2,       mnvh_mc_v1);
    mnvh_mc_v1noPionTune -> Divide(mnvh_mc_v1noPionTune, mnvh_mc_v1);
    mnvh_mc_v1           -> Divide(mnvh_mc_v1,           mnvh_mc_v1);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_v0, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_v0, ylabel_str);
    
    // Y-axis limits
    mnvh_mc_v0 -> SetMinimum(Ymin);
    mnvh_mc_v0 -> SetMaximum(Ymax);
    
    
    // Draw
    // ====
    
    const TAxis* axis = mnvh_mc_v0->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(1);
    line.SetLineWidth(5);
    line.SetLineColor(plot_info.m_mnv_plotter.mc_color);
    
    mnvh_mc_v0           -> Draw("HIST L");
    mnvh_mc_v1noPionTune -> DrawCopy("SAME HIST L");
    mnvh_mc_v1noD2       -> DrawCopy("SAME HIST L");
    mnvh_mc_v1noNonResPi -> DrawCopy("SAME HIST L");
    
    line.DrawLine(lowX, 1.0, highX, 1.0);
    mnvh_data            -> DrawCopy("SAME E1 X0");
    mnvh_data_stat       -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_v0);
    h_vector.push_back(mnvh_mc_v1noNonResPi);
    h_vector.push_back(mnvh_mc_v1noD2);
    h_vector.push_back(mnvh_mc_v1noPionTune);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("GENIE 2.12.6");
    name_vector.push_back("4.0.1 w/o non-RES #pi");
    name_vector.push_back("4.0.1 w/o deuterium #pi");
    name_vector.push_back("4.0.1 w/o any #pi tune");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.57, 0.68, 0.35, 0.22, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.21, 0.87, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v1;
    delete mnvh_mc_v0;
    delete mnvh_mc_v1noNonResPi;
    delete mnvh_mc_v1noD2;
    delete mnvh_mc_v1noPionTune;
}



// ==============================================================================================
//  Plot data-MC ratio with multiple models - VERSION 2
// ==============================================================================================

void PlotDataMCRatio_FullXsecModels(PlotInfo plot_info,
                                    PlotUtils::MnvH1D* h_input_data,         // Data histo
                                    PlotUtils::MnvH1D* h_input_mc_v1,        // MC models
                                    PlotUtils::MnvH1D* h_input_mc_v2MINOS,
                                    PlotUtils::MnvH1D* h_input_mc_v2JOINT, 
                                    PlotUtils::MnvH1D* h_input_mc_v2NU1PI,
                                    PlotUtils::MnvH1D* h_input_mc_v2NUNPI,
                                    PlotUtils::MnvH1D* h_input_mc_v2NUPI0,
                                    PlotUtils::MnvH1D* h_input_mc_v2MENU1PI,
                                    std::string output_str,                  // Output location inside top directory
                                    std::string title_str,                   // Histo title at header
                                    std::string xlabel_str  = "",            // X-axis label
                                    std::string ylabel_str  = "",            // Y-axis label
                                    double Ymin             = 0.0,           // Y-axis minimum: default is y=0
                                    double Ymax             = 4.0,           // Y-axis maximum: default is automatic size
                                    bool add_pot_info       = true,          // Add POT info?
                                    bool data_stat_err_only = false)         // Data histo with stat errors only?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    TH1D* h_mc_v1        = (TH1D*)h_input_mc_v1->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2MINOS   = (TH1D*)h_input_mc_v2MINOS->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2JOINT   = (TH1D*)h_input_mc_v2JOINT->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2NU1PI   = (TH1D*)h_input_mc_v2NU1PI->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2NUNPI   = (TH1D*)h_input_mc_v2NUNPI->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2NUPI0   = (TH1D*)h_input_mc_v2NUPI0->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_v2MENU1PI = (TH1D*)h_input_mc_v2MENU1PI->GetCVHistoWithStatError().Clone("");
    
    PlotUtils::MnvH1D* mnvh_mc_v1        = new PlotUtils::MnvH1D(*h_mc_v1);
    PlotUtils::MnvH1D* mnvh_mc_v2MINOS   = new PlotUtils::MnvH1D(*h_mc_v2MINOS);
    PlotUtils::MnvH1D* mnvh_mc_v2JOINT   = new PlotUtils::MnvH1D(*h_mc_v2JOINT);
    PlotUtils::MnvH1D* mnvh_mc_v2NU1PI   = new PlotUtils::MnvH1D(*h_mc_v2NU1PI);
    PlotUtils::MnvH1D* mnvh_mc_v2NUNPI   = new PlotUtils::MnvH1D(*h_mc_v2NUNPI);
    PlotUtils::MnvH1D* mnvh_mc_v2NUPI0   = new PlotUtils::MnvH1D(*h_mc_v2NUPI0);
    PlotUtils::MnvH1D* mnvh_mc_v2MENU1PI = new PlotUtils::MnvH1D(*h_mc_v2MENU1PI);
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(5);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    mnvh_mc_v2MINOS   -> SetLineColor(kCyan+1);
    mnvh_mc_v2JOINT   -> SetLineColor(kRed+2);
    mnvh_mc_v2NU1PI   -> SetLineColor(kBlue);
    mnvh_mc_v2NUNPI   -> SetLineColor(kOrange+1);
    mnvh_mc_v2NUPI0   -> SetLineColor(kGreen+2);
    mnvh_mc_v2MENU1PI -> SetLineColor(kViolet-1);
    
    mnvh_mc_v2MINOS   -> SetLineWidth(4);
    mnvh_mc_v2JOINT   -> SetLineWidth(4);
    mnvh_mc_v2NU1PI   -> SetLineWidth(4);
    mnvh_mc_v2NUNPI   -> SetLineWidth(4);
    mnvh_mc_v2NUPI0   -> SetLineWidth(4);
    mnvh_mc_v2MENU1PI -> SetLineWidth(4);
    
    // Set base model error to zero
    for ( int bin = 1; bin <= mnvh_mc_v1->GetNbinsX(); ++bin ) {
        mnvh_mc_v1 -> SetBinError(bin, 0.0);
    }
    
    // Bin width normalize
    mnvh_data         -> Scale(mnvh_data         -> GetNormBinWidth(), "width");
    mnvh_data_stat    -> Scale(mnvh_data_stat    -> GetNormBinWidth(), "width");
    mnvh_mc_v1        -> Scale(mnvh_mc_v1        -> GetNormBinWidth(), "width");
    mnvh_mc_v2MINOS   -> Scale(mnvh_mc_v2MINOS   -> GetNormBinWidth(), "width");
    mnvh_mc_v2JOINT   -> Scale(mnvh_mc_v2JOINT   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NU1PI   -> Scale(mnvh_mc_v2NU1PI   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NUNPI   -> Scale(mnvh_mc_v2NUNPI   -> GetNormBinWidth(), "width");
    mnvh_mc_v2NUPI0   -> Scale(mnvh_mc_v2NUPI0   -> GetNormBinWidth(), "width");
    mnvh_mc_v2MENU1PI -> Scale(mnvh_mc_v2MENU1PI -> GetNormBinWidth(), "width");
    
    // Get ratios
    mnvh_data         -> Divide(mnvh_data,         mnvh_mc_v1);
    mnvh_data_stat    -> Divide(mnvh_data_stat,    mnvh_mc_v1);
    mnvh_mc_v2MINOS   -> Divide(mnvh_mc_v2MINOS,   mnvh_mc_v1);
    mnvh_mc_v2JOINT   -> Divide(mnvh_mc_v2JOINT,   mnvh_mc_v1);
    mnvh_mc_v2NU1PI   -> Divide(mnvh_mc_v2NU1PI,   mnvh_mc_v1);
    mnvh_mc_v2NUNPI   -> Divide(mnvh_mc_v2NUNPI,   mnvh_mc_v1);
    mnvh_mc_v2NUPI0   -> Divide(mnvh_mc_v2NUPI0,   mnvh_mc_v1);
    mnvh_mc_v2MENU1PI -> Divide(mnvh_mc_v2MENU1PI, mnvh_mc_v1);
    mnvh_mc_v1        -> Divide(mnvh_mc_v1,        mnvh_mc_v1);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_v2MINOS, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_v2MINOS, ylabel_str);
    
    // Y-axis limits
    mnvh_mc_v2MINOS -> SetMinimum(Ymin);
    mnvh_mc_v2MINOS -> SetMaximum(Ymax);
    
    
    // Draw
    // ====
    
    const TAxis* axis = mnvh_mc_v2MINOS->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(1);
    line.SetLineWidth(5);
    line.SetLineColor(plot_info.m_mnv_plotter.mc_color);
    
    mnvh_mc_v2MINOS   -> Draw("HIST L");
    mnvh_mc_v2JOINT   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NU1PI   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NUNPI   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2NUPI0   -> DrawCopy("SAME HIST L");
    mnvh_mc_v2MENU1PI -> DrawCopy("SAME HIST L");
    
    line.DrawLine(lowX, 1.0, highX, 1.0);
    mnvh_data         -> DrawCopy("SAME E1 X0");
    mnvh_data_stat    -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_v2MINOS);
    h_vector.push_back(mnvh_mc_v2JOINT);
    h_vector.push_back(mnvh_mc_v2NU1PI);
    h_vector.push_back(mnvh_mc_v2NUNPI);
    h_vector.push_back(mnvh_mc_v2NUPI0);
    h_vector.push_back(mnvh_mc_v2MENU1PI);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'MINOS'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'JOINT'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NU1PI'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NUNPI'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'NUPI0'");
    name_vector.push_back("4.0.1 w/low-Q^{2} 'MENU1PI'");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.52, 0.61, 0.41, 0.29, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.21, 0.87, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v1;
    delete mnvh_mc_v2MINOS;
    delete mnvh_mc_v2JOINT;
    delete mnvh_mc_v2NU1PI;
    delete mnvh_mc_v2NUNPI;
    delete mnvh_mc_v2NUPI0;
    delete mnvh_mc_v2MENU1PI;
}



// ==============================================================================================
//  Plot data-MC ratio with multiple models - VERSION 3
// ==============================================================================================

void PlotDataMCRatio_FullXsecModels(PlotInfo plot_info,
                                    PlotUtils::MnvH1D* h_input_data,         // Data histo
                                    PlotUtils::MnvH1D* h_input_mc_v1,        // MC models
                                    PlotUtils::MnvH1D* h_input_mc_GENIE3_02a,
                                    PlotUtils::MnvH1D* h_input_mc_GENIE3_02b, 
                                    PlotUtils::MnvH1D* h_input_mc_GENIE3_10a,
                                    PlotUtils::MnvH1D* h_input_mc_GENIE3_10b,
                                    PlotUtils::MnvH1D* h_input_mc_NEUT_LFG,
                                    std::string output_str,                  // Output location inside top directory
                                    std::string title_str,                   // Histo title at header
                                    std::string xlabel_str  = "",            // X-axis label
                                    std::string ylabel_str  = "",            // Y-axis label
                                    double Ymin             = 0.0,           // Y-axis minimum: default is y=0
                                    double Ymax             = 4.0,           // Y-axis maximum: default is automatic size
                                    bool add_pot_info       = true,          // Add POT info?
                                    bool data_stat_err_only = false)         // Data histo with stat errors only?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    TH1D* h_data = (TH1D*)h_input_data->GetCVHistoWithError(true, plot_info.m_do_cov_area_norm).Clone("");
    PlotUtils::MnvH1D* mnvh_data = new PlotUtils::MnvH1D(*h_data);
    
    TH1D* h_data_stat = (TH1D*)h_input_data->GetCVHistoWithStatError().Clone("");
    PlotUtils::MnvH1D* mnvh_data_stat = new PlotUtils::MnvH1D(*h_data_stat);
    
    TH1D* h_mc_v1         = (TH1D*)h_input_mc_v1->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_GENIE3_02a = (TH1D*)h_input_mc_GENIE3_02a->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_GENIE3_02b = (TH1D*)h_input_mc_GENIE3_02b->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_GENIE3_10a = (TH1D*)h_input_mc_GENIE3_10a->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_GENIE3_10b = (TH1D*)h_input_mc_GENIE3_10b->GetCVHistoWithStatError().Clone("");
    TH1D* h_mc_NEUT_LFG   = (TH1D*)h_input_mc_NEUT_LFG->GetCVHistoWithStatError().Clone("");
    
    PlotUtils::MnvH1D* mnvh_mc_v1         = new PlotUtils::MnvH1D(*h_mc_v1);
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_02a = new PlotUtils::MnvH1D(*h_mc_GENIE3_02a);
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_02b = new PlotUtils::MnvH1D(*h_mc_GENIE3_02b);
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_10a = new PlotUtils::MnvH1D(*h_mc_GENIE3_10a);
    PlotUtils::MnvH1D* mnvh_mc_GENIE3_10b = new PlotUtils::MnvH1D(*h_mc_GENIE3_10b);
    PlotUtils::MnvH1D* mnvh_mc_NEUT_LFG   = new PlotUtils::MnvH1D(*h_mc_NEUT_LFG);
    
    
    // Histogram style
    // ===============
    
    // Data
    mnvh_data -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    mnvh_data_stat -> SetMarkerStyle(plot_info.m_mnv_plotter.ratio_marker);
    mnvh_data_stat -> SetMarkerSize(plot_info.m_mnv_plotter.ratio_marker_size);
    mnvh_data_stat -> SetMarkerColor(plot_info.m_mnv_plotter.ratio_color);
    mnvh_data_stat -> SetLineWidth(plot_info.m_mnv_plotter.ratio_line_width);
    mnvh_data_stat -> SetLineColor(plot_info.m_mnv_plotter.ratio_color);
    
    // Base model
    mnvh_mc_v1 -> SetLineColor(plot_info.m_mnv_plotter.mc_color);
    mnvh_mc_v1 -> SetLineWidth(5);
    mnvh_mc_v1 -> SetLineStyle(plot_info.m_mnv_plotter.mc_line_style);
    
    mnvh_mc_GENIE3_02a -> SetLineColor(kCyan+1);
    mnvh_mc_GENIE3_02b -> SetLineColor(kRed+2);
    mnvh_mc_GENIE3_10a -> SetLineColor(kBlue);
    mnvh_mc_GENIE3_10b -> SetLineColor(kOrange+1);
    mnvh_mc_NEUT_LFG   -> SetLineColor(kGreen+2);
    
    mnvh_mc_GENIE3_02a -> SetLineWidth(4);
    mnvh_mc_GENIE3_02b -> SetLineWidth(4);
    mnvh_mc_GENIE3_10a -> SetLineWidth(4);
    mnvh_mc_GENIE3_10b -> SetLineWidth(4);
    mnvh_mc_NEUT_LFG   -> SetLineWidth(4);
    
    // Set base model error to zero
    for ( int bin = 1; bin <= mnvh_mc_v1->GetNbinsX(); ++bin ) {
        mnvh_mc_v1 -> SetBinError(bin, 0.0);
    }
    
    // Bin width normalize
    mnvh_data          -> Scale(mnvh_data          -> GetNormBinWidth(), "width");
    mnvh_data_stat     -> Scale(mnvh_data_stat     -> GetNormBinWidth(), "width");
    mnvh_mc_v1         -> Scale(mnvh_mc_v1         -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_02a -> Scale(mnvh_mc_GENIE3_02a -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_02b -> Scale(mnvh_mc_GENIE3_02b -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_10a -> Scale(mnvh_mc_GENIE3_10a -> GetNormBinWidth(), "width");
    mnvh_mc_GENIE3_10b -> Scale(mnvh_mc_GENIE3_10b -> GetNormBinWidth(), "width");
    mnvh_mc_NEUT_LFG   -> Scale(mnvh_mc_NEUT_LFG   -> GetNormBinWidth(), "width");
    
    // Get ratios
    mnvh_data          -> Divide(mnvh_data,          mnvh_mc_v1);
    mnvh_data_stat     -> Divide(mnvh_data_stat,     mnvh_mc_v1);
    mnvh_mc_GENIE3_02a -> Divide(mnvh_mc_GENIE3_02a, mnvh_mc_v1);
    mnvh_mc_GENIE3_02b -> Divide(mnvh_mc_GENIE3_02b, mnvh_mc_v1);
    mnvh_mc_GENIE3_10a -> Divide(mnvh_mc_GENIE3_10a, mnvh_mc_v1);
    mnvh_mc_GENIE3_10b -> Divide(mnvh_mc_GENIE3_10b, mnvh_mc_v1);
    mnvh_mc_NEUT_LFG   -> Divide(mnvh_mc_NEUT_LFG,   mnvh_mc_v1);
    mnvh_mc_v1         -> Divide(mnvh_mc_v1,         mnvh_mc_v1);
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    plot_info.SetXLabel(mnvh_mc_NEUT_LFG, xlabel_str);
    plot_info.SetYLabel(mnvh_mc_NEUT_LFG, ylabel_str);
    
    // Y-axis limits
    mnvh_mc_NEUT_LFG -> SetMinimum(Ymin);
    mnvh_mc_NEUT_LFG -> SetMaximum(Ymax);
    
    
    // Draw
    // ====
    
    const TAxis* axis = mnvh_mc_NEUT_LFG->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(1);
    line.SetLineWidth(5);
    line.SetLineColor(plot_info.m_mnv_plotter.mc_color);
    
    mnvh_mc_NEUT_LFG   -> Draw("HIST L");
    mnvh_mc_GENIE3_10b -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_10a -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_02b -> DrawCopy("SAME HIST L");
    mnvh_mc_GENIE3_02a -> DrawCopy("SAME HIST L");
    
    line.DrawLine(lowX, 1.0, highX, 1.0);
    mnvh_data          -> DrawCopy("SAME E1 X0");
    mnvh_data_stat     -> DrawCopy("SAME E1 X0");
    
    
    // Add legend
    // ==========
    
    std::vector<TH1*> h_vector;
    h_vector.push_back(mnvh_data);
    h_vector.push_back(mnvh_mc_GENIE3_02a);
    h_vector.push_back(mnvh_mc_GENIE3_02b);
    h_vector.push_back(mnvh_mc_GENIE3_10a);
    h_vector.push_back(mnvh_mc_GENIE3_10b);
    h_vector.push_back(mnvh_mc_NEUT_LFG);
    
    std::vector<std::string> name_vector;
    name_vector.push_back("Data");
    name_vector.push_back("GENIE 3.0.6 RFG #font[12]{hA}");
    name_vector.push_back("GENIE 3.0.6 RFG #font[12]{hN}");
    name_vector.push_back("GENIE 3.0.6 LFG #font[12]{hA}");
    name_vector.push_back("GENIE 3.0.6 LFG #font[12]{hN}");
    name_vector.push_back("NEUT 5.0.2 LFG");
    
    std::vector<std::string> opts_vector = {"lep", "l", "l", "l", "l", "l"};
    plot_info.m_mnv_plotter.AddPlotLegend(h_vector, name_vector, opts_vector,
                                          0.55, 0.66, 0.37, 0.24, 0.035);
    
    
    // Add text boxes
    // ==============
    
    // POT info
    if ( add_pot_info ) {
        plot_info.m_mnv_plotter.WriteNorm(Form("Data POT: %.2E", plot_info.m_data_pot), 0.3, 0.91-0.03, 0.03);
    }
    
    // Data error labels
    if ( data_stat_err_only ) {
        char* words = Form("Data: Stat. errors only");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.21, 0.87, 0.03, 1, 62);
    }
    else {
        char* words = Form("Data: Stat. & syst. errors");
        plot_info.m_mnv_plotter.AddPlotLabel(words, 0.30, 0.83, 0.03, 1, 62);
    }
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return original settings
    plot_info.m_mnv_plotter.hist_min_zero = true;
    plot_info.m_mnv_plotter.axis_minimum  = -1111;
    plot_info.m_mnv_plotter.axis_maximum  = -1111;
    
    // Release
    delete mnvh_data;
    delete mnvh_data_stat;
    delete mnvh_mc_v1;
    delete mnvh_mc_GENIE3_02a;
    delete mnvh_mc_GENIE3_02b;
    delete mnvh_mc_GENIE3_10a;
    delete mnvh_mc_GENIE3_10b;
    delete mnvh_mc_NEUT_LFG;
}





// ================================================================================================================================================================
//  PLOT ERROR SUMMARY
// ================================================================================================================================================================
 
/**************************************************************************************************************************/
/* 
 * GROUPS OF ERROR BANDS:
 * =====================
 * 
 * Neutrino flux
 * -------------
 *      Flux : Self-explanatory
 * 
 *
 * Cross-section: EL + CCQE + RPA + 2p2h
 * -------------------------------------
 * GENIE Elastic scattering models:
 *      GENIE_MaNCEL  : M_A in elastic scattering cross-section           : +/- 25%
 *      GENIE_EtaNCEL : Eta parameter in elastic scattering cross-section : +/- 30%
 *
 * GENIE CCQE models:
 *      GENIE_MaCCQE            : M_A in Llewellyn-Smith cross-section                             : +25% / -15%
 *      GENIE_CCQEPauliSupViaKF : Pauli blocking (CCQE) at low-Q2                                  : +/- 30%
 *      GENIE_VecFFCCQEshape    : Vector form factor model from BBBA to dipole, affects shape-only : ON / OFF
 *
 * MnvTune 2p2h and RPA cross-section models:
 *      Low_Recoil_2p2h_Tune : Low-recoil 2p2h tune : nn/pp pair-only / np pair-only / QE 1p1h variation
 *      RPA_HighQ2           : RPA high-Q2 tune     : Q2 positive / Q2 negative
 *      RPA_LowQ2            : RPA low-Q2 tune      : Q2 positive / Q2 negative
 *
 *
 * Cross-section: Resonant pion production
 * ---------------------------------------
 * GENIE resonant pion production models:
 *      GENIE_MaRES           : M_A in Rein-Sehgal cross-section, affects shape and normalization : +/- 20%
 *      GENIE_MvRES           : M_V in Rein-Sehgal cross-section, affects shape and normalization : +/- 10%
 *      GENIE_NormNCRES       : Normalization of NC Rein-Sehgal cross-section                     : +/- 20%
 *      GENIE_RDecBR1gamma    : Branching ratio of resonance decay to photon                      : +/- 50%
 *      GENIE_Theta_Delta2Npi : Delta decay angular distribution                                  : ON / OFF
 *
 * MnvTune pion production cross-section models:
 *      GENIE_D2_MaRES     : M_A from Rein-Sehgal refitted from deuterium data                          : +/- 5 GeV
 *      GENIE_D2_NormCCRES : Normalization of CC Rein-Sehgal cross-section refitted from deuterium data : +/- 7%
 *      GENIE_EP_MvRES     : Error in M_V from Rein-Sehgal reduced from electroproduction data          : +/- 3%
 *      LowQ2Pi            : Pion production low-Q2 tune                                                : 
 *
 *
 * Cross-section: Non-resonant pion production
 * -------------------------------------------
 * GENIE non-resonant pion production models:
 *      GENIE_Rvp1pi : NC and CC non-resonant single-pion production with nu-p/antinu-n initial states : +/- 4%
 *      GENIE_Rvn1pi : NC and CC non-resonant single-pion production with nu-n/antinu-p initial states : +/- 4%
 *      GENIE_Rvn2pi : NC and CC non-resonant two-pion production with nu-p/antinu-n initial states    : +/- 50%   
 *      GENIE_Rvp2pi : NC and CC non-resonant two-pion production with nu-n/antinu-p initial states    : +/- 50%
 *
 *
 * Cross-section: DIS/hadronization
 * --------------------------------
 * GENIE DIS and hadronization models:
 *      GENIE_AhtBY     : Bodek-Yang parameter A_{ht} in shape and normalization          : +/- 25%
 *      GENIE_BhtBY     : Bodek-Yang parameter B_{ht} in shape and normalization          : +/- 25%
 *      GENIE_CV1uBY    : Bodek-Yang parameter C_{V1u} in shape an normalization          : +/- 30%
 *      GENIE_CV2uBY    : Bodek-Yang parameter C_{V2u} in shape an normalization          : +/- 40%
 *      GENIE_AGKYxF1pi : xF distribution for low (N + pi) multiplicity DIS in AGKY model : +/- 20%
 *      GENIE_NormDISCC : Overall normalization of non-resonant inclusive cross section   : +/- 0%
 *
 *
 * GENIE nucleon FSI models
 * ------------------------
 *      GENIE_MFP_N      : Mean free path                    : +/- 20%
 *      GENIE_FrElas_N   : Elastic interaction probability   : +/- 30%
 *      GENIE_FrInel_N   : Inelastic interaction probability : +/- 40%
 *      GENIE_FrAbs_N    : Absorption probability            : +/- 20%
 *      GENIE_FrCEx_N    : Charge exchange probability       : +/- 50%
 *      GENIE_FrPiProd_N : Pion production probability       : +/- 20%
 *
 *
 * GENIE pion FSI models
 * ---------------------
 *      GENIE_MFP_pi      : Mean free path                  : +/- 20%
 *      GENIE_FrElas_pi   : Elastic interaction probability : +/- 10%
 *      GENIE_FrAbs_pi    : Absorption probability          : +/- 30%
 *      GENIE_FrCEx_pi    : Charge exchange probability     : +/- 50%
 * Thre     GENIE_FrPiProd_pi : Pion production probability     : +/- 20%
 * 
 * 
 * Muon reconstruction:
 * -------------------
 *      Muon_Energy_MINERvA             : Muon energy in MINERvA           -> Depends on muon reconstruction
 *      Muon_Energy_MINOS               : Muon energy in MINOS             -> Depends on muon reconstruction
 *      Muon_Energy_Resolution          : Muon energy resolution           : +/- 0.4%
 *      MuonAngleXResolution            : Muon track angle resolution in X : +/- 2%
 *      MuonAngleYResolution            : Muon track angle resolution in Y : +/- 2%
 *      BeamAngleX                      : Beam angle uncertainty in X      : +/- 0.001 rad
 *      BeamAngleY                      : Beam angle uncertainty in Y      : +/- 0.0009 rad
 *      MINOS_Reconstruction_Efficiency : MINOS muon efficiency            -> Depends on MINOS reco efficiency
 *
 *
 * Calorimetric energy particle response:
 * -------------------------------------
 *      response_low_proton   : Proton low-KE (KE < 50 MeV) response        : +/- 4%
 *      response_mid_proton   : Proton mid-KE (50 < KE < 100 MeV) response  : +/- 3.5%
 *      response_high_proton  : Proton high-KE (KE > 100 MeV ) response     : +/- 3%
 *      response_low_neutron  : Neutron low-KE (KE < 50 MeV) response       : +/- 25%
 *      response_mid_neutron  : Neutron mid-KE (50 < KE < 150 MeV) response : +/- 10%
 *      response_high_neutron : Neutron high-KE (KE > 150 MeV) response     : +/- 20%
 *      response_meson        : Meson response                              : +/- 5%
 *      response_em           : Pi0 and photon response                     : +/- 3%
 *      response_other        : Other PDG response                          : +/- 20%
 *
 *
 * Other:
 * -----
 *      GEANT_Proton     : GEANT proton cross-section model  -> Depends on GEANT reweighting
 *      GEANT_Neutron    : GEANT neutron cross-section model -> Depends on GEANT reweighting
 *      GEANT_Pion       : GEANT pion cross-section model    -> Depends on GEANT reweighting
 *      Target_Mass_CH   : Hydrocarbon target mass           : +/- 1.4%
 *      Target_Mass_C    : Carbon target mass                : +/- 0.5%
 *      Target_Mass_H2O  : Water target mass                 : +/- 2%
 *      Target_Mass_Fe   : Iron target mass                  : +/- 1%
 *      Target_Mass_Pb   : Lead target mass                  : +/- 0.5%
 *      MichelEfficiency : Michel candidate tag efficiency   : +/- 2.5%
 */
/**************************************************************************************************************************/

// ==============================================================================================
//  Plot error group
// ==============================================================================================

void PlotErrorGroup(PlotInfo plot_info,             
                    PlotUtils::MnvH1D* h_input,      // Input histo with error bands
                    std::string output_str,          // Output location inside top directory
                    std::string title_str,           // Histo title at header
                    std::string error_group_name,    // Error group name
                    double Ymax,                     // Y-axis maximum
                    int Ncolumns,                    // Number of columns on legend
                    bool use_frac_uncertainty,       // Use fractional uncertainties?
                    std::string xlabel_str = "",     // X-axis label
                    std::string ylabel_str = "",     // Y-axis label
                    std::string legend_pos = "TR",   // Legend position
                    bool write_preliminary = false)  // Write 'MINERvA preliminary'?
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Set colors
    plot_info.m_mnv_plotter.good_colors = MnvColors::GetColors(MnvColors::k36Palette);
    
    // Get histogram
    PlotUtils::MnvH1D* mnvh_input = (PlotUtils::MnvH1D*)h_input->Clone("mnvh_mc");
    
    // Axis label in X
    if ( xlabel_str == "" )
        plot_info.SetXLabel(mnvh_input);
    else
        plot_info.SetXLabel(mnvh_input, xlabel_str);
    
    // Axis title in Y (doesn't work)
    if ( ylabel_str != "" )
        plot_info.SetYLabel(mnvh_input, ylabel_str);
    
    // Y-axis limit
    plot_info.m_mnv_plotter.axis_maximum = Ymax;
    
    
    // Draw
    // ====
    
    // Set MC line width a bit wider
    plot_info.m_mnv_plotter.mc_line_width = 4;
    
    // Set legend columns
    plot_info.m_mnv_plotter.legend_n_columns = Ncolumns;
    
    // TBH, don't know well what does this do, but it works
    double ignore_threshold = 0.0;
    
    plot_info.m_mnv_plotter.DrawErrorSummary(mnvh_input,                      // Histogram with error bands
                                             legend_pos,                      // Legend position
                                             plot_info.m_include_stat_error,  // Include stat errors
                                             true,                            // Solid lines only
                                             ignore_threshold,                // Ignore threshold
                                             plot_info.m_do_cov_area_norm,    // Covariance on shape-only or full covariance?
                                             error_group_name,                // Error group name
                                             use_frac_uncertainty);           // Do fractional uncertainty?
    
    
    // Other details
    // =============
    
    // Write "MINERvA preliminary"
    if ( write_preliminary )
        plot_info.m_mnv_plotter.WritePreliminary(0.78, 0.90, 0.03);
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Return MC line width to normal
    plot_info.m_mnv_plotter.mc_line_width = 3;
    
    // Return legend columns to normal
    plot_info.m_mnv_plotter.legend_n_columns = 1;
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
}



// ==============================================================================================
//  Plot error summary
// ==============================================================================================
// (All systematics grouped into error groups)

void PlotErrorSummary(PlotInfo plot_info,
                      PlotUtils::MnvH1D* h_input,     // Input histogram with error bands
                      std::string output_str,         // Output location inside top directory
                      std::string title_str,          // Histo title at header
                      double Ymax,                    // Y-axis maximum
                      bool use_frac_uncertainty,      // Use fractional uncertainties?
                      std::string xlabel_str = "",    // X-axis label
                      std::string ylabel_str = "",    // Y-axis label
                      std::string legend_pos = "TR")  // Legend position
{
    // Define number of columns
    int Ncolumns = 2;
    
    // Plot all systematics grouped
    PlotErrorGroup(plot_info, h_input, output_str, title_str, "", Ymax, Ncolumns, use_frac_uncertainty,
                   xlabel_str, ylabel_str, legend_pos);
}



// ==============================================================================================
//  Plot error groups
// ==============================================================================================
// (Group of systematics individually)

void PlotErrorGroups(PlotInfo plot_info,
                     PlotUtils::MnvH1D* h_input,                  // Input histogram with error bands
                     std::string output_str,                      // Output location inside top directory
                     std::string material_option,                 // Material option
                     std::vector<std::string> error_group_names,  // Vector of names of error groups
                     std::vector<double> error_group_Ymax,        // Vector of Y-axis limits for each error group
                     bool use_frac_uncertainty,                   // Use fractional uncertainties?
                     std::string xlabel_str = "",                 // X-axis label
                     std::string ylabel_str = "",                 // Y-axis label
                     std::string legend_pos = "TR")               // Legend position
{
    // Material title
    std::string material_title;
    if ( material_option == "lead" )      material_title = " - [Lead]";
    else if ( material_option == "iron" ) material_title = " - [Iron]";
    
    // Fractional/absolute error string
    std::string output_sub_str = use_frac_uncertainty ? "Frac" : "Abs";
    
    // Loop over all error groups
    for ( unsigned int i = 0; i < error_group_names.size(); ++i )
    {
        // Flux
        if ( error_group_names[i] == "Neutrino Flux" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_Flux_" + material_option,
                           "Flux errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // EL + CCQE + 2p2h + RPA cross-section models
        if ( error_group_names[i] == "X-Sec: EL/CCQE/2p2h/RPA" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_XsecCCQE2p2h_" + material_option,
                           "EL/CCQE/2p2h/RPA x-sec. model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Resonant pion production cross-section models
        if ( error_group_names[i] == "X-Sec: RES pion" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_XsecResPi_" + material_option,
                           "RES #pi x-sec. model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 2,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Non-resonant pion production cross-section models
        if ( error_group_names[i] == "X-Sec: Non-RES pion" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_XsecNonResPi_" + material_option,
                           "Non-RES #pi x-sec. model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // DIS/hadronization cross-section models
        if ( error_group_names[i] == "X-Sec: DIS" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_XsecDIS_" + material_option,
                           "DIS x-sec. model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Nucleon FSI
        if ( error_group_names[i] == "FSI: Nucleons" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_FSINucl_" + material_option,
                           "Nucleon FSI model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 2,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Pion FSI
        if ( error_group_names[i] == "FSI: Pions" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_FSIPion_" + material_option,
                           "Pion FSI model errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Muon reconstruction
        if ( error_group_names[i] == "Muon Reconstruction" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_Muon_" + material_option,
                           "Muon reconstruction errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
        
        // Particle response
        if ( error_group_names[i] == "Particle Response" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_PartResp_" + material_option,
                           "Calorimetric particle response errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 1,
                           use_frac_uncertainty, xlabel_str, ylabel_str, "TL");
        }
        
        // Other
        if ( error_group_names[i] == "Other" ) {
            PlotErrorGroup(plot_info, h_input,
                           output_str + output_sub_str + "Errors_Other_" + material_option,
                           "Other systematic errors" + material_title,
                           error_group_names[i], error_group_Ymax[i], 2,
                           use_frac_uncertainty, xlabel_str, ylabel_str, legend_pos);
        }
    }
}


#endif  // plotting_functions_finalversion_h