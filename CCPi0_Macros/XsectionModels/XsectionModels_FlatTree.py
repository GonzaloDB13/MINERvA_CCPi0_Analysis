import ROOT
import array, sys, os


## ===========================================
##  Define histograms
## ===========================================

bins_MuonPt = [0., 0.075, 0.15, 0.25, 0.325, 0.4, 0.475, 0.55, 0.7, 0.85, 1.0, 1.25, 1.5, 2.5]

h_mc_MuonPt         = ROOT.TH1D("mc_CrossSection_MuonPt",         "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_QE      = ROOT.TH1D("mc_CrossSection_MuonPt_QE",      "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_MEC     = ROOT.TH1D("mc_CrossSection_MuonPt_MEC",     "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_RES     = ROOT.TH1D("mc_CrossSection_MuonPt_RES",     "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_SoftDIS = ROOT.TH1D("mc_CrossSection_MuonPt_SoftDIS", "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_TrueDIS = ROOT.TH1D("mc_CrossSection_MuonPt_TrueDIS", "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))
h_mc_MuonPt_Other   = ROOT.TH1D("mc_CrossSection_MuonPt_Other",   "", len(bins_MuonPt)-1, array.array("d", bins_MuonPt))



## ===========================================
##  Signal functions
## ===========================================

# Good final state?
# -----------------
def IsTrueCC1Pi0(event) :
    n_fspart   = event.nfsp
    fspart_pdg = event.pdg
    n_muon   = 0
    n_pi0    = 0
    n_pion   = 0
    n_meson  = 0
    n_baryon = 0
    
    bad_mesons = [130, 310, 311, 321, 313, 323, 411, 421]
    bad_baryons = [3122, 3222, 3212, 3112, 4122, 4222, 4212, 4112]
    
    for p in range(0, n_fspart) :
        pdg = fspart_pdg[p]
        
        if ( pdg == 13 ) :         n_muon += 1
        elif ( abs(pdg) == 111 ) : n_pi0  += 1
        elif ( abs(pdg) == 211 ) : n_pion += 1
        elif ( abs(pdg) in bad_mesons ) :  n_meson  += 1
        elif ( abs(pdg) in bad_baryons ) : n_baryon += 1
    
    if ( n_muon == 1 and n_pi0 == 1 and n_pion == 0 and n_meson == 0 and n_baryon == 0 ) :
        return True
    else :
        return False


# Is signal?
# ----------
def IsSignal(event, material) :
    mu_costheta = event.CosLep
    target_Z    = event.tgtz
    good_fs     = IsTrueCC1Pi0(event)
    
    if ( material == "lead" )   : material_Z = 82
    elif ( material == "iron" ) : material_Z = 26
    
    if ( mu_costheta >= 0.95630475596 and target_Z == material_Z and good_fs ) :
        return True
    else :
        return False


# Is ROOTino? (copied from Dan - need to know what does this do)
# -----------
def IsRootino(event) :
    n_fspart   = event.nfsp
    fspart_pdg = event.pdg
    
    is_rootino = True
    for p in range(0, n_fspart) :
        if ( fspart_pdg[p] != 0 and fspart_pdg[p] != 13 ) : is_rootino = False
    
    return is_rootino



## ===========================================
##  Interaction type
## ===========================================

# Return interaction type
# -----------------------
# -> 1 : CCQE
# -> 2 : MEC
# -> 3 : RES
# -> 4 : Soft DIS
# -> 5 : True DIS
# -> 6 : Other
def IntType(event) :
    mode = event.Mode
        
    if ( mode == 1 ) :
        return 1  # QE
        
    if ( mode == 2 ) :
        return 2  # MEC
        
    elif ( mode == 11 or mode == 12 or mode == 13 ) :
        return 3  # RES
        
    elif ( mode == 18 or mode == 19 or mode == 20 or mode == 21 or mode == 26 ) :
        nu_E    = event.Enu_true  # [GeV]
        mu_E    = event.ELep      # [GeV]
        mu_mass = 0.1056583       # [GeV]
        mu_P    = ROOT.TMath.Sqrt((mu_E * mu_E) - (mu_mass * mu_mass))  # [GeV]
        mu_costheta = event.CosLep
        
        prot_mass = 0.938272013  # [GeV]
        neut_mass = 0.93956536   # [GeV]
        nucl_mass = (prot_mass + neut_mass)/2  # [GeV]
        rec_E     = nu_E - mu_E  # [GeV]
        
        Q2 = 2.0 * nu_E * (mu_E - (mu_P * mu_costheta)) - (mu_mass * mu_mass)  # [GeV^2]
        W2 = (nucl_mass * nucl_mass) + (2.0 * nucl_mass * rec_E) - Q2          # [GeV^2]
        
        if ( Q2 > 1 and W2 > 4 ) :
            return 5  # True DIS
        else :
            return 4  # Soft DIS
    
    else :
        return 6  # Other
        


## ===========================================
##  Main function
## ===========================================

# Ask inputs
# ----------
date = raw_input("\tDate: ")

base_model = raw_input("\tBase model: ")
if ( base_model != "v1" ) :
    print ("\tENTER CORRECT BASE MODEL 'v1'!!!\n")
    quit()

generator = raw_input("\tGenerator: ")
if ( generator != "genie3" and generator != "neut" ) :
    print ("\tENTER CORRECT BASE MODEL: EITHER 'genie3' or 'neut'!!!\n")
    quit()

xsec_model = raw_input("\tX-section model: ")
if ( generator == "genie3" ) :
    if ( xsec_model != "02a" and xsec_model != "02b" and xsec_model != "10a" and xsec_model != "10b" ) :
        print ("\tENTER CORRECT X-SECTION MODEL: EITHER '02a', '02b', '10a', or '10b'!!!\n")
        quit()
elif ( generator == "neut" ) :
    if ( xsec_model != "SF" and xsec_model != "LFG" ) :
        print ("\tENTER CORRECT X-SECTION MODEL: EITHER 'SF' or 'LFG'!!!\n")
        quit()

material = raw_input("\tMaterial: ")
if ( material != "lead" and material != "iron" ) :
    print ("\tENTER CORRECT MATERIAL: EITHER 'lead' or 'iron'!!!\n")
    quit()


# Input file
# ----------
input_genie3   = "/pnfs/minerva/persistent/Models/GENIE/Medium_Energy/FHC/v3_0_6/nuclear/G18_" + xsec_model + "_02_11a/" + material + "/flat_GENIE_G18_" + xsec_model + "_02_11a_50M.root"
input_neut_lfg = "/pnfs/minerva/persistent/Models/NEUT/Medium_Energy/FHC/v5.0.2/nuclear/LFG_ma105/" + material + "/flat_NEUT_tune_LFG_maqe1.05_50M.root"
input_neut_sf  = "/pnfs/minerva/persistent/Models/NEUT/Medium_Energy/FHC/v5.0.2/nuclear/SF_ma103/" + material + "/flat_NEUT_tune_SF_maqe1.03_50M.root"

if ( generator == "genie3" ) :
    input_file = input_genie3
    
elif ( generator == "neut" ) :
    if ( xsec_model == "LFG" )  : input_file = input_neut_lfg
    elif ( xsec_model == "SF" ) : input_file = input_neut_sf

print ("\n\tInput file: " + input_file)


# FlatTree TChain
# ---------------
my_tree = ROOT.TChain("FlatTree_VARS")
my_tree.Add(input_file)

Nentries = my_tree.GetEntries()
print ("\n\tNumber of entries: " + str(Nentries) + "\n")

Nentries_fivepercent = int(Nentries * (5.0/100.0))
counter = 0


# Loop over events
# ----------------
for entry in my_tree :
    
    # Print counter every 5%
    counter += 1
    if ( counter%Nentries_fivepercent == 0 ) :
        print ("\tProgress... " + str(int(5*counter/Nentries_fivepercent)) + " percent done!" )
        # break
    
    # If not signal, reject
    if not ( IsSignal(entry, material) ) : continue
    
    # If ROOTino, reject
    if ( IsRootino(entry) ) : continue
    
    # Scale factor to weight histograms
    ScaleFactor = entry.fScaleFactor
    
    # Find muon Pt
    muon_mass     = 0.1056583  # [GeV]
    muon_costheta = entry.CosLep
    
    muon_E  = entry.ELep  # [GeV]
    muon_P  = ROOT.TMath.Sqrt((muon_E * muon_E) - (muon_mass * muon_mass))   # [GeV]
    muon_Pt = muon_P * ROOT.TMath.Sqrt(1 - (muon_costheta * muon_costheta))  # [GeV]
    
    # Fill histograms
    h_mc_MuonPt.Fill(muon_Pt, ScaleFactor)
    if ( IntType(entry) == 1 )   : h_mc_MuonPt_QE.Fill(muon_Pt, ScaleFactor)
    elif ( IntType(entry) == 2 ) : h_mc_MuonPt_MEC.Fill(muon_Pt, ScaleFactor)
    elif ( IntType(entry) == 3 ) : h_mc_MuonPt_RES.Fill(muon_Pt, ScaleFactor)
    elif ( IntType(entry) == 4 ) : h_mc_MuonPt_SoftDIS.Fill(muon_Pt, ScaleFactor)
    elif ( IntType(entry) == 5 ) : h_mc_MuonPt_TrueDIS.Fill(muon_Pt, ScaleFactor)
    elif ( IntType(entry) == 6 ) : h_mc_MuonPt_Other.Fill(muon_Pt, ScaleFactor)

print ("\nDone looping over events!")


# Output file
# -----------
output_genie3 = "/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/" + date + "_" + base_model+ "/" + material + "/XsectionModels_GENIE3_" + xsec_model + "_" + material + ".root"
output_neut   = "/pnfs/minerva/persistent/users/gonzalo/MAT/XsectionModels/" + date + "_" + base_model+ "/" + material + "/XsectionModels_NEUT_" + xsec_model + "_" + material + ".root"

if ( generator == "genie3" ) : output_str = output_genie3
elif ( generator == "neut" ) : output_str = output_neut

output_file = ROOT.TFile(output_str, "RECREATE")
print ("\n\tOutput file: " + output_str + "\n")

h_mc_MuonPt.Write("mc_CrossSection_MuonPt")
h_mc_MuonPt_QE.Write("mc_CrossSection_MuonPt_QE")
h_mc_MuonPt_MEC.Write("mc_CrossSection_MuonPt_MEC")
h_mc_MuonPt_RES.Write("mc_CrossSection_MuonPt_RES")
h_mc_MuonPt_SoftDIS.Write("mc_CrossSection_MuonPt_SoftDIS")
h_mc_MuonPt_TrueDIS.Write("mc_CrossSection_MuonPt_TrueDIS")
h_mc_MuonPt_Other.Write("mc_CrossSection_MuonPt_Other")

output_file.Close()
