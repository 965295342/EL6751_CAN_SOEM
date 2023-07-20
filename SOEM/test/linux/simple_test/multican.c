#include <stddef.h>
#include <stdio.h>
// #include "rtwtypes.h"//todo
// #include "timer_interrupt_program.h"//todo
// #include "mVeh.h"//todo
#include "sendUdp.h"

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

// 引用ethercat
#include <unistd.h>
#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

#define EC_TIMEOUTMON 500

#define true TRUE
#define false FALSE

uint8_t data[8];
float dataTest;
// test
uint8_t data1[16] = {0, 0, 0x06, 0x39, 0, 0, 0x10, 0};
uint8_t data2[16] = {0x10, 0};
uint8_t data3[16];
uint8_t data4[16];
uint8_t data5[16];
uint8_t data6[16];
uint8_t data7[16] = {0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t data8[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t data9[16] ;
typedef struct EtherCATargs
{
    int waitTime; // 程序运行的gap时间
    int endThread;
    char *ifname;
} EtherCATargs;

struct testStruct
{
    float HWlog_uD_single_p1_DSTATE;
    float HWlog_uD_single_p2_DSTATE;
    float HWlog_uD_single_p3_DSTATE;
    float HWlog_uD_single_p4_DSTATE;
    float HWlog_uD_single_p5_DSTATE;
    float HWlog_uD_single_p6_DSTATE;
    float HWlog_uD_single_p7_DSTATE;
    float HWlog_uD_single_p8_DSTATE;
    float HWlog_uD_single_p9_DSTATE;
    float HWlog_uD_single_p10_DSTATE;
    float HWlog_uD_single_p11_DSTATE;
    float HWlog_uD_single_p12_DSTATE;
    float HWlog_uD_single_p13_DSTATE;
    float HWlog_uD_single_p14_DSTATE;
    float HWlog_uD_single_p15_DSTATE;
    float HWlog_uD_single_p16_DSTATE;
    float HWlog_uD_single_p17_DSTATE;
    float HWlog_uD_single_p18_DSTATE;
    float HWlog_uD_single_p19_DSTATE;
    float HWlog_uD_single_p20_DSTATE;
    float HWlog_uD_single_p21_DSTATE;
    float HWlog_uD_single_p22_DSTATE;
    float HWlog_uD_single_p23_DSTATE;
    float HWlog_uD_single_p24_DSTATE;
    float HWlog_uD_single_p25_DSTATE;
    float HWlog_uD_single_p26_DSTATE;
    float HWlog_uD_single_p27_DSTATE;
    float HWlog_uD_single_p28_DSTATE;
    float HWlog_uD_single_p29_DSTATE;
    float HWlog_uD_single_p30_DSTATE;
    float HWlog_uD_single_p31_DSTATE;
    float HWlog_uD_single_p32_DSTATE;

    uint8 HWlog_uD_uint8_p1_DSTATE;
    uint8 HWlog_uD_uint8_p2_DSTATE;
    uint8 HWlog_uD_uint8_p3_DSTATE;
    uint8 HWlog_uD_uint8_p4_DSTATE;
    uint8 HWlog_uD_uint8_p5_DSTATE;
    uint8 HWlog_uD_uint8_p6_DSTATE;
    uint8 HWlog_uD_uint8_p7_DSTATE;
    uint8 HWlog_uD_uint8_p8_DSTATE;
    uint8 HWlog_uD_uint8_p9_DSTATE;
    uint8 HWlog_uD_uint8_p10_DSTATE;
    uint8 HWlog_uD_uint8_p11_DSTATE;
    uint8 HWlog_uD_uint8_p12_DSTATE;
    uint8 HWlog_uD_uint8_p13_DSTATE;
    uint8 HWlog_uD_uint8_p14_DSTATE;
    uint8 HWlog_uD_uint8_p15_DSTATE;
    uint8 HWlog_uD_uint8_p16_DSTATE;
    uint8 HWlog_uD_uint8_p17_DSTATE;
    uint8 HWlog_uD_uint8_p18_DSTATE;
    uint8 HWlog_uD_uint8_p19_DSTATE;
    uint8 HWlog_uD_uint8_p20_DSTATE;
    uint8 HWlog_uD_uint8_p21_DSTATE;
    uint8 HWlog_uD_uint8_p22_DSTATE;
    uint8 HWlog_uD_uint8_p23_DSTATE;
    uint8 HWlog_uD_uint8_p24_DSTATE;
    uint8 HWlog_uD_uint8_p25_DSTATE;
    uint8 HWlog_uD_uint8_p26_DSTATE;
    uint8 HWlog_uD_uint8_p27_DSTATE;
    uint8 HWlog_uD_uint8_p28_DSTATE;
    uint8 HWlog_uD_uint8_p29_DSTATE;
    uint8 HWlog_uD_uint8_p30_DSTATE;
    uint8 HWlog_uD_uint8_p31_DSTATE;
    uint8 HWlog_uD_uint8_p32_DSTATE;
    uint8 HWcan1_uDout_msg1_DSTATE[8];
    uint8 HWcan1_uDout_msg2_DSTATE[8];
    uint8 HWcan1_uDin_msg1_DSTATE[8];
    uint8 HWcan1_uDin_msg2_DSTATE[8];
    uint8 HWcan1_uDin_msg3_DSTATE[8];
    uint8 HWcan2_uDin_msg1_DSTATE[8];
    uint8 HWcan3_uDout_msg1_DSTATE[8]
} mVeh_DW;
// test

// uint64 cycleIndex;
int expectedWKC;
volatile int wkc;
// double waitTime; // 等待时间
udp_struct udp_struct_float, udp_struct_uint8;
OSAL_THREAD_HANDLE thread1, thread2;

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

uint8_t len_of_eachCsv[128];       // 每个csv文件读取的行数
uint8 msgSendDataLength[128][256]; // 各个TxPDO发送data的长度 8Byte,6Byte等，0Byte表示这个msg没有发送data
uint8 msgReadDataLength[128][256]; // 各个RxPDO发送data的长度 8Byte,6Byte等，0Byte表示这个msg没有接收data
uint8 sendMsgNum[256];             // can发送msg数量
uint8 readMsgNum[256];
uint8 reg0x80_Num[256];              // 有几个0x80寄存器
uint16 totalSendDataByteLength[256]; // 所有RxPDO数据区长度和
uint16 totalReadDataByteLength[256]; // 所有TxPDO数据区长度和
uint16 iCurStateRead;
uint64_t msgSendCOBID[128][256]; // 各个TxPDO的COBID
uint32_t msgSendOffset[128][256];
uint32_t msgReadOffset[128][256];
uint64_t msgReadCOBID[128][256];
// function
int slave_setup(uint16_t slave);
__uint32_t get_csv(__uint8_t (*ptr)[256], __uint16_t slave);
int setting_register(uint16 slave, uint8_t (*reg_csv)[256]); // 读取twincat的配置.csv文件
int init_setting(char *ifname);
int use_COB_ID_find_k(uint16_t slave, uint64_t COB_ID);
boolean write_Into_pos(uint16_t slave, uint64_t COB_ID, uint8_t *value); // 往对应的COB_ID写value
boolean read_from_pos(uint16_t slave, uint64_t COB_ID, uint8_t *value);
int use_Read_COB_ID_find_k(uint16_t slave, uint64_t COB_ID);
void show_data();
void load_udp_msgToBuffer();
void SendUdpMsg();
void deal_data(void *args);
// void interactWith_etherCAT(void * argv);
//  for (i_0 = 0; i_0 < 8; i_0++) {
//    rtb_HW_uD_canOut_p1[i_0] = mVeh_DW.HW_uD_canOut_p1_DSTATE[i_0];
// }

float float_data_toBe_display[256];   // float data sent to Linux-simulini to display
uint8_t uint8_data_toBe_display[128]; // uint8 data sent to Linux-simulini to display

char IOmap[4096];

void interactWith_etherCAT(void *args)
{
    EtherCATargs *ptr = args;
    printf("Soem open EtherCAT Master test\n");
    init_udp(32785, &udp_struct_uint8);
    init_udp(32769, &udp_struct_float); // use this port to send data to linux-simulink display
                                        // must be the same as specified in linux-simulink

    osal_thread_create(&thread1, 128000, &SendUdpMsg, NULL);

    int ret = init_setting(ptr->ifname);
    switch (ret)
    {
    case 0:
        printf("bind socket to %s error", ptr->ifname);
        break;
    case 1:
        printf("no connection on %s\n", ptr->ifname);
        break;
    case 2:
        printf("connect and init successfull\n");
        break;
    default:
        printf("connect but init failed\n");
        break;
    }
    osal_usleep(10000); // sleep 程序不要这么快结束

    if (ret == 2)
    {
        // osal_thread_create(&thread2, 128000, &deal_data, (void *)&RunArgs);
        deal_data(args);
        // pthread_detach(thread2);
    }
    return;
}

void deal_data(void *args)
{
    EtherCATargs *ptr = args;
    printf("end of connect %d %d\n", ptr->endThread, ptr->waitTime);
    int indexCount = 0;
    while (ptr->endThread == 0)
    {
        indexCount++;
        osal_usleep(ptr->waitTime);
        // 发送PDO数据
        // write_Into_pos(2, 0x3C, send1);
        // write_Into_pos(2, 0x1fd, send2);
        // getMessage(2, send1);
        ec_send_processdata();
        wkc = ec_receive_processdata(EC_TIMEOUTRET); // 接受PDO数据，这里WKC原理需要了解
                                                     //  WKC 可以理解为检验值，如果各个从站做了正确的操作，此时wkc应该等于或大于期待的WKC

        if (wkc > 0)
        {

            //     // test
            //     memcpy(&mVeh_DW.HWcan1_uDout_msg1_DSTATE, &data1, sizeof(data1));
            //     memcpy(&mVeh_DW.HWcan1_uDout_msg2_DSTATE, &data2, sizeof(data2));
            data1[0] = (indexCount % 256);
            data1[1] = (indexCount % 16) << 4;
            // printf("test %d %d\n",data1[1],indexCount %16);
            memcpy(&mVeh_DW.HWcan1_uDout_msg1_DSTATE, &data1, sizeof(data1));
            if (indexCount % 5 == 0)
                memcpy(&mVeh_DW.HWcan3_uDout_msg1_DSTATE, &data7, sizeof(data7));
            if (indexCount % 5 == 3)
                memcpy(&mVeh_DW.HWcan3_uDout_msg1_DSTATE, &data8, sizeof(data8));
            // printf("index %d\n", indexCount);
            //      // test
            //  write_Into_pos(2, 0x5B, mVeh_DW.HWcan1_uDout_msg1_DSTATE);
            //  write_Into_pos(2, 0x2C3, mVeh_DW.HWcan1_uDout_msg2_DSTATE);
            //  read_from_pos(2, 0x70, mVeh_DW.HWcan1_uDin_msg1_DSTATE);
            //  read_from_pos(2, 0x71, mVeh_DW.HWcan1_uDin_msg2_DSTATE);
            //  read_from_pos(2, 0x80, mVeh_DW.HWcan1_uDin_msg3_DSTATE);
            //  read_from_pos(3, 0x4B, mVeh_DW.HWcan2_uDin_msg1_DSTATE);
            //  write_Into_pos(4, 0x151, mVeh_DW.HWcan3_uDout_msg1_DSTATE);
            //      // // test
            //      //mVeh_DW.HWlog_uD_uint8_p1_DSTATE = mVeh_DW.HWcan1_uDout_msg1_DSTATE[0];
            //     // mVeh_DW.HWlog_uD_uint8_p2_DSTATE = mVeh_DW.HWcan1_uDin_msg1_DSTATE[0];
            //      //mVeh_DW.HWlog_uD_uint8_p3_DSTATE = (mVeh_DW.HWcan2_uDin_msg1_DSTATE[2] & 0x80)>>7;
            //      // time_t nowTime = getMillisecondTime() - beginTime;
            //      // mVeh_DW.HWlog_uD_single_p32_DSTATE = nowTime * 1.0 / 1000;
            //      // mVeh_DW.HWlog_uD_single_p1_DSTATE = (float)sin(number_of_runs) * 64.0;
            //      // // test

            // //test
            if (indexCount % 5 == 0)
                write_Into_pos(2, 0x151, data1);
            read_from_pos(3,0x151,data9);
            printf("read:\n");
            for(int i =0;i<8;i++) {
                printf("%d ",data9[i]);
            }
            printf("\n");
            // if (indexCount % 5 == 3)
            //     write_Into_pos(2, 0x151, data8);
            // //test

            load_udp_msgToBuffer();
            show_data();
        }
    }
    close(udp_struct_float.udp_socket_fd);
    close(udp_struct_uint8.udp_socket_fd);
    ec_slave[0].state = EC_STATE_INIT;
    /* request INIT state for all slaves */
    ec_writestate(0);

    ec_close();
    return;
}

void load_udp_msgToBuffer()
{

    // float_data_toBe_display[0] = mVeh_DW.HWlog_uD_single_p1_DSTATE;
    float_data_toBe_display[0] = dataTest;
    float_data_toBe_display[1] = mVeh_DW.HWlog_uD_single_p2_DSTATE;
    float_data_toBe_display[2] = mVeh_DW.HWlog_uD_single_p3_DSTATE;
    float_data_toBe_display[3] = mVeh_DW.HWlog_uD_single_p4_DSTATE;
    float_data_toBe_display[4] = mVeh_DW.HWlog_uD_single_p5_DSTATE;
    float_data_toBe_display[5] = mVeh_DW.HWlog_uD_single_p6_DSTATE;
    float_data_toBe_display[6] = mVeh_DW.HWlog_uD_single_p7_DSTATE;
    float_data_toBe_display[7] = mVeh_DW.HWlog_uD_single_p8_DSTATE;
    float_data_toBe_display[8] = mVeh_DW.HWlog_uD_single_p9_DSTATE;
    float_data_toBe_display[9] = mVeh_DW.HWlog_uD_single_p10_DSTATE;
    float_data_toBe_display[10] = mVeh_DW.HWlog_uD_single_p11_DSTATE;
    float_data_toBe_display[11] = mVeh_DW.HWlog_uD_single_p12_DSTATE;
    float_data_toBe_display[12] = mVeh_DW.HWlog_uD_single_p13_DSTATE;
    float_data_toBe_display[13] = mVeh_DW.HWlog_uD_single_p14_DSTATE;
    float_data_toBe_display[14] = mVeh_DW.HWlog_uD_single_p15_DSTATE;
    float_data_toBe_display[15] = mVeh_DW.HWlog_uD_single_p16_DSTATE;
    float_data_toBe_display[16] = mVeh_DW.HWlog_uD_single_p17_DSTATE;
    float_data_toBe_display[17] = mVeh_DW.HWlog_uD_single_p18_DSTATE;
    float_data_toBe_display[18] = mVeh_DW.HWlog_uD_single_p19_DSTATE;
    float_data_toBe_display[19] = mVeh_DW.HWlog_uD_single_p20_DSTATE;
    float_data_toBe_display[20] = mVeh_DW.HWlog_uD_single_p21_DSTATE;
    float_data_toBe_display[21] = mVeh_DW.HWlog_uD_single_p22_DSTATE;
    float_data_toBe_display[22] = mVeh_DW.HWlog_uD_single_p23_DSTATE;
    float_data_toBe_display[23] = mVeh_DW.HWlog_uD_single_p24_DSTATE;
    float_data_toBe_display[24] = mVeh_DW.HWlog_uD_single_p25_DSTATE;
    float_data_toBe_display[25] = mVeh_DW.HWlog_uD_single_p26_DSTATE;
    float_data_toBe_display[26] = mVeh_DW.HWlog_uD_single_p27_DSTATE;
    float_data_toBe_display[27] = mVeh_DW.HWlog_uD_single_p28_DSTATE;
    float_data_toBe_display[28] = mVeh_DW.HWlog_uD_single_p29_DSTATE;
    float_data_toBe_display[29] = mVeh_DW.HWlog_uD_single_p30_DSTATE;
    float_data_toBe_display[30] = mVeh_DW.HWlog_uD_single_p31_DSTATE;
    float_data_toBe_display[31] = mVeh_DW.HWlog_uD_single_p32_DSTATE;

    uint8_data_toBe_display[0] = mVeh_DW.HWlog_uD_uint8_p1_DSTATE;
    uint8_data_toBe_display[1] = mVeh_DW.HWlog_uD_uint8_p2_DSTATE;
    uint8_data_toBe_display[2] = mVeh_DW.HWlog_uD_uint8_p3_DSTATE;
    uint8_data_toBe_display[3] = mVeh_DW.HWlog_uD_uint8_p4_DSTATE;
    uint8_data_toBe_display[4] = mVeh_DW.HWlog_uD_uint8_p5_DSTATE;
    uint8_data_toBe_display[5] = mVeh_DW.HWlog_uD_uint8_p6_DSTATE;
    uint8_data_toBe_display[6] = mVeh_DW.HWlog_uD_uint8_p7_DSTATE;
    uint8_data_toBe_display[7] = mVeh_DW.HWlog_uD_uint8_p8_DSTATE;
    uint8_data_toBe_display[8] = mVeh_DW.HWlog_uD_uint8_p9_DSTATE;
    uint8_data_toBe_display[9] = mVeh_DW.HWlog_uD_uint8_p10_DSTATE;
    uint8_data_toBe_display[10] = mVeh_DW.HWlog_uD_uint8_p11_DSTATE;
    uint8_data_toBe_display[11] = mVeh_DW.HWlog_uD_uint8_p12_DSTATE;
    uint8_data_toBe_display[12] = mVeh_DW.HWlog_uD_uint8_p13_DSTATE;
    uint8_data_toBe_display[13] = mVeh_DW.HWlog_uD_uint8_p14_DSTATE;
    uint8_data_toBe_display[14] = mVeh_DW.HWlog_uD_uint8_p15_DSTATE;
    uint8_data_toBe_display[15] = mVeh_DW.HWlog_uD_uint8_p16_DSTATE;
    uint8_data_toBe_display[16] = mVeh_DW.HWlog_uD_uint8_p17_DSTATE;
    uint8_data_toBe_display[17] = mVeh_DW.HWlog_uD_uint8_p18_DSTATE;
    uint8_data_toBe_display[18] = mVeh_DW.HWlog_uD_uint8_p19_DSTATE;
    uint8_data_toBe_display[19] = mVeh_DW.HWlog_uD_uint8_p20_DSTATE;
    uint8_data_toBe_display[20] = mVeh_DW.HWlog_uD_uint8_p21_DSTATE;
    uint8_data_toBe_display[21] = mVeh_DW.HWlog_uD_uint8_p22_DSTATE;
    uint8_data_toBe_display[22] = mVeh_DW.HWlog_uD_uint8_p23_DSTATE;
    uint8_data_toBe_display[23] = mVeh_DW.HWlog_uD_uint8_p24_DSTATE;
    uint8_data_toBe_display[24] = mVeh_DW.HWlog_uD_uint8_p25_DSTATE;
    uint8_data_toBe_display[25] = mVeh_DW.HWlog_uD_uint8_p26_DSTATE;
    uint8_data_toBe_display[26] = mVeh_DW.HWlog_uD_uint8_p27_DSTATE;
    uint8_data_toBe_display[27] = mVeh_DW.HWlog_uD_uint8_p28_DSTATE;
    uint8_data_toBe_display[28] = mVeh_DW.HWlog_uD_uint8_p29_DSTATE;
    uint8_data_toBe_display[29] = mVeh_DW.HWlog_uD_uint8_p30_DSTATE;
    uint8_data_toBe_display[30] = mVeh_DW.HWlog_uD_uint8_p31_DSTATE;
    uint8_data_toBe_display[31] = mVeh_DW.HWlog_uD_uint8_p32_DSTATE;
}

// 从站初始配置
int slave_setup(uint16_t slave)
{
    if (slave > ec_slavecount)
    {
        printf("slave %d do not exist\n", slave);
        return;
    }
    uint16_t c32_01, c33_01, c32_08, c33_08;
    uint32_t c32_02, c33_02, c33_03;
    printf("Running slave config!\n");

    uint16_t cnt = 0;
    // address got from Twincat setup
    c32_01 = 0x0001;     // doesn't seem to change
    c32_02 = 0x003D0900; // doesn't seem to change

    c33_01 = 0x0022;          // doesn't seem to change
    c33_02 = 0x003D0900;      // doesn't seem to change
    c33_03 = 0x000186A0;      // 这个startup里有误，所以不要看startup里的，看coe online里面的   //doesn't seem to change
    c32_08 = c33_08 = 0x0001; // doesn't seem to change

    uint8_t read_data[128][256]; // 从csv中read到的data
    memset(read_data, 0, sizeof(read_data));
    len_of_eachCsv[slave] = get_csv(read_data, slave);
    // sync setup

    cnt += ec_SDOwrite(slave, 0x1C32, 0x01, FALSE, sizeof(c32_01), &c32_01, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C32, 0x02, FALSE, sizeof(c32_02), &c32_02, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C32, 0x08, FALSE, sizeof(c32_08), &c32_08, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C33, 0x01, FALSE, sizeof(c33_01), &c33_01, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C33, 0x02, FALSE, sizeof(c33_02), &c33_02, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C33, 0x03, FALSE, sizeof(c33_03), &c33_03, EC_TIMEOUTSAFE);
    cnt += ec_SDOwrite(slave, 0x1C33, 0x08, FALSE, sizeof(c33_08), &c33_08, EC_TIMEOUTSAFE);
    // CAN Interface config
    cnt += ec_SDOwrite(slave, 0xF800, 0x00, TRUE, sizeof(reg_0xF800), &reg_0xF800, EC_TIMEOUTSAFE);
    cnt += setting_register(slave, read_data);

    // error info
    while (ec_iserror())
    {
        printf("CAN ID: %i :%s \n", slave, ec_elist2string());
    }

    reg0x80_Num[slave] = (len_of_eachCsv[slave] - 2) / 4;

    for (int k = 0; k < sendMsgNum[slave]; k++)
    {
        printf("slave %d msg %d data length = %d COBID = %x \n", slave, k + 1, msgSendDataLength[slave][k], msgSendCOBID[slave][k]);
    }

    for (int k = 0; k < readMsgNum[slave]; k++)
    {
        printf("msg %d data length = %d COBID = %X\n", k + 1, msgReadDataLength[slave][k], msgReadCOBID[slave][k]);
    }

    for (int k = 0; k < sendMsgNum[slave]; k++)
    {
        totalSendDataByteLength[slave] += msgSendDataLength[slave][k];
        if (k > 0)
            msgSendOffset[slave][k] = msgSendOffset[slave][k - 1] + msgSendDataLength[slave][k - 1];
    }
    for (int k = 0; k < readMsgNum[slave]; k++)
    {
        totalReadDataByteLength[slave] += msgReadDataLength[slave][k];
        if (k > 0)
            msgReadOffset[slave][k] = msgReadOffset[slave][k - 1] + msgReadDataLength[slave][k - 1];
    }
    // getchar();
    return 1;
}

__uint32_t get_csv(__uint8_t (*ptr)[256], __uint16_t slave)
{

    char *filePath = (char *)malloc(sizeof(char) * 128);
    memset(filePath, 0, 128);

    filePath = strcat(filePath, "./csv/read");
    char filenum[16] = "0";

    if (slave < 10)
    {
        filenum[0] += slave;
    }
    else if (slave >= 10 && slave < 100)
    {
        filenum[0] = (slave / 10) % 10 + '0';
        filenum[1] = (slave % 10) + '0';
        filenum[2] = '\0';
    }

    filePath = strcat(filePath, filenum);
    filePath = strcat(filePath, ".csv");
    puts(filePath);
    FILE *fp = fopen(filePath, "r");
    const char s[2] = ",";
    if (fp == NULL)
    {
        fprintf(stderr, "open csv file error\n");
        exit(EXIT_FAILURE);
    }
    char row[2048];
    int ans = 1;
    char *token;
    __uint8_t length = 0; // 统计需要读取寄存器有多少
    //__uint32_t ptr[128][128];
    while (fgets(row, 1024, fp) != NULL)
    {
        ans = 1;
        // printf("Row: %s\n", row);
        token = strtok(row, s);
        while (token != NULL && ans <= 4)
        {

            if (ans == 3)
            {
                if (strncmp("0x8", token, 3) && strncmp("0x1C1", token, 5)) // 目前只读取这些开头的寄存器,更改这里
                                                                            // 会改比较多的地方
                    break;
            }
            // printf("%s", token);
            if (ans == 4)
            {
                read_uint8Data(ptr[length++], token);
            }
            token = strtok(NULL, s);
            ans++;
        }
        // printf("\n");
    }
    fclose(fp);
    return length;
}

void read_uint8Data(__uint8_t ptr[], char *token)
{
    char num[64];
    __uint8_t length = 1; // 每一行的data数
    int k = 0;
    int j = 1;
    for (int i = 0; token[i] != '\0'; i++)
    {
        num[k++] = token[i];
        if ((token[i] == ' '))
        {
            num[k] = '\0';
            k = 0;
            ptr[j++] = strtol(num, NULL, 16);
            length++;
        }
        ptr[0] = length;
    }
}
// getchar();
int setting_register(uint16 slave, uint8_t (*reg_csv)[256]) // 读取twincat的配置.csv文件
{
    memset(msgSendCOBID[slave], 0, sizeof(msgSendCOBID[slave]));
    memset(msgReadCOBID[slave], 0, sizeof(msgReadCOBID[slave]));
    int cnt = 0;
    uint8_t NumOf_csv = len_of_eachCsv[slave];
    int k = 0;      // 遍历 reg_address
    int offset = 0; // 偏移，比如从0x8008偏移到0x8018
    uint8_t load_temp[256];
    uint8_t dataLen;
    uint8_t *ptr = msgSendDataLength[slave];
    uint8_t *ptr2 = msgReadDataLength[slave];
    uint64_t *ptr3 = msgSendCOBID[slave];
    uint64_t *ptr4 = msgReadCOBID[slave];
    readMsgNum[slave] = sendMsgNum[slave] = 0;
    for (int i = 0; i < NumOf_csv - 2; i++) // 后两行是1c12，1c13
    {
        if (k == 4)
        {
            k = 0;
            offset += 0x10;
        }
        if (reg_csv[i][0] != 0)
        {
            dataLen = reg_csv[i][0];
            // printf("x=%x  length=%d ",reg_address[k]+offset,reg_csv[i][0]);
            for (int j = 1; j <= dataLen; j++)
            {
                load_temp[j - 1] = reg_csv[i][j];
                // printf("%d ",load_temp[j-1]);
            }

            if (k == 1) // reg_address[]
            {
                if (load_temp[0] == 0 && load_temp[1] == 0)
                    *ptr2 = 0; // 为0的话，表示这条msg没有TxPDO
                else
                {
                    uint8 count = load_temp[0];
                    readMsgNum[slave] += count;
                    for (int j = 0; j < count; j++)
                    {
                        *ptr2++ = load_temp[j * 12 + 7];
                        for (int p = 0; p < 4; p++)
                        {
                            *ptr4 += load_temp[j * 12 + p + 2] * (uint64_t)pow(256, p);
                            // printf("inner ptr4 %.2x\n", *ptr4);
                            // printf("slave = %d %.2x %.2x\n", slave, msgReadCOBID[2][0], msgReadCOBID[2][1]);
                        }
                        ptr4++;
                    }
                }
            }

            if (k == 2) // reg_address[]
            {
                if (load_temp[0] == 0 && load_temp[1] == 0)
                    *ptr = 0; // 为0的话，表示这条msg没有RxPDO
                else
                {
                    uint8 count = load_temp[0];
                    sendMsgNum[slave] += count;
                    for (int j = 0; j < count; j++)
                    {
                        *ptr++ = load_temp[j * 12 + 7];
                        for (int p = 0; p < 4; p++)
                        {
                            *ptr3 += load_temp[j * 12 + p + 2] * (uint64_t)pow(256, p);
                        }
                        ptr3++;
                    }
                }
            }
            cnt += ec_SDOwrite(slave, reg_address[k++] + offset, 0x00, TRUE, dataLen, &load_temp, EC_TIMEOUTSAFE);
        }
        // printf("\n");
    }
    for (int j = 1; j <= reg_csv[NumOf_csv - 2][0]; j++)
        load_temp[j - 1] = reg_csv[NumOf_csv - 2][j];
    cnt += ec_SDOwrite(slave, 0x1C12, 0x00, TRUE, reg_csv[NumOf_csv - 2][0], &load_temp, EC_TIMEOUTSAFE);
    for (int j = 1; j <= reg_csv[NumOf_csv - 1][0]; j++)
        load_temp[j - 1] = reg_csv[NumOf_csv - 1][j];
    cnt += ec_SDOwrite(slave, 0x1C13, 0x00, TRUE, reg_csv[NumOf_csv - 2][0], &load_temp, EC_TIMEOUTSAFE);
    return cnt;
}

int init_setting(char *ifname)
{
    int chk;
    printf("Initing……\n ");
    /* initialise SOEM, bind socket to ifname */
    if (!ec_init(ifname))
    {
        // printf("bind socket to %d error",ifname);
        return 0;
    }
    // printf("socket bind on %s succeeded\n", ifname);
    /* find and auto-config slaves */
    if (ec_config_init(FALSE) <= 0)
    {
        // printf("No connect\n");
        return 1;
    }

    ec_slave[0].state = EC_STATE_PRE_OP | EC_STATE_ACK; /* set to Pre-Op State */
    ec_writestate(0);
    printf("%d slaves found and configured.\n", ec_slavecount);
    chk = 100;
    do
    {
        iCurStateRead = ec_statecheck(0, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);
    } while (iCurStateRead != EC_STATE_PRE_OP && --chk >= 0);

    uint16_t slave_ix;
    for (slave_ix = 0; slave_ix <= ec_slavecount; ++slave_ix)
    {
        printf("slave num:%d  name: %s \n", slave_ix, ec_slave[slave_ix].name);
    }
    for (slave_ix = 2; slave_ix <= ec_slavecount; ++slave_ix)
    {
        ec_slave[slave_ix].PO2SOconfig = &slave_setup;
        // slave_setup(slave_ix);
        ec_slave[slave_ix].CoEdetails &= ~ECT_COEDET_SDOCA;
    }
    ec_config_map(&IOmap);
    // printf("dc = %d\n", ec_configdc()); // dc有问题需要解决
    // getchar();
    printf("Slaves mapped, state to SAFE_OP.\n");
    /* wait for all slaves to reach SAFE_OP state */
    ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE / 10);

    printf("Request operational state for all slaves\n");
    ec_slave[0].state = EC_STATE_OPERATIONAL;

    /* send one valid process data to make outputs in slaves happy*/
    chk = 50;
    do
    {
        ec_send_processdata();
        ec_receive_processdata(EC_TIMEOUTRET);
    } while (--chk);

    /* request OP state for all slaves */
    ec_writestate(0); // 0=master
    chk = 50;
    /* wait for all slaves to reach OP state */
    do
    {
        ec_send_processdata();
        ec_receive_processdata(EC_TIMEOUTRET);
        ec_statecheck(0, EC_STATE_OPERATIONAL, 1000);
    } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

    if (ec_slave[0].state == EC_STATE_OPERATIONAL)
    {
        printf("Operational state reached for all slaves.\n");
        // getchar();
        return 2;
    }

    else
    {
        printf("Not all slaves reached operational state.\n");
        ec_readstate();
        for (int i = 1; i <= ec_slavecount; i++)
        {
            if (ec_slave[i].state != EC_STATE_OPERATIONAL) // 查看哪个从站没有达到OP状态
            {
                printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                       i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
            }
        }
        printf("reinit setting\n");
        // getchar();
        return init_setting(ifname);
    }
    return 1;
}

boolean write_Into_pos(uint16_t slave, uint64_t COB_ID, uint8_t *value)
{
    int k = use_COB_ID_find_k(slave, COB_ID);
    if (k < 0)
    {
        return FALSE;
    }
    uint8_t *data_ptr = ec_slave[slave].outputs;
    data_ptr += msgSendOffset[slave][k];
    uint8_t *value_ptr = value;
    for (int i = 0; i < msgSendDataLength[slave][k]; i++)
    {
        *data_ptr++ = *value_ptr++;
    }

    return TRUE;
}

int use_COB_ID_find_k(uint16_t slave, uint64_t COB_ID)
{
    for (int i = 0; i < sendMsgNum[slave]; i++)
    {
        if (msgSendCOBID[slave][i] == COB_ID)
            return i;
    }
    return -1;
}

void show_data()
{
    for (int slave_ix = 2; slave_ix <= ec_slavecount; ++slave_ix)
    {
        fflush(stdout);
        printf("output canid:%d\n", slave_ix);
        uint16 lengthSum = 0;
        for (int k = 0; k < sendMsgNum[slave_ix]; ++k)
        {

            for (int j = 0; j < msgSendDataLength[slave_ix][k]; ++j)
            {
                printf("%.2x ", *(ec_slave[slave_ix].outputs + lengthSum + j));
            }
            lengthSum += msgSendDataLength[slave_ix][k];
            printf("\n");
        }
        printf("input canid:%d \n", slave_ix);
        lengthSum = 0;
        for (int k = 0; k < readMsgNum[slave_ix]; ++k)
        {
            fflush(stdout);
            for (int j = 0; j < msgReadDataLength[slave_ix][k]; ++j)
            {
                printf("%.2x ", *(ec_slave[slave_ix].inputs + j + lengthSum));
            }
            lengthSum += msgReadDataLength[slave_ix][k];
            printf("\n");
        }
    }
}
boolean read_from_pos(uint16_t slave, uint64_t COB_ID, uint8_t *value)
{
    int k = use_Read_COB_ID_find_k(slave, COB_ID);
    if (k < 0)
        return FALSE;
    uint8_t *data_ptr = ec_slave[slave].inputs + msgReadOffset[slave][k];
    uint8_t *value_ptr = value;
    for (int i = 0; i < msgReadDataLength[slave][k]; i++)
    {
        *value_ptr++ = *data_ptr++;
    }
    return TRUE;
}
int use_Read_COB_ID_find_k(uint16_t slave, uint64_t COB_ID)
{
    for (int i = 0; i < readMsgNum[slave]; ++i)
    {
        if (msgReadCOBID[slave][i] == COB_ID)
            return i;
    }
    return -1;
}

void SendUdpMsg()
{
    while (1)
    {
        osal_usleep(10000);
        load_udp_msgToBuffer();
        int ret1 = sendUdpMsg(float_data_toBe_display, sizeof(float_data_toBe_display) / sizeof(float), &udp_struct_float);
        int ret2 = sendUdpMsg(uint8_data_toBe_display, sizeof(uint8_data_toBe_display), &udp_struct_uint8);
        if (ret1 == -1 || ret2 == -1)
        {
            break;
        }
    }
    return;
}
