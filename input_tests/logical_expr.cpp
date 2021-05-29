int f(){
    int alpha = 2;
    int beta = 2;
    int zero = 0;
    int gamma = alpha^beta;
    gamma = ~beta;
    int theta = !alpha;
    theta = 2||2;
    theta = 2||beta;
    int Hello = alpha&&beta;
    Hello = alpha|beta;
    Hello = alpha&beta;
    if(Hello<theta){
        alpha = 3;
    }
    int a = beta ? gamma : 5; 
}