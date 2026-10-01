import os, tarfile, optparse, shutil, subprocess, errno, glob
import datetime as dt
import os.path



#########################################################################################################
#
#  CONSTANT AND DEFAULT ARGUMENTS
#
#########################################################################################################

# Top directory to copy into to tarball
kTOPDIR = os.getenv("TOPDIR")


# Directory where to copy tarball
kTARBALL_LOCATION = "/pnfs/{EXPERIMENT}/resilient/tarballs/".format(EXPERIMENT = os.getenv("EXPERIMENT"))


# Grid script directory
kCACHE_PNFS_AREA = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/grid_cache/".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                USER = os.getenv("USER"))
kGRID_SCRIPT = os.getenv("CCPI0MACROS") + "/grid_macro.sh"


# Macros
kMACRO_EVENT_SEL       = "EventSelection/EventSelection.C+"
kMACRO_CUT_STUDIES     = "CutStudies/CutStudies.C+"
kMACRO_SIDEBAND        = "SidebandStudies/SidebandStudies.C+"
kMACRO_EFF_OPTIMIZE    = "EfficiencyStudies/EfficiencyOptimize.C+"
kMACRO_EFF_ALTERNATIVE = "EfficiencyStudies/EfficiencyAlternative.C+"
kMACRO_FAKE_DATA       = "FakeDataModels/FakeDataModels.C+"
kMACRO_SUPPORT_STD     = "SupportStudies/SupportStudies.C+"

kALL_MACROS = [kMACRO_EVENT_SEL,
               kMACRO_CUT_STUDIES,
               kMACRO_SIDEBAND,
               kMACRO_EFF_OPTIMIZE,
               kMACRO_EFF_ALTERNATIVE,
               kMACRO_FAKE_DATA,
               kMACRO_SUPPORT_STD]


# Input anatuple directory
kANATUPLE_DIR = "/pnfs/{EXPERIMENT}/persistent/users/{USER}/MergedFiles/2022-02-01".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                           USER = os.getenv("USER"))

kANATUPLE_DIR_TEST = "/pnfs/{EXPERIMENT}/persistent/users/{USER}/MergedFiles/2022-02-01_grid_test_dont_delete".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                                      USER = os.getenv("USER"))


# Input playlists
kPLAYLISTS = ["minervame1A", "minervame1B", "minervame1C", "minervame1D", "minervame1E", "minervame1F",
              "minervame1G", "minervame1L", "minervame1M", "minervame1N", "minervame1O", "minervame1P"]

kPLAYLISTS_TEST = ["minervame1A", "minervame1B", "minervame1C"]


# Input model
kMODELS = ["v0", "v1", "v1noNonResPi", "v1noD2", "v1noPionTune",
           "v2MINOS", "v2JOINT", "v2NU1PI", "v2NUNPI", "v2NUPI0", "v2MENU1PI", "data"]

kMODEL_TEST = "v1"


# Output directory
kOUTDIR_EVENT_SEL = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/EventSelection/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                    USER = os.getenv("USER"))

kOUTDIR_CUT_STUDIES = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/CutStudies/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                  USER = os.getenv("USER"))

kOUTDIR_SIDEBAND = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/SidebandStudies/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                    USER = os.getenv("USER"))

kOUTDIR_EFF_OPTIMIZE = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/EfficiencyStudies/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                          USER = os.getenv("USER"))

kOUTDIR_EFF_ALTERNATIVE = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/EfficiencyStudies/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                             USER = os.getenv("USER"))

kOUTDIR_FAKE_DATA = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/FakeDataModels/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                    USER = os.getenv("USER"))

kOUTDIR_SUPPORT_STD = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/SupportStudies/grid_output".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                                                      USER = os.getenv("USER"))

kOUTDIR_TEST = "/pnfs/{EXPERIMENT}/scratch/users/{USER}/MAT/grid_test".format(EXPERIMENT = os.getenv("EXPERIMENT"),
                                                                              USER = os.getenv("USER"))


# Grid stuff
kMEMORY_MC   = "1000MB"
kMEMORY_DATA = "1000MB"

kMEMORY_LONG_MC   = "1500MB"
kMEMORY_LONG_DATA = "1500MB"

kLIFETIME_MC   = "medium"
kLIFETIME_DATA = "medium"

# kLIFETIME_LONG_MC   = "72h"  # Preferred for event selection
kLIFETIME_LONG_MC   = "24h"
kLIFETIME_LONG_DATA = "medium"

kGRID_OPTIONS = ("--group=minerva "
                 "--resource-provides=usage_model=DEDICATED,OPPORTUNISTIC "
                 "--role=Analysis "
                 "--OS=SL7 ")

kFILETAG = ""





#########################################################################################################
#
#  HELPER FUNCTIONS
#
#########################################################################################################

# Make unique processing ID
# =========================

def MakeUniqueProcessingID(tag):
    processing_id = "{TAG}{DAY}-{TIME}".format(TAG = tag,
                                               DAY = dt.date.today(),
                                               TIME = dt.datetime.today().strftime("%H%M"))
    return processing_id



# Make tarfile
# ============

def MakeTarfile(source_dir, tag):
    tarfile_name = "gonzalo_" + tag + ".tar.gz"
    
    tar = tarfile.open(tarfile_name, "w:gz")
    
    for sub_dir in os.listdir(source_dir):
        if sub_dir == "InputFiles":
            continue
        print("Adding directory: " + source_dir + sub_dir)
        tar.add(source_dir + sub_dir, sub_dir)
    tar.close()
    
    # Pass tarfile to resilient
    tarfile_fullpath = IFDHMove(tarfile_name, kTARBALL_LOCATION)
    # tarfile_fullpath = ""
    return tarfile_name, tarfile_fullpath



# Make directory
# ==============

def MakeDirectory(path):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python > 2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise



# Move to IFDH
# ============

def IFDHMove(source, destination):
    cmd = "ifdh mv " + source + " " + destination
    status = subprocess.call(cmd, shell = True)
    destination_full_path =  destination + "/" + source
    return destination_full_path



# Copy to IFDH
# ============

def IFDHCopy(source, destination):
    cmd = "ifdh cp " + source + " " + destination
    status = subprocess.call(cmd, shell = True)
    destination_full_path = destination + "/" + source
    return destination_full_path





#########################################################################################################
#
#  GET OPTIONS
#
#########################################################################################################

def GetOptions():
    
    # Define parser
    parser = optparse.OptionParser(usage = "usage: %prog [options]")
    
    
    # Macro arguments
    macro_group = optparse.OptionGroup(parser, "Macro arguments")
    
    macro_group.add_option("--event_sel", action = "store_true", default = False,
                           help = "Macro that makes full event selection and plastic/physics sidebands distributions.")
    
    macro_group.add_option("--cut_studies", action = "store_true", default = False,
                           help = "Macro that makes all event selection plots cut-by-cut.")
    
    macro_group.add_option("--sidebands", action = "store_true", default = False,
                           help = "Macro that makes different tentative sideband distributions.")
    
    macro_group.add_option("--eff_optimize", action = "store_true", default = False,
                           help = "Macro that makes efficiency optimiztion distributions as function of true muon angle.")
    
    macro_group.add_option("--eff_alternative", action = "store_true", default = False,
                           help = "Macro that makes efficiency distributions as function of other variables rather than muon pT.")
    
    macro_group.add_option("--fake_data_models", action = "store_true", default = False,
                           help = "Macro that generates fake data distributions for warping studies.")
    
    macro_group.add_option("--support_studies", action = "store_true", default = False,
                           help = "Macro that makes distributions for any supporting needs (like thesis writing).")
    
    # Job arguments
    job_group = optparse.OptionGroup(parser, "Job arguments")
    
    job_group.add_option("--mc", action = "store_true", dest = "is_mc",
                         help = "Process MC anatuples.")
    
    job_group.add_option("--data", action = "store_false", dest = "is_mc",
                         help = "Process data anatuples.")
    
    job_group.add_option("--do_truth", action = "store_true", dest = "do_truth", default = False,  # Default: DO NOT process Truth tree
                         help = "Use this option to process Truth tree. Default: DO NOT process Truth tree.")
    
    job_group.add_option("--do_systematics", action = "store_true", dest = "do_systematics", default = False,  # Default: DO NOT use systematics
                         help = "Use this option to use systematics universes. Default: DO NOT use systematics.")
    
    job_group.add_option("--model", default = kMODEL_TEST,  # Default: "v1"
                         help = "Select MINERvA MC model: <v0> , <v1>, <v2JOINT>, etc. Default: <v1>")
    
    
    # Input/output arguments
    in_out_group = optparse.OptionGroup(parser, "Input/output arguments")
    
    in_out_group.add_option("--playlists", default = "ALL",
                            help = "Select between <ALL> playlists; or individual <minervame1A>, <minervame1B>, etc. Default: <ALL>")
    
    in_out_group.add_option("--anatuple_in_dir", default = kANATUPLE_DIR_TEST,
                            help = "Directory of input merged anatuples (DON'T add '/' at the end). Default: <$SCRATCH/MergedFiles/date_test>")
    
    in_out_group.add_option("--out_dir", default = kOUTDIR_TEST,
                            help = "Output directory of processed anatuples (DON'T add '/' at the end). Default: <$SCRATCH/MAT/grid_test>")
    
    in_out_group.add_option("--run", default = [],
                            help = "File run number. If not specified, it's automatically detected from playlists.")
    
    in_out_group.add_option("--test", action = "store_true", default = False,  # Default: DO NOT run test mode
                            help = "Run test mode: output directory, input anatuples and playlists are set to test mode. Default: DO NOT run test mode.")
    
    
    # Grid arguments
    grid_group = optparse.OptionGroup(parser, "Grid Options")
    
    grid_group.add_option("--memory", default = kMEMORY_MC,
                          help = "Job memory. Default: 750MB")
    
    grid_group.add_option("--lifetime", default = kLIFETIME_DATA,
                          help = "Expected job lifetime. Default: <medium>")
    
    grid_group.add_option("--filetag", default = kFILETAG,
                          help = "Tarball tag. NOTE: Don't use this option!! It's generated automatically.")
    
    grid_group.add_option("--tarfile", default = "",
                          help = "Tarfile path. NOTE: Don't use this option!! It's generated automatically.")
    
    
    # Add options to parser
    parser.add_option_group(macro_group)
    parser.add_option_group(job_group)
    parser.add_option_group(in_out_group)
    parser.add_option_group(grid_group)
    options, remainder = parser.parse_args()
    
    
    # Stop program if running data on a macro supposedly to run on MC-only
    if not options.is_mc:
        if options.eff_optimize:
            print("\nEfficiency optimization macro can't be run on data files!!\n")
            quit()
        elif options.fake_data_models:
            print("\nFake data models macro can't be run on data files!!\n")
            quit()
    
    
    # If running data, automatically change options accordingly 
    if not options.is_mc:
        options.model          = "data"
        options.do_truth       = False
        options.do_systematics = False
        
        # Safety check, just in case
        if options.do_truth:
            print("\nCan't set 'do_truth' as true when running on data files!!\n")
            quit()
        elif options.do_systematics:
            print("\nCan't set 'do_systematics' as true when running on data files!!\n")
            quit()
        elif options.model != "data":
            print("\nModel option must be set as 'data' when running data")
            quit()
    
    
    # Stop program if input model is not within the models allowed by the macros
    if options.model not in kMODELS:
        print("\nSelect a correct model!!\n")
        quit()
    
    
    # Set macro to run and output directories
    if options.event_sel:
        options.macro = kMACRO_EVENT_SEL
        options.out_dir = kOUTDIR_EVENT_SEL
        
    elif options.cut_studies:
        options.macro = kMACRO_CUT_STUDIES
        options.out_dir = kOUTDIR_CUT_STUDIES
    
    elif options.sidebands:
        options.macro = kMACRO_SIDEBAND
        options.out_dir = kOUTDIR_SIDEBAND
        
    elif options.eff_optimize:
        options.macro = kMACRO_EFF_OPTIMIZE
        options.out_dir = kOUTDIR_EFF_OPTIMIZE
        
    elif options.eff_alternative:
        options.macro = kMACRO_EFF_ALTERNATIVE
        options.out_dir = kOUTDIR_EFF_ALTERNATIVE
        
    elif options.fake_data_models:
        options.macro = kMACRO_FAKE_DATA
        options.out_dir = kOUTDIR_FAKE_DATA
        
    elif options.support_studies:
        options.macro = kMACRO_SUPPORT_STD
        options.out_dir = kOUTDIR_SUPPORT_STD
        
    else:
        print("Select a correct macro to run!!\n")
        quit()
    
    
    # Set input anatuple directories
    options.anatuple_in_dir = kANATUPLE_DIR
    
    
    # If running systematics, set large memory and very long expected lifetime
    # Otherwise, only 'EventSelection.C' needs a bit longer to run, other macros can be run normally
    if options.do_systematics:
        options.memory   = kMEMORY_LONG_MC
        options.lifetime = kLIFETIME_LONG_MC
    else:
        if options.is_mc:
            options.memory   = kMEMORY_MC
            options.lifetime = kLIFETIME_MC
        else:
            options.memory   = kMEMORY_DATA
            options.lifetime = kLIFETIME_DATA
        # if options.event_sel:
        #     if options.is_mc:
        #         options.memory   = kMEMORY_LONG_MC
        #         options.lifetime = kLIFETIME_MC  # No need to run for an entire day
        #     else:
        #         options.memory   = kMEMORY_LONG_DATA
        #         options.lifetime = kLIFETIME_LONG_DATA
                
        # elif options.cut_studies:
        #     if options.is_mc:
        #         options.memory   = kMEMORY_LONG_MC
        #         options.lifetime = kLIFETIME_MC  # No need to run for an entire day
        #     else:
        #         options.memory   = kMEMORY_LONG_DATA
        #         options.lifetime = kLIFETIME_LONG_DATA
                
        # else:
        #     if options.is_mc:
        #         options.memory   = kMEMORY_MC
        #         options.lifetime = kLIFETIME_MC
        #     else:
        #         options.memory   = kMEMORY_DATA
        #         options.lifetime = kLIFETIME_DATA
    
    
    # If test mode selected, set special parameters for test mode
    if options.test:
        if options.do_systematics:
            print("\nDon't run systematics on test mode!!\n")
            quit()
            
        if options.is_mc:
            options.memory   = kMEMORY_MC
            options.lifetime = kLIFETIME_MC
        else:
            options.memory   = kMEMORY_DATA
            options.lifetime = kLIFETIME_DATA
        
        options.model   = kMODEL_TEST
        options.out_dir = kOUTDIR_TEST
        options.anatuple_in_dir = kANATUPLE_DIR_TEST
        del kPLAYLISTS[:]
        kPLAYLISTS.extend(kPLAYLISTS_TEST)
    
    
    # Print full configuration
    print("\nMacro: " + options.macro)
    
    print("\nJob arguments: ")
    print("\tProcessing MC?  " + str(options.is_mc))
    print("\tUse Truth tree? " + str(options.do_truth))
    print("\tDo systematics? " + str(options.do_systematics))
    print("\tMINERvA model:  " + str(options.model))
    
    print("\nInput/output arguments: ")
    print("\tInput anatuple dir: " + options.anatuple_in_dir)
    print("\tOutput files dir:   " + options.out_dir)
    print("\tPlaylists: ")
    for i in kPLAYLISTS:
        print("\t\t" + i)
    
    print("\nGrid arguments:")
    print("\tMemory:   " + str(options.memory))
    print("\tLifetime: " + str(options.lifetime))
    
    
    # Fix file tag underscore
    if options.filetag != kFILETAG and not options.filetag[:1] == "_":
        options.filetag = "_" + options.filetag
    
    
    # Return set of options
    return options





#################################################################################################################################################
# -----------------------------------------------------------------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------------------------
#  MAIN SCRIPT
# -----------------------------------------------------------------------------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------------------------
#################################################################################################################################################

def main():
    
    # Get options
    options = GetOptions()
    
    
    # A unique string for this job
    processing_id = MakeUniqueProcessingID(options.filetag)
    
    
    # Make tarfile and pass to resilient
    if options.tarfile:
        tarfile = options.tarfile.split("/")[-1]
        tarfile_fullpath = options.tarfile
    else:
        print("\nTarring up top directory: " + kTOPDIR)
        print("\nCreating tarfile... ")
        tarfile, tarfile_fullpath = MakeTarfile(kTOPDIR + "/", processing_id)
    
    print("\nTarfile: " + tarfile)
    print("\nTarfile full path: " + tarfile_fullpath)
    
    
    # Run number
    if options.run:
        print("\nSubmitting run: " + options.run)
    
    
    # Send grid script to PNFS
    cache = kCACHE_PNFS_AREA + "/" + processing_id + "/"
    print("\nSending 'grid_macro.sh' to: " + cache)
    MakeDirectory(cache)
    grid_script = IFDHCopy("grid_macro.sh", cache)
    
    
    # Loop over playlists
    # ===================
    
    print("\nOutdir (top): " + options.out_dir)
    print("Process ID: " + processing_id)
    
    
    for i_playlist in kPLAYLISTS:
        
        # Check that this playlist exists in the options
        do_this_playlist = (i_playlist == options.playlists) or (options.playlists == "ALL")
        if not do_this_playlist:
            continue
        
        
        # Make output directory of this playlist
        out_dir = options.out_dir + "/" + processing_id + "/" + i_playlist
        print("\nFull outdir of this playlist: " + out_dir)
        print("\nMaking directory...")
        MakeDirectory(out_dir)
        
        
        # Define anatuple directory of this playlist
        if options.is_mc:
            anatuple_dir = options.anatuple_in_dir + "/mc/nuke/{0}".format(i_playlist)
        else:
            anatuple_dir = options.anatuple_in_dir + "/data/{0}".format(i_playlist)
        
        print("\nUsing tuples from directory: " + anatuple_dir)
        
        
        # Get list of anatuples from directory
        anatuples = glob.glob(anatuple_dir + "/*")
        
        
        # Loop over input anatuples
        # =========================
        
        for i_anatuple in anatuples:
            
            if not ("CC" in i_anatuple) or not (".root" in i_anatuple):
                continue
            
            # Get run number from file name
            if options.is_mc:
                run = i_anatuple[-22:-14]
                run = run.lstrip("0")
            else:
                run = i_anatuple[-22:-14]
                run = run.lstrip("0")
            
            
            # Change anatuple according to XROOTD input
            def XROOTDify(anatuple):
                return anatuple.replace("/pnfs/", "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr///")
            
            i_anatuple = XROOTDify(i_anatuple)
            
            print("\nAnatuple to analyze: " + i_anatuple)
            print("Run number: " + run)
            
            
            # Define macro
            macro = options.macro
            
            if (macro == kMACRO_EVENT_SEL):
                macro += ("({IS_MC},\\\\\\\"{PLAYLIST}\\\\\\\","
                          "\\\\\\\"{MODEL}\\\\\\\","
                          "{IS_GRID},{DO_TRUTH},{DO_SYSTEMATICS},"
                          "\\\\\\\"{INPUT_FILE}\\\\\\\",{RUN})".format(IS_MC          = "true" if options.is_mc else "false",
                                                                       PLAYLIST       = i_playlist,
                                                                       MODEL          = options.model,
                                                                       IS_GRID        = "true",
                                                                       DO_TRUTH       = "true" if options.do_truth else "false",
                                                                       DO_SYSTEMATICS = "true" if options.do_systematics else "false",
                                                                       INPUT_FILE     = i_anatuple,
                                                                       RUN            = run))
            
            elif (macro == kMACRO_CUT_STUDIES) or (macro == kMACRO_SIDEBAND) or (macro == kMACRO_SUPPORT_STD):
                macro += ("({IS_MC},\\\\\\\"{PLAYLIST}\\\\\\\","
                          "\\\\\\\"{MODEL}\\\\\\\","
                          "{IS_GRID},"
                          "\\\\\\\"{INPUT_FILE}\\\\\\\",{RUN})".format(IS_MC      = "true" if options.is_mc else "false",
                                                                       PLAYLIST   = i_playlist,
                                                                       MODEL      = options.model,
                                                                       IS_GRID    = "true",
                                                                       INPUT_FILE = i_anatuple,
                                                                       RUN        = run))
            
            elif (macro == kMACRO_EFF_OPTIMIZE) or (macro == kMACRO_EFF_ALTERNATIVE) or (macro == kMACRO_FAKE_DATA):
                macro += ("(\\\\\\\"{PLAYLIST}\\\\\\\","
                          "\\\\\\\"{MODEL}\\\\\\\","
                          "{IS_GRID},{DO_TRUTH},{DO_SYSTEMATICS},"
                          "\\\\\\\"{INPUT_FILE}\\\\\\\",{RUN})".format(PLAYLIST       = i_playlist,
                                                                       MODEL          = options.model,
                                                                       IS_GRID        = "true",
                                                                       DO_TRUTH       = "true" if options.do_truth else "false",
                                                                       DO_SYSTEMATICS = "true" if options.do_systematics else "false",
                                                                       INPUT_FILE     = i_anatuple,
                                                                       RUN            = run))
            
            macro = "\"" + macro + "\""
            print("\nMacro: " + macro)
            
            
            # Prepare submit command
            submit_command = ("jobsub_submit {GRID} " "-d OUT {OUTDIR} " "-L {LOGFILE} "
                              "--memory {MEMORY} "
                              "--expected-lifetime {LIFETIME} "
                              "-e MACRO={MACRO} "
                              "-e TARFILE={TARFILE} "
                              "-f {TARFILE_FULLPATH} "
                              "file://{GRID_SCRIPT}".format(GRID             = kGRID_OPTIONS,
                                                            OUTDIR           = out_dir,
                                                            LOGFILE          = out_dir + "/log{0}.txt".format(run),
                                                            MEMORY           = options.memory,
                                                            LIFETIME         = options.lifetime,
                                                            MACRO            = macro,
                                                            TARFILE          = tarfile,
                                                            TARFILE_FULLPATH = tarfile_fullpath,
                                                            GRID_SCRIPT      = grid_script))
            
            print("\nSubmitting to grid:\n" + submit_command + "\n")
            
            
            # Ship job
            status = subprocess.call(submit_command, shell = True)
        
        
        
if __name__ == '__main__':
    main()
