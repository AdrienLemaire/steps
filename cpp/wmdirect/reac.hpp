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

#ifndef STEPS_WMDIRECT_REAC_HPP
#define STEPS_WMDIRECT_REAC_HPP 1


// STL headers.

// STEPS headers.
#include "../common.h"
#include "kproc.hpp"
#include "../solver/reacdef.hpp"
#include "../solver/types.hpp"


////////////////////////////////////////////////////////////////////////////////

START_NAMESPACE(steps)
START_NAMESPACE(wmdirect)

////////////////////////////////////////////////////////////////////////////////

// Forward declarations
class Patch;
class Comp;

////////////////////////////////////////////////////////////////////////////////

class Reac
: public steps::wmdirect::KProc
{

public:

    ////////////////////////////////////////////////////////////////////////
    // OBJECT CONSTRUCTION & DESTRUCTION
    ////////////////////////////////////////////////////////////////////////

    Reac(steps::solver::Reacdef * rdef, Comp * comp);
    ~Reac(void);

    ////////////////////////////////////////////////////////////////////////
    // CHECKPOINTING
    ////////////////////////////////////////////////////////////////////////
    /// checkpoint data
    void checkpoint(std::fstream & cp_file);

    /// restore data
    void restore(std::fstream & cp_file);

    ////////////////////////////////////////////////////////////////////////
    // DATA ACCESS
    ////////////////////////////////////////////////////////////////////////

    static const int INACTIVATED = 1;

    bool active(void) const;

    bool inactive(void) const
    { return (! active()); }


    ////////////////////////////////////////////////////////////////////////
    // VIRTUAL INTERFACE METHODS
    ////////////////////////////////////////////////////////////////////////

    void setupDeps(void);
    bool depSpecComp(uint gidx, Comp * comp);
    bool depSpecPatch(uint gidx, Patch * patch);
    void reset(void);
    double rate(void) const;
    std::vector<uint> const & apply(void);

	uint updVecSize(void) const
    { return pUpdVec.size(); }

    ////////////////////////////////////////////////////////////////////////

    inline steps::solver::Reacdef * defr(void) const
    { return pReacdef; }

    void resetCcst(void);

    double c(void) const
    { return pCcst; }

    double h(void) const
    { return (rate()/pCcst); }

    ////////////////////////////////////////////////////////////////////////

private:

    ////////////////////////////////////////////////////////////////////////

    steps::solver::Reacdef            * pReacdef;
    Comp                              * pComp;
    std::vector<uint>                   pUpdVec;
    /// Properly scaled reaction constant.
    double                              pCcst;

    ////////////////////////////////////////////////////////////////////////

};

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(wmdirect)
END_NAMESPACE(steps)

#endif
// STEPS_WMDIRECT_REAC_HPP

// END
