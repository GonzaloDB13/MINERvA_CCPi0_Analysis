#ifndef plotting_functions_h
#define plotting_functions_h

#ifndef __CINT__
#include <iostream>
#include <vector>

#include "CVUniverse.h"
#include "MacroUtil.h"
#include "CCPi0Event.h"
#include "Variable.h"
#include "Variable2D.h"
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
        
        // 1D variable
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
        m_variable2D(nullptr),
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
        
        
        // 2D variable
        PlotInfo(Variable2D* variable2D,
                 double mc_pot, double data_pot,
                 bool do_frac_uncertainty,
                 bool do_cov_area_norm,
                 bool include_stat_error,
                 bool do_bin_width_norm,
                 std::string print_format)
        :
        m_mnv_plotter(kCCPi0AnaStyle),
        m_variable(nullptr),
        m_variable2D(variable2D),
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
        m_variable2D(nullptr),
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
        Variable2D* m_variable2D;
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
//  PLOT 1D HISTOGRAM WITH STAT ERRORS
// ================================================================================================================================================================

void Plot1D(PlotInfo plot_info,
            PlotUtils::MnvH1D* mnvh,
            std::string output_str,
            std::string title_str,
            std::string xlabel_str = "",
            std::string ylabel_str = "")
{
    // Define canvas
    TCanvas canvas("c1","c1");
    
    // Get histogram
    PlotUtils::MnvH1D* mnvh_clone = (PlotUtils::MnvH1D*)mnvh->Clone("mnvh_mc_clone");
    TH1* h = (TH1*)mnvh_clone->GetCVHistoWithStatError().Clone("h_mc");
    
    // Axis labels
    if ( xlabel_str == "" ) plot_info.SetXLabel(h);
    else                    plot_info.SetXLabel(h, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(h, mnvh->GetNormBinWidth());
    else                    plot_info.SetYLabel(h, ylabel_str);
    
    
    // Draw
    // ====
    gStyle -> SetEndErrorSize(4);
    h -> SetMarkerStyle(20);
    h -> SetMarkerColor(kBlue+1);
    h -> SetMarkerSize(2);
    h -> SetLineWidth(3);
    h -> SetLineColor(kBlue+1);
    h -> Draw("E1 X0");
    
    
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
    delete mnvh_clone;
    delete h;
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
                       std::vector<double> arrow_X,          // X positions of cut arrows
                       std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                       std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                       std::vector<std::string> arrow_dir,   // Directions of cut arrows
                       std::string xlabel_str = "",          // X-axis label
                       std::string ylabel_str = "",          // Y-axis label
                       std::string legend_pos = "TR")        // Legend position
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
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
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
//  PLOT DATA AND STACKED MC OF PDG BREAKDOWN
// ================================================================================================================================================================

void PlotDataStackedMC(PlotInfo plot_info,
                       PlotUtils::MnvH1D* h_input_data,       // Data histo
                       PlotUtils::MnvH1D* h_input_mc,         // MC histo
                       PlotUtils::MnvH1D* h_input_mc_Pi0,     // MC histos per PDG
                       PlotUtils::MnvH1D* h_input_mc_Proton, 
                       PlotUtils::MnvH1D* h_input_mc_Neutron,
                       PlotUtils::MnvH1D* h_input_mc_Pion,
                       PlotUtils::MnvH1D* h_input_mc_EM,
                       PlotUtils::MnvH1D* h_input_mc_Muon,
                       PlotUtils::MnvH1D* h_input_mc_OthPdg,
                       PlotUtils::MnvH1D* h_input_mc_MCXtalk,
                       PlotUtils::MnvH1D* h_input_mc_Overlay,
                       std::string output_str,               // Output location inside top directory
                       std::string title_str,                // Histo title at header
                       std::vector<double> arrow_X,          // X positions of cut arrows
                       std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                       std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                       std::vector<std::string> arrow_dir,   // Directions of cut arrows
                       std::string xlabel_str = "",          // X-axis label
                       std::string ylabel_str = "",          // Y-axis label
                       std::string legend_pos = "TR")        // Legend position
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_data       = (PlotUtils::MnvH1D*)h_input_data       -> Clone("mnvh_data");
    PlotUtils::MnvH1D* mnvh_mc         = (PlotUtils::MnvH1D*)h_input_mc         -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_Pi0     = (PlotUtils::MnvH1D*)h_input_mc_Pi0     -> Clone("mnvh_mc_Pi0");
    PlotUtils::MnvH1D* mnvh_mc_Proton  = (PlotUtils::MnvH1D*)h_input_mc_Proton  -> Clone("mnvh_mc_Proton");
    PlotUtils::MnvH1D* mnvh_mc_Neutron = (PlotUtils::MnvH1D*)h_input_mc_Neutron -> Clone("mnvh_mc_Neutron");
    PlotUtils::MnvH1D* mnvh_mc_Pion    = (PlotUtils::MnvH1D*)h_input_mc_Pion    -> Clone("mnvh_mc_Pion");
    PlotUtils::MnvH1D* mnvh_mc_EM      = (PlotUtils::MnvH1D*)h_input_mc_EM      -> Clone("mnvh_mc_EM");
    PlotUtils::MnvH1D* mnvh_mc_Muon    = (PlotUtils::MnvH1D*)h_input_mc_Muon    -> Clone("mnvh_mc_Muon");
    PlotUtils::MnvH1D* mnvh_mc_OthPdg  = (PlotUtils::MnvH1D*)h_input_mc_OthPdg  -> Clone("mnvh_mc_OthPdg");
    PlotUtils::MnvH1D* mnvh_mc_MCXtalk = (PlotUtils::MnvH1D*)h_input_mc_MCXtalk -> Clone("mnvh_mc_MCXtalk");
    PlotUtils::MnvH1D* mnvh_mc_Overlay = (PlotUtils::MnvH1D*)h_input_mc_Overlay -> Clone("mnvh_mc_Overlay");
    
    PlotUtils::MnvH1D* mnvh_mc_Pi0EM = (PlotUtils::MnvH1D*)mnvh_mc_Pi0 -> Clone("mnvh_mc_Pi0EM");
    mnvh_mc_Pi0EM -> Add(mnvh_mc_EM);
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc         = mnvh_mc         -> Integral(0, mnvh_mc         -> GetNbinsX()+1);
    double area_mc_Pi0EM   = mnvh_mc_Pi0EM   -> Integral(0, mnvh_mc_Pi0EM   -> GetNbinsX()+1);
    double area_mc_Proton  = mnvh_mc_Proton  -> Integral(0, mnvh_mc_Proton  -> GetNbinsX()+1);
    double area_mc_Neutron = mnvh_mc_Neutron -> Integral(0, mnvh_mc_Neutron -> GetNbinsX()+1);
    double area_mc_Pion    = mnvh_mc_Pion    -> Integral(0, mnvh_mc_Pion    -> GetNbinsX()+1);
    double area_mc_Muon    = mnvh_mc_Muon    -> Integral(0, mnvh_mc_Muon    -> GetNbinsX()+1);
    double area_mc_OthPdg  = mnvh_mc_OthPdg  -> Integral(0, mnvh_mc_OthPdg  -> GetNbinsX()+1);
    double area_mc_MCXtalk = mnvh_mc_MCXtalk -> Integral(0, mnvh_mc_MCXtalk -> GetNbinsX()+1);
    double area_mc_Overlay = mnvh_mc_Overlay -> Integral(0, mnvh_mc_Overlay -> GetNbinsX()+1);
    
    legend_label = Form("#pi^{0} + EM (%.1f%%)", 100.0*(area_mc_Pi0EM/area_mc));
    mnvh_mc_Pi0EM -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Proton (%.1f%%)", 100.0*(area_mc_Proton/area_mc));
    mnvh_mc_Proton -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Neutron (%.1f%%)", 100.0*(area_mc_Neutron/area_mc));
    mnvh_mc_Neutron -> SetTitle(legend_label.c_str());
    
    legend_label = Form("#pi^{#pm} (%.1f%%)", 100.0*(area_mc_Pion/area_mc));
    mnvh_mc_Pion -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Muon (%.1f%%)", 100.0*(area_mc_Muon/area_mc));
    mnvh_mc_Muon -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Other PDG (%.1f%%)", 100.0*(area_mc_OthPdg/area_mc));
    mnvh_mc_OthPdg -> SetTitle(legend_label.c_str());
    
    legend_label = Form("MC X-talk (%.1f%%)", 100.0*(area_mc_MCXtalk/area_mc));
    mnvh_mc_MCXtalk -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Overlay (%.1f%%)", 100.0*(area_mc_Overlay/area_mc));
    mnvh_mc_Overlay -> SetTitle(legend_label.c_str());
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_Pi0EM,   0, 3);
    SetHistColorScheme(mnvh_mc_Proton,  1, 3);
    SetHistColorScheme(mnvh_mc_Neutron, 2, 3);
    SetHistColorScheme(mnvh_mc_Pion,    3, 3);
    SetHistColorScheme(mnvh_mc_Muon,    4, 3);
    SetHistColorScheme(mnvh_mc_OthPdg,  5, 3);
    SetHistColorScheme(mnvh_mc_MCXtalk, 6, 3);
    SetHistColorScheme(mnvh_mc_Overlay, 7, 3);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Overlay);
    h_mc_array -> Add(mnvh_mc_MCXtalk);
    h_mc_array -> Add(mnvh_mc_OthPdg);
    h_mc_array -> Add(mnvh_mc_Muon);
    h_mc_array -> Add(mnvh_mc_Pion);
    h_mc_array -> Add(mnvh_mc_Neutron);
    h_mc_array -> Add(mnvh_mc_Proton);
    h_mc_array -> Add(mnvh_mc_Pi0EM);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_data);
    else                    plot_info.SetXLabel(mnvh_data, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_data, mnvh_data->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_data, ylabel_str);
    
    
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
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
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
    
    // Release
    delete mnvh_data;
    delete mnvh_mc;
    delete mnvh_mc_Pi0;
    delete mnvh_mc_Proton;
    delete mnvh_mc_Neutron;
    delete mnvh_mc_Pion;
    delete mnvh_mc_EM;
    delete mnvh_mc_Muon;
    delete mnvh_mc_OthPdg;
    delete mnvh_mc_MCXtalk;
    delete mnvh_mc_Overlay;
    delete mnvh_mc_Pi0EM;
    delete h_mc_array;
}





// ================================================================================================================================================================
//  PLOT STACKED MC OF MATERIAL BREAKDOWN
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
                       std::string xlabel_str = "",                 // X-axis label
                       std::string ylabel_str = "",                 // Y-axis label
                       std::string legend_pos = "TR")               // Legend position
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
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt4Pb);
    mnvh_mc_TrueTgt4Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Pb);
    mnvh_mc_TrueTgt5Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Fe);
    mnvh_mc_TrueTgt5Fe -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasUp);
    mnvh_mc_TruePlasUp -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasBetw);
    mnvh_mc_TruePlasBetw -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasDown);
    mnvh_mc_TruePlasDown -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueOtherMat);
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
//  PLOT STACKED MC OF SIGNAL AND BACKGROUND BREAKDOWN
// ================================================================================================================================================================

void PlotStackedMC(PlotInfo plot_info,
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
                   std::vector<double> arrow_X,          // X positions of cut arrows
                   std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                   std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                   std::vector<std::string> arrow_dir,   // Directions of cut arrows
                   std::string xlabel_str = "",          // X-axis label
                   std::string ylabel_str = "",          // Y-axis label
                   std::string legend_pos = "TR")        // Legend position
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
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
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc);
    else                    plot_info.SetXLabel(mnvh_mc, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc, mnvh_mc->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc, ylabel_str);
    
    
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
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
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
    
    // Release
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
//  PLOT STACKED MC OF MATERIAL BREAKDOWN
// ================================================================================================================================================================

void PlotStackedMC(PlotInfo plot_info,
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
                   std::string xlabel_str = "",                 // X-axis label
                   std::string ylabel_str = "",                 // Y-axis label
                   std::string legend_pos = "N",                // Legend position
                   double Ymax            = 1.1)                // Y-axis maximum
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
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
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt4Pb);
    mnvh_mc_TrueTgt4Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Pb);
    mnvh_mc_TrueTgt5Pb -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueTgt5Fe);
    mnvh_mc_TrueTgt5Fe -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasUp);
    mnvh_mc_TruePlasUp -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasBetw);
    mnvh_mc_TruePlasBetw -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTruePlasDown);
    mnvh_mc_TruePlasDown -> SetTitle(legend_label.c_str());
    
    legend_label = GetTruthClassification_LegendLabel(kTrueOtherMat);
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
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc);
    else                    plot_info.SetXLabel(mnvh_mc, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc, mnvh_mc->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc, ylabel_str);
    
    
    // Set Y-axis limits
    // =================
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
//  PLOT STACKED MC OF PDG BREAKDOWN
// ================================================================================================================================================================

void PlotStackedMC(PlotInfo plot_info,
                   PlotUtils::MnvH1D* h_input_mc,         // MC histo
                   PlotUtils::MnvH1D* h_input_mc_Pi0,     // MC histos per PDG
                   PlotUtils::MnvH1D* h_input_mc_Proton, 
                   PlotUtils::MnvH1D* h_input_mc_Neutron,
                   PlotUtils::MnvH1D* h_input_mc_Pion,
                   PlotUtils::MnvH1D* h_input_mc_EM,
                   PlotUtils::MnvH1D* h_input_mc_Muon,
                   PlotUtils::MnvH1D* h_input_mc_OthPdg,
                   PlotUtils::MnvH1D* h_input_mc_MCXtalk,
                   PlotUtils::MnvH1D* h_input_mc_Overlay,
                   std::string output_str,               // Output location inside top directory
                   std::string title_str,                // Histo title at header
                   std::vector<double> arrow_X,          // X positions of cut arrows
                   std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                   std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                   std::vector<std::string> arrow_dir,   // Directions of cut arrows
                   std::string xlabel_str = "",          // X-axis label
                   std::string ylabel_str = "",          // Y-axis label
                   std::string legend_pos = "TR")        // Legend position
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_mc         = (PlotUtils::MnvH1D*)h_input_mc         -> Clone("mnvh_mc");
    PlotUtils::MnvH1D* mnvh_mc_Pi0     = (PlotUtils::MnvH1D*)h_input_mc_Pi0     -> Clone("mnvh_mc_Pi0");
    PlotUtils::MnvH1D* mnvh_mc_Proton  = (PlotUtils::MnvH1D*)h_input_mc_Proton  -> Clone("mnvh_mc_Proton");
    PlotUtils::MnvH1D* mnvh_mc_Neutron = (PlotUtils::MnvH1D*)h_input_mc_Neutron -> Clone("mnvh_mc_Neutron");
    PlotUtils::MnvH1D* mnvh_mc_Pion    = (PlotUtils::MnvH1D*)h_input_mc_Pion    -> Clone("mnvh_mc_Pion");
    PlotUtils::MnvH1D* mnvh_mc_EM      = (PlotUtils::MnvH1D*)h_input_mc_EM      -> Clone("mnvh_mc_EM");
    PlotUtils::MnvH1D* mnvh_mc_Muon    = (PlotUtils::MnvH1D*)h_input_mc_Muon    -> Clone("mnvh_mc_Muon");
    PlotUtils::MnvH1D* mnvh_mc_OthPdg  = (PlotUtils::MnvH1D*)h_input_mc_OthPdg  -> Clone("mnvh_mc_OthPdg");
    PlotUtils::MnvH1D* mnvh_mc_MCXtalk = (PlotUtils::MnvH1D*)h_input_mc_MCXtalk -> Clone("mnvh_mc_MCXtalk");
    PlotUtils::MnvH1D* mnvh_mc_Overlay = (PlotUtils::MnvH1D*)h_input_mc_Overlay -> Clone("mnvh_mc_Overlay");
    
    PlotUtils::MnvH1D* mnvh_mc_Pi0EM = (PlotUtils::MnvH1D*)mnvh_mc_Pi0 -> Clone("mnvh_mc_Pi0EM");
    mnvh_mc_Pi0EM -> Add(mnvh_mc_EM);
    
    // Set MC histogram labels
    std::string legend_label;
    double area_mc         = mnvh_mc         -> Integral(0, mnvh_mc         -> GetNbinsX()+1);
    double area_mc_Pi0EM   = mnvh_mc_Pi0EM   -> Integral(0, mnvh_mc_Pi0EM   -> GetNbinsX()+1);
    double area_mc_Proton  = mnvh_mc_Proton  -> Integral(0, mnvh_mc_Proton  -> GetNbinsX()+1);
    double area_mc_Neutron = mnvh_mc_Neutron -> Integral(0, mnvh_mc_Neutron -> GetNbinsX()+1);
    double area_mc_Pion    = mnvh_mc_Pion    -> Integral(0, mnvh_mc_Pion    -> GetNbinsX()+1);
    double area_mc_Muon    = mnvh_mc_Muon    -> Integral(0, mnvh_mc_Muon    -> GetNbinsX()+1);
    double area_mc_OthPdg  = mnvh_mc_OthPdg  -> Integral(0, mnvh_mc_OthPdg  -> GetNbinsX()+1);
    double area_mc_MCXtalk = mnvh_mc_MCXtalk -> Integral(0, mnvh_mc_MCXtalk -> GetNbinsX()+1);
    double area_mc_Overlay = mnvh_mc_Overlay -> Integral(0, mnvh_mc_Overlay -> GetNbinsX()+1);
    
    legend_label = Form("#pi^{0} + EM (%.1f%%)", 100.0*(area_mc_Pi0EM/area_mc));
    mnvh_mc_Pi0EM -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Proton (%.1f%%)", 100.0*(area_mc_Proton/area_mc));
    mnvh_mc_Proton -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Neutron (%.1f%%)", 100.0*(area_mc_Neutron/area_mc));
    mnvh_mc_Neutron -> SetTitle(legend_label.c_str());
    
    legend_label = Form("#pi^{#pm} (%.1f%%)", 100.0*(area_mc_Pion/area_mc));
    mnvh_mc_Pion -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Muon (%.1f%%)", 100.0*(area_mc_Muon/area_mc));
    mnvh_mc_Muon -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Other PDG (%.1f%%)", 100.0*(area_mc_OthPdg/area_mc));
    mnvh_mc_OthPdg -> SetTitle(legend_label.c_str());
    
    legend_label = Form("MC X-talk (%.1f%%)", 100.0*(area_mc_MCXtalk/area_mc));
    mnvh_mc_MCXtalk -> SetTitle(legend_label.c_str());
    
    legend_label = Form("Overlay (%.1f%%)", 100.0*(area_mc_Overlay/area_mc));
    mnvh_mc_Overlay -> SetTitle(legend_label.c_str());
    
    // Set MC histogram colors
    SetHistColorScheme(mnvh_mc_Pi0EM,   0, 3);
    SetHistColorScheme(mnvh_mc_Proton,  1, 3);
    SetHistColorScheme(mnvh_mc_Neutron, 2, 3);
    SetHistColorScheme(mnvh_mc_Pion,    3, 3);
    SetHistColorScheme(mnvh_mc_Muon,    4, 3);
    SetHistColorScheme(mnvh_mc_OthPdg,  5, 3);
    SetHistColorScheme(mnvh_mc_MCXtalk, 6, 3);
    SetHistColorScheme(mnvh_mc_Overlay, 7, 3);
    
    // Set MC histogram array
    TObjArray* h_mc_array = new TObjArray();
    h_mc_array -> Add(mnvh_mc_Overlay);
    h_mc_array -> Add(mnvh_mc_MCXtalk);
    h_mc_array -> Add(mnvh_mc_OthPdg);
    h_mc_array -> Add(mnvh_mc_Muon);
    h_mc_array -> Add(mnvh_mc_Pion);
    h_mc_array -> Add(mnvh_mc_Neutron);
    h_mc_array -> Add(mnvh_mc_Proton);
    h_mc_array -> Add(mnvh_mc_Pi0EM);
    
    // Set plot axis labels from input
    if ( xlabel_str == "" ) plot_info.SetXLabel(mnvh_mc);
    else                    plot_info.SetXLabel(mnvh_mc, xlabel_str);
    
    if ( ylabel_str == "" ) plot_info.SetYLabel(mnvh_mc, mnvh_mc->GetNormBinWidth());
    else                    plot_info.SetYLabel(mnvh_mc, ylabel_str);
    
    
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
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
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
    
    // Release
    delete mnvh_mc;
    delete mnvh_mc_Pi0;
    delete mnvh_mc_Proton;
    delete mnvh_mc_Neutron;
    delete mnvh_mc_Pion;
    delete mnvh_mc_EM;
    delete mnvh_mc_Muon;
    delete mnvh_mc_OthPdg;
    delete mnvh_mc_MCXtalk;
    delete mnvh_mc_Overlay;
    delete mnvh_mc_Pi0EM;
    delete h_mc_array;
}





// ================================================================================================================================================================
//  PLOT PURITY PER BIN OF MC PHYSICS COMPONENTS
// ================================================================================================================================================================

void PlotMCPurityPerBin_Selection(PlotInfo plot_info,
                                  PlotUtils::MnvH1D* h_input_mc_Signal,
                                  PlotUtils::MnvH1D* h_input_mc_Pi0HighW,
                                  PlotUtils::MnvH1D* h_input_mc_QElike,
                                  PlotUtils::MnvH1D* h_input_mc_PionProd,
                                  std::string output_str,
                                  std::string title_str,
                                  std::vector<double> arrow_X,          // X positions of cut arrows
                                  std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                                  std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                                  std::vector<std::string> arrow_dir,   // Directions of cut arrows
                                  std::string xlabel_str = "",
                                  std::string ylabel_str = "",
                                  std::string legend_pos = "TR")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_Signal   = (PlotUtils::MnvH1D*)h_input_mc_Signal   -> Clone("mnvh_Signal");
    PlotUtils::MnvH1D* mnvh_Pi0HighW = (PlotUtils::MnvH1D*)h_input_mc_Pi0HighW -> Clone("mnvh_Pi0HighW");
    PlotUtils::MnvH1D* mnvh_QElike   = (PlotUtils::MnvH1D*)h_input_mc_QElike   -> Clone("mnvh_QElike");
    PlotUtils::MnvH1D* mnvh_PionProd = (PlotUtils::MnvH1D*)h_input_mc_PionProd -> Clone("mnvh_PionProd");
    
    PlotUtils::MnvH1D* mnvh_mc = (PlotUtils::MnvH1D*)mnvh_Signal->Clone("mnvh_mc");
    mnvh_mc -> Add(mnvh_Pi0HighW);
    mnvh_mc -> Add(mnvh_QElike);
    mnvh_mc -> Add(mnvh_PionProd);
    
    PlotUtils::MnvH1D* mnvh_SignalRatio   = (PlotUtils::MnvH1D*)mnvh_Signal   -> Clone("mnvh_SignalRatio");
    PlotUtils::MnvH1D* mnvh_Pi0HighWRatio = (PlotUtils::MnvH1D*)mnvh_Pi0HighW -> Clone("mnvh_Pi0HighWRatio");
    PlotUtils::MnvH1D* mnvh_QElikeRatio   = (PlotUtils::MnvH1D*)mnvh_QElike   -> Clone("mnvh_QElikeRatio");
    PlotUtils::MnvH1D* mnvh_PionProdRatio = (PlotUtils::MnvH1D*)mnvh_PionProd -> Clone("mnvh_PionProdRatio");
    
    mnvh_SignalRatio   -> Reset();
    mnvh_Pi0HighWRatio -> Reset();
    mnvh_QElikeRatio   -> Reset();
    mnvh_PionProdRatio -> Reset();
    
    for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
        double mc       = mnvh_mc->GetBinContent(bin);
        double signal   = mnvh_Signal->GetBinContent(bin);
        double pi0highw = mnvh_Pi0HighW->GetBinContent(bin);
        double qelike   = mnvh_QElike->GetBinContent(bin);
        double pionprod = mnvh_PionProd->GetBinContent(bin);
        
        if ( mc > 0.0 ) {
            mnvh_SignalRatio   -> SetBinContent(bin, signal/mc);
            mnvh_Pi0HighWRatio -> SetBinContent(bin, pi0highw/mc);
            mnvh_QElikeRatio   -> SetBinContent(bin, qelike/mc);
            mnvh_PionProdRatio -> SetBinContent(bin, pionprod/mc);
        }
        else {
            mnvh_SignalRatio   -> SetBinContent(bin, 0.0);
            mnvh_Pi0HighWRatio -> SetBinContent(bin, 0.0);
            mnvh_QElikeRatio   -> SetBinContent(bin, 0.0);
            mnvh_PionProdRatio -> SetBinContent(bin, 0.0);
        }
    }
    
    
    // Draw
    // ====
    
    mnvh_SignalRatio   -> SetLineColor(kAzure-4);
    mnvh_Pi0HighWRatio -> SetLineColor(kRed);
    mnvh_QElikeRatio   -> SetLineColor(kCyan+3);
    mnvh_PionProdRatio -> SetLineColor(kRed+2);
    
    mnvh_SignalRatio   -> SetLineWidth(5);
    mnvh_Pi0HighWRatio -> SetLineWidth(5);
    mnvh_QElikeRatio   -> SetLineWidth(5);
    mnvh_PionProdRatio -> SetLineWidth(5);
    
    mnvh_PionProdRatio -> SetMaximum(1.2);
    mnvh_PionProdRatio -> GetXaxis()->SetTitle(xlabel_str.c_str());
    mnvh_PionProdRatio -> GetYaxis()->SetTitle(ylabel_str.c_str());
    mnvh_PionProdRatio -> GetXaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_x);
    mnvh_PionProdRatio -> GetYaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_y);
    mnvh_PionProdRatio -> GetXaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_x);
    mnvh_PionProdRatio -> GetYaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_y);
    mnvh_PionProdRatio -> GetXaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_PionProdRatio -> GetYaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_PionProdRatio -> GetXaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_PionProdRatio -> GetYaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_PionProdRatio -> GetXaxis()->CenterTitle(kTRUE);
    
    const char* x_label = mnvh_PionProdRatio->GetXaxis()->GetTitle();
    const char* y_label = mnvh_PionProdRatio->GetYaxis()->GetTitle();
    
    mnvh_PionProdRatio -> Draw("HIST");
    mnvh_QElikeRatio   -> Draw("SAME");
    mnvh_Pi0HighWRatio -> Draw("SAME");
    mnvh_SignalRatio   -> Draw("SAME");
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
    }
    
    
    // Add legend box
    // ==============
    
    std::vector<std::string> names_vector;
    names_vector.push_back(GetTruthClassification_LegendLabel(kSignal));
    names_vector.push_back(GetTruthClassification_LegendLabel(kBackgrPi0HighW));
    names_vector.push_back(GetTruthClassification_LegendLabel(kBackgrQElike));
    names_vector.push_back(GetTruthClassification_LegendLabel(kBackgrPionProd));
    
    std::vector<TH1*> hists_vector;
    hists_vector.push_back(mnvh_SignalRatio);
    hists_vector.push_back(mnvh_Pi0HighWRatio);
    hists_vector.push_back(mnvh_QElikeRatio);
    hists_vector.push_back(mnvh_PionProdRatio);
    
    std::vector<std::string> opts_vector;
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    
    size_t legend_width = plot_info.m_mnv_plotter.GetLegendWidthInLetters(names_vector);
    double x1, y1, x2, y2;
    plot_info.m_mnv_plotter.DecodeLegendPosition(x1, y1, x2, y2, legend_pos, hists_vector.size(), legend_width);
    plot_info.m_mnv_plotter.AddPlotLegend(hists_vector, names_vector, opts_vector, x1, y1, x2-x1, y2-y1,
                                          plot_info.m_mnv_plotter.legend_text_size);
    
    
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
    delete mnvh_mc;
    delete mnvh_Signal;
    delete mnvh_Pi0HighW;
    delete mnvh_QElike;
    delete mnvh_PionProd;
    delete mnvh_SignalRatio;
    delete mnvh_Pi0HighWRatio;
    delete mnvh_QElikeRatio;
    delete mnvh_PionProdRatio;
}





// ================================================================================================================================================================
//  PLOT PURITY PER BIN OF MC PDG
// ================================================================================================================================================================

void PlotMCPurityPerBin_Pdg(PlotInfo plot_info,
                            PlotUtils::MnvH1D* h_input_mc_Pi0,
                            PlotUtils::MnvH1D* h_input_mc_Proton,
                            PlotUtils::MnvH1D* h_input_mc_Neutron,
                            PlotUtils::MnvH1D* h_input_mc_Pion,
                            PlotUtils::MnvH1D* h_input_mc_EM,
                            PlotUtils::MnvH1D* h_input_mc_Muon,
                            PlotUtils::MnvH1D* h_input_mc_OthPdg,
                            PlotUtils::MnvH1D* h_input_mc_MCXtalk,
                            PlotUtils::MnvH1D* h_input_mc_Overlay,
                            std::string output_str,
                            std::string title_str,
                            std::vector<double> arrow_X,          // X positions of cut arrows
                            std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                            std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                            std::vector<std::string> arrow_dir,   // Directions of cut arrows
                            std::string xlabel_str = "",
                            std::string ylabel_str = "",
                            std::string legend_pos = "TR")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_Pi0     = (PlotUtils::MnvH1D*)h_input_mc_Pi0     -> Clone("mnvh_Pi0");
    PlotUtils::MnvH1D* mnvh_Proton  = (PlotUtils::MnvH1D*)h_input_mc_Proton  -> Clone("mnvh_Proton");
    PlotUtils::MnvH1D* mnvh_Neutron = (PlotUtils::MnvH1D*)h_input_mc_Neutron -> Clone("mnvh_Neutron");
    PlotUtils::MnvH1D* mnvh_Pion    = (PlotUtils::MnvH1D*)h_input_mc_Pion    -> Clone("mnvh_Pion");
    PlotUtils::MnvH1D* mnvh_EM      = (PlotUtils::MnvH1D*)h_input_mc_EM      -> Clone("mnvh_EM");
    PlotUtils::MnvH1D* mnvh_Muon    = (PlotUtils::MnvH1D*)h_input_mc_Muon    -> Clone("mnvh_Muon");
    PlotUtils::MnvH1D* mnvh_OthPdg  = (PlotUtils::MnvH1D*)h_input_mc_OthPdg  -> Clone("mnvh_OthPdg");
    PlotUtils::MnvH1D* mnvh_MCXtalk = (PlotUtils::MnvH1D*)h_input_mc_MCXtalk -> Clone("mnvh_MCXtalk");
    PlotUtils::MnvH1D* mnvh_Overlay = (PlotUtils::MnvH1D*)h_input_mc_Overlay -> Clone("mnvh_Overlay");
    
    PlotUtils::MnvH1D* mnvh_Pi0EM = (PlotUtils::MnvH1D*)mnvh_Pi0 -> Clone("mnvh_Pi0EM");
    mnvh_Pi0EM -> Add(mnvh_EM);
    
    PlotUtils::MnvH1D* mnvh_mc = (PlotUtils::MnvH1D*)mnvh_Pi0EM->Clone("mnvh_mc");
    mnvh_mc -> Add(mnvh_Proton);
    mnvh_mc -> Add(mnvh_Neutron);
    mnvh_mc -> Add(mnvh_Pion);
    mnvh_mc -> Add(mnvh_Muon);
    mnvh_mc -> Add(mnvh_OthPdg);
    mnvh_mc -> Add(mnvh_MCXtalk);
    mnvh_mc -> Add(mnvh_Overlay);
    
    PlotUtils::MnvH1D* mnvh_Pi0EMRatio   = (PlotUtils::MnvH1D*)mnvh_Pi0EM   -> Clone("mnvh_Pi0EMRatio");
    PlotUtils::MnvH1D* mnvh_ProtonRatio  = (PlotUtils::MnvH1D*)mnvh_Proton  -> Clone("mnvh_ProtonRatio");
    PlotUtils::MnvH1D* mnvh_NeutronRatio = (PlotUtils::MnvH1D*)mnvh_Neutron -> Clone("mnvh_NeutronRatio");
    PlotUtils::MnvH1D* mnvh_PionRatio    = (PlotUtils::MnvH1D*)mnvh_Pion    -> Clone("mnvh_PionRatio");
    PlotUtils::MnvH1D* mnvh_MuonRatio    = (PlotUtils::MnvH1D*)mnvh_Muon    -> Clone("mnvh_MuonRatio");
    PlotUtils::MnvH1D* mnvh_OthPdgRatio  = (PlotUtils::MnvH1D*)mnvh_OthPdg  -> Clone("mnvh_OthPdgRatio");
    PlotUtils::MnvH1D* mnvh_MCXtalkRatio = (PlotUtils::MnvH1D*)mnvh_MCXtalk -> Clone("mnvh_MCXtalkRatio");
    PlotUtils::MnvH1D* mnvh_OverlayRatio = (PlotUtils::MnvH1D*)mnvh_Overlay -> Clone("mnvh_OverlayRatio");
    
    mnvh_Pi0EMRatio   -> Reset();
    mnvh_ProtonRatio  -> Reset();
    mnvh_NeutronRatio -> Reset();
    mnvh_PionRatio    -> Reset();
    mnvh_MuonRatio    -> Reset();
    mnvh_OthPdgRatio  -> Reset();
    mnvh_MCXtalkRatio -> Reset();
    mnvh_OverlayRatio -> Reset();
    
    for ( int bin = 1; bin <= mnvh_mc->GetNbinsX(); ++bin ) {
        double mc      = mnvh_mc->GetBinContent(bin);
        double pi0em   = mnvh_Pi0EM->GetBinContent(bin);
        double proton  = mnvh_Proton->GetBinContent(bin);
        double neutron = mnvh_Neutron->GetBinContent(bin);
        double pion    = mnvh_Pion->GetBinContent(bin);
        double muon    = mnvh_Muon->GetBinContent(bin);
        double othpdg  = mnvh_OthPdg->GetBinContent(bin);
        double mcxtalk = mnvh_MCXtalk->GetBinContent(bin);
        double overlay = mnvh_Overlay->GetBinContent(bin);
        
        if ( mc > 0.0 ) {
            mnvh_Pi0EMRatio   -> SetBinContent(bin, pi0em/mc);
            mnvh_ProtonRatio  -> SetBinContent(bin, proton/mc);
            mnvh_NeutronRatio -> SetBinContent(bin, neutron/mc);
            mnvh_PionRatio    -> SetBinContent(bin, pion/mc);
            mnvh_MuonRatio    -> SetBinContent(bin, muon/mc);
            mnvh_OthPdgRatio  -> SetBinContent(bin, othpdg/mc);
            mnvh_MCXtalkRatio -> SetBinContent(bin, mcxtalk/mc);
            mnvh_OverlayRatio -> SetBinContent(bin, overlay/mc);
        }
        else {
            mnvh_Pi0EMRatio   -> SetBinContent(bin, 0.0);
            mnvh_ProtonRatio  -> SetBinContent(bin, 0.0);
            mnvh_NeutronRatio -> SetBinContent(bin, 0.0);
            mnvh_PionRatio    -> SetBinContent(bin, 0.0);
            mnvh_MuonRatio    -> SetBinContent(bin, 0.0);
            mnvh_OthPdgRatio  -> SetBinContent(bin, 0.0);
            mnvh_MCXtalkRatio -> SetBinContent(bin, 0.0);
            mnvh_OverlayRatio -> SetBinContent(bin, 0.0);
        }
    }
    
    
    // Draw
    // ====
    
    mnvh_Pi0EMRatio   -> SetLineColor(kAzure-4);
    mnvh_ProtonRatio  -> SetLineColor(kCyan+3);
    mnvh_NeutronRatio -> SetLineColor(kViolet-4);
    mnvh_PionRatio    -> SetLineColor(kRed+2);
    mnvh_MuonRatio    -> SetLineColor(kYellow+1);
    mnvh_OthPdgRatio  -> SetLineColor(kGreen+2);
    mnvh_MCXtalkRatio -> SetLineColor(kGray);
    mnvh_OverlayRatio -> SetLineColor(kGray+1);
    
    mnvh_Pi0EMRatio   -> SetLineWidth(5);
    mnvh_ProtonRatio  -> SetLineWidth(5);
    mnvh_NeutronRatio -> SetLineWidth(5);
    mnvh_PionRatio    -> SetLineWidth(5);
    mnvh_MuonRatio    -> SetLineWidth(5);
    mnvh_OthPdgRatio  -> SetLineWidth(5);
    mnvh_MCXtalkRatio -> SetLineWidth(5);
    mnvh_OverlayRatio -> SetLineWidth(5);
    
    mnvh_MCXtalkRatio -> SetMaximum(1.2);
    mnvh_MCXtalkRatio -> GetXaxis()->SetTitle(xlabel_str.c_str());
    mnvh_MCXtalkRatio -> GetYaxis()->SetTitle(ylabel_str.c_str());
    mnvh_MCXtalkRatio -> GetXaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_x);
    mnvh_MCXtalkRatio -> GetYaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_y);
    mnvh_MCXtalkRatio -> GetXaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_x);
    mnvh_MCXtalkRatio -> GetYaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_y);
    mnvh_MCXtalkRatio -> GetXaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_MCXtalkRatio -> GetYaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_MCXtalkRatio -> GetXaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_MCXtalkRatio -> GetYaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_MCXtalkRatio -> GetXaxis()->CenterTitle(kTRUE);
    
    const char* x_label = mnvh_MCXtalkRatio->GetXaxis()->GetTitle();
    const char* y_label = mnvh_MCXtalkRatio->GetYaxis()->GetTitle();
    
    mnvh_MCXtalkRatio -> Draw("HIST");
    mnvh_OverlayRatio -> Draw("SAME");
    mnvh_OthPdgRatio  -> Draw("SAME");
    mnvh_MuonRatio    -> Draw("SAME");
    mnvh_PionRatio    -> Draw("SAME");
    mnvh_NeutronRatio -> Draw("SAME");
    mnvh_ProtonRatio  -> Draw("SAME");
    mnvh_Pi0EMRatio   -> Draw("SAME");
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
    }
    
    
    // Add legend box
    // ==============
    
    std::vector<std::string> names_vector;
    names_vector.push_back("#pi^{0} + EM");
    names_vector.push_back("Proton");
    names_vector.push_back("Neutron");
    names_vector.push_back("#pi^{#pm}");
    names_vector.push_back("Muon");
    names_vector.push_back("Other PDG");
    names_vector.push_back("MC X-talk");
    names_vector.push_back("Overlay");
    
    std::vector<TH1*> hists_vector;
    hists_vector.push_back(mnvh_Pi0EMRatio);
    hists_vector.push_back(mnvh_ProtonRatio);
    hists_vector.push_back(mnvh_NeutronRatio);
    hists_vector.push_back(mnvh_PionRatio);
    hists_vector.push_back(mnvh_MuonRatio);
    hists_vector.push_back(mnvh_OthPdgRatio);
    hists_vector.push_back(mnvh_MCXtalkRatio);
    hists_vector.push_back(mnvh_OverlayRatio);
    
    std::vector<std::string> opts_vector;
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    
    size_t legend_width = plot_info.m_mnv_plotter.GetLegendWidthInLetters(names_vector);
    double x1, y1, x2, y2;
    plot_info.m_mnv_plotter.DecodeLegendPosition(x1, y1, x2, y2, legend_pos, hists_vector.size(), legend_width);
    plot_info.m_mnv_plotter.AddPlotLegend(hists_vector, names_vector, opts_vector, x1, y1, x2-x1, y2-y1,
                                          plot_info.m_mnv_plotter.legend_text_size);
    
    
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
    delete mnvh_mc;
    delete mnvh_Pi0;
    delete mnvh_Proton;
    delete mnvh_Neutron;
    delete mnvh_Pion;
    delete mnvh_EM;
    delete mnvh_Muon;
    delete mnvh_OthPdg;
    delete mnvh_MCXtalk;
    delete mnvh_Overlay;
    delete mnvh_Pi0EM;
    delete mnvh_Pi0EMRatio;
    delete mnvh_ProtonRatio;
    delete mnvh_NeutronRatio;
    delete mnvh_PionRatio;
    delete mnvh_MuonRatio;
    delete mnvh_OthPdgRatio;
    delete mnvh_MCXtalkRatio;
    delete mnvh_OverlayRatio;
}






// ================================================================================================================================================================
//  PLOT MC PDG NORMALIZE TO UNIT - V1
// ================================================================================================================================================================

void PlotMCUnitNorm_PdgV1(PlotInfo plot_info,
                          PlotUtils::MnvH1D* h_input_mc_Pi0,
                          PlotUtils::MnvH1D* h_input_mc_Proton,
                          PlotUtils::MnvH1D* h_input_mc_Neutron,
                          PlotUtils::MnvH1D* h_input_mc_Pion,
                          PlotUtils::MnvH1D* h_input_mc_EM,
                          std::string output_str,
                          std::string title_str,
                          std::vector<double> arrow_X,          // X positions of cut arrows
                          std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                          std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                          std::vector<std::string> arrow_dir,   // Directions of cut arrows
                          std::string xlabel_str = "",
                          std::string ylabel_str = "",
                          std::string legend_pos = "TR")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_Pi0     = (PlotUtils::MnvH1D*)h_input_mc_Pi0     -> Clone("mnvh_Pi0");
    PlotUtils::MnvH1D* mnvh_Proton  = (PlotUtils::MnvH1D*)h_input_mc_Proton  -> Clone("mnvh_Proton");
    PlotUtils::MnvH1D* mnvh_Neutron = (PlotUtils::MnvH1D*)h_input_mc_Neutron -> Clone("mnvh_Neutron");
    PlotUtils::MnvH1D* mnvh_Pion    = (PlotUtils::MnvH1D*)h_input_mc_Pion    -> Clone("mnvh_Pion");
    PlotUtils::MnvH1D* mnvh_EM      = (PlotUtils::MnvH1D*)h_input_mc_EM      -> Clone("mnvh_EM");
    
    PlotUtils::MnvH1D* mnvh_Pi0EM = (PlotUtils::MnvH1D*)mnvh_Pi0 -> Clone("mnvh_Pi0EM");
    mnvh_Pi0EM -> Add(mnvh_EM);
    
    double area_Pi0EM   = mnvh_Pi0EM   -> Integral(0, mnvh_Pi0EM   -> GetNbinsX()+1);
    double area_Proton  = mnvh_Proton  -> Integral(0, mnvh_Proton  -> GetNbinsX()+1);
    double area_Neutron = mnvh_Neutron -> Integral(0, mnvh_Neutron -> GetNbinsX()+1);
    double area_Pion    = mnvh_Pion    -> Integral(0, mnvh_Pion    -> GetNbinsX()+1);
    
    mnvh_Pi0EM   -> Scale(1.0/area_Pi0EM);
    mnvh_Proton  -> Scale(1.0/area_Proton);
    mnvh_Neutron -> Scale(1.0/area_Neutron);
    mnvh_Pion    -> Scale(1.0/area_Pion);
    
    mnvh_Pi0EM   -> Scale(mnvh_Pi0EM->GetNormBinWidth(),   "width");
    mnvh_Proton  -> Scale(mnvh_Proton->GetNormBinWidth(),  "width");
    mnvh_Neutron -> Scale(mnvh_Neutron->GetNormBinWidth(), "width");
    mnvh_Pion    -> Scale(mnvh_Pion->GetNormBinWidth(),    "width");
    
    
    // Draw
    // ====
    
    plot_info.m_mnv_plotter.axis_title_offset_y = 0.80;
    
    mnvh_Pi0EM   -> SetLineColor(kAzure-4);
    mnvh_Proton  -> SetLineColor(kCyan+3);
    mnvh_Neutron -> SetLineColor(kViolet-4);
    mnvh_Pion    -> SetLineColor(kRed+2);
    
    mnvh_Pi0EM   -> SetLineWidth(5);
    mnvh_Proton  -> SetLineWidth(5);
    mnvh_Neutron -> SetLineWidth(5);
    mnvh_Pion    -> SetLineWidth(5);
    
    double Ymax_Pi0EM   = mnvh_Pi0EM->GetMaximum();
    double Ymax_Proton  = mnvh_Proton->GetMaximum();
    double Ymax_Neutron = mnvh_Neutron->GetMaximum();
    double Ymax_Pion    = mnvh_Pion->GetMaximum();
    
    std::vector<double> Ymax_vector;
    Ymax_vector.push_back(Ymax_Pi0EM);
    Ymax_vector.push_back(Ymax_Proton);
    Ymax_vector.push_back(Ymax_Neutron);
    Ymax_vector.push_back(Ymax_Pion);
    double Ymax = *std::max_element(Ymax_vector.begin(), Ymax_vector.end());
    Ymax *= plot_info.m_mnv_plotter.headroom;
    
    mnvh_Neutron -> SetMaximum(Ymax);
    mnvh_Neutron -> GetXaxis()->SetTitle(xlabel_str.c_str());
    mnvh_Neutron -> GetYaxis()->SetTitle(ylabel_str.c_str());
    mnvh_Neutron -> GetXaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_x);
    mnvh_Neutron -> GetYaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_y);
    mnvh_Neutron -> GetXaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_x);
    mnvh_Neutron -> GetYaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_y);
    mnvh_Neutron -> GetXaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_Neutron -> GetYaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_Neutron -> GetXaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_Neutron -> GetYaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_Neutron -> GetXaxis()->CenterTitle(kTRUE);
    
    const char* x_label = mnvh_Neutron->GetXaxis()->GetTitle();
    const char* y_label = mnvh_Neutron->GetYaxis()->GetTitle();
    
    mnvh_Neutron -> Draw("HIST");
    mnvh_Pion    -> Draw("HIST SAME");
    mnvh_Proton  -> Draw("HIST SAME");
    mnvh_Pi0EM   -> Draw("HIST SAME");
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
    }
    
    
    // Add legend box
    // ==============
    
    std::vector<std::string> names_vector;
    names_vector.push_back("#pi^{0} + EM");
    names_vector.push_back("Proton");
    names_vector.push_back("Neutron");
    names_vector.push_back("#pi^{#pm}");
    
    std::vector<TH1*> hists_vector;
    hists_vector.push_back(mnvh_Pi0EM);
    hists_vector.push_back(mnvh_Proton);
    hists_vector.push_back(mnvh_Neutron);
    hists_vector.push_back(mnvh_Pion);
    
    std::vector<std::string> opts_vector;
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    
    size_t legend_width = plot_info.m_mnv_plotter.GetLegendWidthInLetters(names_vector);
    double x1, y1, x2, y2;
    plot_info.m_mnv_plotter.DecodeLegendPosition(x1, y1, x2, y2, legend_pos, hists_vector.size(), legend_width);
    plot_info.m_mnv_plotter.AddPlotLegend(hists_vector, names_vector, opts_vector, x1, y1, x2-x1, y2-y1,
                                          plot_info.m_mnv_plotter.legend_text_size);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return settings to default
    plot_info.m_mnv_plotter.axis_title_offset_y = 1.15;
    
    // Release
    delete mnvh_Pi0;
    delete mnvh_Proton;
    delete mnvh_Neutron;
    delete mnvh_Pion;
    delete mnvh_EM;
    delete mnvh_Pi0EM;
}






// ================================================================================================================================================================
//  PLOT MC PDG NORMALIZE TO UNIT - V2
// ================================================================================================================================================================

void PlotMCUnitNorm_PdgV2(PlotInfo plot_info,
                          PlotUtils::MnvH1D* h_input_mc_Pi0,
                          PlotUtils::MnvH1D* h_input_mc_Proton,
                          PlotUtils::MnvH1D* h_input_mc_Pion,
                          PlotUtils::MnvH1D* h_input_mc_EM,
                          std::string output_str,
                          std::string title_str,
                          std::vector<double> arrow_X,          // X positions of cut arrows
                          std::vector<double> arrow_Ymax,       // Ymax limits of cut arrows (in fractions of canvas Ymax)
                          std::vector<double> arrow_length,     // Lengths of cut arrows (in fractions of canvas Xmax-Xmin)
                          std::vector<std::string> arrow_dir,   // Directions of cut arrows
                          std::string xlabel_str = "",
                          std::string ylabel_str = "",
                          std::string legend_pos = "TR")
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH1D* mnvh_Pi0    = (PlotUtils::MnvH1D*)h_input_mc_Pi0     -> Clone("mnvh_Pi0");
    PlotUtils::MnvH1D* mnvh_Proton = (PlotUtils::MnvH1D*)h_input_mc_Proton  -> Clone("mnvh_Proton");
    PlotUtils::MnvH1D* mnvh_Pion   = (PlotUtils::MnvH1D*)h_input_mc_Pion    -> Clone("mnvh_Pion");
    PlotUtils::MnvH1D* mnvh_EM     = (PlotUtils::MnvH1D*)h_input_mc_EM      -> Clone("mnvh_EM");
    
    PlotUtils::MnvH1D* mnvh_Pi0EM = (PlotUtils::MnvH1D*)mnvh_Pi0 -> Clone("mnvh_Pi0EM");
    mnvh_Pi0EM -> Add(mnvh_EM);
    
    double area_Pi0EM  = mnvh_Pi0EM   -> Integral(0, mnvh_Pi0EM   -> GetNbinsX()+1);
    double area_Proton = mnvh_Proton  -> Integral(0, mnvh_Proton  -> GetNbinsX()+1);
    double area_Pion   = mnvh_Pion    -> Integral(0, mnvh_Pion    -> GetNbinsX()+1);
    
    mnvh_Pi0EM  -> Scale(1.0/area_Pi0EM);
    mnvh_Proton -> Scale(1.0/area_Proton);
    mnvh_Pion   -> Scale(1.0/area_Pion);
    
    mnvh_Pi0EM  -> Scale(mnvh_Pi0EM->GetNormBinWidth(),   "width");
    mnvh_Proton -> Scale(mnvh_Proton->GetNormBinWidth(),  "width");
    mnvh_Pion   -> Scale(mnvh_Pion->GetNormBinWidth(),    "width");
    
    
    // Draw
    // ====
    
    plot_info.m_mnv_plotter.axis_title_offset_y = 0.80;
    
    mnvh_Pi0EM  -> SetLineColor(kAzure-4);
    mnvh_Proton -> SetLineColor(kCyan+3);
    mnvh_Pion   -> SetLineColor(kRed+2);
    
    mnvh_Pi0EM  -> SetLineWidth(5);
    mnvh_Proton -> SetLineWidth(5);
    mnvh_Pion   -> SetLineWidth(5);
    
    double Ymax_Pi0EM  = mnvh_Pi0EM->GetMaximum();
    double Ymax_Proton = mnvh_Proton->GetMaximum();
    double Ymax_Pion   = mnvh_Pion->GetMaximum();
    
    std::vector<double> Ymax_vector;
    Ymax_vector.push_back(Ymax_Pi0EM);
    Ymax_vector.push_back(Ymax_Proton);
    Ymax_vector.push_back(Ymax_Pion);
    double Ymax = *std::max_element(Ymax_vector.begin(), Ymax_vector.end());
    Ymax *= plot_info.m_mnv_plotter.headroom;
    
    mnvh_Pion -> SetMaximum(Ymax);
    mnvh_Pion -> GetXaxis()->SetTitle(xlabel_str.c_str());
    mnvh_Pion -> GetYaxis()->SetTitle(ylabel_str.c_str());
    mnvh_Pion -> GetXaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_x);
    mnvh_Pion -> GetYaxis()->SetTitleFont(plot_info.m_mnv_plotter.axis_title_font_y);
    mnvh_Pion -> GetXaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_x);
    mnvh_Pion -> GetYaxis()->SetTitleSize(plot_info.m_mnv_plotter.axis_title_size_y);
    mnvh_Pion -> GetXaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_Pion -> GetYaxis()->SetLabelFont(plot_info.m_mnv_plotter.axis_label_font);
    mnvh_Pion -> GetXaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_Pion -> GetYaxis()->SetLabelSize(plot_info.m_mnv_plotter.axis_label_size);
    mnvh_Pion -> GetXaxis()->CenterTitle(kTRUE);
    
    const char* x_label = mnvh_Pion->GetXaxis()->GetTitle();
    const char* y_label = mnvh_Pion->GetYaxis()->GetTitle();
    
    mnvh_Pion   -> Draw("HIST");
    mnvh_Proton -> Draw("HIST SAME");
    mnvh_Pi0EM  -> Draw("HIST SAME");
    
    
    // Cut arrows
    // ==========
    
    canvas.Update();
    double canvas_Xmin = canvas.GetUxmin();
    double canvas_Xmax = canvas.GetUxmax();
    double canvas_Ymin = canvas.GetUymin();
    double canvas_Ymax = canvas.GetUymax();
    
    plot_info.m_mnv_plotter.arrow_line_width = 5;
    plot_info.m_mnv_plotter.arrow_line_style = 9;
    plot_info.m_mnv_plotter.arrow_line_color = 1;
    plot_info.m_mnv_plotter.arrow_size       = 0.02;
    
    for ( unsigned int i = 0; i < arrow_X.size(); ++i ) {
        double cutX     = arrow_X.at(i);
        double Ylow     = canvas_Ymin;
        double Yhigh    = canvas_Ymax * arrow_Ymax.at(i);
        double length   = std::fabs(canvas_Xmax-canvas_Xmin) * arrow_length.at(i);
        std::string dir = arrow_dir.at(i);
        plot_info.m_mnv_plotter.AddCutArrow(cutX, Ylow, Yhigh, length, dir);
    }
    
    
    // Add legend box
    // ==============
    
    std::vector<std::string> names_vector;
    names_vector.push_back("#pi^{0} + EM");
    names_vector.push_back("Proton");
    names_vector.push_back("#pi^{#pm}");
    
    std::vector<TH1*> hists_vector;
    hists_vector.push_back(mnvh_Pi0EM);
    hists_vector.push_back(mnvh_Proton);
    hists_vector.push_back(mnvh_Pion);
    
    std::vector<std::string> opts_vector;
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    opts_vector.push_back("l");
    
    size_t legend_width = plot_info.m_mnv_plotter.GetLegendWidthInLetters(names_vector);
    double x1, y1, x2, y2;
    plot_info.m_mnv_plotter.DecodeLegendPosition(x1, y1, x2, y2, legend_pos, hists_vector.size(), legend_width);
    plot_info.m_mnv_plotter.AddPlotLegend(hists_vector, names_vector, opts_vector, x1, y1, x2-x1, y2-y1,
                                          plot_info.m_mnv_plotter.legend_text_size);
    
    
    // Other details
    // =============
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Return settings to default
    plot_info.m_mnv_plotter.axis_title_offset_y = 1.15;
    
    // Release
    delete mnvh_Pi0;
    delete mnvh_Proton;
    delete mnvh_Pion;
    delete mnvh_EM;
}





// ================================================================================================================================================================
//  PLOT GENERIC MnvH2D
// ================================================================================================================================================================

void Plot2D(PlotInfo plot_info,
            PlotUtils::MnvH2D* h2D_input,
            std::string output_str,
            std::string title_str,
            std::vector<std::string> functions,
            std::vector<double> x1_functions,
            std::vector<double> x2_functions,
            std::vector<double> x_vertlines,
            std::vector<double> y1_vertlines,
            std::vector<double> y2_vertlines,
            std::string xlabel_str = "",
            std::string ylabel_str = "",
            std::string zlabel_str = "",
            bool bin_width_norm    = true,
            bool use_log_z         = false,
            bool use_corr_palette  = false,
            int palette_input      = 55)    // kRainbow
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    // Set log Z and palette
    if ( use_log_z ) {
        canvas.SetLogz();
        plot_info.m_mnv_plotter.SetROOT6Palette(55);  // Use kRainbow always in log-z
    }
    else if ( use_corr_palette ) {
        plot_info.m_mnv_plotter.SetCorrelationPalette();
    }
    else {
        plot_info.m_mnv_plotter.SetROOT6Palette(palette_input);
    }
    
    
    // Get histogram
    // =============
    
    PlotUtils::MnvH2D* mnvh2D = (PlotUtils::MnvH2D*)h2D_input->Clone("mnvh2D");
    if ( bin_width_norm ) mnvh2D -> Scale(mnvh2D->GetNormBinWidthX() * mnvh2D->GetNormBinWidthY(), "width");
    
    // Axis labels
    plot_info.SetXLabel(mnvh2D, xlabel_str);
    plot_info.SetYLabel(mnvh2D, ylabel_str);
    plot_info.SetZLabel(mnvh2D, zlabel_str);
    
    
    // Draw
    // ====
    
    plot_info.Set2DHistoStyle(&canvas, mnvh2D);
    //if ( use_log_z ) mnvh2D -> SetMinimum(1.0);
    mnvh2D -> Draw("COLZ");
    
    // Plot title
    plot_info.m_mnv_plotter.title_size = 0.05;
    plot_info.SetTitle(title_str);
    
    // Draw functions
    for ( unsigned int i = 0; i < functions.size(); ++i ) {
        std::string func_str = functions.at(i);
        double x1 = x1_functions.at(i);
        double x2 = x2_functions.at(i);
        
        TF1* func = new TF1("func", Form("%s", func_str.c_str()), x1, x2);
        func -> SetLineStyle(1);
        func -> SetLineWidth(5);
        func -> SetLineColor(1);
        func -> Draw("SAME");
        //delete func;
    }
    
    // Draw vertical lines
    for ( unsigned int i = 0; i < x_vertlines.size(); ++i ) {
        double x  = x_vertlines.at(i);
        double y1 = y1_vertlines.at(i);
        double y2 = y2_vertlines.at(i);
        
        TLine line;
        line.SetLineStyle(1);
        line.SetLineWidth(5);
        line.SetLineColor(1);
        line.DrawLine(x, y1, x, y2);
    }
    
    
    // Other details
    // =============
    
    // Output name
    std::string output = Form("%s", output_str.c_str());
    
    // Print
    plot_info.m_mnv_plotter.MultiPrint(&canvas, output, plot_info.m_print_format);
    
    // Release
    delete mnvh2D;
}





// ================================================================================================================================================================
//  PLOT PDG 2D HISTOGRAMS RATIOS
// ================================================================================================================================================================

void PlotMCPurityPerBin_Pdg2D(PlotInfo plot_info,
                              PlotUtils::MnvH2D* h2D_input_mc_Pi0,
                              PlotUtils::MnvH2D* h2D_input_mc_Proton,
                              PlotUtils::MnvH2D* h2D_input_mc_Neutron,
                              PlotUtils::MnvH2D* h2D_input_mc_Pion,
                              PlotUtils::MnvH2D* h2D_input_mc_EM,
                              PlotUtils::MnvH2D* h2D_input_mc_Muon,
                              PlotUtils::MnvH2D* h2D_input_mc_OthPdg,
                              PlotUtils::MnvH2D* h2D_input_mc_MCXtalk,
                              PlotUtils::MnvH2D* h2D_input_mc_Overlay,
                              std::string output_str,
                              std::string title_str,
                              std::string option_material,
                              std::vector<std::string> functions,
                              std::vector<double> x1_functions,
                              std::vector<double> x2_functions,
                              std::vector<double> x_vertlines,
                              std::vector<double> y1_vertlines,
                              std::vector<double> y2_vertlines,
                              std::string xlabel_str = "",
                              std::string ylabel_str = "",
                              std::string zlabel_str = "",
                              bool use_log_z         = false,
                              bool use_corr_palette  = false,
                              int palette_input      = 55)    // kRainbow
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    // Define PDG histograms
    PlotUtils::MnvH2D* mnvh2D_Pi0     = (PlotUtils::MnvH2D*)h2D_input_mc_Pi0     -> Clone("mnvh2D_Pi0");
    PlotUtils::MnvH2D* mnvh2D_Proton  = (PlotUtils::MnvH2D*)h2D_input_mc_Proton  -> Clone("mnvh2D_Proton");
    PlotUtils::MnvH2D* mnvh2D_Neutron = (PlotUtils::MnvH2D*)h2D_input_mc_Neutron -> Clone("mnvh2D_Neutron");
    PlotUtils::MnvH2D* mnvh2D_Pion    = (PlotUtils::MnvH2D*)h2D_input_mc_Pion    -> Clone("mnvh2D_Pion");
    PlotUtils::MnvH2D* mnvh2D_EM      = (PlotUtils::MnvH2D*)h2D_input_mc_EM      -> Clone("mnvh2D_EM");
    PlotUtils::MnvH2D* mnvh2D_Muon    = (PlotUtils::MnvH2D*)h2D_input_mc_Muon    -> Clone("mnvh2D_Muon");
    PlotUtils::MnvH2D* mnvh2D_OthPdg  = (PlotUtils::MnvH2D*)h2D_input_mc_OthPdg  -> Clone("mnvh2D_OthPdg");
    PlotUtils::MnvH2D* mnvh2D_MCXtalk = (PlotUtils::MnvH2D*)h2D_input_mc_MCXtalk -> Clone("mnvh2D_MCXtalk");
    PlotUtils::MnvH2D* mnvh2D_Overlay = (PlotUtils::MnvH2D*)h2D_input_mc_Overlay -> Clone("mnvh2D_Overlay");
    
    PlotUtils::MnvH2D* mnvh2D_Pi0EM = (PlotUtils::MnvH2D*)mnvh2D_Pi0->Clone("mnvh2D_Pi0EM");
    mnvh2D_Pi0EM -> Add(mnvh2D_EM);
    
    // Define total MC histogram
    PlotUtils::MnvH2D* mnvh2D_mc = (PlotUtils::MnvH2D*)mnvh2D_Pi0EM->Clone("mnvh2D_mc");
    mnvh2D_mc -> Add(mnvh2D_Proton);
    mnvh2D_mc -> Add(mnvh2D_Neutron);
    mnvh2D_mc -> Add(mnvh2D_Pion);
    mnvh2D_mc -> Add(mnvh2D_Muon);
    mnvh2D_mc -> Add(mnvh2D_OthPdg);
    mnvh2D_mc -> Add(mnvh2D_MCXtalk);
    mnvh2D_mc -> Add(mnvh2D_Overlay);
    
    // Define PDG ratio histograms
    PlotUtils::MnvH2D* mnvh2D_Pi0EMRatio   = (PlotUtils::MnvH2D*)mnvh2D_Pi0EM   -> Clone("mnvh2D_Pi0EMRatio");
    PlotUtils::MnvH2D* mnvh2D_ProtonRatio  = (PlotUtils::MnvH2D*)mnvh2D_Proton  -> Clone("mnvh2D_ProtonRatio");
    PlotUtils::MnvH2D* mnvh2D_NeutronRatio = (PlotUtils::MnvH2D*)mnvh2D_Neutron -> Clone("mnvh2D_NeutronRatio");
    PlotUtils::MnvH2D* mnvh2D_PionRatio    = (PlotUtils::MnvH2D*)mnvh2D_Pion    -> Clone("mnvh2D_PionRatio");
    PlotUtils::MnvH2D* mnvh2D_MuonRatio    = (PlotUtils::MnvH2D*)mnvh2D_Muon    -> Clone("mnvh2D_MuonRatio");
    PlotUtils::MnvH2D* mnvh2D_OthPdgRatio  = (PlotUtils::MnvH2D*)mnvh2D_OthPdg  -> Clone("mnvh2D_OthPdgRatio");
    PlotUtils::MnvH2D* mnvh2D_OverlayRatio = (PlotUtils::MnvH2D*)mnvh2D_Overlay -> Clone("mnvh2D_OverlayRatio");
    
    mnvh2D_Pi0EMRatio   -> Reset();
    mnvh2D_ProtonRatio  -> Reset();
    mnvh2D_NeutronRatio -> Reset();
    mnvh2D_PionRatio    -> Reset();
    mnvh2D_MuonRatio    -> Reset();
    mnvh2D_OthPdgRatio  -> Reset();
    mnvh2D_OverlayRatio -> Reset();
    
    for ( int binx = 1; binx <= mnvh2D_mc->GetNbinsX(); ++binx ) {
        for ( int biny = 1; biny <= mnvh2D_mc->GetNbinsY(); ++biny ) {
            double mc      = mnvh2D_mc->GetBinContent(binx, biny);
            double pi0em   = mnvh2D_Pi0EM->GetBinContent(binx, biny);
            double proton  = mnvh2D_Proton->GetBinContent(binx, biny);
            double neutron = mnvh2D_Neutron->GetBinContent(binx, biny);
            double pion    = mnvh2D_Pion->GetBinContent(binx, biny);
            double muon    = mnvh2D_Muon->GetBinContent(binx, biny);
            double othpdg  = mnvh2D_OthPdg->GetBinContent(binx, biny);
            double mcxtalk = mnvh2D_MCXtalk->GetBinContent(binx, biny);
            double overlay = mnvh2D_Overlay->GetBinContent(binx, biny);
            
            if ( mc > 0.0 ) {
                mnvh2D_Pi0EMRatio   -> SetBinContent(binx, biny, pi0em/mc);
                mnvh2D_ProtonRatio  -> SetBinContent(binx, biny, proton/mc);
                mnvh2D_NeutronRatio -> SetBinContent(binx, biny, neutron/mc);
                mnvh2D_PionRatio    -> SetBinContent(binx, biny, pion/mc);
                mnvh2D_MuonRatio    -> SetBinContent(binx, biny, muon/mc);
                mnvh2D_OthPdgRatio  -> SetBinContent(binx, biny, othpdg/mc);
                mnvh2D_OverlayRatio -> SetBinContent(binx, biny, overlay/mc);
            }
            else {
                mnvh2D_Pi0EMRatio   -> SetBinContent(binx, biny, 0.0);
                mnvh2D_ProtonRatio  -> SetBinContent(binx, biny, 0.0);
                mnvh2D_NeutronRatio -> SetBinContent(binx, biny, 0.0);
                mnvh2D_PionRatio    -> SetBinContent(binx, biny, 0.0);
                mnvh2D_MuonRatio    -> SetBinContent(binx, biny, 0.0);
                mnvh2D_OthPdgRatio  -> SetBinContent(binx, biny, 0.0);
                mnvh2D_OverlayRatio -> SetBinContent(binx, biny, 0.0);
            }
        }
    }
    
    // Bin width normalize PDG histograms
    mnvh2D_Pi0EM   -> Scale(mnvh2D_Pi0->GetNormBinWidthX()     * mnvh2D_Pi0->GetNormBinWidthY(),     "width");
    mnvh2D_Proton  -> Scale(mnvh2D_Proton->GetNormBinWidthX()  * mnvh2D_Proton->GetNormBinWidthY(),  "width");
    mnvh2D_Neutron -> Scale(mnvh2D_Neutron->GetNormBinWidthX() * mnvh2D_Neutron->GetNormBinWidthY(), "width");
    mnvh2D_Pion    -> Scale(mnvh2D_Pion->GetNormBinWidthX()    * mnvh2D_Pion->GetNormBinWidthY(),    "width");
    mnvh2D_Muon    -> Scale(mnvh2D_Muon->GetNormBinWidthX()    * mnvh2D_Muon->GetNormBinWidthY(),    "width");
    mnvh2D_OthPdg  -> Scale(mnvh2D_OthPdg->GetNormBinWidthX()  * mnvh2D_OthPdg->GetNormBinWidthY(),  "width");
    mnvh2D_MCXtalk -> Scale(mnvh2D_MCXtalk->GetNormBinWidthX() * mnvh2D_MCXtalk->GetNormBinWidthY(), "width");
    mnvh2D_Overlay -> Scale(mnvh2D_Overlay->GetNormBinWidthX() * mnvh2D_Overlay->GetNormBinWidthY(), "width");
    
    
    // Draw
    // ====
    
    mnvh2D_Pi0EMRatio   -> SetMinimum(0.0);
    mnvh2D_ProtonRatio  -> SetMinimum(0.0);
    mnvh2D_NeutronRatio -> SetMinimum(0.0);
    mnvh2D_PionRatio    -> SetMinimum(0.0);
    mnvh2D_MuonRatio    -> SetMinimum(0.0);
    mnvh2D_OthPdgRatio  -> SetMinimum(0.0);
    mnvh2D_OverlayRatio -> SetMinimum(0.0);
    
    mnvh2D_Pi0EMRatio   -> SetMaximum(1.0);
    mnvh2D_ProtonRatio  -> SetMaximum(1.0);
    mnvh2D_NeutronRatio -> SetMaximum(1.0);
    mnvh2D_PionRatio    -> SetMaximum(1.0);
    mnvh2D_MuonRatio    -> SetMaximum(1.0);
    mnvh2D_OthPdgRatio  -> SetMaximum(1.0);
    mnvh2D_OverlayRatio -> SetMaximum(1.0);
    
    // Pi0 + EM
    Plot2D(plot_info, mnvh2D_Pi0EM, output_str + "_Pi0_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "#pi^{0} + EM showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_Pi0EMRatio, output_str + "_Pi0Ratio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "#pi^{0} + EM shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    // Proton
    Plot2D(plot_info, mnvh2D_Proton, output_str + "_Proton_" + option_material, title_str,
          functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
          xlabel_str, ylabel_str, "Proton showers / bin",
          false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_ProtonRatio, output_str + "_ProtonRatio_" + option_material, title_str,
          functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
          xlabel_str, ylabel_str, "Proton shower purity / bin",
          false, use_log_z, use_corr_palette, palette_input);
    
    // Neutron
    Plot2D(plot_info, mnvh2D_Neutron, output_str + "_Neutron_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Neutron showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_NeutronRatio, output_str + "_NeutronRatio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Neutron shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    // Pion
    Plot2D(plot_info, mnvh2D_Pion, output_str + "_Pion_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "#pi^{#pm} showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_PionRatio, output_str + "_PionRatio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "#pi^{#pm} shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    // Muon
    Plot2D(plot_info, mnvh2D_Muon, output_str + "_Muon_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Muon showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_MuonRatio, output_str + "_MuonRatio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Muon shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    // Other PDG
    Plot2D(plot_info, mnvh2D_OthPdg, output_str + "_OthPdg_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Other PDG showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_OthPdgRatio, output_str + "_OthPdgRatio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Other PDG shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    // Overlay
    Plot2D(plot_info, mnvh2D_Overlay, output_str + "_Overlay_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Overlay showers / bin",
           false, true, use_corr_palette, palette_input);
    
    Plot2D(plot_info, mnvh2D_OverlayRatio, output_str + "_OverlayRatio_" + option_material, title_str,
           functions, x1_functions, x2_functions, x_vertlines, y1_vertlines, y2_vertlines,
           xlabel_str, ylabel_str, "Overlay shower purity / bin",
           false, use_log_z, use_corr_palette, palette_input);
    
    
    // Other details
    // =============
    
    // Release
    delete mnvh2D_Pi0;
    delete mnvh2D_Proton;
    delete mnvh2D_Neutron;
    delete mnvh2D_Pion;
    delete mnvh2D_EM;
    delete mnvh2D_Muon;
    delete mnvh2D_OthPdg;
    delete mnvh2D_MCXtalk;
    delete mnvh2D_Overlay;
    delete mnvh2D_Pi0EM;
    delete mnvh2D_Pi0EMRatio;
    delete mnvh2D_ProtonRatio;
    delete mnvh2D_NeutronRatio;
    delete mnvh2D_PionRatio;
    delete mnvh2D_MuonRatio;
    delete mnvh2D_OthPdgRatio;
    delete mnvh2D_OverlayRatio;
}





// ================================================================================================================================================================
//  PLOT PDG 2D HISTOGRAMS IN THE SAME BOX FORMAT
// ================================================================================================================================================================

void Plot2DMultiBox_Pdg(PlotInfo plot_info,
                        PlotUtils::MnvH2D* h2D_input_mc_Pi0,
                        PlotUtils::MnvH2D* h2D_input_mc_Proton,
                        PlotUtils::MnvH2D* h2D_input_mc_Neutron,
                        PlotUtils::MnvH2D* h2D_input_mc_Pion,
                        PlotUtils::MnvH2D* h2D_input_mc_EM,
                        PlotUtils::MnvH2D* h2D_input_mc_Muon,
                        PlotUtils::MnvH2D* h2D_input_mc_OthPdg,
                        PlotUtils::MnvH2D* h2D_input_mc_MCXtalk,
                        PlotUtils::MnvH2D* h2D_input_mc_Overlay,
                        std::string output_str,
                        std::string title_str,
                        std::string xlabel_str = "",
                        std::string ylabel_str = "",
                        std::string zlabel_str = "",
                        bool bin_width_norm    = true)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get histograms
    // ==============
    
    PlotUtils::MnvH2D* mnvh2D_Pi0     = (PlotUtils::MnvH2D*)h2D_input_mc_Pi0     -> Clone("mnvh2D_Pi0");
    PlotUtils::MnvH2D* mnvh2D_Proton  = (PlotUtils::MnvH2D*)h2D_input_mc_Proton  -> Clone("mnvh2D_Proton");
    PlotUtils::MnvH2D* mnvh2D_Neutron = (PlotUtils::MnvH2D*)h2D_input_mc_Neutron -> Clone("mnvh2D_Neutron");
    PlotUtils::MnvH2D* mnvh2D_Pion    = (PlotUtils::MnvH2D*)h2D_input_mc_Pion    -> Clone("mnvh2D_Pion");
    PlotUtils::MnvH2D* mnvh2D_EM      = (PlotUtils::MnvH2D*)h2D_input_mc_EM      -> Clone("mnvh2D_EM");
    PlotUtils::MnvH2D* mnvh2D_Muon    = (PlotUtils::MnvH2D*)h2D_input_mc_Muon    -> Clone("mnvh2D_Muon");
    PlotUtils::MnvH2D* mnvh2D_OthPdg  = (PlotUtils::MnvH2D*)h2D_input_mc_OthPdg  -> Clone("mnvh2D_OthPdg");
    PlotUtils::MnvH2D* mnvh2D_MCXtalk = (PlotUtils::MnvH2D*)h2D_input_mc_MCXtalk -> Clone("mnvh2D_MCXtalk");
    PlotUtils::MnvH2D* mnvh2D_Overlay = (PlotUtils::MnvH2D*)h2D_input_mc_Overlay -> Clone("mnvh2D_Overlay");
    
    PlotUtils::MnvH2D* mnvh2D_Pi0EM = (PlotUtils::MnvH2D*)mnvh2D_Pi0->Clone("mnvh2D_Pi0EM");
    mnvh2D_Pi0EM -> Add(mnvh2D_EM);
    
    if ( bin_width_norm ) {
        mnvh2D_Pi0EM   -> Scale(mnvh2D_Pi0->GetNormBinWidthX()     * mnvh2D_Pi0->GetNormBinWidthY(),     "width");
        mnvh2D_Proton  -> Scale(mnvh2D_Proton->GetNormBinWidthX()  * mnvh2D_Proton->GetNormBinWidthY(),  "width");
        mnvh2D_Neutron -> Scale(mnvh2D_Neutron->GetNormBinWidthX() * mnvh2D_Neutron->GetNormBinWidthY(), "width");
        mnvh2D_Pion    -> Scale(mnvh2D_Pion->GetNormBinWidthX()    * mnvh2D_Pion->GetNormBinWidthY(),    "width");
        mnvh2D_Muon    -> Scale(mnvh2D_Muon->GetNormBinWidthX()    * mnvh2D_Muon->GetNormBinWidthY(),    "width");
        mnvh2D_OthPdg  -> Scale(mnvh2D_OthPdg->GetNormBinWidthX()  * mnvh2D_OthPdg->GetNormBinWidthY(),  "width");
        mnvh2D_MCXtalk -> Scale(mnvh2D_MCXtalk->GetNormBinWidthX() * mnvh2D_MCXtalk->GetNormBinWidthY(), "width");
        mnvh2D_Overlay -> Scale(mnvh2D_Overlay->GetNormBinWidthX() * mnvh2D_Overlay->GetNormBinWidthY(), "width");
    }
    
    // Axis labels
    plot_info.SetXLabel(mnvh2D_Muon, xlabel_str);
    plot_info.SetYLabel(mnvh2D_Muon, ylabel_str);
    plot_info.SetZLabel(mnvh2D_Muon, zlabel_str);
    
    
    // Draw
    // ====
    
    plot_info.Set2DHistoStyle(&canvas, mnvh2D_Overlay);
    canvas.SetRightMargin(0.10);
    
    mnvh2D_Pi0EM   -> SetLineWidth(2);
    mnvh2D_Proton  -> SetLineWidth(2);
    mnvh2D_Neutron -> SetLineWidth(2);
    mnvh2D_Pion    -> SetLineWidth(2);
    mnvh2D_Muon    -> SetLineWidth(2);
    mnvh2D_OthPdg  -> SetLineWidth(2);
    mnvh2D_MCXtalk -> SetLineWidth(2);
    mnvh2D_Overlay -> SetLineWidth(2);
    
    mnvh2D_Pi0EM   -> SetLineColor(kAzure-1);
    mnvh2D_Proton  -> SetLineColor(kCyan+2);
    mnvh2D_Neutron -> SetLineColor(kViolet-4);
    mnvh2D_Pion    -> SetLineColor(kRed+3);
    mnvh2D_Muon    -> SetLineColor(kYellow+1);
    mnvh2D_OthPdg  -> SetLineColor(kGreen+2);
    mnvh2D_MCXtalk -> SetLineColor(kGray);
    mnvh2D_Overlay -> SetLineColor(kGray+1);
    
    // mnvh2D_Overlay -> Draw("BOX");
    // mnvh2D_MCXtalk -> Draw("BOX SAME");
    // mnvh2D_OthPdg  -> Draw("BOX SAME");
    // mnvh2D_Muon    -> Draw("BOX SAME");
    // mnvh2D_Pion    -> Draw("BOX SAME");
    // mnvh2D_Neutron -> Draw("BOX SAME");
    // mnvh2D_Proton  -> Draw("BOX SAME");
    mnvh2D_Pi0EM   -> Draw("BOX");
    
    
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
    delete mnvh2D_Pi0;
    delete mnvh2D_Proton;
    delete mnvh2D_Neutron;
    delete mnvh2D_Pion;
    delete mnvh2D_EM;
    delete mnvh2D_Muon;
    delete mnvh2D_OthPdg;
    delete mnvh2D_MCXtalk;
    delete mnvh2D_Overlay;
    delete mnvh2D_Pi0EM;
}





// ================================================================================================================================================================
//  PLOT UNFOLDING STATISTICAL STUDIES TGRAPHS
// ================================================================================================================================================================

void PlotUnfoldStatStudyGraphs(PlotInfo plot_info,
                               TGraph* input_StatFactor10,
                               TGraph* input_StatFactor11,
                               TGraph* input_StatFactor11_1345,
                               TGraph* input_StatFactor12,
                               TGraph* input_StatFactor13,
                               std::string output_str,
                               std::string title_str,
                               std::string xlabel_str = "",
                               std::string ylabel_str = "",
                               double ndf             = 13.0)
{
    // Define canvas
    TCanvas canvas("c1", "c1");
    
    
    // Get graphs
    // ==========
    
    TGraph* gr_StatFactor10      = (TGraph*)input_StatFactor10->Clone("");
    TGraph* gr_StatFactor11      = (TGraph*)input_StatFactor11->Clone("");
    TGraph* gr_StatFactor11_1345 = (TGraph*)input_StatFactor11_1345->Clone("");
    TGraph* gr_StatFactor12      = (TGraph*)input_StatFactor12->Clone("");
    TGraph* gr_StatFactor13      = (TGraph*)input_StatFactor13->Clone("");
    
    
    // Set graphs
    // ==========
    
    gr_StatFactor10 -> SetMarkerStyle(23);
    gr_StatFactor10 -> SetMarkerColor(kRed+3);
    gr_StatFactor10 -> SetLineWidth(3);
    gr_StatFactor10 -> SetLineColor(kRed+3);
    
    gr_StatFactor11 -> SetMarkerStyle(33);
    gr_StatFactor11 -> SetMarkerColor(kCyan+1);
    gr_StatFactor11 -> SetLineWidth(3);
    gr_StatFactor11 -> SetLineColor(kCyan+1);
    
    gr_StatFactor11_1345 -> SetMarkerStyle(20);
    gr_StatFactor11_1345 -> SetMarkerColor(kRed+1);
    gr_StatFactor11_1345 -> SetLineWidth(3);
    gr_StatFactor11_1345 -> SetLineColor(kRed+1);
    
    gr_StatFactor12 -> SetMarkerStyle(34);
    gr_StatFactor12 -> SetMarkerColor(kOrange+1);
    gr_StatFactor12 -> SetLineWidth(3);
    gr_StatFactor12 -> SetLineColor(kOrange+1);
    
    gr_StatFactor13 -> SetMarkerStyle(45);
    gr_StatFactor13 -> SetMarkerColor(kMagenta);
    gr_StatFactor13 -> SetLineWidth(3);
    gr_StatFactor13 -> SetLineColor(kMagenta);
    
    double Ymax_StatFactor10      = TMath::MaxElement(10, gr_StatFactor10->GetY());
    double Ymax_StatFactor11      = TMath::MaxElement(10, gr_StatFactor11->GetY());
    double Ymax_StatFactor11_1345 = TMath::MaxElement(10, gr_StatFactor11_1345->GetY());
    double Ymax_StatFactor12      = TMath::MaxElement(10, gr_StatFactor12->GetY());
    double Ymax_StatFactor13      = TMath::MaxElement(10, gr_StatFactor13->GetY());
    
    std::vector<double> Ymax_vector;
    Ymax_vector.push_back(Ymax_StatFactor10);
    Ymax_vector.push_back(Ymax_StatFactor11);
    Ymax_vector.push_back(Ymax_StatFactor11_1345);
    Ymax_vector.push_back(Ymax_StatFactor12);
    Ymax_vector.push_back(Ymax_StatFactor13);
    double Ymax = *std::max_element(Ymax_vector.begin(), Ymax_vector.end());
    
    
    // Axis labels and Y-axis limits
    // =============================
    
    // Axis labels
    gr_StatFactor10 ->GetXaxis() -> SetTitle(xlabel_str.c_str());
    gr_StatFactor10 ->GetXaxis() -> CenterTitle();
    gr_StatFactor10 ->GetYaxis() -> SetTitle(ylabel_str.c_str());
    
    // Y-axis limits
    gr_StatFactor10 -> SetMinimum(0.0);
    gr_StatFactor10 -> SetMaximum(1.5*Ymax);
    if ( Ymax > 50.0 ) gr_StatFactor10 -> SetMaximum(50.0);
    
    
    // Draw
    // ====
    
    // Graphs
    gr_StatFactor10      -> Draw("ALP");
    gr_StatFactor11      -> Draw("SAME LP");
    gr_StatFactor12      -> Draw("SAME LP");
    gr_StatFactor13      -> Draw("SAME LP");
    gr_StatFactor11_1345 -> Draw("SAME LP");
    
    // Line at Y = ndf
    const TAxis* axis = gr_StatFactor10->GetXaxis();
    double lowX  = axis->GetBinLowEdge(axis->GetFirst());
    double highX = axis->GetBinUpEdge(axis->GetLast());
    
    TLine line;
    line.SetLineStyle(2);
    line.SetLineWidth(3);
    line.SetLineColor(36);
    line.DrawLine(lowX, ndf, highX, ndf);
    
    
    // Add legend
    // ==========
    
    auto legend = new TLegend(0.14, 0.77, 0.92, 0.89);
    legend -> SetNColumns(3);
    legend -> SetTextSize(0.033);
    legend -> AddEntry(gr_StatFactor10,      "Stat. factor: 10",      "lep");
    legend -> AddEntry(gr_StatFactor11,      "Stat. factor: 11",      "lep");
    legend -> AddEntry(gr_StatFactor11_1345, "Stat. factor: 11.1345", "lep");
    legend -> AddEntry(gr_StatFactor12,      "Stat. factor: 12",      "lep");
    legend -> AddEntry(gr_StatFactor13,      "Stat. factor: 13",      "lep");
    legend -> Draw("SAMES");
    
    
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
    delete gr_StatFactor10;
    delete gr_StatFactor11;
    delete gr_StatFactor12;
    delete gr_StatFactor13;
    delete gr_StatFactor11_1345;
}


#endif  // plotting_functions_h