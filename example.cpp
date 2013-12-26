#include <cmdparser.h>
#pragma GCC diagnostic ignored "-Wunused-variable"
using namespace std;

int main(int argc, char* argv[]) {

  CmdParser cmdParser(argc, argv);

  cmdParser.addGroup("Generic options:")
    .add("input-file")
    .add("output-file", false)
    .add("--type", "choose one of the following type of algorithm\n"
	"0 -- quicksort\n"
	"1 -- shellsort\n"
	"2 -- bubblesort", "0");

  cmdParser
    .addGroup("Other options:")
    .add("--verbose", "log everythings while running program", "false")
    .add("--config", "filename of configuration");

  cmdParser
    .addGroup("Example: ./example -f a.txt -o b.txt --verbose");

  if ( !cmdParser.isOptionLegal() )
    cmdParser.showUsageAndExit();

  string a = cmdParser[1];
  string b = cmdParser[2];
  size_t type = cmdParser["--type"];
  bool verbose = cmdParser["--verbose"];
  string config_file = cmdParser["--config"];

  //cmdParser.showAll();
  cout << a << endl;
  cout << b << endl;
  cout << type << endl;
  cout << verbose << endl;
  cout << config_file << endl;

  return 0;
}
