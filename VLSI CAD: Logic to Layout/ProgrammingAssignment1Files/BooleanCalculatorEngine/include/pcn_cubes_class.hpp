#define _MAXnvars_ 20
class pcn_cubes: public pcn_vals {
      private:
        unsigned int _nvars_ = 0;
        unsigned long long int _ncubes_ = 0;
        vector<vector<pcn_vals>> _pcnLST_;   // 2d array representing the PCN cube list.
        vector<vector<pcn_vals>> _ccLST_;    // 2d array representing complement of the PCN cube list.
        set<string> _cubeLST_;
        bool _isADC_;
        bool _isEMPTY_;
        bool _isHALF_;
      public:
        void init_nvars_and_ncubes(int num_vars, unsigned long long num_cubes);
        pcn_cubes(unsigned int num_vars, unsigned long long int num_cubes);
        vector<pcn_vals> generate_adc_vector();
        set<string> generate_adc_cubeset();
        pcn_cubes(unsigned int num_vars, unsigned long long int num_cubes, stringstream *ss);
        set<string> generate_cube_set_for_given_cube(int num_dontcare, string cube);
        pcn_cubes(unsigned int num_vars, unsigned long long int num_cubes, vector<vector<int>> vector_ints_2d);
        set<string> generate_cubeset();
        void generate_and_add_cubes_to_cube_set_for_given_cube(int num_dontcare, string cube, set<string> *cube_set);
        bool check_if_adc(unsigned long long int num_cubes,vector<vector<pcn_vals>> *ipc);
        bool check_if_adc();
        pcn_cubes(unsigned int num_vars, unsigned long long int num_cubes, vector<vector<pcn_vals>> vector_pcn_vals);
        bool check_if_half();
        bool is_empty();
        bool is_half();
        bool is_ADC();
        unsigned int get_nvars();
        unsigned long long int get_ncubes();
        vector<vector<pcn_vals>> get_pcnLST();
        set<string> get_cubeLST();
        void display_int(ostream &output, string comment);
        void display_pcn(ostream &output, string comment);
        void display_cube_set(ostream &output, string comment);
        void add_cube(vector<pcn_vals> cube );
        vector<vector<pcn_vals>> a_n_d(vector<vector<pcn_vals>> cubeList);
        vector<vector<pcn_vals>> a_n_d(vector<vector<pcn_vals>> *cubeListN, vector<vector<pcn_vals>> *cubeListP);
        vector<vector<pcn_vals>> o_r(vector<vector<pcn_vals>> cubeList);
        vector<vector<pcn_vals>> o_r(vector<vector<pcn_vals>> *cubeListN, vector<vector<pcn_vals>> *cubeListP);
        void and_x(vector<vector<pcn_vals>> *ipc, int var_to_split_about, pcn_vals *var);
        vector<vector<pcn_vals>> find_cofactor(vector<vector<pcn_vals>> *ipc, int var_to_split_about, pcn_vals pos_or_neg );
        int get_variable_to_split_about(vector<vector<pcn_vals>> *ipc);
        void display_pcn(vector<vector<pcn_vals>> *container, string comment); 
        vector<vector<pcn_vals>> complement();
        vector<vector<pcn_vals>> comp(vector<vector<pcn_vals>> *ipc);
};
