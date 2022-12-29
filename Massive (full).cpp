#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cstdio>
#include <math.h>
using namespace std;

int i, j, h, numb = 0, np, id, stp, mod, endp;

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
	x[q][p]=1;
      	printf("%d\t", x[q][p]);
    }
    printf("\n");
  }
}

void Printres(int** z)
{
    printf("Result:\n");
    for(int g = 0; g < i; g++)
       for(int d = 0; d < 1; d++)
	  printf("%d \n", z[g][d]);
}

void multi(int** x, int** y, int** z)
{
  	for(int g=0; g< i; g++)
        {
            for (int f = 0; f < 1; f++)
            {
      	       z[g][f] = 0;
      	       for (int k = 0; k < j; k++)
                   z[g][f] += x[g][k] * y[k][f];
     	    }
        }	 
}

void ParallelFor (int** x, int** y, int** z)
{
    //Устанавливаем число потоков
    //int threadsNum = 4;
    //omp_set_num_threads(threadsNum);
	#pragma omp parallel shared(x, y, z) private(i, j, h)
	{
		#pragma omp for
		for ( int g = 0; g < i; g++) 
	   	{
       		    for ( int f = 0; f < 1; f++)
		    {
                    	z[g][j] = 0;
                        for (int k = 0; k < j; k++)
		        {
                            z[g][f] += (x[g][k] * y[k][f]);
            	        }
        	    }
    	        }	
	 } 	   
	
}

void Parallel (int** x, int** y, int** z)
{

        for ( int g = 0; g < i; g++ )
	  for ( int d = 0; d < 1; d++ )
             z[g][d]=0;
	#pragma omp parallel private(id) shared(h,j, i)
	{
	    #pragma omp single
	    {
		getr_rows_for_thread(omp_get_num_threads(), i);
	    }

	    id = omp_get_thread_num();
	    startend_for_thread(id);
		
	    for(int u=stp; u<endp; u++)
	    {
	       for(int d = 0; d < 1; d++ )
		 for(int v=0; v<j; v++)
		     z[u][d]=z[u][d]+x[u][v]*y[v][d];
	    }
        }    
}


int main ()
{
    srand(time(0));

    // Задаём размеры массива и вектора
    printf("Enter size for matrix: \n");
    printf("Row: ");
    scanf("%d", &i); //строка
    printf("Column: ");
    scanf("%d", &j); //столбец
    printf("\n");

    printf("Enter size for vector: ");
    scanf("%d", &h); //	
    printf("\n");

    // Выделение памяти для массива и вектора
    int **a = new int* [i];
    for (int cn = 0; cn < i; cn++)
        a[cn] = new int [j];
	
    int **b = new int* [h];
    for (int cn = 0; cn < h; cn++)
        b[cn] = new int [1];

    int **c = new int* [i];
    for (int cn = 0; cn < i; cn++)
        c[cn] = new int [1];	

    printf("Elements of matrix: \n");
    printMatrix(a, i, j);
    printf("\n");

    printf("Elements of vector: \n");
    printMatrix(b, h, 1);
    printf("\n");

    if(j==h)
    {   
	//while (numb != 4)
	//{	
	    printf("\n");
	    printf("Perform matrix-vector multiplication: \n1.Consistently \n2.Using the directive 'parallel' and 'for' \n3.Using the directive 'parallel' \n4.End run\n");
    	    printf("Your choice: ");
    	    scanf("%d", &numb);
            printf("\n");

    	    switch(numb)
    	    {
		case 1:
		{	
		   multi (a, b, c);
		   Printres(c);
		   break;
		}

		case 2:
		{
		    #pragma omp parallel //shared(a, b, c) private(i, j, h)
		    {
			#pragma omp for
		        for ( int g = 0; g < i; g++) 
	   	        {
       		           for ( int f = 0; f < 1; f++)
		           {
                    	      c[g][j] = 0;
                              for (int k = 0; k < j; k++)
		              {
                                 c[g][f] += (a[g][k] * b[k][f]);
            	              }
        	           }
    	                }	
	            } 	   
		    // ParallelFor(a, b, c);
		     Printres(c);
		     break;
		}
		     
		case 3:
		{
                     Parallel(a, b, c);
		     Printres(c);
		     break;
		}

		case 4:
		     break;
    	    }
     	//}
    }
    	
     else
	printf("Multi matrix and vector is impossible \n");

// Освобождение памяти динамического массива
   for (int cn = 0; cn < i; cn++)
        delete []c[cn];	
    printf("\n");
    for (int cn = 0; cn < i; cn++)
        delete []a[cn];
    for (int cn = 0; cn < h; cn++)
        delete []b[cn];	
    return 0; 

}
