
// This script is intended to be called by a ~/.rootrc file
// Copied directly from Ben's 'rootlogon_grid.C'

{
  std::cout << " rootlogon.C FOR GRID " << std::endl;
  
  if( gSystem->Getenv("PLOTUTILSROOT") )
  {
    string newpath = string(gROOT->GetMacroPath()) + ":" + string("${PLOTUTILSROOT}/../bin" );
    gROOT -> SetMacroPath( newpath.c_str() );
    
    gInterpreter -> AddIncludePath("${PLOTUTILSROOT}/../include");
    gInterpreter -> AddIncludePath("${PLOTUTILSROOT}/../include/PlotUtils");
    
    std::vector<std::string> packages = {"MAT", "MAT-MINERvA"};
    for( const std::string& package: packages )
    {
      gSystem -> Load(gSystem->ExpandPathName(("$PLOTUTILSROOT/lib" + package + ".so").c_str()));
    }
  }
}
