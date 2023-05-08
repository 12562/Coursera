#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <bitset>
#include <sstream>
#include <fstream>
#include <ostream>
#include <iostream>

using namespace std;
#include "pcn_vals_class.hpp"
#include "pcn_cubes_class.hpp"

void pcn_cubes::init_nvars_and_ncubes(int num_vars, unsigned long long num_cubes) {
  this->_nvars_ = num_vars;
  this->_ncubes_ = num_cubes;
}

pcn_cubes::pcn_cubes(unsigned int num_vars, unsigned long long num_cubes) {
  init_nvars_and_ncubes(num_vars, num_cubes);
  vector<pcn_vals> single_adc_cube = generate_adc_vector();
  while (num_cubes--)
    this->_pcnLST_.push_back(single_adc_cube);
  this->_cubeLST_ = generate_adc_cubeset();
  this->_isADC_ = true;    
  this->_isEMPTY_ = false;    
  this->_isHALF_ = false;
}

vector<pcn_vals> pcn_cubes::generate_adc_vector() {
  vector<pcn_vals> single_cube;
  int num_vars = this->_nvars_;
  while ( num_vars-- ) {
    pcn_vals adc_pcn_val(3);
    single_cube.push_back(adc_pcn_val);
  }
  return single_cube;
}

set<string> pcn_cubes::generate_adc_cubeset() {
  set<string> cube_set;
  string binary;
  int num_dontcare = this->_nvars_;
  for ( int i = 0 ; i < pow(2,num_dontcare); i++ ) {
    binary = (bitset<_MAXnvars_>(i)).to_string();
    binary = binary.substr((_MAXnvars_ - num_dontcare),(_MAXnvars_ - 1));
    cube_set.insert(binary);
  }
  return cube_set;
}

pcn_cubes::pcn_cubes(unsigned int num_vars, unsigned long long num_cubes, stringstream *ss) {
  init_nvars_and_ncubes(num_vars, num_cubes);
  vector<pcn_vals> single_adc_cube = generate_adc_vector();
  while (num_cubes--) {
    vector<pcn_vals> current_cube = single_adc_cube;
    int num_terms, term, num_dontcare;
    *ss >> num_terms;
    num_dontcare = num_vars - num_terms;
    string cube(num_terms, 'x');
    while(num_terms--) {
      *ss >> term;
      current_cube[abs(term)-1].set_value(1 + int(term<0));
      cube[abs(term)-1] = int(term>0) + '0';
    }
    this->_pcnLST_.push_back(current_cube);
    this->_cubeLST_.merge(generate_cube_set_for_given_cube(num_dontcare, cube));
  }  
  this->_isADC_ = check_if_adc();
  this->_isEMPTY_ = ! this->_ncubes_;    
  this->_isHALF_ = check_if_half();
}
   
set<string> pcn_cubes::generate_cube_set_for_given_cube(int num_dontcare, string cube) {
  set<string> cube_set;
  string binary, given_cube;
  for ( int i = 0 ; i < pow(2,num_dontcare); i++ ) {
      given_cube = cube;
      binary = bitset<_MAXnvars_>(i).to_string();
      binary = binary.substr((_MAXnvars_ - num_dontcare),(_MAXnvars_ - 1));
      auto j = binary.begin();
      for ( char& ch: given_cube)
          if ( ch == 'x' )
             ch = *j++;
      cube_set.insert(given_cube);
  }
  return cube_set;
}

pcn_cubes::pcn_cubes(unsigned int num_vars, unsigned long long num_cubes, vector<vector<int>> vector_ints_2d) {
  init_nvars_and_ncubes(num_vars, num_cubes);
  vector<pcn_vals> single_adc_cube = generate_adc_vector();
  unsigned long long int i = 0; int j = 0;
  while ( i < num_cubes ) {
    vector<pcn_vals> current_cube = single_adc_cube;
    int num_terms = vector_ints_2d[i].size(), term;
    while(j < num_terms) {
      term = (vector_ints_2d[i])[j];
      current_cube[abs(term)-1].set_value(1 + int(term<0));
      j++;
    }
    this->_pcnLST_.push_back(current_cube);
    i++;
  }  
  this->_cubeLST_.merge(generate_cubeset());
  this->_isADC_ = check_if_adc();
  this->_isEMPTY_ = ! this->_ncubes_;    
  this->_isHALF_ = check_if_half();
}

set<string> pcn_cubes::generate_cubeset() {
  set<string> cube_set;
  for ( vector<pcn_vals> it : this->_pcnLST_ ) {
    int num_dont_care = 0; string cube = "";
    for ( pcn_vals vals : it ) {
        if ( vals.get_value() == pcn::dc )
           num_dont_care++;
        cube += vals.get_binary_value();
    }
    generate_and_add_cubes_to_cube_set_for_given_cube(num_dont_care, cube, &cube_set);
  }
  return cube_set;
}

void pcn_cubes::generate_and_add_cubes_to_cube_set_for_given_cube(int num_dontcare, string cube, set<string> *cube_set) {
  string binary, given_cube;
  for ( int i = 0 ; i < pow(2,num_dontcare); i++ ) {
      given_cube = cube;
      binary = bitset<_MAXnvars_>(i).to_string();
      binary = binary.substr((_MAXnvars_ - num_dontcare),(_MAXnvars_ - 1));
      auto j = binary.begin();
      for ( char& ch: given_cube)
          if ( ch == 'x' )
             ch = *j++;
      (*cube_set).insert(given_cube);
  }
}

bool pcn_cubes::check_if_adc ( unsigned long long int num_cubes, vector<vector<pcn_vals>> *ipc) {
  set<string> cube_set; bool adc = true;
  for ( vector<pcn_vals> it : (*ipc) ) {
      int num_dont_care = 0; string cube = "";
      for ( pcn_vals vals : it ) {
          if ( vals.get_value() != pcn::dc )
             adc = false;
          else
             num_dont_care++;
          cube += vals.get_binary_value();
      }
      generate_and_add_cubes_to_cube_set_for_given_cube(num_dont_care, cube, &cube_set);
  }
  if ( adc || (cube_set).size() == pow(2, this->_nvars_) )
     return true;
  else
     return false;
}


bool pcn_cubes::check_if_adc() {
  if ( this->_cubeLST_.size() == pow(2, this->_nvars_) )
     return true;
  else
     return false;
}

pcn_cubes::pcn_cubes(unsigned int num_vars, unsigned long long num_cubes, vector<vector<pcn_vals>> vector_pcn_vals) {
  init_nvars_and_ncubes(num_vars, num_cubes);
  this->_pcnLST_ = vector_pcn_vals;
  this->_cubeLST_.merge(generate_cubeset());
  this->_isADC_ = check_if_adc();
  this->_isEMPTY_ = ! this->_ncubes_;    
  this->_isHALF_ = check_if_half();
}

bool pcn_cubes::check_if_half() {
  if ( this->_cubeLST_.size() == pow(2, (this->_nvars_ - 1)) )
     return true;
  else
     return false;
}

bool pcn_cubes::is_empty() {
  return this->_isEMPTY_;
}

bool pcn_cubes::is_ADC() {
  return this->_isADC_;
}

bool pcn_cubes::is_half() {
  return this->_isHALF_;
}

unsigned int pcn_cubes::get_nvars() {
  return this->_nvars_;
}

unsigned long long int pcn_cubes::get_ncubes() {
  return this->_ncubes_;
}

vector<vector<pcn_vals>> pcn_cubes::get_pcnLST() {
  return this->_pcnLST_;
}

set<string> pcn_cubes::get_cubeLST() {
  return this->_cubeLST_;
}

void pcn_cubes::display_int(ostream &output, string comment) {
  cout << comment;
  output << this->_nvars_ << endl << this->_ncubes_ << endl;
  for (vector<pcn_vals> it : this->_pcnLST_) {
      string to_print = "";
      int pos = 1, cnt = 0;
      for (pcn_vals it2 : it ) {
          string val = it2.get_binary_value();
          if ( val == "0" ) {
             to_print += "-" + to_string(pos) + " ";
             cnt++;
          } else if ( val == "1") {
             to_print += to_string(pos) + " ";
             cnt++;
          }
          pos++;
      }
      output << to_string(cnt) << " " << to_print << endl;
  }
  output << endl;
}

void pcn_cubes::display_pcn(ostream &output, string comment) {
  cout << comment;
  output << "NCUBES: " << this->_ncubes_ << endl;
  if ( ! is_empty() ) {
     for (vector<pcn_vals> it : this->_pcnLST_ ) {
         for (pcn_vals it2 : it ) {
             output << it2.get_cube_notation() << " ";
         }
         output << endl;
     }
     output << endl;
  } else {
     output << "Empty PCNList...";
     output << endl << endl;
  }
}


void pcn_cubes::display_cube_set(ostream &output, string comment) {
  cout << comment;
  if ( ! is_empty() ) {
     for (string it: this->_cubeLST_ )
         output << it << " ";
     output << endl << endl;
  } else {
     output << "Empty CubeSet...";
     output << endl << endl;
  }
}

void pcn_cubes::add_cube(vector<pcn_vals> cube ) {
  this->_pcnLST_.push_back(cube);
  this->_cubeLST_ = this->generate_cubeset();
  this->_isADC_ = check_if_adc();
  this->_isEMPTY_ = ! this->_ncubes_;
  this->_isHALF_ = check_if_half();
}

void pcn_cubes::and_x(vector<vector<pcn_vals>> *ipc, int var_to_split_about, pcn_vals *var) {
  for (vector<pcn_vals> &it : *ipc )
         it[var_to_split_about].set_pcn((*var).get_value());
}

vector<vector<pcn_vals>> pcn_cubes::a_n_d(vector<vector<pcn_vals>> cubeList) {
  return this->a_n_d(&cubeList, &(this->_pcnLST_));
}

vector<vector<pcn_vals>> pcn_cubes::a_n_d(vector<vector<pcn_vals>> *cubeListA, vector<vector<pcn_vals>> *cubeListB) {
  vector<vector<pcn_vals>> notA = comp(cubeListA);
  vector<vector<pcn_vals>> notB = comp(cubeListB);
  vector<vector<pcn_vals>> orAB = o_r(&notA, &notB);
  vector<vector<pcn_vals>> tmp = comp(&orAB);
        //display_pcn(&tmp, "Complement");
  return tmp;
}


vector<vector<pcn_vals>> pcn_cubes::o_r(vector<vector<pcn_vals>> cubeList) {
  return this->o_r(&cubeList, &(this->_pcnLST_));
}

vector<vector<pcn_vals>> pcn_cubes::o_r(vector<vector<pcn_vals>> *cubeListN, vector<vector<pcn_vals>> *cubeListP) {
  vector<vector<pcn_vals>> result;
  for (vector<pcn_vals> it : *cubeListP)
      result.push_back(it);
  for (vector<pcn_vals> it : *cubeListN)
      result.push_back(it);
  return result;
}

vector<vector<pcn_vals>> pcn_cubes::find_cofactor(vector<vector<pcn_vals>> *ipc, int var_to_split_about, pcn_vals pos_or_neg ) {
  vector<vector<pcn_vals>> cofactor;
  bool append;
  for (vector<pcn_vals> it : *ipc ) {
      vector<pcn_vals> tmp = it;
      append = true;
      if ( it[var_to_split_about].get_value() == pos_or_neg.get_value() ) {
         tmp[var_to_split_about].set_value(3);
      } else if ( it[var_to_split_about].get_value() != pcn::dc ) {
         append = false;
      }
      if ( append )
         cofactor.push_back(tmp);
  }
  return cofactor;
}

int pcn_cubes::get_variable_to_split_about(vector<vector<pcn_vals>> *ipc) {
  pair<int,int> p(0,0);
  //cout << this->_nvars_ << ": nvars\n";
  vector<pair<unsigned long long int,unsigned long long int>> extent_of_binate_or_unate(this->_nvars_, p);
  int pos;
  for (vector<pcn_vals> it : *ipc) {
      pos = 0;
      for (pcn_vals it2 : it ) {
          if ( it2.get_value() == pcn::nc )
             extent_of_binate_or_unate[pos].first++;
          else if ( it2.get_value() == pcn::c )
             extent_of_binate_or_unate[pos].second++;
          pos++;
      }
      //cout << pos << endl;
  }
  //for ( auto it_tmp : extent_of_binate_or_unate )
  //  cout << it_tmp.first << ":" << it_tmp.second << endl;

  unsigned long long int minimum = numeric_limits<unsigned long long int>::max(), most_binate_max = 0, most_unate_max = 0;
  pos = 0;
  vector<int> track_binate_maxes, track_unate_maxes;
  for ( pair<unsigned long long int,unsigned long long int> vars : extent_of_binate_or_unate ) {
      if ( vars.first > 0 && vars.second > 0  ) {
         if ( vars.first + vars.second > most_binate_max ) {
            most_binate_max = vars.first + vars.second;
            minimum = max(vars.first,vars.second) - min(vars.first, vars.second);
            track_binate_maxes.clear();
            track_binate_maxes.push_back(pos);
         } else if ( (vars.first + vars.second == most_binate_max) ) {
            if ( max(vars.first,vars.second) - min(vars.first,vars.second) < minimum ) {
               minimum = max(vars.first, vars.second) - min(vars.first, vars.second);
               track_binate_maxes.clear();
               track_binate_maxes.push_back(pos);
            } else if ( max(vars.first, vars.second) - min(vars.first, vars.second) == minimum ) {
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
  //cout << track_binate_maxes.size() << endl;
  if ( track_binate_maxes.size() > 0 ) {
     return track_binate_maxes[0];
  } else {
     return track_unate_maxes[0];
  }

}

void pcn_cubes::display_pcn(vector<vector<pcn_vals>> *container, string comment) {
  cout << "NCUBES: " << (*container).size() << endl;
  cout << comment << endl;
  if ( (*container).size() ) {
     for (vector<pcn_vals> it : *container) {
         for (pcn_vals it2 : it ) {
             cout << it2.get_cube_notation();
             cout << " ";
         }
         cout << endl;
     }
     cout << endl;
  } else {
     cout << "Empty CubeList...";
     cout << endl << endl;
  }
}


vector<vector<pcn_vals>> pcn_cubes::complement() {
  this->_ccLST_ = this->comp(&(this->_pcnLST_));
  return this->_ccLST_;
}

vector<vector<pcn_vals>> pcn_cubes::comp(vector<vector<pcn_vals>> *ipc) {
  vector<vector<pcn_vals>> cc;
  unsigned long long int ncubes = (*ipc).size();
  if ( ncubes == 0 ) {
     vector<pcn_vals> dc_pcn = generate_adc_vector();
     cc.push_back(dc_pcn);
  } else if ( check_if_adc(ncubes, ipc) ) {
  } else if ( ncubes == 1 ) {
     vector<pcn_vals> tmp;
     vector<pcn_vals> dc_pcn = generate_adc_vector();
     for (vector<pcn_vals> it : *ipc) {
         int pos = 0;
         for (vector<pcn_vals>::iterator current=it.begin(); (current!=it.end()); ++current) {
             if ( (*current).get_value() != pcn::dc ) {
                tmp = dc_pcn;
                tmp[pos].set_pcn((*current).invert_term());
                cc.push_back(tmp);
             }
             pos++;
         }
     }
  } else {
     vector<vector<pcn_vals>> P, N, pc, nc;
     //cout << "\nGetting variable to split about\n\n";
     int x = get_variable_to_split_about(ipc);
     //cout << "Variable to split about : " << x << endl;
     pcn_vals positive(1), negative(2);
     pc = find_cofactor(ipc, x, positive);
     //display_pcn(&pc, "Positive cofactor");
     P = comp(&pc);
     //display_pcn(&P, "Positive cofactor complement");
     nc = find_cofactor(ipc, x, negative);
     //display_pcn(&nc, "Negative cofactor");
     N = comp(&nc);
     //display_pcn(&P, "Negative cofactor complement");
     and_x(&P, x, &positive);
     and_x(&N, x, &negative);
     cc = o_r(&N, &P);
  }
  return cc;
}

