#!/usr/bin/env python

# class for sc test parsing
import os
from gts.utils import getJSON
from gts.BaseParser import BaseParser
from ROOT import gROOT, TFile, TTree
from ROOT import gDirectory
gROOT.SetBatch(True)
gROOT.ProcessLine("gErrorIgnoreLevel = 5000;")


# This only works for NA61 so far !!!
# Provisions for HARP, SASM6E, and NA49 need to be added !!!

class Test (BaseParser):
    TEST = "test19"
    beam = ""
    material = ""
    energy = ""
    secondary = ""
    detector = ""
    binEdgeLow = []
    binEdgeHigh = []
    binContent = []
    yStatErrorPlus = []
    yStatErrorMinus = []
    
    def parse(self, jobs):
        # should be just one job for each job
        job = jobs[0]
	print "path = ", job["path"]
	
        self.material = job["MATERIAL"]
	if 'G4_' in job["MATERIAL"]:
	   self.material = (job["MATERIAL"]).split("_")[-1] # [-1] means counting from right
	
	self.energy = job["ENERGY"]
	if 'MeV' in job["ENERGY_UNIT"]:
	   en = round(float(self.energy)/1000.,1)
	   self.energy = str(en)
        if not '.' in self.energy:
	   self.energy = self.energy + ".0"
	
	self.beam = job["PARTICLE"]
	if 'pi+' in job["PARTICLE"]:
	   self.beam = 'piplus'
	if 'pi-' in job["PARTICLE"]:
	   self.beam = 'piminus'
		
	self.detector = job["DETECTOR"].lower()
	
	fname = self.detector + "-histo/" + self.beam + self.material + self.energy + "GeV" + job["GENERATOR"] + ".root"	
        filename = os.path.join(job["path"], fname)
# -->	print "filename = ", filename

        hfile = _open_file(filename)
	
	skip_na49_ddiff = { 'pTpim0', 'pTpim1', 'pTpim2', 'pTpim3', 'pTpim4', 'pTpim5', 'pTpim6',
	                    'pTpip0', 'pTpip1', 'pTpip2', 'pTpip3', 'pTpip4', 'pTpip5', 'pTpip6'
	                  }

	for h in hfile.GetListOfKeys():  # keep in mind that h is not a TH1 but is a TKey
	   skip = False
	   if 'NSec' in h.GetName():
	      print "skip histo ", h.GetName()   # skip this histo whereever found as it's for normalization only
	      skip = True
	   if 'Mult_' in h.GetName() or 'kplus2piplus' in h.GetName(): # skip NA61 K+/pi+ for now
	      print "skip histo ", h.GetName()
	      skip = True
	   if '_mom_' in h.GetName():
	      print "skip histo ", h.GetName()   # skip HARP Mu2e-like plots for now
	      skip = True
	   if '_dXSecdxF' in h.GetName() or '_pT2' in h.GetName():  # skip average NA49 pt**2 and dXsec/dxF for now
	      print "skip histo ", h.GetName()
	      skip = True
           for hnm in skip_na49_ddiff:
	      if hnm == h.GetName():
	         print "skip histo ", h.GetName()
		 skip = True
		 break
	   if skip:
	      continue       
	   
	   if 'piplus' in h.GetName() or 'pTpip' in h.GetName():
	      self.secondary = 'pi+'
	   if 'piminus' in h.GetName() or 'pTpim' in h.GetName():
	      self.secondary = 'pi-'
	   if 'proton' in h.GetName() or 'pTpro' in h.GetName():
	      self.secondary = 'proton'
	   if 'antiproton' in h.GetName() or 'pTpbar' in h.GetName():
	      self.secondary = 'anti_proton'
	   if 'k0s' in h.GetName():
	      self.secondary = 'kaon0s'
	   if 'kplus' in h.GetName():
	      self.secondary = 'kaon+'
	   if 'kminus' in h.GetName():
	      self.secondary = 'kaon-'
	   if 'lambda' in h.GetName():
	      self.secondary = 'lambda'
	   if 'neutron' in h.GetName():
	      self.secondary = 'neutron'

	   hh = hfile.Get( h.GetName() );
	   
	   rjson = ""
	   if 'na61' in self.detector:
	      rjson = self.parse_NA61(job,hh)
	   if 'harp' in self.detector:
	      rjson = self.parse_HARP(job,hh)
	   if 'na49' in self.detector:
	      rjson = self.parse_NA49(job,hh)
	   if 'sasm6e' in self.detector:
	      rjson = self.parse_SASM6E(job,hh)
	   
	   yield rjson


    def parse_NA61(self, job, hh):
    	   	   
       self.getBins(hh)
       
       nBins = []
       nBins.append(len(self.binContent))
              
       beamenergies = []
       # beamenergies.append(float(job['ENERGY']))
       beamenergies.append(float(self.energy))
       	    
       parameters = []
       parameters_test = {"names": "THETA", 
                          "values": hh.GetName().split("_")[1] + "-" + hh.GetName().split("_")[2] + " mrad"}
       parameters.append(parameters_test)
       
       htitle = job["PARTICLE"] + " + " + self.material + " -> " + self.secondary + " + X" 
       #
       # Note: using "yield" here somehow messes things up;
       #       so use old-fashoned "return"
       #
       return getJSON(  job, "histogram",
	                mctool_name="GEANT4",
			mctool_version=job["VERSION"],
			mctool_model=job["GENERATOR"],
			testName=self.TEST,
			plotType="TH1",
			observableName="D2(SIG)/DP/DTHETA",
			reaction="particle production", # otherwise it'll write the default which is "reaction name"
			targetName=self.material,
			beamParticle=job["PARTICLE"],
			beamEnergies=beamenergies,
			secondaryParticle=self.secondary,
			parameters=parameters,
			nBins=nBins,
			binContent=self.binContent,
			binEdgeLow=self.binEdgeLow,
			binEdgeHigh=self.binEdgeHigh,
                        yStatErrorsPlus=self.yStatErrorPlus,
                        yStatErrorsMinus=self.yStatErrorMinus,
			xAxisName="p (GeV/c)",
			yAxisName="D2(SIG)/DP/DTHETA (mb/mrad/GeV/c)",
			title=htitle
			)
              
    def parse_NA49(self, job, hh):
              
       mydict = { 'proton' : {  '0' : '-0.8',
                                '1' : '-0.75',
			        '2' : '-0.7',
			        '3' : '-0.65',
			        '4' : '-0.6',
			        '5' : '-0.55',
			        '6' : '-0.5',
			        '7' : '-0.45',
			        '8' : '-0.4',
			        '9' : '-0.35',
			       '10' : '-0.3',
			       '11' : '-0.25',
			       '12' : '-0.2',
			       '13' : '-0.15',
			       '14' : '-0.1',
			       '15' : '-0.075',
			       '16' : '-0.05',
			       '17' : '-0.025',
			       '18' : '0.0',
			       '19' : '0.025',
			       '20' : '0.05',
			       '21' : '0.075',
			       '22' : '0.1',
			       '23' : '0.15',
			       '24' : '0.2',
			       '25' : '0.25',
			       '26' : '0.3',
			       '27' : '0.35',
			       '28' : '0.4',
			       '29' : '0.45',
			       '30' : '0.5',
			       '31' : '0.55',
			       '32' : '0.6',
			       '33' : '0.65',
			       '34' : '0.7',
			       '35' : '0.75',
			       '36' : '0.8',
			       '37' : '0.85',
			       '38' : '0.9',
			       '39' : '0.95'
                             },
	          'pi+'    : {  '0' : '-0.5',   # skip - no exp.data
		                '1' : '-0.4',   # skip
			        '2' : '-0.3',   # skip
			        '3' : '-0.25',  # skip
			        '4' : '-0.2',   # skip
			        '5' : '-0.15',  # skip
			        '6' : '-0.125', # skip
			        '7' : '-0.1',
			        '8' : '-0.075',
			        '9' : '-0.05',
			       '10' : '-0.04',
			       '11' : '-0.03',
			       '12' : '-0.02',
			       '13' : '-0.01',
			       '14' : '0.0',
			       '15' : '0.01',
			       '16' : '0.02',
			       '17' : '0.03',
			       '18' : '0.04',
			       '19' : '0.05',
			       '20' : '0.075',
			       '21' : '0.1',
			       '22' : '0.125',
			       '23' : '0.15',
			       '24' : '0.2',
			       '25' : '0.25',
			       '26' : '0.3',
			       '27' : '0.4',
			       '28' : '0.5' 
		             },
                  'pi-'    : {  '0' : '-0.5',   # skip - no exp.data
		                '1' : '-0.4',   # skip
			        '2' : '-0.3',   # skip
			        '3' : '-0.25',  # skip
			        '4' : '-0.2',   # skip
			        '5' : '-0.15',  # skip
			        '6' : '-0.125', # skip
			        '7' : '-0.1',
			        '8' : '-0.075',
			        '9' : '-0.05',
			       '10' : '-0.04',
			       '11' : '-0.03',
			       '12' : '-0.02',
			       '13' : '-0.01',
			       '14' : '0.0',
			       '15' : '0.01',
			       '16' : '0.02',
			       '17' : '0.03',
			       '18' : '0.04',
			       '19' : '0.05',
			       '20' : '0.075',
			       '21' : '0.1',
			       '22' : '0.125',
			       '23' : '0.15',
			       '24' : '0.2',
			       '25' : '0.25',
			       '26' : '0.3',
			       '27' : '0.4',
			       '28' : '0.5' 
		             },
		  'anti_proton' : {  '0' : '-0.2',
		                     '1' : '-0.15',
				     '2' : '-0.1',
				     '3' : '-0.075',
				     '4' : '-0.05',
				     '5' : '-0.025',
				     '6' : '0.0',
				     '7' : '0.025',
				     '8' : '0.05',
				     '9' : '0.1',
				    '10' : '0.15',
				    '11' : '0.2',
				    '12' : '0.3'
		                  }
		}
       
       self.getBins(hh)
       
       nBins = []
       nBins.append(len(self.binContent))
              
       beamenergies = []
       # beamenergies.append(float(job['ENERGY']))
       beamenergies.append(float(self.energy))
       
       parameters = []
       observable = ""
       xaxis = ""
       yaxis = ""
       
       if 'xF' in hh.GetName():
          observable = "density distribution dn/dx_{F}"
	  xaxis = "x_{F}"
	  yaxis = "dN/d_x{F}"
       if '_pT' in hh.GetName():
          observable = "average p_{T} vs x_{F}"
	  xaxis = "x_{F}"
	  yaxis = "<p_{T}> (GeV/c)"
       if 'pTp' in hh.GetName():
          observable = "E(D3SIG/DP3)" 
	  xaxis = "p_{T} (GeV/c)"
	  yaxis = "E(D3SIG/DP3) (mb/GeV^{2}/c^{3})"
	  index = ""
	  for i in hh.GetName():
	     if i.isdigit():
	        index += i
          parameters_test = { "names": "x_{F}", "values": mydict[self.secondary][index] }
          parameters.append(parameters_test)

       htitle = job["PARTICLE"] + " + " + self.material + " -> " + self.secondary + " + X" 

       return getJSON(  job, "histogram",
	                mctool_name="GEANT4",
			mctool_version=job["VERSION"],
			mctool_model=job["GENERATOR"],
			testName=self.TEST,
			plotType="TH1",
			observableName=observable,
			reaction="particle production", # otherwise it'll write the default which is "reaction name"
			targetName=self.material,
			beamParticle=job["PARTICLE"],
			beamEnergies=beamenergies,
			secondaryParticle=self.secondary,
			parameters=parameters,
			nBins=nBins,
			binContent=self.binContent,
			binEdgeLow=self.binEdgeLow,
			binEdgeHigh=self.binEdgeHigh,
                        yStatErrorsPlus=self.yStatErrorPlus,
                        yStatErrorsMinus=self.yStatErrorMinus,
			xAxisName=xaxis,
			yAxisName=yaxis,
			title=htitle
			)
    
    def parse_HARP(self, job, hh):
       
       mydict = { 'FW_0' : '0.05-0.1 rad',
                  'FW_1' : '0.1-0.15 rad',
		  'FW_2' : '0.15-0.2 rad',
		  'FW_3' : '0.2-0.25 rad',
		  'LA_0' : '0.35-0.55 rad',
		  'LA_1' : '0.55-0.75 rad',
		  'LA_2' : '0.75-0.95 rad',
		  'LA_3' : '0.95-1.15 rad',
		  'LA_4' : '1.15-1.35 rad',
		  'LA_5' : '1.35-1.55 rad',
		  'LA_6' : '1.55-1.75 rad',
		  'LA_7' : '1.75-1.95 rad',
		  'LA_8' : '1.95-2.15 rad'
	        }
       	   	   
       self.getBins(hh)
       
       nBins = []
       nBins.append(len(self.binContent))
              
       beamenergies = []
       # beamenergies.append(float(job['ENERGY']))
       beamenergies.append(float(self.energy))

       # NOTE: ptag = hh.GetName()[hh.GetName().find("_")+1:]
       #       gives an error related to ROOT (can't split or something) 
       #
       hname = hh.GetName()
       ptag = hname[hname.find("_")+1:]
       parameters = []
       parameters_test = { "names": "THETA", "values": mydict[ptag] }
       parameters.append(parameters_test)
       
       htitle = job["PARTICLE"] + " + " + self.material + " -> " + self.secondary + " + X" 
       #
       # Note: using "yield" here somehow messes things up;
       #       so use old-fashoned "return"
       #
       return getJSON(  job, "histogram",
	                mctool_name="GEANT4",
			mctool_version=job["VERSION"],
			mctool_model=job["GENERATOR"],
			testName=self.TEST,
			plotType="TH1",
			observableName="D2(SIG)/DP/DTHETA",
			reaction="particle production", # otherwise it'll write the default which is "reaction name"
			targetName=self.material,
			beamParticle=job["PARTICLE"],
			beamEnergies=beamenergies,
			secondaryParticle=self.secondary,
			parameters=parameters,
			nBins=nBins,
			binContent=self.binContent,
			binEdgeLow=self.binEdgeLow,
			binEdgeHigh=self.binEdgeHigh,
                        yStatErrorsPlus=self.yStatErrorPlus,
                        yStatErrorsMinus=self.yStatErrorMinus,
			xAxisName="p (GeV/c)",
			yAxisName="D2(SIG)/DP/DTHETA (mb/rad/GeV/c)",
			title=htitle
			)
    

    def parse_SASM6E(self, job, hh):
       
       mydict = { 'pt180' : '0.18 GeV/c',
                  'pt300' : '0.3 GeV/c',
		  'pt500' : '0.5 GeV/c'
	        }

       self.getBins(hh)
       
       nBins = []
       nBins.append(len(self.binContent))
              
       beamenergies = []
       beamenergies.append(float(self.energy))

       hname = hh.GetName()
       ptag = hname[hname.find("_")+1:]
       parameters = []
       parameters_test = { "names": "p_{T}", "values": mydict[ptag] }
       parameters.append(parameters_test)
       
       htitle = job["PARTICLE"] + " + " + self.material + " -> " + self.secondary + " + X" 

       return getJSON(  job, "histogram",
	                mctool_name="GEANT4",
			mctool_version=job["VERSION"],
			mctool_model=job["GENERATOR"],
			testName=self.TEST,
			plotType="TH1",
			observableName="E(D3SIG/DP3)",
			reaction="particle production", # otherwise it'll write the default which is "reaction name"
			targetName=self.material,
			beamParticle=job["PARTICLE"],
			beamEnergies=beamenergies,
			secondaryParticle=self.secondary,
			parameters=parameters,
			nBins=nBins,
			binContent=self.binContent,
			binEdgeLow=self.binEdgeLow,
			binEdgeHigh=self.binEdgeHigh,
                        yStatErrorsPlus=self.yStatErrorPlus,
                        yStatErrorsMinus=self.yStatErrorMinus,
			xAxisName="p (GeV/c)",
			yAxisName="E(D3SIG/DP3) (mb/GeV^{2}/c^{3})",
			title=htitle
			)


    def getBins(self, hh):
       
       del self.binEdgeLow[:]
       del self.binEdgeHigh[:]
       del self.binContent[:]
       del self.yStatErrorPlus[:]
       del self.yStatErrorMinus[:]

       firstNonZeroBin = hh.FindFirstBinAbove(0)
       lastNonZeroBin  = hh.FindLastBinAbove(0)
       
       for x in range(firstNonZeroBin, lastNonZeroBin+1):
          self.binContent.append(round(hh.GetBinContent(x),3))
	  self.binEdgeLow.append(round(hh.GetBinLowEdge(x),2)) # it's better/safer to round it to the 2nd digit after decimal point
	  self.binEdgeHigh.append(round((hh.GetBinLowEdge(x)+hh.GetBinWidth(x)),2))
	  #
	  # NOTE: don't use BinErrorLow/Up as they return BinError !!!
	  #
	  self.yStatErrorPlus.append(round(hh.GetBinError(x)/2.,3))
	  self.yStatErrorMinus.append(round(hh.GetBinError(x)/2.,3))


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
