#include <string>

using namespace std;

enum class pcn {np = 00, nc = 01, c = 10, dc = 11};
class pcn_vals {
  private:
    pcn term_value;
  public:
    pcn_vals() {
      term_value = pcn::dc;
    }
    pcn_vals(int val) {
      switch(val) {
        case 1: term_value = pcn::nc;
                break;
        case 2: term_value = pcn::c;
                break;
        case 3: term_value = pcn::dc;
                break;
        default: term_value = pcn::np;
      }
    }
    pcn invert_term() {
      if ( this->term_value == pcn::nc ) {
         return pcn::c;
      } else if ( this->term_value == pcn::c ) {
         return pcn::nc;
      } else {
         return pcn::dc;
      }
    }
    
    void set_pcn (pcn value) {
         this->term_value = value;
    }
         
    void set_value(int value) {
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

    pcn get_value() {
        return this->term_value;
    }
    
    string get_binary_value() {
        if ( this->term_value == pcn::nc ) {
           return "1";
        } else if ( this->term_value == pcn::c ) {
           return "0";
        } else {
           return "x";
        }
    }
};
