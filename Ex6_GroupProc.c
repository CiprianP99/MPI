/*Se da un vector cu n elemente. Sa se calculeze suma, produsul, minimul si maximul elementelor, simultan, folosind 4 grupuri de procese. */

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 6


//mpiexec -n 8 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Group orig_group, sum_group, prod_group, min_group, max_group;
	MPI_Comm newComm, sumComm, prodComm, minComm, maxComm;

	int ranks1[2] = { 0, 1 };
	int ranks2[2] = { 2, 3 };
	int ranks3[2] = { 4, 5 };
	int ranks4[2] = { 6, 7 };
	int arr[N] = { 1,2,3,4,5,6 };
	int recvSum = 0;
	int recvProd = 0;
	int recvMin = 0;
	int recvMax = 0;
	MPI_Init(NULL, NULL);
	int sum = 0;
	int product = 1;
	int min;
	int max;
	int sumRank, prodRank, minRank, maxRank;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors

	MPI_Comm_group(MPI_COMM_WORLD, &orig_group);

	MPI_Group_incl(orig_group, 2, ranks1, &sum_group);
	MPI_Group_incl(orig_group, 2, ranks2, &prod_group);
	MPI_Group_incl(orig_group, 2, ranks3, &min_group);
	MPI_Group_incl(orig_group, 2, ranks4, &max_group);

	MPI_Comm_create(MPI_COMM_WORLD, sum_group, &sumComm);
	MPI_Comm_create(MPI_COMM_WORLD, prod_group, &prodComm);
	MPI_Comm_create(MPI_COMM_WORLD, min_group, &minComm);
	MPI_Comm_create(MPI_COMM_WORLD, max_group, &maxComm);

	if(rank==0 || rank==1)
		MPI_Comm_rank(sumComm, &sumRank);
	if (rank == 2 || rank == 3)
		MPI_Comm_rank(prodComm, &prodRank);
	if (rank == 4 || rank == 5)
		MPI_Comm_rank(minComm, &minRank);
	if (rank == 6 || rank == 7)
		MPI_Comm_rank(maxComm, &maxRank);
	
	if (rank == 0)
	{
		for (int i = 0; i < N/2; i++)
			sum = arr[i] + sum;
		//printf("The sum is %d\n", sum);
	}
	if(rank == 1)
	{
		for (int i = N/2; i <= N - 1; i++)
			sum = arr[i] + sum;
		//printf("The sum is %d\n", sum);
	}
	

	if (rank == 2)
	{
		for (int i = 0; i < N/2; i++)
			product = arr[i] * product;
		//printf("The final product is %d\n", product);
	}
	if (rank == 3)
	{
		for (int i = N / 2; i <= N - 1; i++)
			product = arr[i] * product;
	}

	min = max = arr[0];
	if (rank == 4)
	{
		for(int i = 0; i < N/2;i++)
		{
			if (min > arr[i])
				min = arr[i];
		}
		//printf("The minimum number is %d\n", min);
	}
	if (rank == 5)
	{
		for (int i = N / 2; i <= N - 1; i++)
		{
			if (min > arr[i])
				min = arr[i];
		}
	}
	if (rank == 6)
	{
		for (int i = 0; i < N/2; i++)
		{
			if (max < arr[i])
				max = arr[i];
		}
		//printf("The maximum number is %d\n", max);
	}
	if (rank == 7)
	{
		for (int i = N / 2; i <= N - 1; i++)
		{
			if (max < arr[i])
				max = arr[i];
		}
	}

	if (rank == 0 || rank == 1)
	{
		MPI_Allreduce(&sum, &recvSum, 1, MPI_INT, MPI_SUM, sumComm);
		printf("The final sum is %d from comm %d\n", recvSum, sumRank);
	}
	if (rank == 2 || rank == 3)
	{
		MPI_Allreduce(&product, &recvProd, 1, MPI_INT, MPI_PROD, prodComm);
		printf("The final product is %d from comm %d\n", recvProd, prodRank);
	}
	if (rank == 4 || rank == 5)
	{
		MPI_Allreduce(&min, &recvMin, 1, MPI_INT, MPI_MIN, minComm);
		printf("The minimum number is %d from comm %d\n", recvMin, minRank);
	}
	
	if (rank == 6 || rank == 7)
	{
		MPI_Allreduce(&max, &recvMax, 1, MPI_INT, MPI_MAX, maxComm);
		printf("The maximum number is %d from comm %i\n", recvMax, maxRank);
	}
	MPI_Finalize();
}
