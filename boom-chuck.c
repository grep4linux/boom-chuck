/*  Decription: System load generator utility
*   Author: Raine Curtis 
*   Complier: gcc
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/statvfs.h>

#define APPVER 1.1
#define NUM_OF_CORES 8
#define MAX_PRIME 100000


/* 
 * FUNCTIONS
 */

void show_header() {
    printf("### Boom chuck system load generator ##########################\n");
    printf("#   by Raine Curtis\n");
    printf("#   Note: Use responsibily!\n");
    printf("#   No guarantees or warranties, \n");
    printf("#    or responsibilities are provided by the author.\n");
    printf("#   v. %2.1f \n", APPVER);
    printf("#########################################################\n");
}

void do_primes() {
    unsigned long i, num, primes = 0;
    printf("Gerating primes\n");
    for (num = 1; num <= MAX_PRIME; ++num) {
        for (i = 2; (i <= num) && (num % i != 0); ++i);
        if (i == num)
            ++primes;
    }
    printf("Calculated %d primes.\n", primes);
}

void show_limit_settings() {
    printf("===================================================\n");
    printf("You can limit this utility by limiting resources\n");
    printf("Edit /etc/security/limits.conf\n");
    printf("Add the following line:\n");
    printf("\n");
    printf("  <user_name> hard nproc 10\n");
    printf("Or you can use ulimit, view by running 'ulimit -a' \n");
    printf("===================================================\n");

}

/*
 * Main (start of execution)
 *
 */

int main(int argc, char *argv[]) {
    show_header();
    printf("Arguments: %d\n", argc);
    for (int i=0; i < argc; i++) {
            printf("%i -  %s \n", i, argv[i]);
    }
    show_limit_settings();
    if (argc == 1 ) {
        printf("Usage: boom-chuck <load generator> \n");
        printf("       boom-chuck net <site> \n");
        printf("       boom-chuck cpu \n");
        printf("       boom-chuck mem \n");
        printf("       boom-chuck disk\n");
        
    }
    else {
        if (argc > 1 ) {
            printf("# --> %s \n", argv[1]);
            if ( strcmp(argv[1], "net" ) == 0) {
                printf("Running network load generator\n");
                if (argc == 3) {
                char syscmd[2048];
                strcpy(syscmd, "wget  --no-http-keep-alive --no-cache --no-cookies --header=\"Accept: text/html\" --user-agent=\"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.8; rv:21.0) Gecko/20100101 Firefox/21.0\" -O /dev/null");
                strcat(syscmd, " ");
                strcat(syscmd, argv[2]);
                int cpid[512];
                for (int k = 1; k < 8; k++) {
                    for (int j = 1; j <= 2; j++) {
                        cpid[j] = fork();
                        printf("# Parent pid: %d \n", getppid());
                        printf("# |--Child pid: %d \n", getpid());
                        printf("CMD: %s\n", syscmd);
                        system(syscmd);
                    }
                    while(wait(NULL) != -1 || errno != ECHILD) {
                        printf("Child processes to finished\n");
                    }
                }
                printf("Sleeping\n");
                sleep(1);
                }
            }
            else if (strcmp(argv[1], "cpu") == 0) {
                printf("Running cpu load generator\n");
                time_t start, end;
                time_t run_time;
                unsigned long i;
                pid_t pids[NUM_OF_CORES];

                /* start of test */
                start = time(NULL);
                for (i = 0; i < NUM_OF_CORES; ++i) {
                    if (!(pids[i] = fork())) {
                        do_primes();
                        exit(0);
                    }
                    if (pids[i] < 0) {
                        perror("Fork");
                        exit(1);
                    }
                }
                for (i = 0; i < NUM_OF_CORES; ++i) {
                    waitpid(pids[i], NULL, 0);
                }
                end = time(NULL);
                run_time = (end - start);
                printf("This machine calculated all prime numbers under %d %d times "
                       "in %d seconds\n", MAX_PRIME, NUM_OF_CORES, run_time);
            }
            else if (strcmp(argv[1], "mem") == 0) {
                printf("Running memory load generator\n");
                printf("Forking processes to generate random numbers\n");
                int cpid[512];
                for (int k = 1; k < 12; k++) {
                    for (int j = 1; j <= 2; j++) {
                        cpid[j] = fork();
                        printf("# Parent pid: %d \n", getppid());
                        printf("# |--Child pid: %d \n", getpid());
                        system("echo \"Random number:  $RANDOM\"");
                        sleep(1);
                    }
                    while(wait(NULL) != -1 || errno != ECHILD) {
                        printf("Child processes to finished\n");
                    }
                }
            }    
            else if (strcmp(argv[1], "disk") == 0) {
                printf("Running disk load generator\n");
                for (int i = 1; i <= 12; i++) {
                    system("dd if=/dev/zero of=/tmp/test1.img bs=500M count=1 oflag=dsync");
                    system("cat /tmp/test1.img > /dev/null");
                    system("rm /tmp/test1.img");
                }
                
            }
        }
    }
    return 0;
}
