using namespace std;

enum class pcn {np = 00, nc = 01, c = 10, dc = 11};
class pcn_vals {
  private:
    pcn term_value;
  public:
    pcn_vals();
    pcn_vals(int val);
    pcn invert_term();
    void set_pcn (pcn value);
    void set_value(int value);
    pcn get_value();
    string get_binary_value();
    string get_cube_notation();
};
