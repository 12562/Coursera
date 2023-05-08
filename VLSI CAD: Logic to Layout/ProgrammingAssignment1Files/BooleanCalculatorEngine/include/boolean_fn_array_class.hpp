class boolean_fn_array {
  private:
    unsigned int _nfx_;
    unsigned int _nvars_;
    bool _writeOUT_;
    bool _ASCII_;
    bool _PCN_;
    bool _cubeSET_;
    bool _DEBUG_;
    string _IO_;
    map<string, tuple<string, bool, pcn_cubes>> _FXlst_;
  public:
    boolean_fn_array();
    boolean_fn_array(unsigned int num_functions, vector<tuple<string, string, bool>> file_map);
    boolean_fn_array(map<string, tuple<string, bool, pcn_cubes>> inp_map, unsigned int num_vars); 
    void set_nfx(unsigned int num);
    void set_nvars(unsigned int nvars);
    unsigned int get_nvars();
    void set_FXlst(map<string, tuple<string, bool, pcn_cubes>> m);
    void set_writeOUT(bool write_out);
    void set_writeOUT(bool *write_out);
    void set_ASCII(bool *ascii);
    void set_PCN(bool *pcn); 
    void set_DEBUG(bool *debug);
    void set_io(string *i_o);
    void set_cubeSET(bool *cubeset);
    string get_filename(string num);
    bool read_input(vector<string> inp_lst);
    bool complement(vector<string> inp_lst);
    bool o_r(vector<string> inp_lst);
    bool a_n_d(vector<string> inp_lst);
    bool print(vector<string> inp_lst);
    bool quit(vector<string> inp_lst);
    void display_outputs(pcn_cubes *to_display_pcn_lst, string num, bool *p_flag, string comment);
};
