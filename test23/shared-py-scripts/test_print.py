#!/usr/bin/python

import sys
import argparse
import numpy as np
import os
import uproot
import h5py

def main():
    print("TEST PROCESSING")

# main()

# this is the actual call to main
# it is needed because python does NOT automatically execute main
# one can do just main() (see above), or sys.exit(main()), 
# or the way it is done below
# the check for __main__ prevents main from being called if the file
# is being imported as opposed to being executed
if __name__ == "__main__":
    exit(main())
