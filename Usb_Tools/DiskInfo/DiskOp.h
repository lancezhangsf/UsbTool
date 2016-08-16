/*
 * DiskOp.h
 *
 *  Created on: Jul 28, 2015
 *      Author: root
 */

#ifndef DISKOP_H_
#define DISKOP_H_

#include "./DiskInfo.h"
#include "../Protocol/Xmodem.h"

using namespace PROTOCOL;
namespace DISK
{
  enum DiskState
  {
    DISK_PORT_NONE, //表示该工位不绑定串口
    DISK_PORT_OPEND, //绑定有串口且打开成功
    DISK_PORT_FALSE, //绑定有串口，但打开失败
    DISK_C_RESIVED, //串口打开成功后，发现有盘片接入
    DISK_DOWN_FT_OK,
    DISK_DOWN_FT_FALSE,
    DISK_CONNECT_OK, //FT下载成功，且与FT的握手连接成功
    DISK_CONNECT_FALSE,
  //FT下载成功,但建立握手失败
  };
  /// <summary>
  /// 功能：主要用于监控盘片的状态。
  /// 使用方法：1.用串口名/nones实例化该类
  ///           2.绑定diskOp的状态变化处理函数
  ///
  ///           3.打开绑定的串口
  /// 其他：提供开始测试的函数接口。事先需指定itemsop测试结果更新事件处理函数。
  /// </summary>
  class DiskOp
  {
    private:
      char *dataBuff; //存放串口接收数据
      //     FrameReceiver frameReceiver = null;//用于FT帧发送
      //     FrameSender frameSender = null; //用于帧接收

    public:
      XmodemOp *xmOperator; //用于下载FT文件
     // DiskInfo *diskinfo;
      //   FtLinker ftLinker = null;//用于主机在未开展产品测试时，与FT固件建立握手

      //    FtItemsOp ftTtemsOperator = null; //用于产品测试命令的具体操作和执行
      //	DiskState diskState; //界面可访问


    public:
      DiskOp( Serialcom *sc) //上层调用应保证，只传入串口设备名，或者none，不可传入其他字符串或者null
      {
	xmOperator = new XmodemOp(sc);
	//	OnEvent += new EventHandler<EventArgs> (ReceiveAsynProcess);

      }
      ~DiskOp()
      {
	delete xmOperator;
      }
#if 0
      //调用该函数的前提是：盘片状态为连接成功
      public void StartFTTest()
      {
	//todo:高耦合代码，待修改
	ftTtemsOperator.ExecuteFT(ftLinker, null);//开始执行测试过程。
      }

      private:
      void
      OnDiskStateChange(EventArgs e)
      {
	if(DiskStateChange != null)
	{
	  if(DiskStateChange.Target is System.ComponentModel.ISynchronizeInvoke)
	  {
	    System.ComponentModel.ISynchronizeInvoke asynInvoke = DiskStateChange.Target as System.ComponentModel.ISynchronizeInvoke;
	    if(asynInvoke.InvokeRequired)
	    {
	      object[] args = new object[2]
	      { this, e};
	      asynInvoke.Invoke(DiskStateChange,args);
	    }
	    else
	    {
	      DiskStateChange(this,e);
	    }
	  }
	}
      }

      public:
      void
      OpenAttachedSerialPort() //返回打开结果的状态,只有打开成功/连接成功，sp才不为空

      {
	if(diskState == DiskState.DISK_PORT_NONE) //默认情况,串口未打

	{

	  if(!diskInfo.portName.Equals("none")) //绑定有串口

	  {
	    if(sp != null)
	    {
	      if(sp.IsOpen) //释放打开的串口资源

	      {
		sp.Close();
		sp = null;
	      }
	    }
#if 0
	    //尝试打开串口
	    try
	    {
	      sp = SerialOp.OpenSPortByName(diskInfo.portName);
	      //打开成功
	      if (sp != null)
	      {
		SetDiskState(DiskState.DISK_PORT_OPEND);
	      }
	      else
	      {
		SetDiskState(DiskState.DISK_PORT_FALSE);
	      }
	      //todo:check 盘片是否连接(封装成单个函数)，以更新盘片状态图标,由接着跟改状态
	    }
	    catch
	    {
	      sp = null;
	      diskInfo.portName = "none";
	      SetDiskState(DiskState.DISK_PORT_FALSE);
	      Log.WriteLog(this,diskInfo,"串口打开失败");
	    }
#endif
	  }
	  else//没有串口绑定，返回默认态

	  {
	    SetDiskState(DiskState.DISK_PORT_NONE);
	  }
	}

	if(diskState == DiskState.DISK_PORT_OPEND) //串口打开情况下，绑定接收时间，添加帧控制层

	{
	  //下述代码中，不会出现多事件绑定的情况
	  sp.DataReceived += new SerialDataReceivedEventHandler(
	      UartRxEventHandler);
	  //只有在此情况下，帧实例与上层实例的串口实例保持一致
	  frameReceiver = new FrameReceiver(sp,diskInfo);
	  frameSender = new FrameSender(sp,diskInfo);
	  xmOperator = new XmodemOp(sp,diskInfo);
	  ftLinker = new FtLinker(frameReceiver,frameSender);
	  ftTtemsOperator = new FtItemsOp(frameReceiver,frameSender,diskInfo);//该测试通过对diskInfo的内容修改来间接上报事件，降低耦合
	  xmOperator.sendFilePath = GlobalSetting.GetInstance().ftPath;
	  xmOperator.XmTransportDoneHandler += new EventHandler(XmTransDone);
	  ftLinker.LinkCheckHandler += new EventHandler(LinkerUpdate);
	}
      }

      private:
      void
      LinkerUpdate(object sender,EventArgs e)
      {
	if(e is MessageArgs)
	{
	  if(((MessageArgs)e).isSendSuccessd) //

	  {
	    SetDiskState(DiskState.DISK_CONNECT_OK);
	  }
	  else
	  {
	    Log.WriteLog(this,diskInfo,"与FT 握手连接失败，检测串口是否断开、电源是否掉电");
	    SetDiskState(DiskState.DISK_CONNECT_FALSE);
	    // 通过状态变化事件，交上层处理
	  }
	}
      }

      //传输完毕时有效
      public:
      void
      XmTransDone(object sender,EventArgs e)
      {
	//	if(e is MessageArgs)

	{
	  if(((MessageArgs)e).isSendSuccessd) //文件发送成功，FT已经运行起来

	  {
	    Thread.Sleep(500); //确保FT固件已运行
	    SetDiskState(DiskState.DISK_DOWN_FT_OK);

	    ftLinker.StartLinkThread(); //启动主机与FT固件握手线程
	  }
	  else
	  {
	    Log.WriteLog(this,diskInfo,"XModem 下载FT失败");
	    SetDiskState(DiskState.DISK_DOWN_FT_FALSE);
	  }

	  // MessageBox.Show(Thread.CurrentThread.IsAlive.ToString());
	  // Thread temp = Thread.CurrentThread;

	  xmOperator.EndSendThread();

	}
      }

      private:
      int head = 0;
      int tail = 0;
      //缓存小buff，防止串口丢数据，事件处理代码应尽快退出
      byte eventBytes[] = new byte[256];
      void
      UartRxEventHandler(object sender,SerialDataReceivedEventArgs e)
      {

	int count = 0;
#if 0
	try
	{
	  count = ((SerialPort)sender).Read(eventBytes, 0, 256);
	}
	catch
	{
	}
#endif
	for(int i = 0;i < count;i++)
	{
	  dataBuff[head] = eventBytes[i];
	  if(head == BUFF_SIZE - 1)
	  {
	    head = 0;
	  }
	  else
	  {
	    head++;
	  }
	}

	//启动状态机异步线程（运转状态机，修改对应盘的帧状态，封装成结果帧）
	if(ir == null || ir.IsCompleted)
	{

	  //	  Delegate[] delegAry = OnEvent.GetInvocationList();
	  //Int32 idx=Convert.ToInt32( diskName.Last())-49;
	  //ir = ((EventHandler<EventArgs>)delegAry[idx]).BeginInvoke(null, EventArgs.Empty, null, null);
	  ir = ((EventHandler<EventArgs> )delegAry[0]).BeginInvoke(
	      null,EventArgs.Empty,null,null);

	}

      }

      //接收异步线程

      private:
      void
      ReceiveAsynProcess(object sender,EventArgs e)
      {
	//取数据直到buff空
	byte temp;
	while(head != tail) //表示有数据

	{
	  //状态机/修改帧状态、内容
	  temp = dataBuff[tail];
	  if((diskState == DiskState.DISK_CONNECT_OK) || (diskState
		  == DiskState.DISK_DOWN_FT_OK)) //都需要与FT固件通信，只是通信的内容不同

	  {
	    if(diskState == DiskState.DISK_CONNECT_OK)
	    {
	      frameReceiver.StateMachine(temp); //只有正常连接状态下才可以接收帧
	    }
	    frameSender.FrameSendMachine(temp); ////由ftlinker，itemtester监管
	  }

	  else if(diskState == DiskState.DISK_C_RESIVED) //文件传输过程

	  {
	    //投递字符到XModel控制层
	    xmOperator.XMRevMachine(temp);
	  }
	  else if(diskState == DiskState.DISK_PORT_OPEND) //

	  {
	    //表示盘片接入，可能收到C/NCK字符
	    if(temp == 'C')
	    {
	      //todo:check is sn asigned?
	      if(!FtMainFrm.isHaveUnusedReady) // 当用户未先扫码即开始插盘事，报错误提示

	      {
		this.ownner.Invoke(new MethodInvoker(delegate
			{
			  MessageBox.Show(this.ownner, "请先扫描盘片SN，再接入盘片到装备板", "接入警告S" + diskInfo.slotNum, MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}));

		tail = head; //使中间接收到的数据无效
		break;
	      }
	      //若存在 可用SN 则需要确认是否已被其他判断锁定。
	      if(ownner.IsSnLockedByDisks()) // 当前SN已被其他盘片锁定

	      {

		this.ownner.Invoke(new MethodInvoker(delegate
			{
			  MessageBox.Show(this.ownner, "当前SN已被上次接入的盘片锁定，请等待其初始化完毕", "接入警告S" + diskInfo.slotNum, MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}));

		tail = head; //使中间接收到的数据无效
		break;
	      }

	      //未锁定，则自己锁定该SN
	      diskInfo.isSnLocked = true;

	      SetDiskState(DiskState.DISK_C_RESIVED); //表示接收到校验传输通知，即与ROM握手成功
	      Thread.Sleep(100);
	      xmOperator.StartSendThread(); //启动文件传输线程
	      //启动xmodem传输线程
	    }

	  }

	  //delete data
	  if(tail == BUFF_SIZE - 1)
	  {
	    tail = 0;
	  }
	  else
	  {
	    tail++;
	  }

	}

      }
#endif
  };
}

#endif /* DISKOP_H_ */
