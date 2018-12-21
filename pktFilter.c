#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/uaccess.h>          // Required for the copy to user function
#include <uapi/linux/string.h>
#define  DEVICE_NAME "ebbchar"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "ebb"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("AHMAD");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("packet droper REEEEE");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users
 
static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256][40];             ///< Memory for the string that is passed from userspace
static int    index = 0;
static int    state =0;

static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  ebbcharClass  = NULL; ///< The device-driver class struct pointer
static struct device* ebbcharDevice = NULL; ///< The device-driver device struct pointer

struct sk_buff *sock_buff;
struct iphdr *ip_header;

struct udphdr *udp_header;
struct tcphdr *tcp_header;


 
// The prototype functions for the character driver -- must come before the struct definition
static int     dev_openn(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
 

unsigned int icmp_hook(unsigned int hooknum, struct sk_buff *skb,
                       const struct net_device *in, const struct net_device *out,
                       int(*okfn)(struct sk_buff *));


static struct nf_hook_ops icmp_drop __read_mostly = {
        .pf = NFPROTO_IPV4,
        .priority = NF_IP_PRI_FIRST,
        .hooknum =NF_INET_LOCAL_IN,
        .hook = (nf_hookfn *) icmp_hook
};



static struct file_operations fops =
{
   .open = dev_openn,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};
 
static int dev_openn(struct inode *inodep, struct file *filep){
    state =0;
    index =0;

   numberOpens++;
   printk(KERN_INFO "EBBChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static int __init ebbchar_init(void){
   printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM\n");
 
   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "EBBChar failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "EBBChar: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(ebbcharClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(ebbcharClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "EBBChar: device class registered correctly\n");
 
   // Register the device driver
   ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(ebbcharDevice)){               // Clean up if there is an error
      class_destroy(ebbcharClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(ebbcharDevice);
   }
   printk(KERN_INFO "EBBChar: device class created correctly\n"); // Made it! device was initialized
   int ret = nf_register_net_hook(&init_net,&icmp_drop); /*Record in net filtering */
       if(ret)
           printk(KERN_INFO "FAILED");
   return ret;
}
 

static void __exit ebbchar_exit(void){
   device_destroy(ebbcharClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(ebbcharClass);                          // unregister the device class
   class_destroy(ebbcharClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
    nf_unregister_net_hook(&init_net,&icmp_drop);          /*UnRecord in net filtering */
   printk(KERN_INFO "EBBChar: Goodbye from the LKM!\n");
   
}
 


 

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);
 
   if (error_count==0){            // if true then have success
      printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}
 

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    
    
   sprintf(message[index], "%s", buffer);   // appending received string with its length
   if ( index ==0){
       char temp[20];
       sscanf(message[0],"%s",temp);
       if(strcmp(temp,"whitelist")==0){
           state =1;
       }
       if (strcmp(temp,"blacklist")==0){
           state =2;
       }
       
   }

    printk(KERN_INFO "%s\n", message[index]);
    size_of_message = strlen(message[index]);   // store the length of the stored message
    index++;
    printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
    return len;
   
}

static int dev_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO " these massages Received\n");
    int i = 0;
    char typeFil[20];
    
    sscanf(message[i],"%s",typeFil);
    printk(KERN_INFO "type filter : %s\n",typeFil);
    printk(KERN_INFO "state is %d \n",state );
     for ( i=1; i < index;i++){
        char ipAddr[20];
        char portAdrr[20];
        sscanf(message[i],"%s %s",ipAddr , portAdrr);
        printk(KERN_INFO "ipAddr Revv: %s portadrr Recv: %s\n",ipAddr,portAdrr);
        

    }
   printk(KERN_INFO "EBBChar: Device successfully closed\n");
   return 0;
}
 

unsigned int icmp_hook(unsigned int hooknum, struct sk_buff *skb,

        const struct net_device *in, const struct net_device *out,

        int(*okfn)(struct sk_buff *))
        
{
        sock_buff = skb;
        unsigned int src_port = 0;
        char src_ip[16];
        int flag = 0;
       
        ip_header = (struct iphdr *)skb_network_header(sock_buff);
        snprintf(src_ip,16,"%pI4",&ip_header->saddr);
        if(!sock_buff) { return NF_DROP;}
         if (ip_header->protocol==IPPROTO_ICMP) {
             
            // printk(KERN_INFO "%pI4 Got ICMP Reply packet and accepted  it. \n",&ip_header->saddr);
             //return NF_ACCEPT;
             //return NF_DROP;
         }
      
            
        if ( ip_header->protocol == 6){ ///TCP protocol
            
            tcp_header = (struct tcphdr *)skb_transport_header(sock_buff);
            src_port = (unsigned int)ntohs(tcp_header->source);
            flag=1;
            
        }
        
        if ( ip_header->protocol == 17){ ///UDP
            udp_header = (struct udphdr *)skb_transport_header(sock_buff);
            src_port  = (unsigned int )ntohs(udp_header->source);
            
               flag =1;
            
        }
        if(flag){
            
            if (state == 1){ //whitelist
                int i = 1;
                
                for ( ; i < index; i++){
                    
                    char ipList[20];
                    char portList[20];
                    sscanf(message[i],"%s %s",ipList , portList);
                    
                    unsigned long portli;
                    char *dummy;
                    portli = simple_strtoul(portList, &dummy,10);
                    
                    if( strcmp(ipList,src_ip)==0 && portli == src_port){
                        printk(KERN_INFO "IN packet info: source ip : %s , source port : %u was in whitelist ACCEPTED   ",src_ip,src_port);

                        return NF_ACCEPT;
                    }
                }
                printk(KERN_INFO "IN packet info: source ip : %s , source port : %u was'nt in whitelist DROPPED   ",src_ip,src_port);

                return NF_DROP;
            }
            else if (state == 2 ){//blacklist
                int i = 1;
                for ( ; i < index; i++){
                    
                    char ipList[20];
                    char portList[20];
                    sscanf(message[i],"%s %s",ipList , portList);
                    
                    unsigned long portli;
                    char *dummy;
                    portli = simple_strtoul(portList, &dummy,10);
                    
                    if( strcmp(ipList,src_ip)==0 && portli == src_port){
                        printk(KERN_INFO "IN packet info: source ip : %s , source port : %u was in blacklist DROPPED   ",src_ip,src_port);
                        return NF_DROP;
                    }
                }
                
                printk(KERN_INFO "IN packet info: source ip : %s , source port : %u was'nt in blacklist ACCEPTED   ",src_ip,src_port);

                return NF_ACCEPT ;
            }
        }
    return NF_ACCEPT ;
            
}
        
         
  
         
        



 
 
 
module_init(ebbchar_init);
module_exit(ebbchar_exit);
