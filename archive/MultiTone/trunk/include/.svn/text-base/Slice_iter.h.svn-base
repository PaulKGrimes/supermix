/* Slice_iter.h
*
*   Written by Paul Grimes, after Bjarne Stroustrop, "The C++ Programming Language
*   : Special Edition" pp672.
*
*   Definition of slice iterator
*/

/* Changes
*   26/04/04 : PKG : Initial code directly from Stroustrop
*/

#include <valarray>

using namespace std;

template<class T> class Slice_iter
{
    valarray<T> *v;     /*<! The originating valarray */
    slice s;            /*<! The slice over the valarray */
    size_t curr;        /*<! Index of the current element */
    
    T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
    
public:
    Slice_iter(valarray<T> *vv, slice ss) : v(vv), s(ss), curr(0) {};
    
    Slice_iter end() const
    {
        Slice_iter t = *this;
        t.curr = s.size(); // Index of last +1 element
        return t;
    }
    
    Slice_iter& operator++() { curr++; return *this; }
    Slice_iter operator++(int) { Slice_iter t=*this; curr++; return t; }
    
    T& operator[](size_t i) { return ref(i); }
    T& operator()(size_t i) { return ref(i); }
    T& operator*() { return ref(curr); }
    
    size_t size() const { return s.size(); }
    
    friend bool operator==<>(const Slice_iter& p, const Slice_iter& q);
    friend bool operator!=<>(const Slice_iter& p, const Slice_iter& q);
    friend bool operator< <>(const Slice_iter& p, const Slice_iter& q);
};
    

// A constant form of Slice_iter
template<class T> class CSlice_iter
{
    valarray<T> *v;     /*<! The originating valarray */
    slice s;            /*<! The slice over the valarray */
    size_t curr;        /*<! Index of the current element */
    
    const T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
    
public:
    CSlice_iter(valarray<T> *vv, slice ss) : v(vv), s(ss), curr(0) {};
    
    CSlice_iter end() const
    {
        CSlice_iter t = *this;
        t.curr = s.size(); // Index of last +1 element
        return t;
    }
    
    CSlice_iter& operator++() { curr++; return *this; }
    CSlice_iter operator++(int) { CSlice_iter t=*this; curr++; return t; }
    
    const T& operator[](size_t i) { return ref(i); }
    const T& operator()(size_t i) { return ref(i); }
    const T& operator*() { return ref(curr); }
    
    size_t size() const { return s.size(); }
    
    friend bool operator==<>(const CSlice_iter& p, const CSlice_iter& q);
    friend bool operator!=<>(const CSlice_iter& p, const CSlice_iter& q);
    friend bool operator< <>(const CSlice_iter& p, const CSlice_iter& q);
};

template<class T> bool operator==(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return p.curr==q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
};


template<class T> bool operator!=(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return !(p==q);
};


template<class T> bool operator< (const Slice_iter<T>& p, const Slice_iter<T>& q)
{
    return p.curr<q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
};


template<class T> bool operator==(const CSlice_iter<T>& p, const CSlice_iter<T>& q)
{
    return p.curr==q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
};


template<class T> bool operator!=(const CSlice_iter<T>& p, const CSlice_iter<T>& q)
{
    return !(p==q);
};


template<class T> bool operator<(const CSlice_iter<T>& p, const CSlice_iter<T>& q)
{
    return p.curr<q.curr && p.s.stride()==q.s.stride() && p.s.start()==q.s.start();
};


