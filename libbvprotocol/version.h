/*************************************************************************
    > File Name: version.h
    > Author: albertfang
    > Mail: fang.qi@besovideo.com 
    > Created Time: 2014年12月15日 星期一 10时46分54秒
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
 * You should hbve received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) albert@BesoVideo, 2014
 */

#ifndef BV_PROTOCOL_VERSION_H
#define BV_PROTOCOL_VERSION_H

#include <libbvutil/version.h>

#define LIBBVPROTOCOL_VERSION_MAJOR 0
#define LIBBVPROTOCOL_VERSION_MINOR  0
#define LIBBVPROTOCOL_VERSION_MICRO 3

#define LIBPROTOCOL_VERSION_INT BV_VERSION_INT(LIBBVPROTOCOL_VERSION_MAJOR, \
                                               LIBBVPROTOCOL_VERSION_MINOR, \
                                               LIBBVPROTOCOL_VERSION_MICRO)
#define LIBPROTOCOL_VERSION     BV_VERSION(LIBBVPROTOCOL_VERSION_MAJOR, \
                                           LIBBVPROTOCOL_VERSION_MINOR, \
                                           LIBBVPROTOCOL_VERSION_MICRO)
#define LIBPROTOCOL_BUILD       LIBBVPROTOCOL_VERSION_INT

#define LIBPROTOCOL_IDENT       "Lbvprto" BV_STRINGIFY(LIBBVPROTOCOL_VERSION)


#endif /* end of include guard: BV_PROTOCOL_VERSION_H */

