#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cstdio>
#include <math.h>
using namespace std;

int i,j,h,np,id, stp, mod, endp;

void getr_rows_for_thread(int getnum, int x)
{
	 np=i/getnum;
	 mod=i%getnum;
}

void startend_for_thread(int getid)
{
    if(getid < mod)
      {
          stp = getid * np + getid;
          endp = np + stp + 1;
      }
      else
      {
         stp = getid * np + mod;
         endp = np + stp;
      }
}

void printMatrix(int** x, int n, int m)
{
  for (int q = 0; q < n; q++)
  {
    for (int p = 0; p < m; p++)
    {
	x[q][p]=rand() % 30 +1;
      	printf("%d\t", x[q][p]);
    }
    printf("\n");
  }
}

int main() 
{
    srand(time(0));

    // Задаём размеры массива и вектора
    printf("Enter size for matrix: \n");
    scanf("%d", &i); //строка
    scanf("%d", &j); //столбец
    printf("\n");

    printf("Enter size for vector: \n");
    scanf("%d", &h); //
    printf("\n");

    // Выделение памяти для массива и вектора
    int **a = new int* [i];
    for (int cn = 0; cn < i; cn++)
        a[cn] = new int [j];
	
    int **b = new int* [h];
    for (int cn = 0; cn < h; cn++)
        b[cn] = new int [1];

    //Вывод массива и вектора
    printf("Elements of matrix: \n");
    printMatrix(a, i, j);
    printf("\n");

    printf("Elements of vector: \n");
    printMatrix(b, h, 1);
    printf("\n");

    // Произведение матрицы и вектора
    printf("Result:\n");
    if(j==h)
    {
	    int **c = new int* [i];
    		for (int cn = 0; cn < i; cn++)
        		c[cn] = new int [1];	
	
	    for ( int g = 0; g < i; g++ )
	 	    for ( int d = 0; d < 1; d++ )
		        c[g][d]=0;

	    #pragma omp parallel private(id) shared(h,j, i)
	    {
		    #pragma omp single
		    {
			    getr_rows_for_thread(omp_get_num_threads(), i);
		    }

		    id = omp_get_thread_num();
		    startend_for_thread(id);
		
		    for(int x=stp; x<endp; x++)
		    {
	            	for(int d = 0; d < 1; d++ )
		            for(int v=0; v<j; v++)
				c[x][d]=c[x][d]+a[x][v]*b[v][d];
		    }
        }    
	// Вывод результата
	    for(int g = 0; g < i; g++)
	        for(int d = 0; d < 1; d++)
		        printf("%d \n", c[g][d]);

	// Освобождение памяти динамического массива
	    for (int cn = 0; cn < i; cn++)
             delete []c[cn];
	
	}
	else
	    printf("Multi matrix and vector is impossible \n");

// Освобождение памяти динамического массива
    printf("\n");
    for (int cn = 0; cn < i; cn++)
        delete []a[cn];
    for (int cn = 0; cn < h; cn++)
        delete []b[cn];	
    return 0; 
}