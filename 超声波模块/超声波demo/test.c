#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc,char **argv){
	int fd;
	uint32_t distance;
	
	fd=open("dev/mysr04",O_RDWR);
	if(fd<0){
		perror("fail open dev/mysr04\n");
		return -1;
	}
	
	while(1){
		//ssize_t read(int fd, void *buf, size_t count);
		//buf	用户空间的缓冲区指针，用于接收数据（这里是 &distance）
		//count	希望读取的数据字节数（这里是 sizeof(distance)，一般是 4 字节）
		ssize_t ret=read(fd,&distance,sizeof(distance));
		if(ret<0){
			perror("fail read \n");
			close(fd);
			return -1;
		}
		
		if (t > 100000) {
            printk(KERN_WARNING "Echo pulse is too long\n");
            return 0;  // 返回一个合理值或错误码
        }
		
		printf("distance=%d \n",distance);
		sleep(1);
		
	}
	
	close(fd);
	return 0;
	
}


//关于int argc,char** argc
//./test_led /dev/myled
//argc = 2;
//argv[0] = "./test_led";
//argv[1] = "/dev/myled";