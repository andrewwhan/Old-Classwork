// Nathan Bryant and Andrew Han -- Mailbox_LKM.c

// We need to define __KERNEL__ and MODULE to be in Kernel space
// If they are defined, undefined them and define them again:
#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE

#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include "mailbox.h"

#define PIDDNE		100
#define KERNPID		101
#define VALIDPID	102

#define NULLBOX		200

struct kmem_cache* mailboxCache;
struct kmem_cache* messageCache;
struct kmem_cache* listCache;
struct kmem_cache* contentCache;

spinlock_t creationLock;

char* maCache = "mailboxCache";
char* meCache = "messageCache";
char* lCache = "listCache";
char* cCache = "contentCache";

struct Mailbox{
	pid_t owner;
	int numberofMessages;
	bool status;
	spinlock_t lock;
	int waitingEmpty;
	int waitingFull;
	atomic_t references;
	wait_queue_head_t canExit;
	wait_queue_head_t notEmpty;
	wait_queue_head_t notFull;
	struct Message *message;
};

struct Message{
	pid_t sender;
	int length;
	void *msg;
	struct Message *next;
};	

struct list{
	struct Mailbox* mailbox;
	struct list *next;
	pid_t pid;
};

struct list *Hashmap[11];

pid_t Hash(pid_t toHash){
	//printk(KERN_INFO "Hashing %d into %d \n", toHash, toHash % 11);
	return toHash % 11;
}

struct Mailbox* HashFind(pid_t toFind){
	int hash = Hash(toFind);
	struct list *theList = Hashmap[hash];
	//printk(KERN_INFO "Searching hash for %d \n", toFind);
	// If linked list is empty, pid DNE in hash
	if (theList == NULL){
		//printk(KERN_INFO "I think the list is empty (%d) \n", toFind);
		return NULL;
	}
	// Iterate through linked list until end or pid is found
	while (toFind != theList->pid && theList->next != NULL){
		theList = theList->next;
	}
	// If pid is found, return: mailbox if it exists, otherwise defined constant due to kernel process
	if (theList->pid == toFind){
		if(theList->mailbox != NULL)
			return theList->mailbox;
		else
			return (struct Mailbox*)NULLBOX;
	}
	// Otherwise, pid DNE in hash
	else{
		printk(KERN_INFO "I think %d doesn't exist in the list \n", toFind);
		return NULL;
	}
}

int HashRemove(pid_t toRemove){
	int hash = Hash(toRemove);
	struct list *theList = Hashmap[hash];
	struct list *temp;
	//pid DNE in hash
	if (Hashmap[hash] == NULL){
		return 1;
	}
	//If first element in linked list matches, remove it
	if(toRemove ==  Hashmap[hash]->pid){
		if(Hashmap[hash]->mailbox){
			kmem_cache_free(mailboxCache, Hashmap[hash]->mailbox);
		}
		temp = Hashmap[hash]->next;
		kmem_cache_free(listCache, Hashmap[hash]);
		Hashmap[hash] = temp;
		return 0;
	}
	//If there was only one element in the list and it wasn't a match, pid DNE in hash
	if (Hashmap[hash]->next == NULL){
		Hashmap[hash] = theList;
		return 0;
	}
	//If second element is a match, remove it
	if (Hashmap[hash]->next->pid == toRemove){
		if (Hashmap[hash]->next->mailbox){
			kmem_cache_free(mailboxCache, Hashmap[hash]->next->mailbox);
		}
		temp = Hashmap[hash]->next->next;
		kmem_cache_free(listCache, Hashmap[hash]->next);
		Hashmap[hash]->next = temp;
		return 0;
	}
	//Otherwise iterate through list until reaching end or pid is found
	while (toRemove != Hashmap[hash]->next->pid){ 
		Hashmap[hash] = Hashmap[hash]->next;
		if (Hashmap[hash]->next == NULL){
			Hashmap[hash] = theList;
			return 0;
		}
	}
	//Remove pid's link
	if(Hashmap[hash]->next->mailbox){
		kmem_cache_free(mailboxCache, Hashmap[hash]->next->mailbox);
	}
	temp = Hashmap[hash]->next->next;
	kmem_cache_free(listCache, Hashmap[hash]->next);
	Hashmap[hash]->next = temp;
	Hashmap[hash] = theList;
	return 0;
}

int HashAdd (pid_t toAdd, struct list* link){
	int hash = Hash(toAdd);
	struct list *theList = Hashmap[hash];
	if (Hashmap[hash] == NULL){
		Hashmap[hash] = link;
		return 1;
	}
	if (Hashmap[hash]->next == NULL){
		Hashmap[hash]->next = link;
		return 1;
	}
	while (Hashmap[hash]->next != NULL){
		Hashmap[hash] = Hashmap[hash]->next;
	}
	Hashmap[hash]->next = link;
	Hashmap[hash] = theList;
	return 0;
}

//Checks that a PID DNE, is a kernel PID, or a valid user PID
int verifyPID(pid_t pid){
	struct task_struct* task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(!task){
		return PIDDNE;
	}
	else{
		if(task->flags & PF_EXITING){
			printk(KERN_INFO "Flags: %d \n", task->flags & PF_EXITING);
			printk(KERN_INFO "Process %d is dead \n", pid);
			return PIDDNE;
		}
		if(!task->mm){
			return KERNPID;
		}
		return VALIDPID;
	}
}
	
	
unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall1)(void);
asmlinkage long (*ref_sys_cs3013_syscall2)(void);
asmlinkage long (*ref_sys_cs3013_syscall3)(void);
asmlinkage long (*ref_sys_exit)(int exit_code);
asmlinkage long (*ref_sys_exit_group)(int exit_code);

asmlinkage long SendMsg(pid_t dest, void* msg, int len, bool block){
 	struct Mailbox* receiver;
	struct list* hashLink;
	struct Message* sendingMessage;
	struct Message* messages;
	int pidstatus;
	spin_lock_irq(&creationLock);
	pidstatus = verifyPID(dest);
	if(pidstatus == PIDDNE){
		spin_unlock_irq(&creationLock);
		return MAILBOX_INVALID;
	}
	else if(pidstatus == KERNPID){
		if(HashFind(dest) == NULL){
			hashLink = kmem_cache_alloc(listCache, GFP_KERNEL);
			hashLink->mailbox = NULL;
			hashLink->next = NULL;
			HashAdd(dest, hashLink);
		}
		spin_unlock_irq(&creationLock);
		return MAILBOX_INVALID;
	}
	if ((receiver = HashFind(dest)) == NULL){
		//Allocate and initialize the mailbox for the receiver
		printk(KERN_INFO "Mailbox created via send for %d \n", dest);
		receiver = kmem_cache_alloc(mailboxCache, GFP_KERNEL);
		receiver->owner = dest;
		receiver->numberofMessages = 0;
		receiver->status = false;
		receiver->message = NULL;
		atomic_set(&receiver->references, 0);
		receiver->waitingFull = 0;
		receiver->waitingEmpty = 0;
		init_waitqueue_head(&receiver->canExit);
		spin_lock_init(&receiver->lock);
		init_waitqueue_head(&receiver->notEmpty);
		init_waitqueue_head(&receiver->notFull);
		//Allocate and initialize the hash link for the receiver
		hashLink = kmem_cache_alloc(listCache, GFP_KERNEL);
		hashLink->mailbox = receiver;
		hashLink->pid = dest;		
		hashLink->next = NULL;
		HashAdd(dest, hashLink);
	}
	if (atomic_read(&receiver->references) == -1){
		spin_unlock_irq(&creationLock);	
		return MAILBOX_ERROR;
	}
	atomic_add(1, &receiver->references);
	spin_unlock_irq(&creationLock);	
	spin_lock_irq(&receiver->lock);
//If the message length is greater than the max or less than zero, it returns the message length error
	if (len > MAX_MSG_SIZE || len < 0){
		atomic_sub(1, &receiver->references);
		wake_up_all(&receiver->canExit);
		spin_unlock_irq(&receiver->lock);
		return MSG_LENGTH_ERROR;
	}
//If the receiver's mailbox is stopped, it will return an error 
	if (receiver->status == true){
		atomic_sub(1, &receiver->references);
		wake_up_all(&receiver->canExit);
		spin_unlock_irq(&receiver->lock);
		return MAILBOX_STOPPED;
	}
	spin_unlock_irq(&receiver->lock);
//Allocates and initializes the message to be sent		
	sendingMessage = kmem_cache_alloc(messageCache, GFP_KERNEL);
	sendingMessage->sender = current->tgid;
	sendingMessage->length = len;
	sendingMessage->msg = kmem_cache_alloc(contentCache, GFP_KERNEL);
	copy_from_user(sendingMessage->msg, msg, len);
	sendingMessage->next = NULL;
	spin_lock_irq(&receiver->lock);
// Mailbox is full, and block is set to false, need to return an error
	if (receiver->numberofMessages == 32 && block == false){
		atomic_sub(1, &receiver->references);
		wake_up_all(&receiver->canExit);
		spin_unlock_irq(&receiver->lock);
		return MAILBOX_FULL;
	}
// Mailbox is full, and block is true, so it needs to wait on the queue
	else if (receiver->numberofMessages == 32 && block == true){
		receiver->waitingFull++;
		spin_unlock_irq(&receiver->lock);	
		wait_event(receiver->notFull, (receiver->numberofMessages < 32 || receiver->status == true));
		spin_lock_irq(&receiver->lock);		
		receiver->waitingFull--;	
		if (receiver->waitingFull == 0){
			wake_up_all(&receiver->canExit);
		}
		if (receiver->status == true){
			atomic_sub(1, &receiver->references);
			wake_up_all(&receiver->canExit);
			spin_unlock_irq(&receiver->lock);
			return MAILBOX_STOPPED;
		}
	} 
	messages = receiver->message;
// If the mailbox is empty, the current message being sent becomes the head of the receivers message list
	if (messages == NULL){
		receiver->message = sendingMessage;
		receiver->numberofMessages++;
		wake_up_all(&receiver->notEmpty);
		atomic_sub(1, &receiver->references);
		wake_up_all(&receiver->canExit);
		spin_unlock_irq(&receiver->lock);
		return 0;
	}
// The message is put onto the end of the receiver's list of messages
	while(messages->next != NULL){
		messages = messages->next;
	}
	messages->next = sendingMessage;
	receiver->numberofMessages++;
	atomic_sub(1, &receiver->references);
	wake_up_all(&receiver->canExit);
	spin_unlock_irq(&receiver->lock);
	return 0;
}

asmlinkage long RcvMsg(pid_t *sender, void *msg, int *len, bool block){
	struct Mailbox* self;
	struct list* hashLink;
	struct Message* messages;
	spin_lock_irq(&creationLock);
	if ((self = HashFind(current->tgid)) == NULL){
		//Allocate and initialize the mailbox for the receiver
		printk(KERN_INFO "Mailbox created via rcv for %d \n", current->tgid);
		self = kmem_cache_alloc(mailboxCache, GFP_KERNEL);
		self->owner = current->tgid;
		self->numberofMessages = 0;
		self->status = false;
		self->message = NULL;
		atomic_set(&self->references, 0);
		self->waitingFull = 0;
		self->waitingEmpty = 0;
		init_waitqueue_head(&self->canExit);
		spin_lock_init(&self->lock);
		init_waitqueue_head(&self->notEmpty);
		init_waitqueue_head(&self->notFull);
		//Allocate and initialize the hash link for the 		//receiver
		hashLink = kmem_cache_alloc(listCache, GFP_KERNEL);
		hashLink->mailbox = self;
		hashLink->pid = current->tgid;
		hashLink->next = NULL;
		HashAdd(current->tgid, hashLink);
	}	
	atomic_add(1, &self->references);
	spin_unlock_irq(&creationLock);
	spin_lock_irq(&self->lock);
// If the mailbox of the calling process is stopped, return an error
	if (self->status == true && self->numberofMessages == 0){
		atomic_sub(1, &self->references);
		wake_up_all(&self->canExit);
		spin_unlock_irq(&self->lock);
		return MAILBOX_STOPPED;
	}
// Number of messages in the process' mailbox is 0, and block is false,
// so return an error
	if (self->numberofMessages == 0 && block == false){
		atomic_sub(1, &self->references);
		wake_up_all(&self->canExit);
		spin_unlock_irq(&self->lock);
		return MAILBOX_EMPTY;
	}
// Process' mailbox is empty and block is true, so need to wait on the wait queue until a message is sent
	else if (self->numberofMessages == 0 && block == true){
		self->waitingEmpty++;
		spin_unlock_irq(&self->lock);
		wait_event(self->notEmpty, (self->numberofMessages > 0 || self->status == true));
		spin_lock_irq(&self->lock);	
		self->waitingEmpty--;	
		if (self->waitingEmpty == 0){
			wake_up(&self->canExit);
		}
		if (self->status == true){
			self->waitingEmpty--;
			atomic_sub(1, &self->references);
			wake_up_all(&self->canExit);
			spin_unlock_irq(&self->lock);
			return MAILBOX_STOPPED;
		}
	}
// Fills the given pointers with the data that was contained in the message
	messages = self->message;
	if(copy_to_user(sender, &messages->sender, sizeof(pid_t)) ||
		copy_to_user(len, &messages->length, sizeof(int)) ||
		copy_to_user(msg, messages->msg, messages->length)){
			atomic_sub(1, &self->references);
			wake_up_all(&self->canExit);
			spin_unlock_irq(&self->lock);
			return MSG_ARG_ERROR;
		}
	self->message = self->message->next;
	kmem_cache_free(contentCache, messages->msg);
	kmem_cache_free(messageCache, messages);
	self->numberofMessages--;
	wake_up_all(&self->notFull);
	atomic_sub(1, &self->references);
	wake_up_all(&self->canExit);
	spin_unlock_irq(&self->lock); 
	return 0;
}

asmlinkage long ManageMailbox(bool stop, int *count){
	struct Mailbox* self;
	struct list* hashLink;
	spin_lock_irq(&creationLock);
	if ((self = HashFind(current->tgid)) == NULL){
		//Allocate and initialize the mailbox for the receiver
		printk(KERN_INFO "Mailbox created via mng for %d \n", current->tgid);
		self = kmem_cache_alloc(mailboxCache, GFP_KERNEL);
		self->owner = current->tgid;
		self->numberofMessages = 0;
		self->status = false;
		self->message = NULL;
		atomic_set(&self->references, 0);
		self->waitingFull = 0;
		self->waitingEmpty = 0;
		init_waitqueue_head(&self->canExit);
		spin_lock_init(&self->lock);
		init_waitqueue_head(&self->notEmpty);
		init_waitqueue_head(&self->notFull);
		//Allocate and initialize the hash link for the 		//receiver
		hashLink = kmem_cache_alloc(listCache, GFP_KERNEL);
		hashLink->mailbox = self;
		hashLink->pid = current->tgid;
		hashLink->next = NULL;
		HashAdd(current->tgid, hashLink);
	}	
	atomic_add(1, &self->references);
	spin_unlock_irq(&creationLock);
	spin_lock_irq(&self->lock);
	// If the count pointer isn't null, copy the number of messages to user space
	if (count != NULL){
		if(copy_to_user(count, &self->numberofMessages, sizeof(int))){
			atomic_sub(1, &self->references);
			wake_up_all(&self->canExit);
			spin_unlock_irq(&self->lock);
			return MSG_ARG_ERROR;
		}
	}
	// If stop is set to true, need to wake up all the waiting processes so that they can return
	if (stop == true){
		self->status = stop;
		wake_up_all(&self->notFull);
		wake_up_all(&self->notEmpty);
	}
	atomic_sub(1, &self->references);
	wake_up_all(&self->canExit);
	spin_unlock_irq(&self->lock);
	return 0;
} 

asmlinkage long NewExit(int exit_code){
	int group_dead;
	struct task_struct* tsk = current;
	struct Mailbox* self;	
// If the calling process does not have a mailbox, simply call the original exit system call
	if((self = HashFind(current->tgid))==NULL){
		return (*ref_sys_exit)(exit_code);
	}
// If the calling process is a kernel process, remove it's mailbox and then exit
	if(verifyPID(current->pid)==KERNPID){
		spin_lock_irq(&creationLock);
		HashRemove(current->tgid);
		spin_unlock_irq(&creationLock);
		return (*ref_sys_exit)(exit_code);
	}
	group_dead = atomic_read(&tsk->signal->live) - 1;
// If the calling process is the last process/thread in its group, it needs to delete the mailbox 
	if(group_dead == 0){
	// Manage the mailbox to stop it, which removes all waiting processes on send or receive
		ManageMailbox(true, NULL);
		spin_lock_irq(&self->lock);
	// If there are no processes waiting on either waiting queue, then flush the messages in the mailbox
		if (self->waitingFull != 0 || self->waitingEmpty != 0 || atomic_read(&self->references) != 0){
			spin_unlock_irq(&self->lock);
			wait_event(self->canExit, self->waitingFull == 0 && self->waitingEmpty == 0 && atomic_read(&self->references) == 0);
			atomic_set(&self->references, -1);
			spin_lock_irq(&self->lock);
		}
		atomic_set(&self->references, -1);
		printk(KERN_INFO "%d is flushing %d messages. \n", current->tgid, self->numberofMessages);
		while(self->numberofMessages > 0){
			struct Message* messages = self->message;
			//printk(KERN_INFO "Number of messages: %d\n", self->numberofMessages);
			self->message = self->message->next;
			kmem_cache_free(contentCache, messages->msg);
			kmem_cache_free(messageCache, messages);
			self->numberofMessages--;
		}
		//printk(KERN_INFO "Number of messages: %d\n", self->numberofMessages);
	// If there are no processes with a reference to the mailbox, delete the mailbox and hash link using HashRemove()
		spin_unlock_irq(&self->lock);
		spin_lock_irq(&creationLock);
		HashRemove(current->tgid);
		spin_unlock_irq(&creationLock);
		printk(KERN_INFO "Exiting and deleting mailbox for %d \n", current->tgid);
		//printk(KERN_INFO "Last member of thread group, so the mailbox has been deleted\n");
		return (*ref_sys_exit)(exit_code);
	}
	//printk(KERN_INFO "Not the last member of thread group\n");
	return (*ref_sys_exit)(exit_code);
}

// NewExitGroup() behaves exactly the same as NewExit, except when the the original exit system call was called in NewExit, the original exit group system call is called instead

asmlinkage long NewExitGroup(int exit_code){
	int group_dead;
	struct task_struct* tsk = current;
	struct Mailbox* self;	
	if((self = HashFind(current->tgid))==NULL){
		return (*ref_sys_exit_group)(exit_code);
	}
	if(verifyPID(current->pid)==KERNPID){
		spin_lock_irq(&creationLock);
		HashRemove(current->tgid);
		spin_unlock_irq(&creationLock);
		return (*ref_sys_exit_group)(exit_code);
	}
	group_dead = atomic_read(&tsk->signal->live) - 1;
	if(group_dead == 0){
		ManageMailbox(true, NULL);
		spin_lock_irq(&self->lock);
		if (self->waitingFull != 0 || self->waitingEmpty != 0 ||  atomic_read(&self->references) != 0){
			spin_unlock_irq(&self->lock);
			wait_event(self->canExit, self->waitingFull == 0 && self->waitingEmpty == 0 &&  atomic_read(&self->references) == 0);
			atomic_set(&self->references, -1);
			spin_lock_irq(&self->lock);
		}
		atomic_set(&self->references, -1);
		printk(KERN_INFO "%d is flushing %d messages. \n", current->tgid, self->numberofMessages);
		while(self->numberofMessages > 0){
			struct Message* messages = self->message;
			//printk(KERN_INFO "Number of messages: %d\n", self->numberofMessages);
			self->message = self->message->next;
			kmem_cache_free(contentCache, messages->msg);
			kmem_cache_free(messageCache, messages);
			self->numberofMessages--;
		}
		//printk(KERN_INFO "Number of messages: %d\n", self->numberofMessages);
		spin_unlock_irq(&self->lock);
		spin_lock_irq(&creationLock);
		HashRemove(current->tgid);
		spin_unlock_irq(&creationLock);
		printk(KERN_INFO "Exiting and deleting mailbox for %d \n", current->tgid);
		//printk(KERN_INFO "Last member of thread group, so the mailbox has been deleted\n");
		return (*ref_sys_exit_group)(exit_code);
	}
	//printk(KERN_INFO "Not the last member of thread group\n");
	return (*ref_sys_exit_group)(exit_code);
}

static unsigned long **find_sys_call_table(void) {
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close) {
			printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX", (unsigned long) sct);
			return sct;
		}

	offset += sizeof(void *);
	}

	return NULL;
}	// static unsigned long **find_sys_call_table(void)


static void disable_page_protection(void) {
	/*
	Control Register 0 (cr0) governs how the CPU operates.

	Bit #16, if set, prevents the CPU from writing to memory marked as
	read only. Well, our system call table meets that description.
	But, we can simply turn off this bit in cr0 to allow us to make
	changes. We read in the current value of the register (32 or 64
	bits wide), and AND that with a value where all bits are 0 except
	the 16th bit (using a negation operation), causing the write_cr0
	value to have the 16th bit cleared (with all other bits staying
	the same. We will thus be able to write to the protected memory.

	It's good to be the kernel!
	*/

	write_cr0 (read_cr0 () & (~ 0x10000));

}	//static void disable_page_protection(void)


static void enable_page_protection(void) {
	/*
	See the above description for cr0. Here, we use an OR to set the
	16th bit to re-enable write protection on the CPU.
	*/

	write_cr0 (read_cr0 () | 0x10000);

}	// static void enable_page_protection(void)


static int __init interceptor_start(void) {
	/* Find the system call table */
	if(!(sys_call_table = find_sys_call_table())) {
		/* Well, that didn't work.
		Cancel the module loading step. */
		return -1;
	}


	/* Store a copy of all the existing functions */
	ref_sys_cs3013_syscall1 = (void *)sys_call_table[__NR_cs3013_syscall1];
	ref_sys_cs3013_syscall2 = (void *)sys_call_table[__NR_cs3013_syscall2];
	ref_sys_cs3013_syscall3 = (void *)sys_call_table[__NR_cs3013_syscall3];
	ref_sys_exit = (void *)sys_call_table[__NR_exit];
	ref_sys_exit_group = (void *)sys_call_table[__NR_exit_group];

	/* Replace the existing system calls */
	disable_page_protection();

	sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)SendMsg;
	sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)RcvMsg;
	sys_call_table[__NR_cs3013_syscall3] = (unsigned long *)ManageMailbox;
	sys_call_table[__NR_exit] = (unsigned long *)NewExit;
	sys_call_table[__NR_exit_group] = (unsigned long *)NewExitGroup;
	enable_page_protection();

	/* And indicate the load was successful */
	// Initializes the three slab caches, as well as the universal 		//spin lock that is used for creating mailboxes
	mailboxCache = kmem_cache_create(maCache, sizeof(struct Mailbox), 0, 0, NULL);
	messageCache = kmem_cache_create(meCache, sizeof(struct Message), 0, 0, NULL); 
	listCache = kmem_cache_create(lCache, sizeof(struct Mailbox), 0, 0, NULL);
	contentCache = kmem_cache_create(cCache, 128, 0, 0, NULL);
	spin_lock_init(&creationLock);
	printk(KERN_INFO "Loaded interceptor!");

	return 0;
}	// static int __init interceptor_start(void)


static void __exit interceptor_end(void) {
	/* If we don't know what the syscall table is, don't bother. */
	if(!sys_call_table)
		return;

	/* Revert all system calls to what they were before we began. */
	disable_page_protection();
	sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)ref_sys_cs3013_syscall1;
	sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)ref_sys_cs3013_syscall2;
	sys_call_table[__NR_cs3013_syscall3] = (unsigned long *)ref_sys_cs3013_syscall3;
	sys_call_table[__NR_exit] = (unsigned long *)ref_sys_exit;
	sys_call_table[__NR_exit_group] = (unsigned long *)ref_sys_exit_group;
	enable_page_protection();
	kmem_cache_destroy(mailboxCache);
	kmem_cache_destroy(messageCache);
	kmem_cache_destroy(listCache);
	kmem_cache_destroy(contentCache);

	printk(KERN_INFO "Unloaded interceptor!");
}	// static void __exit interceptor_end(void)

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);


