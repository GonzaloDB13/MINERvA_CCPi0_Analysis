import os,sys
import ROOT
import PlotUtils

ROOT.TH1.AddDirectory(False)


# Auxiliar function
# =================

def sortInputFiles(filelist):
    newlist = []
    factors = []
    for f in filelist:
        fn = f.rstrip("\n")
        mcfactor = float(fn.split(".txt")[-2].split("_")[-1])
        factors.append(mcfactor)

    factors = sorted(factors)
    for f in factors:
        for fi in filelist:
            mcfactor = float(fi.split(".txt")[-2].split("_")[-1])
            if(f==mcfactor): newlist.append(fi)

    return newlist



# Main function
# =============

inputfiles = sys.argv[1:]
newlist = sortInputFiles(inputfiles)

date = raw_input("\tDate: ")
base_model = raw_input("\tBase model: ")
uncfactor = raw_input("\tUnc. factor: ")

chi2_by_iter_by_file = []
for f in newlist:
    fn = f.rstrip("\n")
    temp_file = open(fn,"r").readlines()
    n_uni = 0
    current_iter = 0
    chi2_by_iter = []
    for l in temp_file:
        temp_line = l.split()
        chi2 = float(temp_line[0])
        iteration = int(temp_line[1])
        universe = int(temp_line[2])
        if (chi2>1.5e3):
            continue
        if(universe>n_uni):
            n_uni=universe+1
        if(iteration!=current_iter):
            if(current_iter!=0):
                chi2_by_iter[-1][1]/=n_uni
            chi2_by_iter.append([iteration,chi2])
            current_iter=iteration
        else:
            chi2_by_iter[-1][1]+=chi2
        if l == temp_file[-1]:
            chi2_by_iter[-1][1]/=n_uni

    print chi2_by_iter
    chi2_by_iter_by_file.append(chi2_by_iter)


# Make graphs
mygraphs = []
for i in range(0,len(chi2_by_iter_by_file)):
    tmpgraph = ROOT.TGraph()
    for j,el in enumerate(chi2_by_iter_by_file[i]):
        tmpgraph.SetPoint(j,el[0],el[1])
    mygraphs.append(tmpgraph)


# Output file
output_str = "/pnfs/minerva/persistent/users/gonzalo/MAT/UnfoldStatStudies/" + date + "_" + base_model + "/iron/PlotMCSampleSizeScan/Uncfactor" + uncfactor + "_iron.root"
output_file = ROOT.TFile(output_str, "RECREATE")

for i,g in enumerate(mygraphs):
    g.Write("StatFactor%s"%(str(newlist[i].split(".txt")[-2].split("_")[-1])))

raw_input("DONE")

output_file.Close()
