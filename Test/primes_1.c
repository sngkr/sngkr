#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../user/user.h"

#define MAX 10
#define FIRST_PRIME 2

int generate_natural();  // -> out_fd
int prime_filter(int in_fd, int prime);  // -> out_fd

int
main(int argc, char *argv[])
{
	int prime;

	int in = generate_natural();
	while (read(in, &prime, sizeof(int))) {
		// printf("prime %d: in_fd: %d\n", prime, in);  // debug
		printf("prime %d\n", prime);
		in = prime_filter(in, prime);
	}

	exit(0);
}

// 生成自然数: 2, 3, 4, ..< MAX
int
generate_natural() {
	int out_pipe[2];

	pipe(out_pipe);

	if (!fork()) {
		for (int i = FIRST_PRIME; i < MAX; i++) {
			write(out_pipe[1], &i, sizeof(int));
		}
		close(out_pipe[1]);

		exit(0);
	}

	close(out_pipe[1]);

	return out_pipe[0];
}

// 素数筛
int
prime_filter(int in_fd, int prime)
{
	int num;
	int out_pipe[2];

	pipe(out_pipe);

	if (!fork()) {
		while (read(in_fd, &num, sizeof(int))) {
			if (num % prime) {
				write(out_pipe[1], &num, sizeof(int));
			}
		}
		close(in_fd);
		close(out_pipe[1]);

		exit(0);
	}

	close(in_fd);
	close(out_pipe[1]);

	return out_pipe[0];
}
// #include <stdio.h>
// #include <stdlib.h>
// int main(void)
// {
//     int i;
//     for(i = 0;i<2; i++)
//     {
//         fork();
//         printf(" - %d %d", getpid(), i);
//     }

//     return 0;
// }

// #include <unistd.h>
// #include <stdio.h>
// int main(void)
// {
//   int i = 0;
//   printf("i son/pa ppid pid  fpid\n");
//   //ppid指当前进程的父进程pid
//   //pid指当前进程的pid,
//   //fpid指fork返回给当前进程的值
//   printf("当前进程PID：%4d\n", getppid());
//   printf("当前进程ID：%4d\n", getpid());
//   for (i = 0; i < 2; i++)
//   {
//     pid_t fpid = fork();
//     if (fpid == 0)
//       {
//         printf("%d child  %4d %4d %4d", i, getppid(), getpid(), fpid);
//         printf("\n");
//       }
//     else
//       {
//         printf("%d parent %4d %4d %4d", i, getppid(), getpid(), fpid);
//         printf("\n");
//       }
//   }
//   return 0;
// }
// #include <stdio.h>
// #include <unistd.h>
// int main(void)
// {
//   printf("one");
//   fork();
//   printf("two\n");
//   return 0;
// }
