#include<mpi.h>
#include<stdio.h>

int main() {
	int np, pid;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Status sta;

	if (pid == 0) {
		int numerics[8][8] = {
			{0, 2, 0, 3, 4, 12, 8, 2},
			{7, 20, 12, 0, 13, 8, 19, 24},
			{14, 17, 6, 0, 13, 8, 25, 4},
			{2, 0, 11, 0, 13, 3, 0, 17},
			{10, 4, 24, 1, 14, 0, 17, 3},
			{15, 4, 0, 2, 4, 5, 20, 11},
			{17, 4, 18, 4, 18, 4, 0, 17},
			{21, 14, 11, 0, 19, 8, 11, 4}
		};

		char alphabets[26] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
			'U', 'V', 'W', 'X', 'Y', 'Z'
		};

		for (int i = 1; i < 9; i++) {
			MPI_Send(&numerics[i-1], 8, MPI_INT, i, 50, MPI_COMM_WORLD);
			MPI_Send(alphabets, 26, MPI_CHAR, i, 51, MPI_COMM_WORLD);
		}

		char recv_decode[9];
		printf("Processor id: %d \n", pid);
		printf("All the decoded strings: \n");
		for (int i = 1; i < 9; i++) {
			MPI_Recv(recv_decode, 8, MPI_CHAR, i, 52, MPI_COMM_WORLD, &sta);
			recv_decode[8] = '\0';
			printf("Processor id: %d | Decoded string: %s \n", i, recv_decode);
		}
		printf("\n \n");
	}
	else {
		int recv_numerics[8];
		char recv_alphabets[26];
		char decode[8];

		MPI_Recv(recv_numerics, 8, MPI_INT, 0, 50, MPI_COMM_WORLD, &sta);
		MPI_Recv(recv_alphabets, 26, MPI_CHAR, 0, 51, MPI_COMM_WORLD, &sta);
		printf("Processor id: %d \nReceived array: \n", pid);
		for (int i = 0; i < 8; i++) {
			printf("%d ", recv_numerics[i]);
		}
		printf("\n");

		printf("Decoded word: \n");
		for (int i = 0; i < 8; i++) {
			decode[i] = recv_alphabets[recv_numerics[i]];
			printf("%c ", decode[i]);
		}
		printf("\n\n");

		MPI_Send(decode, 8, MPI_CHAR, 0, 52, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
