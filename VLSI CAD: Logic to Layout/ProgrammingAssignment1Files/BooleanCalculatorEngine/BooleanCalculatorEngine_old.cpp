#include <boost/program_options.hpp>

#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <set>
#include <cmath>
#include "pcn_vals_class.hpp"
#include "display_functions.hpp"
#include "unaterecursivecomplement_helper.hpp"
using namespace std;

void parse_input(string file, list<pair<string, vector<string>>> *data) {
     ifstream fd(file);
     string cmd, inp;
     
     pair<string, vector<string>> cmd_opt_pair;
     regex exp("[0-9]+");
     smatch match;
     while ( getline(fd, inp) ) {
       cmd = inp[0];
       vector<string> opts = {};
       while ( regex_search(inp, match, exp) ) {
          int pos = 0;
          for ( auto x: match ) {
              opts.push_back(x);
              pos++;
          }
          inp = match.suffix().str();
       }
       cmd_opt_pair = make_pair(cmd, opts);
       (*data).push_back(cmd_opt_pair);
     }
}

void solve(list<pair<string, vector<string>>> * data) {
  map<string, list<vector<pcn_vals>>> boolean_fn_lst;
  map<string, function<int(vector<string>, map<string, list<vector<pcn_vals>>>*)>> m({
     {"r", read_input},
     {"!", complement},
//     {"&", a_n_d},
//     {"+", o_r},
//     {"p", print}, 
//     {"q", quit}
  });
  for ( auto it : *data )
      if ( m[it.first](it.second, &boolean_fn_lst) ) {
         cout << "\n\n Correct previous errors !! \n\n";
         break;
      }
  list<vector<pcn_vals>> ipc0 = boolean_fn_lst["0"];
  list<vector<pcn_vals>> ipc1 = boolean_fn_lst["1"];
  display_pcn(&ipc1, "input PCN cubes: \n", true);
  check_if_adc(ipc1.size(), 6, &ipc1, true, "Input cube set is: ");
  display_pcn(&ipc0, "Complement of input PCN cubes: \n", true);
  check_if_adc(ipc0.size(), 6, &ipc0, true, "Output cube set is: ");
}

namespace po = boost::program_options;
int main ( int argc, char* argv[] ) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("display-pcn,P", po::value<bool>()->default_value(false), "display PCN data")
        ("display-ascii-data,A", po::value<bool>()->default_value(false), "display ASCII data")
        ("display-cube-set,C", po::value<bool>()->default_value(false), "display cube set")
        ("display-cmd-file,F", po::value<bool>()->default_value(false), "display cmd file")
        ("write-out,W", po::value<bool>()->default_value(false), "write output file")
        ("out-dir,O", po::value<string>()->default_value("./"), "output file dir")
        ("debug,D", po::value<bool>()->default_value(false), "debug program")
        ("input-cmd-file,I", po::value<vector<string>>(), "input command files")
    ;
    
    po::positional_options_description p;
    p.add("input-cmd-file", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).\
              options(desc).positional(p).run(), vm);
    po::notify(vm); 
    
    if (vm.count("help")) {
       cout << desc << "\n";
       return 1;
    }
    
    string out_dir;
    bool pcn, ascii, cubeset, cmd, debug, write_out;
    if ( vm.count("display-pcn") )
       pcn = vm["display-pcn"].as<bool>();
    
    if ( vm.count("display-ascii-data") ) 
       ascii = vm["display-ascii-data"].as<bool>();
    
    if ( vm.count("display-cube-set") )
       cubeset = vm["display-cube-set"].as<bool>();

    if ( vm.count("debug") )
       debug = vm["debug"].as<bool>();

    if (vm.count("write-out") )
       write_out = vm["write-out"].as<bool>();
    
    if (vm.count("out-dir") )
       out_dir = vm["out-dir"].as<string>();
    
    if (vm.count("display-cmd-file") )
       cmd = vm["display-cmd-file"].as<bool>();    

    if ( vm.count("input-cmd-file")) {
       list<pair<string, vector<string>>> idata;
       for ( auto file : vm["input-cmd-file"].as<vector<string>>() ) { 
           cout << "\n\n\n####" << file << "####" << endl;
           parse_input(file, &idata);
           if ( cmd )
              display_cmd_file(&idata);
           solve(&idata);
           //list<vector<pcn_vals>> inp_pcn_container;
           //list<vector<pcn_vals>> inp_pcn_complement_container;
           //list<vector<int>> inp_int_container;
           //bool all_dont_care = false;
           //int num_vars; unsigned long long int num_cubes;
           //set<string> cube_set;
           //if ( read_input(file, &num_vars, &num_cubes, &inp_pcn_container, &inp_int_container, &cube_set) ) { 
           //   if ( pcn || ascii || cubeset ) {
           //      cout << "********************************************************************************\n";
           //      cout << "Input data is as follows: \n";
           //   }
           //   display(num_vars, num_cubes, &inp_pcn_container, ascii, cout);
           //   display_pcn(&inp_pcn_container, "PCN cubes as per input data: \n", pcn);
           //   if ( check_if_adc(inp_pcn_container.size(), num_vars, &inp_pcn_container, cubeset, "Input cube set is: ") ) 
           //      cout << "Input is ADC \n";
           //   if ( pcn || ascii || cubeset ) 
           //      cout << "********************************************************************************\n";

           //   inp_pcn_complement_container = complement(&inp_pcn_container, num_vars, debug);

           //   if ( pcn || ascii || cubeset ) {
           //      cout << "********************************************************************************\n";
           //      cout << "Output data is as follows: \n";
           //   }
           //   display(num_vars, inp_pcn_complement_container.size(), &inp_pcn_complement_container, ascii, cout);
           //   if ( write_out ) {
           //      ofstream filestream(out_dir + "/out." + file);
           //      display(num_vars, inp_pcn_complement_container.size(), &inp_pcn_complement_container, write_out, filestream);
           //   }
           //   display_pcn(&inp_pcn_complement_container, "Complement of input PCN cubes: \n", pcn);
           //   if ( check_if_adc(inp_pcn_complement_container.size(), num_vars, &inp_pcn_complement_container, cubeset, "Output cube set is: ") ) 
           //      cout << "Output is ADC \n";
           //   if ( pcn || ascii || cubeset)
           //      cout << "********************************************************************************\n";
           //} 
       }
    }
    return 0;
}
