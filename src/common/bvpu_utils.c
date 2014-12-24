/**
 *          File: bvpu_utils.c
 *
 *        Create: 2014年12月19日 星期五 14时13分24秒
 *
 *   Discription: 
 *
 *        Author: yuwei.zhang
 *         Email: yuwei.zhang@besovideo.com
 *
 *===========================================================================
 */
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
 * Copyright (C) @ BesoVideo, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 
// Common functions
//
#include "bvpu_utils.h"

int bvpu_parse_data(unsigned char *data, char *string)
{
    char p[128];
    char *str;
    char *token;
    int  i = 0;
    int val = 0;

    strncpy(p, string, strlen(string));

    for (str = p;;str = NULL)
    {
        token = strtok(str, ",");
        if (token == NULL) {
            break;
        } else {
            sscanf(token, "%02x", &val);
            data[i++] = val;
        }
    }
    
    return i+1;
}

int bvpu_parse_data_string(char *string, char *save, int arrsize, int maxlen, const char sign)
{
    if (string == NULL || save == NULL || arrsize < 0 || maxlen < 0) return -1;
    int      i = 0, j = 0;
    int      counts = 0;
    char    *pos = index(string, sign);
    if (!pos) return -2;
    while((pos = index(pos, sign))) {
        pos++;
        counts++;
    };
    counts += 1;   // we find counts

    pos = string;
    memset(save, 0, arrsize*maxlen);
    for (i = 0; i < maxlen; i++) {
        if (i >= counts) break;    // only deal we find counts
        char *p = save+i*(arrsize);
        for (j = 0; j < arrsize;j++) {
            if (j == arrsize-1 || *pos == sign || *pos == '\0') { 
                *(p+j) = '\0'; 
                if ((pos = index(pos, sign))) pos++;
                break;
            }
            *(p+j) = *pos++;
        }   
    }
    return i;    // return what we realy deal, maybe not maxlen
}

int bvpu_parse_data_char(unsigned char *data, char *string, int maxlen, const char *sign)
{
    char p[128];
    char *str;
    char *token;
    int  i = 0;
    char val;

    strncpy(p, string, strlen(string));

    for (str = p;;str = NULL)
    {
        if (i >= maxlen) break;
        token = strtok(str, sign);
        if (token == NULL) {
            break;
        } else {
            sscanf(token, "%c", &val);
            data[i++] = val;
        }
    }
    
    return i;
}

int bvpu_parse_string_int(char *string, int *save, int maxlen, const char *sign)
{
    char  p[128];
    char *str;
    char *token;
    int   i = 0;
    int   val = 0;

    strncpy(p, string, strlen(string));

    for (str = p;;str = NULL)
    {
        if (i >= maxlen) break;
        token = strtok(str, sign);
        if (token == NULL) {
            break;
        } else {
            sscanf(token, "%d", &val);
            save[i++] = val;
        }
    }
    
    return i;
}

// (xx,xx);(xx,xx)
// We don't control if great then imageSize max size
int bvpu_parse_string_imagerects(char *string, BVPU_SDK_ImageSize *imageSize, const char *sign)
{
    if (!string || !imageSize) return -1;

    BVPU_SDK_ImageSize *inImage = imageSize;

    char tmp[512] = {0};
    sprintf(tmp, "%s", string);
    char *p = tmp;
    char *str = NULL;
    char *token = NULL;
    int i = -1;
    for (str = p; ; str = NULL) {
        token = strtok(str, sign);
        if (token == NULL) {
            break;
        } else {
            sscanf(token, "(%d,%d)", &inImage->iWidth, &inImage->iHeight);
            inImage++;   // goto next
            i++;
        }
    }
   
    return i+1;
}

int bvpu_clear_parenthesis(char *save, char *string)
{
    if (save == NULL || string == NULL) return -1;

    int    len = strlen(string);
    if (string[0] != '(' && string[len-1] != ')') return -2;

    int    i = 0;
    int    j = 1;
    if (string[0] != '(') j = 0;
    for (i = 0; i < len; i++) {
        if (string[i] == ')') break;
        save[i] = string[j++];
    }

    return 0;
}
int bvpu_clear_parenthesis_self(char *string)
{
    if (string == NULL) return -1;
    
    char  save[1024] = {0};
    bvpu_clear_parenthesis(save, string);
    return sprintf(string, "%s", save);
}

int bvpu_in_walltime(BVPU_SDK_WallTime *start, BVPU_SDK_WallTime *end, struct tm *now)
{
    if (!start || !end || !now) return BVPU_SDK_RESULT_E_INVALIDPARAM;
#if 0
    BVINFO("[%d:%d:%d-%d:%d:%d, %d:%d:%d]\n", start->iYear, start->iMonth, start->iDay,
              end->iYear, end->iMonth, end->iDay, 
              now->tm_year, now->tm_mon, now->tm_mday);
#endif

    // now year is outside of start and end year
    int now_year = now->tm_year + 1900;
    int now_mon = now->tm_mon + 1;
    if (now_year > end->iYear || now_year < start->iYear) {
        return -1;
    }

    // Others is valid year, judge month
    // 1. now year == start year, check if month and day valid
    // 2. now year == end year, check if month and day valid
    // 3. maybe now year == start year == end year, so need check
    //    start year and end year
    if (now_year == start->iYear) {
        // 1. cur mon < mon is fail
        // 2. cur mon > mon is valid
        // 3. cur mon == mon, need to check day
        if (now_mon < start->iMonth) return -1;
        if (now_mon == start->iMonth) {
            if (now->tm_mday < start->iDay) return -1;
        }
    } 

    if (now_year == end->iYear) {
        // same with upper rule
        if (now_mon > end->iMonth) return -1;
        if (now_mon == end->iMonth) {
            if (now->tm_mday > end->iDay) return -1;
        }
    }
 
    // if year, month, day check valid
    // next to check time
    // check time only when the start and end year,month,day are same
#if 0
    if (start->iYear == end->iYear && start->iMonth == end->iMonth 
           && start->iDay == end->iDay) {
BVINFO("%d,%d,%d-%d,%d,%d---%d,%d,%d-%d,%d,%d-- now %d,%d,%d,%d,%d,%d\n",
        start->iYear, start->iMonth, start->iDay, start->iHour, start->iMinute, start->iSecond,
        end->iYear, end->iMonth, end->iDay, end->iHour, end->iMinute, end->iSecond,
        now->tm_year, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
#endif
        
    time_t starttime = start->iHour * 3600 + start->iMinute * 60 + start->iSecond;
    time_t endtime = end->iHour * 3600 + end->iMinute * 60 + end->iSecond;
    time_t nowtime = now->tm_hour * 3600 + now->tm_min * 60 + now->tm_sec;
    // The same day, check time
    if (start->iYear == end->iYear && start->iMonth == end->iMonth &&
           start->iDay == end->iDay) {
        if (nowtime > endtime || nowtime < starttime) return -1;
    // The same with start day, check start time
    } else if (start->iYear == now_year && start->iMonth == now_mon
                  && start->iDay == now->tm_mday) {
        if (nowtime < starttime) return -1;
    // The same with end day, check end time
    } else if (end->iYear == now_year && end->iMonth == now_mon
                  && end->iDay == now->tm_mday) {
        if (nowtime > endtime) return -1;
    }
     
#if 0
    }
#endif

    return 0;
}

// 
// Check if now time in daytimeslice
int bvpu_in_daytimeslice(BVPU_SDK_DayTimeSlice *slice, struct tm *now)
{
    if (!slice || !now) return BVPU_SDK_RESULT_E_INVALIDPARAM;

    time_t nowsec = now->tm_hour * 3600 + now->tm_min * 60 + now->tm_sec;
    int j;
    // I just compare today depends on week day
    int today = now->tm_wday;    // what the week today
    for (j = 0; j < BVPU_SDK_MAX_DAYTIMESLICE_COUNT; j++) {
        //BV_DayTimeSlice *inSlice = &slice[i][j];
        // Here scanf day time slice * today begin from slice
        // And then is the read data
        BVPU_SDK_DayTimeSlice *inSlice = slice+today*BVPU_SDK_MAX_DAYTIMESLICE_COUNT+j;
        if (inSlice) {
#if 0
            BVINFO("compare time [%d][%d] [%d:%d:%d-%d:%d:%d]\n",
                  today, j, inSlice->cHourBegin, inSlice->cMinuteBegin, inSlice->cSecondBegin,
                  inSlice->cHourEnd, inSlice->cMinuteEnd, inSlice->cSecondEnd);
#endif
            time_t start = inSlice->cHourBegin * 3600 + 
                   inSlice->cMinuteBegin * 60 + inSlice->cSecondBegin;
            time_t end = inSlice->cHourEnd * 3600 + 
                   inSlice->cMinuteEnd * 60 + inSlice->cSecondEnd;
            // Find one, ignore others
            // If start and end are 0 means don't snapshots any
            if (start == end && end == 0) return -1;
            if (nowsec >= start && nowsec <= end) {
                return 0;
            }
        }
    }

    return -1;
}

int bvpu_remove_last_break(char *args)
{
    if (args == NULL) return -1;

    int  len = strlen(args);
    if (args[len-1] == '\n') 
        args[len-1] = '\0';

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
///////////////   Network
/////////////////////////////////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define h_addr h_addr_list[0]
char *bvpu_get_ip(char *dn_or_ip, const char *eth)
{
   struct hostent *host;
   struct ifreq req;
   int sock;
   if (dn_or_ip == NULL) return NULL;
   if (strcmp(dn_or_ip, "localhost") == 0) 
   {
	  sock = socket(AF_INET, SOCK_DGRAM, 0);
	  strncpy(req.ifr_name, eth, IFNAMSIZ);
	  if ( ioctl(sock, SIOCGIFADDR, &req) < 0 ) 
	  {
		 printf("ioctl error: %s\n", strerror (errno));
		 return NULL;
	  }
	  dn_or_ip = (char *)inet_ntoa(*(struct in_addr *) &((struct sockaddr_in *) &req.ifr_addr)->sin_addr);
	  shutdown(sock, 2);
	  close(sock);
   } 
   else 
   {
	  host = gethostbyname(dn_or_ip);
	  if (host == NULL) 
	  {
		 return NULL;
	  }
	  dn_or_ip = (char *)inet_ntoa(*(struct in_addr *)(host->h_addr));
   }
   return dn_or_ip;
}

int bvpu_get_flow(const char *interface, unsigned long long *recv,unsigned long long *send, unsigned long long *total)
{
    int ret = 0;
    unsigned int l1,l2;
    FILE *fp = fopen("/proc/net/dev","r");
    if(!fp) {
        perror("fail to open /proc/net/dev");
        if(recv)  *recv  = 0; 
        if(send)  *send  = 0; 
        if(total) *total = 0; 
        return -1; 
    }   
    char buf[512];
    while(fgets(buf,512,fp)) {
        if(strstr(buf,interface)) {
            char *ptr;
            if((ptr = strchr(buf, ':')) == NULL) {
                ret = -1; 
                goto out;
            }   
            sscanf(++ptr,"%d",&l1);
            sscanf(++ptr,"%*d%*d%*d%*d%*d%*d%*d%*d%d",&l2);
            if(recv)  *recv  = l1; 
            if(send)  *send  = l2; 
            if(total) *total = l1 + l2; 
            goto out;
        }   
    }   
out:
    if (fp) { fclose(fp); fp = NULL; }
    return ret;
}



/*=============== End of file: bvpu_utils.c ==========================*/
