
void generate_and_add_cubes_to_cube_set(int num_dontcare, string cube, set<string> *cube_set) {
     string binary, given_cube;
     for ( int i = 0 ; i < pow(2,num_dontcare); i++ ) {
         given_cube = cube; 
         binary = bitset<20>(i).to_string();
         binary = binary.substr(20 - num_dontcare,19); 
         auto j = binary.begin();
         for ( char& ch: given_cube) 
             if ( ch == 'x' ) 
                ch = *j++; 
         (*cube_set).insert(given_cube);
     }    
    
}

vector<pcn_vals> generate_adc_vector(int num_vars) {
      vector<pcn_vals> lst;
      while ( num_vars-- ) {
        pcn_vals myvar(3);
        lst.push_back(myvar); 
      }
      return lst;
}

bool read_input(string file, int *nvars, unsigned long long int *ncubes, list<vector<pcn_vals>> *container, list<vector<int>> *icontainer, set<string> *cube_set) {
     int num_vars; unsigned long long int num_cubes; string line;

     ifstream infile ( file ); 
     if (infile.is_open()) {
        infile >> num_vars; *nvars = num_vars;
        infile >> num_cubes; *ncubes = num_cubes; 
        vector<pcn_vals> lst = generate_adc_vector(num_vars); 
        vector<pcn_vals> lst_copy = lst; vector<int> ilst;
        string cube = "";
        while ( num_cubes-- ) {
          int num_terms, tmp;
          infile >> num_terms; ilst = {num_terms};
          lst = lst_copy; 
          while ( num_terms-- ) {
            infile >> tmp; ilst.push_back(tmp);
            if ( tmp > 0 ) {
               lst[tmp-1].set_value(1);
            } else if ( tmp < 0 ) {
               lst[-(tmp+1)].set_value(2);
            }
          }
          (*container).push_back(lst);
          (*icontainer).push_back(ilst);
        }
        infile.close();
     } else {
        cout << "Unable to open file";
        return false;
     }
     return true;
}

int get_variable_to_split_about(list<vector<pcn_vals>> *ipc) {
    int nvars = (*((*ipc).begin())).size();
    pair<int,int> p(0,0);
    vector<pair<int,int>> extent_of_binate_or_unate(nvars, p);
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

list<vector<pcn_vals>> find_cofactor(list<vector<pcn_vals>> *ipc, int var_to_split_about, pcn_vals pos_or_neg ) {
    list<vector<pcn_vals>> cofactor;
    bool append;
    for (vector<pcn_vals> it : *ipc) {
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

bool check_if_adc ( unsigned long long int num_cubes, int num_vars, list<vector<pcn_vals>> *ipc, bool print_cube_set, string to_print ) {
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
         generate_and_add_cubes_to_cube_set(num_dont_care, cube, &cube_set);
     }
     if ( print_cube_set ) {
        cout << "\n" << to_print << endl;
        cout << cube_set.size() << endl;
        display_cube_set(&cube_set);
     }
         
     if ( adc || (cube_set).size() == pow(2, num_vars) )
        return true;
     else
        return false;
}

void a_n_d(list<vector<pcn_vals>> *cubeList, int var_split_about, pcn_vals *var) { 
     for (vector<pcn_vals> &it : *cubeList)
         it[var_split_about].set_pcn((*var).get_value());
}

list<vector<pcn_vals>> o_r(list<vector<pcn_vals>> *cubeListP, list<vector<pcn_vals>> *cubeListN) {
     list<vector<pcn_vals>> result;
     for (vector<pcn_vals> it : *cubeListP)
         result.push_back(it);
     for (vector<pcn_vals> it : *cubeListN)
         result.push_back(it);
     return result;
}

list<vector<pcn_vals>> complement(list<vector<pcn_vals>> *ipc, int nvars, bool debug ) {
     list<vector<pcn_vals>> cc;
     unsigned long long int ncubes = (*ipc).size(); 
     if ( ncubes == 0 ) {
        vector<pcn_vals> dc_pcn = generate_adc_vector(nvars);
        cc.push_back(dc_pcn); 
     } else if ( check_if_adc(ncubes, nvars, ipc, debug, "Cube set is: " ) ) {
     } else if ( (*ipc).size() == 1 ) {
        vector<pcn_vals> tmp;
        vector<pcn_vals> dc_pcn = generate_adc_vector(nvars);
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
        list<vector<pcn_vals>> P, N, pc, nc;
        int x = get_variable_to_split_about(ipc);
        pcn_vals positive(1), negative(2);
        if ( debug ) 
           cout << "Variable to split about : " << x << endl;
        pc = find_cofactor(ipc, x, positive);
        display_pcn(&pc, "Displaying positive cofactor: \n", debug);
        P = complement(&pc, nvars, debug);
        display_pcn(&P, "Displaying complement of positive cofactor: \n", debug);
        nc = find_cofactor(ipc, x, negative);
        display_pcn(&nc, "Displaying negative cofactor: \n", debug);
        N = complement(&nc, nvars, debug);
        display_pcn(&N, "Displaying complement of negative cofactor: \n", debug);
        a_n_d(&P, x, &positive);
        display_pcn(&P, "Displaying P after and: \n", debug);
        a_n_d(&N, x, &negative);
        display_pcn(&N, "Displaying N after and: \n", debug);
        cc = o_r(&P, &N);
        display_pcn(&cc, "Displaying complement OR : \n", debug);
     }
     return cc;   
}
