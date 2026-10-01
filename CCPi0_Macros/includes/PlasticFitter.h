#ifndef PlasticFitter_h
#define PlasticFitter_h

#include <string>

#include "Constants.h"
#include "CVUniverse.h"
#include "Histograms.h"



class PlasticFitter
{
    public:
        
        // Constructors and destructor
        // ===========================
        
        PlasticFitter(const CVUniverse& universe,
                      Histograms hists,
                      const EnumModels& type_model,
                      std::string phys_region,
                      std::string plas_sideband);
        
        ~PlasticFitter();
        
        
        
        // Data members
        // ============
        
        std::string m_phys_region;
        std::string m_plas_sideband;
        
        const EnumModels& m_model_type;
        
        static double m_function_pivotX;
        
        double m_Xmin;
        double m_Xmax;
        
        static TH1D* m_data;
        static TH1D* m_mc_Tgt4Pb;
        static TH1D* m_mc_Tgt5Pb;
        static TH1D* m_mc_Tgt5Fe;
        static TH1D* m_mc_DominPlas;
        static TH1D* m_mc_OtherPlas1;
        static TH1D* m_mc_OtherPlas2;
        static TH1D* m_mc_OtherMat;
        
        TF1* m_function;
        
        
        
        // Data functions
        // ==============
        
        // Initializer
        void Initialize(const CVUniverse& universe,
                        Histograms hist);
        
        // Minimizer functions
        static double Chi2Function(const double* scale_vector);
        
        // Fit function
        void Fit();
        
};


#endif  // PlasticFitter_h