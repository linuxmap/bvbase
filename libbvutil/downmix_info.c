/*
 * Copyright (c) 2014 Tim Walker <tdskywalker@gmail.com>
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

#include "downmix_info.h"
#include "frame.h"

BVDownmixInfo *bv_downmix_info_update_side_data(BVFrame *frame)
{
    BVFrameSideData *side_data;

    side_data = bv_frame_get_side_data(frame, BV_FRAME_DATA_DOWNMIX_INFO);

    if (!side_data)
        side_data = bv_frame_new_side_data(frame, BV_FRAME_DATA_DOWNMIX_INFO,
                                           sizeof(BVDownmixInfo));

    if (!side_data)
        return NULL;

    return (BVDownmixInfo*)side_data->data;
}
