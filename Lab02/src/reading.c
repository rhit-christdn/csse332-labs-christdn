/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Implementation of the memory area with several types.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "reading.h"

int
open_file(const char *name)
{
  return open(name, O_RDONLY);
}

ssize_t
get_file_size(int fd)
{
  struct stat sb;

  if(fstat(fd, &sb) == -1) {
    return -1;
  }

  if(!S_ISREG(sb.st_mode)) {
    return -1;
  }

  return sb.st_size;
}

ssize_t
read_bytes(int fd, char *buf, ssize_t len, size_t incr)
{
  // TODO: Complete this with your instructor

  // we have about 4 million bytes to read
  // we are reading incr bytes at a time

  size_t bytes_read = 0;
  ssize_t total_bytes = 0;

  while ((len > 0) && (bytes_read = read(fd, buf, incr)))
  {
    if (bytes_read == -1)
    {
      if (errno == EINTR)
      {
        //not an issue, continue loop
        continue;
      }
      else
      {
        return -1;
      }
    }
    else
    {
      len -= bytes_read; // update len
      buf += bytes_read; // update buffer up so we write in new memory

      total_bytes += bytes_read;
    }
  }

  return total_bytes;
}

static double
_subtract_timspec(struct timespec t1, struct timespec t2)
{
  struct timespec diff;

  diff.tv_sec  = t1.tv_sec - t2.tv_sec;
  diff.tv_nsec = t1.tv_nsec - t2.tv_nsec;
  if(diff.tv_nsec < 0) {
    // we need to subtract a second out and then adjust the remainder
    diff.tv_nsec += 1000000000;
    diff.tv_sec--;
  }

  return (double)diff.tv_sec + (double)diff.tv_nsec / 1e09;
}

int
_main(int argc, char **argv)
{
  int fd;
  int rc = EXIT_SUCCESS;
  char *endptr;
  ssize_t fsize;
  ssize_t blk              = 1;
  struct timespec ts_start = {0, 0}, ts_end = {0, 0};

  // TODO: Please comment out this line when you implement the last step in
  // this file.
  (void)_subtract_timspec(ts_start, ts_end);

  if(argc > 1) {
    errno = 0;
    blk   = strtoll(argv[1], &endptr, 10);
    if(errno || endptr < (argv[1] + strlen(argv[1]))) {
      fprintf(
          stderr,
          "[ERROR] Argument provided could not be parsed into an integer.\n");
      return EXIT_FAILURE;
    }
  }

  //opening file "large.dat"
  printf("[LOG] Using a chunk size of %ld\n", blk);
  fd = open_file("large.dat");
  if(fd == -1) {
    fprintf(stderr, "[ERROR]: Failed to open large.dat!\n");
    return EXIT_FAILURE;
  }

  // checking if file size is valid
  if((fsize = get_file_size(fd)) == -1) {
    fprintf(stderr, "[ERROR]: Failed to get file size of large.dat!\n");
    close(fd);
    return EXIT_FAILURE;
  }

  //allocate memory to read file
  fprintf(stdout, "[LOG ]: allocating %d bytes of memory.\n", (int)fsize);
  char* buf = malloc(fsize*sizeof(char));

  //just in case we're out of memory
  if (!buf)
  {
    //we've failed to allocate memory

    fprintf(stderr, "[ERROR]: could not allocate enough memory via malloc!\n");
    return EXIT_FAILURE;
  }


  // TODO:
  // =====
  //  Add code here to read all of the bytes of the input file fd.

  // HINT:
  // =====
  // To measure time and print it, use the following:
  //
  // Add #include <time.h> if it's not there.
  //

  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  //
  //   THING YOU'D LIKE TO MEASURE HERE
  //
  // TODO:
  // =====
  //    PLEASE USE THE SAME FPRINTF STATEMENT BELOW AS THE GRADING SCRIPT
  //    DEPENDS ON IT.
  //
  
  //read the file contents into buf
  // read_bytes(intfd, char* buf, ssize_t len, size_t incr)
  ssize_t total = read_bytes(fd, buf, fsize, blk); //how long does this take?
  printf("[LOG ]: read_bytes finished, read %d bytes out of %d bytes\n", (int)total, (int)fsize);


  clock_gettime(CLOCK_MONOTONIC, &ts_end);
  fprintf(stderr, "%lf seconds time elapsed\n",
          _subtract_timspec(ts_end, ts_start));

  close(fd);
  return rc;
}
