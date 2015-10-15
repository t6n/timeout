#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
usage(void)
{
	fprintf(stderr, "usage: timeout -t timeout command\n");
	exit(EXIT_FAILURE);
}

void
signalHandler(int sig)
{
	exit(EXIT_SUCCESS);
}

void
execute(char **argv, int timeout)
{
	pid_t  pid;
	int    status;

	switch(pid = fork()) {
		case -1:
			fprintf(stderr, "forking child process failed\n");
			exit(EXIT_FAILURE);
		case 0:
			if (execvp(*argv, argv) < 0) {
				fprintf(stderr, "exec failed with command %s\n", *argv);
				exit(EXIT_FAILURE);
			}
		default:
			signal(SIGCHLD,signalHandler);
			sleep(timeout);
			fprintf(stderr, "Time Limit of %d Exceeded!\n", timeout);
			kill(pid, SIGINT);
			exit(EXIT_FAILURE);

			/*
			switch(waitpid(pid, &status, WNOHANG)) {
				case -1:
					fprintf(stderr, "child process failed\n");
					exit(EXIT_FAILURE);
				case 0:
					kill(pid, SIGTERM);
					fprintf(stderr, "Time Limit of %d Exceeded!\n", timeout);
					exit(EXIT_FAILURE);
			}
			*/
	}
}

int
main( int argc, const char* argv[] )
{
	int c, timeout;
	char *cmd;

	while ((c = getopt(argc, argv, "t:")) != -1) {
		switch (c) {
		case 't':
			timeout = atoi(optarg);
			break;
		case 'h':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc <= 0)
		usage();

	execute(argv, timeout);

	exit(EXIT_SUCCESS);
}
