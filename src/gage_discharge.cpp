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

#include"gage_discharge.h"
#include"lexer.h"
#include"fdm.h"
#include"ghostcell.h"
#include<sys/stat.h>
#include<sys/types.h>

gage_discharge::gage_discharge(lexer *p, fdm* a, ghostcell *pgc)
{
	p->Iarray(iloc,p->P67);
	p->Iarray(flag,p->P67);
	p->Darray(q,p->P67);
	
	// Create Folder
	if(p->mpirank==0 && p->P14==1)
	mkdir("./REEF3D_Log",0777);
	
    if(p->mpirank==0 && p->P67>0)
    {
    // open file
	if(p->P14==0)
    qout.open("REEF3D-discharge.dat");
	
	if(p->P14==1)
	qout.open("./REEF3D_Log/REEF3D-discharge.dat");

    qout<<"number of gages:  "<<p->P67<<endl<<endl;
    qout<<"x_coord  "<<endl;
    for(n=0;n<p->P67;++n)
    qout<<n+1<<"\t "<<p->P67_x[n]<<endl;

    qout<<endl<<endl;

    qout<<"iter";
    for(n=0;n<p->P67;++n)
    qout<<"\t P"<<n+1;

    qout<<endl<<endl;
    }
	
	ini_location(p,a,pgc);
}

gage_discharge::~gage_discharge()
{
    qout.close();
}

void gage_discharge::start(lexer *p, fdm *a, ghostcell *pgc)
{

    for(n=0;n<p->P67;++n)
    q[n]=0.0;

	
    for(n=0;n<p->P67;++n)
    {
	area=0.0;
	Ai=0.0;

    i=iloc[n];
		
		if(flag[n]==1)
		JLOOP
        KLOOP
        PCHECK
        {
			area=0.0;
            if(a->phi(i,j,k)>-0.5*p->DXM-1.0e-20 && a->topo(i,j,k)>0.0)
			{
            if(a->phi(i,j,k)>=0.5*p->DXM)
            area=p->DXM*p->DXM;

            if(a->phi(i,j,k)<0.5*p->DXM && a->phi(i,j,k)>0.0*p->DXM)
            area=p->DXM*(p->DXM*0.5 + a->phi(i,j,k));
			
			if(a->phi(i,j,k)>=-0.5*p->DXM -1.0e-20 && a->phi(i,j,k)<=0.0*p->DXM)
            area=p->DXM*(p->DXM*0.5 - a->phi(i,j,k));

            q[n]+=area*0.5*(a->u(i,j,k) + a->u(i-1,j,k));
			}
        }
	
    }
	
	
    for(n=0;n<p->P67;++n)
    q[n]=pgc->globalsum(q[n]);
	
	if(p->mpirank==0 && p->P66==1)
	for(n=0;n<p->P67;++n)
	cout<<n+1<<setprecision(6)<<" Qi: "<<q[n]<<endl;  

    // write to file
    if(p->mpirank==0)
    {
    qout<<setprecision(9)<<p->count<<"\t";
    for(n=0;n<p->P67;++n)
    qout<<setprecision(6)<<q[n]<<"  \t  ";
    qout<<endl;
    }
}

void gage_discharge::ini_location(lexer *p, fdm *a, ghostcell *pgc)
{
    int check;
	
	for(n=0;n<p->P67;++n)
	flag[n]=0;

    for(n=0;n<p->P67;++n)
    {
    iloc[n]=conv((p->P67_x[n]-p->originx)/p->DXM);
	
	if(iloc[n]>=0 && iloc[n]<p->knox)
	flag[n]=1;
	}
}


int gage_discharge::conv(double a)
{

int b,c;
double d,diff;

c= int( a);
d=double(c);
diff=a-d;

b=c;

if(diff>0.5)
b=c+1;

if(diff<=-0.5)
b=c-1;

return b;

}
