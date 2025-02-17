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

#include"sflow_sediment_f.h"
#include"lexer.h"
#include"fdm2D.h" 
#include"ghostcell.h"

#define HP (fabs(b->hp(i,j))>1.0e-20?b->hp(i,j):1.0e20)
 
void sflow_sediment_f::bedshear(lexer *p, fdm2D *b, ghostcell *pgc, slice &P, slice &Q)
{
    double ux,vy,uabs,cf,manning;
    double U,V;
    
    SLICELOOP4
    {
    ux = 0.5*(P(i,j) + P(i-1,j));
    vy = 0.5*(Q(i,j) + Q(i,j-1));
    
    
    if(ux>=0.0)
    U = P(i-1,j);
    
    if(ux<0.0)
    U = P(i,j);
    
    if(vy>=0.0)
    V = Q(i,j);
    
    if(vy<0.0)
    V = Q(i,j-1);
    
    //U = ux;
    //V = vy;
    
    uabs = sqrt(U*U + V*V);
    

    manning = pow(p->S21*ks(i,j),1.0/6.0)/26.0;
    
    cf = pow(manning,2.0)/pow(HP,1.0/3.0);
    
    tau(i,j) = p->W1*9.81*cf*uabs*uabs; 
    
    //if(p->mpirank==2)
    //cout<<"uabs: "<<uabs<<" ks: "<<ks(i,j)<<" manning: "<<manning<<" cf: "<<cf<<" tau: "<<tau(i,j)<<endl;
    
    //b->test(i,j) = tau(i,j);
    }
    
    //pgc->gcsl_start4(p,b->test,1);
    
}
