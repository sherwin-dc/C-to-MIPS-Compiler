struct x {
  double a;
  char b;
  int c;
};

struct x x1 = {2.5, 'Z', 10};

int f(struct x * x3) {
  struct x x2 = {3.5, 'd' /* 100 */, 50};
  return x1.c + x2.b + x3->c;
}
