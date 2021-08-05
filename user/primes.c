#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../user/user.h"

int GenerateNatural(int);  // 返回生成自然数序列的管道: 2, 3, 4, ...
int PrimeFilter(int, int); // 管道过滤器: 删除能被素数整除的数
//切莫过于专注进程细节，注重管道的创建和关闭
int main(int argc, char *arg[])
{
  int Read, prime;
  Read = GenerateNatural(32);
  while (read(Read, &prime, sizeof(int)))
  {
    printf("prime %d\n", prime);
    Read = PrimeFilter(Read, prime);
  }
  exit(0);
}

int GenerateNatural(int num)
{
  int w_pipe[2];
  pipe(w_pipe);

  if (!fork())
  {
    for (int i = 2; i <= num; i++)
    {
      write(w_pipe[1], &i, sizeof(int));
    }
    close(w_pipe[1]);
    exit(0);
  }

  close(w_pipe[1]);
  return w_pipe[0];
}

int PrimeFilter(int Read, int prime)
{
  int r_pipe[2];
  int temp;
  pipe(r_pipe);
  if (!fork())
  {
    while (read(Read, &temp, sizeof(int)))
    {
      if (temp % prime)
      {
        write(r_pipe[1], &temp, sizeof(int));
      }
    }
    close(Read);
    close(r_pipe[1]);
    exit(0);
  }
  close(Read);
  close(r_pipe[1]);
  return r_pipe[0];
}
// #define STDDER_FILENO 2

// #define READEND 0
// #define WRITEEND 1

// typedef int pid_t;

// int main(void)
// {
//   int numbers[36], fd[2];
//   int i, index = 0;
//   pid_t pid;
//   for (i = 2; i <= 35; i++)
//   {
//     numbers[index++] = i;
//   }
//   while (index > 0)
//   {
//     pipe(fd);
//     if ((pid = fork()) < 0)
//     {
//       fprintf(STDDER_FILENO, "fork error\n");
//       exit(0);
//     }
//     else if (pid > 0)
//     {
//       close(fd[READEND]);
//       for (i = 0; i < index; i++)
//       {
//         write(fd[WRITEEND], &numbers[i], sizeof(numbers[i]));
//       }
//       close(fd[WRITEEND]);
//       wait((int *)0);
//       exit(0);
//     }
//     else
//     {
//       close(fd[WRITEEND]);
//       int prime = 0;
//       int temp = 0;
//       index = -1;

//       while (read(fd[READEND], &temp, sizeof(temp)) != 0)
//       {
//         // the first number must be prime
//         if (index < 0)
//           prime = temp, index++;
//         else
//         {
//           if (temp % prime != 0)
//             numbers[index++] = temp;
//         }
//       }
//       printf("prime %d\n", prime);
//       // fork again until no prime
//       close(fd[READEND]);
//     }
//   }
//   exit(0);
// }

//_____________________________
// void child_process(int p[2])
// {
//   int len;
//   int prime;
//   int num;
//   int pp[2];
//   // no need to write to left neighbor
//   close(p[1]);
//   len = read(p[0], &prime, sizeof(int));
//   if (len == 0)
//   {
//     close(p[0]);
//     exit(0);
//   }
//   printf("prime %d\n", prime);
//   pipe(pp);
//   // create right neighbor
//   if (fork() == 0)
//   {
//     // right neighbor don't need p
//     close(p[0]);
//     child_process(pp);
//   }
//   else
//   {
//     // no need to read from right
//     close(pp[0]);
//     while (1)
//     {
//       // read from left neighbor
//       len = read(p[0], &num, sizeof(int));
//       if (len == 0)
//       {
//         break;
//       }
//       // filter the number read
//       if (num % prime != 0)
//       {
//         // write to right neighbor
//         write(pp[1], &num, sizeof(int));
//       }
//     }
//     close(p[0]);
//     close(pp[1]);
//     wait(0);
//   }
//   exit(0);
// }
// int main(int argc, char *argv[])
// {
//   int i;
//   int p[2];
//   if (argc > 1)
//   {
//     fprintf(2, "usage: primes\n");
//     exit(1);
//   }
//   pipe(p);
//   // first round
//   // generate 2 through 35 to child process
//   if (fork() == 0)
//   {
//     child_process(p);
//   }
//   else
//   {
//     // no need to read from right
//     close(p[0]);
//     for (i = 2; i <= 35; i++)
//     {
//       write(p[1], &i, sizeof(int));
//     }
//     close(p[1]);
//     wait(0);
//   }
//   exit(0);
// }
