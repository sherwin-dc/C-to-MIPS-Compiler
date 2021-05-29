int f()
{
    int a; int b; int c;
    a=10;
    b = ++a; /* b should be 11 */
    a=100;
    c = a++; /* c should be 100 */

    return b+c;
}
