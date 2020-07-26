#ifndef MAP_H
#define MAP_H

#include <cstddef>
#include <fstream>

#include <map>
#include <string>
#include <vector>

#include <random>

enum class header_v
{
    ev_not_defined,
    ev_ver1,
    ev_ver2,
};

class multidet
{
public:
    multidet(double, double);
    void distribute(ulong&);
private:
    double jlo;
    double jhi;
    uint n_overlap;
    const double m;
    const double c;
    std::random_device r;
    std::default_random_engine e1;
    std::discrete_distribution<int> discrete_dist;
};

class input_stream
{
public:
    input_stream(std::string fname) : prev(0), next(0)
    {
        // The below initiliazes buff
        std::ifstream input(fname);
        read_stream_into_container(input);
        input.close();
    }
    bool get_line(std::string&);
    size_t get_size()
    {
        return buff.size();
    }
    void fix_header()
    {
        buff.replace(0, sizeof ("ver.001"), "ver.001\n");
    }
    std::string get_buffer() const
    {
        return buff;
    }
private:
    void read_stream_into_container(std::ifstream& in);

    std::string buff;
    size_t prev;
    size_t next;

};

class image_xrf {

public:
    image_xrf(std::string _name) :
        fname(_name),
        hist(16384, 0), hist_sum(16384, 0), hist_max(16384, 0),
        masks_det({ 0x1312D00, 0x1C9C380, 0x10000000, 0x20000000 }),
        masks_pos({ 0x2FAF080, 0x3938700 }),
        p_in(nullptr), p_multi(nullptr)
    {
        flag_odd = false;
        counter = 0;

        p_multi = new multidet(1.19815, -17.2263);
        open_map_file();
    }
    ~image_xrf()
    {
        delete p_multi;
        delete p_in;
    }
    void open_map_file();
    void optimize();

    struct map_var_t
    {
        ulong x_min;
        ulong x_max;
        ulong y_min;
        ulong y_max;
        ulong step_x;
        ulong step_y;
        ulong dwell;

        ulong x_dim;
        ulong y_dim;
        ulong pixels;
    } map_var;

private:
    void make_header();
    void parse_energies();
    void parse_positions();
    inline void write_preceding();

    std::string fname;
    std::string line;
    std::string data;
    std::string header;
    std::vector<std::string*> odd_pix;

    bool flag_odd;

    ulong pixels;
    ulong datum1;
    ulong datum2;
    ulong counter;
    ulong prev_y;

    //std::ifstream input;
    std::ofstream output;
    std::vector<uint> hist;
    std::vector<uint> hist_sum;
    std::vector<uint> hist_max;
    std::vector<uint> masks_det;
    std::vector<uint> masks_pos;

    input_stream* p_in;
    multidet* p_multi;
};

#endif // MAP_H
