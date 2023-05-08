#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <tuple>
#include <list>
#include <vector>
#include <regex>

using namespace std;

#include "pcn_vals_class.hpp"
#include "pcn_cubes_class.hpp"
#include "boolean_fn_array_class.hpp"
#include "cmds_class.hpp"

void cmds::get_file_map(bool *from_user, unsigned int *nfx) {
  if (*from_user) {
     unsigned int num_functions;
     cout << "\nEnter number of function file map pairs to enter: ";
     cin >> num_functions;
     for ( unsigned int i=0; i<num_functions; i++ ) {
         string num, file_name;
         cout << "\nEnter function number and corresponding file name for FX num = " << i << "\n";
         cout << "\nEnter function number: ";
         cin >> num;
         cout << "\nEnter file name: ";
         cin >> file_name;
         (this->_fileMAP_).insert(make_pair(num, file_name));
     }
  } else
     for ( unsigned int i=0; i<(*nfx); i++ ) {
         string num, file_name;
         num = to_string(i);
         file_name = num + ".pcn";
         (this->_fileMAP_).insert(make_pair(num, file_name));
     }
}

void cmds::parse_input(string file) {
  this->_inputFILE_ = file;
  this->_cmds_ = parse_input();
}

// Parses the input cmd file for commands; Frome the input file read, it creates command-options pair for every line in cmd file to create a list of command-option pairs. Also, resets the _FXnumSET_ and _INPfxNUMset_ variables;
list<pair<string, vector<string>>> cmds::parse_input() {
  ifstream fd (this->_inputFILE_); // Open input cmd file for reading
  string cmd, inp;
  (this->_FXnumSET_).clear();
  (this->_INPfxNUMset_).clear();

  list<pair<string, vector<string>>> cmd_opt_pair_lst;
  pair<string, vector<string>> cmd_opt_pair;
  regex exp("[0-9]+"); // Since, the cmd file is of the format: [alpha] [number]{0, } i.e. a number repeated zero or more times after an alphabet (command), we create a regex to find all numbers to create a list of options.
  smatch match;
  while ( getline(fd, inp) ) { // While there are lines in the input file, do the following:
    cmd = inp[0];              //    Set the first character in the input string as the command 
                               //    Possible commands are '&', '!', '+', 'p', 'r', 'q'
    vector<string> opts = {};
    while ( regex_search(inp, match, exp) ) {
       int pos = 0;
       for ( auto x: match ) { //    For each match of number(read as string), add it to the vector of 
                               //    strings to create a vector of options.
           opts.push_back(x);
           (this->_FXnumSET_).insert(x); // Add each number to the set of function numbers (_FXnumSET_). 
           if ( cmd == "r" ) 
              (this->_INPfxNUMset_).insert(x); // For each 'r' command, add corresponding number as string
                                               // to the set of input function numbers (_INPfxNUMset_)
           pos++;
       }
       inp = match.suffix().str();
    }
    cmd_opt_pair = make_pair(cmd, opts); // Create a pair of command and options.
    (cmd_opt_pair_lst).push_back(cmd_opt_pair); // Add the command-option pair to the the global list.
  }
  fd.close();
  return cmd_opt_pair_lst;
}

// If user specified values for nvars and nfx are given then verify if they are same as file read values
// If user specified values for nvars and nfx are not given then assume it to be file read values
bool cmds::get_and_verify_bfa_inps(unsigned int *nvars, unsigned int *nfx, string *inDIR) {
  if ( *nvars == 0 ) {
     ifstream infile (*inDIR + "/" + (this->_fileMAP_)[*((this->_INPfxNUMset_).begin())]);
     infile >> *nvars;
     infile.close();
  }
  if ( *nfx == 0 )
     *nfx = (this->_FXnumSET_).size();

  unsigned int file_nfx;
  set<unsigned int> nvars_set;
  unsigned int num_vars;
  file_nfx = (this->_FXnumSET_).size();
  for ( auto it : this->_INPfxNUMset_ ) {
      ifstream infile ((this->_fileMAP_)[it]);
      infile >> num_vars;
      (nvars_set).insert(num_vars);
      infile.close();
  }

  bool rvalue = true;
  if ( file_nfx != *nfx ) {
     cout << "Number of functions in input file (" << file_nfx << ") is different from user specified number (" << *nfx << "). Please correct!!\n";
     rvalue = false;
  } else if ( nvars_set.size() > 1 ) {
     cout << "Number of variables is different in .pcn files . Please correct!!\n";
     rvalue = false;
  } else if ( nvars_set.find(*nvars) == nvars_set.end() ) {
     cout << "Number of variables in .pcn files is different from user specified number. Please correct!!\n";
     rvalue = false;
  } else {
     rvalue = true;
  }
  return rvalue;
}

map<string, tuple<string, bool, pcn_cubes>> cmds::calculate_FXlst(string *outDIR, string *inDIR) {
  map<string, tuple<string, bool, pcn_cubes>> m;
  pcn_cubes empty(this->get_nvars(), 0);
  for ( auto it : this->_FXnumSET_ ) {
    (this->_fileMAP_)[it] = ((this->_fileMAP_).find(it) == (this->_fileMAP_).end())?it + ".pcn":(this->_fileMAP_)[it];
    bool is_input_file;

    if ( (this->_INPfxNUMset_).find(it) != (this->_INPfxNUMset_).end() ) {
       is_input_file = true;
       m.insert(make_pair(it, make_tuple(*inDIR + "/" + (this->_fileMAP_)[it], is_input_file, empty)));
    } else {
       is_input_file = false;
       m.insert(make_pair(it, make_tuple(*outDIR + "/" + (this->_fileMAP_)[it], is_input_file, empty)));
    }
  }

  return m;
}

cmds::cmds() {
  cout << "Constructor called!!" << endl;
  this->_inputFILE_ = "cmd1.txt";
  this->_cmds_ = parse_input();
}

void cmds::set_nfx_nvars(unsigned int nfx, unsigned int nvars, string *outDIR, string *inDIR, bool *from_user) {
  cout << "###############################################################\n";
  nfx = (this->_FXnumSET_).size();
  nvars = 0;
  get_file_map(from_user, &nfx);
  if ( ! get_and_verify_bfa_inps(&nvars, &nfx, inDIR) ) { // (bfa => boolean_fn_array) Get and Verify boolean function array parent class variables
     cout << "Correct previous errors!! \n\n";
     exit(1);
  } else {
     this->set_nfx(nfx); // Set parent class private variable (_nfx_)
     this->set_nvars(nvars); // Set parent class private variable (_nvars_)
     this->set_FXlst(calculate_FXlst(outDIR, inDIR)); // Calculate and Set parent class private variable _FXlst_
  }
}

void cmds::set_global_flags(bool *write_out, bool *ascii, bool *pcn, bool *cubeset, bool *debug, string *i_o) {
  this->set_writeOUT(write_out);
  this->set_ASCII(ascii);
  this->set_PCN(pcn);
  this->set_cubeSET(cubeset);
  this->set_DEBUG(debug);
  this->set_io(i_o);
}

void cmds::solve() {
  cout << "===> Solving " << this->_inputFILE_ << "!!\n";
  typedef bool (boolean_fn_array::*fn)(vector<string>);
  map<string, fn> m({
     {"r", &boolean_fn_array::read_input},
     {"!", &boolean_fn_array::complement},
     {"&", &boolean_fn_array::a_n_d},
     {"+", &boolean_fn_array::o_r},
     {"p", &boolean_fn_array::print},
     {"q", &boolean_fn_array::quit}
  });
  for ( auto it : this->_cmds_ )
      if ( ! (this->*m[it.first])(it.second) ) {
         cout << "\n\n Correct previous errors !! \n\n";
         break;
      }
}


void cmds::display_cmd_file() {
     for ( auto it : this->_cmds_ ) {
         cout << it.first << " ";
         for ( auto opts : it.second )
             cout << opts << " ";
         cout << endl;
     }
}
