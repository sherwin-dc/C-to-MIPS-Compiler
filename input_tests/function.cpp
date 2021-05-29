int my_fn(int a, int b, int c, int d){
    return a + 1;
}

int random_fn(int a);

int main(){
    int b = 5;
    int result = my_fn(b,b ,b, b);
    int hello = random_fn(b);
    return 0;
}

int random_fn(int a){
    return a+2;
}