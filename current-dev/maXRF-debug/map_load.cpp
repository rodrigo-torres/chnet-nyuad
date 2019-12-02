#include "mainwindow.h"
#include <fstream>
#include <time.h>           // For seeding the random distributing function
using namespace std;

#define ia 106
#define ic 1283
#define im 6075

/****************************************************
 *  Look into SelectChannels() and OnLine behavior  *
 ****************************************************/

long *idum, seed;
int callstorandom = 0;

void setseed() {

    time_t timer;
    struct tm y2k = {};
    double seconds;

    y2k.tm_hour = 0;	y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 30;	y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);

    seconds = difftime(timer,mktime(&y2k));



    seed = seconds;
    idum = &seed;
}

long ranqd1(long *idum, double jlo, double jhi) {
    if (callstorandom > 50000) {
        setseed();
        callstorandom=0;
    }

    *idum = (*idum * ia + ic) % im;
    double j = jlo + ((jhi - jlo + 1) * static_cast<double>(*idum)) / static_cast<double>(im);
    callstorandom++;

    if ((j-floor(j)) >= 0.5) return static_cast<long>(ceil(j));
    else return static_cast<long>(floor(j));
}

extern bool MapIsOpened;
extern int *shared_memory, *shared_memory2, *shared_memory3,*shared_memory_cmd;
extern double *shared_memory5;

long codePosX = 50000000, codePosY = 60000000, codeDetA = 20000000, codeDetB = 30000000;


void adjust_map_limits() {
    // Called if energy calibration is active in the spectrum visualization program.
    QString dir = QDir::currentPath();
    dir += "/conf/Calibration.txt";
    QFile file(dir);
    file.open(QIODevice::ReadOnly);
    QString a,b;
    a = file.readLine();
    b = file.readLine();
    double const_a = a.toDouble();
    double const_b = b.toDouble();
    file.close();

    for (int i = 0; i < 8; i ++) {
    *(shared_memory5+100+i) -= const_b;
    *(shared_memory5+100+i) /= const_a;
    }
}

void MainWindow::LoadNewFileWithNoCorrection_SHM() {
    bool firstData = true;
    int dataread, vectorMap[20000] = {0};
    double jlo = 0, jhi = 0;

    double calGrad = static_cast<double>(*(shared_memory_cmd+101)) / static_cast<double>(*(shared_memory_cmd+103));
    double calOffs = static_cast<double>(*(shared_memory_cmd+102)) / static_cast<double>(*(shared_memory_cmd+103));
    setseed();

    if (*(shared_memory+24) == 1) adjust_map_limits();

    printf("Lower bound:\t%.0f\nUpper bound:\t%.0f\n", *(shared_memory5+100), *(shared_memory5+101));

    printf("... Records in memory:\t%d\n", *(shared_memory2+4));

    int j = 0, numdati = 0, casenumber;
    while (j < *(shared_memory2+4)) {

        dataread =*(shared_memory2+11+j);

        if ( dataread >= 50000000 && firstData )  casenumber=0;     // First 'X' position
        else if ( dataread >= 50000000 && !firstData ) casenumber=1;     // Other positions -> writes the preceding vector
        else if ( dataread < 50000000 ) casenumber=2;                    // Energy
        else casenumber = 10;

        switch (casenumber) {
        case 0:
            //dataread -= codePosX;
            vectorMap[j] = dataread;
            j++;

            dataread = *(shared_memory2+11+j);
            //dataread -= codePosY;
            vectorMap[j] = dataread ;
            j++;

            firstData = false;
            break;
        case 1:
            for (int vectorSize = 0; vectorSize < 20000; vectorSize++) { // Writes the preceding vector to shared memory
                if (vectorSize < 3) {
                    *(shared_memory3+numdati) = vectorMap[vectorSize];
                    numdati++;
                }
                else if (vectorMap[vectorSize] != 0) {
                    *(shared_memory3+numdati) = vectorMap[vectorSize];
                    numdati++;
                    *(shared_memory3+numdati) = vectorSize-3;
                    numdati++;
                }
                vectorMap[vectorSize] = 0;
            }

            *(shared_memory3+numdati) = -1;
            numdati++;

            //dataread -= codePosX;
            vectorMap[0] = dataread;
            j++;

            dataread = *(shared_memory2+11+j);
            //dataread -= codePosY;
            vectorMap[1] = dataread;
            j++;

            break;
        case 2:
            if (dataread >= codeDetA && dataread < codeDetB) {
                if (*(shared_memory_cmd+100) == 1) {
                    j++;
                    break;
                }
                else dataread -= codeDetA;
            }

            if (dataread >= codeDetB) {
                if (*(shared_memory_cmd+100) == 0) {
                    j++;
                    break;
                }
                else if (*(shared_memory_cmd+100) == 2) {
                    dataread -= codeDetB;

                    jlo = static_cast<double>(dataread - 1) * calGrad + calOffs;
                    jhi = jlo + calGrad;
                    dataread = static_cast<int>(ranqd1(idum,jlo,jhi));
                }
                else dataread -= codeDetB;
            }

            if (dataread >= *(shared_memory5+100) && dataread <= *(shared_memory5+101)) {
                vectorMap[2] += 1;
                vectorMap[dataread + 3] += 1; // The vectorMap is shifted by 3 positions
            }
            //else printf("!!! Invalid data found : %d\n", dataread);
            j++;
            break;
        default:
            printf("[!] No valid data found");
            break;
        }
    }

    for (int entry = 0; entry < 20000; entry++) { // This last loop serves to write the last pixel of memory
        if(entry<3) {
            *(shared_memory3+numdati) = vectorMap[entry];
            numdati++;
        }
        else if(vectorMap[entry] != 0) {
            *(shared_memory3+numdati) = vectorMap[entry];
            numdati++;
            *(shared_memory3+numdati) = entry - 3;
            numdati++;
        }
        vectorMap[entry]=0;
    }
    *(shared_memory3+numdati) = -1;
    numdati++;
    *(shared_memory3+numdati) = -2;

    if (false)
    {
        //Dump file onto tmp
        string line;
        std::ofstream ofile("tmp.txt");
        if (ofile.is_open())
        {
            for (int i = 0; i < numdati + 1; i++)
            {
                ofile<<shared_memory3[i]<<'\n';
            }
        }
        else {
            printf("[!] Unsuccesful dump\n");
        }
        ofile.close();
    }

    printf("... Map correctly loaded\n... No pixel correction applied\n");
}

void MainWindow::LoadSHM_SumMap() {


    if (*(shared_memory+24)) adjust_map_limits();



    double jlo = 0, jhi = 0;
    double calGrad = static_cast<double>(*(shared_memory_cmd+101)) / static_cast<double>(*(shared_memory_cmd+103));
    double calOffs = static_cast<double>(*(shared_memory_cmd+102)) / static_cast<double>(*(shared_memory_cmd+103));

    bool firstData = true;
    int dataread = 0, vectorMap[20000]= { 0 };
    int numdati = 0;


    int j = 0, casenumber = 10;
    while (j < *(shared_memory2+4)) {
        dataread = *(shared_memory2+11+j);

        if ( dataread >= 50000000 && firstData )        casenumber = 0; // First position, writes coordinates
        else if ( dataread >= 50000000 && !firstData )  casenumber = 1; // Writes the preceding vector
        else if ( dataread < 50000000 )                 casenumber = 2;
        else casenumber = 10;

        switch (casenumber) {
        case 0:
            dataread -= codePosX;
            vectorMap[j] = dataread;
            j++;

            dataread = *(shared_memory2+11+j);
            dataread -= codePosY;
            vectorMap[j] = dataread ;
            j++;

            firstData = false;
            break;
        case 1:
            for (int vectorSize = 0; vectorSize < 20000; vectorSize++) { // Writes the preceding vector to shared memory
                if (vectorSize < 5) {
                    *(shared_memory3+numdati) = vectorMap[vectorSize];
                    numdati++;
                }
                else if (vectorMap[vectorSize] != 0) {
                    *(shared_memory3+numdati) = vectorMap[vectorSize];
                    numdati++;
                    *(shared_memory3+numdati) = vectorSize-5;
                    numdati++;
                }
                vectorMap[vectorSize] = 0;
            }

            *(shared_memory3+numdati) = -1;
            numdati++;

            dataread -= codePosX;
            vectorMap[0] = dataread;
            j++;

            dataread = *(shared_memory2+11+j);
            dataread -= codePosY;
            vectorMap[1] = dataread;
            j++;
            break;
        case 2:
            if (dataread >= codeDetA && dataread < codeDetB) {
                if (*(shared_memory_cmd+100) == 1) {
                    j++;
                    break;
                }
                else dataread -= codeDetA;
            }

            if (dataread >= codeDetB) {
                if (*(shared_memory_cmd+100) == 0) {
                    j++;
                    break;
                }
                else if (*(shared_memory_cmd+100) == 2) {
                    dataread -= codeDetB;

                    jlo = static_cast<double>(dataread - 1) * calGrad + calOffs;
                    jhi = jlo + calGrad;
                    dataread = static_cast<int>(ranqd1(idum,jlo,jhi));
                }
                else dataread -= codeDetB;
            }

            if (dataread >= *(shared_memory5+100) && dataread <= *(shared_memory5+101)) vectorMap[dataread+5] += 1;

            if (dataread >= *(shared_memory5+102) && dataread <= *(shared_memory5+103))       vectorMap[2] += 1;
            else if (dataread >= *(shared_memory5+104) && dataread <= *(shared_memory5+105)) vectorMap[3] += 1;
            else if (dataread >= *(shared_memory5+106) && dataread <= *(shared_memory5+107) ) vectorMap[4] += 1;

            j++;
            break;
        default:
            printf("[!] No valid data found");
            break;
        }
    }

    for (int vectorSize = 0; vectorSize < 20000; vectorSize++) {
        if (vectorSize < 5) {
            *(shared_memory3+numdati) = vectorMap[vectorSize];
            numdati++;
        }
        else if (vectorMap[vectorSize] != 0) {
            *(shared_memory3+numdati) = vectorMap[vectorSize];
            numdati++;
            *(shared_memory3+numdati) = vectorSize - 5;
            numdati++;
        }
        vectorMap[vectorSize]=0;
    }
    *(shared_memory3+numdati) = -1;
    numdati++;
    *(shared_memory3+numdati) = -2;
    printf("... Composed element map correctly loaded\n");
    displaySumImage_SHM();
}
