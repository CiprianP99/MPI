/*Let there be n processes. Each process generates m (m>=100) random numbers ( <=1000 ), prints them to the console, computes their sum and prints it to the console. 
Determine the time it takes for each process to complete the job.*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 100
int data[N];
//mpiexec -n 6 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	
	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors
	
	
	int numbersToGenerate;
	int* arr;
	
	if (rank == 0) {
		srand(0);
		for (int i = 1; i < size; i++)
		{
			
			numbersToGenerate = (rand() % (1000 - 100 + 1) + 100);
			MPI_Send(&numbersToGenerate, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
	
	else {
		double start = MPI_Wtime();

		MPI_Recv(&numbersToGenerate, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		printf("Process %d must generate %d random numbers\n", rank, numbersToGenerate);

		arr = malloc(numbersToGenerate * sizeof(int*));
		int s = 0;
		srand(0);
		for (int i = 0; i < numbersToGenerate; i++)
		{
			arr[i] = rand();
			s = s + arr[i];
		}
		printf("From rank %d we generated the following numbers: ", rank);

		for (int i = 0; i < numbersToGenerate; i++)
			printf("%d ", arr[i]);

		printf("\n");
		printf("The sum from the rank %d is %d\n", rank, s);

		double end = MPI_Wtime();

		printf("The process from rank %d took %lf seconds to run\n", rank, end - start);
	}
	
	
	MPI_Finalize();
	return 0;
}
