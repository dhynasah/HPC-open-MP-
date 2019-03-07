#include <iostream>
#include "Matrix.h"
#include <cstdlib>
#include "omp.h"
#include <chrono>


using namespace std;

// Initialization:
Matrix:: Matrix()
{


	// set matrix to 0
	#pragma omp praallel for private(lattice[i][j])
	for(int i=0; i<N; i++){
        for (int j =0; j<N; j++){
            lattice[i][j]=0;
        }
	}

	lattice[N/2][N/2]=1;
   // int  radius = new int [1];


}


  void Matrix:: walkOneParticle () {

//    1. spawn a particle outside the radius of the crystal

    int index = rand() % N;
    int index2 = rand() % N;
 // int  savedPosition[2] = {index,index2};
  int x = lattice[index][index2];
    while(x==1){
    index = rand() % N;
    index2 = rand() % N;
    //savedPosition[2] = {index,index2};
    x = lattice[index][index2];
    }


   /* 2. while particle is on the board and hasn't hit the crystal and moves < max:
        move randomly in a direction
        moves += 1
        */

        // chose random adjacent index


    bool crystal = false;
    while(index<N && index2<N && index >= 0  && index2>=0)
    {
        //#pragma omp critical
        if( index+1<N && lattice[index + 1][index2] == 1 ||
          index2+1<N  && lattice[index][index2 + 1] == 1 ||
           index-1 >= 0 && lattice[index - 1][index2] == 1 ||
           index2-1>=0 && lattice[index][index2 - 1] == 1)
           {
                crystal = true;
               break;
           }

        int move = rand() % 4;

        if (move == 0)
        {
            index++;
        }
        else if (move == 1)
        {
            index--;
        }
        else if (move == 2)
        {
            index2--;
        }
        else if (move == 3)
        {
            index2++;
        }
    }

    //if particle hit crystal it becomes  fixed
    if(crystal == true){
        #pragma omp critical
        lattice[index][index2] = 1;
    }


}
  //}

    void Matrix:: printLattice() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout <<" " << lattice[i][j];
		}
		cout << endl;
	}
}


int main()
{
    Matrix Lattice1;
    int totalNum = 1000;
    // portable timing with chrono
    auto start_time = chrono::high_resolution_clock::now();
            // code to time
   #pragma omp parallel for num_threads(2)
    for(int i= totalNum; i>0; i-- )
    {
        Lattice1.walkOneParticle();
    }

 	auto end_time = chrono::high_resolution_clock::now();
	Lattice1.printLattice();
	cout << endl << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count() << " time" << endl;
	//delete [] radius;
	return 0;
}
