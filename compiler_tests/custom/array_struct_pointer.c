struct x {
  double a;
  char b;
  int c[3][4];

  /* 2D Array of nested structs */
  struct y {
    int d;
    double e;
  } ys[2][5];
};

/* Global 3D Array of Structs */
struct x x1[2][3][4];

/* Function takes in pointer to 2D array of structs */
int f(struct x (* ptr)[3][2]) { 
  
  
  /* Set the value of an element of a multidimensional
  array wihtin the struct, and then retrieve it*/
  x1[1][0][3].c[2][1] = 3;
  int a = x1[1][0][3].c[2][1];

  /* Retrieve the value of an element of a multidimensional array
  of structs, nested inside of another struct, after being given 
  a pointer to a multidimensional array of such structs that have 
  been initialized by GCC */
  int b = (*ptr)[2][1].ys[1][4].d;

  return a+b;
}
