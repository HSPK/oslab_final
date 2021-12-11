#include "stdio.h"
#include "string.h"
#define DIR_ENTRY_SIZE 16

int main(int argc, char * argv[])
{
	int root_fd;
	char root_dir_buf[40];
	root_fd = open(".", O_RDWR);

	int ret;
	for(;;) {
		ret = read(root_fd, root_dir_buf, DIR_ENTRY_SIZE);
		if (ret <= 0 || root_dir_buf[4] == 0) break;
		printf("%s ", root_dir_buf + 4);
	}

	printf("\n");
	close(root_fd);
	return 0;
}
