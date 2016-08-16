/*
 * UsbFind.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */
#include "UsbFind.h"
int
getAll_Usb_SerailNumber(char **usbSerialNumber)
{

  int usbdev_num = 0;

  struct hid_device_info *devs, *cur_dev;

  devs = hid_enumerate(0x0,0x0);
  cur_dev = devs;
  while(cur_dev)
  {
    printf(
	"Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
	cur_dev->vendor_id,cur_dev->product_id,cur_dev->path,
	cur_dev->serial_number);

    if((cur_dev->vendor_id == VID) && (cur_dev->product_id == PID))
    {
      usbSerialNumber[usbdev_num] = new char[SERIAL_NUMBER_LENGTH];
      bzero(usbSerialNumber,SERIAL_NUMBER_LENGTH);

      strcpy(usbSerialNumber[usbdev_num],(const char *)cur_dev->serial_number);
      usbdev_num++;
    }
    printf("\n");
    printf("  Manufacturer: %ls\n",cur_dev->manufacturer_string);
    printf("  Product:      %ls\n",cur_dev->product_string);
    printf("  Release:      %hx\n",cur_dev->release_number);
    printf("  Interface:    %d\n",cur_dev->interface_number);
    printf("\n");
    cur_dev = cur_dev->next;

  }
  printf("usb list is empty\n");
  hid_free_enumeration(devs);
  return usbdev_num;
}

void
release_Usb(char **usbAllSerialNumber,char *usbSerialNum,int count)
{

  if((usbAllSerialNumber == NULL) || (usbSerialNum == NULL))
  {
    cout << "Usb serial Number is NULL" << endl;
  }

  for(int i = 0;i < count;i++)
  {

    if(!(strcmp(usbAllSerialNumber[i],usbSerialNum)))
    {
      cout << "release:" << usbSerialNum << endl;
      free(usbAllSerialNumber[i]);
    }
  }

}

void
release_all_Usb(char **usbAllSerialNumber,int count)
{
  if((usbAllSerialNumber == NULL) || (count == 0))
  {
    cout << "Usb serial Number is NULL" << endl;
  }

  for(int i = 0;i < count;i++)
  {
    cout << "release:" << usbAllSerialNumber[i] << endl;
    free(usbAllSerialNumber[i]);

  }

}
