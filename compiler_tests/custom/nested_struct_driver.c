struct x {
  double a;
  char b;
  int c;

  struct {
    int d;
    double e;
    struct x * ptr;
  } ys;
};

int f(struct x * x3);

int main()
{   
    /* Create a linked-list */

    struct x l0 = {5.5, '/', 1000}; l0.ys.ptr = 0;
    l0.ys.d = 5000;
    struct x l1; l1.ys.ptr = &l0;
    struct x l2; l2.ys.ptr = &l1;
    struct x l3; l3.ys.ptr = &l2;
    struct x l4; l4.ys.ptr = &l3;
    struct x l5; l5.ys.ptr = &l4;
    struct x l6; l6.ys.ptr = &l5;
    struct x l7; l7.ys.ptr = &l6;
    struct x l8; l8.ys.ptr = &l7;
    struct x l9; l9.ys.ptr = &l8;
    struct x l10; l10.ys.ptr = &l9;
    

    return !(f(&l10)==5110);
}
