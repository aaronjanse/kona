#include "incs.h"
#include "0.h"
#include "getline.h"


#include <termios.h>
#include <stdio.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

//Based on BSD's getdelim.c - [BSD License] (c) 2009 David Schultz <das@FreeBSD.org>

I expander(S *s, I n) //grow s? n - needs size
{
  S t; I q;

  //XXX: these lines are deactivated since dlmalloc was removed. the q=n line bypasses (delete if reactivated)
  //this lightly assumes realloc is efficient (nlogn) for shrinking and appending (nice when true)
  //if(n <= malloc_usable_size(*s)) R 0;
  //q = rp2(n);
  q = n;
  //

  t = realloc(*s, MAX(1,q)); //if you want to mremap this still have to avoid malloc_useable_size above... actually, maybe better to not pass mmapped here
  if(!t){ME; R -1;} //mm/o - failed
  *s=t;
  R 0;
}

I appender(S *s, I *n, S t, I k) //concatenate t to s
{
  if(expander(s,*n+k+1))R -1; //mm/o - failed
  memcpy(*s+*n,t,k);
  *n += k;
  (*s)[*n] = '\0';
  R 0;
}

I getline_(S *s,I *n,FILE *f){

  // s = "1+1\n";
  // fprintf(stderr, "plz type char:");
  // fflush(stderr);
  // char c = getche();
  // if (c == 'x') {
  //   exit(0);
  // }
  // fprintf(stderr, "\nYou typed: %c\n", c);
  // fprintf(stdin, "1+1\n");
  // printf()
  // O("haha n (b4): %i\n", n);

  // printf("n is: %i\n", n);
  // int nnn = n;
  int x = getdelim_(s,n,'\n',f);
  // n = s + x;
  // printf("len is: %d\n", s+x);
  // printf("len is: %d\n", n);
  // strdupn("1+1",s);
  // printf("s[0] is: [%s]\n", s[0]);
  // printf("s[1] is: %x\n", s[1]);
  // *s = 
  // n = 5;

  // s[0] = "1+1\n\0";
  // n = s+5;

  // O("haha x: %i\n", x);
  // O("haha n: %i\n", n);
  // int idx = 0;
  // s[idx] = '1'; idx++;
  // s[idx] = '1'; idx++;
  // s[idx] = '\n'; idx++;
  // s[idx] = '\0'; idx++;
  // n = s+idx;
  return x;
  // s = "1+1\n";
  // return 5;

  // R getdelim_(s,n,'\n',f);
}

// char *history[] = [];
// std::vector<char> hist = {};
// x.push_back("d");


int history_pos = -1;


I getdelim_(S *s,I *n,I d,FILE *f)
{
  S z;
  // getdelim(s,&o,d,f);
  // *s = "7+7";
  // printf("plz type char:");
  // // fflush(stdout);
  // char c = getch();
  // if (c == 'x') {
  //   exit(0);
  // }
  // printf("\nYou typed: %c\n", c);
  // printf("plz type char:");
  // // fflush(stdout);
  // c = getch();
  // if (c == '\n') {
  //   exit(0);
  // }
  // printf("\nYou typed: %c\n", c);
  // fflush(stdout);
  int bufsize = 50;

  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  while (1) {
    // Read a character
    c = getch();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      break;
    } else if (c == '\033') { // if the first value is esc
      // getch(); // skip the [
      // switch(getch()) { // the real value
      //     case 'A':
      //         printf("UP ARROW");
      //         break;
      //     case 'B':
      //         // code for arrow down
      //         break;
      //     case 'C':
      //         // code for arrow right
      //         break;
      //     case 'D':
      //         // code for arrow left
      //         break;
      // }
    } else {
      buffer[position] = c;
      history_pos = -1;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += 50;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  // *n=o;
  z=strdupn(buffer, position);
  free(*s);
  *s=z;
  return position;
}

#if defined(__MACH__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1070
I getline(S *s,size_t*n, FILE *f){ R getdelim(s,n,'\n',f);}
I getdelim(S *s,size_t*n, I d, FILE *f)//target, current capacity, delimiter, file
{
  unsigned char *q;
  I w=0;

  flockfile(f);
  //ORIENT(f,-1)  //is this dangerous?

  if (!s) {errno = EINVAL; goto error;}

  if (f->_r <= 0 && __srefill(f))
  {
    /* If f is at EOF already, we just need space for the NUL. */
    if (__sferror(f) || expander(s, 1)) goto error;
    funlockfile(f);
    (*s)[0] = '\0';
    R *n=-1;
  }


  while ((q = memchr(f->_p, d, f->_r)) == NULL)
  {
    if (appender(s, &w, (S) f->_p, f->_r)) goto error;
    if (__srefill(f))
    {
      if (__sferror(f)) goto error;
      goto done;  /* hit EOF */
    }
  }
  q++;  /* snarf the delimiter, too */
  if (appender(s, &w, (S) 'a', q - f->_p)) goto error;
  f->_r -= q - f->_p;
  f->_p = q;

  done:
    /* Invariant: *s has space for at least w+1 bytes. */
    (*s)[w] = '\0';
    funlockfile(f);
    R *n=w;

  error:
    f->_flags |= __SERR;
    funlockfile(f);
    R *n=-1;
}
#endif

#ifdef WIN32
ssize_t getline (S *s, size_t *n, FILE *f){ R getdelim(s,n,'\n',f);}
ssize_t getdelim (S *s, size_t *n, int d, FILE *f){   //target, current capacity, delimiter, file
#if 0 
  // this code is MSVC runtime version specific
  char *q; I w=0;
  if (!s) {errno = EINVAL; goto error;}
  if (f->_cnt <= 0) {
    if (expander(s, 1)) goto error;
    (*s)[0] = '\0'; R *n=-1;
  }
  while ((q = memchr(f->_ptr, d, f->_cnt)) == NULL) {
    if (appender(s, &w, (S) f->_ptr, f->_cnt)) goto error;
    goto done;  /* hit EOF */
  }
  q++;  /* snarf the delimiter, too */
  if (appender(s, &w, (S) f->_ptr, q - f->_ptr)) goto error;
  f->_cnt -= q - f->_ptr; f->_ptr = q;
#endif
  I w=0;
  if (!s) {errno = EINVAL; goto error;}
  for(;;) {
    C c=fgetc(f);
    if (EOF == c) R -1;
    if (appender(s, &w, (S)&c, 1)) goto error;
    if (d==c) break;
  }
  (*s)[w] = '\0'; R *n=w;
  error: R *n=-1;
}
#endif
