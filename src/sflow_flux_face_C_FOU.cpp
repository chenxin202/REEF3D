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

#include"sflow_flux_face_C_FOU.h"
#include"lexer.h"
#include"slice.h"
#include"fdm2D.h"

sflow_flux_face_C_FOU::sflow_flux_face_C_FOU(lexer *pp, fdm2D *bb)
{
    p=pp;
    b=bb;
}

sflow_flux_face_C_FOU::~sflow_flux_face_C_FOU()
{
}

void sflow_flux_face_C_FOU::u_flux(int ipol, slice& uvel, double &uflux1, double &uflux2)
{    
    if(ipol==1)
	{
    pip=1;
    
	if(0.5*(uvel(i,j)+uvel(i-1,j)) >= 0.0)
    uflux1 = uvel(i-1,j)*b->hx(i-1,j);
    
    if(0.5*(uvel(i,j)+uvel(i-1,j)) < 0.0)
    uflux1 = uvel(i,j)*b->hx(i,j);
    
    
    if(0.5*(uvel(i,j)+uvel(i+1,j)) >= 0.0)
    uflux2 = uvel(i,j)*b->hx(i,j);
    
    if(0.5*(uvel(i,j)+uvel(i+1,j)) < 0.0)
    uflux2 = uvel(i+1,j)*b->hx(i+1,j);
    
	pip=0;
	}

	if(ipol==2)
	{
	pip=1;
	uflux1= 0.25*(uvel(i-1,j) + uvel(i-1,j+1))*(b->hx(i-1,j) + b->hx(i-1,j+1));
	uflux2= 0.25*(uvel(i,j) + uvel(i,j+1))*(b->hx(i,j) + b->hx(i,j+1));
	pip=0;
	}

	if(ipol==4)
	{
	pip=1;
	uflux1= uvel(i-1,j)*b->hx(i-1,j);
	uflux2= uvel(i,j)*b->hx(i,j);
	pip=0;
	}
}

void sflow_flux_face_C_FOU::v_flux(int ipol, slice& vvel, double &vflux1, double &vflux2)
{
	if(ipol==1)
	{
	pip=2;
	vflux1= 0.25*(vvel(i,j-1) + vvel(i+1,j-1))*(b->hy(i,j-1) + b->hy(i+1,j-1));
	vflux2= 0.25*(vvel(i,j) + vvel(i+1,j))*(b->hy(i,j) + b->hy(i+1,j));
	pip=0;
	}

    if(ipol==2)
	{
    pip=2;
    
    if(0.5*(vvel(i,j)+vvel(i,j-1)) >= 0.0)
    vflux1 = vvel(i,j-1)*b->hy(i,j-1);
    
    if(0.5*(vvel(i,j)+vvel(i,j-1)) < 0.0)
    vflux1 = vvel(i,j)*b->hy(i,j);
    
    
    if(0.5*(vvel(i,j)+vvel(i,j+1)) >= 0.0)
    vflux2 = vvel(i,j)*b->hy(i,j);
    
    if(0.5*(vvel(i,j)+vvel(i,j+1)) < 0.0)
    vflux2 = vvel(i,j+1)*b->hy(i,j+1);
    
	pip=0;
	}


	if(ipol==4)
	{
	pip=2;
	vflux1= vvel(i,j-1)*b->hy(i,j-1);
	vflux2= vvel(i,j)*b->hy(i,j);
	pip=0;
	}
}
