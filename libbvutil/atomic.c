/*
 * Copyright (c) 2012 Ronald S. Bultje <rsbultje@gmail.com>
 *
 * This file is part of BVBase.
 *
 * BVBase is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * BVBase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BVBase; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "config.h"
#include "atomic.h"

#if !BV_HAVE_ATOMICS_NATIVE

#if BV_HAVE_PTHREADS

#include "thread.h"

static BVMutex atomic_lock = PTHREAD_MUTEX_INITIALIZER;

int bvpriv_atomic_int_get(volatile int *ptr)
{
    int res;

    bv_mutex_lock(&atomic_lock);
    res = *ptr;
    bv_mutex_unlock(&atomic_lock);

    return res;
}

void bvpriv_atomic_int_set(volatile int *ptr, int val)
{
    bv_mutex_lock(&atomic_lock);
    *ptr = val;
    bv_mutex_unlock(&atomic_lock);
}

int bvpriv_atomic_int_add_and_fetch(volatile int *ptr, int inc)
{
    int res;

    bv_mutex_lock(&atomic_lock);
    *ptr += inc;
    res = *ptr;
    bv_mutex_unlock(&atomic_lock);

    return res;
}

void *bvpriv_atomic_ptr_cas(void * volatile *ptr, void *oldval, void *newval)
{
    void *ret;
    bv_mutex_lock(&atomic_lock);
    ret = *ptr;
    if (ret == oldval)
        *ptr = newval;
    bv_mutex_unlock(&atomic_lock);
    return ret;
}

#elif !BV_HAVE_THREADS

int bvpriv_atomic_int_get(volatile int *ptr)
{
    return *ptr;
}

void bvpriv_atomic_int_set(volatile int *ptr, int val)
{
    *ptr = val;
}

int bvpriv_atomic_int_add_and_fetch(volatile int *ptr, int inc)
{
    *ptr += inc;
    return *ptr;
}

void *bvpriv_atomic_ptr_cas(void * volatile *ptr, void *oldval, void *newval)
{
    if (*ptr == oldval) {
        *ptr = newval;
        return oldval;
    }
    return *ptr;
}

#else /* BV_HAVE_THREADS */

/* This should never trigger, unless a new threading implementation
 * without correct atomics dependencies in configure or a corresponding
 * atomics implementation is added. */
#error "Threading is enabled, but there is no implementation of atomic operations available"

#endif /* BV_HAVE_PTHREADS */

#endif /* !BV_HAVE_ATOMICS_NATIVE */

#ifdef TEST
#include "bvassert.h"

int main(void)
{
    volatile int val = 1;
    int res;

    res = bvpriv_atomic_int_add_and_fetch(&val, 1);
    bv_assert0(res == 2);
    bvpriv_atomic_int_set(&val, 3);
    res = bvpriv_atomic_int_get(&val);
    bv_assert0(res == 3);

    return 0;
}
#endif
