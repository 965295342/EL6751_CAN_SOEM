#include "multican.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        EtherCATargs args ;
        args.waitTime =10000;
        args.endThread = 0; 
        args.ifname = argv[1];
        pthread_t thread1;
        int ret = pthread_create(&thread1, NULL, (void *)interactWith_etherCAT, (void *)&args);
        if (ret != 0)
        {
            printf("pthread_create error\n");
            return -1;
        }
        // interactWith_etherCAT("eno2");
        uint16 index = 0;
        while (index <= 2000)
        {
            index++;
            usleep(10000);
        }
        args.endThread = 1;
        usleep(10000);
    }

    return 0;
}
