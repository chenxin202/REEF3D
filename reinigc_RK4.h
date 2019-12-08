/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

This file is part of REEF3D.

REEF3D is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"reini.h"
#include"gradient.h"
#include"field4.h"

class picard;

using namespace std;

#ifndef REINIGC_RK4_H_
#define REINIGC_RK4_H_

class reinigc_RK4 : public reini, gradient
{
public:
	reinigc_RK4(lexer* p, fdm *a);
	virtual ~reinigc_RK4();
	virtual void start(fdm*,lexer*,field&,ghostcell*,ioflow*);
    virtual void startV(fdm*,lexer*,vec&,ghostcell*,ioflow*);

	double dstx, dsty, dstz, dnorm, sign;
	double sx,sy,sz,snorm,op;

	field4 brk3,d0;

private:
    picard *ppicard;

	void step(fdm*, lexer*);
	double dt;
	void disc(lexer *p, fdm*, ghostcell*, field&);
	double Cd(lexer *p, fdm*, ghostcell*, field&);
    void constraint(lexer *p, fdm*, ghostcell*, field&);
    
	double xmin,xplus,ymin,yplus,zmin,zplus;
	double dxmin,dxplus,dymin,dyplus,dzmin,dzplus;
	double uwx,uwy,uwz,ddt;
	double lsv,lsSig;
	double starttime,endtime;

	int gcval_phi,gcval_ro,gcval_iniphi,reiniter;
	const double deltax,epsi;
	double H,H0,grad,dT,dirac;
	double lambda1,lambda2,dV,dval,Cs;
	
	double dx,dy,dz;
};

#endif
