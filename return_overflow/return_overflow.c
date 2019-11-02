#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// COMPILE WITH: gcc -fno-stack-protector  -z execstack return_overflow.c

// Helpers
int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char* src, char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
}

struct Executor {
  char data[64];
  void (*func_ptr) (char*);
};

char* printData(char* data) {
  printf("data: %s\n", data);
}

void main() {
  struct Executor exec;
  struct Executor* exec_ptr = &exec;

  // You can try allocating on heap too!
  // exec_ptr = (struct Executor*) malloc(sizeof(struct Executor));

  exec_ptr->func_ptr = &printData;

  printf("data address: %p\n", &(exec_ptr->data)); // debug print out
  printf("func_ptr address: %p\n", &(exec_ptr->func_ptr)); // debug print out


  char hex_string[512];
  int num_bytes = 0;

  printf("Enter hex string:\n");
  scanf("%s", hex_string);

  printf("Enter num bytes:\n");
  scanf("%d", &num_bytes);

  char converted_data[512];
  hex2bin(hex_string, converted_data); // convert input hex string into bytes

  printf("converted_data: %s\n", converted_data); // debug print out
  printf("num_bytes: %d\n", num_bytes); // debug print out

  memcpy(exec_ptr->data, converted_data, num_bytes); // copy data into struct

  printf("data: %s\n", exec_ptr->data); // debug print out
  printf("func_ptr: %p\n", exec_ptr->func_ptr); // debug print out

  exec_ptr->func_ptr(exec_ptr->data);
}
