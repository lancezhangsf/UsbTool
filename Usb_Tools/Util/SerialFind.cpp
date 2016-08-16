/*
 * SerialFind.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <dirent.h>

#include "./SerialFind.h"
#define DEV_PATH "/dev/"

int
get_all_Serial(char **serialAllName)
{

  DIR *dir;

  int count = 0;
  dir = opendir(DEV_PATH);
  if(dir == NULL)
  {
    cout << "dir error" << endl;
  }

  struct dirent *rd;

  while((rd = readdir(dir)) != NULL)
  {

    if(!strncmp(rd->d_name,"ttyS",4))
    {
      serialAllName[count] = (char *)malloc(sizeof(char) * PORT_NAME_LENGTH);

      strcpy(serialAllName[count],DEV_PATH);
      strcat(serialAllName[count],rd->d_name);
      //  strcpy(serialAllName[count],rd->d_name);

      cout << serialAllName[count] << endl;

      ++count;
    }
  }

  return count;

}

void
release_Serial(char **serialAllName,char *serialName,int count)
{

  if((serialName == NULL) || (serialAllName == NULL))
  {
    cout << "serial Name is NULL" << endl;
  }

  for(int i = 0;i < count;i++)
  {

    if(!(strcmp(serialAllName[i],serialName)))
    {
      cout << "release:" << serialName << endl;
      free(serialAllName[i]);
    }
  }

}

void
release_all_Serial(char **serialAllName,int count)
{

  if((serialAllName == NULL) || (count == 0))
  {
    cout << "serial Name is NULL" << endl;
  }

  for(int i = 0;i < count;i++)
  {
      cout << "release:" << serialAllName[i] << endl;
      free(serialAllName[i]);
  }
}
#if 0
int main()
{

  int count = get_all_Serial();

  cout<< count <<endl;
  release_all_Serial(serialName,0);

  return 0;
}
#endif
