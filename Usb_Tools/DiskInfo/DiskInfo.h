/*
 * DiskInfo.h
 *
 *  Created on: Jul 22, 2015
 *      Author: root
 */

#ifndef DISKINFO_H_
#define DISKINFO_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include "./BackBoard.h"
#include "../SerialPort/SerialPort.h"
#include "./DiskOp.h"


namespace DISK
{
  class DiskInfo
  {
    private:
      bool isDiskInsert; //表示该槽位是有否接有盘片.界面只可读。backwacher将周期更新该值
    public:
      BackBoardInfo  *bbi;
      Serialcom *sc;
      int slotNum; //记录盘片所在的插槽号?可删除
      DiskOp *diskop;


    public:
      DiskInfo(Serialcom *sc)
      {
	this->slotNum  = -1;
	this->sc = sc;
	this->diskop = new DiskOp(sc);
      }




  };
}
#endif /* DISKINFO_H_ */
