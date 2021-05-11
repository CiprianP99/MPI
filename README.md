# MPI
### Here I will list the following exercises that I implemented on my PDA laboratory
1. Ex1 Write a program that prints all the prime numbers less than N using M processes.
1. Ex2 Write a program that searches an element inside an array and prints its position in case the array contains that element, otherwise it prints 'Not found.'
1. Ex3 Let there be n processes. Each process generates m (m>=100) random numbers ( <=1000 ), prints them to the console, computes their sum and prints it to the console. Determine the time it takes for each process to complete the job.
1. Ex4 Write a program that searches an element inside an array. 
   - A.Use MPI_Broadcast for sending the array. If the element is found, print the maximum
position index. For computing the maximum position, you need to use MPI_Reduce
   - B.Use scatter for sending the array. If the element is found many times, print all its positions.
Use MPI_Gather for sending back the positions
1. Ex5 Define a type called Student that stores information about students. Create a list of students, and
then using n processors, search for one specific student.
1. Ex6 Given a n elements array, perform sum, product, min and max of the elements, simultaneous, using 4 groups of processes.
.
