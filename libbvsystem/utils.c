/*************************************************************************
    > File Name: utils.c
    > Author: albertfang
    > Mail: fang.qi@besovideo.com 
    > Created Time: 2014年12月14日 星期日 20时51分18秒
 ************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) albert@BesoVideo, 2014
 */

#line 25 "utils.c"

#include <libbvutil/bvstring.h>
#include <libbvutil/atomic.h>

#include "bvsystem.h"

static BVSystem *first_sys = NULL;
static BVSystem **last_sys = &first_sys;

int bv_system_register(BVSystem * sys)
{
    BVSystem **p = last_sys;
    sys->next = NULL;
    while (*p || bvpriv_atomic_ptr_cas((void *volatile *) p, NULL, sys))
        p = &(*p)->next;
    last_sys = &sys->next;
    return 0;
}

BVSystem *bv_system_next(BVSystem * sys)
{
    if (sys)
        return sys->next;
    else
        return first_sys;
}

BVSystem *bv_system_find_system(enum BVSystemType type)
{
    BVSystem *sys = NULL;
    if (first_sys == NULL) {
        bv_log(NULL, BV_LOG_ERROR, "BVSystem Not RegisterAll");
        return NULL;
    }

    while ((sys = bv_system_next(sys))) {
        if (sys->type == type) {
            return sys;
        }
    }
    return NULL;
}

