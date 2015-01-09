#ifndef _CMD_PARSER_H
#define _CMD_PARSER_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

class CmdParser {

  private:

    class AutoType {
      public:

	AutoType(std::string str): _str(str) {}
	operator std::string () { return _str; }
	operator int    () { return str2int(_str); }
	operator size_t () { return str2int(_str); }
	operator float  () { return str2float(_str); }
	operator double () { return str2double(_str); }
	operator bool   () { 
	  assert(_str == "true" || _str == "false");
	  return _str == "true";
	}

      private:
	std::string _str;
    };
  
  public:
    CmdParser(int argc, char** argv): _argc(argc), _argv(argv), _usage(""), _options("") {
      _usage = "Usage: " + std::string(_argv[0]) + " ";
    }

    CmdParser& add(std::string option, bool mandatory = true) {
      assertValidOption(option);

      static size_t counter = 0;
      std::string description = option;
      option = int2str(++counter);
      _appendArugmentToUsage(!mandatory, description);

      Arg arg(option, description, !mandatory, "");
      _arguments[option] = arg;

      return *this;
    }

    CmdParser& add(std::string option, const char* description, bool mandatory = true) {
      assertValidOption(option);

      bool optional = !mandatory;
      Arg arg(option, description, optional, "");
      _arguments[option] = arg;
      _options += arg.getDescription();
      _appendOptionsToUsage(optional, option);

      return *this;
    }

    CmdParser& add(std::string option, const char* description, const char* defaultArg) {
      assertValidOption(option);

      Arg arg(option, description, true, defaultArg);
      _arguments[option] = arg;
      _options += arg.getDescription();
      _appendOptionsToUsage(true, option);

      return *this;
    }

    CmdParser& addGroup(std::string description) {
      _options += "\n" + description + "\n";
      return *this;
    }

    AutoType operator[] (int argNumber) const {
      return find(int2str(argNumber));

    }

    AutoType operator[] (std::string option) const {
      return find(option);
    }

    AutoType find(std::string option) const {
      std::map<std::string, Arg>::const_iterator itr = _arguments.find(option);
      assert(itr != _arguments.end());
      return itr->second.parameter;
    }

    void showUsageAndExit() const {
      std::cout << std::endl << _usage << " [options]" << std::endl << _options << std::endl;
      exit(-1);
    }

    void printArgs() const {
      for (int i=0; i<_argc; ++i)
	std::clog << _argv[i] << " ";
      std::clog << std::endl;
    }

    bool isOptionLegal() {
      if ( this->_lookingForHelp() )
	this->showUsageAndExit();

      return this->parse();
    }
    
    void showAll() {
      for (std::map<std::string, Arg>::iterator i=_arguments.begin(); i != _arguments.end(); ++i) {
	const std::string& opt = i->first;
	const std::string& parm = i->second.parameter;

	if (isNumber(opt))
	  printf("argument #%d:	%s\n", str2int(opt), parm.c_str());
	else
	  printf("option %s: %s\n", opt.c_str(), parm.c_str());
      }
    }

  private:

    struct Arg {
      Arg() {}

      Arg(std::string opt, std::string des, bool o, std::string darg) {
	option = opt;
	optional = o;
	default_arg = darg;
	description = replace_all(des, "\n", "\n" + getPadding());
	parameter = default_arg;
      }

      std::string _optionStr() const {
	std::string opt = "  " + option;
	opt.resize(PADDING_RIGHT, ' ');
	opt += "\t";
	return opt;
      }

      std::string getPadding() const {
	std::string padding;
	padding.resize(PADDING_RIGHT, ' ');
	padding += "\t";
	return padding;
      }

      std::string _defaultArgStr() const {
	if (optional && default_arg != "")
	  return getPadding() + "\33[1;30m(default = " + default_arg + ")\33[0m\n";

	return "";
      }

      std::string getDescription() const {
	return _optionStr() + description + "\n" + _defaultArgStr();
      }

      std::string getDefaultArg() const {
	return default_arg;
      }

      static const size_t PADDING_RIGHT = 24;

      std::string option;
      std::string description;
      bool optional;
      std::string default_arg;
      std::string parameter;
    };

    bool parse() {

      size_t counter = 0;

      for(int i=1; i<_argc; ++i) {
	std::string arg(_argv[i]);

	size_t pos = arg.find_first_of('=');
	std::string left = arg.substr(0, pos);
	std::string right = arg.substr(pos + 1);

	if (pos != std::string::npos && isValidOption(left) ) {
	  if (!has(left))
	    return unknown(left);

	  if (right.empty())
	    return miss(_arguments[left]);

	  _arguments[left].parameter = right;
	}
	else {

	  if (has(arg)) {
	    if (i+1 >= _argc)
	      return miss(_arguments[arg]);
	    _arguments[arg].parameter = _argv[++i];
	  }
	  else {
	    if (!has(counter + 1))
	      return unknown(arg);

	    _arguments[int2str(counter + 1)].parameter = arg;
	    ++counter;
	  }
	}
      }

      std::map<std::string, Arg>::iterator itr = _arguments.begin();
      for (; itr != _arguments.end(); ++itr) {
	const Arg& arg = itr->second;
	if (arg.parameter.empty() && !arg.optional)
	  return miss(arg);
      }

      return true;
    }

    bool isValidOption(const std::string& option) const {
      // No space in option.
      // Ex: --filename=a.txt, --file-name=a.txt both OK, But --file name=a.txt
      // is outrageous. whitespace in "--file name" is not valid => return false
      return option.find(' ') == std::string::npos;
    }

    void assertValidOption(const std::string& option) const {
      if (!isValidOption(option))
	throw std::runtime_error("\33[31m[Error]\33[0m Invalid option \"" 
	    + option + "\". Don't use whitespace in option.\n");
    }

    bool miss(const Arg& arg) const {
      const std::string ERR_MSG = "\33[31m[ERROR]\33[0m ";
      if (isNumber(arg.option))
	std::cerr << ERR_MSG << "Missing argument " + arg.description << std::endl;
      else
	std::cerr << ERR_MSG << "Missing argument after " + arg.option << std::endl;
      return false;
    }

    bool unknown(std::string opt) {
      std::cerr << "Unknown option " + opt << std::endl;
      return false;
    }

    bool has(std::string arg) {
      return _arguments.count(arg) > 0;
    }

    bool has(int n) {
      return _arguments.count(int2str(n)) > 0;
    }

    bool _lookingForHelp() const {
      std::string help("--help");
      for (int i=1; i<_argc; ++i) {
	if (help.compare(_argv[i]) == 0)
	  return true;
      }
      return false;
    }

    void _appendArugmentToUsage(bool optional, const std::string& option) {
      if (!optional)
	_usage += " <" + option + ">";
      else
	_usage += " [" + option + "]";
    }

    void _appendOptionsToUsage(bool optional, const std::string& option) {
      if (!optional)
	_usage += " <" + option + ">";
    }

    // ===== Static Utility Functions =====
    static bool isNumber(const std::string& s) {

      if (s.empty())
	return false;

      size_t start = (s[0] == '+' || s[0] == '-') ? 1 : 0;

      std::string::const_iterator it = s.begin() + start;
      while (it != s.end() && std::isdigit(*it))
	++it;

      return it == s.end();
    }

    static bool isFloat( std::string myString ) {
      std::istringstream iss(myString);
      float f;
      iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
      // Check the entire string was consumed and if either failbit or badbit is set
      return iss.eof() && !iss.fail(); 
    }

    static std::string int2str(int n) {
      std::stringstream ss;
      ss << n;
      return ss.str();
    }

    static std::string replace_all(const std::string& str, const std::string &token, const std::string &s) {
      std::string result(str);
      size_t pos = 0;
      while((pos = result.find(token, pos)) != std::string::npos) {
	result.replace(pos, token.size(), s);
	pos += s.size();
      }
      return result;
    }
    
    static int str2int(std::string str) {
      assert(isNumber(str));
      return ::atoi(str.c_str());
    }

    static float str2float(std::string str) {
      assert(isFloat(str));
      return ::atof(str.c_str());
    }

    static double str2double(std::string str) {
      assert(isFloat(str));
      return ::atof(str.c_str());
    }

    int _argc;
    char** _argv;

    std::string _usage;
    std::string _options;
    std::map<std::string, Arg> _arguments;
};

#endif // _CMD_PARSER_H
