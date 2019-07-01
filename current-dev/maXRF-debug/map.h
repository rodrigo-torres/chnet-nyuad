#ifndef MAP_H
#define MAP_H

#include <iterator>
#include <algorithm>
#include <vector>

#include <QInputDialog>
#include <QString>
#include <QDebug>

using namespace std;
extern vector<int> x_coord;
extern vector<int> y_coord;
extern vector<int> integral;
extern size_t map_size;
extern int x_len, y_len, x_step, y_step, min_x, min_y;

typedef struct punto_t{
    int point_x;
    int point_y;
} punto;

typedef struct pixel_t{
    ~pixel_t() {
        if (total != nullptr) delete[] total;
    }
    int dimensione;
    punto *total;  /// (dimensione*dimensione) punti
} Pixel_BIG;

extern Pixel_BIG *Pointer;

class map {

};

#endif // MAP_H
