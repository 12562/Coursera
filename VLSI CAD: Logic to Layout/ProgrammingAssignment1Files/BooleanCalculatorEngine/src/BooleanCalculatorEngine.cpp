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
#include <map>

#include "pcn_vals_class.hpp"
#include "pcn_cubes_class.hpp"
#include "boolean_fn_array_class.hpp"
#include "cmds_class.hpp"
using namespace std;

namespace po = boost::program_options;
int main ( int argc, char* argv[] ) {

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("display-pcn,P", po::value<bool>()->default_value(false), "Display PCN data")
        ("display-ascii-data,A", po::value<bool>()->default_value(false), "Display ASCII data")
        ("display-cube-set,C", po::value<bool>()->default_value(false), "Display cube set")
        ("display-cmd-file,F", po::value<bool>()->default_value(false), "Display cmd file")
        ("display-in-out-inout-none,D", po::value<string>()->default_value("none"), "Display either only input or output or both or none (Valid values are in/out/both/none)")
        ("write-out,W", po::value<bool>()->default_value(true), "write output file")
        ("in-dir,I", po::value<string>()->default_value("."), "input file dir")
        ("out-dir,O", po::value<string>()->default_value("."), "output file dir")
        ("debug,B", po::value<bool>()->default_value(false), "debug program")
        ("input-cmd-file,M", po::value<vector<string>>(), "input command files")
        ("enter-fn-file-map,E", po::value<bool>()->default_value(false), "Enter function number and file map")
        ("num-vars,V", po::value<unsigned int>()->default_value(0), "Number of variables in the boolean functions (default:6)")
        ("num-fx,X", po::value<unsigned int>()->default_value(0), "Number of boolean functions to operate on (default:1)")
    ;
    
    po::positional_options_description p;
    p.add("input-cmd-file", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm); 
    
    if (vm.count("help")) {
       cout << desc << "\n";
       return 1;
    }
    
    unsigned int num_vars = 0, num_fx = 0;
    string out_dir, in_dir, i_o = "none";
    bool pcn, ascii, cubeset, cmd, debug, write_out, from_user;
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
    
    if (vm.count("display-in-out-inout-none") )
       i_o = vm["display-in-out-inout-none"].as<string>();

    if (vm.count("in-dir") )
       in_dir = vm["in-dir"].as<string>();

    if (vm.count("display-cmd-file") )
       cmd = vm["display-cmd-file"].as<bool>();    
    
    if (vm.count("enter-fn-file-map") )
       from_user = vm["enter-fn-file-map"].as<bool>();

    if ( vm.count("num-vars") )
       num_vars = vm["num-vars"].as<unsigned int>();
 
    if ( vm.count("num-fx") )
       num_fx = vm["num-fx"].as<unsigned int>();

    cmds inst;
    inst.set_global_flags(&write_out, &ascii, &pcn, &cubeset, &debug, &i_o );
    for ( string file : vm["input-cmd-file"].as<vector<string>>() ) { 
        cout << "\n\n\n####" << file << "####" << endl;
        inst.parse_input(file);
        inst.set_nfx_nvars(num_fx, num_vars, &out_dir, &in_dir, &from_user); 
        if ( cmd )
           inst.display_cmd_file();
        inst.solve();
    }
    return 0;
}
