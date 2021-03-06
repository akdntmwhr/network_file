#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 512
#define MAX_SOCK 128

char *escapechar = "exit";
char name[10];

int main(int argc, char *argv[]){
	char line[MAXLINE], msg[MAXLINE+1];
	int n, pid;
	struct sockaddr_in server_addr;
	int maxfdp1;
	int s;

	fd_set read_fds;
	
	if(argc != 4){
		printf("사용법 : %s server_IP port name \n", argv[0]);
		exit(0);
	}

	sprintf(name, "[%s]", argv[3]);

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0 ){
		printf("클라이언트 : 스트림 소켓을 열 수 없습니다.\n");
		exit(0);
	}

	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0){
		printf("클라이언트 : 서버에 접속할 수 없습니다.\n");
		exit(0);
	} else {
		printf("서버에 접속되었습니다.\n");
	}

	maxfdp1 = s+1;
	FD_ZERO(&read_fds);

	while(1){
		FD_SET(0, &read_fds);
		FD_SET(s, &read_fds);

		if(select(maxfdp1, &read_fds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0){
			printf("select error\n");
			exit(0);
		}

		if(FD_ISSET(s, &read_fds)) {
			int size;
			if((size = recv(s, msg, MAXLINE, 0)) > 0) {
				msg[size] = '\0';
				printf("%s \n", msg);
			}
		}

		if(FD_ISSET(0, &read_fds)){
			if(fgets(msg, MAXLINE, stdin)){
				sprintf(line, "%s %s", name, msg);

				if(send(s, line, strlen(line), 0) < 0)
					printf("Error : Write error on socket.\n");

				if(strstr(msg, escapechar) !=NULL) {
					printf("종료되었습니다.\n");
					close(s);
					exit(0);
				}
			}
		}
		kill(7468, SIGUSR1);
	}
}
