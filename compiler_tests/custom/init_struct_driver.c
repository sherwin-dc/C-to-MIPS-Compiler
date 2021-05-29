struct x {
  double a;
  char b;
  int c;
};

int f(struct x * x3);

int main()
{   
    struct x x3 = {5.5, '/', 1000};

    return !(f(&x3)==1110);
}
