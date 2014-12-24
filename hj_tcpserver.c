#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAXLINE 127

void reverseString(int i);

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	int listen_sock, accp_sock, addrlen=sizeof(client_addr), nbyte;
	char tcp_buf[MAXLINE+1];
	int key, shmid;
	char *shm_msg;
	struct linger ling;
	ling.l_onoff=1;
	ling.l_linger=0;
	if(argc != 2) {
		printf("usage : %s port\n", argv[0]);
		exit(0);
	}

	if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket fail");
		exit(0);
	}

	bzero((char *)&server_addr, sizeof(server_addr));
	setsockopt(listen_sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind fail");
		exit(0);
	}
	
	listen(listen_sock, 5);
	while(1){
		puts("클라이언트의 요청을 기다리는 중 [listen]");
		accp_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addrlen);
		if(accp_sock < 0) {
			perror("accept fail");
			exit(0);
		}	
		puts("클라이언트가 연결됨");
		nbyte = read(accp_sock, tcp_buf, sizeof(tcp_buf));
		key = atoi(tcp_buf);

		reverseString(key);
 		printf("reverse end %d", key);	
		strcpy(tcp_buf, "correct!!");
		printf("write %s", tcp_buf);
		write(accp_sock, tcp_buf, sizeof(tcp_buf));
		printf("write end");
		close(accp_sock);
	}
	close(listen_sock);
	return 0;
}

void reverseString(int key){
	char *shm_msg;
	char *tmp_msg="";
	int count,i;
	int shmid = shmget(key, sizeof(shm_msg), 0600|IPC_CREAT);
	if(shmid == -1){
		perror("no shared memory");
		exit(2);
	}
	shm_msg = shmat(shmid, (char *) 0,0);
	count = strlen(shm_msg);
	tmp_msg = (char *)malloc(sizeof(char)*(count+1));
	strcpy(tmp_msg, shm_msg);
	for(i=0; i<count; i++){
		shm_msg[i] = tmp_msg[count-1-i];
	}
	free(tmp_msg);
}
