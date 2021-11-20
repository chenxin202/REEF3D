/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2021 Hans Bihs

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

#include"pressure.h"
#include"increment.h"

class density;
class solver;
class heat;
class concentration;

using namespace std;

#ifndef PJM_SIGSS_H_
#define PJM_SIGSS_H_

class pjm_sigss : public pressure, public increment
{

public:

	pjm_sigss(lexer*, fdm*, ghostcell*, heat*&, concentration*&);
	virtual ~pjm_sigss();

	virtual void start(fdm*,lexer* p, poisson*, solver*, ghostcell*,ioflow*, field&, field&, field&,double);
	virtual void rhscalc(lexer*,fdm*,ghostcell*,field&,field&,field&,double);
	virtual void vel_setup(lexer*,fdm*,ghostcell*,field&,field&,field&,double);
	virtual void ucorr(lexer*p,fdm*,field&,double);
	virtual void vcorr(lexer*p,fdm*,field&,double);
	virtual void wcorr(lexer*p,fdm*,field&,double);
	virtual void upgrad(lexer*,fdm*);
	virtual void vpgrad(lexer*,fdm*);
    virtual void wpgrad(lexer*,fdm*);
	virtual void ptimesave(lexer*,fdm*,ghostcell*);
    
    void poisson2D(lexer *,fdm*,field&);
    void poisson3D(lexer *,fdm*,field&);

private:
	double starttime,endtime;
	int count, gcval_press;
	int gcval_u, gcval_v, gcval_w;
	
	void debug(lexer*,fdm*);
    
    density *pd;
    solver *psolv;
    int vecsize;
    
    double *M,*x,*rhs;

};



#endif

