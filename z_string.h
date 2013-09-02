/* 
   String: This head file is used to build String, use this file,
   it can make you use String easier. You can use it instead of 
   char*. It can make you use less malloc() and free() to manage
   mem. What you need to do for the mem is call create_string() 
   and destroy_string(). But there are also some limitions for
   this String, for example, you shouldn't call remove when the
   String's value is NULL.
   
   Use this head, you can do many operators, for example : insert,
   compare, remove, substring, get size, .etc.
   
   It is supposed to work well, but we don't provide any warranty.
   
   <ul>
   <li>author :  zhang</li>
   <li>email  :  gongheguoyingpai@163.com</li>
   </ul>
 */
 
#ifndef  Z_STRING_H
#define  Z_STRING_H

#include "z_str_search.h"

typedef  struct String
{
    char*  value;
    int    len;
} * String;

static String  create_string(char*  str)
{
    String string = (String)malloc(sizeof(struct String));
    
    char*  value = (char*)malloc(strlen(str) + 1);
    
    strcpy(value, str);
    
    string->value = value;
    string->len = strlen(str);
    
    return string;
}

static int  string_compare(String string, String other_string)
{
    return strcmp(string->value, other_string->value);
}

static int  string_compare_n(String string, String other_string, int n)
{
    return strncmp(string->value, other_string->value, n);
}


static char string_at(String string, int index)
{
    if (index >= 0 && index < string->len)
    {
        return  string->value[index];
    }
    
    return -1;
}

/* This function will create a new String, so when you finish to using it,
   you should use destroy_string() to free the memory. Otherwise, it will
   generate memory leak. And there is no enough characters, it will only
   get characters util reach the tail. 
 */
static String string_sub(String string, int pos, int length)
{
    int end = 0;
    int i = 0;
    
    if (pos >= string->len || pos < 0)
    {
        return NULL;
    }
    
    if (pos + length <= string->len)
    {
        end = pos + length;
    }
    else
    {
        end = string->len;
    }
    
    char* str = (char*)malloc(length + 1);
    
    for (; pos < end; ++pos)
    {
        str[i++] = string->value[pos];
    }
    str[i] = '\0';
    
    String temp = create_string(str);
    
    free(str);
    
    return temp;
}



static void string_print(String string)
{
     printf("%s", string->value);
}

static void string_println(String string)
{
     printf("%s\n", string->value);
}



static int string_index(String string, char ch)
{
    int i;
    
    for (i = 0; i < string->len; ++i)
    {
        if (ch == string->value[i])
        {
            return i;
        }
    }
    
    return -1;
}

/* Usr KMP Algorithm to search, if don't find, it will return -1,
   else it will return the pos. */
static int string_search(String string, String sub_string)
{
    return kmp_search(string->value, sub_string->value);
}



static void malloc_more(String string, int length)
{
    int len = string->len + length + 1;
    
    string->value = (char*)realloc(string->value, len);
}

static void string_insert(String string, String str)
{
     malloc_more(string, str->len);
     
     strcat(string->value, str->value);
     string->len += str->len;
}

static void string_insert_n(String string, String str, int pos)
{
     if (pos < 0 || pos > string->len)
     {
         ;
     }
     else if (pos == string->len)
     {
         string_insert(string, str);
     }
     else
     {
         malloc_more(string, str->len);
         
         char* temp = (char*)malloc(string->len + 1);
         
         strcpy(temp, string->value);
         
         strncpy(string->value, temp, pos);
         string->value[pos] = '\0';
         strncat(string->value, str->value, str->len);
         char* another_temp = temp + pos;
         strcat(string->value, another_temp); 
         
         string->len += str->len;
     }
}

static int  string_size(String string)
{
    return string->len;
}

static int  string_length(String string)
{
     return string_size(string);
}

/* If pos + len is bigger than string's length, we will remove from the pos 
   to the end of the string. */
static void  string_remove(String string, int pos, int len)
{
    if ((pos >= 0 && pos < string->len) && len > 0)
    {
        if (pos + len >= string->len)
        {
            string->value[pos] = '\0';
            string->len = pos;
        }
        else
        {
            int i;
            
            for (i = pos + len; i < string->len; ++i)
            {
                string->value[i - len] = string->value[i];
            }
            string->value[i - len] = '\0';
            string->len -= len;
        }
    }
}


static void  string_replace(String string, String other_string, int pos1, int pos2, int len1, int len2)
{
    /* The position should small than String's length and bigger than zero,
       and postion + len should smaller than String's length.    
    */
    if ((pos1 >= 0 && pos1 < string->len) 
        && (pos2 >= 0 && pos2 < other_string->len)
        && (pos1 + len1 <= string->len) 
        && (pos2 + len2 <= other_string->len))
    {
        if (len2 > len1)                      /* If the replaced str's length is shorter than the new str's length   */ 
        {                                     /* At first we will get enough memory for replace, next we will copy   */
            malloc_more(string, len2 - len1); /* the original string to a temp string, and copy it back in apporiate */
                                              /* order and copy the new str to the original string, and change the   */
            int i, j, k = 0;                  /* original string's length.*/
            char* temp = (char*)malloc(string->len + 1);
            
            strcpy(temp, string->value);
            
            for (i = 0; i < pos1; ++i)
            {
                string->value[k++] = temp[i]; 
            }
            
            for (j = 0; j < len2; j++)
            {
                string->value[k++] = other_string->value[pos2 + j];
            } 
            
            for (i = pos1 + len1; i < string->len; ++i)
            {
                string->value[k++] = temp[i];
            }
            string->value[k] = '\0';
            string->len += len2 - len1;
        }
        else                               /* If the replaced str's length is longer the new str's length, we will */
        {                                  /* copy the new str to replace the replaced str, and move the orignal   */
            int i, j, k;                   /* string's characters behind the replaced str to ahead, and change the */
            int distance = len1 - len2;    /* original string's length */ 
            
            for (i = pos1, j = pos2, k = 0; k < len2; ++i, ++j, ++k)
            {
                string->value[i] = string->value[j];
            }
            
            for (i = pos1 + len1; i < string->len; ++i)
            {
                string->value[i - distance] = string->value[i];
            }
            string->value[i - distance] = '\0';
            string->len -= distance;
        }
    }
} 

/* Clear all character of the String, it will free the String's value's memory,
   and make the string's length be zero.
 */
static void string_clear(String string)
{
    free(string->value);
    string->value = NULL;
    string->len = 0;
}

/* Reverse the String.*/
static void string_reverse(String string)
{
    int i;
    int len = string->len;
    
    for (i = 0; i < len / 2; ++i)
    {
        char temp = string->value[i];
        string->value[i] = string->value[len - i - 1];
        string->value[len - i - 1] = temp;
    }
}

static void string_toupper(String string)
{
    int i;
    
    for (i = 0; i < string->len; ++i)
    {
        string->value[i] = toupper(string->value[i]);
    }
}

static void string_tolower(String string)
{
    int  i;
    
    for (i = 0; i < string->len; ++i)
    {
        string->value[i] = tolower(string->value[i]);
    }
}

/* At first free the string's value point, next free the string point.*/
static void destroy_string(String string)
{
    free(string->value);
    free(string);
}

#endif
