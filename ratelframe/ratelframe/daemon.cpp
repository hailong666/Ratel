#include "stdafx.h"
#include "daemon.h"
#include <signal.h>
void Cdaemon::run(void){
	//signal(SIGCHLD, SIG_IGN);
	//int pid;
	//pid = fork();
	//if(pid < 0){
	//	exit(-1);
	//}
	//else if(pid > 0){
	//	exit(0);
	//}

	//int fd = fopen("/dev/null", O_RDWR, 0);
	//if(fd != -1){
	//	dup2(fd, STDIN_FILENO);
	//	dup2(fd, STDOUT_FILENO);
	//	dup2(fd, STDERR_FILENO);
	//}
	//else if(fd > 2){
	//	close(fd);
	//}

}