/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Implementation of the memory area with several types.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

// The length of the header we are using.
#define HLEN 2 * sizeof(int)

/**
 * Implementation of getmem()
 */
void *
getmem(int nc, int ni)
{
  // TODO: Add your code here...
  int fakeNc = nc;
  if ((nc % 4) != 0)
  {
    fakeNc = nc + (4 - (nc % 4)); //bc memory should always be aligned to multiples of 4 bytes
  }
  
  

  int *p = calloc(1, fakeNc * (sizeof(char)) + ni * (sizeof(int)) + HLEN);

  p[0] = nc;
  p[1] = ni;

  return (char *)p + HLEN;
}

/**
 * Implementation of freemem()
 */
void
freemem(void *mem)
{
  // TODO: Add your code here...
  free((char *)mem - HLEN);
}

/**
 * Implementation of getnc()
 */
int
getnc(void *mem)
{
  // TODO: Add your code here...
  char *addr = (char *)mem - HLEN;

  int *header = (int *)addr;

  int numChar = header[0];

  return numChar;
}

/**
 * Implementation of getni()
 */
int
getni(void *mem)
{
  // TODO: Add your code here...
  char *addr = (char *)mem - HLEN;

  int *header = (int *)addr;

  int numChar = header[1];

  return numChar;
}

/**
 * Implementation of getstr()
 */
char *
getstr(void *mem)
{
  // TODO: Add your code here...
  return (char *) mem;
}

/**
 * Implementation of getintptr()
 */
int *
getintptr(void *mem)
{
  // TODO: Add your code here...
  int nc = getnc(mem);
  if ((nc % 4) != 0)
  {
    nc = nc + (4 - (nc%4));
  }

  int *ptr = (int *)((char *) mem + nc);

  return ptr;
}

/**
 * Implementation of getint_at()
 */
int
getint_at(void *mem, int idx, int *res)
{
  // TODO: Add your code here...
  if (idx < 0 || idx >= getni(mem)) return -1;
  int *valptr = getintptr(mem);
  *res = valptr[idx];
  return 0;
}

/**
 * Implementation of setint_at()
 */
int
setint_at(void *mem, int idx, int val)
{
  // TODO: Add your code here...
    if (idx < 0 || idx >= getni(mem)) return -1;
  int *valptr = getintptr(mem);
  valptr[idx] = val;
  return 0;
}

/**
 * Implementation of cpstr()
 */
size_t
cpstr(void *mem, const char *str, size_t len)
{
  // TODO: Add your code here...
  int nc = getnc(mem);

  if (len >= nc)
  {
    memcpy(mem, str, nc-1);
    ((char *)mem)[nc-1] = '\0';
    return nc;
  }
  else{
    memcpy(mem,str,len);
    ((char *)mem)[len] = '\0';
    return len + 1;
  }
}
