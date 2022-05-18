#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define MY_FIFO "./fifo"
#define PATH_NAME "./"
#define PRO_ID 6666
#define SIZE 4097
