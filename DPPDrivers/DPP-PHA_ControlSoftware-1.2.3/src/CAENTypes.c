
#include <CAENTypes.h>

int32_t _dppDescrInited = 0;

int32_t c_initDPPDescr() {
    int32_t ret = 0;
    if(_dppDescrInited)
        goto QuitFunction;

    DPP_CI_Descr.DPPType = c_DPPType_CI;
    DPP_CI_Descr.ENType = TYPE_INT16;
    DPP_CI_Descr.TSType = TYPE_UINT32;

    DPP_PSD_Descr.DPPType = c_DPPType_PSD;
    DPP_PSD_Descr.TSType = TYPE_UINT32;
    DPP_PSD_Descr.ENType = TYPE_INT32;

    DPP_PHA_Descr.DPPType = c_DPPType_PHA;
    DPP_PHA_Descr.TSType = TYPE_UINT64;
    DPP_PHA_Descr.ENType = TYPE_UINT16;

    DPP_None_Descr.DPPType = c_DPPType_None;
    DPP_None_Descr.TSType = TYPE_INT8;
    DPP_None_Descr.ENType = TYPE_INT8;

    _dppDescrInited = 1;

QuitFunction:
    return ret;
}

int32_t c_getSizeOfType(c_type_t t) {
    switch(t) {
    case TYPE_INT8: {
        return sizeof(int8_t);
                    }
    case TYPE_UINT8: {
        return sizeof(uint8_t);
                     }
    case TYPE_INT16: {
        return sizeof(int16_t);
                     }
    case TYPE_UINT16: {
        return sizeof(uint16_t);
                      }
    case TYPE_INT32: {
        return sizeof(int32_t);
                     }
    case TYPE_UINT32: {
        return sizeof(uint32_t);
                      }
    case TYPE_INT64: {
        return sizeof(int64_t);
                     }
    case TYPE_UINT64: {
        return sizeof(uint64_t);
                      }
    case TYPE_LONG: {
        return sizeof(long);
                    }
    case TYPE_DOUBLE: {
        return sizeof(double);
                      }
    case TYPE_CHAR: {
        return sizeof(char);
                    }
    case TYPE_SHORT: {
        return sizeof(short);
                     }

    case TYPE_RESCODE:
    case TYPE_STRING:
    case TYPE_MEMORY:
    case TYPE_NONE:
    default: {
        return -1;
             }
    }
}