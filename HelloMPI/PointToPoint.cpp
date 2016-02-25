#include "stdafx.h"

int main123(int argc, char *argv[])
{
	int size, rank;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		// give time for the recepient to set up
		//this_thread::sleep_for(chrono::seconds(1));

		int value = 42;
		for (int i = 1; i < size; i++)
		{
			cout << "Ready to send " << rank << "-->" << i << endl;
			auto result = MPI_Ssend(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      cout << "Data send     " << rank << "-->" << i << endl;
		}
	} 
	else
	{
		this_thread::sleep_for(chrono::seconds(1));

		int value = -1;
		//MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//cout << rank << " received from 0 the value " << value << endl;
	}

	MPI_Finalize();

	return 0;
}