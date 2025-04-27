#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>


int main() {
	int np, pid;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Status sta;
	int arr[100];

	if (pid == 0) {
		for (int i = 0; i < 100; i++) {
			arr[i] = rand() % 1000;
		}

		for (int i = 1; i < 10; i++) {
			MPI_Send(&arr[i * 10], 10, MPI_INT, i, 50, MPI_COMM_WORLD);
		}

		printf("Processor ID: %d | Data is: \n", pid);
		for (int i = 0; i < 10; i++) {
			printf("%d ", arr[i]);
		}
		printf("\n");	

		int globalMaximum = 0;
		int globalMinimum = 1000;

		for (int i = 0; i < 10; i++) {
			if (arr[i] > globalMaximum) globalMaximum = arr[i];
			if (arr[i] < globalMinimum) globalMinimum = arr[i];
		}

		for (int i = 1; i < 10; i++) {
			int recMaximum, recMinimum;
			MPI_Recv(&recMaximum, 1, MPI_INT, i, 51, MPI_COMM_WORLD, &sta);
			MPI_Recv(&recMinimum, 1, MPI_INT, i, 52, MPI_COMM_WORLD, &sta);
			printf("Processor ID: %d | Local Maximum is: %d \t Local Minimum is: %d \n", i, recMaximum, recMinimum);
			if (recMaximum > globalMaximum) globalMaximum = recMaximum;
			if (recMinimum < globalMinimum) globalMinimum = recMinimum;
		}
		printf("Processor ID: %d | Global Maximum is: %d \t Global Minimum is: %d \n\n", pid, globalMaximum, globalMinimum);
	}

	if (pid != 0) {
		int recarr[10];
		MPI_Recv(recarr, 10, MPI_INT, 0, 50, MPI_COMM_WORLD, &sta);
		printf("Processor ID: %d | Data is: \n", pid);
		for (int i = 0; i < 10; i++) {
			printf("%d ", recarr[i]);
		}
		printf("\n");

		int localMaximum = 0;
		int localMinimum = 1000;

		for (int i = 0; i < 10; i++) {
			if (recarr[i] > localMaximum) localMaximum = recarr[i];
			if (recarr[i] < localMinimum) localMinimum = recarr[i];
		}

		printf("Local Maximum: %d | Local Minimum: %d \n\n", localMaximum, localMinimum);

		MPI_Send(&localMaximum, 1, MPI_INT, 0, 51, MPI_COMM_WORLD);
		MPI_Send(&localMinimum, 1, MPI_INT, 0, 52, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
