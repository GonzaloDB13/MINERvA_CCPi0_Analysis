#ifndef CCPi0MacroUtil_h
#define CCPi0MacroUtil_h

#include "Constants.h"  // EnumDataMCTruth, EnumModels
#include "PlotUtils/MacroUtil.h"



namespace CCPi0
{
    class MacroUtil : public PlotUtils::MacroUtil
    {
        public:
            
            // Data constructor
            MacroUtil(const std::string& data_file_list,
                      const std::string& plist_name);
            
            
            // MC (and Truth) constructor
            MacroUtil(const std::string& mc_file_list,
                      const std::string& plist_name,
                      const bool do_truth,
                      const bool do_systematics,
                      const EnumModels& type_model);
            
            
            // Data, MC (and Truth) constructor
            MacroUtil(const std::string& mc_file_list,
                      const std::string& data_file_list,
                      const std::string& plist_name,
                      const bool do_truth,
                      const bool do_systematics,
                      const EnumModels& type_model);
            
            
            // Data members
            bool m_do_data;
            bool m_do_mc;
            bool m_do_truth;
            bool m_do_systematics;
            
            EnumModels  m_model_type;
            CVUniverse* m_data_universe;
            UniverseMap m_error_bands;
            UniverseMap m_error_bands_truth;
            
            double m_pot_scale;
            
            #ifndef __CINT__
            void PrintMacroConfiguration(std::string macro_name = "") override;
            #endif  // __CINT__
            
            
        private:
            
            void Initialize();
            void InitializeSystematics();
    };
}   // namespace CCPi0



// SetupLoop (for looping and filling functions)
void SetupLoop(const EnumDataMCTruth& type_DataMCTruth,
               const EnumModels& type_model,
               const CCPi0::MacroUtil& util,
               bool& is_mc, bool& is_truth, Long64_t& n_entries);


#endif  // CCPi0MacroUtil_h