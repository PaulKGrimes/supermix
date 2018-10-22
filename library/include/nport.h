// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file nport.h
 *
 * Holds the heart of the linear %circuit simulation code.
 * This includes:
 *   @li class device
 *   @li class nport
 *   @li class alias
 *   @li class data_ptr_nport
 *
 * @author John Ward
 * @date September 5, 1997
 */
// **************************************************************************

#ifndef NPORT_H
#define NPORT_H

#include "device.h"
#include "global.h"
#include "port.h"
#include "state_tag.h"
#include "parameter.h"
#include "sdata.h"

// **************************************************************************

/**
 * @class nport
 *
 * This is the most generic linear %circuit/element device.
 * All circuits and primitive elements are subclasses of
 * class nport.
 *
 * Class nport is an abstract class.  It will never be
 * instantiated directly.
 */
class nport : public device
{

public:
  /**
   * The following structure, returned by get_data_info(), contains flags which
   * may be used to speed circuit calculations. The default behavior is for all
   * flags to be set to true, in order to be on the safe side.
   */
  struct data_info {

    /** True if the noise correlation matrix may be nonzero. */
    bool noise;

    /**
     * True if a passive (Johnson) noise calculation at the current value of
     * device::T is inadequate (ie, must have recalc() calculate the noise).
     * If false, then the noise correlation matrix is proportional to:
     * <pre>
     *   device::T*(1 - data.S*dagger(data.S))
     * </pre>
     * where "1" is a unit matrix.
     */
    bool active;

    /**
     * True if the source vector may be nonzero (ie, must have recalc() do it).
     */
    bool source;
  };

protected:
  /** data includes the scattering matrix, noise matrix, size, etc. */
  sdata data;

  /** holds the flags returned by get_data_info() */
  data_info info;

  /**
   * The function for calculating sdata. recalc() should do the following:
   *  @li (1) Read device::f, device::Z0, and, by default, device::T to get
   *          the operating conditions for the response calculation.
   *  @li (2) Calculate the S, C, and B matrix values and store them to
   *          nport::data, with all values normalized using device::Z0.
   *  @li (3) Store the normalizing impedance to nport::data by calling
   *          data.set_znorm().
   */
  virtual void recalc() = 0;

  /**
   * If the following function is implemented, it should only calculate the
   * S matrix and B vector parts of data, leaving the C matrix part stale.
   */
  virtual void recalc_S() { recalc(); }

public:
  /**
   * The default constructor.
   *
   * @param n the number of ports (defaults to 0.)
   */
  nport(int n = 0) : data(n)
    { info.noise = info.active = info.source = true; }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~nport() { }

  /** @return the nport data in one chunk after recalculating the nport */
  virtual const sdata& get_data() { last_state.reset(); recalc(); return data; }

  /**
   * Return the nport data, recalculating only if calc_id is different from
   * when we last recalculated.
   *
   * @param calc_id used to determine if this device needs to be recalculated
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data(state_tag calc_id)
  {
    if(last_state == calc_id) return data;
    last_state = calc_id;
    recalc();
    return data;
  }

  /**
   * Return the nport data.  Do not perform a calculation.
   *
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_last_data() const { return data; }

  /**
   * Special function to save time in circuits: calculate only the S matrix
   * and B vector parts of the sdata. The noise correlation matrix will 
   * contain stale values.
   *
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data_S() { last_state.reset(); recalc_S(); return data; }

  /**
   * Special function to save time in circuits: calculate only the S matrix
   * and B vector parts of the sdata only if calc_id is different from
   * when we last recalculated. The noise correlation matrix will 
   * contain stale values.
   *
   * @param calc_id used to determine if this device needs to be recalculated
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data_S(state_tag calc_id)
  {
    if(last_state == calc_id) return data;
    last_state = calc_id;
    recalc_S();
    return data;
  }

  /**
   * The number of ports this device has.
   *
   * @return the number of ports
   */
  virtual int size() { return data.size(); }  // return the number of ports

  /** @return info about the sdata of the device */
  virtual const data_info & get_data_info() { return info; }

  /** @return true if the noise correlation matrix should be nonzero */
  virtual bool has_noise() { return get_data_info().noise; }

  /**
   * @return true if passive (Johnson) noise calculation at the current value
   * of device::T is inadequate (ie, must have recalc() calculate the noise).
   */
  virtual bool is_active() { return get_data_info().active; }

  /**
   * @return true if the source vector may be nonzero
   * (ie, must have recalc() do it).
   */
  virtual bool is_source() { return get_data_info().source; }

  /**
   * Recalculate the nport and return a single scattering parameter.
   *
   * @param p1 row of the scattering matrix
   * @param p2 column of the scattering matrix
   * @return the element of the scattering matrix
   */
  virtual complex S(int p1, int p2);

  /**
   * Recalculate the nport and return a single element of the noise
   * correlation matrix.
   *
   * @param p1 row of the noise correlation matrix
   * @param p2 column of the noise correlation matrix
   * @return the element of the noise correlation matrix
   */
  virtual complex C(int p1, int p2);

  /**
   * Recalculate the nport and return a single element of the source vector.
   *
   * @param p the port
   * @return the element of the source vector at this freq
   */
  virtual complex B(int p);

  /**
   * This is a trivial function that returns a port's port id.
   *
   * @param index the index of the port
   * @return the port, or (0,0) if index is not in proper range.
   */
  virtual port get_port(int index);

  /**
   * This is a trivial function that returns a port's index.
   *
   * @param p the port we are searching for
   * @return the index of the port, or 0 if the port is not found
   */
  virtual int get_port(port p);

  /**
   * A function for those rare cases when an nport may not use Index_1
   * indexing.
   */
  virtual v_index_mode mode() { return data.mode(); }  // sdata indexing mode

};

// **************************************************************************

/**
 * @class data_ptr_nport
 *
 * Some classes don't store their scattering and noise
 * matrices locally.  For example, they might use another
 * subclass of nport to do the calculation for them.
 *
 * data_ptr_nport uses a pointer to keep track of where its
 * sdata is.  This avoids the inefficiency of copying the
 * matrices over from someplace else.  It saves the programmer
 * the trouble of overloading functions such as get_data().
 *
 * Class data_ptr_nport should never be instantiated directly.
 */
class data_ptr_nport : public nport
{
protected:
  /**
   * data includes the scattering matrix, noise matrix, size, etc.
   * You must be careful that this pointer always points to a
   * valid sdata object.
   */
  const sdata * data_ptr;

public:
  /**
   * Constructor:
   * Make data_ptr point to data so that it won't cause a core dump.
   */
  data_ptr_nport() : nport(0), data_ptr(&data) { }

  /**
   * Copy constructor.
   * Make sure data_ptr doesn't point to data of the instance we're copying.
   */
  data_ptr_nport(const data_ptr_nport & c) : nport(c), data_ptr(&data) { }

  /**
   * Assignment operator.
   * Make sure data_ptr doesn't point to data of the instance we're copying.
   */
  data_ptr_nport & operator=(const data_ptr_nport & c)
  { data = c.data; data_ptr = &data; return *this; }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~data_ptr_nport() { }

  /** Return the nport data in one chunk after recalculating the nport. */
  virtual const sdata& get_data()
  {
    last_state.reset(); recalc(); return *data_ptr;
  }

  /**
   * Return the nport data, recalculating only if calc_id is different from
   * when we last recalculated.
   *
   * @param calc_id used to determine if this device needs to be recalculated
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data(state_tag calc_id)
  {
    if(last_state == calc_id) return *data_ptr;
    last_state = calc_id;
    recalc();
    return *data_ptr;
  }

  /**
   * Special function to save time in circuits: calculate only the S matrix
   * and B vector parts of the sdata. The noise correlation matrix will
   * contain stale values.
   *
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data_S()
  {
    last_state.reset(); recalc_S(); return *data_ptr;
  }

  /**
   * Special function to save time in circuits: calculate only the S matrix
   * and B vector parts of the sdata only if calc_id is different from
   * when we last recalculated. The noise correlation matrix will
   * contain stale values.
   *
   * @param calc_id used to determine if this device needs to be recalculated
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_data_S(state_tag calc_id)
  {
    if(last_state == calc_id) return *data_ptr;
    last_state = calc_id;
    recalc_S();
    return *data_ptr;
  }

  /**
   * Return the nport data.  Do not perform a calculation.
   *
   * @return the scattering matrix, noise correlation matrix, etc.
   */
  virtual const sdata& get_last_data() const { return *data_ptr; }

  /**
   * The number of ports this device has.
   *
   * @return the number of ports
   */
  virtual int size() { return data_ptr->size(); } // return the number of ports

  /**
   * A function for those rare cases when an nport may not use Index_1
   * indexing.
   */
  virtual v_index_mode mode() { return data_ptr->mode(); }
};

// **************************************************************************

/**
 * @class alias
 *
 * Used when more than one identical nport %device or circuit is needed.
 *
 * If more than one copy of exactly the same circuit or nport %device is
 * needed, it may be desirable to create a single %device and then make
 * aliases for the rest.  For example, imagine that five identical
 * %microstrip sections are needed in a circuit.  The best way to do this
 * is to create a single microstrip, and then construct four alias instances
 * from it.  If later the %microstrip is changed, such as to change the
 * substrate thickness, the alias instances will also change accordingly.
 *
 * An alias has its own id number so that it can be used in a circuit, but
 * it relies on another device to calculate its sdata.  Thus, it doesn't
 * have its own parameters, because it doesn't need any.
 */
class alias : public data_ptr_nport
{
private:
  /** A pointer to the nport that we are aliasing. */
  nport *original;

  /** Recalc just gets the data pointer from the original. */
  void recalc()   { if(original) data_ptr = &original->get_data(); } 

  /** Recalc just gets the data pointer from the original. */
  void recalc_S() { if(original) data_ptr = &original->get_data_S(); } 

public:
  // The default constructor.
  alias() : data_ptr_nport(), original(0) { }

  /**
   * @param o a reference to the device we are aliasing.
   */
  alias(nport & o) : data_ptr_nport(), original(&o) { }

  // return the number of ports
  // (need this definition since data may not be properly sized until get_data())
  int size() { return (original) ? original->size() : 0; }

  /** Refer to the original device to answer this question.... */
  const nport::data_info & get_data_info()
    { return (original) ? original->get_data_info() : info; }

  /**
   *Assignment operator.  We can assign any nport to an alias.
   *
   * @param n any nport device to be aliased
   */
  alias & operator=(nport & n);
};

// **************************************************************************

/**
 * @class simple_nport
 *
 * simple_nport doesn't do very much. It provides public access
 * to its data; recalc() doesn't do anything.
 */
class simple_nport : public nport
{

public:
  /**
   * The constructor can size the data matrix.
   *
   * @param n the number of ports (defaults to 0.)
   */
  simple_nport(int n = 0) : nport(n) { }

  /** @return a reference to the internal sdata object */
  sdata & operator()() { return data; }

  /** @return a reference to the data_info object so you can change it */
  data_info & set_info() { return info; }

private:
  /** recalc() doesn't do anything. */
  void recalc()   { }

  /** recalc_S() doesn't do anything. */
  void recalc_S() { }

};

#endif /* NPORT_H */
