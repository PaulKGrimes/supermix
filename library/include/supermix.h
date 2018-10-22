// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file supermix.h
 *
 * Automatically <code>\#include</code> almost the entire supermix library.
 * This header is used only for convenience, so that a user can use the
 * library with the single line <code>\#include "supermix.h"</code> rather
 * than having to include each header file separately.
 *
 * Including this header file may include a lot of things you don't need and
 * thus make compiles slower, especially if you are only using a small
 * subset of the supermix library.  If this is a problem, then include the
 * specific headers you need instead of this supermix.h.
 */
// **************************************************************************

#ifndef SUPERMIX_H
#define SUPERMIX_H

// c++ input/output
#include <iostream>
#include <fstream>
#include <iomanip>
#include "ioext.h"

// Primitive data objects, containers, utilities:
#include "global.h"
#include "units.h"
#include "matmath.h"
#include "error.h"
#include "io.h"
#include "datafile.h"
#include "parameter.h"
#include "parameter/complex_parameter.h"
#include "sdata.h"
#include "real_interp.h"
#include "complex_interp.h"

// Basic circuit components:
#include "nport.h"
#include "circuit.h"
#include "elements.h"
#include "sources.h"
#include "transformer.h"
#include "trlines.h"
#include "surfaceZ.h"

// More complex components:
#include "instrument.h"
#include "attenuator.h"
#include "circulator.h"
#include "delay.h"
#include "hybrid.h"
#include "radial_stub.h"
#include "antenna.h"
#include "sdata_interp.h"
#include "via.h"
#include "deembed.h"

// The FET models, and amplifier-related classes:
#include "fet.h"
#include "hemt.h"
#include "ampdata.h"

// Nonlinear junction and mixer objects:
#include "junction.h"
#include "sisdevice.h"
#include "mixer.h"

// Optimizer stuff
#include "sweeper.h"
#include "optimizer.h"
#include "error_terms.h"
#include "error_func.h"
#include "powell.h"
#include "montecarlo.h"

using namespace std;

#endif /* SUPERMIX_H */
