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
    
    muminus_singer_tgt = [ "Al", "Si", "Ca", "Fe", "Ag", "I", "Au", "Pb" ]
    muminus_sundelin_tgt = [ "Si", "S", "Ca" ]
    piminus_tgt = [ "Al", "C", "Cu", "N", "O", "Pb", "Ta" ]

    def parse(self, jobs):
        # should be just one job for each job
        job = jobs[0]
	print "path = ", job["path"]
	
        material = job["MATERIAL"]
	if 'G4_' in job["MATERIAL"]:
	   material = (job["MATERIAL"]).split("_")[-1] # [-1] means counting from right
	
        particle = job["PARTICLE"]
	if 'pi-' in particle:
	   particle = 'piminus'
	if 'mu-' in particle:
	   particle = 'muminus'
	
	# FIXME !!! Need to refine "captureUpdate" think in the app...
	#
	fname = particle + job["MATERIAL"] + job["GENERATOR"] + ".root"
	filename = os.path.join(job["path"], fname)
	print "filename = ", filename

        hfile = _open_file(filename)	
	
	for h in hfile.GetListOfKeys():
	   if 'piminus' in particle:
	      hh = hfile.Get("NvsT")
	      observableName = "NNeutrons/MeV"
	      xAxisTitle = "EKin (MeV)"
	      yAxisTitle = "NNeutrons/MeV"
	   if 'muminus' in particle:
	      if material in muminus_singer_tgt:
	         hh = hfile.Get("NNuetrons")
	         observableName = "Neutron Yield"
	         xAxisTitle = "NNeutrons/capture"
	         yAxisTitle = "Normalized yield"
	      elif material in muminus_sundelin_tgt:
	         hh = hfile.Get("NeutronKineticEnergy")
	         observableName = "NNeutrons/capture/MeV"
	         xAxisTitle = "EKin (MeV)"
	         yAxisTitle = "NNeutrons/capture/MeV"
	      else:
	         print "Skip histo ", h.GetName()
		 continue
	   	   
	   firstNonZeroBin = hh.FindFirstBinAbove(0)
	   lastNonZeroBin  = hh.FindLastBinAbove(0)

           nBins = []
           binEdgeLow = []
           binEdgeHigh = []
           binContent = []
           yStatErrorPlus = []
           yStatErrorMinus = []
           beamenergies = []
           beamenergies.append(float(job['ENERGY']))

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
	    
	   # --> parameters = []

	   htitle = job["PARTICLE"] + material + " -> " + secondary + " + X" 
	   	    
	   rjson = getJSON( job, "histogram",
	                     mctool_name="GEANT4",
			     mctool_version=job["VERSION"],
			     mctool_model=job["GENERATOR"],
			     testName="test48",
			     plotType="TH1",
			     observableName=observableName,
			     reaction="particle production", # otherwise it'll write the default which is "reaction name"
			     targetName=material,
			     beamParticle=job["PARTICLE"],
			     beamEnergies=beamenergies,
			     secondaryParticle="neutron",
			     parameters=[],
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
        print "Exception: ", str(e)
        # ROOT related exception
        return None


def _close_file(root_file):
    try:
        root_file.Close()
        del root_file
    except:
        # ROOT related exception
        return None
