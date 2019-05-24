// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// mixer_helper.h
//
// mixer_helper.h holds the private members and classes defined within
// the mixer class.
//
// THIS FILE IS MEANT TO BE INCLUDED ONLY BY mixer.h AND WITHIN THE
// DEFINITION OF CLASS mixer !!!
//
// holds classes mixer::balancer and mixer::analyzer
//
// F. Rice 12/15/98
// ********************************************************************

// Here are the private member data and functions of class mixer:

// class mixer : public data_ptr_nport
private:

  void auto_state();                   // fix invalid junction states or exit.
  void recalc();                       // the small signal mixer analysis.
  void changed()                       // tell the caculators that the mixer has
  {                                    // changed, so their structures will be rebuilt.
    balance_.changed(); ssignal_.changed(); tsignal_.changed();
    balance_not_ok_flag = (num_junctions != 0);
  }

  static unsigned global_mixer_index;  // used to detect multiple mixer circuits
  int max_harmonics;                   // how many harmonics
  int num_junctions;                   // how many junctions
  double LO_saved;                     // the LO freq used at last balance time
  int balance_init_flag;               // if nonzero, balance() initializes states
  int auto_balance_flag;               // if nonzero, recalc() will call balance()
  int balance_not_ok_flag;             // something changed since last balance

  nport *bias_circuit, *if_circuit, *rf_circuit;   // the linear circuit objects
  std::vector <nport *> term;          // rf_circuit port terminators for balance
  std::vector<generator> default_term; // default Z0 terminators for RF circuit
  std::vector <junction *> junc;       // the junction objects

// ********************************************************************

// This is the class which actually performs the linear balance operation.
// It uses a multidimensional Newton-Raphson algorithm inherited from
// class newton.

class balancer : private newton
{
public:

  balancer(mixer &);// The constructor

  int operator()(); // Perform a harmonic balance of the mixer, using
                    // the current operating state as a starting point.

  void i_state();   // initialize the junction operating states using
                    // the linear circuit voltages, but do not balance

  inline void changed() { must_rebuild = 1; }
                    // tell balancer that the mixer circuit has
                    // changed, so that it will rebuild its internal
                    // structures on the next balance. This must be called
                    // anytime num_junctions, max_harmonics, rf_circuit,
                    // or term changes.

  balancer & parameters(
    int MAXITS,     // maximum number of iterations
    double TOLF,    // single voltage balance error tolerance
    double TOLMIN,  // variance of all voltages from balance tolerance
    double TOLX,    // change in a single voltage step tolerance
    double ALF      // if variance of all voltages changes by less than
                    // this, assume at a local minimum of the error function
    );

  int iterations()  // the number of iterations required by the most recent
  { return iter; }  // balance operation.

private:
  // member functions:
  void init();       // set up balancer to start the balance
  void calc();       // calculate currents and derivatives
  void rebuild();    // if reqd, rebuild data structures and clear must_rebuild.
  void fill_data();  // fetch all the linear circuit sdatas

  // data:
  mixer & mix;                    // reference to this mixer object
  int must_rebuild;               // flag to tell init() to rebuild data
  real_vector ival;               // temporarily hold junction currents
  int iter;                       // iteration counter
  std::vector<const Matrix*> pY;  // will point to junction Ymn
  std::vector<sdata> linear;      // will hold linear circuit sdatas
  std::vector<generator> default_term; // our Z0 terminations
  circuit temp;                   // hold the terminated RF circuit
  struct
  {
    // these values define the internal representation for the vectors
    // used by the newton member functions:
    int length,        // the length of newton::xlast
      harm_inc,        // index increment to step to next harmonic
      junc_inc,        // index increment to step to next junction
      imag_inc;        // index increment to step to imaginary part
  }
  rep;                 // the scheme is established by init()

  // helper functions (n is a junction index, m is a harmonic number):
  inline int index(int n, int m);     // calculate location in representation
  inline Complex B(int n, int m);          // linear source vector element
  inline Complex S(int n1, int n2, int m); // linear S matrix element
  inline Complex Y(int n, int m1, int m2); // junction Y matrix element
  Complex SV(int n, int m, const real_vector & V); // S*V matrix element


  // more helpers: these functions move junction state data to and from the
  // internal newton vector representations:
  void to_rep(real_vector & rep, const Vector & state, int n);
  void fm_rep(Vector & state, const real_vector & rep, int n);

}; // class mixer::balancer

friend class mixer::balancer;

// ********************************************************************

// This is the class which performs the small signal and noise analysis
// of a previously balanced mixer

class analyzer
{
public:

  analyzer(mixer &);// The constructor

  const sdata & operator()();  // Perform the small signal and noise
                    // analyses and return the resulting sdata object

  inline void changed() { must_rebuild = 1; }
                    // tell analyzer that the mixer circuit has
                    // changed, so that it will rebuild its internal
                    // structures on the next call. This must be called
                    // anytime num_junctions, max_harmonics, rf_circuit,
                    // or term changes.

  analyzer & terminate_rf(int); // tell analyzer whether or not it should
                    // terminate the rf circuit with balance terminators
                    // when performing the analysis.
                    //   Nonzero argument: terminate first
                    //   Zero argument:    don't terminate
                    // Applies to all subsequent calls to operator ().

private:
  // member functions:
  void rebuild();   // if reqd, rebuild data structures and clear must_rebuild.
  void fill_data(); // fetch all the component circuit and junction sdatas

  // data:
  sdata result;                   // the results of the analysis
  mixer & mix;                    // reference to this mixer object
  int must_rebuild;               // flag to rebuild data
  int terminate_flag;             // must terminate rf circuit if set
  std::vector<sdata> linear_p;    // will hold linear circuit sdatas for h>=0
  std::vector<sdata> linear_m;    // will hold linear circuit sdatas for h<0
  std::vector<sdata> junctions;   // will hold junction response sdatas
  circuit temp;                   // hold the terminated RF circuit, if required
  Matrix T_, X_, Y_;              // temporaries
  int h_low, h_high, h_size, m_low, m_high, n_low;  // limits for indexes

  // helper functions (n is a port index, m a junction index, h a harmonic number)
  // ranges on the indices:
  // h in: { -mix.max_harmonics, +mix.max_harmonics }
  // m in: { m_low, m_high }, where: m_high - m_low + 1 = mix.num_junctions
  // n in: if (h == 0): { mix.num_junctions + 1, mix.if_circuit->size() }
  //       if (h != 0 && terminate_flag == 0):
  //          { mix.num_junctions + 1, mix.rf_circuit->size() }
  //       if (h != 0 && terminate_flag != 0):
  //          { }

  int n_high(int h);   // return maximum external port index at harmonic h
  inline int Delta(int i, int j)  // Kronecker Delta
  { return (i == j); }

  inline Complex & S11(int n1, int n2, int h); // linear S joining two ports
  inline Complex & S12(int n, int m, int h);   // linear S joining junction to port
  inline Complex & S21(int m, int n, int h);   // linear S joining port to junction
  inline Complex & S22(int m1, int m2, int h); // linear S joining two junctions

  inline Complex & C11(int n1, int n2, int h); // linear C joining two ports
  inline Complex & C12(int n, int m, int h);   // linear C joining junction to port
  inline Complex & C21(int m, int n, int h);   // linear C joining port to junction
  inline Complex & C22(int m1, int m2, int h); // linear C joining two junctions

  inline Complex & S(int n1, int n2, int h1, int h2); // access result S matrix
  inline Complex & C(int n1, int n2, int h1, int h2); // access result C matrix

  inline Complex & Sj(int m, int h1, int h2);  // junction S matrix element
  inline Complex & Cj(int m, int h1, int h2);  // junction C matrix element

  inline Complex & T(int m1, int m2, int h1, int h2); // access T_ matrix
  inline Complex & X(int n, int m, int h1, int h2);   // access X_ matrix
  inline Complex & Y(int n, int m, int h1, int h2);   // access Y_ matrix

  inline Complex * Xrow(int n, int h);
  inline int Xcol(int m, int h);
  inline Complex * Yrow(int n, int h);
  inline int Ycol(int m, int h);

  void calc_T();    // calculate the T_ matrix
  void calc_X();    // calculate the X_ matrix
  void calc_Y();    // calculate the Y_ matrix

}; // mixer::analyzer

friend class mixer::analyzer;

mixer::balancer balance_; // the actual balancer object
mixer::analyzer ssignal_; // the small signal analysis object
mixer::analyzer tsignal_; // the analysis object for terminated rf_circuit ports

// end of the private section of class mixer
