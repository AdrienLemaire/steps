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

// STEPS headers.
#include "../common.h"
#include "../math/constants.hpp"
#include "../solver/diffdef.hpp"
#include "../solver/compdef.hpp"
#include "diff.hpp"
#include "tet.hpp"
#include "kproc.hpp"
#include "tetexact.hpp"

#include "iostream"

////////////////////////////////////////////////////////////////////////////////

NAMESPACE_ALIAS(steps::tetexact, stex);
NAMESPACE_ALIAS(steps::solver, ssolver);
NAMESPACE_ALIAS(steps::math, smath);

////////////////////////////////////////////////////////////////////////////////

stex::Diff::Diff(ssolver::Diffdef * ddef, stex::Tet * tet)
: KProc()
, pDiffdef(ddef)
, pTet(tet)
, pUpdVec()
, pScaledDcst(0.0)
, pDcst(0.0)
, pCDFSelector()
, pNeighbCompLidx()
{
	assert(pDiffdef != 0);
	assert(pTet != 0);
	stex::Tet * next[4] =
	{
		pTet->nextTet(0),
		pTet->nextTet(1),
		pTet->nextTet(2),
		pTet->nextTet(3)
	};

    ligGIdx = pDiffdef->lig();
    ssolver::Compdef * cdef = pTet->compdef();
    lidxTet = cdef->specG2L(ligGIdx);
    
    for (uint i = 0; i < 4; ++i) { pDiffBndDirection[i] = pTet->getDiffBndDirection(i);}

    for (uint i = 0; i < 4; ++i)
    {
        if (next[i] == 0)
        {
        	pNeighbCompLidx[i] = -1;
        	continue;
        }
        else
        {
        	pNeighbCompLidx[i] = next[i]->compdef()->specG2L(ligGIdx);
        }
    }

    // Precalculate part of the scaled diffusion constant.
	uint ldidx = pTet->compdef()->diffG2L(pDiffdef->gidx());
	double dcst = pTet->compdef()->dcst(ldidx);
    pDcst = dcst;

    double d[4] = { 0.0, 0.0, 0.0, 0.0 };
    for (uint i = 0; i < 4; ++i)
    {
        // Compute the scaled diffusion constant.
    	// Need to here check if the direction is a diffusion boundary
        double dist = pTet->dist(i);
        if ((dist > 0.0) && (next[i] != 0))
        {
        	if (pDiffBndDirection[i] == true)
        	{
        		if (pDiffBndActive[i]) d[i] = (pTet->area(i) * dcst) / (pTet->vol() * dist);
        		else d[i] = 0.0;
        	}
        	else d[i] = (pTet->area(i) * dcst) / (pTet->vol() * dist);
        }
    }
    // Compute scaled "diffusion constant".
    for (uint i = 0; i < 4; ++i)
    {
		pScaledDcst += d[i];
	}

    // Should not be negative!
    assert(pScaledDcst >= 0);

    // Setup the selector distribution.
    if (pScaledDcst == 0.0)
    {
        pCDFSelector[0] = 0.0;
        pCDFSelector[1] = 0.0;
        pCDFSelector[2] = 0.0;
    }
    else
    {
        pCDFSelector[0] = d[0] / pScaledDcst;
        pCDFSelector[1] = pCDFSelector[0] + (d[1] / pScaledDcst);
        pCDFSelector[2] = pCDFSelector[1] + (d[2] / pScaledDcst);
    }
}

////////////////////////////////////////////////////////////////////////////////

stex::Diff::~Diff(void)
{
}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::checkpoint(std::fstream & cp_file)
{
    cp_file.write((char*)&pScaledDcst, sizeof(double));
    cp_file.write((char*)&pDcst, sizeof(double));
    cp_file.write((char*)pCDFSelector, sizeof(double) * 3);
    cp_file.write((char*)pDiffBndActive, sizeof(bool) * 4);
    cp_file.write((char*)pDiffBndDirection, sizeof(bool) * 4);
    cp_file.write((char*)pNeighbCompLidx, sizeof(int) * 4);
}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::restore(std::fstream & cp_file)
{
    cp_file.read((char*)&pScaledDcst, sizeof(double));
    cp_file.read((char*)&pDcst, sizeof(double));
    cp_file.read((char*)pCDFSelector, sizeof(double) * 3);
    cp_file.read((char*)pDiffBndActive, sizeof(bool) * 4);
    cp_file.read((char*)pDiffBndDirection, sizeof(bool) * 4);
    cp_file.read((char*)pNeighbCompLidx, sizeof(int) * 4);
}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::setupDeps(void)
{
    // We will check all KProcs of the following simulation elements:
    //   * the 'source' tetrahedron
    //   * any neighbouring triangles
    //
    // But also in the possible 'destination' tetrahedrons (leading to
    // four different dependency lists, each containing a copy of the
    // dependencies in the 'source' tet):
    //   * any neighbouring tetrahedrons
    //   * any neighbouring triangles of these neighbouring tets
    //
    // Since there can be no diffusion between tetrahedrons blocked by
    // a triangle, there is no need to filter out duplicate dependent
    // kprocs.

    // Search for dependencies in the 'source' tetrahedron.
    std::set<stex::KProc*> local;
    //std::set<stex::KProc*> local_objs;
    
    KProcPVecCI kprocend = pTet->kprocEnd();
    for (KProcPVecCI k = pTet->kprocBegin(); k != kprocend; ++k)
    {
        // Check locally.
        if ((*k)->depSpecTet(ligGIdx, pTet) == true) {
            //local.insert((*k)->getSSARef());
            local.insert(*k);
        }
    }
    // Check the neighbouring triangles.
    for (uint i = 0; i < 4; ++i)
    {
        stex::Tri * next = pTet->nextTri(i);
        if (next == 0) continue;
        kprocend = next->kprocEnd();
        for (KProcPVecCI k = next->kprocBegin(); k != kprocend; ++k)
        {
            if ((*k)->depSpecTet(ligGIdx, pTet) == true) {
                //local.insert((*k)->getSSARef());
                local.insert(*k);
            }
        }
    }

    // Search for dependencies in neighbouring tetrahedra.
    for (uint i = 0; i < 4; ++i)
    {
        // Fetch next tetrahedron, if it exists.
        stex::Tet * next = pTet->nextTet(i);
        if (next == 0) continue;
        if (pTet->nextTri(i) != 0) continue;

        // Copy local dependencies.
        std::set<stex::KProc*> local2(local.begin(), local.end());
        //std::set<stex::KProc*> local2_objs(local_objs.begin(), local_objs.end());
        
        // Find the ones 'locally' in the next tet.
        kprocend = next->kprocEnd();
        for (KProcPVecCI k = next->kprocBegin(); k != kprocend; ++k)
        {
            if ((*k)->depSpecTet(ligGIdx, next) == true) {
                //local.insert((*k)->getSSARef());
                local2.insert(*k);
            }
        }

        // Find deps in neighbouring triangles in the next tet.
        // As said before, this cannot logically include the shared
        // triangle.
        for (uint j = 0; j < 4; ++j)
        {
            // Fetch next triangle, if it exists.
            stex::Tri * next2 = next->nextTri(j);
            if (next2 == 0) continue;

            // Find deps.
            kprocend = next2->kprocEnd();
            for (KProcPVecCI k = next2->kprocBegin(); k != kprocend; ++k)
            {
                if ((*k)->depSpecTet(ligGIdx, next) == true) {
                    //local.insert((*k)->getSSARef());
                    local2.insert(*k);
                }
            }
        }

        // Copy the set to the update vector.
        pUpdVec[i].assign(local2.begin(), local2.end());
        //pUpdObjVec[i].assign(local2_objs.begin(), local2_objs.end());
        
    }
}

////////////////////////////////////////////////////////////////////////////////

bool stex::Diff::depSpecTet(uint gidx, stex::Tet * tet)
{
    if (pTet != tet) return false;
    if (gidx != ligGIdx) return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool stex::Diff::depSpecTri(uint gidx, stex::Tri * tri)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::reset(void)
{
    crData.recorded = false;
    crData.pow = 0;
    crData.pos = 0;
    crData.rate = 0.0;
    resetExtent();

    // NOTE: These must become the dcst calculation for obvious reasons
    pDiffBndActive[0] = false;
    pDiffBndActive[1] = false;
    pDiffBndActive[2] = false;
    pDiffBndActive[3] = false;

    uint ldidx = pTet->compdef()->diffG2L(pDiffdef->gidx());
	double dcst = pTet->compdef()->dcst(ldidx);
    setDcst(dcst);

    setActive(true);

}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::setDiffBndActive(uint i, bool active)
{
	assert (i < 4);
	assert(pDiffBndDirection[i] == true);

	// Only need to update if the flags are changing
	if (pDiffBndActive[i] != active)
	{
		pDiffBndActive[i] = active;
		setDcst(pDcst);
	}

}

////////////////////////////////////////////////////////////////////////////////

bool stex::Diff::getDiffBndActive(uint i) const
{
	assert (i < 4);
	assert(pDiffBndDirection[i] == true);

	return pDiffBndActive[i];
}

////////////////////////////////////////////////////////////////////////////////

void stex::Diff::setDcst(double dcst)
{
	assert(dcst >= 0.0);
	pDcst = dcst;

	stex::Tet * next[4] =
	{
		pTet->nextTet(0),
		pTet->nextTet(1),
		pTet->nextTet(2),
		pTet->nextTet(3)
	};

    double d[4] = { 0.0, 0.0, 0.0, 0.0 };

    for (uint i = 0; i < 4; ++i)
    {
        // Compute the scaled diffusion constant.
    	// Need to here check if the direction is a diffusion boundary
        double dist = pTet->dist(i);
        if ((dist > 0.0) && (next[i] != 0))
        {
        	if (pDiffBndDirection[i] == true)
        	{
        		if (pDiffBndActive[i]) d[i] = (pTet->area(i) * dcst) / (pTet->vol() * dist);
        		else d[i] = 0.0;
        	}
        	else d[i] = (pTet->area(i) * dcst) / (pTet->vol() * dist);
        }
    }

    // Compute scaled "diffusion constant".
    pScaledDcst = 0.0;

    for (uint i = 0; i < 4; ++i)
    {
    	pScaledDcst += d[i];
	}
    // Should not be negative!
    assert(pScaledDcst >= 0);

    // Setup the selector distribution.
    if (pScaledDcst == 0.0)
    {
        pCDFSelector[0] = 0.0;
        pCDFSelector[1] = 0.0;
        pCDFSelector[2] = 0.0;
    }
    else
    {
        pCDFSelector[0] = d[0] / pScaledDcst;
        pCDFSelector[1] = pCDFSelector[0] + (d[1] / pScaledDcst);
        pCDFSelector[2] = pCDFSelector[1] + (d[2] / pScaledDcst);
    }
}

////////////////////////////////////////////////////////////////////////////////

double stex::Diff::rate(void) const
{
    if (inactive()) return 0.0;


    // Compute the rate.
    double rate = (pScaledDcst) * static_cast<double>(pTet->pools()[lidxTet]);
    assert(std::isnan(rate) == false);
    // Return.
    return rate;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<stex::KProc*> const & stex::Diff::apply(steps::rng::RNG * rng)
{
    //uint lidxTet = this->lidxTet;
    // Pre-fetch some general info.


    // Apply local change.
    uint * local = pTet->pools() + lidxTet;
	bool clamped = pTet->clamped(lidxTet);

    if (clamped == false)
    {
        assert(*local > 0);
    }

    // Apply change in next voxel: select a direction.
    double sel = rng->getUnfEE();
    
    if (sel < pCDFSelector[0])
    {
            
        // Direction 1.
        stex::Tet * nexttet = pTet->nextTet(0);
        // If there is no next tet 0, pCDFSelector[0] should be zero
        // So we can assert that nextet 0 does indeed exist
        assert (nexttet != 0);
        assert(pNeighbCompLidx[0] > -1);

        if (nexttet->clamped(pNeighbCompLidx[0]) == false)
        {
            nexttet->incCount(pNeighbCompLidx[0],1);
        }
        if (clamped == false) {pTet->incCount(lidxTet, -1); }

        rExtent++;

        return pUpdVec[0];
    }
    else if (sel < pCDFSelector[1])
    {
            
        // Direction 2.
        stex::Tet * nexttet = pTet->nextTet(1);
        // If there is no next tet 1, pCDFSelector[1] should be zero
        // So we can assert that nextet 1 does indeed exist
        assert (nexttet != 0);
        assert(pNeighbCompLidx[1] > -1);
      
        if (nexttet->clamped(pNeighbCompLidx[1]) == false)
        {
            nexttet->incCount(pNeighbCompLidx[1],1);
        }
            
        if (clamped == false) {pTet->incCount(lidxTet, -1); }

        rExtent++;

        return pUpdVec[1];
    }
    else if (sel < pCDFSelector[2])
    {
            
        // Direction 3.
        stex::Tet * nexttet = pTet->nextTet(2);
        // If there is no next tet 2, pCDFSelector[2] should be zero
        // So we can assert that nextet 2 does indeed exist
        assert (nexttet != 0);
        assert(pNeighbCompLidx[2] > -1);
        if (nexttet->clamped(pNeighbCompLidx[2]) == false)
        {
            nexttet->incCount(pNeighbCompLidx[2],1);
        }
    
        if (clamped == false) {pTet->incCount(lidxTet, -1); }

        rExtent++;
        return pUpdVec[2];
    }
    else
    {
            
        // Direction 4.
        stex::Tet * nexttet = pTet->nextTet(3);
        // If there is no next tet 3, pCDFSelector[3] should be zero
        // So we can assert that nextet 3 does indeed exist
        assert (nexttet != 0);
        assert(pNeighbCompLidx[3] > -1);

        if (nexttet->clamped(pNeighbCompLidx[3]) == false)
        {
            nexttet->incCount(pNeighbCompLidx[3],1);
        }
        if (clamped == false) {pTet->incCount(lidxTet, -1); }

        rExtent++;
    
        return pUpdVec[3];
        

    }

    // This should never happen!
    assert(0);
    std::cerr << "Cannot find a suitable direction for diffusion!\n";
    throw;
    return pUpdVec[0];
}

////////////////////////////////////////////////////////////////////////////////

uint stex::Diff::updVecSize(void) const
{
	uint maxsize = pUpdVec[0].size();
	for (uint i=1; i <= 3; ++i)
	{
		if (pUpdVec[i].size() > maxsize) maxsize = pUpdVec[i].size();
	}
	return maxsize;
}

////////////////////////////////////////////////////////////////////////////////

// END
