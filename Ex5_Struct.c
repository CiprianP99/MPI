/*Define a type called Student that stores information about students. Create a list of students, and then using n processors, search for one specific student.*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 8


//mpiexec -n 4 PDALab1Ex1.exe
int main(int argc, char* argv[])
{
	int x, low, high;
	int position = -1;
	typedef struct {
		int id;
		int age;
		char name[50];
	}		Student;
	
	const char toFind[] = "Robert";
	int positionIndex = -1;

	Student StudentsBcast[N] = { {1, 21, "Danut"},{5, 20, "Maria"},{3, 20, "Claudiu"},{7, 21, "Dana"},{2, 21, "Mihai"},{6, 22, "Paul"},{4, 19, "David"},{8, 20, "Robert"} };
	MPI_Datatype finaltype, oldtypes[2];
	int blockcounts[2];
	MPI_Aint    offsets[2], lb, extent;

	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //processor id
	MPI_Comm_size(MPI_COMM_WORLD, &size); //number of processors

	//type int
	offsets[0] = 0;
	oldtypes[0] = MPI_INT;
	blockcounts[0] = 2;

	//type char
	MPI_Type_get_extent(MPI_INT, &lb, &extent);
	offsets[1] = 2 * extent;
	oldtypes[1] = MPI_CHAR;
	blockcounts[1] = 50;

	MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, &finaltype);
	MPI_Type_commit(&finaltype);

	MPI_Bcast(StudentsBcast, N, finaltype, 0, MPI_COMM_WORLD);
	
	x = N / size;
	low = rank * x;
	high = low + x;

	for (int i = low; i < high; i++)
	{
		if (strcmp(StudentsBcast[i].name, toFind) == 0)
		{
			positionIndex = i;
		}
	}

	MPI_Reduce(&positionIndex, &position, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		if (position == -1)
		{
			printf("Student not found");
		}
		else
		{
			printf("The student has been found on position:  %d", position);
		}
	}
	// free datatype when done using it
	MPI_Type_free(&finaltype);
	MPI_Finalize();
}
}
