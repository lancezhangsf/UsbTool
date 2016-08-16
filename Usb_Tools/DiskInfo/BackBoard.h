/*
 * BackBoard.h
 *
 *  Created on: Jul 27, 2015
 *      Author: root
 */

#ifndef BACKBOARD_H_
#define BACKBOARD_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include "../HidLib/hidapi.h"
#include "../DataHandler/ReceiveDataHandle.h"
#include "../DataHandler/SendDataHandle.h"
#include "../HidLib/hid_interface.h"

#include "../Util/Common.h"

using namespace std;
using namespace DATA;
namespace DISK
{
#define MAX_SLOT_NUM   4
#define MAX_USB        2
#define SERIAL_NUMBER_LENGTH  12

  class BackBoardInfo
  {

    public:
      ReceiveData *rd; //receive data handler
      SendData *sd; //send    data handler

    private:
      char *serialNumber; //标记背板的串号，用于区分 背板
      string usbName; //记录盘片所在的插槽号?可删除
      HidUsb *HidOp;
      int slotNum[MAX_SLOT_NUM];
      bool isOpen;

    public:

      BackBoardInfo()
      {
	this->HidOp = new HidUsb();
	bzero(slotNum,MAX_SLOT_NUM);
	this->serialNumber = this->HidOp->getUsbSerialNumber();
	isOpen = false;
	this->rd = new ReceiveData();
	this->sd = new SendData();

      }

      char *
      getSerialNumber()
      {

	return this->serialNumber;
      }

      void
      setSerialNumebr(char *serialNumber)
      {

	strcpy(this->serialNumber,serialNumber);

      }
      int
      BackBoardOpen()
      {

	hid_device *handle_hid;
	handle_hid = this->HidOp->openHidDev((wchar_t *)this->serialNumber);
	if(!handle_hid)
	{
	  return RET_ERROR;
	}
	else
	{
	  this->isOpen = true;
	  return RET_SUCCESS;
	}

      }

      bool
      isBackBoardOpen()
      {

	return this->isOpen;
      }
      int
      BackBoardRead(unsigned char *buf,int len)
      {
	int result;
	result = this->HidOp->readDev(buf,len);
	if(result < 0)
	{

	  printf("read failure\n");
	  return RET_ERROR;
	}
	printf("Len:%d",result);
	return RET_SUCCESS;

      }

      int
      BackBoardWrite(unsigned char *buf,int len)
      {

	int result;
	result = this->HidOp->writeDev(buf,len);
	if(result < 0)
	{

	  printf("Write failure\n");
	  return RET_ERROR;
	}
	printf("Write Len:%d",result);
	return RET_SUCCESS;

      }

      void
      BackBoardClose()
      {
	this->HidOp->CloseDev();
      }
      ~BackBoardInfo()
      {
	delete HidOp;
	delete rd;
	delete sd;

      }
      void
      setSerialNumber(char * serialnumber)
      {

	serialNumber = serialnumber;
      }

  };

}

#endif /* BACKBOARD_H_ */
