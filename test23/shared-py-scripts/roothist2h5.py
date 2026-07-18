#!/usr/bin/env python
import sys
import argparse
import numpy as np
import os
import uproot
import h5py

def parse_args(argv):
    p = argparse.ArgumentParser()
    p.add_argument("--HDF5Output", type=str, default="")
    p.add_argument("--RootInput", type=str, default="")
    args = p.parse_args()
    return args

def main(argv):
   args = parse_args(argv)
   print(f"Input: {args.RootInput}")
   print(f"Output: {args.HDF5Output}")
   
   with uproot.open(args.RootInput) as rf:
      # it means the array hnames where each element "key" gets filled (valus assigned) as long 
      # the key is in the list of keys in the Root file, and the class is of type TH1 or TH2 
      # NOTE: one can also consider adding provisions for TProfile
      # NOTE: for converting files containing TTree structures there seem to exist dedicated 
      # tool root2hdf5 that can be installed with pip and/or is available via github     
      hnames = [key for key in rf.keys() if "TH1" in rf[key].classname or "TH2" in rf[keys].classname]
      # one can test it by printing selected elements of the hname array
      #print(hnames[2:4])
      
      with h5py.File(args.HDF5Output, "w" ) as hf:
         for hn in hnames:
            hist = rf[hn]
	    # create a group for each histogram in the HDF5 file
	    # as opposed to creating separate datasets for values, errors, bin edges, etc. 
            hgroup = hf.create_group(hn)
	    # add histogram title, if desired
            hgroup.attrs["title"] = hist.title
	    # now create datasets within each group
            hgroup.create_dataset("values", data=hist.values())
            hgroup.create_dataset("errors", data=hist.variances())
	    
            if "TH1" in hist.classname:
	       # now create datasets within each group
               hgroup.create_dataset("edges", data=hist.axis().edges()) # one can also do just hist.edges()
            elif "TH2" in hist.classname:
               hgroup.create_dataset("xedges", data=hist.axes[0].edges()) # one can also do just hist.edges()
               hgroup.create_dataset("yedges", data=hist.axes[1].edges()) # one can also do just hist.edges()

   hf.close()

# main(sys.argv)

# this is the actual call to main
# it is needed because python does NOT automatically execute main
# one can do just main(sys.argv) (see above), or sys.exit(main(sys.argv[1:])), 
# or the way it is done below
# the check for __main__ prevents main from being called if the file
# is being imported as opposed to being executed
if __name__ == "__main__":
    exit(main(sys.argv[1:]))
