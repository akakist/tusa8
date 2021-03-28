
#include <string>
#include <map>

int main()
{
    struct f
    {
        int gender:2;
        int voice:1;
    };
    f a;
    memset(&a,0,sizeof(a));
//	a=0;
    a.gender=2;
    a.voice=1;
    printf ("%d %d\n",a,sizeof(a));
    return 0;

}
