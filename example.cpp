#include <cmdparser.h>
using namespace std;

int main(int argc, char* argv[]) {

  CmdParser cmdParser(argc, argv);

  cmdParser
    .addGroup("Generic options:")
    .add("-f", "input filename")
    .add("-o", "output filename");

  cmdParser
    .addGroup("Other options:")
    .add("--verbose", "log everythings while running program", false, "false")
    .add("--config", "filename of configuration", false, "config.txt");

  cmdParser
    .addGroup("Example: ./example -f a.txt -o b.txt --verbose");

  if ( !cmdParser.isOptionLegal() )
    cmdParser.showUsageAndExit();

  string a = cmdParser.find("-f");
  string b = cmdParser.find("-o");
  bool verbose = cmdParser.find("--verbose") == "true";
  string config_file = cmdParser.find("--config");

  if (verbose) {
    cout << "Loading file..." << endl;
    cout << "Processing file..." << endl;
    cout << "Saving file..." << endl;
  }

  return 0;
}
