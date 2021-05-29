
void * f(int (*p1)[5]);

int main()
{
  int x[5];
  int (*ptr1)[5] = &x;  
  void * ptr2 = f(ptr1);
  int ptr_val1 = (int) ptr1;
  int ptr_val2 = (int) ptr2;
  int diff = ptr_val2-ptr_val1;

    return !(diff==5*sizeof(int));
}
