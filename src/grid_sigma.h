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

#include"fnpf.h"
#include"increment.h"

class lexer;
class fdm;
class ghostcell;
class field;
class fnpf_convection;
class fnpf_ddx;
class grid_sigma_data;
class slice;

using namespace std;

#ifndef GRID_SIGMA_H_
#define GRID_SIGMA_H_

class grid_sigma : public increment
{
public:
	grid_sigma(lexer*);
	virtual ~grid_sigma();
    
    virtual void sigma_coord_ini(lexer*);
    virtual void sigma_ini(lexer*, fdm*, ghostcell*, slice&);
    virtual void sigma_update(lexer*, fdm*, ghostcell*, slice&, slice&, double);
    
    
    double sigmax(lexer*,field&,int);
    double sigmay(lexer*,field&,int);
    double sigmaz(lexer*,field&,int);
    double sigmat(lexer*,field&,int);
    
    void omega_update(lexer*,fdm*,ghostcell*,field&,field&,field&,slice&,slice&,double);

        
private:
    
    void disc_bed(lexer*, fdm*, ghostcell*);
    void disc_eta(lexer*, fdm*, ghostcell*);
    
    fnpf_convection *pdx;
    fnpf_ddx *pddx;
    grid_sigma_data *pd;
    
    
    
    double sig;
};

#endif
