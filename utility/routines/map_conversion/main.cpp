#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

int main(int argc, char* argv[])
{
    int masks[5]    = { 0x3938700, 0x2FAF080, 0x1C9C380, 0x1312D00, 0x0000000 };
    int n_masks[4]  = { 0x8000000, 0x4000000, 0x2000000, 0x1000000 };

    string filename = argv[1];
    //string filename = "20180706.1530_JMTSample_Scan";
    ifstream ifile(filename);
    ofstream ofile(filename + "_converted");

    string line;
    if (ifile.is_open())
    {
        getline(ifile, line);
        ofile<<line + '\n';

        int number;
        while (getline(ifile, line))
        {
            number = stoi(line);
            if (number < masks[3])
            {
                ofile<<line + '\n';
                continue;
            }
            else
            {
                if (number >= masks[0])
                {
                    number -= masks[0];
                    number ^= n_masks[0];
                    line = to_string(number);
                    ofile<<line + '\n';
                    continue;
                }

                else if (number < masks[0] && number >= masks[1])
                {
                    number -= masks[1];
                    number ^= n_masks[1];
                    line = to_string(number);
                    ofile<<line + '\n';
                    continue;
                }
                else if (number < masks[1] && number >= masks[2])
                {
                    number -= masks[2];
                    number ^= n_masks[2];
                    line = to_string(number);
                    ofile<<line + '\n';
                    continue;
                }
                else if (number < masks[2] && number >= masks[3])
                {
                    number -= masks[3];
                    number ^= n_masks[3];
                    line = to_string(number);
                    ofile<<line + '\n';
                    continue;
                }
                else
                {
                    ofile<<line + '\n';
                    continue;
                }

            }
        }
    }

    ifile.close();
    ofile.close();

    return 0;
}
