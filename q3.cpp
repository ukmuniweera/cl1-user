#include<mpi.h>
#include<stdio.h>

int main() {
	int np, pid;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Status sta;

	if (pid == 0) {
		int phd = 0;
		int hds[8];
		int thd = 0;

		int sequence1[8][8] = {
			{1,1,0,0,0,0,1,1},
			{1,0,1,1,0,1,1,0},
			{0,1,0,0,0,0,0,1},
			{0,0,0,0,0,1,1,0},
			{1,1,1,1,0,1,0,1},
			{1,0,1,0,0,0,1,0},
			{0,1,0,1,0,1,0,1},
			{0,0,0,1,0,1,0,0}
		};

		int sequence2[8][8] = {
			{1,0,0,0,1,0,0,1},
			{1,1,0,1,1,1,0,0},
			{1,0,0,1,0,1,1,1},
			{0,1,0,0,0,1,1,0},
			{1,1,0,1,1,1,0,1},
			{1,1,1,0,1,0,1,1},
			{0,1,1,1,1,0,0,0},
			{1,0,1,1,1,0,1,1}
		};

		for (int i = 1; i < 8; i++) {
			MPI_Send(&sequence1[i], 8, MPI_INT, i, 50, MPI_COMM_WORLD);
			MPI_Send(&sequence2[i], 8, MPI_INT, i, 51, MPI_COMM_WORLD);
		}

		printf("My processor id is: %d \n", pid);
		printf("Received Data from sequence 1 is: ");
		for (int i = 0; i < 8; i++) {
			printf("%d ", sequence1[0][i]);

		}
		printf("\n");

		printf("Received Data from sequence 2 is: ");
		for (int i = 0; i < 8; i++) {
			printf("%d ", sequence2[0][i]);
		}
		printf("\n");

		for (int i = 0; i < 8; i++) {
			if (sequence1[0][i] != sequence2[0][i]) {
				phd = phd + 1;
			}
		}
		hds[0] = phd;
		printf("Hamming Distance: %d \n", hds[0]);

		for (int i = 1; i < 8; i++) {
			MPI_Recv(&hds[i], 1, MPI_INT, i, 52, MPI_COMM_WORLD, &sta);
		}

		printf("Hamming Distances are: ");
		for (int i = 0; i < 8; i++) {
			printf("%d ", hds[i]);
			thd = thd + hds[i];
		}
		printf("\n");

		printf("Total Hamming Distance: %d at PID: %d \n", thd, pid);
	}
	else {
		int recv_sequence1[8];
		int recv_sequence2[8];
		int hd = 0;

		MPI_Recv(recv_sequence1, 8, MPI_INT, 0, 50, MPI_COMM_WORLD, &sta);
		MPI_Recv(recv_sequence2, 8, MPI_INT, 0, 51, MPI_COMM_WORLD, &sta);

		printf("My processor id is: %d \n", pid);
		printf("Received Data from sequence 1 is: ");
		for (int i = 0; i < 8; i++) {
			printf("%d ", recv_sequence1[i]);

		}
		printf("\n");

		printf("Received Data from sequence 2 is: ");
		for (int i = 0; i < 8; i++) {
			printf("%d ", recv_sequence2[i]);

		}
		printf("\n");

		for (int i = 0; i < 8; i++) {
			if (recv_sequence1[i] != recv_sequence2[i]) {
				hd = hd + 1;
			}
		}
		printf("Hamming Distance: %d \n\n", hd);

		MPI_Send(&hd, 1, MPI_INT, 0, 52, MPI_COMM_WORLD);

		printf("########################################################### \n\n");
	}

	MPI_Finalize();
	return 0;
}