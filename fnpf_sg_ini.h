/*--------------------------------------------------------------------
REEF3D
Copyright 2008-2019 Hans Bihs

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

#include"fnpf_sg.h"
#include"increment.h"
#include"fnpf_sg_fsf_update.h"
#include"fnpf_sg_bed_update.h"

class lexer;
class fdm_fnpf;
class ghostcell;
class print;
class ioflow;
class reini;
class onephase;

using namespace std;

#ifndef FNPF_SG_INI_H_
#define FNPF_SG_INI_H_

class fnpf_sg_ini : public fnpf_sg, public increment, public fnpf_sg_fsf_update, public fnpf_sg_bed_update
{
public:
	fnpf_sg_ini(lexer*, fdm_fnpf*, ghostcell*);
	virtual ~fnpf_sg_ini();
    
    virtual void ini(lexer*, fdm_fnpf*, ghostcell*, ioflow*, reini*, onephase*);
    
    void velcalc(lexer*, fdm_fnpf*, ghostcell *pgc, field&);
    
private:

    void lsm_ini(lexer*, fdm_fnpf*, ghostcell*, ioflow*);

    int gcval,gcval_u,gcval_v,gcval_w;

};

#endif
