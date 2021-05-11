#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 100

int data[N];

int main(int argc, char* argv[])
{
	
	MPI_Init(&argc, &argv);

	int rank, size;
	int isPrime = 1; //flag for prime
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors
	
	int* arr1; 
	if (rank == 0) {  //master process
		arr1 = malloc((N*2) * sizeof(int*)); //allocating memory for our array
		for (int i = 0; i <= N-1; i++) {
			arr1[i] = i;
		}
		int elements = 0;
		int slaves = size - 1; //setting the number of total slaves
		if (slaves != 0)
		{
			elements = N / slaves;
		}
			
	
		for (int i = 1; i < size; i++) {
			int start = (i-1) * elements; //start point in functie de proces  inmultit cu cate elemente pe fiecare proces avem
			int end = start + elements - 1; //end point in functie de start point adunat cu elementele pe fiecare proces - 1 pt ca urmatorul interval sa inceapa la un element dupa

			if (i == (size - 1)) //caz pt ultimul interval
				end = N - 1;

			//printf("The process %d has the start %d and the end %d \n", i, start, end);
			int element_count = end - start + 1; //counting the elements we have to send
			//printf("We want to send %d elements \n", element_count);
			MPI_Send(&element_count, 1 , MPI_INT, i, 1, MPI_COMM_WORLD); //trimitem numarul de elemente pe care vrem sa il trimitem
			MPI_Send((arr1 + start), element_count, MPI_INT, i, 1, MPI_COMM_WORLD); //trimitem elementele in functie de element_count
		}
	}
	else { //other process
		
		int element_count;
		MPI_Recv(&element_count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); //primim numarul de elemente pe care dorim sa-l primim
		arr1 = malloc((element_count*2) * sizeof(int*));
		MPI_Recv(arr1, element_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); //primim numerele in functie de element_count pe care l-am primit mai sus
		//for (int i = 0; i < element_count; i++)
			//printf("%d ", arr1[i]);
		printf("rank %i : ", rank);
		for (int i = 0; i < element_count; i++)
		{
			int containPrime = 0;
			isPrime = 1;
			if (arr1[i] == 0 || arr1[i] == 1)
				isPrime = 0;
			else {
				for (int j = 2; j <= arr1[i] / 2; ++j) {
					if (arr1[i] % j == 0)
						isPrime = 0;
				}
			}
			if (isPrime)
				printf("%d ", arr1[i]);
		} 
		printf("\n");
		
	}
	free(arr1);
	
	MPI_Finalize();
	return 0;
}
