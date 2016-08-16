/*
 * main.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */

#include "./SerialFind.h"

char *serialName[MAX_COM];
int
main(int argc,char *argv[])
{

  memset(serialName,0,64);
  int count = get_all_Serial(serialName);

  release_all_Serial(serialName,count);

  return 0;
}
