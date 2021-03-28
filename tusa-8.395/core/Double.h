#ifndef DOUBLE____HJKK
#define DOUBLE____HJKK
#include "oscar_buffer.h"
struct Double
{
    double container;
    Double():container(0) {}
    int operator < (const Double& b)const  {
        return container<b.container;
    }
    int operator > (const Double& b)const  {
        return container>b.container;
    }
    bool operator == (const Double& b)const  {
        return container==b.container;
    }
    bool operator != (const Double& b)const  {
        return container!=b.container;
    }

};
oscar_buffer& operator>>(oscar_buffer & b, Double&c);
out_oscar_buffer& operator<<(out_oscar_buffer & b, const Double& c);

#endif
