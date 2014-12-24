#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define SHMKEY 5355
#define MAXLINE 127

int main(int argc, char *argv[]){
	key_t shmkey = SHMKEY;
	struct sockaddr_in server_addr;
	int s, nbyte, shmid;
	char *shm_msg;
	char tcp_msg[MAXLINE+1];

	if(argc!=3){
		printf("usage: %s ip_address\n", argv[0]);
		exit(0);
	}
	
	shmid = shmget(shmkey, sizeof(shm_msg), 0600|IPC_CREAT);
	if(shmid== -1) {
		perror("producer semget():");
		exit(2);
	}
	shm_msg = shmat(shmid, (char *) 0, 0);
	strcpy(shm_msg, "ABCDEFG");
	sprintf(tcp_msg, "%d", SHMKEY);

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket fail");
		exit(0);
	}

	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0) {
		perror("connect fail");
		exit(0);
	}
	
	printf("공유 메모리 키 : 5355\n");

	nbyte = sizeof(SHMKEY);
	if(write(s, tcp_msg, nbyte) < 0){
		printf("write error\n");
		exit(0);
	}

	printf("수신 : ");
	while(1){
		if((nbyte=read(s, tcp_msg, sizeof(tcp_msg))) >0 ){
			printf("%s\n", tcp_msg);
			printf("result : %s", shm_msg);
			break;
		}
	}	 

	shmdt(shm_msg);
	shmctl(shmid, IPC_RMID, NULL);	
	close(s);
	return 0;
}

