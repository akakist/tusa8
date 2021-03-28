#include "stdafx.h"
#include "oscar_buffer.h"
#include "Double.h"
#include "str_lib.h"
oscar_buffer& operator>>(oscar_buffer & b, Double&c)
{
    string s;
    b>>s;
    c.container=atof(s.c_str());
    return b;
}
out_oscar_buffer& operator<<(out_oscar_buffer & b, const Double& c)
{
    b<<ftostring(c.container);
    return b;
}


