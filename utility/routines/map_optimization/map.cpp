#include <map.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;


// Random assignment protocol
multidet::multidet(double _m, double _c) : m(_m),c(_c)
{
    jlo = 0;
    jhi = 1;
    n_overlap = ceil(jhi) - floor(jlo);
    std::default_random_engine e1(r());
    std::discrete_distribution<int> discrete_dist({1, 1});
}

void multidet::distribute(ulong& j)
{
    jlo = (double)(j) * m + c;
    if (jlo < 0)
    {
        j = 0;
        return;
    }
    if (jlo > 16383)
    {
        j = 16383;
        return;
    }

    jhi = jlo + m;
    n_overlap = ceil(jhi) - floor(jlo);
    if (n_overlap == 1)
    {
        j = floor(jlo);
        return;
    }

    std::vector<double> weights;
    weights.push_back(ceil(jlo) - jlo);

    while (--n_overlap)
    {
        //We construct a probability array { p_begin, p_mid, p_mid, ..., p_end }
        switch(n_overlap)
        {
        case 1:
            weights.push_back(jhi - floor(jhi));
            break;
        default:
            weights.push_back(1);
            break;
        }
    }
    std::discrete_distribution<int> discrete_dist(weights.begin(), weights.end());
    //std::vector<double> p = discrete_dist.probabilities();
    j = floor(jlo) + discrete_dist(e1);
}


// Loading onto shm procedures
void image_xrf::open_map_file()
{
    p_in = new input_stream(fname);
    data.reserve(p_in->get_size());

//    if (false)
//    {   // hot fix for fixing the header version
//        output.open(fname);
//        p_in->fix_header();
//        output << p_in->get_buffer();
//        output.close();
//        return;
//    }

    map<string, header_v> m_header_v;
    m_header_v["ver.001"] = header_v::ev_ver1;
    m_header_v["{\n"] = header_v::ev_ver2;
    header.reserve(std::numeric_limits<short>::max());

    // Get the header version
    p_in->get_line(line);
    switch (m_header_v[line])
    {
    case header_v::ev_ver1:
        make_header();
        optimize();
        break;
    case header_v::ev_ver2:
        // No need for optimization just load onto memory
        break;
    default:
        break;
    }
}

void image_xrf::make_header()
{
    // Parse the header and
    // construct new header
    header.append("{\n");
    std::cout<<"I need some information to construct "
               "the new header.\n"
               "Please enter the object identifier:\n";
    getline(std::cin, line);
    header.append("Object_ID = " + line);
    header.push_back('\n');
    std::cout<<"Enter the date when this scan was acquired.\n"
               "Use the format YYYY-MM-DD:\n";
    getline(std::cin, line);
    header.append("DAQ_date = " + line);
    header.push_back('\n');
    std::cout<<"Enter the end time of the scan.\n"
               "Use the 24 hour format HH-MM:\n";
    getline(std::cin, line);
    header.append("DAQ_time = " + line);
    header.push_back('\n');
    std::cout<<"Enter the beam accelerating voltage "
               "in units of kV:\n";
    getline(std::cin, line);
    header.append("Beam_kV = " + line);
    header.push_back('\n');
    std::cout<<"Enter the filament current of the "
               "X-ray tube in units of uA:\n";
    getline(std::cin, line);
    header.append("Beam_uA = " + line);
    header.push_back('\n');

    // Get the scan variables from the old header
    p_in->get_line(line);
    map_var.x_min = stoul(line);
    p_in->get_line(line);
    map_var.x_max = stoul(line);
    p_in->get_line(line);
    map_var.y_min = stoul(line);
    p_in->get_line(line);
    map_var.y_max = stoul(line);
    p_in->get_line(line);
    map_var.step_x = stoul(line);
    p_in->get_line(line);
    map_var.step_y = stoul(line);
    p_in->get_line(line);
    map_var.dwell = stoul(line);

    // Calculate image dimensions
    map_var.x_dim = (map_var.x_max - map_var.x_min) / map_var.step_x;
    map_var.y_dim = (map_var.y_max - map_var.y_min) / map_var.step_y;
    map_var.pixels = map_var.x_dim * (++map_var.y_dim);

    // Add the scan variables to the new header
    header.append("Row_length = " + to_string(map_var.x_dim));
    header.push_back('\n');
    header.append("Rows_total = " + to_string(map_var.y_dim));
    header.push_back('\n');
    header.append("Pixels_total = " + to_string(map_var.pixels));
    header.push_back('\n');

    std::stringstream ss(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.x_min / 1000);
    header.append("Motor_start_x = " + ss.str());
    header.push_back('\n');
    ss.str(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.y_min / 1000);
    header.append("Motor_start_y = " + ss.str());
    header.push_back('\n');
    ss.str(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.step_x / 1000);
    header.append("Motor_step_x = " + ss.str());
    header.push_back('\n');
    ss.str(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.step_y / 1000);
    header.append("Motor_step_y = " + ss.str());
    header.push_back('\n');
    ss.str(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.dwell / 1000);
    header.append("Motor_speed = " + ss.str());
    header.push_back('\n');

    //Calculate actual dwell time
    ss.str(std::string{});
    ss << std::fixed << std::setprecision(3) << static_cast<float>(map_var.step_x / map_var.dwell);
    header.append("Dwell_time = " + ss.str());
    header.push_back('\n');
    header.push_back('}');
    header.push_back('\n');
}

void image_xrf::optimize()
{
    // Set buffers for pixels in odd lines
    // Buffers are used to correct the order of the pixels
    size_t size = static_cast<size_t>(p_in->get_size() * 1.1) / map_var.pixels;
    for (size_t i = 0; i < map_var.x_dim; i++)
    {
        odd_pix.push_back(new string(size, 0));
    }

    pixels = 0;
    datum2 = 0;
    prev_y = map_var.y_min;
    p_in->get_line(line);
     datum1 = stoul(line);
    parse_positions();


    while (p_in->get_line(line))
    {
        datum1 = stoul(line);
        if (datum1 > masks_pos[0])
        {
            write_preceding();
            parse_positions();
        }
        else
            parse_energies();
    }
    write_preceding();
    data.pop_back();

    // Cleaning up resources
    for (auto i : odd_pix)
    {
        delete i;
    }

    output.open(fname + "_opt");
    output << header;
    output << data;
    output.close();

    string path("");
    if (mkdir("./dmsum", 0777) == 0)
        path = string("./dmsum/");
    else
        printf("Couldn't create output directory\n");

    // Save the sum and maxima spectra
    output.open(path + fname + "_sum.spe");
    output<<"$SPEC_ID:\n";
    output<<fname<<endl;
    output<<"$DATA:\n";
    output<<"0 16383\n";
    for (auto i : hist_sum)
        output << i << endl;
    output.close();

    output.open(path + fname + "_max.spe");
    output<<"$SPEC_ID:\n";
    output<<fname<<endl;
    output<<"$DATA:\n";
    output<<"0 16383\n";
    for (auto i : hist_max)
        output << i << endl;
    output.close();
}

void image_xrf::parse_positions()
{
    datum1 -= masks_pos[0];
    p_in->get_line(line);
    datum2 = stoul(line);
    datum2 -= masks_pos[1];

    datum1 = (datum1 - map_var.x_min) / map_var.step_x;
    datum2 = (datum2 - map_var.y_min) / map_var.step_y;
    if (prev_y != datum2)
    {
        prev_y = datum2;
        printf("%3.1f%% progress\n", (double)(counter * 100) / map_var.pixels);
    }
    pixels = datum2 * map_var.x_dim + datum1;
    counter++;

    line = "P\x09" + to_string(pixels) + "\x09" + to_string(datum1) + "\x09" + to_string(datum2) + "\x0A";
    if (datum2 % 2)
    {
        flag_odd = true;
        odd_pix.at(datum1)->clear();
        odd_pix.at(datum1)->append(line);
        pixels = datum1;
    }
    else
        data.append(line);
}

void image_xrf::parse_energies()
{
    if (datum1 < masks_det[1] && datum1 >= masks_det[0]) // detA
    {
        datum1 -= masks_det[0];
        hist.at(datum1) += 1;
    }
    else if (datum1 >= masks_det[1])
    {
//        datum1 -= masks_det[1];
//        datum2 = datum1;
//        p_multi->distribute(datum1);
//        hist.at(datum1) += 1;
    }
}


inline void image_xrf::write_preceding()
{
    uint index = 0;
    string* ptr = nullptr;
    flag_odd ?
                ptr = odd_pix.at(pixels):
            ptr = &data ;

    for (auto& val : hist)
    {
        if (val)
        {
            // Populate sum spectrum
            hist_sum.at(index) += val;
            // Populate max spectrum
            if (hist_max.at(index) < val)
                hist_max.at(index) = val;
            // Mask events for optimized output file
            val |= (index << 14);
            ptr->append(to_string(val) + "\x0A");
            val = 0;
        }
        index++;
    }

//    for (auto& val : detb)
//    {
//        if (val)
//        {
//            val = (masks_det[3] | (index << 14) | val);
//            ptr->append(to_string(val));
//            ptr->append(1, '\n');
//            val = 0;
//        }
//        index++;
//    }

    if (flag_odd && (pixels % map_var.x_dim) == 0)
    {
        for (auto str : odd_pix)
            data.append(*str);
        flag_odd = false;
    }
}

// Input methods

bool input_stream::get_line(string& str)
{
    if ((next = buff.find('\n', prev)) != string::npos)
    {
        str = buff.substr(prev, next - prev);
        prev = ++next;
        return true;
    }
    else
        return false;
}


void input_stream::read_stream_into_container(std::ifstream& in)
{
    using namespace std;
    auto const start_pos = in.tellg();
    if (streamsize(-1) == start_pos)
        throw ios_base::failure{"error"};

    if (!in.ignore(numeric_limits<streamsize>::max()))
        throw ios_base::failure{"error"};

    auto const char_count = in.gcount();

    if (!in.seekg(start_pos))
        throw ios_base::failure{"error"};

    string::allocator_type alloc = {};
    buff = string(move(alloc));
    buff.resize(static_cast<ulong>(char_count));

    if (0 != buff.size())
    {
        if (!in.read(reinterpret_cast<char*>(&buff[0]), static_cast<long>(buff.size())))
            throw std::ios_base::failure{"error"};
    }
}
