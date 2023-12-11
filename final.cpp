#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int array_size = 100;
    int array[array_size];

    // Inicializar el array con valores aleatorios 
    if (world_rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < array_size; ++i) {
            array[i] = rand() % 100;
        }
    }

    // Distribuir el array entre los procesos MPI
    MPI_Bcast(array, array_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso MPI suma su parte del array
    int local_sum = 0;
    for (int i = 0; i < array_size / world_size; ++i) {
        local_sum += array[i + world_rank * (array_size / world_size)];
    }

    // Imprimir el trabajo que hace cada proceso en paralelo
    cout << "( Proceso " << world_rank << " Suma parcial: " << local_sum << " ) "<< endl;

    // Recopilar los resultados de cada proceso MPI
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprimir el resultado 
    if (world_rank == 0) {
        cout << "Suma Total: " << global_sum << endl;
    }

    MPI_Finalize();

    return 0;
}
