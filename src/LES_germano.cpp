/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2022 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
--------------------------------------------------------------------*/

#include"LES_germano.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"
#include"strain.h"

LES_germano::LES_germano(lexer* p, fdm* a) : LES(p,a)
{
	gcval_sgs=24;
	c_sgs=0.2;
}

LES_germano::~LES_germano()
{
}

void LES_germano::start(fdm* a, lexer* p, convection* pconvec, diffusion* pdiff,solver* psolv, ghostcell* pgc, ioflow* pflow, vrans* pvrans)
{
    LOOP
    a->eddyv(i,j,k) = pow(p->DXM*c_sgs,2.0) * sqrt(2.0) * strainterm(p,a);

    pgc->start4(p,a->eddyv,gcval_sgs);
}

void LES_germano::ktimesave(lexer* p, fdm* a, ghostcell *pgc)
{
}

void LES_germano::etimesave(lexer* p, fdm* a, ghostcell *pgc)
{
}



