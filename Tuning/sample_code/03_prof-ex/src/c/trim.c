/* Copyright 2024 Research Organization for Information Science and Technology */
/*-------------------------------------------------------------------*/
/* trim.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 28 July 2017                                         */
/*-------------------------------------------------------------------*/
#include <string.h>
#include <ctype.h>
#include "trim.h"

/*--------------------------------------------------------------------*/
/* ltrim: trimming space from head                                     */
/*--------------------------------------------------------------------*/
char *ltrim(char *s)
{
    if (s && *s) {
        size_t len = strlen(s);
        size_t lws = strspn(s, " \n\r\t\v");
        memmove(s, s + lws, len -= lws);
        s[len] = '\0';
    }
    return s;
}
/*--------------------------------------------------------------------*/
/* rtrim: trimming space from tail                                     */
/*--------------------------------------------------------------------*/
char *rtrim(char *s)
{
    if (s && *s) {
        size_t len = strlen(s);
        while (len && isspace(s[len-1])) --len;
        s[len] = '\0';
    }
    return s;
}
/*--------------------------------------------------------------------*/
/* trim: trimming space                                                */
/*--------------------------------------------------------------------*/
char *trim(char *s)
{
  return ltrim(rtrim(s));
}