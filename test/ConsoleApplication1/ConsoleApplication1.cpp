// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <iostream>

int main()
{
    int a[9] = { -1 };
    //const char *s = "f 2014/7794/1996 2017/7797/1998 7334/7795/7330";
    //sscanf_s(s, "f%d/%d/%d%d/%d/%d%d/%d/%d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8]);

    const char *s = "f 3442//4986 3391//4986 3389//4986";
    sscanf_s(s, "f%d//%d%d//%d%d//%d", &a[0], &a[2], &a[3], &a[5], &a[6],  &a[8]);


}

