////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2007-2009 Okinawa Institute of Science and Technology, Japan.
// Copyright (C) 2003-2006 University of Antwerp, Belgium.
//
// See the file AUTHORS for details.
//
// This file is part of STEPS.
//
// STEPS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// STEPS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

/*
 *  Last Changed Rev:  $Rev: 380 $
 *  Last Changed Date: $Date: 2010-11-03 11:04:45 +0900 (Wed, 03 Nov 2010) $
 *  Last Changed By:   $Author: wchen $
 */

#ifndef STEPS_TETEXACT_CRSTRUCT_HPP
#define STEPS_TETEXACT_CRSTRUCT_HPP 1

#include "kproc.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
////////////////////////////////////////////////////////////////////////////////

START_NAMESPACE(steps)
START_NAMESPACE(tetexact)
class KProc;

struct CRGroup {
    CRGroup(int power, uint init_size = 1024) {
        max = pow(2, power);
        sum = 0.0;
        capacity = 1024;
        size = 0;
        indices = (KProc**)malloc(sizeof(KProc*) * init_size);
        if (indices == NULL) {
            std::cerr << "DirectCR: unable to allocate memory for SSA group.\n";
            throw;
        }
        #ifdef SSA_DEBUG
        std::cout << "SSA: CRGroup Created\n";
        std::cout << "power: " << power << "\n";
        std::cout << "max: " << max << "\n";
        std::cout << "capacity: " << capacity << "\n";
        std::cout << "--------------------------------------------------------\n";
        #endif
    }
    
    unsigned                                capacity;
    unsigned                                size;
    double                                  max;
    double                                  sum;
    KProc**                                 indices;
};

struct CRKProcData {
    CRKProcData() {
        recorded = false;
        pow = 0;
        pos = 0;
        rate = 0.0;
    }
    
    bool                                    recorded;
    int                                     pow;
    unsigned                                pos;
    double                                  rate;
};

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(tetexact)
END_NAMESPACE(steps)

#endif

// STEPS_TETEXACT_CRSTRUCT_HPP

// END
