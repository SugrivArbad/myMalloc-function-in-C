# myMalloc-function-in-C
Here I have written ymalloc() function which behaves like a malloc() function in C lagauage.
I have also created yfree() function which frees up memory allocated by ymalloc() function. 
It behaves like a free() function in C.  

Note yalloc() abd ymalloc() functions are different.

Usage : 
1] allocate memory in bulk using yalloc(int nb) function which you find is enough for the lifetime of your program
      nb = no. of Bytes
2] Now you can call ymalloc() function to allocate memory from this bulk
3] use yfree() whenever you need
