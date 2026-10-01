#ifndef Systematics_h
#define Systematics_h

#include "Constants.h"  // typedef UniverseMap
#include "CVUniverse.h"

#include "PlotUtils/ChainWrapper.h"

// Flux systematics
#include "PlotUtils/FluxSystematics.h"

// GENIE systematics
#include "PlotUtils/GenieSystematics.h"

// MINERvA tunes systematics
#include "PlotUtils/MnvTuneSystematics.h"

// Muon systematics
#include "PlotUtils/MuonSystematics.h"
#include "PlotUtils/MuonResolutionSystematics.h"
#include "PlotUtils/AngleSystematics.h"
#include "PlotUtils/MinosEfficiencySystematics.h"

// Particle response systematics
#include "PlotUtils/ResponseSystematics.h"

// Other systematics
#include "PlotUtils/GeantHadronSystematics.h"
#include "PlotUtils/TargetMassSystematics.h"
#include "PlotUtils/MichelSystematics.h"



// =============================================================================
// Get container of systematics.
// Reminder: Universes own a chain.
// This is so one can get values within a universe.
// If you're not accessing a universe's values, the chain isn't needed.
// This is the case when I load an MnvH1D from a file into a HistWrapper.
// You can instead pass a dummy chain.
// You don't need a real CHW to make the universes and use them with a HW.
// =============================================================================

namespace systematics
{
    // ============================================================================
    //  Get MAT systematics from PlotUtils
    // ============================================================================
    
    UniverseMap GetSystematicUniversesMap(PlotUtils::ChainWrapper* chain,
                                          const EnumModels& type_model,
                                          bool is_truth = false,
                                          bool do_systematics = true)
    {
        // Map with all error bands
        UniverseMap error_bands;
        
        
        // Add central value as first error band
        error_bands[std::string("cv")].push_back(new CVUniverse(chain));
        
        
        if ( do_systematics )
        {
            // Flux
            // ====
            
            UniverseMap bands_flux = PlotUtils::GetFluxSystematicsMap<CVUniverse>(chain, CCPi0AnaConstants::kNFluxUniverses);
            error_bands.insert(bands_flux.begin(), bands_flux.end());
            
            
            // GENIE
            // =====
            
            // "Standard" GENIE
            UniverseMap bands_genie = PlotUtils::GetStandardGenieSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_genie.begin(), bands_genie.end());
            
            // Axial mass in Llewellyn-Smith cross section
            // -> 2nd argument: Number of universes ('-1' means using standard +/-1 sigma shifts, otherwise it's the Z-expansion)
            UniverseMap bands_genie_maccqe = PlotUtils::GetGenieFaCCQESystematicsMap<CVUniverse>(chain, -1);
            error_bands.insert(bands_genie_maccqe.begin(), bands_genie_maccqe.end());
            
            // Modified MvRES error from electroproduction data
            UniverseMap bands_genie_mvres = PlotUtils::GetGenieEPMvResSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_genie_mvres.begin(), bands_genie_mvres.end());
            
            // Modified MaRES and NormCCRES from deuterium fit
            UniverseMap bands_genie_d2respi = PlotUtils::GetGenieResPionFitSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_genie_d2respi.begin(), bands_genie_d2respi.end());
            
            // Non-resonant single-pion production cross-section
            UniverseMap bands_genie_nonres1pi = PlotUtils::GetGenieRvx1piSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_genie_nonres1pi.begin(), bands_genie_nonres1pi.end());
            
            
            // MINERvA tunes
            // =============
            
            // Low-recoil 2p2h
            UniverseMap bands_mnvtune_2p2h = PlotUtils::Get2p2hSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_mnvtune_2p2h.begin(), bands_mnvtune_2p2h.end());
            
            // Low- and high-Q2 RPA
            UniverseMap bands_mnvtune_rpa = PlotUtils::GetRPASystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_mnvtune_rpa.begin(), bands_mnvtune_rpa.end());
            
            // Pion production low-Q2 supression
            UniverseMap bands_mnvtune_lowQ2pi = PlotUtils::GetLowQ2PiSystematicsMap<CVUniverse>(chain);
            error_bands.insert(bands_mnvtune_lowQ2pi.begin(), bands_mnvtune_lowQ2pi.end());
            
            
            if ( !is_truth )
            {
                // Muon reconstruction
                // ===================
                
                // MINERvA muon energy
                UniverseMap bands_muon_minerva = PlotUtils::GetMinervaMuonSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_muon_minerva.begin(), bands_muon_minerva.end());
                
                // MINOS muon energy
                UniverseMap bands_muon_minos = PlotUtils::GetMinosMuonSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_muon_minos.begin(), bands_muon_minos.end());
                
                // Muon energy resolution
                UniverseMap bands_muon_energy_resol = PlotUtils::GetMuonResolutionSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_muon_energy_resol.begin(), bands_muon_energy_resol.end());
                
                // Muon angle resolution
                UniverseMap bands_muon_angle_resol = PlotUtils::GetMuonAngleResolutionSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_muon_angle_resol.begin(), bands_muon_angle_resol.end());
                
                // Beam angle
                UniverseMap bands_beam_angle = PlotUtils::GetAngleSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_beam_angle.begin(), bands_beam_angle.end());
                
                // MINOS efficiency
                UniverseMap bands_muon_minos_eff = PlotUtils::GetMinosEfficiencySystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_muon_minos_eff.begin(), bands_muon_minos_eff.end());
                
                
                // Particle response
                // =================
                
                // -> 2nd argument: NEUTRON
                // -> 3rd argument: use_new_part_response
                // -> 4th argument: PROTON
                UniverseMap bands_part_response = PlotUtils::GetResponseSystematicsMap<CVUniverse>(chain, true, true, true);
                error_bands.insert(bands_part_response.begin(), bands_part_response.end());
                
                
                // Other
                // =====
                
                // GEANT hadrons with MnvHadronReweight
                UniverseMap bands_geant_hadron = PlotUtils::GetGeantHadronSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_geant_hadron.begin(), bands_geant_hadron.end());
                
                // Target mass
                UniverseMap bands_target_mass = PlotUtils::GetTargetMassSystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_target_mass.begin(), bands_target_mass.end());
                
                // Michel tag efficiency
                UniverseMap bands_michel_eff = PlotUtils::GetMichelEfficiencySystematicsMap<CVUniverse>(chain);
                error_bands.insert(bands_michel_eff.begin(), bands_michel_eff.end());
            }
        }
        
        
        
        // ========================================================================
        //  Return error bands with systematics
        // ========================================================================
        
        for ( auto band : error_bands ) {
            std::vector<CVUniverse*> universes = band.second;
            
            for ( auto universe : universes )
                universe -> SetTruth(is_truth);
        }
        
        return error_bands;
    }
}


#endif  // Systematics_h