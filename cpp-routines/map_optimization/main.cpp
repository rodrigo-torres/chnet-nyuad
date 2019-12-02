#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
long masks_det[] = { 0x1312D00, 0x1C9C380, 0x10000000, 0x30000000 };
long masks_pos[] = { 0x2FAF080, 0x3938700, 0x90000000, 0xA0000000 };

void parse_positions();
void parse_energies();
void write_preceding();

struct
{
    string str;
    ifstream input;
    ofstream output;
    vector<uint> deta;
    vector<uint> detb;

    long datum;
} var;

int main(int argc, char* argv[])
{
    var.str = string(argv[1]);
    //var.str = "try";
    var.input.open(var.str);
    var.output.open(var.str + "_optimized");
    var.deta = vector<uint>(16384, 0);
    var.detb = vector<uint>(16384, 0);

    // Get the header version
    getline(var.input, var.str);

    // Parse the header
    var.datum = -1;
    do
    {
        getline(var.input, var.str);
        var.datum = stol(var.str);
    } while (var.datum < masks_pos[0]);
    parse_positions();

    while (getline(var.input, var.str))
    {
        var.datum = stol(var.str);
        if (var.datum > masks_pos[0])
        {
            write_preceding();
            parse_positions();
        }
        else
            parse_energies();
    }
    write_preceding();
    var.input.close();
    var.output.close();

    return 0;
}

void parse_positions()
{
    var.datum -= masks_pos[0];
    var.output << (var.datum | masks_pos[2]) << endl;

    getline(var.input, var.str);
    var.datum = stol(var.str);
    var.datum -= masks_pos[1];
    var.output << (var.datum | masks_pos[3]) << endl;
}

void parse_energies()
{
    if (var.datum < masks_det[1] && var.datum >= masks_det[0]) // detA
    {
        var.datum -= masks_det[0];
        var.deta.at(var.datum) += 1;
    }
    else if (var.datum >= masks_det[1])
    {
        var.datum -= masks_det[1];
        var.detb.at(var.datum) += 1;
    }
}

void write_preceding()
{
    uint index = 0;
    for (auto val : var.deta)
    {
        if (val)
        {
            var.output << (masks_det[2] | (index << 14) | val) << endl;
        }
        index++;
    }

    index = 0;
    for (auto val : var.detb)
    {
        if (val)
        {
            var.output << (masks_det[3] | (index << 14) | val) << endl;
        }
        index++;
    }

    var.deta = vector<uint>(16384, 0);
    var.detb = vector<uint>(16384, 0);
}
