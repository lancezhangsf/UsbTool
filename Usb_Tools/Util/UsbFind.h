/*
 * UsbFind.h
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */

#ifndef USBFIND_H_
#define USBFIND_H_

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "../HidLib/hidapi.h"

// Headers needed for sleeping.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define  VID 0x2047
#define  PID 0x0301
#define  SERIAL_NUMBER_LENGTH  12
using namespace std;


int
getAll_Usb_SerailNumber(char **usbSerialNumber);

void
release_Usb(char **usbAllSerialNumber,char *usbSerialNum,int count);

void
release_all_Usb(char **usbAllSerialNumber,int count);





#endif /* USBFIND_H_ */
