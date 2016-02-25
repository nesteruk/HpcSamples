#include "stdafx.h"

struct Person
{
  string name;
  int age;
  
  friend class boost::serialization::access;

  template <typename Ar> void serialize(Ar& ar, const unsigned int version)
  {
    ar & name;
    ar & age;
  }
};

int main4(int argc, char* argv[])
{
  namespace mpi = boost::mpi;

  mpi::environment env;
  mpi::communicator world;

  if (world.rank() == 0)
  {
    Person p;
    p.name = "Dmitri";
    p.age = 321;
    for (int i = 1; i < world.size(); ++i)
      world.send(i, 0, p);
  }
  else
  {
    Person p;
    world.recv(0, 0, p);
    cout << "Hello, " << p.name << " (" << p.age << ")" << endl;
  }

  return 0;
}

int main2(int argc, char* argv[])
{
  int size, rank;
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    Person p;
    p.name = "Dmitri";
    p.age = 123;
    
    ostringstream ofs;
    boost::archive::text_oarchive ar(ofs);
    ar << p;

    auto& s = ofs.str();
    int len = s.length() + 1;

    for (int i = 1; i < size; ++i)
    {
      MPI_Send(s.c_str(), len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }
  else 
  {
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_CHAR, &count);

    cout << "We have " << count << endl;
    char *buffer = new char[count];

    MPI_Recv(buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    cout << "we got " << buffer << endl;

    istringstream iss(buffer);
    boost::archive::text_iarchive ar(iss);
    Person p;
    ar >> p;

    cout << "Hello, " << p.name << ", you are " << p.age << " years old" << endl;

    delete[] buffer;
  }

  MPI_Finalize();
  return 0;
}

// include 
// need impi.lib;impicxx.lib

int main444(int argc, char* argv[])
{
  cout.setf(ios::fixed);

  int size, rank;
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  cout << "Size is " << size << " rank is " << rank << endl;

  if (rank == 0)
  {
    cout << "Enter a number to see its powers: " << endl;
    int n;
    cin >> n;
    for (int i = 1; i < size; ++i)
      MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  }
  else {
    int n;
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    cout  << setprecision(0) << n << "^" << rank << " = " << pow(n, rank);
    char cpu[256] = {0};
    MPI_Get_processor_name(cpu, &n);
    cout << " (computed by " << cpu << ")" << endl;
  }

  MPI_Finalize();

	return rank == 0;
}

