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
Author: Hans Bihs
--------------------------------------------------------------------*/

#include"levelset_AB2.h"
#include"levelset_AB3.h"
#include"levelset_RK2.h"
#include"levelset_RK3.h"
#include"levelset_RK3_V.h"
#include"levelset_RK4.h"
#include"levelset_void.h"

#include"reini_RK3.h"
#include"reini_RK3_V.h"
#include"reini_RK4.h"
#include"reini_AB2.h"
#include"reini_AB3.h"
#include"reinigc_RK3.h"
#include"reinigc_RK4.h"
#include"reinivc_RK3.h"
#include"reinifluid_AB2.h"
#include"reinifluid_AB3.h"
#include"reinifluid_RK3.h"
#include"reinifluid_RK4.h"
#include"directreini.h"
#include"reini_void.h"

#include"particle.h"
#include"particle_void.h"

#include"VOF_AB.h"
#include"VOF_RK3.h"
#include"VOF_PLIC.h"

#include"onephase_v.h"
#include"onephase_f.h"
