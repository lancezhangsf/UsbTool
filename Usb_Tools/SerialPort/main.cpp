/*
 * main.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: lance
 */


#include "SerialPort.h"



int main(){

  Serialcom *sp = new Serialcom();
  sp->SerialcomOpen((char *)"/dev/ttyS3");

  return 1;


}


