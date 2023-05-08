class cmds : public boolean_fn_array {
  private:
    list<pair<string, vector<string>>> _cmds_;
    string _inputFILE_;
    set<string> _FXnumSET_;
    set<string> _INPfxNUMset_;
    map<string, string> _fileMAP_; 
  public:
    void get_file_map(bool *from_user, unsigned int *nfx);
    void parse_input(string file);
    list<pair<string, vector<string>>> parse_input();
    bool get_and_verify_bfa_inps(unsigned int *nvars, unsigned int *nfx, string *inDIR);
    map<string, tuple<string, bool, pcn_cubes>> calculate_FXlst(string *outDir, string *inDir);
    cmds();
    void set_nfx_nvars(unsigned int nfx, unsigned int nvars, string *outDIR, string *inDIR, bool *from_user);
    void set_global_flags(bool *write_out, bool *ascii, bool *pcn, bool *cubeset, bool *debug, string *i_o);
    void solve();
    void display_cmd_file();
};
