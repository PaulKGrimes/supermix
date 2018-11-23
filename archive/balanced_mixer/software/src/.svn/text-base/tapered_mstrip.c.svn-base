
#include "tapered_mstrip.h"
#include "error.h"
#include <math.h>

tapered_mstrip::tapered_mstrip() : data_ptr_nport()
{
    info.source = false;
    // Unless a separate temperature parameter gets added, tapers are passive
    info.active = false;
    nsections = 31;

    for(int i=0; i<=nsections; i++)
        sec[i].sub_thick = &sub_thick;

    sec[0].width = &width1;
    sec[0].length = &length;
    sec[nsections].width = &width2;
    sec[nsections].length = &length;

    for(int i=0; i<nsections; i++)
        taper.connect(sec[i], 2, sec[i+1], 1);

    taper.add_port(sec[0], 1);
    taper.add_port(sec[nsections], 2);
}

tapered_mstrip & tapered_mstrip::substrate(dielectric & d)
{
    sub = &d;

    for(int i=0; i<=nsections; i++)
        sec[i].substrate(*sub);

    return *this;
}

tapered_mstrip & tapered_mstrip::superstrate(dielectric & d)
{
    super = &d;

    for(int i=0; i<=nsections; i++)
        sec[i].superstrate(*super);

    return *this;
}

tapered_mstrip & tapered_mstrip::top_strip(surfimp & s)
{
    top = &s;

    for(int i=0; i<=nsections; i++)
        sec[i].top_strip(*top);

    return *this;
}

tapered_mstrip & tapered_mstrip::ground_plane(surfimp & s)
{
    ground = &s;

    for(int i=0; i<=nsections; i++)
        sec[i].ground_plane(*ground);

    return *this;
}

void tapered_mstrip::recalc(bool noise)
{
    parameter sect_len = length/(double)(nsections+1);

    for(int i=0; i<=nsections; i++)
    {
        sec[i].length = sect_len;
        sec[i].width = width1 + (width2-width1)*(double)i/(double)(nsections+1);
    }

    // Now that the microstrips are set, have the circuit calculate itself.
    data_ptr = (noise) ? &taper.get_data() : &taper.get_data_S();

    // Print out microstrip sizes for debugging.
    cout << "width1 = " << width1/Micron << endl;
    cout << "width2 = " << width2/Micron << endl;
    cout << "length = " << length/Micron << endl;
    for(int i=0; i <= nsections; i++)
    {
        cout << "Section " << i << ": " << sec[i].width/Micron
             << " microns wide X " << sec[i].length/Micron
             << " microns long." << endl;
    }
    // End of debug code.
}

tapered_mstrip::tapered_mstrip(const tapered_mstrip & t)
{
    info.source = false;
    // Unless a separate temperature parameter gets added, stubs are passive
    info.active = false;
    nsections = t.nsections;

    width1 = t.width1;
    width2 = t.width2;
    length = t.length;
    sub_thick = t.sub_thick;

    if(t.sub != 0)
        substrate(*t.sub);
    if(t.super != 0)
        superstrate(*t.super);
    if(t.top != 0)
        top_strip(*t.top);
    if(t.ground != 0)
        ground_plane(*t.ground);
  
    for(int i=0; i<=nsections; i++)
        sec[i].sub_thick = &sub_thick;

    sec[0].width = &width1;
    sec[0].length = &length;

    for(int i=0; i<nsections; i++)
        taper.connect(sec[i], 2, sec[i+1], 1);

    taper.add_port(sec[0], 1);
    taper.add_port(sec[nsections], 2);
}

tapered_mstrip & tapered_mstrip::operator=(const tapered_mstrip & t)
{
    // Beware of self assignment: r = r
    if(this != &t)
    {
        width1 = t.width1;
        width2 = t.width2;
        length = t.length;
        sub_thick = t.sub_thick;
    

        if(t.sub != 0)
            substrate(*t.sub);
        if(t.super != 0)
            superstrate(*t.super);
        if(t.top != 0)
            top_strip(*t.top);
        if(t.ground != 0)
            ground_plane(*t.ground);

    }
    return *this;
}
