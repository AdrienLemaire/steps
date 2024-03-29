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


// Standard library & STL headers.
#include <vector>
#include <cassert>

// STEPS headers.
#include "../common.h"
#include "kproc.hpp"

////////////////////////////////////////////////////////////////////////////////

NAMESPACE_ALIAS(steps::tetexact, stex);

////////////////////////////////////////////////////////////////////////////////

stex::KProc::KProc(void)
: rExtent(0)
, pFlags(0)
, pSchedIDX(0)
, crData()
{
}

////////////////////////////////////////////////////////////////////////////////

stex::KProc::~KProc(void)
{
}

////////////////////////////////////////////////////////////////////////////////

void stex::KProc::setActive(bool active)
{
    if (active == true) pFlags &= ~INACTIVATED;
    else pFlags |= INACTIVATED;
}

////////////////////////////////////////////////////////////////////////////////

uint stex::KProc::getExtent(void) const
{
	return rExtent;
}

////////////////////////////////////////////////////////////////////////////////

void stex::KProc::resetExtent(void)
{
	rExtent = 0;
}
////////////////////////////////////////////////////////////////////////////////

void stex::KProc::resetCcst(void) const
{
	// This should never get called on base object
	assert (false);
}

////////////////////////////////////////////////////////////////////////////////

double stex::KProc::c(void) const
{
    // Should never get called on base object
	assert (false);
}

////////////////////////////////////////////////////////////////////////////////

double stex::KProc::h(void) const
{
    // Should never get called on base object
	assert (false);
}

////////////////////////////////////////////////////////////////////////////////
/*
steps::solver::Reacdef * swmd::KProc::defr(void) const
{
	// Should only be called on derived object
	assert (false);
}

////////////////////////////////////////////////////////////////////////////////

steps::solver::SReacdef * swmd::KProc::defsr(void) const
{
	// Should olny be called on derived object
	assert (false);
}
*/
////////////////////////////////////////////////////////////////////////////////

// END
