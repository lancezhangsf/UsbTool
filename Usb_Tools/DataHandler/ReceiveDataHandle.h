/*
 * ReceiveDataHandle.h
 *
 *  Created on: Jul 27, 2015
 *      Author: root
 */

#ifndef RECEIVEDATAHANDLE_H_
#define RECEIVEDATAHANDLE_H_

#include <iostream>
#include <cstring>
#include <cstdlib>


using namespace std;


namespace DATA{
#define MAX_RECEIVER_LEN 1024

  class ReceiveData{
    public:
      unsigned char *data;

    private:

    public:
      ReceiveData(){
	this->data = new unsigned char[MAX_RECEIVER_LEN];
	bzero(this->data,MAX_RECEIVER_LEN);

      }

      ~ReceiveData(){
	delete[]  this->data;

      }

    private:


  };


}





#endif /* RECEIVEDATAHANDLE_H_ */
