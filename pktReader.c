
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include <linux/ip.h>
#include <linux/netfilter_ipv4.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   fd = open("/dev/ebbchar", O_RDWR);   // Open the device with read/write access
   
   
   
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
     
   
    int counter =0;
    char ip[20];
    char port[20];
   FILE *filep;
   
   
   char filename[20];
   
   char buffer[30];

   printf("Type filename:\n");
   
   scanf("%s",filename);
   
   sprintf(buffer,"%s",filename);
   
   filep = fopen(buffer,"r");
    while(fgets(buffer,255,(FILE *)filep)!= NULL){
        
        ret = write(fd, buffer, strlen(buffer)); // Send the string to the LKM
   
        if (ret < 0){
            perror("Failed to write the message to the device.");
        
            return errno;
        }
    }
   
    

    fclose(filep);
    return 0;
}
