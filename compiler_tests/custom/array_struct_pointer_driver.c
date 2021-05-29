struct x {
  double a;
  char b;
  int c[3][4];

  struct y {
    int d;
    double e;
  } ys[2][5];
};

int f(struct x (* ptr)[3][2]);

int main()
{   
  struct x strArr[3][2];
  strArr[2][1].ys[1][4].d = 1000;


    return !(f(&strArr)==1003);
}
