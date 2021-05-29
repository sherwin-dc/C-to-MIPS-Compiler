
/* Function takes in 
pointer to 6-D array */
int f(int (*ptr)[2][1][2][1][1][2]) {

  /* Declare and set the value of
  an element in a 12-D array */
  int x[4][2][1][2][1][4][2][2][1][4][2][3];
   3[x][1][0][1][0][2][0][1][0][2][1][2] = 20;

  /* Retrieve values from the array declared 
  locally and the array initialised by GCC */
  return 1[*ptr][0][1][0][0][1] + x[3][1][0][1][0][2][0][1][0][2][1][2];
}
