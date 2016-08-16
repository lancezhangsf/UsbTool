/*
 * Xmodem.h
 *
 *  Created on: Jul 28, 2015
 *      Author: root
 */

#ifndef XMODEM_H_
#define XMODEM_H_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "../DiskInfo/DiskInfo.h"
#include "../Main.h"
using namespace std;
using namespace DISK;

#define UInt16  unsigned short
#define UInt32  unsigned int
#define  Int32    int
#define  byte   unsigned char

#define MAX_BUFFER_LENGTH 128
namespace PROTOCOL

{
  //完成XModel文件传输功能
  enum XmodemState
  {
    XM_IDLE, XM_WAIT_ACK, XM_SACK, XM_SNCK, XM_TIMEOUT

  };

  class CRC32
  {
    public:
      const UInt32 CRC32_POLYNOMIAL;//= 0x04C11DB7; // Use reversed one in this implementation, CRC32_REVERSED_POLYNOMIAL
      const UInt32 CRC32_REVERSED_POLYNOMIAL;// = 0xEDB88320;
      const UInt32 CRC32_INITIAL_REMAINDER;//= 0xFFFFFFFF;
      const UInt32 CRC32_FINALIZE_REMAINDER;// = 0xFFFFFFFF;
      const UInt32 CRC32_TABLE_SIZE;// = 256;
      UInt32 *crcTable;// = new UInt32[CRC32_TABLE_SIZE];

    public:

      CRC32() :
	CRC32_POLYNOMIAL(0x04C11DB7), CRC32_REVERSED_POLYNOMIAL(0xEDB88320),
	    CRC32_INITIAL_REMAINDER(0xFFFFFFFF), CRC32_FINALIZE_REMAINDER(
	        0xFFFFFFFF), CRC32_TABLE_SIZE(256)
      {
	crcTable = new UInt32[CRC32_TABLE_SIZE];
      }

      ~CRC32()
      {
	delete[] crcTable;
      }
      void
      Crc32Init()
      {
	UInt32 remainder;
	UInt32 dividend;
	int bit;

	for(dividend = 0U;dividend < CRC32_TABLE_SIZE;++dividend)
	{
	  remainder = dividend;
	  for(bit = 8;bit > 0;--bit)
	  {
	    if((remainder & 1U) != 0)
	    {
	      remainder = (remainder >> 1) ^ CRC32_REVERSED_POLYNOMIAL;
	    }
	    else
	    {
	      remainder >>= 1;
	    }
	  }

	  crcTable[dividend] = remainder;
	}

      }

      UInt32
      Crc32(byte pMessage[],int length)
      {
	UInt32 remainder = CRC32_INITIAL_REMAINDER;

	byte index;

	// Divide the message by the CRC32_POLYNOMIAL, a byte at a time.
	for(int i = 0;i < length;i++)
	{
	  // TODO: Check the assembly code, confirm the truncation operation.
	  index = (byte)(pMessage[i] ^ remainder); // There is a implicit truncation.
	  remainder = crcTable[index] ^ (remainder >> 8); // Same as above.
	}

	return remainder ^ CRC32_FINALIZE_REMAINDER;
      }

  };

  class CRC16
  {

    public:
      //Used for crc16
      const int CCITT_INITIAL_REMAINDER; //= 0;
      const int CCITT_TABLE_SIZE;//= 256;
      const UInt32 CCITT_POLYNOMIAL;// = 0x1021;
      UInt16 *crcTable;// = new UInt16[CCITT_TABLE_SIZE];
    public:
      CRC16() :
	CCITT_INITIAL_REMAINDER(0), CCITT_TABLE_SIZE(256), CCITT_POLYNOMIAL(
	    0x1021)
      {
	crcTable = new UInt16[CCITT_TABLE_SIZE];
	Crc16CcittInit();
      }

      ~CRC16()
      {
	delete[] crcTable;
      }

      UInt16
      Crc16Ccitt(byte pMessage[],int length)
      {
	UInt16 remainder = CCITT_INITIAL_REMAINDER;
	byte index;
	// Divide the message by the CCITT_POLYNOMIAL, a byte at a time.
	for(int i = 0;i < length;i++)
	{
	  index = (byte)(pMessage[i] ^ (byte)(remainder >> 8));
	  remainder = (UInt16)(crcTable[index] ^ (UInt16)(remainder << 8));
	}

	return remainder;
      }

    private:
      void
      Crc16CcittInit()
      {
	UInt16 remainder;
	UInt32 dividend;
	Int32 bit;

	// Compute the remainder of each possible dividend.
	for(dividend = 0U;dividend < (UInt32)CCITT_TABLE_SIZE;++dividend)
	{
	  // Start with the dividend followed by zeros.
	  remainder = (UInt16)(dividend << 8); // width - 8

	  // Perform modulo-2 division, a bit at a time.
	  for(bit = 8;bit > 0;--bit)
	  {
	    // Try to divide the current data bit.
	    if((remainder & (1 << 15)) != 0)
	    {
	      remainder = (UInt16)(((UInt32)remainder << 1) ^ CCITT_POLYNOMIAL);
	    }
	    else
	    {
	      remainder = (UInt16)(remainder << 1);
	    }
	  }

	  // Store the result into the table.
	  crcTable[dividend] = remainder;
	}

      }
  };

  /// <summary>
  /// 使用方法：1.实例化时自定传输串口通道
  ///           2.初始化传输文件路径
  ///           3.将串口接收到的字符串投递到该模块的状态机中
  ///           4.开始发送文件线程
  ///           5.模块文件发送完毕后，上报完成事件，并投递结果信息。上一层在事件处理函数中执行相关后处理操作，并关闭传输线程
  ///           6.结束.
  /// </summary>
  class XmodemOp
  {
    public:
      //XMODEM KED Macro
      const int XM_SOH;//= 0x01;
      const int XM_EOT;// = 0x04;
      const int XM_CAN; //= 0x18;
      const int XM_ACK;// = 0x06;
      const int XM_NCK; //= 0x15;

      Serialcom *sc; // 上一层获取：传输通道
      XmodemState xmState;//= XmodemState.XM_IDLE; //内部状态机控制
      byte xmFrameNum; //= 1;
      //sendData长度应为128
      byte buffFrame[150];
      CRC16 *crc16;

    public:
      string sendFilePath; //外部设置，指示发送文件路径


      XmodemOp(Serialcom *sc) :
	XM_SOH(0x01), XM_EOT(0x04), XM_CAN(0x18), XM_ACK(0x06), XM_NCK(0x15)
      {
	this->sc = sc;
	crc16 = new CRC16();
	xmFrameNum = 1;
	xmState = XM_IDLE;
	bzero(buffFrame,150);
      }
#if 0
      XmodemOp(Serialcom *sc,DiskInfo *diskInfo):XM_SOH(0x01), XM_EOT(0x04), XM_CAN(0x18), XM_ACK(0x06), XM_NCK(0x15)
      {
	this->diskInfo = diskInfo;
	this->sc = sc;
	xmFrameNum = 1;
	crc16 = new CRC16();
	xmState = XM_IDLE;
	bzero(buffFrame,150);
      }
#endif
    public:
      void
      XMRevMachine(byte revByte)
      {
	if(xmState == XM_WAIT_ACK)
	{
	  if(revByte == XM_ACK) //ACK
	  {
	    xmState = XM_SACK;
	  }
	  else if(revByte == XM_NCK) //NCK
	  {
	    xmState = XM_SNCK;
	  }
	}
      }

      //该函数处理后，xmodem只能为timeout或者ACK状态，返回发送次数，大于1存在重发
      int
      XMSendCrcFrame(byte *sendData,int length)
      {
	int sendTimes = 0;
	byte index = 0;
	if((length != 128) || (!sc->isSerialOpen()))
	{
	  return sendTimes;
	}
	//组装帧
	buffFrame[index++] = 0x01; //帧头
	buffFrame[index++] = xmFrameNum;
	buffFrame[index++] = (byte)(~xmFrameNum);
	for(int i = 0;i < length;i++)
	{
	  buffFrame[index++] = sendData[i];
	}

	//计算校验
	UInt16 crcValue = crc16->Crc16Ccitt(sendData,length);

	buffFrame[index++] = (byte)(crcValue >> 8);
	buffFrame[index++] = (byte)(crcValue);
	byte eventBytes[256] =
	{ 0 };
	byte tmp = 0;
	while(true)
	{
	  this->sc->SerialcomWrite((char *)buffFrame,index); //SPortWrite(this,sp,buffFrame,0,index);
	  sendTimes++;
	  xmState = XM_WAIT_ACK;
	  while(xmState == XM_WAIT_ACK)
	  {

	    if(this->sc->CheckSerialcomData() > 0)
	    {
	      cout << "serial have data" << endl;
	      this->sc->SerialcomRead((char *)eventBytes,256);
	      tmp = eventBytes[0];
	      printf("%x ",tmp);
	      XMRevMachine(tmp);
	    }
	    else
	    {
	      break; //jump this while , resend;
	    }

	    // 若串口没有收到回应，应该重发还是.....


#if 0
	    Thread.Sleep(5); //尽量减少CUP占用
	    //1.由XMRevMachine修改状态，以跳出循环
	    if((Environment.TickCount - tmrPoint1) > 500) //超时了

	    {
	      xmState = XmodemState.XM_TIMEOUT;
	      break;
	    }
#endif
	  }
	  if(xmState == XM_SACK) //收到应答，发送下一帧
	  {
	    xmFrameNum++;
	    break;
	  }
	  else if(xmState == XM_TIMEOUT)
	  {
	    break;
	  }
	  else if(sendTimes > 10) //连续重发超过10次,也可认为是超时
	  {
	    xmState = XM_TIMEOUT;
	    break;
	  }

	}
	return sendTimes;
      }

      bool
      XMSendFile(string filePath)
      {
	if(xmState != XM_IDLE)
	{
	  //  Log.WriteLog(this,diskInfo,"XModem只能在Idle状态下发起文件传输");
	  return false;
	}

	FILE *infile;
	infile = fopen(filePath.c_str(),"rb");
	unsigned char dataPiece[MAX_BUFFER_LENGTH] =
	{ 0 };
	if(infile == NULL)
	{
	  printf("%s, %s","./ft.bin","not exist/n");
	  exit(1);
	}
	int rc;
	while((rc = fread(
	    dataPiece,sizeof(unsigned char),MAX_BUFFER_LENGTH,infile)) != 0)
	{
	  if(rc < 128)
	  {
	    for(int i = rc;i < 128;i++)
	    {
	      dataPiece[i] = 0x1A;
	    }
	  }
	  cout<<"Read Bin file Success"<<endl;
	  int sendTimes = XMSendCrcFrame(dataPiece,MAX_BUFFER_LENGTH);
	  if(xmState == XM_SACK) //发送成功
	  {
	    // FrameStatistics.TxFrameCounter[diskInfo.slotNum - 1]++;
	    if(sendTimes > 1) //存在重发，记录日志
	    {
	      cout << "resend:" << sendTimes << endl;
	    }
	  }
	  else if(xmState == XM_TIMEOUT) //超时:长时间无应答，或者连续重发次数超过10次
	  {
	    xmState = XM_IDLE; //停止传输
	    if(sendTimes > 10)
	    {
	      cout << "超时，原因为：帧连续重发次数超过10次" << endl;

	    }
	    else
	    {
	      cout << "超时，原因为：帧无ACK/NCK响应" << endl;
	    }
	    return false;
	  }

	  //此时数据包发送完毕，发送EOT
	  XModemEndNormal();
	}
	return true;
      }

      bool
      XModemEndNormal()
      {
	bool state;
	byte tmp = 0;
	byte eot[1] =
	{ XM_EOT };

	this->sc->SerialcomWrite((char *)eot,1);
	//SerialOp.SPortWrite(this,sp,eot,0,1);
	xmState = XM_WAIT_ACK;
	//int tmrPoint1 = Environment.TickCount;
	byte eventBytes[256] =
	{ 0 };
	while(xmState == XM_WAIT_ACK)
	{
	  if(this->sc->CheckSerialcomData() > 0)
	  {
	    cout << "serial have data" << endl;
	    this->sc->SerialcomRead((char *)eventBytes,256);
	    tmp = eventBytes[0];
	    XMRevMachine(tmp);
	  }
	  else
	  {
	    break; //jump this while , resend;
	  }

	}

	if(xmState == XM_SACK)
	{
	  state = true;
	}
	else
	{
	  state = false;
	}
	xmState = XM_IDLE;
	return state;
      }
  };
}

#endif /* XMODEM_H_ */
