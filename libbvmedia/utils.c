/*************************************************************************
    > File Name: utils.c
    > Author: albertfang
    > Mail: fang.qi@besovideo.com 
    > Created Time: 2014年12月30日 星期二 17时10分17秒
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

#include <libbvutil/atomic.h>
#include <libbvutil/bvassert.h>

#include "bvmedia.h"
#include "driver.h"

static BVInputMedia *first_ifmt = NULL;
static BVInputMedia **last_ifmt = &first_ifmt;

static BVOutputMedia *first_ofmt = NULL;
static BVOutputMedia **last_ofmt = &first_ofmt;

static BVMediaDriver *first_driver = NULL;
static BVMediaDriver **last_driver = &first_driver;

void bv_input_media_register(BVInputMedia *ifmt)
{
    BVInputMedia **p = last_ifmt;
    ifmt->next = NULL;
    while (*p || bvpriv_atomic_ptr_cas((void *volatile *) p, NULL, ifmt))
        p = &(*p)->next;
    last_ifmt = &ifmt->next;
}

void bv_output_media_register(BVOutputMedia *media)
{
    BVOutputMedia **p = last_ofmt;

    media->next = NULL;
    while(*p || bvpriv_atomic_ptr_cas((void * volatile *)p, NULL, media))
        p = &(*p)->next;
    last_ofmt = &media->next;
}

void bv_media_driver_register(BVMediaDriver *driver)
{
    BVMediaDriver **p = last_driver;
    driver->next = NULL;
    while(*p || bvpriv_atomic_ptr_cas((void * volatile *)p, NULL, driver))
        p = &(*p)->next;
    last_driver = &driver->next;
}

BVInputMedia * bv_input_media_next(BVInputMedia *ifmt)
{
    if (ifmt)
        return ifmt->next;
    else
        return first_ifmt;
}

BVOutputMedia *bv_output_media_next(const BVOutputMedia *f)
{
    if (f)
        return f->next;
    else
        return first_ofmt;
}

BVMediaDriver *bv_media_driver_next(const BVMediaDriver *driver)
{
    if (driver)
        return driver->next;
    else
       return first_driver; 
}

BVStream * bv_stream_new(BVMediaContext *s, const BVCodec *c)
{
    BVStream *st;
    BVStream **streams;

    if (s->nb_streams >= INT_MAX/sizeof(*streams))
        return NULL;
    streams = bv_realloc_array(s->streams, s->nb_streams + 1, sizeof(*streams));
    if (!streams)
        return NULL;
    s->streams = streams;

    st = bv_mallocz(sizeof(BVStream));
    if (!st)
        return NULL;

    st->codec = bv_codec_context_alloc(c);
    if (!st->codec) {
        bv_free(st);
        return NULL;
    }
#if 0
    if (!(st->info = av_mallocz(sizeof(*st->info)))) {
        av_free(st);
        return NULL;
    }
    st->info->last_dts = AV_NOPTS_VALUE;
    if (s->iformat) {
        /* no default bitrate if decoding */
        st->codec->bit_rate = 0;

        /* default pts setting is MPEG-like */
        avpriv_set_pts_info(st, 33, 1, 90000);
    }
#endif

    st->index      = s->nb_streams;
    s->streams[s->nb_streams++] = st;
    return st;
}

void bv_stream_free(BVMediaContext *s, BVStream *st)
{
    bv_assert0(s->nb_streams>0);
    bv_assert0(s->streams[ s->nb_streams - 1 ] == st);

    bv_codec_context_free(st->codec);
    bv_freep(&st->priv_data);
    bv_freep(&s->streams[ --s->nb_streams ]);
}

int bv_media_context_control(BVMediaContext *s, enum BVMediaMessageType type, const BVControlPacket *pkt_in, BVControlPacket *pkt_out)
{
    if (s->imedia && s->imedia->media_control) {
        return s->imedia->media_control(s, type, pkt_in, pkt_out); 
    }

    if (s->omedia && s->omedia->media_control) {
        return s->omedia->media_control(s, type, pkt_in, pkt_out);
    }
    return BVERROR(ENOSYS);
}
