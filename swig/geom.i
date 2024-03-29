////////////////////////////////////////////////////////////////////////////////
// STEPS - STochastic Engine for Pathway Simulation
// Copyright (C) 2007-2011 Okinawa Institute of Science and Technology, Japan.
// Copyright (C) 2003-2006 University of Antwerp, Belgium.
//
// See the file AUTHORS for details.
//
// This file is part of STEPS.
//
// STEPS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// STEPS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

/*
 *  Last Changed Rev:  $Rev: 410 $
 *  Last Changed Date: $Date: 2011-04-07 16:11:28 +0900 (Thu, 07 Apr 2011) $
 *  Last Changed By:   $Author: iain $
 */

%module geom_swig

%include "python/std_map.i"
%include "python/std_string.i"
%include "python/std_vector.i"
%include "python/std_set.i"
%include "error.i"
%import "cpp/common.h"

%{
#include "../cpp/geom/geom.hpp"
#include "../cpp//geom/comp.hpp"
#include "../cpp/geom/patch.hpp"
#include "../cpp/geom/tmcomp.hpp"
#include "../cpp/geom/tmpatch.hpp"
#include "../cpp/geom/tet.hpp"
#include "../cpp/geom/tetmesh_rw.hpp"
#include "../cpp/geom/tetmesh.hpp"
#include "../cpp/geom/tri.hpp"
#include "../cpp/error.hpp"
#include "../cpp/geom/diffboundary.hpp"
%}


////////////////////////////////////////////////////////////////////////////////

namespace steps
{
namespace wm
{

class Comp;
class Patch;
class Geom;

}
}

namespace steps
{
namespace model
{	

class Surfsys;
class Volsys;

}
}

namespace steps
{
namespace tetmesh
{
		
class Tetmesh;
class Tri;
class Tet;
class TmPatch;
class TmComp;
class DiffBoundary;

}
}

///////////////////////////////////////////////////////////////////////////////

namespace std
{
	%template(set_str)    set<std::string>;
	%template(set_ptc)    set<steps::wm::Patch *>;
	%template(vector_ptc) vector<steps::wm::Patch *>;
	%template(vector_cmp) vector<steps::wm::Comp *>;
	%template(vector_int) vector<int>;
	%template(vector_uint) vector<unsigned int>;
	%template(vector_dbl) vector<double>;
	%template(vector_bool) vector<bool>;
}

%feature("autodoc", "1");

///////////////////////////////////////////////////////////////////////////////

%exception 
{
	try {
		$action
	} catch (steps::ArgErr & ae) {
		PyErr_SetString(PyExc_NameError, ae.getMsg());
		return NULL;
	} catch (steps::NotImplErr & nie) {
		PyErr_SetString(PyExc_NotImplementedError, nie.getMsg());
		return NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////

namespace steps
{
namespace wm
{

////////////////////////////////////////////////////////////////////////////////
/*
bool isValidID(std::string const & id);
void checkID(std::string const & id);
*/
////////////////////////////////////////////////////////////////////////////////

class Geom
{
	
public:
	
	Geom(void);
	virtual ~Geom(void);
	
    %feature("autodoc", 
"
Returns a reference to the steps.model.Comp compartment object with 
identifier string comp_id (if defined).

Syntax::

    getComp(comp_id)
    
Arguments:
    string comp_id
             
Return:
    steps.model.Comp
");
	steps::wm::Comp * getComp(std::string const & id) const;

    %feature("autodoc", 
"
Removes the steps.geom.Comp object with identifier string comp_id (if defined) 
from the geometry container.

Syntax::

    delComp(comp_id)
    
Arguments:
    string comp_id
             
Return:
    None
");
	void delComp(std::string const & id);
    
    %feature("autodoc", 
"
Returns a list of references to all steps.geom.Comp compartment objects in the 
geometry container.

Syntax::

    getAllComps()
    
Arguments:
    None
             
Return:
    list<steps.geom.Comp>
");
	std::vector<steps::wm::Comp *> getAllComps(void) const;
	
    %feature("autodoc", 
"
Removes the steps.geom.Patch object with identifier string patch_id (if defined) 
from the geometry container.

Syntax::

    getPatch(patch_id)
    
Arguments:
    string patch_id
             
Return:
    steps.geom.Patch
");
	steps::wm::Patch * getPatch(std::string const & id) const;
    
    %feature("autodoc", 
"
Removes the steps.geom.Patch object with identifier string patch_id (if defined) 
from the geometry container.

Syntax::

    delPatch(patch_id)
    
Arguments:
    string patch_id
             
Return:
    None
");
	void delPatch(std::string const & id);
    
    
    %feature("autodoc", 
"
Returns a list of references to all steps.geom.Patch patch objects in the 
geometry container.

Syntax::

    getAllPatches()
    
Arguments:
    None
             
Return:
    list<steps.geom.Patch>
");
	std::vector<steps::wm::Patch *> getAllPatches(void) const;
	
};

////////////////////////////////////////////////////////////////////////////////

%feature("kwargs") Patch::Patch;

class Patch
{
	
public:
	
	Patch(std::string const & id, steps::wm::Geom * container, 
		  steps::wm::Comp * icomp, steps::wm::Comp * ocomp = 0, double area = 0.0);
	virtual ~Patch(void);
	
    %feature("autodoc", 
"
Get the identifier string of the patch.

Syntax::

    getID()
    
Arguments:
    None
             
Return:
    string
");
	std::string getID(void) const;
    
    %feature("autodoc", 
"
Set the identifier string of the patch.

Syntax::

    setID(name)
    
Arguments:
    string name
             
Return:
    None
");
	void setID(std::string const & id);
	
    %feature("autodoc", 
"
Returns a reference to the parent steps.geom.Geom container object.

Syntax::

    getContainer()
    
Arguments:
    None
             
Return:
    steps.geom.Geom
");
	steps::wm::Geom * getContainer(void) const;
    
    %feature("autodoc", 
"
Get the area of the patch (in m^2).

Syntax::

    getArea()

Arguments:
    None
             
Return:
    float
");
	double getArea(void) const;
    
    %feature("autodoc", 
"
Set the area of the patch (in m^2).

Syntax::

    setArea(area)
    
Arguments:
    float area
             
Return:
    None
");
	virtual void setArea(double vol);
	
    %feature("autodoc", 
"
Add surface system identifier string surfsys_id to the patch object.

Syntax::

    addSurfsys(surfsys_id)
    
Arguments:
    string surfsys_id
             
Return:
    None
");
	void addSurfsys(std::string const & id);

    %feature("autodoc", 
"
Returns a list of the surface system identifier strings which have 
been added to the patch.

Syntax::

    getSurfsys()
    
Arguments:
    None
             
Return:
    list<string>
");
	std::set<std::string> getSurfsys(void) const;
    
    %feature("autodoc", 
"
Removes surface system identifier string surfsys_id from this patch.

Syntax::

    delSurfsys(surfsys_id)

Arguments:
    string surfsys_id
             
Return:
    None
");
	void delSurfsys(std::string const & id);
	
    %feature("autodoc", 
"
Returns a reference to the steps.geom.Comp compartment object representing
the inner compartment.

Syntax::

    getIComp()

Arguments:
    None
             
Return:
    steps.geom.Comp
");
	steps::wm::Comp * getIComp(void) const;
    
    %feature("autodoc", 
"
Returns a reference to the steps.geom.Comp compartment object representing
the outer compartment.

Syntax::
    
    getOComp()

Arguments:
    None
             
Return:
    steps.geom.Comp
");
	steps::wm::Comp * getOComp(void) const;
	
};

////////////////////////////////////////////////////////////////////////////////

%feature("kwargs") Comp::Comp;

class Comp
{
	
public:
	
	Comp(std::string const & id, steps::wm::Geom * container,
		 double vol = 0.0);
	virtual ~Comp(void);
	
    %feature("autodoc", 
"
Get the identifier string of the compartment.

Syntax::

    getID()

Arguments:
    None
             
Return:
    string
");
	std::string getID(void) const;
    
    %feature("autodoc", 
"
Set the identifier string of the compartment.

Syntax::

    setID(name)

Arguments:
    string name
             
Return:
    None
");
	void setID(std::string const & id);
	
    %feature("autodoc", 
"
Returns a reference to the parent steps.geom.Geom container object.

Syntax::

    getContainer()

Arguments:
    None
             
Return:
    steps.geom.Geom
");
	steps::wm::Geom * getContainer(void) const;
	
    %feature("autodoc", 
"
Get the volume of the compartment (in m^3).

Syntax::

    getVol()
    
Arguments:
    None
             
Return:
    float
");
	double getVol(void) const;
    
    %feature("autodoc", 
"
Set the volume of the compartment (in m^3).

Syntax::

    setVol(vol)

Arguments:
    float vol
             
Return:
    None
");

	virtual void setVol(double vol);
	
    %feature("autodoc", 
"
Add volume system identifier string volsys_id to the compartment object.

Syntax::

    addVolsys(volsys_id)

Arguments:
    string volsys_id
             
Return:
    None
");
	void addVolsys(std::string const & id);
    
    %feature("autodoc", 
"
Returns a list of the volume system identifier strings which have been 
added to the compartment.

Syntax::

    getVolsys()

Arguments:
    None
             
Return:
    list<string>
");
	std::set<std::string> getVolsys(void) const;
    
    %feature("autodoc", 
"
Removes volume system identifier string volsys_id from this compartment.

Syntax::

    delVolsys(volsys_id)

Arguments:
    string volsys_id
             
Return:
    None
");
	void delVolsys(std::string const & id);
	
    %feature("autodoc", 
"
Returns a list of references to steps.geom.Patch patch objects: 
the 'inner' patches.

Syntax::

    getIPatches()

Arguments:
    None
             
Return:
    list<steps.geom.Patch>
");
	std::set<steps::wm::Patch *> getIPatches(void) const;
    
    %feature("autodoc", 
"
Returns a list of references to steps.geom.Patch patch objects: 
the 'outer' patches.

Syntax::

    getOPatches()

Arguments:
    None
             
Return:
    list<steps.geom.Patch>
");
	std::set<steps::wm::Patch *> getOPatches(void) const;
    
};

////////////////////////////////////////////////////////////////////////////////

} // namespace wm
} // namespace steps

////////////////////////////////////////////////////////////////////////////////

namespace steps
{
namespace tetmesh
{	

////////////////////////////////////////////////////////////////////////////////

class Tetmesh : public steps::wm::Geom
{

public:
	
	Tetmesh(unsigned int nverts, unsigned int ntets, unsigned int ntris);
	Tetmesh(std::vector<double> const & verts,
			std::vector<unsigned int> const & tets,
			std::vector<unsigned int> const & tris = std::vector<unsigned int>());
	Tetmesh(std::vector<double> const & verts,
			std::vector<unsigned int> const & tris,
			std::vector<double> const & tri_areas,
			std::vector<double> const & tri_norms,
			std::vector<int> const & tri_tet_neighbs,
			std::vector<unsigned int> const & tets,
			std::vector<double> const & tet_vols,
			std::vector<double> const & tet_barycs,
			std::vector<unsigned int> const & tet_tri_neighbs,
			std::vector<int> const & tet_tet_neighbs);
	
	virtual ~Tetmesh(void);
	
    %feature("autodoc", 
"
Set a vertex with index vidx to coordinates x, y, z. Should be used nverts 
number of times to supply all vertex information if the second constructor 
is used. Cannot be called after setup() has been called.

Syntax::

    setVertex(vidx, x, y, z)

Arguments:
    * uint vidx
    * float x
    * float y
    * float z
             
Return:
    None
");
	void setVertex(unsigned int vidx, double x, double y, double z);
    
    %feature("autodoc", 
"
Set the triangle with index tidx formed by vertices vidx0, vidx1, vidx2. 
Should be called ntris number of times to supply triangle information if the 
second constructor is used. Cannot be called after setup() has been called.

Syntax::

    setTri(tidx, vidx0, vidx1, vidx2)
                
Arguments:
    * uint tidx
    * uint vidx0
    * uint vidx1
    * uint vidx2
             
Return:
    None
");
	void setTri(unsigned int tidx, unsigned int vidx0,
				unsigned int vidx1, unsigned int vidx2);
                
    %feature("autodoc", 
"
Set the tetrahedron with index tidx formed by vertices vidx0, vidx1, 
vidx2, vidx3. Should be called ntets number of times to supply tetrahedron 
information if the second constructor is used. Cannot be called after setup() 
has been called. 

Syntax::

    setTet(tidx, vidx0, vidx1, vidx2, vidx3)

Arguments:
    * uint tidx
    * uint vidx0
    * uint vidx1
    * uint vidx2
    * uint vidx3
             
Return:
    None
");
	void setTet(unsigned int tidx, unsigned int vidx0, unsigned int vidx1,
				unsigned int vidx2, unsigned int vidx3);
	
    %feature("autodoc", 
"
Setup the Tetmesh object by computing the auxiliary data. This method should 
be called when the second constructor is used and all vertex, tetrahedron and 
triangle information has been supplied with the set methods. The first constructor 
calls this method internally, so setup does not have to be called when using the 
first constructor.

Syntax::

    setup()

Arguments:
    None
             
Return:
    None
");
	void setup(void);
    
    %feature("autodoc", 
"
Check if setup() has been called, either internally by the first constructor, 
or by the user if the second constructor was used.

Syntax::

    isSetupDone()

Arguments:
    None
             
Return:
    True if setup is done.
    False if setup is not done.
");
	bool isSetupDone(void) const;
	
    %feature("autodoc", 
"
Returns the coordinates of vertex with index vidx in the container.

Syntax::

    getVertex(vidx)

Arguments:
    uint vidx
             
Return:
    list<float, length = 3>
");
	std::vector<double> getVertex(unsigned int vidx) const;
    
    %feature("autodoc", 
"
Returns the total number of vertices in the mesh.

Syntax::

    countVertices()

Arguments:
    None
             
Return:
    uint
");
	unsigned int countVertices(void) const;
	
	//steps::tetmesh::TmPatch * getTmPatch(std::string const & id) const;
    
    %feature("autodoc", 
"
Returns the triangle with index tidx in the container by its three vertex indices.

Syntax::

    getTri(tidx)

Arguments:
    uint tidx
             
Return:
    list<uint, length = 3>
");    
	std::vector<unsigned int> getTri(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the total number of triangles in the mesh.

Syntax::

    countTris()

Arguments:
    None
             
Return:
    uint
");  
	unsigned int countTris(void) const;
    
    %feature("autodoc", 
"
Returns the area of the triangle with index tidx.

Syntax::

    getTriArea(tidx)

Arguments:
    uint tidx
             
Return:
    float
");  
	double getTriArea(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the Cartesian coordinates of the barycenter of triangle with index tidx.

Syntax::

    getTriBarycenter(tidx)

Arguments:
    uint tidx
             
Return:
    list<float, length = 3>
");  
	std::vector<double> getTriBarycenter(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the radius-edge-ratio (a quality measurement) of tetrahedron with index tidx.

Syntax::
    
    getTetQualityRER(tidx)

Arguments:
    uint tidx
             
Return:
    float
");
	double getTetQualityRER(unsigned int tidx) const;

    %feature("autodoc", 
"
Returns the normal vector of the triangle with index tidx.

Syntax::

    getTriNorm(tidx)

Arguments:
    uint tidx
             
Return:
    list<float, length = 3>
");
	std::vector<double> getTriNorm(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns a reference to a step.geom.TmPatch object: the patch which triangle 
with index tidx belongs to. Returns None if triangle not assigned to a patch.

Syntax::

    getTriPatch(tidx)

Arguments:
    uint tidx
             
Return:
    steps.geom.TmPatch
");
	steps::tetmesh::TmPatch * getTriPatch(unsigned int tidx) const;

    %feature("autodoc", 
"
Returns a reference to a step.geom.Diffboundary object: the diffusion boundary triangle 
with index tidx belongs to. Returns None if triangle not assigned to a diffusion boundary.
             
Syntax::
             
    getTriDiffBoundary(tidx)
             
Arguments:
    uint tidx
             
Return:
    steps.geom.DiffBoundary
");
	steps::tetmesh::DiffBoundary * getTriDiffBoundary(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the indices of the two neighbouring tetrahedrons of triangle with 
index tidx. An index of -1 indicates no neighbour (triangle is on the mesh border). 

Syntax::

    getTriTetNeighb(tidx)

Arguments:
    uint tidx
             
Return:
    list<int, length = 2>
");
	std::vector<int> getTriTetNeighb(unsigned int tidx) const;
    
    // added by Weiliang
    %feature("autodoc", 
"
Returns a list of triangles that form the mesh boundary.
Support function for steps.utilities.visual.

Syntax::

    getTriBoundary()

Arguments:
    None
             
Return:
    list<int>
    
Status: Obsolete. rename as getTriSurfs()
");
	std::vector<int> getTriBoundary(void) const;
    
    // added by Weiliang
    %feature("autodoc", 
"
Returns a list of triangles that form the mesh boundary.
Support function for steps.utilities.visual.

Syntax::

    getSurfTris()

Arguments:
    None
             
Return:
    list<unsigned int>
");
	std::vector<unsigned int> getSurfTris(void) const;
    
	//steps::tetmesh::TmComp * getTmComp(std::string const & id) const;
    
    %feature("autodoc", 
"
Returns the tetrahedron with index tidx in the container by its four vertex indices.

Syntax::
    getTet(tidx)
    
Arguments:
    uint tidx
             
Return:
    list<uint, length = 4>
");
	std::vector<unsigned int> getTet(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the total number of tetrahedrons in the mesh.

Syntax::

    countTets()

Arguments:
    None
             
Return:
    uint
");
	unsigned int countTets(void) const;
    
    
    %feature("autodoc", 
"
Returns the volume of the tetrahedron with index tidx.

Syntax::

    getTetVol(tidx)

Arguments:
    uint tidx
             
Return:
    float
");
	double getTetVol(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the barycenter of the tetrahedron with index tidx.

Syntax::

    getTetBarycenter(tidx)

Arguments:
    uint tidx
             
Return:
    list<float, length = 3>
");
	std::vector<double> getTetBarycenter(unsigned int tidx) const;

    %feature("autodoc", 
"
Returns a reference to a steps.geom.Comp object: the compartment which 
tetrahedron with index tidx belongs to. Returns None if tetrahedron not 
assigned to a compartment.

Syntax::

    getTetComp(tidx)

Arguments:
    uint tidx
             
Return:
    steps.geom.TmComp
");
	steps::tetmesh::TmComp * getTetComp(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the indices of the four neighbouring triangles of tetrahedron with index tidx.

Syntax::

    getTetTriNeighb(tidx)

Arguments:
    uint tidx
             
Return:
    list<uint, length = 4>
");
	std::vector<unsigned int> getTetTriNeighb(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the indices of the four neighbouring tetrahedrons of tetrahedron with index tidx. 
An index of -1 indicates no neighbour (tetrahedron is on the mesh border).

Syntax::

    getTetTetNeighb(tidx)

Arguments:
    uint tidx
             
Return:
    list<int, length = 4>
");
	std::vector<int> getTetTetNeighb(unsigned int tidx) const;
    
    %feature("autodoc", 
"
Returns the index of the tetrahedron which encompasses a given point 
p (given in Cartesian coordinates x,y,z). Returns -1 if p is a position 
outside the mesh.

Syntax::

    findTetByPoint(p)

Arguments:
    list<float, length = 3> p
             
Return:
    int
");
	int findTetByPoint(std::vector<double> p) const;
	
    %feature("autodoc", 
"
Returns the minimal Cartesian coordinate of the rectangular bounding box of the mesh. 

Syntax::

    getBoundMin()

Arguments:
    None
             
Return:
    list<float, length = 3>
");
	std::vector<double> getBoundMin(void) const;
    
    %feature("autodoc", 
"
Returns the maximal Cartesian coordinate of the rectangular bounding box of the mesh. 

Syntax::

    getBoundMax()

Arguments:
    None
             
Return:
    list<float, length = 3>
");
	std::vector<double> getBoundMax(void) const;
    
    %feature("autodoc", 
"
Returns the total volume of the mesh. 

Syntax::

    getMeshVolume()

Arguments:
    None
             
Return:
    float
");
	double getMeshVolume(void) const;
	
};

////////////////////////////////////////////////////////////////////////////////
/*
Tetmesh * loadASCII(std::string pathname);

void saveASCII(std::string pathname, Tetmesh * m);
*/
////////////////////////////////////////////////////////////////////////////////

/* /////////////////////////////////////////////////////////////////////////////
//////// OBJECT REMOVED BECAUSE OF MEMORY ISSUES. SEE TODO NOTE IN C++ /////////
///////////////////////// CONSTRUCTOR FOR DETAILS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Tri	
{
	
public:
	
	Tri(Tetmesh * mesh, unsigned int tidx);
	~Tri(void);
	
	unsigned int getIdx(void);
	double getArea(void) const;
	std::vector<double> getBarycenter(void) const;
	std::vector<double> getNorm(void) const;
	steps::tetmesh::TmPatch * getPatch(void) const;
	
	Tet getTet0(void) const;
    Tet getTet1(void) const;
    
    
    Tet getInnerTet(void) const;
    Tet getOuterTet(void) const;
    
    
    unsigned int getTet0Idx(void) const;
    unsigned int getTet1Idx(void) const;

    
    unsigned int getInnerTetIdx(void) const;
    unsigned int getOuterTetIdx(void) const;
    

    unsigned int getVertex0Idx(void) const;
    unsigned int getVertex1Idx(void) const;
    unsigned int getVertex2Idx(void) const;
	
};
	
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//////// OBJECT REMOVED BECAUSE OF MEMORY ISSUES. SEE TODO NOTE IN C++ /////////
///////////////////////// CONSTRUCTOR FOR DETAILS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Tet
{
	
public:
	
	Tet(Tetmesh * mesh, unsigned int tidx);
	~Tet(void);
	
	unsigned int getIdx(void) const;
	double getVol(void) const;
	std::vector<double> getBarycenter(void) const;
	double getQualityRER(void) const;
	steps::tetmesh::TmComp * getComp(void) const;
	
	Tet getTet0(void) const;
    Tet getTet1(void) const;
    Tet getTet2(void) const;
    Tet getTet3(void) const;

	unsigned int getTet0Idx(void) const;
    unsigned int getTet1Idx(void) const;
    unsigned int getTet2Idx(void) const;
    unsigned int getTet3Idx(void) const;
	
    double getTet0Dist(void) const;
    double getTet1Dist(void) const;
    double getTet2Dist(void) const;
    double getTet3Dist(void) const;
	
    Tri getTri0(void) const;
    Tri getTri1(void) const;
    Tri getTri2(void) const;
    Tri getTri3(void) const;
	
    unsigned int getTri0Idx(void) const;
    unsigned int getTri1Idx(void) const;
    unsigned int getTri2Idx(void) const;
    unsigned int getTri3Idx(void) const;

    double getTri0Dist(void) const;
    double getTri1Dist(void) const;
    double getTri2Dist(void) const;
    double getTri3Dist(void) const;
	
    double getTri0Area(void) const;
    double getTri1Area(void) const;
    double getTri2Area(void) const;
    double getTri3Area(void) const;

    unsigned int getVertex0Idx(void) const;
    unsigned int getVertex1Idx(void) const;
    unsigned int getVertex2Idx(void) const;
    unsigned int getVertex3Idx(void) const;
	
	unsigned int getVertex0(void) const;
    unsigned int getVertex1(void) const;
    unsigned int getVertex2(void) const;
    unsigned int getVertex3(void) const;
	
    bool isInside(std::vector<double> p) const;
	std::vector<double> getRanPnt(steps::rng::RNG * r, unsigned int n = 1) const;

};

*/
////////////////////////////////////////////////////////////////////////////////
	
%feature("kwargs") TmComp::TmComp;

class TmComp : public steps::wm::Comp
{
public:
		
	TmComp(std::string const & id, Tetmesh * container, 
		 std::vector<unsigned int> const & tets);
	virtual ~TmComp(void);
	
    %feature("autodoc", "Obsolete");
	virtual void setVol(double vol);
	
    %feature("autodoc", 
"
Returns a list of indices of all tetrahedrons assigned to the compartment. 

Syntax::

    getAllTetIndices()

Arguments:
    None
             
Return:
    list<uint>
");

	std::vector<unsigned int> getAllTetIndices(void) const;
    
    %feature("autodoc", 
"
Returns the number of tetrahedrons assigned to the compartment. 

Syntax::

    countTets()

Arguments:
    None
             
Return:
    uint
");
    unsigned int countTets(void) const;

    %feature("autodoc", 
"
Returns a list of Booleans describing if tetrahedrons tets are 
assigned to the compartment.

Syntax::
    
    isTetInside(tets)

Arguments:
    list<uint> tets
             
Return:
    list<bool, length = length(tets)>
");
	std::vector<bool> isTetInside(std::vector<unsigned int> tet) const;
	
    %feature("autodoc", 
"
Returns the minimal Cartesian coordinate of the rectangular bounding box 
of the compartment. 

Syntax::

    getBoundMin()

Arguments:
    None
             
Return:
    list<float, length = 3>
");
	std::vector<double> getBoundMin(void) const;
    
    %feature("autodoc", 
"
Returns the maximal Cartesian coordinate of the rectangular bounding box 
of the compartment. 

Syntax::

    getBoundMax()

Arguments:
    None
             
Return:
    list<float, length = 3>
");
	std::vector<double> getBoundMax(void) const;
	
};

////////////////////////////////////////////////////////////////////////////////

%feature("kwargs") TmPatch::TmPatch;

class TmPatch : public steps::wm::Patch
{
public:
	/*
	TmPatch(std::string const & id, Tetmesh * container,
		  std::vector<unsigned int> const & tris, steps::tetmesh::TmComp * icomp,
		  steps::tetmesh::TmComp * ocomp = 0, steps::model::Surfsys * surfsys = 0);
	*/
	TmPatch(std::string const & id, Tetmesh * container,
			std::vector<unsigned int> const & tris, steps::wm::Comp* icomp,
			steps::wm::Comp* ocomp = 0);
	virtual ~TmPatch(void);
	
    %feature("autodoc", "Obsolete");
	virtual void setArea(double area);
	
    %feature("autodoc", 
"
Returns a list of indices of all triangles assigned to the patch.

Syntax::

    getAllTriIndices()

Arguments:
    None
             
Return:
    list<uint>
");
	std::vector<unsigned int> getAllTriIndices(void) const;
    
    %feature("autodoc", 
"
Returns a list of Booleans describing if triangles tris are 
assigned to the patch.

Syntax::

    isTriInside(tris)

Arguments:
    list<uint> tris
             
Return:
    list<bool, length = length(tris)>
");
	std::vector<bool> isTriInside(std::vector<unsigned int> tet) const;
	
};

////////////////////////////////////////////////////////////////////////////////

%feature("kwargs") DiffBoundary::DiffBoundary;

class DiffBoundary
{
        
public:
        
    DiffBoundary(std::string const & id, Tetmesh * container,
                     std::vector<unsigned int> const & tris);
    
    virtual ~DiffBoundary(void);

    %feature("autodoc", 
"
Get the identifier string of the diffusion boundary.

Syntax::

    getID()

Arguments:
    None
             
Return:
    string
");
	std::string getID(void) const;
    
    %feature("autodoc", 
"
Set the identifier string of the diffusion boundary.

Syntax::

    setID(name)

Arguments:
    string name

Return:
    None
");
	void setID(std::string const & id);

    %feature("autodoc", 
"
Returns a reference to the parent steps.geom.Tetmesh container object.

Syntax::

    getContainer()

Arguments:
    None

Return:
    steps.geom.Tetmesh
");
	steps::tetmesh::Tetmesh * getContainer(void) const;

    %feature("autodoc", 
"
Returns a list of the two compartments this diffusion boundary connects.
             
Syntax::
             
    getComps()
             
Arguments:
    None
             
Return:
    list<steps::wm::Comp, length = 2>
");
    std::vector<steps::wm::Comp *>  getComps(void) const;

    %feature("autodoc", 
"
Returns a list of Booleans describing if triangles tris are 
assigned to the Diffusion Boundary.
             
Syntax::
             
    isTriInside(tris)
             
Arguments:
    list<uint> tris
             
Return:
    list<bool, length = length(tris)>
");
    std::vector<bool> isTriInside(std::vector<unsigned int> tri) const;
        
    %feature("autodoc", 
"
Returns a list of indices of all triangles assigned to the patch.
             
Syntax::
             
    getAllTriIndices()
             
Arguments:
    None
             
Return:
    list<uint>
");
    std::vector<unsigned int> getAllTriIndices(void) const;                
};	

////////////////////////////////////////////////////////////////////////////////

}	// end namespace tetmesh
}	// end namespace steps

// END
	
