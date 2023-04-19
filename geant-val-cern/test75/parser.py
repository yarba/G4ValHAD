#!/usr/bin/env python

# class for sc test parsing
import os
from gts.utils import getJSON
from gts.BaseParser import BaseParser
from ROOT import gROOT, TFile, TTree
from ROOT import gDirectory
gROOT.SetBatch(True)
gROOT.ProcessLine("gErrorIgnoreLevel = 5000;")

class Test (BaseParser):
    TEST = "test75"

    def parse(self, jobs):
        # should be just one job for each job
        job = jobs[0]
        print("path = ", job["path"])
	
        material = job["MATERIAL"]
        if 'G4_' in job["MATERIAL"]:
           material = (job["MATERIAL"]).split("_")[-1] # [-1] means counting from right

        energy = job["ENERGY"]
        if not 'MeV' in job["ENERGY_UNIT"]:
           en = float(energy) * 1000.
           energy = str(en)
        if '.' in energy:
           energy = energy.split(".")[0]

        fname = job["PARTICLE"] + energy + "MeV" + job["MATERIAL"] + job["GENERATOR"] + ".root"
        filename = os.path.join(job["path"], fname)
# -->         print("filename = ", filename)

        hfile = _open_file(filename)

        # FIXME !!! Also need to skip those theta that we don't have data for
        for h in hfile.GetListOfKeys():
           if '300' in energy:
              if 'pip' in h.GetName() or 'pim' in h.GetName():
                 print("Skip histo ", h.GetName())
                 continue
              if '60' in h.GetName() or '72' in h.GetName() or '84' in h.GetName():
                 print("Skip histo ", h.GetName()) 
                 continue
              if 'proKE' in h.GetName() or 'Cos' in h.GetName():
                 print("Skip histo ", h.GetName())
                 continue
              secondary = 'proton'
           elif '668' in energy:
              if 'Pb' in material and '28' in h.GetName(): 
                 print("Skip histo ", h.GetName())
                 continue
              if 'pip' in h.GetName():
                 secondary = "pi+"
              elif 'pim' in h.GetName():
                 secondary = "pi-"
              else:
                 print("Skip histo ", h.GetName()) 
                 continue
           else:
              print(" Unknown study case; skip")
              continue

           parameter_name = []
           parameter_name.append("THETA")
           parameter_value = []
#           parvalue = filter(str.isdigit, h.GetName())
           parvalue = ""
           for c in h.GetName():
              if c.isdigit():
                 parvalue += c
           if '28' in parvalue:
              parvalue += ".4"
           if '44' in parvalue:
              parvalue += ".2" 
           parameter_value.append( parvalue )
   
           hh = hfile.Get( h.GetName() );
   	   
           firstNonZeroBin = hh.FindFirstBinAbove(0)
           lastNonZeroBin  = hh.FindLastBinAbove(0)

           nBins = []
           binEdgeLow = []
           binEdgeHigh = []
           binContent = []
           yStatErrorPlus = []
           yStatErrorMinus = []
           beamenergies = []
           # beamenergies.append(float(job['ENERGY']))
           beamenergies.append(float(energy))

           for x in range(firstNonZeroBin, lastNonZeroBin+1):
               binContent.append(round(hh.GetBinContent(x),3))
               binEdgeLow.append(round(hh.GetBinLowEdge(x),2)) # it's better/safer to round it to the 2nd digit after decimal point
               binEdgeHigh.append(round((hh.GetBinLowEdge(x)+hh.GetBinWidth(x)),2))
               #
               # NOTE: don't use BinErrorLow/Up as they return BinError !!!
               #
               yStatErrorPlus.append(round(hh.GetBinError(x)/2.,3))
               yStatErrorMinus.append(round(hh.GetBinError(x)/2.,3))

           nBins.append(len(binContent))
    
           parameters = []
           parameters_test = {"names": parameter_name[0], "values": parameter_value[0] + " deg"}
           parameters.append(parameters_test)

           htitle = job["PARTICLE"] + " + " + material + " -> " + secondary + " + X" 
   
           observableName = "D2(SIG)/DP/DTHETA"
           xAxisTitle = "p (MeV/c)"
           yAxisTitle = "D2(SIG)/DP/DTHETA (mub/sr/MeV)"
           if 'proton' in secondary:
              observableName = "D2(SIG)/DE/DTHETA"
              xAxisTitle = "EKin (MeV)"
              yAxisTitle = "D2(SIG)/DE/DTHETA (mub/sr/MeV)"
    
           rjson = getJSON( job, "histogram",
                            mctool_name="GEANT4",
                            mctool_version=job["VERSION"],
                            mctool_model=job["GENERATOR"],
                            testName="test75",
                            plotType="TH1",
                            observableName=observableName,
                            reaction="particle production", # otherwise it'll write the default which is "reaction name"
                            targetName=material,
                            beamParticle=job["PARTICLE"],
                            beamEnergies=beamenergies,
                            secondaryParticle=secondary,
                            parameters=parameters,
                            nBins=nBins,
                            binContent=binContent,
                            binEdgeLow=binEdgeLow,
                            binEdgeHigh=binEdgeHigh,
                            yStatErrorsPlus=yStatErrorPlus,
                            yStatErrorsMinus=yStatErrorMinus,
                            xAxisName=xAxisTitle,
                            yAxisName=yAxisTitle,
                            title=htitle
                          )
     
           yield rjson

# =========================================
#
# root functions
#


def _open_file(file_path):
    # Open as ROOT file
    try:
        f = TFile.Open(file_path)
        if f and f.IsOpen():
            if f.GetListOfKeys().GetEntries() == 0:
                return None
            return f
        else:
            return None
    except Exception as e:
        print("Exception: ", str(e))
        # ROOT related exception
        return None


def _close_file(root_file):
    try:
        root_file.Close()
        del root_file
    except:
        # ROOT related exception
        return None
