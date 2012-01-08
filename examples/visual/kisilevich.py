# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # ## STEPS - STochastic Engine for Pathway Simulation# Copyright (C) 2007-2011 Okinawa Institute of Science and Technology, Japan.# Copyright (C) 2003-2006 University of Antwerp, Belgium.## See the file AUTHORS for details.## This file is part of STEPS.## STEPS is free software: you can redistribute it and/or modify# it under the terms of the GNU General Public License as published by# the Free Software Foundation, either version 3 of the License, or# (at your option) any later version.## STEPS is distributed in the hope that it will be useful,# but WITHOUT ANY WARRANTY; without even the implied warranty of# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the# GNU General Public License for more details.## You should have received a copy of the GNU General Public License# along with this program. If not, see <http://www.gnu.org/licenses/>.## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #import steps.model as smodimport steps.geom as sgeomimport steps.rng as srngimport steps.solver as ssolvimport mathimport timeimport steps.utilities.meshio as smeshioimport randomimport steps.utilities.visual as svisual########################################################################DT = 0.001			# Sampling time-stepINT = 1		# Sim endtimeDCSTA = 400*1e-12DCSTB = DCSTARCST = 100000.0e6NA0 = 1000			# Initial number of A moleculesNB0 = NA0		# Initial number of B moleculesscale = 1.0e-6OUTPUTFILE = "reac_diff.trace"########################################################################rng = srng.create('mt19937', 16384) rng.initialize(int(time.time()%4294967295)) # The max unsigned longmdl  = smod.Model()A = smod.Spec('A', mdl)B = smod.Spec('B', mdl)C = smod.Spec('C', mdl)volsys = smod.Volsys('vsys',mdl)R1 = smod.Reac('R1', volsys, lhs = [A,B], rhs = [C])R1.setKcst(RCST)D_a = smod.Diff('D_a', volsys, A, dcst = DCSTA)D_b = smod.Diff('D_b', volsys, B, dcst = DCSTB)D_c = smod.Diff('D_c', volsys, C, dcst = DCSTB)mesh, nodeproxy, tetproxy, triproxy = smeshio.importAbaqus('brick_40_4_4_2_comp.inp', scale)ntets = mesh.ntetstet_groups = tetproxy.blocksToGroups()Atets = tet_groups['EB1']nATets = len(Atets)Btets = tet_groups['EB2']nBTets = len(Btets)comp1 = sgeom.TmComp('comp1', mesh, xrange(mesh.ntets))comp1.addVolsys('vsys')print "create sim"sim = ssolv.Tetexact(mdl, mesh, rng)svisual.GUISim(mdl, mesh, mesh) 