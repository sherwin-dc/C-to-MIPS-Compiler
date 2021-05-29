
/* Duffs device: https://en.wikipedia.org/wiki/Duff%27s_device */
int f(int count)
{
    int to = 0;
    int n = (count+7) / 8;
    
    switch(count%8)
    {
       case 0: do { to++;
       case 7:      to++;
       case 6:      to++; 
       case 5:      to++;
       case 4:      to++; 
       case 3:      to++; 
       case 2:      to++;
       case 1:      to++;
        } while (--n>0);
    }

    return to;
}