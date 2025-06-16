#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,char **argv){
	char write_buf[2]={'7','1'};
	char read_buf[1]={'0'};
	int len;
	
	int fd_led = open("/dev/myled",O_RDWR);
	if(fd_led < 0){
		perror("open /dev/myled");
		return -1;
	}
	
	len = read(fd_led, read_buf, 1);
    if (len < 0) {
        perror("read");
    } else {
        printf("read len = %d, data = %d\n", len, read_buf[0]);
    }
	
	//ssize_t write(int fd, const void *buf, size_t count);
	len = write(fd_led,write_buf,2);
	printf("len = %d",len);
	
	sleep(3);
	
	close(fd_led);
	
}


//关于int argc,char** argc
//./test_led /dev/myled
//argc = 2;
//argv[0] = "./test_led";
//argv[1] = "/dev/myled";