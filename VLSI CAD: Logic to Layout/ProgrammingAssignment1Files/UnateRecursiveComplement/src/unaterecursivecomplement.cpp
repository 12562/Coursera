#include <boost/program_options.hpp>

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

namespace po = boost::program_options;
int main ( int argc, char* argv[] ) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("display-pcn,P", po::value<bool>()->default_value(false), "display PCN data")
        ("display-ascii-data,A", po::value<bool>()->default_value(false), "display ASCII data")
        ("display-cube-set,C", po::value<bool>()->default_value(false), "display cube set")
        ("write-out,W", po::value<bool>()->default_value(false), "write output file")
        ("out-dir,O", po::value<string>()->default_value("./"), "output file dir")
        ("debug,D", po::value<bool>()->default_value(false), "debug program")
        ("input-file,I", po::value<vector<string>>(), "input file")
    ;
    
    po::positional_options_description p;
    p.add("input-file", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).\
              options(desc).positional(p).run(), vm);
    po::notify(vm); 
    
    if (vm.count("help")) {
       cout << desc << "\n";
       return 1;
    }
    
    string out_dir;
    bool pcn, ascii, cubeset, debug, write_out;
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

    if ( vm.count("input-file")) {
       for ( auto file : vm["input-file"].as<vector<string>>() ) { 
           cout << "\n\n\n####" << file << "####" << endl;
           list<vector<pcn_vals>> inp_pcn_container;
           list<vector<pcn_vals>> inp_pcn_complement_container;
           list<vector<int>> inp_int_container;
           bool all_dont_care = false;
           int num_vars; unsigned long long int num_cubes;
           set<string> cube_set;
           if ( read_input(file, &num_vars, &num_cubes, &inp_pcn_container, &inp_int_container, &cube_set) ) { 
              if ( pcn || ascii || cubeset ) {
                 cout << "********************************************************************************\n";
                 cout << "Input data is as follows: \n";
              }
              display(num_vars, num_cubes, &inp_pcn_container, ascii, cout);
              display_pcn(&inp_pcn_container, "PCN cubes as per input data: \n", pcn);
              if ( check_if_adc(inp_pcn_container.size(), num_vars, &inp_pcn_container, cubeset, "Input cube set is: ") ) 
                 cout << "Input is ADC \n";
              if ( pcn || ascii || cubeset ) 
                 cout << "********************************************************************************\n";

              inp_pcn_complement_container = complement(&inp_pcn_container, num_vars, debug);

              if ( pcn || ascii || cubeset ) {
                 cout << "********************************************************************************\n";
                 cout << "Output data is as follows: \n";
              }
              display(num_vars, inp_pcn_complement_container.size(), &inp_pcn_complement_container, ascii, cout);
              if ( write_out ) {
                 ofstream filestream(out_dir + "/out." + file);
                 display(num_vars, inp_pcn_complement_container.size(), &inp_pcn_complement_container, write_out, filestream);
              }
              display_pcn(&inp_pcn_complement_container, "Complement of input PCN cubes: \n", pcn);
              if ( check_if_adc(inp_pcn_complement_container.size(), num_vars, &inp_pcn_complement_container, cubeset, "Output cube set is: ") ) 
                 cout << "Output is ADC \n";
              if ( pcn || ascii || cubeset)
                 cout << "********************************************************************************\n";
           } 
       }
    }
    return 0;
}
