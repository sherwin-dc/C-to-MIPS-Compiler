struct x {
  double a;
  char b;
  int c;

  /* Unnamed nested struct */
  struct {
    int d;
    double e;
    struct x * ptr; /* For linked-list */
  } ys;
};

/* Globally initialise struct */
struct x x1 = {2.5, 'Z', 10};

/* Function takes in start of linked-list */
int f(struct x * x3) {

  struct x x2 = {3.5, 'd' /* 100 */, 50};

  /* Retrieve values from the global and local struct */
  int a = x1.c;
  int b = x2.b;

  /* Retrieve value at end of linked-list */
  struct x * p = x3;
  while (p->ys.ptr) {
    p = p->ys.ptr;
  }
  
  int c = p->ys.d;
  return a + b + c;
}
