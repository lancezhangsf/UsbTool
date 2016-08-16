/*
 * downftTest.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: root
 */

#include "Main.h"

int
main(int argc,char *argv[])
{

#if 1
  InitLogging("Usb_Tool",INFO,"./log/UsbTool");
  bzero(serialName,MAX_COM);
  //1.find all serial,count 个串口
  int i;
  int serial_count = get_all_Serial(serialName);
  if((serial_count > 0) && (serial_count < MAX_COM))
  {
    for( i = 0;i < serial_count;i++)
    {
      sc[i] = new Serialcom();
      sc[i]->setSerialName(serialName[i]);
      if(strcmp(serialName[i],"/dev/ttyUSB0")==0){
	break;
      }
    }
  }

  cout << "=====================" << "Serial  Number:" << serial_count
      << "=====================" << endl;
  Logger::GetInstance().Info("find serial :%d ",serial_count);

  //release all serial port
  release_all_Serial(serialName,serial_count);
#endif

  int fd = sc[i]->SerialcomOpen();
  if(fd > 0)
  {
    perror("open failure");
  };


  DiskInfo di(sc[i]);
  if(di.diskop->xmOperator->XMSendFile(ftfilepath))
  {
    //disk---i down ft OK  // update xml
    //di[i]
    printf("send ok\n");
    //   updateXmlFile("Relation.xml","Disks","SlotNumber",tmp,"DownFt","Success");
  }

  return 1;
}
