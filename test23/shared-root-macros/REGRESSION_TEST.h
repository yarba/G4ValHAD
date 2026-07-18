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

// std::string Versions[4] = { "geant4-10-04-patch-03", "geant4-10-05", "geant4-10-06-patch-03",
//                            "geant4-10-07-patch-02" };
// std::string Versions[4] = { "geant4-10-07-patch-02", "geant4-11-00-patch-02", 
//                            "geant4-11-01", "geant4-11-02-patch-01" };
// --> Jan 2025 --> 
//std::string Versions[4] = { "geant4-11-01", "geant4-11-02-patch-01", /* "geant4-11-03", */ 
//                              "geant4-11-03-patch-02", "11.3.p02.bert.11.2"}; // 
// Jan 2026 -->
std::string Versions[3] = { "geant4-11-02-patch-01", "geant4-11-03", "geant4-11-04" };

// for Geant4 Spring 2025 Talk Series
//std::string Versions[4] = { "geant4-10-06-patch-03",
//                            "geant4-10-07-patch-02",
//			    "geant4-11-00-patch-02",
//			    "geant4-11-01" };
//std::string Versions[3] = { "geant4-11-01", "geant4-11-02-patch-01", "geant4-11-03" };

// for MR5264
// std::string Versions[2] = { "bertini_11.3.r08", "bertini_MR5264" };
// std::string Versions[2] = { "bertini11.2", "bertini11.2_MR5264" };

// std::string CurrentVersion = "11.3.p02.bert.11.2"; // "geant4-11-03"; // "geant4-11-02-patch-01";
std::string CurrentVersion = "geant4-11-04"; 

// --> int ColorVersion[5] = { kRed, kGreen, 7, kBlue /* kBlack */ , 14 };
// this is to match SASM6E
// --> int ColorVersion[5] = { kRed, kGreen, kBlue /* kBlack */ , kMagenta, 7 };
// int ColorVersion[5] = { kBlue /* kBlack */ , kRed, kGreen, kMagenta, 7 };
//                      11.1    11.2.p01           11.3.p02     11.3.p02.bert.11.2
// --> int ColorVersion[4] = { kGreen, kMagenta, /* 7, */ 14,          43 };
//
// MR5264
// int ColorVersion[2] = { kRed, kGreen };
int ColorVersion[3] = { 7, kMagenta, 14 };
//
// int ColorVersion[4] = { kMagenta, 7, kGreen, kRed };
int SymbVersion[5]     = { 20, 21, 34, 29, 23 };

// LQ 
// --> std::string regre_test_dir = " /lustre1/g4/yarba_j/g4-had-validation/regression-test-files";
// NERSC
std::string regre_test_dir = " /global/cfs/cdirs/m4599/Users/yarba/geant4/hadronic-models/g4-had-validation/regression-test-files";

#endif
