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
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"
#include"ddweno.h"
#include"reinidisc_f2.h"
#include"cpt.h"

reinidisc_f2::reinidisc_f2(lexer *p) :  ddweno(p)
{
}

reinidisc_f2::~reinidisc_f2()
{
}

void reinidisc_f2::start(lexer *p, fdm *a, ghostcell *pgc, vec &b, vec &L, int ipol)
{	
	if(ipol==4)
	disc(p,a,pgc,b,L,p->sizeM4,a->C4);
	
	if(ipol==5)
	disc(p,a,pgc,b,L,p->sizeM4a,a->C4a);
    
    if(ipol==6)
	disc(p,a,pgc,b,L,p->sizeM6,a->C6);
}


void reinidisc_f2::disc(lexer *p, fdm *a, ghostcell *pgc, vec &b, vec &L, int *sizeM, cpt &C)
{
	
	NLOOP
	L.V[n] = 0.0;
	
    NLOOP
	if((b.V[I_J_K]>=0.0 && b.V[Ip1_J_K]>=0.0 && b.V[Im1_J_K]>=0.0 && b.V[I_Jp1_K]>=0.0 && b.V[I_Jm1_K]>=0.0 && b.V[I_J_Kp1]>=0.0 && b.V[I_J_Km1]>=0.0) 
	|| (b.V[I_J_K]<0.0  && b.V[Ip1_J_K]<0.0  && b.V[Im1_J_K]<0.0  && b.V[I_Jp1_K]<0.0  && b.V[I_Jm1_K]<0.0   && b.V[I_J_Kp1]<0.0  && b.V[I_J_Km1]<0.0) 
	)
	{
	dx=0.0;
	dy=0.0;
	dz=0.0;
	lsv=b.V[I_J_K];
    lsSig=lsv/sqrt(lsv*lsv);

    if(fabs(lsv)<1.0e-8)
    lsSig=1.0;

// x
	xmin=(lsv-b.V[Im1_J_K])/p->DXP[IM1];
	xplus=(b.V[Ip1_J_K]-lsv)/p->DXP[IP];
	
	if(xmin*lsSig>0.0 && xplus*lsSig>-xmin*lsSig)
	dx=ddwenox(a,b,1.0,C);

	if(xplus*lsSig<0.0 && xmin*lsSig<-xplus*lsSig)
	dx=ddwenox(a,b,-1.0,C);

	if(xplus*lsSig>0.0 && xmin*lsSig<0.0)
	dx=0.0;

// y
    if(p->j_dir==1)
    {
	ymin=(lsv-b.V[I_Jm1_K])/p->DYP[JM1];
	yplus=(b.V[I_Jp1_K]-lsv)/p->DYP[JP];
	
	if(ymin*lsSig>0.0 && yplus*lsSig>-ymin*lsSig)
	dy=ddwenoy(a,b,1.0,C);

	if(yplus*lsSig<0.0 && ymin*lsSig<-yplus*lsSig)
	dy=ddwenoy(a,b,-1.0,C);

	if(yplus*lsSig>0.0 && ymin*lsSig<0.0)
	dy=0.0;
    }

// z
	zmin=(lsv-b.V[I_J_Km1])/p->DZP[KM1];
	zplus=(b.V[I_J_Kp1]-lsv)/p->DZP[KP];
	
	if(zmin*lsSig>0.0 && zplus*lsSig>-zmin*lsSig)
	dz=ddwenoz(a,b,1.0,C);

	if(zplus*lsSig<0.0 && zmin*lsSig<-zplus*lsSig)
	dz=ddwenoz(a,b,-1.0,C);

	if(zplus*lsSig>0.0 && zmin*lsSig<0.0)
	dz=0.0;						

	dnorm=sqrt(dx*dx + dy*dy + dz*dz);
	
    deltax = (1.0/3.0)*(p->DXN[IP] + p->DYN[JP] + p->DZN[KP]);
	
	sign=lsv/sqrt(lsv*lsv+ dnorm*dnorm*deltax*deltax);
    
    if(sign!=sign)
    sign= 1.0;

	L.V[n] = -(sign*dnorm - sign);
    }
}
