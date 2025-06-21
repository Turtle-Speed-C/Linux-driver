#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	char buf[2]={7,1};
	int i;
	int fd_led = open("/dev/myled",O_RDWR);
	if(fd_led < 0)
	{
		perror("open /dev/myled");
		
		return -1;
		
	}
	
	while(1)
	{
		for(i=0; i<4; i++)
		{
			buf[0]=7+i;
			buf[1]=1;
			write(fd_led,buf,2);
			
			sleep(1);	

			buf[0]=7+i;
			buf[1]=0;
			write(fd_led,buf,2);
			
			sleep(1);				
			
		}
	
		
	}

	close(fd_led);

	return 0;
	
}
