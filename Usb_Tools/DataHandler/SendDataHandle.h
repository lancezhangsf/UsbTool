/*
 * SendDataHandle.h
 *
 *  Created on: Jul 27, 2015
 *      Author: root
 */

#ifndef SENDDATAHANDLE_H_
#define SENDDATAHANDLE_H_

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

namespace DATA
{
#define MAX_SEND_LEN 1024
  struct SendDataStruct
  {
      char head;
      int content_len;
      char cmd;
      char *content;
      char checkNum;
      char tail;

  } ;

  class SendData
  {
    public:
      unsigned char *data;
      SendDataStruct *sds;
    private:

    public:
      SendData()
      {
	this->data = new unsigned char[MAX_SEND_LEN];
	bzero(this->data,MAX_SEND_LEN);

      }

      SendData(SendDataStruct *sds)
      {
	this->data = new unsigned char[MAX_SEND_LEN];
	bzero(this->data,MAX_SEND_LEN);
	this->sds = sds;
	memcpy(data,sds,sizeof(SendDataStruct));

      }

      ~SendData()
      {
	delete[] this->data;

      }

    private:

  };

}

#endif /* SENDDATAHANDLE_H_ */
