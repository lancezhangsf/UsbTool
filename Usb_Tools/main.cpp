/*
 * Main.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */

#include "Main.h"

int
main(int argc,char *argv[])
{
#if 1
  InitLogging("Usb_Tool",INFO,"./log/UsbTool");
  bzero(serialName,MAX_COM);
  //1.find all serial,count 个串口
  int serial_count = get_all_Serial(serialName);
  if((serial_count > 0) && (serial_count < MAX_COM))
  {
    for(int i = 0;i < serial_count;i++)
    {
      sc[i] = new Serialcom();
      sc[i]->setSerialName(serialName[i]);
    }
  }

  cout << "=====================" << "Serial  Number:" << serial_count
      << "=====================" << endl;
  Logger::GetInstance().Info("find serial :%d ",serial_count);

  //release all serial port
  release_all_Serial(serialName,serial_count);
#endif

  //3. find all usb，2个USB device
  int usb_count = getAll_Usb_SerailNumber(usbSerialNumber);
  if((usb_count > 0) && (usb_count < MAX_USB_NUM))
  {
    for(int i = 0;i < usb_count;i++)
    {
      bbi[i] = new BackBoardInfo();
      bbi[i]->setSerialNumber(usbSerialNumber[i]);
      cout << bbi[i]->getSerialNumber() << endl;
    }
  }

  cout << "=======================" << "Usb Number:" << usb_count
      << "=======================" << endl;
  Logger::GetInstance().Info("find usb :%d ",usb_count);
  //release all usb serial
  release_all_Usb(usbSerialNumber,usb_count);

  //4. control all usb  for bb powerup
  fd_set fdstr;
  int ret = -1;
  int maxfd = 0;
  FD_ZERO(&fdstr);
  struct timeval tv;

  // 打开所有的串口,监听所有的串口，
  for(int i = 0;i < serial_count;i++)
  {
    int fd = sc[i]->SerialcomOpen();
    if(fd > 0)
    {
      if(fd > maxfd)
	maxfd = fd;
      FD_SET(fd,&fdstr);
    };
  }
  //send usb[0] open slotnum powerup  1 time
  int current_usb = 0, statistic_powerup = 0;
  int disk_num = 0;
  if(usb_count > 0)
  {
    if(!bbi[current_usb]->isBackBoardOpen())
    {
      bbi[current_usb]->BackBoardOpen();
    }
    bbi[current_usb]->BackBoardWrite(
	(unsigned char *)"slot[0] powerup msg",1024); //usb[0]口的第一slot上电信息
    cout << bbi[current_usb]->getSerialNumber() << "->send:" << endl;
    // ++statistic_powerup;//slot msg
  }

  unsigned char readbuffer[MAX_BUFFER_LENGTH] =
  { 0 };
  unsigned char writebuffer[MAX_BUFFER_LENGTH] =
  { 0 };

  bool isOver = false;
  while(1)
  {
    tv.tv_sec = 1;
    tv.tv_usec = 5;//每隔5 us轮询一次
    // select读取fd数据
    int powerup_slot;
    ret = select(maxfd + 1,&fdstr,NULL,NULL,&tv);
    if(ret <= 0) // <0 error =0 timeout
    {
      usleep(10000);
    }
    else if(ret > 0)
    {
      for(int i = 0;i < serial_count;i++)
      {
	if(FD_ISSET(sc[i]->getSerialfd(), &fdstr))
	{
	  //sc[i] receive data ,bbi[current_usb]
	  sc[i]->SerialcomRead((char *)readbuffer,1024);
	  cout << sc[i]->getSerialName() << "receive data:"
	      << "slot[0] powerup" << endl;
	  di[disk_num] = new DiskInfo(sc[i]);
	  di[disk_num]->bbi = bbi[current_usb];
	  di[disk_num]->slotNum = statistic_powerup;
	  disk_num++;
	  powerup_slot = statistic_powerup;
	  //send current_usb slot1 powerup
	  if(!bbi[current_usb]->isBackBoardOpen())
	  {
	    bbi[current_usb]->BackBoardOpen();
	  }
	  ++statistic_powerup;
	  cout << "Powerup Slot: " << powerup_slot << endl; //signed powerup slot
	  bbi[current_usb]->BackBoardWrite((unsigned char *)writebuffer,1024); //usb[0]口的第一slot上电信息
	  cout << bbi[current_usb]->getSerialNumber() << "->send" << endl;
	  if(statistic_powerup >= 3) // bb[0]  slot[3] have powered up
	  {
	    statistic_powerup = 0;
	    ++current_usb; //jump next backboard
	    if((disk_num >= MAX_DISK) || (current_usb >= MAX_USB))
	    {
	      cout << "disk_num:" << disk_num << "usb_num:" << current_usb
		  << endl;
	      isOver = true;
	      break;
	    }
	  }

	}
	else
	{
	  printf("serial error");
	}
      }
    }

    if(isOver)
    {
      break;
    }

  }

  cout << "==================" << "Relation Bind Complet"
      << "===================" << endl;
  //write xml and send ft.bin
  RelationBuild(di,disk_num);

  cout << "====================" << "Xml Build Complete"
      << "====================" << endl;
  //down ft.bin
  //close usb device
  for(int i = 0;i < usb_count;i++)
  {
    bbi[i]->BackBoardClose();
    cout << bbi[i]->getSerialNumber() <<"Close"<< endl;
  }

  //DownFt("./ft.bin",disk_num,di);
  {
    for(int i = 0;i < disk_num;i++)
    {
      char tmp[4] =
      { 0 };
      if(di[i]->diskop->xmOperator->XMSendFile(ftfilepath))
      {
	//disk---i down ft OK  // update xml
	//di[i]
	bzero(tmp,4);
	memcpy(tmp,&i,4);
	di[i]->diskop->xmOperator->sc->SerialcomClose();
	updateXmlFile(
	    "Relation.xml","Disks","SlotNumber",tmp,"DownFt","Success");
      }
      else
      {
	bzero(tmp,4);
	memcpy(tmp,&i,4);
	di[i]->diskop->xmOperator->sc->SerialcomClose();
	updateXmlFile(
	    "Relation.xml","Disks","SlotNumber",tmp,"DownFt","Failure");
      };
    }
  }

  return 0;
}
