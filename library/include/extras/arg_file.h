// arg_file.h
// contains class arg_file, which can read a data file, inserting each uncommented
// line as an argument in the command line argument list being parsed by cmd_line.

#ifndef ARG_FILE_H
#define ARG_FILE_H

#include <fstream>
#include "extras/cmd_line.h"

struct arg_file : public command_line_parser, public state_display {

  void got_here(int i) { cerr << "*** " << prompt_1 << ", Got Here " << i << endl; }

  int params();
  bool parse(int & i, int argc, const char ** argv);
  void help_1(ostream & s);
  void help_2(ostream & s);
  void display_state(ostream & s);

  arg_file(const char * prompt_string_1 = 0, const char * prompt_string_2 = 0)
    : delim("#!"), file_name(0), 
    prompt_1(prompt_string_1), prompt_2(prompt_string_2),
    new_argv(0)
  { }

  arg_file(const command_line_parser & clp, const state_display & sd,
	   const char * prompt_string_1 = 0, const char * prompt_string_2 = 0)
    : command_line_parser(clp), state_display(sd),
      delim("#!"), file_name(0), 
      prompt_1(prompt_string_1), prompt_2(prompt_string_2),
      new_argv(0)
  { }

  arg_file(const command_line_parser & clp,
	   const char * prompt_string_1 = 0, const char * prompt_string_2 = 0)
    : command_line_parser(clp),
      delim("#!"), file_name(0), 
      prompt_1(prompt_string_1), prompt_2(prompt_string_2),
      new_argv(0)
  { }

  ~arg_file()
  { delete [] new_argv; }

  // String containing the comment delimiter chars. The default chars
  // are '#' and '!'.
  string delim;
  bool comment(string & s); // returns true if s[0] is in delim

  // The file name (or 0 if no file)
  const char * file_name;

  // The help prompts
  const char *prompt_1, *prompt_2;

private:
  ifstream file;
  list <string> lines;
  const char ** new_argv;

  // hide copy constructor and operator =
  arg_file(const arg_file &);
  arg_file & operator=(const arg_file &);

};

int arg_file::params()
{
  int n = 1;  // only a single file name is needed for ALL REMAINING ARGUMENTS 

  // get the number of params for all subsequent command_line_parsers and
  // reduce n by this number, so the n + remaining = 1:
  command_line_parser::pobj i = my_cmd_list_pos;
  for (++i; i != cmd_list.end(); ++i)
    n -= (*i)->params();
  return n;
}

void arg_file::help_1(ostream & s)
{
  if(prompt_1)
    s << " <" << prompt_1 << ">";
  else
    s << " <" << "arg file" << ">";
}
  
void arg_file::help_2(ostream & s)
{
  s << " *";
  help_1(s);
  s << ": ";
  if(prompt_2)
    s << prompt_2 << endl;
  else
    s << " name of file containing command line argument values." << endl;
  s << "   Enter \"-\" for the file name if no file is available." << endl;
}

void arg_file::display_state(ostream & s)
{
  if (file_name)
    s << state_display::comment 
      << "Argument values read from file " << file_name << endl;
}

bool arg_file::parse(int & i, int argc, const char ** argv)
{
  if(++i >= argc)
    return false;

  file_name = argv[i];

  // check for a "-" indicating no file data
  if(file_name[0] == '-' && file_name[1] == '\0') {
    file_name = 0;  // null pointer
    return true;
  }
 
  // open the file...
  file.open(file_name);
  if(!file) {
    error::warning("arg_file: could not open", file_name);
    return false;
  }

  // read in the lines from the file into a container of strings
  while(file) {
    string temp;
    file >> temp;  // skips leading whitespace
    if(!comment(temp) && temp.length() != 0) lines.push_back(temp);
    getline(file, temp); // skip to next line of input
  }
  file.close();

  // If necessary, create a new argv array, inserting data from
  // the file.
  if (lines.size() == 0) return true; // nothing need be done
  int new_argc = lines.size() + argc - i;
  new_argv = new (const char*)[new_argc];
  new_argv[0] = argv[0];
  int j = 1;
  for(list<string>::iterator n = lines.begin(); n != lines.end(); ++n, ++j)
    new_argv[j] = (*n).c_str();
  for(int k = i+1; k < argc; ++k, ++j) 
    new_argv[j] = argv[k];

  // Now use new_argv to read the remaining command line arguments
  const char * const alert = "= = = = = = = = = = = = = = = ="
    " = = = = = = =\n";
  j = 0;
  for(cmd_list_pos = my_cmd_list_pos, ++cmd_list_pos;
      cmd_list_pos != cmd_list.end();) {
    if(!((*cmd_list_pos++)->parse(j, new_argc, new_argv)) ) {
      // some error occured when parsing
      if(j >= new_argc) {
	// ran out of command line arguments
	i = argc; return false;
      }
      else {
	// an error interpreting an argument.
	// Was it in the file or in the remaining "real" command line?
	if (unsigned(j) <= lines.size()) {
	  error::stream() << alert
			  << "I don't understand argument line " << j
			  << ": " << new_argv[j] << " read from file "
			  << file_name << endl;
	  return false;
	}
	else {
	  i += j - lines.size();
	  return false;
	}
      }
    }
  } // for
  // check for too many arguments in the file...
  if (unsigned(j) < lines.size()) {
    error::stream() << alert
		    << "Too many arguments in file " << file_name << endl
		    << "File argument " << j+1 << ": " << new_argv[j+1] << endl
		    << "does not seem to be needed." << endl;
    return false;
  }
  else {
    i += j - lines.size();
    return true;
  }
}

bool arg_file::comment(string & s)
{ return delim.find(s[0]) != string::npos; }

#endif /* ARG_FILE_H */     
