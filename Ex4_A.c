/*a. Use MPI_Broadcast for sending the array. If the element is found, print the maximum position index.
For computing the maximum position, you need to use MPI_Reduce. */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 20
#define BUF 5
int data[N];
//mpiexec -n 5 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors

	int numberToFind = 10;
	int arr[N];
	int x, low, high;
	int maximumIndex=-1;
	if (rank == 0) {
		srand(time(NULL));
		for (int i = 0; i <= N - 1; i++) {  //generating random numbers between 5 and 15
			arr[i] = (rand() % (15 - 5 + 1) + 5);
			printf("%d ", arr[i]);
		}
		printf("\n------------------------\n");
		
		
	}

	MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);
	/*printf("\nFrom rank %d, we got the following array:", rank);
	for (int i = 0; i < N; i++)
		printf("%d ", arr[i]); */
	
	x = N / size; /* must be an integer */
	low = rank * x;
	high = low + x;
	printf("From rank %d we got the following array: ", rank);
	for (int i = low; i < high; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	/*printf("I got %d from %d\n", myresult, myid); */
	int secondArr[50];
	int k = 0;
	for (int i = low; i < high; i++) {
		if (numberToFind == arr[i])
		{
			secondArr[k] = i;
			k++;
		}
	}
	if (k == 0) {
		printf("Number not found in process %d \n", rank);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&secondArr, &maximumIndex, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (0 == rank) {
		if (maximumIndex<-1)
			printf("No number is matching\n");
		else
			printf("The maximum index is %d\n", maximumIndex);
	}
	MPI_Finalize();
	return 0;
}
