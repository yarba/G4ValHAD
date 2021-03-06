////////////////////////////////////////////////////////////////////////
//  gamma + Cu -> p + X (theta_proton = 45 degrees)
//
//  Data from: R. Schumacher et al., Phys. Rev. C 25, 2269 (1982).
//
// NOTE: Errors are statistical only.
//       Overall systematic error is +/-7%
//
////////////////////////////////////////////////////////////////////////

TGraphErrors* gCup45() {
  Float_t xp45[] = {47.0,  51.7,  56.9,  62.5,  68.7,  75.5,  82.9,  91.0, 99.9,
		    109.6, 120.2, 131.8, 144.4, 158.1, 173.0, 189.3, 206.9};
 
  Float_t yp45[] = {29.0, 30.0, 25.0, 33.0, 23.0, 15.5, 13.7, 10.7, 11.2,
		    11.0,  8.9,  7.4,  7.1,  5.5,  4.0,  3.4,  2.5};
 
  Float_t xep45[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  Float_t yep45[] = { 6.0, 4.0, 4.0, 4.0, 4.0, 1.6, 1.8, 1.1, 1.9,
		      0.6, 1.9, 0.9, 0.6, 0.3, 0.6, 0.3, 0.2};
  
  // take sys err into account
  //
  int np = sizeof(xp45)/sizeof(Float_t);
  for ( int i=0; i<np; ++i )
  {
     float tmp2 = yep45[i]*yep45[i];
     tmp2 += ((yp45[i]*0.07)*(yp45[i]*0.07));
     yep45[i] = sqrt(tmp2);
  }
  
  TGraphErrors* gp45 = new TGraphErrors(sizeof(xp45)/sizeof(Float_t),
					xp45,yp45,xep45,yep45);
  gp45->SetMarkerStyle(8);
  gp45->SetMarkerColor(kBlue);
  return gp45;
}

////////////////////////////////////////////////////////////////////////
//  gamma + Cu -> p + X (theta_proton = 90 degrees)
//
//  Data from: R. Schumacher et al., Phys. Rev. C 25, 2269 (1982).
////////////////////////////////////////////////////////////////////////

TGraphErrors* gCup90() {
  Float_t xp90[] = {47.0,  51.7,  56.9,  62.5,  68.7,  75.5,  82.9,  91.0, 99.9,
                     109.6, 120.2, 131.8, 144.4, 158.1, 173.0, 189.3, 206.9};

  Float_t yp90[] = {19.8, 14.0, 14.0, 11.6, 10.8, 10.4,  7.7,  6.8,  8.6,
                       4.9,  3.1,  3.3,  2.4,  1.9,  1.19, 0.65, 0.27};

  Float_t xep90[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  Float_t yep90[] = { 1.6, 1.2, 2.7, 1.8, 2.2,  0.9,  1.0,  0.6, 1.8,
                        1.1, 0.8, 0.2, 0.2, 0.16, 0.07, 0.04, 0.02};

  // take sys err into account
  //
  int np = sizeof(xp90)/sizeof(Float_t);
  for ( int i=0; i<np; ++i )
  {
     float tmp2 = yep90[i]*yep90[i];
     tmp2 += ((yp90[i]*0.07)*(yp90[i]*0.07));
     yep90[i] = sqrt(tmp2);
  }

  TGraphErrors* gp90 = new TGraphErrors(sizeof(xp90)/sizeof(Float_t),
					xp90,yp90,xep90,yep90);
  gp90->SetMarkerStyle(8);
  gp90->SetMarkerColor(kBlue);
  return gp90;
}

////////////////////////////////////////////////////////////////////////
//  gamma + Cu -> p + X (theta_proton = 135 degrees)
//
//  Data from: R. Schumacher et al., Phys. Rev. C 25, 2269 (1982).
////////////////////////////////////////////////////////////////////////

TGraphErrors* gCup135() {
  Float_t xp135[] = {47.0,  51.7,  56.9,  62.5,  68.7,  75.5,  82.9,  91.0,
                        99.9, 109.6, 120.2, 131.8, 144.4, 158.1, 173.0, 206.9};

  Float_t yp135[] = {11.6,  9.8, 10.5,  8.4,  9.9,  8.0,  8.5,   5.06,
                         3.31, 3.04, 2.28, 1.42, 1.04, 0.51, 0.288, 0.039};

  Float_t xep135[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  Float_t yep135[] = { 0.9, 1.4,  1.1,  0.8,  1.0,  0.5,  0.5,   0.26,
                         0.19, 0.23, 0.18, 0.07, 0.09, 0.03, 0.018, 0.003};

  // take sys err into account
  //
  int np = sizeof(xp135)/sizeof(Float_t);
  for ( int i=0; i<np; ++i )
  {
     float tmp2 = yep135[i]*yep135[i];
     tmp2 += ((yp135[i]*0.07)*(yp135[i]*0.07));
     yep135[i] = sqrt(tmp2);
  }

  TGraphErrors* gp135 = new TGraphErrors(sizeof(xp135)/sizeof(Float_t),
					xp135,yp135,xep135,yep135);
  gp135->SetMarkerStyle(8);
  gp135->SetMarkerColor(kBlue);
  return gp135;
}

TGraphErrors* gCup150() {

   Float_t xp150[] = { 47.0, 56.9, 68.7, 82.9, 99.9 };
   Float_t yp150[] = { 12.4,  7.2,  6.2,  4.9,  2.5 }; 
   Float_t xep150[] = { 0, 0, 0, 0, 0 };
   Float_t yep150[] = { 1.3, 0.5, 0.4, 0.3, 0.18 };

  // take sys err into account
  //
  int np = sizeof(xp150)/sizeof(Float_t);
  for ( int i=0; i<np; ++i )
  {
     float tmp2 = yep150[i]*yep150[i];
     tmp2 += ((yp150[i]*0.07)*(yp150[i]*0.07));
     yep150[i] = sqrt(tmp2);
  }

   TGraphErrors* gp150 = new TGraphErrors(sizeof(xp150)/sizeof(Float_t),
	 				  xp150,yp150,xep150,yep150);
   gp150->SetMarkerStyle(8);
   gp150->SetMarkerColor(kBlue);

   return gp150;
}
