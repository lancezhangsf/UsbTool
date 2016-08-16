/*
 * SerialFind.h
 *
 *  Created on: Jul 27, 2015
 *      Author: lance
 */

#ifndef SERIALFIND_H_
#define SERIALFIND_H_
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <strings.h>
#include <string.h>

using namespace std;

#define MAX_COM 64
#define PORT_NAME_LENGTH 13




int get_all_Serial(char **serialAllName);
void release_Serial(char **serialAllName,char *serialName,int count);

void release_all_Serial(char **serialAllName,int count);

#endif /* SERIALFIND_H_ */
