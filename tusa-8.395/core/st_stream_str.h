
#ifndef ST_STUFFSTREMASTR_H
#define ST_STUFFSTREMASTR_H
#include "st_stuff.h"
class st_stream_str:public st_stream
{
public:
    std::string container;
    int inidx;
    int readsome (void *, int );
    int writesome (const void *, int ) const;
    ~st_stream_str() {}
    st_stream_str() {
        inidx=0;
    }

};
#endif
