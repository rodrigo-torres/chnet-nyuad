#ifndef ENUMS_AND_WRAPPERS_H
#define ENUMS_AND_WRAPPERS_H

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>

typedef unsigned long ulong;
typedef const uint8_t int8;

namespace widgets {
enum class pushbuttons : int
{
    openttyx = 0,
    openttyy,
    openttyz,
    openttyk,
    initstagex,
    initstagey,
    initstagez,
    TOTAL_NO
};

enum class spinboxes : int
{
    targetx = 0,
    targety,
    targetz,
    TOTAL_NO
};

enum class menus : int
{
    file = 0,
    map,
    daq,
    daq_channel,
    histo,
    tools,
    about,
    infosoftware,
    infokernel,
    TOTAL_NO
};

enum class actions : int
{
    file_openmap = 0,
    //file_savetxt,
    file_export,
    file_exit,
    map_channelfilter,
    map_pixelsize,
    map_reload,
    map_reloadsum,
    map_show,
    map_viewlive,
    daq_time,
    daq_channel0,
    daq_channel1,
    daq_channel0and1,
//    daq_detectorparams,
    daq_start,
    daq_stop,
    daq_rate,
    histo_openwindow,
    histo_3dim,
    tools_daqparams,
    tools_xraytable,
    tools_camera,
    info_open0,
    info_open1,
    info_open3,
    TOTAL_NO
};

template <class E> ulong index_of(E member)
{
    return static_cast<ulong>(member);
}

template <class E>
ulong operator + (E& member, ulong add)
{
    return static_cast<ulong>(member) + add;
}

template <class E>
ulong operator + (E& member, int add)
{
    return static_cast<int>(member) + add;
}

}

template <class T>
ulong get_index(std::vector<T> vec, T elem)
{
    ulong index = 0;
    for (auto &i : vec)
    {
        if (i == elem)
            return index;
        else
            index++;
    }
    return index = -1;
}

template <class T, class E>
class wrapper
{
public:
    wrapper()
    {
        auto size = static_cast<size_t>(E::TOTAL_NO);
        vec.resize(size);
    }

    typename std::vector<T>::iterator begin()
    {
        return vec.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return vec.end();
    }

    T& at(E en)
    {
       auto ind = static_cast<int>(en);
       try {
           return vec.at(ind);
       } catch (const std::out_of_range &oos) {
           std::cout<<"[!] Wrapper out of range: "<<oos.what()<<'\n';
           std::vector<T> ret(1, nullptr);
           return ret.at(0);

       }
    }

    T& at(const int &i)
    {
        return vec.at(i);
    }
    void push_back(const T &val)
    {
        vec.push_back(val);
    }
private:
    std::vector<T> vec;
};


#endif // ENUMS_AND_WRAPPERS_H
