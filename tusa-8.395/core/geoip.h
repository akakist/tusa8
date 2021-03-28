#ifndef GEO____IP__H
#define GEO____IP__H
struct geoip_location_dump
{
    string country,region,city,postalCode;
    string coord;
};
out_oscar_buffer & operator<< (out_oscar_buffer& b,const geoip_location_dump &s);
oscar_buffer & operator>> (oscar_buffer& b,  geoip_location_dump &s);

#endif
