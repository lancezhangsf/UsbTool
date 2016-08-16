/*
 * hid_interface.cpp
 *
 *  Created on: Jul 24, 2015
 *      Author: lance
 */

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hidapi.h"
#include "../Util/UsbFind.h"

#ifndef HID_INTERFACE_H
#define HID_INTERFACE_H

// Headers needed for sleeping.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../Util/Common.h"

#define MAX_STR 255
#define MAX_USB_NUM 64


class HidUsb
{

  private:
    int usbdev_num;
    hid_device *handle;
    struct hid_device_info *devs, *cur_dev;
    //  char *usbSerialNumber[MAX_USB_NUM];
    char *usbSerialNumber;
    static int count;

  public:
    HidUsb();

    char *
    getUsbSerialNumber();

    hid_device *
    openHidDev(wchar_t *serial_number);

    int
    readDev(unsigned char *data,size_t length);
    int
    writeDev(unsigned char *data,size_t length);
    void
    CloseDev();

    void
    releaseAllDevSerailNumber();

    ~HidUsb();


};

#endif
