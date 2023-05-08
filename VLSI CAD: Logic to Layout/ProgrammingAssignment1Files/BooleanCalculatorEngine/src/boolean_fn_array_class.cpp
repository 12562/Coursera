#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>

#include <set>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <bitset>
#include <fstream>
#include <sstream>


using namespace std;
#include "pcn_vals_class.hpp"
#include "pcn_cubes_class.hpp"
#include "boolean_fn_array_class.hpp"


boolean_fn_array::boolean_fn_array() {
  this->_nfx_ = 0;
  this->_nvars_ = 0;
  this->_FXlst_ = {};
}

boolean_fn_array::boolean_fn_array(unsigned int num_functions, vector<tuple<string, string, bool>> file_map) {
  this->_nfx_ = num_functions;
  set<int> nvar_set;
  for ( auto it: file_map ) {
      ifstream file ( get<1>(it) );
      unsigned int num_vars;
      unsigned long long int num_cubes;
      if ( file.is_open() ) {
         file >> num_vars;
         file >> num_cubes;
         file.close();
      } else {
         cout << "Unable to open input file: " << get<1>(it);
         exit(0); 
      }
      nvar_set.insert(num_vars);
      if ( nvar_set.size() == 1 ) {
         pcn_cubes pcn_cube_inst(num_vars, num_cubes);
         (this->_FXlst_).at(get<0>(it)) = make_tuple(get<1>(it), get<2>(it), pcn_cube_inst);
         this->_nvars_ = num_vars;
      } else {
         cout << "Number of input function vars is different for following file: " << endl;
         cout << "File: " << get<1>(it) << " Num Vars: " << num_vars << endl;
         exit(0); 
      }
  }
}

boolean_fn_array::boolean_fn_array(map<string, tuple<string,bool,pcn_cubes>> inp_map, unsigned int num_vars ) {
  this->_nfx_ = inp_map.size();
  this->_FXlst_ = inp_map;
  this->_nvars_ = num_vars;
}

void boolean_fn_array::set_nfx(unsigned int num) {
  this->_nfx_ = num;
}

void boolean_fn_array::set_nvars(unsigned int nvars) {
  this->_nvars_ = nvars;
}

unsigned int boolean_fn_array::get_nvars() {
  return this->_nvars_;
}

void boolean_fn_array::set_FXlst(map<string, tuple<string, bool, pcn_cubes>> m) {
  this->_FXlst_ = m;
}

void boolean_fn_array::set_writeOUT(bool *write_out) {
  this->_writeOUT_ = *write_out;
}

void boolean_fn_array::set_ASCII(bool *ascii) {
  this->_ASCII_ = *ascii;
}

void boolean_fn_array::set_PCN(bool *pcn) {
  this->_PCN_ = *pcn;
}

void boolean_fn_array::set_DEBUG(bool *debug) {
  this->_DEBUG_ = *debug;
}

void boolean_fn_array::set_io(string *i_o) {
  this->_IO_ = *i_o;
}

void boolean_fn_array::set_cubeSET(bool *cubeset) {
  this->_cubeSET_ = *cubeset;
}

string boolean_fn_array::get_filename(string num) {
  if ( this->_FXlst_.find(num) != this->_FXlst_.end() )
     return get<0>((this->_FXlst_).at(num));
  else 
     return num + ".pcn";
}

bool boolean_fn_array::read_input(vector<string> inp_lst) {
  string num = inp_lst[0];
  string file = this->get_filename(num);
  unsigned int num_vars; 
  unsigned long long int num_cubes; 
  stringstream data;
  cout << "* Reading input number: " << num << ", corresponding to file: " << file << "\n";

  ifstream infile ( file );
  if (infile.is_open()) {
     infile >> num_vars;
     infile >> num_cubes;
     data << infile.rdbuf();
     pcn_cubes container(num_vars, num_cubes, &data);
     (this->_FXlst_).at(num) = make_tuple(file, true, container);
     infile.close();
  } else {
     cout << "Unable to open file";
     return false;
  }
  return true;
}

/*
pcn_cubes boolean_fn_array::find_cofactor(pcn_cubes *ipc, int var_to_split_about, pcn_vals pos_or_neg ) {
    pcn_cubes cofactor(this->_nvars_,0);
    bool append;
    for (vector<pcn_vals> it : (*ipc).get_pcnLST()) {
        vector<pcn_vals> tmp = it;
        append = true;
        if ( it[var_to_split_about].get_value() == pos_or_neg.get_value() ) {
           tmp[var_to_split_about].set_value(3);
        } else if ( it[var_to_split_about].get_value() != pcn::dc ) {
           append = false;
        }
        if ( append )
           cofactor.add_cube(tmp);
    }
    return cofactor;
}

int boolean_fn_array::get_variable_to_split_about(pcn_cubes *ipc) {
    int nvars = (*ipc).get_nvars();
    pair<int,int> p(0,0);
    vector<pair<int,int>> extent_of_binate_or_unate(nvars, p);
    int pos;
    for (vector<pcn_vals> it : (*ipc).get_pcnLST()) {
        pos = 0;
        for (pcn_vals it2 : it ) {
            if ( it2.get_value() == pcn::nc )
               extent_of_binate_or_unate[pos].first++;
            else if ( it2.get_value() == pcn::c )
               extent_of_binate_or_unate[pos].second++;
            pos++;
        }
    }
    unsigned long long int min = numeric_limits<unsigned long long int>::max(), most_binate_max = 0, most_unate_max = 0;
    pos = 0;
    vector<int> track_binate_maxes, track_unate_maxes;
    for ( pair<int,int> vars : extent_of_binate_or_unate ) {
        if ( vars.first > 0 && vars.second > 0  ) {
           if ( vars.first + vars.second > most_binate_max ) {
              most_binate_max = vars.first + vars.second;
              min = abs(vars.first - vars.second);
              track_binate_maxes.clear();
              track_binate_maxes.push_back(pos);
           } else if ( (vars.first + vars.second == most_binate_max) ) {
              if ( abs(vars.first - vars.second) < min ) {
                 min = abs(vars.first - vars.second);
                 track_binate_maxes.clear();
                 track_binate_maxes.push_back(pos);
              } else if ( abs(vars.first - vars.second) == min ) {
                 track_binate_maxes.push_back(pos);
              }
           }
        } else if ( vars.first > 0 || vars.second > 0 ) {
           if ( vars.first + vars.second > most_unate_max ) {
              most_unate_max = vars.first + vars.second;
              track_unate_maxes.clear();
              track_unate_maxes.push_back(pos);
           } else {
              track_unate_maxes.push_back(pos);
           }
        }
        pos++;
    }
    if ( track_binate_maxes.size() > 0 ) {
       return track_binate_maxes[0];
    } else {
       return track_unate_maxes[0];
    }

}
 


pcn_cubes boolean_fn_array::comp(pcn_cubes *ipc, bool debug ) {
     pcn_cubes cc(this->_nvars_, 0);
     unsigned long long int ncubes = ((*ipc).get_pcnLST()).size();
     if ( ncubes == 0 ) {
        cc.add_cube((*ipc).generate_adc_vector());
     } else if ( (*ipc).check_if_adc() ) {
     } else if ( ((*ipc).get_pcnLST()).size() == 1 ) {
        vector<pcn_vals> tmp;
        vector<pcn_vals> dc_pcn = (*ipc).generate_adc_vector();
        for (vector<pcn_vals> it : (*ipc).get_pcnLST()) {
            int pos = 0;
            for (vector<pcn_vals>::iterator current=it.begin(); (current!=it.end()); ++current) {
                if ( (*current).get_value() != pcn::dc ) {
                   tmp = dc_pcn;
                   tmp[pos].set_pcn((*current).invert_term());
                   cc.add_cube(tmp);
                }
                pos++;
            }
        }
     } else {
        pcn_cubes P(this->_nvars_,0), N(this->_nvars_,0), pc(this->_nvars_,0), nc(this->_nvars_,0);
        int x = get_variable_to_split_about(ipc);
        pcn_vals positive(1), negative(2);
        if ( debug )
           cout << "Variable to split about : " << x << endl;
        pc = find_cofactor(ipc, x, positive);
        pc.display_pcn("Displaying positive cofactor: \n", cout);
        P = comp(&pc, debug);
        P.display_pcn("Displaying complement of positive cofactor: \n", cout);
        nc = find_cofactor(ipc, x, negative);
        nc.display_pcn("Displaying negative cofactor: \n", cout);
        N = comp(&nc, debug);
        N.display_pcn("Displaying complement of negative cofactor: \n", cout);
        P.and_x(x, &positive);
        P.display_pcn("Displaying P after and: \n", cout);
        N.and_x(x, &negative);
        N.display_pcn("Displaying N after and: \n", cout);
        cc = P.o_r(&N);
        cc.display_pcn("Displaying complement OR : \n", cout);
     }
     return cc;
}
*/

bool boolean_fn_array::complement(vector<string> inp_lst) {
  bool p_flag = false;
  string comment;
  string to_complement_fn_num = inp_lst[1];
  cout << "* Complementing " << to_complement_fn_num << "\n";
  pcn_cubes ipc = get<2>((this->_FXlst_).at(to_complement_fn_num));
  //display_inputs(&ipc);
  vector<vector<pcn_vals>> cc = ipc.complement();
  pcn_cubes comp(this->_nvars_, cc.size(), cc);
  comment = "result for: ! " + to_complement_fn_num;
  display_outputs(&comp, inp_lst[0], &p_flag, comment);
  string file = this->get_filename(inp_lst[0]);
  (this->_FXlst_).at(inp_lst[0]) = make_tuple(file, false, comp);
  return true;
}

bool boolean_fn_array::o_r(vector<string> inp_lst) {
  bool p_flag = false;
  string comment_in, comment_out;
  string or_inp_A = inp_lst[1], or_inp_B = inp_lst[2];
  cout << "* Oring : " << or_inp_A << " and " << or_inp_B << "\n";
  pcn_cubes ipcA = get<2>((this->_FXlst_).at(or_inp_A));
  pcn_cubes ipcB = get<2>((this->_FXlst_).at(or_inp_B));
  //display_inputs(&ipcA, &ipcB);
  vector<vector<pcn_vals>> result = ipcA.o_r(ipcB.get_pcnLST());
  pcn_cubes out(this->_nvars_, result.size(), result);
  comment_out = "result for: + " + or_inp_A + " " + or_inp_B;
  display_outputs(&out, inp_lst[0], &p_flag, comment_out);
  string file = this->get_filename(inp_lst[0]);
  (this->_FXlst_).at(inp_lst[0]) = make_tuple(file, false, out);
  return true;
}


bool boolean_fn_array::a_n_d(vector<string> inp_lst) {
  bool p_flag = false;
  string comment_in, comment_out;
  string and_inp_A = inp_lst[1], and_inp_B = inp_lst[2];
  cout << "* Anding : " << and_inp_A << " and " << and_inp_B << "\n";
  pcn_cubes ipcA = get<2>((this->_FXlst_).at(and_inp_A));
  pcn_cubes ipcB = get<2>((this->_FXlst_).at(and_inp_B));
  //display_inputs(&ipcA, &ipcB);
  vector<vector<pcn_vals>> result = ipcA.a_n_d(ipcB.get_pcnLST());
  pcn_cubes out(this->_nvars_, result.size(), result);
  comment_out = "result for: & " + and_inp_A + " " + and_inp_B;
  display_outputs(&out, inp_lst[0], &p_flag, comment_out);
  string file = this->get_filename(inp_lst[0]);
  (this->_FXlst_).at(inp_lst[0]) = make_tuple(file, false, out);
  return true;
}

bool boolean_fn_array::print(vector<string> inp_lst) {
  bool p_flag = true;
  string comment; 
  for ( auto num : inp_lst ) {
      cout << "* Outputting for " << num << endl;
      comment = "result for p " + num;
      pcn_cubes to_display = get<2>((this->_FXlst_).at(num));
      display_outputs(&to_display, num, &p_flag, comment);
  }
  return true;
}

bool boolean_fn_array::quit(vector<string> inp_lst) {
  cout << "###############################################################\n";
  return true;
}

// p_flag => Flag set to true when the command being run is "p" else false (for "&" "+" "!")
void boolean_fn_array::display_outputs(pcn_cubes *to_display_pcn_lst, string num, bool *p_flag, string comment) {
  string file = this->get_filename(num);
  string final_comment = "";
  ofstream ascii_file_out, pcn_file_out, cubeset_file_out;
  if ( this->_writeOUT_ ) {
     if ( ! *p_flag ) {
        if ( this->_ASCII_ ) {
           final_comment = "   - Writing ASCII " + comment + " to file: " + file + ".ASCII\n";
           ascii_file_out.open(file + ".ASCII");
           (*to_display_pcn_lst).display_int(ascii_file_out, final_comment);
        }
     } else {
        final_comment = "   - Writing " + comment + " to file: " + file + "\n";
        ascii_file_out.open(file);
        (*to_display_pcn_lst).display_int(ascii_file_out, final_comment);
     }
     if ( this->_PCN_ ) {
        final_comment = "   - Writing PCN " + comment + " to file: " + file + ".PCN\n";
        pcn_file_out.open(file + ".PCN");
        (*to_display_pcn_lst).display_pcn(pcn_file_out, final_comment);
     }
     if ( this->_cubeSET_ ) {
        final_comment = "   - Writing cubeSET " + comment + " to file: " + file + ".cubeSET\n";
        cubeset_file_out.open(file + ".cubeSET");
        (*to_display_pcn_lst).display_cube_set(cubeset_file_out, final_comment);
     }
  } else {
     if ( ! *p_flag ) {
        if ( this->_ASCII_ ) {
           final_comment = "   - Printing ASCII " + comment + ":\n";
           (*to_display_pcn_lst).display_int(cout, final_comment);
        }
     } else {
        final_comment = "   - Printing " + comment + ":\n";
        (*to_display_pcn_lst).display_int(cout, final_comment);
     }
     if ( this->_PCN_ ) {
        final_comment = "   - Printing PCN " + comment + ":\n";
        (*to_display_pcn_lst).display_pcn(cout, final_comment);
     }
     if ( this->_cubeSET_ ) {
        final_comment = "   - Printing cubeSET " + comment + ":\n";
        (*to_display_pcn_lst).display_cube_set(cout, final_comment);
     }
  }
  //   ofstream file_out;
  //   if ( this->_writeOUT_ ) {
  //      cout << ", into file: " << file << endl;
  //      file_out.open(file);
  //   } else {
  //      cout << endl;
  //   }
  //   ostream &out = this->_writeOUT_?file_out:cout;
  //to_display.display_int(out);
}
