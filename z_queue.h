/*
  Queue : This head file is to build Queue, if use this Queue
  you can use it to push new element at tail, and pop element 
  at head, and you can push infinit elements in the theory ,
  and you need't to use malloc() and free() to manage the mem
  you use, you can only use destroy_queue() to make sure all
  mem will be free at one time.
  
  It is supposed to work well, but we don't provide any warranty.
  
  <ul>
   <li>author :  zhang</li>
   <li>email  :  gongheguoyingpai@163.com</li>
   </ul>
 */
#ifndef  Z_QUEUE_H
#define  Z_QUEUE_H

typedef struct Node
{
   int    value;
   struct Node*  next; 
}  Node;

typedef struct Queue
{
   Node*  head;           /* queue's head point */
   Node*  tail;           /* queue's tail point */ 
   Node*  original_head;  /* record the original head to make free all memory, 
                           it will nerver change after init and before run destroy. */
}  Queue;

/* create a queue and return a Queue point*/
static Queue*  create_queue()
{
    Queue*  queue = (Queue*)malloc(sizeof(Queue));
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->original_head = NULL; 
    
    return queue;
}

/* push an element to a queue, if the queue is empty we will make all queue point point to the new 
   element, else we only change the tail point to point the new element. */
static void queue_push(Queue* queue, int number)
{
    Node*  element = (Node*)malloc(sizeof(Node));
    
    element->value = number;
    element->next = NULL;
    
    if (queue->head == NULL)
    {
        queue->head = element;
        queue->tail = element;
        queue->original_head = element;
    }
    else
    {
        queue->tail->next = element;
        queue->tail = element;
    }
}

/* pop an element from a queue, if all elements has poped, but queue->head != NULL does't true in outer if
   we should check the head point in inner if and if the head point is NUll, we should make the tail point
   to be NULL because in the queue_size() we need compare head point and tail to decide if have more elements.
*/
static Node*  queue_pop(Queue* queue)
{
    if (queue->head != NULL)
    {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        
        if (queue->head == NULL)
        {
            queue->tail = NULL;
        }
        
        return temp;
    }
    else
    {
        return  NULL;
    }
}

static int queue_size(Queue* queue)
{
    Node* temp = queue->head;
    int count = temp == NULL ? 0 : 1;   /* because we use queue->head == queue->tail to get the size, so when head == tail
                                           we can add 1 at count, so if head != NULL, we should make count = 1*/
       
    while (temp != queue->tail)
    {
       ++count;
       temp = temp->next;
    }
    
    return count;
}

static void queue_destroy(Queue* queue)
{
    while (queue->original_head != NULL)
    {
         Node* temp = queue->original_head;
         queue->original_head = queue->original_head->next;
         free(temp);
    }
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->original_head = NULL;
}

#endif
