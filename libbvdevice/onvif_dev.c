/*************************************************************************
    > File Name: onvif_dev.c
    > Author: albertfang
    > Mail: fang.qi@besovideo.com 
    > Created Time: 2014年12月29日 星期一 10时17分53秒
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

#define _GNU_SOURCE
#include <wsseapi.h>
#include <wsaapi.h>
#include <string.h>

#include "bvdevice.h"

#define ONVIF_NVT "NetWorkVideoTransmitter"
#define ONVIF_NVS "NetWorkVideoStorage"
#define ONVIF_NVD "NetWorkVideoDisplay"
#define ONVIF_NVA "NetWorkVideoAnalytics"

#define ONVIF_TMO    (-500000)

typedef struct OnvifDeviceContext {
    BVClass *bv_class;
    struct soap *soap;
    int timeout;
} OnvifDeviceContext;

static struct soap *bv_soap_new(int timeout)
{
    struct soap *soap = NULL;
    soap = soap_new();
    if (soap == NULL) {
        return NULL;
    }
    soap_set_namespaces(soap, namespaces);
    if (!timeout) {
        timeout = ONVIF_TMO;
    }
    soap->recv_timeout = timeout;
    soap->send_timeout = timeout;
    soap->connect_timeout = timeout;
    return soap;
}

static void bv_soap_free(struct soap *soap)
{
    if (soap == NULL) {
        bv_log(NULL, BV_LOG_ERROR, "Soap is NULL\n");
        return;
    }
    soap_destroy(soap);
    soap_end(soap);
    soap_done(soap);
    soap_free(soap);
}

static void set_device_info(BVMobileDevice *device_info, struct __wsdd__ProbeMatches *resp)
{
    char *input_str = resp->wsdd__ProbeMatches->ProbeMatch->Types;
#if 1
    bv_log(NULL, BV_LOG_INFO, "Target Service Address  : %s\r\n",
        resp->wsdd__ProbeMatches->ProbeMatch->XAddrs);
    bv_log(NULL, BV_LOG_INFO, "Target EP Address       : %s\r\n",
        resp->wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);
    bv_log(NULL, BV_LOG_INFO, "Target Type             : %s\r\n",
        resp->wsdd__ProbeMatches->ProbeMatch->Types);
    bv_log(NULL, BV_LOG_INFO, "Target Metadata Version : %d\r\n",
        resp->wsdd__ProbeMatches->ProbeMatch->MetadataVersion);
#endif
    strncpy(device_info->url, resp->wsdd__ProbeMatches->ProbeMatch->XAddrs, sizeof(device_info->url) -1);
    device_info->timeout = ONVIF_TMO;
    device_info->type = BV_MOBILE_DEVICE_TYPE_NONE;
    if(strcasestr(input_str,ONVIF_NVS)) {
        bv_log(NULL, BV_LOG_INFO, "device is Storage\n");
        device_info->type |= BV_MOBILE_DEVICE_TYPE_NVS;
    }
    if(strcasestr(input_str,ONVIF_NVT)) {
        bv_log(NULL, BV_LOG_INFO, "device is Transmitter\n");
        device_info->type |= BV_MOBILE_DEVICE_TYPE_NVT;
    }
    if(strcasestr(input_str,ONVIF_NVA)) {
        bv_log(NULL, BV_LOG_INFO, "device is Analytics\n");
        device_info->type |= BV_MOBILE_DEVICE_TYPE_NVA;
    }
    if(strcasestr(input_str,ONVIF_NVD)) {
        bv_log(NULL, BV_LOG_INFO, "device is Display\n");
        device_info->type |= BV_MOBILE_DEVICE_TYPE_NVD;
    }
}

static int onvif_device_scan(BVDeviceContext *h, BVMobileDevice *device, int *max_ret)
{
    OnvifDeviceContext *devctx = h->priv_data;
    BVMobileDevice *current = device;
    int device_num = 0;
    int retval = SOAP_OK;
    wsdd__ProbeType req;
    struct __wsdd__ProbeMatches resp;
    wsdd__ScopesType sScope;
    struct SOAP_ENV__Header header;
    struct soap *soap;

    //这个就是传递过去的组播的ip地址和对应的端口发送广播信息  
    const char *soap_endpoint = "soap.udp://239.255.255.250:3702/";

    //这个接口填充一些信息并new返回一个soap对象，本来可以不用额外接口，
    // 但是后期会作其他操作，此部分剔除出来后面的操作就相对简单了,只是调用接口就好
    //soap = bv_soap_new(&header, was_To, was_Action, 5);
    soap = devctx->soap;

    soap_default_SOAP_ENV__Header(soap, &header);

    header.wsa__MessageID = (char *)soap_wsa_rand_uuid(soap);
    header.wsa__To = (char *) "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
    header.wsa__Action = (char *) "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
    soap->header = &header;

    soap_default_wsdd__ScopesType(soap, &sScope);
    sScope.__item = (char *)"";
    soap_default_wsdd__ProbeType(soap, &req);
    req.Scopes = &sScope;
    req.Types = (char *)"";                //"dn:NetworkVideoTransmitter";
    retval = soap_send___wsdd__Probe(soap, soap_endpoint, NULL, &req);
    //发送组播消息成功后，开始循环接收各位设备发送过来的消息
    while (retval == SOAP_OK) {
        retval = soap_recv___wsdd__ProbeMatches(soap, &resp);
        if (retval == SOAP_OK) {
            if (resp.wsdd__ProbeMatches &&resp.wsdd__ProbeMatches->ProbeMatch != NULL
                && resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL) {
                current = device + device_num;
                set_device_info(current, &resp);
                device_num ++;
                if(device_num > *max_ret) {
                    bv_log(NULL, BV_LOG_INFO, "reach max ret %d\n", *max_ret);
                    break;
                }
            }
        } else if (soap->error) {
            bv_log(NULL, BV_LOG_INFO, "[%d]: recv soap error :%d, %s, %s\n", __LINE__, soap->error,
                *soap_faultcode(soap), *soap_faultstring(soap));
            retval = soap->error;
        }
    }

    *max_ret = device_num;
    return device_num > 0 ? device_num : -1;
}

static int onvif_device_open(BVDeviceContext *h)
{
    OnvifDeviceContext *devctx = h->priv_data;
    devctx->soap = bv_soap_new(devctx->timeout);
    if (!devctx->soap) {
        return BVERROR(ENOMEM);
    }
    return 0;
}
static int onvif_device_probe(BVDeviceContext *h, const char *args)
{
    if (strcmp("onvif_dev", args) == 0) {
        return 100;
    }
    return 0;
}

static int onvif_device_close(BVDeviceContext *h)
{
    OnvifDeviceContext *devctx = h->priv_data;
    bv_soap_free(devctx->soap);
    return 0;
}

#define OFFSET(x) offsetof(OnvifDeviceContext, x)
#define DEC BV_OPT_FLAG_DECODING_PARAM
static const BVOption options[] = {
    {"timeout", "read write time out", OFFSET(timeout), BV_OPT_TYPE_INT, {.i64 =  -500000}, INT_MIN, INT_MAX, DEC},
    {NULL}
};

static const BVClass onvif_class = {
    .class_name     = "onvif device",
    .item_name      = bv_default_item_name,
    .option         = options,
    .version        = LIBBVUTIL_VERSION_INT,
    .category       = BV_CLASS_CATEGORY_DEVICE,
};

BVDevice bv_onvif_dev_device = {
    .name           = "onvif_dev",
    .type           = BV_DEVICE_TYPE_ONVIF_DEVICE,
    .priv_data_size = sizeof(OnvifDeviceContext),
    .dev_open       = onvif_device_open,
    .dev_probe      = onvif_device_probe,
    .dev_close      = onvif_device_close,
    .dev_scan       = onvif_device_scan,
    .priv_class     = &onvif_class,
};
