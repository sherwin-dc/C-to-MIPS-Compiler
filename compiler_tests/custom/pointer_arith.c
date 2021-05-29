/* This function takes in a pointer
to an array of 5 ints */
void * f(int (*p1)[5])
{   
    /* Main function checks that the pointer 
    value is increased by 20 */
    return ++p1;
}
