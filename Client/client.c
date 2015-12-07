// Simple UDP client

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
 
#define SERVER "127.0.0.1"
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to send data
#define TESTLEN 1000000

void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(void)
{
	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	// Time
	long nsBefore, nsAfter;
	time_t sBefore, sAfter;
	struct timespec specBefore, specAfter;
	intmax_t interval, maxInt = 0, minInt = 1000000, avgInt = 0, avgInt2 = 0;
 
	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
 
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	 
	if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
 
	printf("Enter message : ");
	fgets(message, sizeof(message), stdin);
	for (i = 0; i < TESTLEN; i++)
	{
		// Getting the time before
		clock_gettime(CLOCK_MONOTONIC, &specBefore);
		//send the message
		if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
		{
			die("sendto()");
		}
		 
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*)&slen) == -1)
		{
			die("recvfrom()");
		}
		// Getting the time after
		clock_gettime(CLOCK_MONOTONIC, &specAfter);
		sBefore = specBefore.tv_sec;
		sAfter = specAfter.tv_sec;
		nsBefore = specBefore.tv_nsec;
		nsAfter = specAfter.tv_nsec;
		
		// Counting time interval
		interval = (sAfter-sBefore)*1.0e9 + (nsAfter-nsBefore);
		if (interval > maxInt) maxInt = interval;
		if (interval < minInt) minInt = interval;
		avgInt += (interval - avgInt)/(i+1);
		avgInt2 += interval;
		printf("%"PRIdMAX"\n", interval);
	}
	avgInt2 /= TESTLEN;
	printf("Max:\t%"PRIdMAX"\nMin:\t%"PRIdMAX"\nAverage:\t%"PRIdMAX"\nAverage2:\t%"PRIdMAX"\n", maxInt, minInt, avgInt, avgInt2);
	close(s);
	return 0;
}
