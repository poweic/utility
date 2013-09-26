#include <cmdparser.h>
using namespace std;

int main(int argc, char* argv[]) {

  CmdParser cmdParser(argc, argv);

  cmdParser
    .addGroup("Generic options")
    .regOpt("-f", "input filename")
    .regOpt("-o", "output filename");

  cmdParser
    .addGroup("Other options")
    .regOpt("--verbose", "log everythings while running program")
    .regOpt("--config", "filename of configuration");


  if ( !cmdParser.isOptionLegal() )
    cmdParser.showUsageAndExit();

  return 0;
}
