/*
 * Main.h
 *
 *  Created on: Jul 27, 2015
 *      Author: root
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "./Util/Common.h"
#include "./Util/SerialFind.h"
#include "./Util/UsbFind.h"
#include "./DiskInfo/BackBoard.h"
#include "./DiskInfo/DiskInfo.h"
#include "./LogRecord/Logger.h"
#include "./LogRecord/FileHelper.h"
#include "./LogRecord/Mutex.h"

#include "./Protocol/Xmodem.h"

#include "./XmlParse/tinystr.h"
#include "./XmlParse/tinyxml.h"
#include "./XmlParse/tinyxmlconfig.h"

#include "./HidLib/hid_interface.h"
#include "./SerialPort/SerialPort.h"

using namespace std;
using namespace DISK;
using namespace FrameWork;
#define MAX_DISK   8

char *serialName[MAX_COM];// save all serialName
char *usbSerialNumber[MAX_USB_NUM];//save all usb serialnumber;
BackBoardInfo *bbi[MAX_USB]; //bb number;
Serialcom *sc[MAX_COM];
DiskInfo *di[MAX_DISK];

#define ftfilepath  "./ft.bin"



static struct option long_options[] =
{
{ "open", required_argument, NULL, 'o' },
{ "close", required_argument, NULL, 'c' },
{ "help", no_argument, NULL, 'h' },
{ "Help", no_argument, NULL, 'H' },
{ "voltage", required_argument, NULL, 'v' },
{ "version", no_argument, NULL, 'V' },
{ 0, 0, 0, 0 } };

struct Msg_op
{
    char *device_name;
    char cmd;
    char *cmd_data;
    int do_help;
    int do_verbose;
};

int
RelationBuild(DiskInfo *di[],int disk_num)
{
  {
    TiXmlDocument *writeDoc = new TiXmlDocument;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","yes");
    writeDoc->LinkEndChild(decl);

    int n = disk_num; //SearchMode number;
    TiXmlElement *RootElement = new TiXmlElement("Disks");//rootElement
    //  RootElement->SetAttribute("Count",n);
    writeDoc->LinkEndChild(RootElement);
    //char *SearchMode[];
    for(int i = 0;i < n;i++)
    {
      TiXmlElement *SearchModeElement = new TiXmlElement("Disk");//tixmlelement
      //        SearchModeElement->SetAttribute("Name","Numbers");
      //        SearchModeElement->SetAttribute("Count",4);
      //        SearchModeElement->SetAttribute("Space",10000);
      RootElement->LinkEndChild(SearchModeElement);
      for(int j = 0;j < 1;j++)
      {
	TiXmlElement *RegExElement = new TiXmlElement("BBSerialNumber");//element
	SearchModeElement->LinkEndChild(RegExElement);
	TiXmlText *RegExContent = new TiXmlText(di[i]->bbi->getSerialNumber());
	//TiXmlText *RegExContent = new TiXmlText("1234567");
	RegExElement->LinkEndChild(RegExContent);
#if 1
	TiXmlElement *RegExElement1 = new TiXmlElement("SlotNumber");//element
	SearchModeElement->LinkEndChild(RegExElement1);
	char tmp[4] =
	{ 0 };
	memcpy(tmp,&(di[i]->slotNum),4);
	TiXmlText *RegExContent1 = new TiXmlText(tmp);
	//TiXmlText *RegExContent1 = new TiXmlText("1");
	RegExElement1->LinkEndChild(RegExContent1);

	TiXmlElement *RegExElement2 = new TiXmlElement("SerialName");//element
	SearchModeElement->LinkEndChild(RegExElement2);
	TiXmlText *RegExContent2 = new TiXmlText(di[i]->sc->getSerialName());
	//TiXmlText *RegExContent2 = new TiXmlText("/dev/ttyS5");
	RegExElement2->LinkEndChild(RegExContent2);

	TiXmlElement *RegExElement3 = new TiXmlElement("DownFt");//element
	SearchModeElement->LinkEndChild(RegExElement3);
	TiXmlText *RegExContent3 = new TiXmlText("0");
	//TiXmlText *RegExContent2 = new TiXmlText("/dev/ttyS5");
	RegExElement3->LinkEndChild(RegExContent3);

	//

#endif
      }
    }
    writeDoc->SaveFile("Relation.xml");
    delete writeDoc;
    return RET_SUCCESS;
  }
}

int
usb_InfoBuild(Msg_op *op)
{

  TiXmlDocument *writeDoc = new TiXmlDocument;
  TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","yes");
  writeDoc->LinkEndChild(decl);

  int n = 1; //SearchMode number;


  TiXmlElement *RootElement = new TiXmlElement("BackBoards");//rootElement
  // RootElement->SetAttribute("Count",n);
  writeDoc->LinkEndChild(RootElement);
  //char *SearchMode[];
  for(int i = 0;i < n;i++)
  {
    TiXmlElement *SearchModeElement = new TiXmlElement("BackBoard");//tixmlelement
    //        SearchModeElement->SetAttribute("Name","Numbers");
    //        SearchModeElement->SetAttribute("Count",4);
    //        SearchModeElement->SetAttribute("Space",10000);
    RootElement->LinkEndChild(SearchModeElement);
    for(int j = 0;j < 1;j++)
    {

      TiXmlElement *RegExElement = new TiXmlElement("DeviceName");//element
      SearchModeElement->LinkEndChild(RegExElement);

      TiXmlText *RegExContent = new TiXmlText(op->device_name);
      RegExElement->LinkEndChild(RegExContent);

      TiXmlElement *RegExElement1 = new TiXmlElement("Voltage");//element
      SearchModeElement->LinkEndChild(RegExElement1);

      TiXmlText *RegExContent1 = new TiXmlText("05");
      RegExElement1->LinkEndChild(RegExContent1);

      TiXmlElement *RegExElement2 = new TiXmlElement("Open");//element
      SearchModeElement->LinkEndChild(RegExElement2);

      TiXmlText *RegExContent2 = new TiXmlText("1111");
      RegExElement2->LinkEndChild(RegExContent2);

      TiXmlElement *RegExElement3 = new TiXmlElement("Close");//element
      SearchModeElement->LinkEndChild(RegExElement3);

      TiXmlText *RegExContent3 = new TiXmlText("0000");
      RegExElement3->LinkEndChild(RegExContent3);

    }
  }
  writeDoc->SaveFile("RegExWithModes.xml");
  delete writeDoc;
  return 1;

}

int
DownFt(const char *ftpath,int disk_num,DiskInfo *di[])
{

  return 1;
}
;

#endif /* MAIN_H_ */
