#!/usr/bin/env python
import sys
import argparse
import os

# Usage: python check_envvar.pt --Input $G4ValHAD --Output $PWD
# NOTE-1: it'll automatically extend both env.vars when printing out
# NOTE-2: one can also use os.getenv()

def parse_args(argv):
    p = argparse.ArgumentParser()
    p.add_argument("--Output", type=str, default="")
    p.add_argument("--Input", type=str, default="")
    args = p.parse_args()
    return args

def main(argv):
   args = parse_args(argv)
   print(f"Input: {args.Input}")
   print(f"Output: {args.Output}")
   test_g4install = os.getenv("G4INSTALL")
   print(f"G4INSTALL: {test_g4install}")
   
# main(sys.argv)

# this is the actual call to main
# it is needed because python does NOT automatically execute main
# one can do just main(sys.argv) (see above), or sys.exit(main(sys.argv[1:])), 
# or the way it is done below
# the check for __main__ prevents main from being called if the file
# is being imported as opposed to being executed
if __name__ == "__main__":
    exit(main(sys.argv[1:]))
