#include "../symtab/object_data.h"
#include "../symtab/jsonapi.h"
#include <ctype.h>
#include <stdio.h>

int isNumber(char*numbers)
{
    int i, correctNum = 1;
    if (strlen(numbers) == 0) return 0;
    for(i = 0 ; i < strlen(numbers) ; ++i)
    {
        if(!isdigit(numbers[i]))
        {
            correctNum = 0;
            break;
        }
    }

    if(correctNum == 0) return 0;
    return 1;
}
int main()
{
    int a = isNumber("");
    printf("%d", a);
    return 1;
}