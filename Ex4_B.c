/*b. Use scatter for sending the array. If the element is found many times, print all its positions.
Use MPI_Gather for sending back the positions */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 20

int data[N];
//mpiexec -n 5 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors
	int resultIndexTotal;
	int numberToFind = 10;
	int sendArr[N];
	int recvArr[N];
	int indexArr[N] = { 0 };
	int recvIndex[N] = { 0 };
	int elementsPerProc = N / size;
	int elementsPerProcRecv = N / size;

	if (rank == 0) {

		srand(time(NULL));
		for (int i = 0; i <= N - 1; i++) {  //generating random numbers between 5 and 15
			sendArr[i] = (rand() % (15 - 5 + 1) + 5);
			printf("%d ", sendArr[i]);
		}
		printf("\n------------------------\n");
		
	}
	MPI_Scatter(sendArr, elementsPerProc, MPI_INT, recvArr, elementsPerProcRecv, MPI_INT, 0, MPI_COMM_WORLD);
	/*printf("From rank %d we have the following array ", rank);
	for (int i = 0; i < elementsPerProc; i++) {
		printf("%d ", recvArr[i]);
	}*/
	int k = 0;
	//MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < elementsPerProc; i++) {
		if (recvArr[i] == numberToFind)
		{
			indexArr[k] = (i + (rank * elementsPerProc));
			printf("The index that found the number %d from rank %d is %d\n", numberToFind, rank,indexArr[k] );
			k++;
		}
	}
	
	//MPI_Barrier(MPI_COMM_WORLD);
	//MPI_Reduce(&k, &resultIndexTotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	//MPI_Barrier(MPI_COMM_WORLD);
	if(k>=0)
	//MPI_Gather(indexArr, k, MPI_INT, recvIndex, k, MPI_INT, 0, MPI_COMM_WORLD);
		//sau gatherV???
	MPI_Gather(indexArr, elementsPerProc, MPI_INT, recvIndex, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		{
			/*printf("Resulted Final Index Numbers %d \n", 5);
			printf("Final indexes: ");
			for (int i = 0; i < resultIndexTotal; i++)
				printf("%d ", recvIndex[i]);  */
		//printf("Resulted Final Index Numbers %d \n", resultIndexTotal);
		printf("Final indexes: ");
		for (int i = 0; i < N; i++)
			if (recvIndex[i] == 0)
				printf("");
			else printf("%d ",recvIndex[i]);
		}
	MPI_Finalize();
	return 0;
}
