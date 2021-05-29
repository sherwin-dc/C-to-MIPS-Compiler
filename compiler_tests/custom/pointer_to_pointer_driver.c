
float f(float ************a12, float **b2);

int main()
{
  float a = 5.5f;           float b = 4.5f;
  float *a1 = &a;           float *b1 = &b;
  float **a2 = &a1;         float **b2 = &b1;
  float ***a3 = &a2;
  float ****a4 = &a3;
  float *****a5 = &a4;
  float ******a6 = &a5;
  float *******a7 = &a6;
  float ********a8 = &a7;
  float *********a9 = &a8;
  float **********a10 = &a9;
  float ***********a11 = &a10;
  float ************a12 = &a11;
    return !(f(a12, b2)==10.0f);
}
