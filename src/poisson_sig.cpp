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

#include"poisson_sig.h"
#include"lexer.h"
#include"fdm.h"
#include"heat.h"
#include"concentration.h"
#include"density_f.h"
#include"density_comp.h"
#include"density_conc.h"
#include"density_heat.h"
#include"density_vof.h"

poisson_sig::poisson_sig(lexer * p, heat *&pheat, concentration *&pconc)  
{
    if((p->F80==0||p->A10==5) && p->H10==0 && p->W30==0)
	pd = new density_f(p);
	
	if(p->F80==0 && p->H10==0 && p->W30==1)
	pd = new density_comp(p);
	
	if(p->F80==0 && p->H10>0)
	pd = new density_heat(p,pheat);
	
	if(p->F80==0 && p->C10>0)
	pd = new density_conc(p,pconc);
    
    if(p->F80>0 && p->H10==0 && p->W30==0)
	pd = new density_vof(p);
    
    teta=0.5;
}

poisson_sig::~poisson_sig()
{
}

void poisson_sig::start(lexer* p, fdm *a, field &f)
{	
    double sigxyz2;
   
	n=0;
    LOOP
	{
        if(a->wet(i,j)==1)
        {
            sigxyz2 = pow(0.5*(p->sigx[FIJK]+p->sigx[FIJKp1]),2.0) + pow(0.5*(p->sigy[FIJK]+p->sigy[FIJKp1]),2.0) + pow(p->sigz[IJ],2.0);
            
            
            a->M.p[n]  =  (CPOR1*PORVAL1)/(pd->roface(p,a,1,0,0)*p->DXP[IP]*p->DXN[IP])
                        + (CPOR1m*PORVAL1m)/(pd->roface(p,a,-1,0,0)*p->DXP[IM1]*p->DXN[IP])
                        
                        + (CPOR2*PORVAL2)/(pd->roface(p,a,0,1,0)*p->DYP[JP]*p->DYN[JP])*p->y_dir
                        + (CPOR2m*PORVAL2m)/(pd->roface(p,a,0,-1,0)*p->DYP[JM1]*p->DYN[JP])*p->y_dir
                        
                        + (sigxyz2*CPOR3*PORVAL3)/(pd->roface(p,a,0,0,1)*p->DZP[KP]*p->DZN[KP])
                        + (sigxyz2*CPOR3m*PORVAL3m)/(pd->roface(p,a,0,0,-1)*p->DZP[KM1]*p->DZN[KP]);


            a->M.n[n] = -(CPOR1*PORVAL1)/(pd->roface(p,a,1,0,0)*p->DXP[IP]*p->DXN[IP]);
            a->M.s[n] = -(CPOR1m*PORVAL1m)/(pd->roface(p,a,-1,0,0)*p->DXP[IM1]*p->DXN[IP]);

            a->M.w[n] = -(CPOR2*PORVAL2)/(pd->roface(p,a,0,1,0)*p->DYP[JP]*p->DYN[JP])*p->y_dir;
            a->M.e[n] = -(CPOR2m*PORVAL2m)/(pd->roface(p,a,0,-1,0)*p->DYP[JM1]*p->DYN[JP])*p->y_dir;

            a->M.t[n] = -(sigxyz2*CPOR3*PORVAL3)/(pd->roface(p,a,0,0,1)*p->DZP[KP]*p->DZN[KP])     
                        + CPOR4*PORVAL4*0.5*(p->sigxx[FIJK]+p->sigxx[FIJKp1])/(a->ro(i,j,k)*(p->DZN[KP]+p->DZN[KM1]));
                        
            a->M.b[n] = -(sigxyz2*CPOR3m*PORVAL3m)/(pd->roface(p,a,0,0,-1)*p->DZP[KM1]*p->DZN[KP]) 
                        - CPOR4*PORVAL4*0.5*(p->sigxx[FIJK]+p->sigxx[FIJKm1])/(a->ro(i,j,k)*(p->DZN[KP]+p->DZN[KM1]));
            
            
            a->rhsvec.V[n] +=  CPOR4*PORVAL4*(p->sigx[FIJK]+p->sigx[FIJKp1])*(f(i+1,j,k+1) - f(i-1,j,k+1) - f(i+1,j,k-1) + f(i-1,j,k-1))
                        /(a->ro(i,j,k)*(p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))
                        
                        + CPOR4*PORVAL4*(p->sigy[FIJK]+p->sigy[FIJKp1])*(f(i,j+1,k+1) - f(i,j-1,k+1) - f(i,j+1,k-1) + f(i,j-1,k-1))
                        /((a->ro(i,j,k)*p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
        }
	
	++n;
	}
    
    
    n=0;
	LOOP
	{
        if(a->wet(i,j)==1)
        {
            if(p->flag4[Im1JK]<0)
            {
            a->rhsvec.V[n] -= a->M.s[n]*f(i-1,j,k);
            a->M.s[n] = 0.0;
            }
            
            if(p->flag4[Ip1JK]<0)
            {
            a->rhsvec.V[n] -= a->M.n[n]*f(i+1,j,k);
            a->M.n[n] = 0.0;
            }
            
            if(p->flag4[IJm1K]<0)
            {
            a->rhsvec.V[n] -= a->M.e[n]*f(i,j-1,k)*p->y_dir;
            a->M.e[n] = 0.0;
            }
            
            if(p->flag4[IJp1K]<0)
            {
            a->rhsvec.V[n] -= a->M.w[n]*f(i,j+1,k)*p->y_dir;
            a->M.w[n] = 0.0;
            }
            
            // BEDBC
            if(p->flag4[IJKm1]<0)
            {
            /*a->rhsvec.V[n] += a->M.b[n]*p->DZP[KM1]*a->WL(i,j)*a->ro(i,j,k)*a->dwdt(i,j);
            a->M.p[n] += a->M.b[n];
            a->M.b[n] = 0.0;*/
            
            a->rhsvec.V[n] -= a->M.b[n]*f(i,j,k-1);
            a->M.b[n] = 0.0;
            }
            
            // FSFBC
            if(p->flag4[IJKp1]<0)
            {
                if(p->D37==1)
                {
                a->rhsvec.V[n] -= a->M.t[n]*f(i,j,k+1);
                a->M.t[n] = 0.0;
                }
                
                if(p->D37==3)
                {
                sigxyz2 = pow(0.5*(p->sigx[FIJK]+p->sigx[FIJKp1]),2.0) + pow(0.5*(p->sigy[FIJK]+p->sigy[FIJKp1]),2.0) + pow(p->sigz[IJ],2.0);
                
                a->M.p[n] -= (sigxyz2*CPOR3*PORVAL3)/(pd->roface(p,a,0,0,1)*p->DZP[KP]*p->DZN[KP]);
                a->M.p[n] += (sigxyz2*CPOR3*PORVAL3)/(pd->roface(p,a,0,0,1)*teta*p->DZP[KP]*p->DZN[KP]);
                           
                a->M.t[n] = 0.0;
                
                
                a->rhsvec.V[n] -=  CPOR4*PORVAL4*(p->sigx[FIJK]+p->sigx[FIJKp1])*(f(i+1,j,k+1) - f(i-1,j,k+1) - f(i+1,j,k-1) + f(i-1,j,k-1))
                        /(a->ro(i,j,k)*(p->DXN[IP]+p->DXN[IM1])*(p->DZN[KP]+p->DZN[KM1]))
                        
                        + CPOR4*PORVAL4*(p->sigy[FIJK]+p->sigy[FIJKp1])*(f(i,j+1,k+1) - f(i,j-1,k+1) - f(i,j+1,k-1) + f(i,j-1,k-1))
                        /((a->ro(i,j,k)*p->DYN[JP]+p->DYN[JM1])*(p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                        
                a->rhsvec.V[n] +=  CPOR4*PORVAL4*(p->sigx[FIJK]+p->sigx[FIJKp1])*( - f(i+1,j,k-1) + f(i-1,j,k-1))
                        /(a->ro(i,j,k)*(p->DXN[IP]+p->DXN[IM1])*(teta*p->DZN[KP]+p->DZN[KM1]))
                        
                        + CPOR4*PORVAL4*(p->sigy[FIJK]+p->sigy[FIJKp1])*( - f(i,j+1,k-1) + f(i,j-1,k-1))
                        /((a->ro(i,j,k)*p->DYN[JP]+p->DYN[JM1])*(teta*p->DZN[KP]+p->DZN[KM1]))*p->y_dir;
                }
            }
        }
	++n;
	}
}
