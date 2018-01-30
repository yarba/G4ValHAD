
#include "../../test23/shared-upload-scripts/SQLCompose.C"

// NOTE(JVY): "using namespace std" is declared in SQLCompose.C

void test75_gen_histo_sql( string g4version, string status )
{

   // 300 MeV gamma+Cu -> proton + X 
   //
   int nang1 = 4;
   string angles1[4] = { "45", "90", "135", "150" };
   
   for ( int i=0; i<nang1; ++i )
   {
      string hname = "p" + angles1[i] + "deg";
      string reaction_details = " (@" + angles1[i] + "deg)"; 
      SQLCompose( 75, "gamma", "Cu", "", "300MeV", "proton",
	          "Bertini", 
		  g4version,
		  hname,
		  reaction_details,
		  "proton kinetic energy [MeV]", "cross section [microbarn]",
		  "",
		  "cross section versus proton kinetic energy",
		  status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   }
   
   // 668MeV gamma+Cu -> pi+/- at 28.4 & 44.2 deg
   //
   string hname = "pim28deg";
   string reaction_details = " (@28.4deg)";
   SQLCompose( 75, "gamma", "Cu", "", "668MeV", "pi-",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi- momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi- momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   hname = "pip28deg";
   SQLCompose( 75, "gamma", "Cu", "", "668MeV", "pi+",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi+ momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi+ momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   hname = "pim44deg";
   reaction_details = " (@44.2deg)";
   SQLCompose( 75, "gamma", "Cu", "", "668MeV", "pi-",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi- momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi- momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   string hname = "pip44deg";
   SQLCompose( 75, "gamma", "Cu", "", "668MeV", "pi+",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi+ momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi+ momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   
   // 668MeV gamma+Pb -> pi+/- at 44.2deg
   //
   hname = "pim44deg";
   reaction_details = " (@44.2deg)";
   SQLCompose( 75, "gamma", "Pb", "", "668MeV", "pi-",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi- momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi- momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)
   string hname = "pip44deg";
   SQLCompose( 75, "gamma", "Pb", "", "668MeV", "pi+",
	       "Bertini", 
	       g4version,
	       hname,
	       reaction_details,
	       "pi+ momentum [MeV/c]", "cross section [microbarn]",
	       "",
	       "cross section versus pi+ momentum",
	       status, 1 ); // last arg mask=1 --> beam momentum (mask=0 --> energy)

   return;

}
