////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2007-2011�Okinawa Institute of Science and Technology, Japan.
// Copyright (C) 2003-2006�University of Antwerp, Belgium.
//
// See the file AUTHORS for details.
//
// This file is part of STEPS.
//
// STEPS�is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// STEPS�is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.�If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

/*
 *  Last Changed Rev:  $Rev: 410 $
 *  Last Changed Date: $Date: 2011-04-07 16:11:28 +0900 (Thu, 07 Apr 2011) $
 *  Last Changed By:   $Author: iain $
 */

#ifndef STEPS_TETEXACT_COMP_HPP
#define STEPS_TETEXACT_COMP_HPP 1


// STL headers.
#include <cassert>
#include <vector>
#include <fstream>

// STEPS headers.
#include "../common.h"
//#include <steps/tetexact/kproc.hpp>
//#include <steps/tetexact/patch.hpp>
#include "tet.hpp"
#include "../solver/compdef.hpp"
#include "../solver/types.hpp"

////////////////////////////////////////////////////////////////////////////////

START_NAMESPACE(steps)
START_NAMESPACE(tetexact)

////////////////////////////////////////////////////////////////////////////////

NAMESPACE_ALIAS(steps::tetexact, stex);

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.
class Comp;

// Auxiliary declarations.
typedef Comp *                          CompP;
typedef std::vector<CompP>              CompPVec;
typedef CompPVec::iterator              CompPVecI;
typedef CompPVec::const_iterator        CompPVecCI;

////////////////////////////////////////////////////////////////////////////////

class Comp
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    Comp(steps::solver::Compdef * compdef);
    ~Comp(void);

    ////////////////////////////////////////////////////////////////////////
    // CHECKPOINTING
    ////////////////////////////////////////////////////////////////////////
    /// checkpoint data
    void checkpoint(std::fstream & cp_file);

    /// restore data
    void restore(std::fstream & cp_file);

    /// Checks whether the Tet's compdef() corresponds to this object's
    /// CompDef. There is no check whether the Tet object has already
    /// been added to this Comp object before (i.e. no duplicate checking).
    ///
    void addTet(stex::Tet * tet);

    ////////////////////////////////////////////////////////////////////////

    inline void reset(void)
    { def()->reset(); }

    ////////////////////////////////////////////////////////////////////////
    // DATA ACCESS
    ////////////////////////////////////////////////////////////////////////

    inline steps::solver::Compdef * def(void) const
    { return pCompdef; }

    inline double vol(void) const
    { return pVol; }

    inline double * pools(void) const
    { return def()->pools(); }

    void modCount(uint slidx, double count);

    inline uint countTets(void) const
    { return pTets.size(); }

    stex::Tet * pickTetByVol(double rand01) const;

    inline TetPVecCI bgnTet(void) const
    { return pTets.begin(); }
    inline TetPVecCI endTet(void) const
    { return pTets.end(); }

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    steps::solver::Compdef            * pCompdef;
    double                              pVol;

    TetPVec                             pTets;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(tetexact)
END_NAMESPACE(steps)

#endif

// STEPS_TETEXACT_COMP_HPP

// END
