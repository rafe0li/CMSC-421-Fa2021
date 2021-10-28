# Circular Buffer

A circular buffer that holds 20 integers.


## Description

The buffer is a 20 node linked list where the tail node points to the head. Once the buffer is full, 
it cannot be inserted into anymore. The buffer contains two pointers, called the read and write
pointers. The read pointer is the head of the list, or the first inserted node, and the write pointer
is the location of the last inserted node (the position is updated each time there is an insertion).

It is implemented in kernel space with four separate syscalls with the following syscall numbers:
```
init_buffer_421(void) **442**

insert_buffer_421(int i) **443**

print_buffer_421(void) **444**

delete_buffer_421(void) **445**
```
`init_buffer_421` fails if the buffer is initialized
`insert_buffer_421` fails if the buffer is full or uninitialized
`print_buffer_421` fails if the buffer is uninitialized
`delete_buffer_421` fails if the buffer is uninitialized


## Syscall Implementation
`init_buffer_421`
Buffer contained in a global ring_buff struct named BUFF that contains members read/write. Checks if
BUFF.read is initialized. If it is, exit with failure.

If not, declares two temporary node pointers called curr and prev and begins to loop until 
SIZE_OF_BUFFER (20) is reached.

Each loop, a new node pointer n_node declared and initialized by allocating memory. curr and the 
read/write pointers are initialized with this node.

For every loop after, prev is set to curr, and when curr is reassigned to the newly allocated n_node, 
prev->next will be assigned to curr, linking the last node to the current. 

This occurs up until the last loop, during which prev->next will be assigned to the head, or BUFF.read.

`insert_buffer_421`
Checks if BUFF.read is initialized or if BUFF.length is equal to SIZE_OF_BUFFER (buffer is full). If it
is not, exit with failure.

If it is, set the value of the current write pointer's node->data to the input integer from the user.
Then set the write pointer to it's own next value, or whatever node is next in the list. Increment
the length.

`print_buffer_421`
Checks if BUFF.read is initialized. If it is not, exit with failure.

If it is, create a temporary node pointer curr and assign it to the head, BUFF.read. Print out the
node->data member, and loop until BUFF.read is reached again.

`delete_buffer_421` fails if the buffer is uninitialized
Checks if BUFF.read is initialized. If it is not, exit with failure.

If it is, create temporary node pointers curr and prev. Set curr to the head BUFF.read, and loop 
through the list setting prev to curr, curr to curr->next, and freeing prev.