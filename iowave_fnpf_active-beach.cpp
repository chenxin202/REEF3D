/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2020 Hans Bihs

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

#include"iowave.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"

void iowave::active_beach_fnpf(lexer *p, ghostcell* pgc, double *Fi, double *Uin, slice &Fifsf, slice &eta)
{
    
        double eta_R,Uc,Un,Vc,Wc,eta_T,eta_M,wsf;
		double posx,posy,posz,uvel,vvel,uabs,fx,fy,pval,fp;
        double fxdir,fydir;
		double x=0.0;
		double z=0.0;
		double fac1,fac,multiplier;
		int aa,bb,ii,jj;

		// U / FI
		for(n=0;n<p->gcslout_count;++n)
		{
		i=p->gcslout[n][0];
		j=p->gcslout[n][1];
		
		ii=0;
		if(p->gcslout[n][3]==4)
		ii=1;
		
		eta_T = 0.0;
		eta_M = eta(i,j); 
		eta_R = eta_M-eta_T;
        
        //cout<<p->mpirank<<" eta_R: "<<eta_R<<" eta_M: "<<eta_M<<"   eta: "<<eta(i,j)<<endl;


        aa=bb=0;
		if(p->gcslout[n][3]==1)
		aa=-1;
		
		if(p->gcslout[n][3]==4)
		aa=1;
		
		if(p->gcslout[n][3]==3)
		bb=-1;
		
		if(p->gcslout[n][3]==2)
		bb=1;

        fx=1.0;
        Uc=eta_R*sqrt(9.81/p->wd);
            
			//if(wsf>-1.0e20)
			FKLOOP 
			{
				z=p->ZSN[FIJK]-p->phimean;
				
				if(p->B99==3)
				Uc=eta_R*sqrt(9.81/p->wd);
				
				if(p->B99==4)
				Uc=eta_R*p->ww*( cosh(p->wk*(p->wd+z))/sinh(p->wk*p->wd));
                
               if(p->B99==5)
               {
                   if(p->pos_z()>p->B123)
                   {
                   fac = (p->pos_z()-p->B123)/(wsf-p->B123);
                   multiplier = 2.0*((wsf)/(wsf-p->B123));
                   Uc =   multiplier*fac*eta_R*sqrt(9.81/p->wd);
                   }
                   
                   if(p->pos_z()<=p->B123)
                   Uc=0.0;
               }
                   

				Uin[FIp1JK]=Uc*fx;
			}
          
        
         
        Fifsf(i+1,j) = Fifsf(i,j) + Uc*fx*1.0*p->DXP[IP1];
        Fifsf(i+2,j) = Fifsf(i,j) + Uc*fx*2.0*p->DXP[IP1];
        Fifsf(i+3,j) = Fifsf(i,j) + Uc*fx*3.0*p->DXP[IP1];
        
        //cout<<p->mpirank<<" AWA Uc: "<<Uc<<" fx: "<<fx<<" p->DXP[IM1]: "<<p->DXP[IM1]<<" FiFsF_i "<<Fifsf(i,j)<<" FiFsF_i+1 "<<Fifsf(i+1,j)<<endl;

		}
        


      
}
