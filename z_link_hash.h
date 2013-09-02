#ifndef  Z_LINK_HASH_H
#define  Z_LINK_HASH_H

#include <stdbool.h>
#include <assert.h>

#define  HASH_NUM    26

typedef struct Node
{
    char*  value;
    struct Node*  next;
} Node;

typedef struct Hash
{
   Node*   head;
   Node*   tail;
}  Hash;

static Hash *  create_hash()
{
    Hash*  hash = (Hash*)malloc(sizeof(Hash) * HASH_NUM);
    int i;
    
    for (i = 0; i < HASH_NUM; ++i)
    {
        hash[i].head = NULL;
        hash[i].tail = NULL;
    } 
    
    return hash;
}

static int hash_code(char first_char)
{
    first_char = toupper(first_char);
    
    return  first_char - 65;
}

static void  hash_insert(Hash* hash, char*  value)
{
    assert(value != NULL && strlen(value) >= 1);
    
    int pos = hash_code(value[0]);
    
    char*  str_value = (char*)malloc(strlen(value) + 1);
    strcpy(str_value, value);
    
    Node*  new_elem = (Node*)malloc(sizeof(Node));
    new_elem->value = str_value;
    new_elem->next = NULL;
    
    if (hash[pos].head == NULL)
    {
         hash[pos].head = new_elem;
         hash[pos].tail = new_elem;
    }
    else
    {
         hash[pos].tail->next = new_elem;
         hash[pos].tail = new_elem;
    }
} 

static Node*  hash_search(Hash* hash, char* value)
{
    assert(value != NULL && strlen(value) >= 1);
    
    int pos = hash_code(value[0]);
    
    Node* temp = hash[pos].head;
    Node* ahead = hash[pos].head;
    
    while (temp != NULL)
    {
         if (strcmp(temp->value, value) == 0)
         {
             return ahead;
         }
         
         ahead = temp;
         temp = temp->next;
    }
    
    return NULL;
}

static bool hash_delete(Hash* hash, char* str)
{
    assert(str != NULL && strlen(str) >= 1);
    
    Node* pos_point = hash_search(hash, str);
    int pos = hash_code(str[0]);
    
    if (pos_point != NULL)
    {
        if (hash[pos].head == pos_point)
        {
            Node* temp = hash[pos].head;
            hash[pos].head = hash[pos].head->next;
            free(temp);
        }
        else
        {
            Node* temp = pos_point->next;
            pos_point->next = temp->next;
            free(temp);
        }
        
        return  true;
    }
    
    return  false;
}

static void  destroy_hash(Hash* hash)
{
     int i;
     
     for (i = 0; i < HASH_NUM; ++i)
     {
         Node* head = hash[i].head;
         
         while (head)
         {
             Node* temp = head;
             head = head->next;
             free(temp->value);
             free(temp);
         }
         
         hash[i].head = NULL;
         hash[i].tail = NULL;
     }
}

#endif
