/*
 * main.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: root
 */

/*
 * xmldemo.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: lance
 */

#include<iostream>
#include <string.h>
#include <cstdlib>

#include "tinystr.h"
#include "tinyxml.h"

using namespace std;

bool
GetNodePointerByName(TiXmlElement* pRootEle,const char* strNodeName,
    TiXmlElement* &destNode)
{ //根据结点名获取结点指针
  // 假如等于根节点名，就退出
  if(0 == strcmp(strNodeName,pRootEle->Value()))
  {
    destNode = pRootEle;
    return true;
  }

  TiXmlElement* pEle = pRootEle;
  for(pEle = pRootEle->FirstChildElement();pEle;pEle
      = pEle->NextSiblingElement())
  {
    // 递归处理子节点，获取节点指针
    if(0 != strcmp(pEle->Value(),strNodeName))
    {
      GetNodePointerByName(pEle,strNodeName,destNode);
    }
    else
    {
      destNode = pEle;
      printf("destination node name: %s\n",pEle->Value());
      return true;
    }
  }
  return false;
}

int
readXmlFile(const char * XmlFilePath,const char * rootElement)
{
  TiXmlDocument *mydoc = new TiXmlDocument(XmlFilePath);
  bool loadOk = mydoc->LoadFile();
  if(!loadOk)
  {
    cout << "could not load the test file.Error:" << mydoc->ErrorDesc() << endl;
    exit(1);
  }

  TiXmlElement *RootElement = mydoc->RootElement(); //根元素
  TiXmlElement *pEle = NULL;

  GetNodePointerByName(RootElement,rootElement,pEle); //找到值为SearchModes的结点

  //遍历该结点
  for(TiXmlElement *SearchModeElement = pEle->FirstChildElement();SearchModeElement;SearchModeElement
      = SearchModeElement->NextSiblingElement())
  {
    cout << SearchModeElement->Value() << " ";//disk
    TiXmlAttribute *pAttr = SearchModeElement->FirstAttribute();
    while(pAttr) //输出所有属性  DISK1.。。
    {
      cout << pAttr->Name() << ":" << pAttr->Value() << " ";
      pAttr = pAttr->Next();
    }
    cout << endl;
    {
      for(TiXmlElement *RegExElement = SearchModeElement->FirstChildElement();RegExElement;RegExElement
	  = RegExElement->NextSiblingElement())//输出子元素的值
      {

	cout << RegExElement->FirstChild()->Value() << endl; // SN,WWN.....
      }

    }
  }
  delete mydoc;
  return 1;
}
int
writeXmlFile()
{
  TiXmlDocument *writeDoc = new TiXmlDocument;
  TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","yes");
  writeDoc->LinkEndChild(decl);

  int n; //SearchMode number;
  cout << "Please input the number of Search Mode:";
  cin >> n;

  TiXmlElement *RootElement = new TiXmlElement("Disks");//rootElement
  //RootElement->SetAttribute("Count",n);
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
      TiXmlElement *RegExElement = new TiXmlElement("BackBoard");//element
      SearchModeElement->LinkEndChild(RegExElement);

      TiXmlText *RegExContent = new TiXmlText("[0-9]{2,2}");
      RegExElement->LinkEndChild(RegExContent);

      TiXmlElement *RegExElement1 = new TiXmlElement("disk");//element
      SearchModeElement->LinkEndChild(RegExElement1);

      TiXmlText *RegExContent1 = new TiXmlText("[0-9]{2,2}");
      RegExElement1->LinkEndChild(RegExContent1);
    }
  }
  writeDoc->SaveFile("RegExWithModes.xml");
  delete writeDoc;
  return 1;
}
bool
ModifyNode_Text(const char * XmlFile,const char *conditionNode,const char *str,
    const char * strNodeName,const char * strText)
{
  // 定义一个TiXmlDocument类指针
  TiXmlDocument *pDoc = new TiXmlDocument(XmlFile);
  if(NULL == pDoc)
  {
    return false;
  }

  pDoc->LoadFile();
  TiXmlElement *pRootEle = pDoc->RootElement();
  if(NULL == pRootEle)
  {
    return false;
  }

  TiXmlElement *pNode = NULL;
  TiXmlElement *pDNode = NULL;

  GetNodePointerByName(pRootEle,conditionNode,pNode);

  if(NULL != pNode)
  {
    if(strcmp(pNode->GetText(),str) == 0)
    {
      GetNodePointerByName(pRootEle,strNodeName,pDNode);
      if(NULL != pDNode)
      {
	// 首先清除所有文本
	pDNode->Clear();
	// 然后插入文本，保存文件
	TiXmlText *pValue = new TiXmlText(strText);
	pNode->LinkEndChild(pValue);

      }
    }

    pDoc->SaveFile(XmlFile);
    return true;
  }
  else
    return false;
}

int
updateXmlFile(const char * XmlFilePath,const char * rootElement,
    const char *conditionNode,const char *str,const char * strNodeName,
    const char * strText)
{
  TiXmlDocument *mydoc = new TiXmlDocument(XmlFilePath);
  bool loadOk = mydoc->LoadFile();
  if(!loadOk)
  {
    cout << "could not load the test file.Error:" << mydoc->ErrorDesc() << endl;
    exit(1);
  }

  TiXmlElement *RootElement = mydoc->RootElement(); //根元素
  TiXmlElement *pEle = NULL;

  GetNodePointerByName(RootElement,rootElement,pEle); //找到值为SearchModes的结点

  bool isfind = false;
  //遍历该结点
  for(TiXmlElement *SearchModeElement = pEle->FirstChildElement();SearchModeElement;SearchModeElement
      = SearchModeElement->NextSiblingElement())
  {
    cout << SearchModeElement->Value() << " "; //disk
    TiXmlAttribute *pAttr = SearchModeElement->FirstAttribute();

    cout << endl;
    {

      for(TiXmlElement *RegExElement = SearchModeElement->FirstChildElement();RegExElement;RegExElement
	  = RegExElement->NextSiblingElement())//输出子元素的值
      {



	if((strcmp(RegExElement->Value(),conditionNode) == 0) && (strcmp(
	    RegExElement->FirstChild()->Value(),str) == 0))
	{
	  isfind = true;
	}
	if((strcmp(RegExElement->Value(),strNodeName) == 0) && isfind)
	{
	  // 首先清除所有文本
	  RegExElement->Clear();
	  // 然后插入文本，保存文件
	  TiXmlText *pValue = new TiXmlText(strText);
	  RegExElement->LinkEndChild(pValue);
	  break;
	}

	if(RegExElement->FirstChild() == NULL)
	{
	  continue;
	}
	cout << RegExElement->Value() << endl;
	cout << RegExElement->FirstChild()->Value() << endl; // SN,WWN....

      }

    }
    if(isfind)
    {
      break;
    }
  }
  mydoc->SaveFile(XmlFilePath);
  delete mydoc;
  return 1;
}

#if 1
int
main(int argc,char *argv[])
{
  //readXmlFile("XmlConfig.xml","Disks");
  //writeXmlFile();
  //ModifyNode_Text("CpuStatusHba1.xml","PortNum","2" ,"ReadPCBSN","23");
  updateXmlFile("CpuStatusHba1.xml","Disks","PortNum","2","WriteOBBandinittables","23");

}
#endif
