#include <sys/time.h>

void ShowTime ()
{
	struct timeval the_time;
	struct timezone the_zone;

	gettimeofday (&the_time, &the_zone);
	printf ("%d seconds, %d microseconds \n", the_time.tv_sec, the_time.tv_usec);
//printf ("Time zone: %d\n", the_zone.tz_minuteswest);
//printf ("Daylight savings time: %d\n", the_zone.tz_dsttime);

}