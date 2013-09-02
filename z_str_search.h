#ifndef  Z_STR_SEARCH_H
#define  Z_STR_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**************  Start Simple Algorithm  ******************/

static int  simple_search(const char * str, const char * s_str)
{
     int i = 0, j = 0;
     int str_len = strlen(str);
     int sstr_len = strlen(s_str);
     int len = str_len - sstr_len + 1;
     
     for (; i < len; ++i)
     {
         for (j = 0; j < sstr_len; ++j)
         {
             if (str[i] != s_str[j])
             {
                 break;
             }
         }
         
         if (j >= sstr_len)
         {
            return i;
         }
     }
     
     return  -1;
}

/*************** End  Simple Algorithm   ******************/

/***************  Start Rabin-Karp Algorithm  **************/

static const int mod_num = 13;

static int  get_exp(int len)
{
   int i = 1, exp = 1;
   
   while (i < len)
   {
       exp *= 10;
       ++i;
   }
   
   return  exp;
}

static int  init_weight(int * num, int len)
{
    int sum = num[len - 1];
    int exp = 1;
    int i;
    
    for (i = len - 2; i >= 0; --i)
    {
        exp *= 10;
        sum += num[i] * exp;
    }
    
    return  sum;
}

static int  other_weight(int old_sum, int * num, int low, int high)
{
    return  10 * (old_sum - get_exp(high - low) * num[low]) + num[high];
}

static void  trans_to_num(const char * str, int* num)
{
     int i = 0;
     int len = strlen(str);
     
     for (; i < len; ++i)
     {
         num[i] = str[i] % 10;
     }
}

static int  rabin_karp_search(const char * str, const char * s_str)
{
    int*  str_num = (int*)malloc(sizeof(int) * strlen(str));
    int*  sstr_num = (int*)malloc(sizeof(int) * strlen(s_str));
    
    if (str_num == NULL || sstr_num == NULL)
    {
         fprintf(stderr, "Memory Error!\n");
         return  -1;
    }
    else
    {
        trans_to_num(str, str_num);
        trans_to_num(s_str, sstr_num);
        
        int str_len = strlen(str), sstr_len = strlen(s_str);
        int len = str_len - sstr_len + 1;
        
        int   sstr_sum = 0;
        int   sstr_mod = 0, str_mod = 0;
        int*  str_sum = (int*)malloc(sizeof(int) * len);
        bool* status  = (bool*)malloc(sizeof(bool) * len); 
        
        if (str_sum == NULL || status == NULL)
        {
             fprintf(stderr, "Memory Error!\n");
             return -1;
        }
        
        sstr_sum = init_weight(sstr_num, sstr_len);
        str_sum[0] = init_weight(str_num, sstr_len);
        
        sstr_mod = sstr_sum % 10;
        str_mod = str_sum[0] % 10;
        
        status[0] = str_mod == sstr_mod ? true : false;
        
        int i;
    
        for (i = 1; i < len; ++i)
        {
            str_sum[i] = other_weight(str_sum[i - 1], str_num, i - 1, i + sstr_len - 1);
            str_mod = str_sum[i] % 10;
            status[i] = str_mod == sstr_mod ? true : false;
        }
        
        for (i = 0; i < len; ++i)
        {
            int j;
            
            for (j = i; j - i < sstr_len; ++j)
            {
                if (str[j] != s_str[j - i])
                {
                    break;
                }
            }
            
            if (j - i >= sstr_len)
            {
                return i;
            } 
        }
        
        return -1;
    }
}

/***************  End Rabin-Karp Algorithm  ****************/

/***************  Start KMP Algorithm  *********************/

typedef  struct 
{
    char  ch;
    int   next;
}   Chars;

static void  get_next(Chars * s_str, int len)
{
     s_str[0].next = -1;
     s_str[1].next = 0;
     
     int i = 0, j = 1;
     
     while (j < len - 1)
     {
         if (i == -1 || s_str[i].ch == s_str[j].ch)
         {
             ++i; ++j;
             s_str[j].next = i;
         }
         else
         {
             i = s_str[i].next;
         }
     }
}

static int  kmp_search(const char * str, const char * s_str)
{
     Chars* Sstr = (Chars*)malloc(strlen(s_str) * sizeof(Chars));
     
     if (Sstr == NULL)
     {
          fprintf(stderr, "Momery Error!\n");
          return -1;
     }
     else
     {
         int i, j;
         int sstr_len = strlen(s_str);
         int str_len = strlen(str);
         
         for (i = 0; i < sstr_len; ++i)
         {
             Sstr[i].ch = s_str[i];
             Sstr[i].next = 0;
         }
         
         get_next(Sstr, sstr_len);
         
         i = 0; j = 0;
         
         while (i < str_len && j < sstr_len)
         {
              if (j == -1 || str[i] == Sstr[j].ch)
              {
                 ++i; ++j;
              }
              else
              {
                 j = Sstr[j].next;
              }
         }
         
         return j >= sstr_len ? i - sstr_len : -1;
     }
}

/***************** End KMP Algorithm *********************/
#endif
