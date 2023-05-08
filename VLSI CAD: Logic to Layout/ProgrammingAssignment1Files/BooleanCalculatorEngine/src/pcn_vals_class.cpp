#include <string>
#include <map>
#include "pcn_vals_class.hpp"

using namespace std;

pcn_vals::pcn_vals() {
    this->term_value = pcn::dc;
}

pcn_vals::pcn_vals(int val) {
    switch(val) {
      case 1: this->term_value = pcn::nc;
              break;
      case 2: this->term_value = pcn::c;
              break;
      case 3: this->term_value = pcn::dc;
              break;
      default: this->term_value = pcn::np;
    }
}

pcn pcn_vals::invert_term() {
    if ( this->term_value == pcn::nc ) {
       return pcn::c;
    } else if ( this->term_value == pcn::c ) {
       return pcn::nc;
    } else {
       return pcn::dc;
    }
}
    
void pcn_vals::set_pcn (pcn value) {
    this->term_value = value;
}
         
void pcn_vals::set_value(int value) {
    switch(value) {
      case 1: this->term_value = pcn::nc;
              break;
      case 2: this->term_value = pcn::c;
              break;
      case 3: this->term_value = pcn::dc;
              break;
      default: this->term_value = pcn::np;
    }
}

pcn pcn_vals::get_value() {
    return this->term_value;
}
    
string pcn_vals::get_binary_value() {
    if ( this->term_value == pcn::nc ) {
       return "1";
    } else if ( this->term_value == pcn::c ) {
       return "0";
    } else {
       return "x";
    }
}

string pcn_vals::get_cube_notation() {
  map<pcn, string> m = {{pcn::dc, "11"},
                        {pcn::np, "00"},
                        {pcn::nc, "01"},
                        {pcn::c,  "10"}};
  return m[this->term_value];
} 
