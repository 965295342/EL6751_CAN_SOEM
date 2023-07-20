#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#define pi 3.141592
#define EC_TIMEOUTMON 500
//#define AXIS_UPDATE_CYCLE 2
#define true TRUE
#define false FALSE

void interactWith_etherCAT(void *args);
typedef struct EtherCATargs
{
    int waitTime; // 程序运行的gap时间
    int endThread;
    char *ifname;
} EtherCATargs;

static uint64 cycleIndex;
char IOmap[4096];
OSAL_THREAD_HANDLE thread1, thread2;
OSAL_THREAD_HANDLE thread_fresh; // 刷新PDO数据进程
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
//uint8_t read_data[128][256]; // 从csv中read到的data
static uint8 currentgroup = 0;
double waitTime; // 等待时间
time_t lastCycleTime, nowCycleTime, gapTIme, beginCycleTime;

uint8_t len_of_eachCsv[128];       // 每个csv文件读取的行数
uint8 msgSendDataLength[128][256]; // 各个TxPDO发送data的长度 8Byte,6Byte等，0Byte表示这个msg没有发送data
uint8 msgReadDataLength[128][256]; // 各个RxPDO发送data的长度 8Byte,6Byte等，0Byte表示这个msg没有接收data
uint8 sendMsgNum[256];             // can发送msg数量
uint8 readMsgNum[256];
uint8 reg0x80_Num[256];              // 有几个0x80寄存器
uint16 totalSendDataByteLength[256]; // 所有RxPDO数据区长度和
uint16 totalReadDataByteLength[256]; // 所有TxPDO数据区长度和
uint16 iCurStateRead;
uint64_t msgSendCOBID[128][256];//各个TxPDO的COBID
uint64_t msgReadCOBID[128][256];
uint32_t msgSendOffset[128][256];
uint32_t msgReadOffset[128][256];

u_int8_t udp_flag;
int udp_socket_fd;
//struct sockaddr_in dest_addr;
float _udp_msg[128]; // 向matlab发送的single数组

uint8_t init_udp(int port);
void sendUdpMsg();
uint8_t readData[8][8];
// test

float msg[256];
uint8_t send1[4096] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
uint8_t send2[128 ] = {21,22,23,24,25,26,27,28};
uint8_t sendZero[2048] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// test

static int reg_address[] = {
    0x8000, 0x8006, 0x8008, 0x8003};

static uint8_t reg_0xF800[] = { // twincat的F800似乎读取会配置出错,建议手动修改
    0x0A, 0x00,
    0x7F, 0x02,
    0x80, 0x00,
    0x1E, 0x00,
    0x01, 0x00,
    0x11, 0x00,
    0x64, 0x00};
static uint8_t reg0x8008[] = {
    0x01, 0x00, 0x0B, 0x05, 0x00, 0x00, 0xFF, 0x08, 0x0, 0x0,0x14};

static uint8_t sub_addr[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};

static uint8 reg0x1c12[] = {
    0x02, 00, 00, 0x16, 0x01, 0x16};