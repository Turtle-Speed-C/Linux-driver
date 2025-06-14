#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char **argv){
	int fd_led = open("/dev/myled",O_RDWR);
	
	if(fd_led < 0){
		perror("open /dev/myled");
		
		return -1;
	}
	
	sleep(3);
	
	close(fd_led);
	
}


//关于int argc,char** argc
//./test_led /dev/myled
//argc = 2;
//argv[0] = "./test_led";
//argv[1] = "/dev/myled";