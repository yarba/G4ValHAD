//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#ifndef G4VAL_REGRESSION_H
#define G4VAL_REGRESSION_H

const int NVersions = 3;

std::string Versions[3] = { "geant4-10-04-patch-03",
                            "geant4-10-06",
			    "geant4-10-07-patch-01"
                          };
// std::string Versions[4] = { "geant4-09-06-patch-04", 
//                             "geant4-10-01-patch-03", 
//                             "geant4-10-02-patch-03", 
//                             "geant4-10-03-patch01" };

std::string CurrentVersion = "geant4-10-07-patch-01";

// --> int ColorVersion[5] = { kRed, kGreen, 7, kBlack, 14 };
// this is to match SASM6E
int ColorVersion[5] = { kRed, kGreen, kBlack, kMagenta, 7 };
// int ColorVersion[4] = { kMagenta, 7, kGreen, kRed };
int SymbVersion[5]     = { 20, 21, 34, 29, 23 };

// std::string regre_test_dir = " /scratch-shared/g4p/pbs/g4-had-validation/regression-test-files";
// std::string regre_test_dir = " /g4/g4p/pbs/g4-had-validation/regression-test-files";
//
std::string regre_test_dir = " /work1/g4v/yarba_j/geant4-regression-test-files";

#endif
