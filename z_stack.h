/*
  Stack : This head file is used to build Stack, use this file,
  it can make you use stack easier. You can push element to the stack,
  or pop element from the stack, and you can get the stack's size, and
  get the stack's top element.
  
  It will auto add the stack's capacity if nesessary (when you push new
  element and the stack's size has equal the stack's capacity.) It will 
  make you use stack easier. And you needn't use malloc() , realloc(),
  free() to manage the memory, but only use create_stack() and destory_
  stack() will be ok.
  
   It is supposed to work well, but we don't provide any warranty.
   
   <ul>
   <li>author :  zhang</li>
   <li>email  :  gongheguoyingpai@163.com</li>
   </ul>
 */

#ifndef  Z_STACK_H
#define  Z_STACK_H

#define  Z_INT_MAX   2147483647

typedef  struct Stack
{
   int*  values;
   int   size;
   int   capacity;
}  Stack;


static Stack*  create_stack(int size)
{
    Stack*  stack = (Stack*)malloc(sizeof(Stack));
    
    stack->values = (int*)malloc(sizeof(int) * size);
    stack->size = 0;
    stack->capacity = size;
    
    return  stack;
}

/* When users use stack_pus() to push more elements but the stack has no enough space
   we use this function to malloc more memory, and the new stack's capacity is as twice
   as the old stack's capacity.
 */
 
static void malloc_more(Stack* stack)
{
    int new_capacity = stack->capacity * 2;
    
    stack->values = (int*)realloc(stack->values, sizeof(int) * new_capacity);
    stack->capacity = new_capacity;
}

/* If stack->size < stack->capacity, we wiil push new element in the stack,
   else we will try to malloc more memory to push the new element.
 */
   
static void  stack_push(Stack* stack, int num)
{
     if (stack->size + 1 <= stack->capacity)
     {
         stack->values[stack->size++] = num;
     }
     else
     {
         malloc_more(stack);
         stack->values[stack->size++] = num;
     }
}

static void stack_pop(Stack* stack)
{
     if (stack->size > 0)
     {
          --stack->size;
     }
}

static int stack_size(Stack* stack)
{
     return  stack->size;
}

/* if stack's size is bigger than 0, we will return the top number of stack,
   else we will return a special number as Z_INT_MAX(2147483647).
 */
 
static int stack_top(Stack* stack)
{
     if (stack_size(stack) > 0)
     {
          return stack->values[stack->size - 1];
     }
     else
     {
          return -1 * Z_INT_MAX;
     }
}

void  destory_stack(Stack* stack)
{
      free(stack->values);
      free(stack);
}


#endif
