/* equivalent to `typedef int a` */
int typedef a;

typedef struct x {
  a my_ptr;
} b;

int f() {
  b my_struct;
  my_struct.my_ptr = 500;

  return my_struct.my_ptr;
}