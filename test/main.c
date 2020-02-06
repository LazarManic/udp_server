#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "udp_server.h"


void my_sleep(double duration)
{
    time_t start = time(NULL);
    double end = duration;
    time_t now;
    do {
        now = time(NULL);
    } while (difftime(now, start) < end);
}



int main()
{
	init_server(1234, 1235, 70);
	

	int x = 1;
	int y = 1;
	int z = x+y;
	int t;
	int i = 0;


	for(i=0; i<10000; i++)
		usleep(1000);

	for(i=0; i<100000; i++)
	{
		send_data_s3("gravity", t, x, y, z);
		usleep(1);
	}

	printf("everything succesfully sent");

	return 0;
}
