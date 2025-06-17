#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char **argv){
	char write_buf[10]={0};
	//char test_buf[2]={'0','0'};
	int len;
	
	int fd_led = open("/dev/myled",O_RDWR);
	if(fd_led < 0){
		perror("open /dev/myled");
		return -1;
	}
		
	//ssize_t write(int fd, const void *buf, size_t count);
	len = write(fd_led,write_buf,10);
	printf("len = %d\n",len);
	
	sleep(3);
	/*
	for(int i=0;i<10;i++){
		len=write(fd_led,write_buf,2);
		printf("[INFO] LED ON, write len = %d\n", len);
        sleep(1);
		len = write(fd_led, test_buf, 2);
        printf("[INFO] LED OFF, write len = %d\n", len);
        sleep(1);
		
	}*/
	
	close(fd_led);
	
}


//关于int argc,char** argc
//./test_led /dev/myled
//argc = 2;
//argv[0] = "./test_led";
//argv[1] = "/dev/myled";