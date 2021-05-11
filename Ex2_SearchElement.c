/*Write a program that searches an element inside an array and prints its position in case the array contains that element, otherwise it prints 'Not found.' */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 20
#define BUF 5
int data[N];
//mpiexec -n 6 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	
	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors
	
	int numberToFind = 10;
	int numbersToGenerate;
	int* arr1;
	int arr2[BUF];
	int arr3[BUF];
	int posArr[BUF];
	int notFound = -1;

	if (rank == 0) {
		arr1 = malloc((N * 2) * sizeof(int*)); //allocating memory for our array
		srand(time(NULL));
		printf("Array before splitting: ");
		for (int i = 0; i <= N - 1; i++) {  //generating random numbers between 5 and 15
			arr1[i] = (rand()% (15-5+1)+5);
			printf("%d ", arr1[i]);
		}
		printf("\n------------------------\n");
		int elements = 0;
		int slaves = size - 1; //setting the number of total slaves
		if (slaves != 0)
		{
			elements = N / slaves; 
		}
		for (int i = 1; i < size; i++) {
			int start = (i - 1) * elements;
			int end = start + elements - 1;

			//if (i == 1) //the case for the first slave
			//{
			//	start = 0;
			//	end = i * (elements - 1);
			//}
			if (i == (size - 1)) //the case for the last slave
				end = N - 1;

			int element_count = end - start + 1; //countint the elements we have to send
			MPI_Send(&element_count, 1, MPI_INT, i, 1, MPI_COMM_WORLD); //sedning the number of elements we want to send
			MPI_Send((arr1 + start), element_count, MPI_INT, i, 1, MPI_COMM_WORLD); //sendomg the elements by the element_count

		}
		int var; //variable for storing the received number of elements
		for (int i = 1; i < size; i++) {
			MPI_Recv(&var, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status); //receiving from slaves the number of position found
			if (var == notFound)
				printf("Element not found in the process %d \n", i);
			else {
				printf("Element/s found in the process %d :", i);
				MPI_Recv(&arr3, var, MPI_INT, i, 1, MPI_COMM_WORLD, &status); //receiving from slaves the array with the found positions
				for (int j = 0; j < var; j++)
					printf("pos %d ", arr3[j]);
				printf("\n");
			}
		} 
	}

	
	else {
		printf("From rank %d :", rank);
		int element_count;
		MPI_Recv(&element_count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); //receiving the number of elements we have to receive
		arr1 = malloc((element_count * 2) * sizeof(int*));
		MPI_Recv(arr1, element_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); //receiving the elements by the element_count we just received above

		for (int i = 0; i < element_count; i++)
			printf("%d ", arr1[i]);

		int k = 0;
		for (int i = 0; i < element_count; i++) {
			if (arr1[i] == numberToFind) {
				arr2[k] = i;
				k++; //incrementing when element is found
			}
		}
		
		if (k == 0)//if k is unmodified
			MPI_Send(&notFound, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); //sending to master -1 if not found
		else {
			MPI_Send(&k, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); //sending to master the number of positions

			for (int i = 0; i < k; i++)
				posArr[i] = arr2[i] + (rank - 1) * element_count;
			
			MPI_Send(&posArr, k, MPI_INT, 0, 1, MPI_COMM_WORLD); //sending to master the positions
		}
			printf("\n");
	}
	free(arr1);

	MPI_Finalize();
	return 0;
}
