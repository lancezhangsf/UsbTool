/*
 * hid_interfaceimp.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: root
 */

#include "./hid_interface.h"

int HidUsb::count = 0;

HidUsb::HidUsb()
{

  this->usbdev_num = 0;
  this->usbSerialNumber = new char[SERIAL_NUMBER_LENGTH];
  //memset(usbSerialNumber,0,MAX_USB_NUM);

  //getAllDevSerailNumber();
}

char *
HidUsb::getUsbSerialNumber()
{
#if 0
  if(count == 0)
  {

    printf("no usbDevice\n");
    return NULL;

  }

  if(count >= this->usbdev_num)
  {
    printf("over flow getted devices\n");
    return NULL;
  }
  return usbSerialNumber[count];
  ++count;

#endif
  return this->usbSerialNumber;

}

hid_device *
HidUsb::openHidDev(wchar_t *serial_number)
{
  if(serial_number == NULL)
  {
    printf("serial_number:%s is NULL\n",(char *)serial_number);
    return NULL;
  }
  this->handle = hid_open(VID,PID,serial_number);
  return this->handle;
}

int
HidUsb::readDev(unsigned char *data,size_t length)
{

  assert(data != NULL);
  //set read ,no blocking
  int result;
  result = hid_set_nonblocking(this->handle,1);

  if(result != RET_SUCCESS)
  {
    printf("set noblock error\n");

  }
  return hid_read(this->handle,data,length);
}
int
HidUsb::writeDev( unsigned char *data,size_t length)
{
  assert(data != NULL);
  //set read ,no blocking
  int result;
  result = hid_set_nonblocking(this->handle,1);
  if(result != RET_SUCCESS)
  {
    printf("set noblock error\n");

  }

  return hid_write(this->handle,data,length);
}

void
HidUsb::CloseDev()
{

  hid_close(this->handle);
  hid_exit();
}

void
HidUsb::releaseAllDevSerailNumber()
{


}

HidUsb::~HidUsb()
{
  delete[] this->usbSerialNumber;
}

