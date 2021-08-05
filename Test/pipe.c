
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../user/user.h"

int test()
{
  int p[2];
  pipe(p);
  char str[] = "Hello World!";
  if (!fork())
  {
    write(p[1], str, sizeof(str));
    printf("test() father：write over\n");
    printf("p[1]: %d\n", p[1]);
    close(p[1]);
    printf("test() father：close(p[1])\n");
    wait(0);
    exit(0);
  }
  else
  {
    printf("test() son: close(p[1])\n");
    close(p[1]);
    printf("test() son: return p[0]\n");
      close(p[0]);
    return p[0];
  }
}
int main()
{

  int fd = test();
  printf("return : %d\n",fd);
  char result[20];
  read(fd,result,12 );
  // close(fd);
  printf("%s\n",result);
  exit(0);
}
