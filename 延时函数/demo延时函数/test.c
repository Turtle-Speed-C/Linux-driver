#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char **argv){
	
	int fd = 0;
	int len = 0;
	
	char d7_on[2]={7,1};
	char d7_off[2]={7,0};
	
	char d8_on[2]={8,1};
	char d8_off[2]={8,0};
	
	char d9_on[2]={9,1};
	char d9_off[2]={9,0};
	
	char d10_on[2]={10,1};
	char d10_off[2]={10,0};
	
	//open("/dev/myled", O_RDWR) 的作用是让用户空间程序访问内核中注册的字符设备，
	//从而触发内核中 file_operations 中注册的函数（如 led_open、led_write 等）。
	fd=open("/dev/myled",O_RDWR);
	if(fd<0){
		perror("open");
		return -1;
	}
	
	while(1){
		write(fd,d7_off,2);
		//sleep(1);
		
		write(fd,d7_on,2);
		//sleep(1);
		
		write(fd,d8_off,2);
		//sleep(1);
		
		write(fd,d8_on,2);
		//sleep(1);
		
		write(fd,d9_off,2);
		//sleep(1);
		
		write(fd,d9_on,2);
		//sleep(1);
		
		write(fd,d10_off,2);
		//sleep(1);
		
		write(fd,d10_on,2);
		//sleep(1);
	}
	close(fd);
	
	return 0;
}


//关于int argc,char** argc
//./test_led /dev/myled
//argc = 2;
//argv[0] = "./test_led";
//argv[1] = "/dev/myled";