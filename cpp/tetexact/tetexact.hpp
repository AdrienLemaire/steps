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

#ifndef STEPS_TETEXACT_TETEXACT_HPP
#define STEPS_TETEXACT_TETEXACT_HPP 1


// STL headers.
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

// STEPS headers.
#include "../common.h"
#include "../solver/api.hpp"
#include "../solver/statedef.hpp"
#include "../geom/tetmesh.hpp"
#include "tri.hpp"
#include "tet.hpp"
#include "kproc.hpp"
#include "comp.hpp"
#include "patch.hpp"
#include "diffboundary.hpp"
#include "crstruct.hpp"

////////////////////////////////////////////////////////////////////////////////

START_NAMESPACE(steps)
START_NAMESPACE(tetexact)

////////////////////////////////////////////////////////////////////////////////

// Forward declarations.

// Auxiliary declarations.
typedef uint                            SchedIDX;
typedef std::set<SchedIDX>              SchedIDXSet;
typedef SchedIDXSet::iterator           SchedIDXSetI;
typedef SchedIDXSet::const_iterator     SchedIDXSetCI;
typedef std::vector<SchedIDX>           SchedIDXVec;
typedef SchedIDXVec::iterator           SchedIDXVecI;
typedef SchedIDXVec::const_iterator     SchedIDXVecCI;

////////////////////////////////////////////////////////////////////////////////

/// Copies the contents of a set of SchedIDX entries into a vector.
/// The contents of the vector are completely overridden.
///
extern void schedIDXSet_To_Vec(SchedIDXSet const & s, SchedIDXVec & v);

////////////////////////////////////////////////////////////////////////////////

class Tetexact: public steps::solver::API
{

public:

    Tetexact(steps::model::Model * m, steps::wm::Geom * g, steps::rng::RNG * r);
    ~Tetexact(void);

    steps::tetmesh::Tetmesh * mesh(void) const
    { return pMesh; }

    ////////////////////////////////////////////////////////////////////////
    // SOLVER INFORMATION
    ////////////////////////////////////////////////////////////////////////

    std::string getSolverName(void) const;
    std::string getSolverDesc(void) const;
    std::string getSolverAuthors(void) const;
    std::string getSolverEmail(void) const;


    ////////////////////////////////////////////////////////////////////////
    // SOLVER CONTROLS
    ////////////////////////////////////////////////////////////////////////

    void reset(void);
    void run(double endtime);
    void advance(double adv);
    void advanceSteps(uint nsteps);
    void step(void);

    void checkpoint(std::string const & file_name);
    void restore(std::string const & file_name);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      GENERAL
    ////////////////////////////////////////////////////////////////////////

    double getTime(void) const;

    inline double getA0(void) const
    { return pA0;}

    uint getNSteps(void) const;

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      ADVANCE
    //      Developer only
    ////////////////////////////////////////////////////////////////////////

    void setTime(double time);
    void setNSteps(uint nsteps);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      COMPARTMENT
    ////////////////////////////////////////////////////////////////////////

 	double _getCompVol(uint cidx) const;

 	double _getCompCount(uint cidx, uint sidx) const;
 	void _setCompCount(uint cidx, uint sidx, double n);

 	double _getCompAmount(uint cidx, uint sidx) const;
	void _setCompAmount(uint cidx, uint sidx, double a);

	double _getCompConc(uint cidx, uint sidx) const;
 	void _setCompConc(uint cidx, uint sidx, double c);

	bool _getCompClamped(uint cidx, uint sidx) const;
	void _setCompClamped(uint cidx, uint sidx, bool b);

	double _getCompReacK(uint cidx, uint ridx) const;
	void _setCompReacK(uint cidx, uint ridx, double kf);

 	bool _getCompReacActive(uint cidx, uint ridx) const;
	void _setCompReacActive(uint cidx, uint ridx, bool a);

    double _getCompReacH(uint cidx, uint ridx) const;
    double _getCompReacC(uint cidx, uint ridx) const;
    double _getCompReacA(uint cidx, uint ridx) const;

    uint _getCompReacExtent(uint cidx, uint ridx) const;
    void _resetCompReacExtent(uint cidx, uint ridx);

	double _getCompDiffD(uint cidx, uint didx) const;
	void _setCompDiffD(uint cidx, uint didx, double dk);

	bool _getCompDiffActive(uint cidx, uint didx) const;
	void _setCompDiffActive(uint cidx, uint didx, bool act);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      PATCH
    ////////////////////////////////////////////////////////////////////////

	double _getPatchArea(uint pidx) const;

 	double _getPatchCount(uint pidx, uint sidx) const;
	void _setPatchCount(uint pidx, uint sidx, double n);

	double _getPatchAmount(uint pidx, uint sidx) const;
 	void _setPatchAmount(uint pidx, uint sidx, double a);

	bool _getPatchClamped(uint pidx, uint sidx) const;
	void _setPatchClamped(uint pidx, uint sidx, bool buf);

	double _getPatchSReacK(uint pidx, uint ridx) const;
  	void _setPatchSReacK(uint pidx, uint ridx, double kf);

 	bool _getPatchSReacActive(uint pidx, uint ridx) const;
 	void _setPatchSReacActive(uint pidx, uint ridx, bool a);

    double _getPatchSReacH(uint pidx, uint ridx) const;
    double _getPatchSReacC(uint pidx, uint ridx) const;
    double _getPatchSReacA(uint pidx, uint ridx) const;

    uint _getPatchSReacExtent(uint pidx, uint ridx) const;
    void _resetPatchSReacExtent(uint pidx, uint ridx);

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      DIFFUSION BOUNDARIES
    ////////////////////////////////////////////////////////////////////////

    void _setDiffBoundaryDiffusionActive(uint dbidx, uint sidx, bool act);
    bool _getDiffBoundaryDiffusionActive(uint dbidx, uint sidx) const;

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      TETRAHEDRAL VOLUME ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    double _getTetVol(uint tidx) const;
    void _setTetVol(uint tidx, double vol);

    double _getTetCount(uint tidx, uint sidx) const;
    void _setTetCount(uint tidx, uint sidx, double n);

    bool _getTetSpecDefined(uint tidx, uint sidx) const;

    double _getTetAmount(uint tidx, uint sidx) const;
    void _setTetAmount(uint tidx, uint sidx, double m);

    double _getTetConc(uint tidx, uint sidx) const;
    void _setTetConc(uint tidx, uint sidx, double c);

    bool _getTetClamped(uint tidx, uint sidx) const;
    void _setTetClamped(uint tidx, uint sidx, bool buf);

    double _getTetReacK(uint tidx, uint ridx) const;
    void _setTetReacK(uint tidx, uint ridx, double kf);

    bool _getTetReacActive(uint tidx, uint ridx) const;
    void _setTetReacActive(uint tidx, uint ridx, bool act);

    double _getTetDiffD(uint tidx, uint didx) const;
    void _setTetDiffD(uint tidx, uint didx, double dk);

    bool _getTetDiffActive(uint tidx, uint didx) const;
    void _setTetDiffActive(uint tidx, uint didx, bool act);

    ////////////////////////////////////////////////////////////////////////

    double _getTetReacH(uint tidx, uint ridx) const;
    double _getTetReacC(uint tidx, uint ridx) const;
    double _getTetReacA(uint tidx, uint ridx) const;

    double _getTetDiffA(uint tidx, uint didx) const;

    ////////////////////////////////////////////////////////////////////////
    // SOLVER STATE ACCESS:
    //      TRIANGULAR SURFACE ELEMENTS
    ////////////////////////////////////////////////////////////////////////

    double _getTriArea(uint tidx) const;
    void _setTriArea(uint tidx, double area);

    bool _getTriSpecDefined(uint tidx, uint sidx) const;

    double _getTriCount(uint tidx, uint sidx) const;
    void _setTriCount(uint tidx, uint sidx, double n);

    double _getTriAmount(uint tidx, uint sidx) const;
    void _setTriAmount(uint tidx, uint sidx, double m);

    bool _getTriClamped(uint tidx, uint sidx) const;
    void _setTriClamped(uint tidx, uint sidx, bool buf);

    double _getTriSReacK(uint tidx, uint ridx) const;
    void _setTriSReacK(uint tidx, uint ridx, double kf);

    bool _getTriSReacActive(uint tidx, uint ridx) const;
    void _setTriSReacActive(uint tidx, uint ridx, bool act);

    ////////////////////////////////////////////////////////////////////////

    double _getTriSReacH(uint tidx, uint ridx) const;
    double _getTriSReacC(uint tidx, uint ridx) const;
    double _getTriSReacA(uint tidx, uint ridx) const;

	////////////////////////////////////////////////////////////////////////

	// Called from local Comp or Patch objects. Ass KProc to this object
	void addKProc(steps::tetexact::KProc * kp);

	inline uint countKProcs(void) const
	{ return pKProcs.size(); }

private:

	////////////////////////////////////////////////////////////////////////

	steps::tetmesh::Tetmesh * 			pMesh;

    ////////////////////////////////////////////////////////////////////////
    // TETEXACT SOLVER METHODS
    ////////////////////////////////////////////////////////////////////////

	uint _addComp(steps::solver::Compdef * cdef);

	uint _addPatch(steps::solver::Patchdef * pdef);

	uint _addDiffBoundary(steps::solver::DiffBoundarydef * dbdef);

	void _addTet(uint tetidx, steps::tetexact::Comp * comp, double vol, double a1,
			     double a2, double a3, double a4, double d1, double d2,
			     double d3, double d4, int tet0, int tet1, int tet2, int tet3);

	void _addTri(uint triidx, steps::tetexact::Patch * patch, double area,
				 int tinner, int touter);

	// called when local tet, tri, reac, sreac objects have been created
	// by constructor
	void _setup(void);

	void _build(void);

	steps::tetexact::KProc * _getNext(void) const;

	void _reset(void);

	void _executeStep(steps::tetexact::KProc * kp, double dt);

    /// Update the kproc's of a tet, after a species has been changed.
    /// This also updates kproc's in surrounding triangles.
    ///
    /// Currently doesn't care about the species.
    ///
    void _updateSpec(steps::tetexact::Tet * tet, uint spec_lidx);

    /// Update the kproc's of a triangle, after a species has been changed.
    /// This does not need to update the kproc's of any neighbouring
    /// tetrahedrons.
    ///
    /// Currently doesn't care about the species.
    ///
    void _updateSpec(steps::tetexact::Tri * tri, uint spec_lidx);

    ////////////////////////////////////////////////////////////////////////
    // LIST OF TETEXACT SOLVER OBJECTS
    ////////////////////////////////////////////////////////////////////////

    std::vector<steps::tetexact::Comp *>       pComps;
    std::map<steps::solver::Compdef *, Comp *> pCompMap;
    inline steps::tetexact::Comp * _comp(uint cidx) const
    { return pComps[cidx]; }

    std::vector<steps::tetexact::Patch *>      pPatches;
    inline steps::tetexact::Patch * _patch(uint pidx) const
    { return pPatches[pidx]; }

    std::vector<steps::tetexact::DiffBoundary *> pDiffBoundaries;
    inline steps::tetexact::DiffBoundary * _diffboundary(uint dbidx) const
    { return pDiffBoundaries[dbidx]; }

    std::vector<steps::tetexact::Tet *>        pTets;
    inline steps::tetexact::Tet * _tet(uint tidx) const
    { return pTets[tidx]; }

    std::vector<steps::tetexact::Tri *>        pTris;
    

    ////////////////////////////////////////////////////////////////////////
    // CR SSA Kernel Data and Methods
    ////////////////////////////////////////////////////////////////////////
    uint                                        nEntries;
    double                                      pSum;
    double                                      nSum;
    double                                      pA0;
    
    std::vector<KProc*>                         pKProcs;
    
    std::vector<CRGroup*>                       nGroups;
    std::vector<CRGroup*>                       pGroups;
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline void _update(std::vector<KProc*> const & upd_entries) {
        #ifdef SSA_DEBUG
        std::cout << "SSA: update selected entries\n";
        #endif
        uint n_upd_entries = upd_entries.size();
        
        for (uint i = 0; i < n_upd_entries; i++) {
            _updateElement(upd_entries[i]);
        }
        
        _updateSum();
        #ifdef SSA_DEBUG
        std::cout << "--------------------------------------------------------\n";
        #endif
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline void _update(void) {
        #ifdef SSA_DEBUG
        std::cout << "SSA: update all entries\n";
        #endif
        for (uint i = 0; i < nEntries; i++) {
            _updateElement(pKProcs[i]);
        }
        
        _updateSum();
        #ifdef SSA_DEBUG
        std::cout << "--------------------------------------------------------\n";
        #endif
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline CRGroup* _getGroup(int pow) {
        #ifdef SSA_DEBUG
        std::cout << "SSA: get group with power " << pow << "\n";
        #endif
        
        if (pow >= 0) {
            #ifdef SSA_DEBUG
            std::cout << "positive group" << pow << "\n";
            #endif
            return pGroups[pow];
        }
        else {
            #ifdef SSA_DEBUG
            std::cout << "negative group" << -pow << "\n";
            #endif
            return nGroups[-pow];
        }
        #ifdef SSA_DEBUG
        std::cout << "--------------------------------------------------------\n";
        #endif
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline void _extendPGroups(uint new_size) {
        uint curr_size = pGroups.size();
        
        #ifdef SSA_DEBUG
        std::cout << "SSA: extending positive group size to " << new_size;
        std::cout << " from " << curr_size << ".\n";
        std::cout << "--------------------------------------------------------\n";
        #endif
        
        while (curr_size < new_size) {
            pGroups.push_back(new CRGroup(curr_size));
            curr_size ++;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline void _extendNGroups(uint new_size) {
        
        uint curr_size = nGroups.size();
        
        #ifdef SSA_DEBUG
        std::cout << "SSA: extending negative group size to " << new_size;
        std::cout << " from " << curr_size << ".\n";
        std::cout << "--------------------------------------------------------\n";
        #endif
        
        while (curr_size < new_size) {
            
            nGroups.push_back(new CRGroup(-curr_size));
            curr_size ++;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    inline void _extendGroup(CRGroup* group, uint size = 1024) {
        #ifdef SSA_DEBUG
        std::cout << "SSA: extending group storage\n";
        std::cout << "current capacity: " << group->capacity << "\n";
        #endif
        
        group->capacity += size;
        group->indices = (KProc**)realloc(group->indices,
                                          sizeof(KProc*) * group->capacity);
        if (group->indices == NULL) {
            std::cerr << "DirectCR: unable to allocate memory for SSA group.\n";
            throw;
        }
        #ifdef SSA_DEBUG
        std::cout << "capacity after extending: " << group->capacity << "\n";
        std::cout << "--------------------------------------------------------\n";
        #endif
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    
    void _updateElement(KProc* kp);
    
    inline void _updateSum(void) {
        #ifdef SSA_DEBUG
        std::cout << "update A0 from " << pA0 << " to ";
        #endif
        
        pA0 = 0.0;
        
        uint n_neg_groups = nGroups.size();
        uint n_pos_groups = pGroups.size();
        
        for (uint i = 0; i < n_neg_groups; i++) {
            pA0 += nGroups[i]->sum;
        }
        
        for (uint i = 0; i < n_pos_groups; i++) {
            pA0 += pGroups[i]->sum;
        }
        
        #ifdef SSA_DEBUG
        std::cout << pA0 << "\n";
        #endif
    }

};

////////////////////////////////////////////////////////////////////////////////

END_NAMESPACE(tetexact)
END_NAMESPACE(steps)

#endif
// STEPS_TETEXACT_TETEXACT_HPP

// END
