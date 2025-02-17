#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/wait.h>


#define DATA_SIZE 16*1024*1024

#define KB (1024)
#define MB (1024*KB)

char info[DATA_SIZE];
char data[DATA_SIZE] = {1};

int main() {
	printf("PID: %u\n", getpid());

	const long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
	printf("PAGE_SIZE: %ld\n", PAGE_SIZE);

	printf("#1 (press ENTER to continue)"); getchar();
	
	// a. Increase the resident set (Rss) by about 3MB in the .bss region.
	
	for (int i = 0; i < 3 * MB; i += PAGE_SIZE) {
		info[i] = 1;
	}

	printf("#2 (press ENTER to continue)"); getchar();
    
	// b. Access 256 bytes of initialized data (.data) with maximum impact in Private Clean pages
	
	int count = 0;
    for (int i = 0; i < 256; i++) {
		count += data[i * PAGE_SIZE];
	}

	printf("#3 (press ENTER to continue)"); getchar();

	// c. Reduce the Pss of non-initialized data (.bss) to around 1.5MB for 30 seconds, while keeping Rss.
    
	pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }
	
	if (pid == 0) {
		sleep(30);
		exit(0);
	}

	waitpid(pid, NULL, 0);

	printf("#4 (press ENTER to continue)"); getchar();

	// d. Execute a single operating system function that results in two new regions being added to the
	// existing address space, one with code (~ 4KB) and one with data (~ 512KB).

    void* handle = dlopen("./prog2.so", RTLD_NOW);

	if (handle == NULL) {
		fprintf(stderr, "dlopen failed: %s\n", dlerror());
		return 1;
	}
  
	printf("#5 (press ENTER to continue)"); getchar();

	// e. Increase Private dirty pages by about 128KB in the region for the data section created in d)

	void (*affect_data)(int, int); 

	* (void **) (&affect_data) = dlsym(handle, "affect_data");

	affect_data(128 * KB, PAGE_SIZE);

	printf("#END (press ENTER to continue)"); getchar();

	dlclose(handle);

	return 0;
}