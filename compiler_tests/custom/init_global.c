char c = 'a';
char x = 'M';

char f()
{
    char * c0 = &c;
    return *(c0+1);
}
