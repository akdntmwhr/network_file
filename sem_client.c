#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEYPATH "/dev/null"
#define SEMKEYID 1

#define SHMKEYPATH "/dev/null"
#define SHMKEYID 1

#define NUMSEMS 2
#define SIZEOFSHMSEG 50

int main(int argc, char *argv[])
{
	struct sembuf operations[2];
	void *shm_address;
	int semid, shmid, rc;
	key_t semkey, shmkey;

	semkey = ftok(SEMKEYPATH, SEMKEYID);
	if(semkey == (key_t) -1)
	{
		printf("main : ftok() for sem failed\n");
		return -1;
	}
	shmkey = ftok(SHMKEYPATH, SHMKEYID);
	if(shmkey == (key_t) -1)
	{
		printf("main: ftok() for shm failed\n");
		return -1;
	}
	semid = semget(semkey, NUMSEMS, 0666);
	if(semid == -1)
	{	
		printf("main: semget() failed\n");
		return -1;
	{

	shm_address = shmat(shmid, NULL, 0) 
	if(shm_address==NULL)
	{
		printf("main : semget() failed\n");
		return -1;
	}

	shmid = shmget(shmkey, SIZEOFSHMSEG, 0666);
	if(shmid == -1)
	{	
		printf("main: shmget() failed\n");
		return -1;
	}

	shm_address = shmat(shmid, NULL, 0);
	if(shm_address==NULL)
	{
		printf("main: shmat() failed\n");
		return -1;
	}
	
	operations[0].sem_num = 0;
	operations[0].sem_op = 0;
	operations[0].sem_flg = 0;

	operations[1].sem_num = 0;
	operations[1].sem_op = 1;
	operations[1].sem_flg = 0;

	rc = semop(semid, operations, 2);
	if(rc == -1)
	{
		printf("main: semop() failed\n");
		return -1;
	}

	strcpy((char *) shm_address, "hello from client");

	operations[0].sem_num = 0;
	operations[0].sem_op = -1;
	operations[0].sem_flg = 0;
		
	operations[1].sem_num = 1;
	operations[1].sem_op = 1;
	operations[1].sem_flg = 0;

	rc = semop(semid, operations, 2);
	if(rc == -1)
	{
		printf("main : semop() failed\n");
		return -1;
	}

	rc = shmdt(shm_address);
	if(rc==-1)
	{
		printf("main : shmdt() failed\n");
		return -1;
	}
	return 0;
}

		
}

