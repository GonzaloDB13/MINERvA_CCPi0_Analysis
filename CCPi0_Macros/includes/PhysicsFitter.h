#ifndef PhysicsFitter_h
#define PhysicsFitter_h

#include <string>

#include "CVUniverse.h"
#include "Histograms.h"



class PhysicsFitter
{
    public:
        
        // Constructors and destructor
        // ===========================
        
        PhysicsFitter(const CVUniverse& universe,
                      Histograms hists,
                      const EnumModels& type_model,
                      std::string option_material,
                      std::string option_fit_function,
                      int n_iterations);
        
        ~PhysicsFitter();
        
        
        
        // Data members
        // ============
        
        std::string m_option_material;
        static std::string m_option_fit_function;
        
        const EnumModels& m_model_type;
        
        static double m_function_pivotX;
        static double m_function_switchX;
        
        int m_n_iterations;
        
        double m_Xmin;
        double m_Xmax;
        
        static TH1D* m_data_SigReg;
        static TH1D* m_mc_SigReg_Signal;
        static TH1D* m_mc_SigReg_BackgrPi0HighW;
        static TH1D* m_mc_SigReg_BackgrQElike;
        static TH1D* m_mc_SigReg_BackgrPionProd;
        static TH1D* m_mc_SigReg_BackgrPlasUp;
        static TH1D* m_mc_SigReg_BackgrPlasBetw;
        static TH1D* m_mc_SigReg_BackgrPlasDown;
        static TH1D* m_mc_SigReg_BackgrOther;
        
        static TH1D* m_data_PionBlobSB;
        static TH1D* m_mc_PionBlobSB_Signal;
        static TH1D* m_mc_PionBlobSB_BackgrPi0HighW;
        static TH1D* m_mc_PionBlobSB_BackgrQElike;
        static TH1D* m_mc_PionBlobSB_BackgrPionProd;
        static TH1D* m_mc_PionBlobSB_BackgrPlasUp;
        static TH1D* m_mc_PionBlobSB_BackgrPlasBetw;
        static TH1D* m_mc_PionBlobSB_BackgrPlasDown;
        static TH1D* m_mc_PionBlobSB_BackgrOther;
        
        static TH1D* m_data_ProtonBlobSB;
        static TH1D* m_mc_ProtonBlobSB_Signal;
        static TH1D* m_mc_ProtonBlobSB_BackgrPi0HighW;
        static TH1D* m_mc_ProtonBlobSB_BackgrQElike;
        static TH1D* m_mc_ProtonBlobSB_BackgrPionProd;
        static TH1D* m_mc_ProtonBlobSB_BackgrPlasUp;
        static TH1D* m_mc_ProtonBlobSB_BackgrPlasBetw;
        static TH1D* m_mc_ProtonBlobSB_BackgrPlasDown;
        static TH1D* m_mc_ProtonBlobSB_BackgrOther;
        
        static TH1D* m_data_HighWSB;
        static TH1D* m_mc_HighWSB_Signal;
        static TH1D* m_mc_HighWSB_BackgrPi0HighW;
        static TH1D* m_mc_HighWSB_BackgrQElike;
        static TH1D* m_mc_HighWSB_BackgrPionProd;
        static TH1D* m_mc_HighWSB_BackgrPlasUp;
        static TH1D* m_mc_HighWSB_BackgrPlasBetw;
        static TH1D* m_mc_HighWSB_BackgrPlasDown;
        static TH1D* m_mc_HighWSB_BackgrOther;
        
        std::vector<double> m_Par_a_Signal;
        std::vector<double> m_Par_m_Signal;
        std::vector<double> m_Par_b_Signal;
        std::vector<double> m_Par_T_Signal;
        
        std::vector<double> m_Par_a_Pi0HighW;
        std::vector<double> m_Par_m_Pi0HighW;
        std::vector<double> m_Par_b_Pi0HighW;
        std::vector<double> m_Par_T_Pi0HighW;
        
        std::vector<double> m_Par_a_QElike;
        std::vector<double> m_Par_m_QElike;
        std::vector<double> m_Par_b_QElike;
        std::vector<double> m_Par_T_QElike;
        
        std::vector<double> m_Par_a_PionProd;
        std::vector<double> m_Par_m_PionProd;
        std::vector<double> m_Par_b_PionProd;
        std::vector<double> m_Par_T_PionProd;
        
        TF1* m_function_Signal;
        TF1* m_function_BackgrPi0HighW;
        TF1* m_function_BackgrQElike;
        TF1* m_function_BackgrPionProd;
        
        
        
        // Data functions
        // ==============
        
        // Initializer
        void Initialize(const CVUniverse& universe,
                        Histograms hist,
                        std::string option_material,
                        std::string option_fit_function);
        
        // Minimizer functions
        static double Chi2Function_Combined(const double* scale_vector);
        
        // Fit functions
        void Fit_Combined();
};


#endif  // PhysicsFitter_h