#include "abc.h"

int get_time_ms() {
    #ifdef WIN64
        return GetTickCount ();
    #else
        struct timeval t;
        gettimeofday (&t, NULL);
        return t.tv_sec*1000 + t.tv_usec/1000;
    #endif	
}