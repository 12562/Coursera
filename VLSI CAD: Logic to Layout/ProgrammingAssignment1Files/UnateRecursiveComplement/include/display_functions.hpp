
ostream& operator<<(ostream& out,  pcn_vals val) {
   string to_print;
   if ( val.get_value() == pcn::nc ) {
     to_print = "01";
   } else if ( val.get_value() == pcn::c ) {
     to_print = "10";
   } else if ( val.get_value() == pcn::dc ) {
     to_print = "11";
   } else {
     to_print = "00"; 
   }
   return out << to_print;
}

void display_pcn(list<vector<pcn_vals>> *container, string comment, bool debug) {
     if ( debug ) {
        cout << "NCUBES: " << (*container).size() << endl;
        cout << comment << endl;
        if ( (*container).size() ) {
           for (vector<pcn_vals> it : *container) {
               for (pcn_vals it2 : it ) {
                   cout << it2;
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
}

void display(int num_vars, unsigned long  long int num_cubes, list<vector<pcn_vals>> *icontainer, bool debug, ostream &output) {
     if ( debug ) {
        output << num_vars << endl << num_cubes << endl;
        for (vector<pcn_vals> it : *icontainer) {
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
}

void display_cube_set(set<string> *cube_set) {
     if ( (*cube_set).size() ) {
        for (string it: *cube_set)
            cout << it << " ";
        cout << endl;
     } else {
        cout << "Empty CubeSet...";
        cout << endl << endl;
     }
}
