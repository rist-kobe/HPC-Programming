/* Copyright 2024 Research Organization for Information Science and Technology */
/*---------------------------------------------------------------------
  Check endianness
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: June 1 2018

  Description:
  Expected output of 0x12345678 
  1. Big Endian
  Byte-ID:  0  1  2  3
           12 34 56 78 
  2. Little Endian
  Byte-ID:  0  1  2  3
           78 56 34 12 

  Remark 1: Endianness only affects byte order. 
  Since char is exactly 1 BYTE, it is the same as
  all endianness.

  Remark 2: You can check everything, via hexdump or xxd.
  (ex) hexdump -C -v output_native
       hexdump -C -v output_swap
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint16_t bswap_uint16( uint16_t ) ;
int16_t  bswap_int16( int16_t   ) ;
uint32_t bswap_uint32( uint32_t ) ;
int32_t  bswap_int32( int32_t   ) ;
void bswap_float( float * ) ;
void bswap_double( double * ) ;
void bswap_uint16_array(uint16_t *restrict, const int ) ;
void bswap_int16_array(int16_t *restrict, const int ) ;
void bswap_uint32_array(uint32_t *restrict, const int ) ;
void bswap_int32_array(int32_t *restrict, const int) ;
void bswap_float_array( float *restrict, const int ) ;
void bswap_double_array( double *restrict, const int ) ;
#ifdef __BIGINT__
int64_t bswap_int64( int64_t  ) ;
void bswap_int64_array(int64_t *restrict, const int) ;
#endif

int main ( int argc, char** argv ) 
{
  const int len = 3 ;
  char name[64] ;

  uint16_t usa ;
  int16_t  sa[len] ;
  uint32_t ua[len] ;
  int32_t  a[len]  ;
  float    fw[len] ;
  double   dw[len] ;

  strcpy( name, "Is your array big or little?") ;
  usa    = 0x1234 ;     /* 4660      */
  sa[0]  = -4660 ;
  ua[0]  = 0x12345678 ; /* 305419896 */
  a[0]   = -305419896 ;
  fw[0]  = 1.1f ;
  dw[0]  = -3.14159265359 ;
#ifdef __BIGINT__
  int64_t la[3] ;
  la[0]  = 0x123456789ABCDEF0 ;
#endif
  for ( int i=1; i<len; ++i ) {
     sa[i]  = sa[i-1]  + (int16_t)1 ;
     ua[i]  = ua[i-1]  + (uint32_t)1 ;
     a[i]   = a[i-1]   + (int)1 ;
     fw[i]  = fw[i-1]  + 1.0f ;
     dw[i]  = dw[i-1]  + 1.0 ;
#ifdef __BIGINT__
     la[i]  = la[i-1]  + (int64_t)1 ;
#endif
  }

  /* header */
  printf("[Check endianness]\n") ;
  printf("Files, output_native and output_swap, include characters\n"
    " and numbers with binary data.\n") ;
  printf("  uint16_t usa   = 0x1234\n") ;
  printf("  int16_t  sa[0] = -4660 \n") ;
  printf("  uint32_t ua[0] = 0x12345678 \n") ;
  printf("  int32_t  a[0]  = -305419896 \n") ;
  printf("  float    fw[0] = 1.1f \n") ;
  printf("  double   dw[0] = 3.14156265359 \n") ;
#ifdef __BIGINT__
  printf("  int64_t  la[0] = 0x123456789ABCDEF0\n") ;
#endif
  printf("Each array increases just by 1 as the index.\n") ;
  printf("Remark: You can check everything, via hexdump or xxd.\n") ; 
  printf("        hexdump -C -v output_native\n") ; 
  printf("        hexdump -C -v output_swap\n") ; 

  /* native */
  {
    FILE *fp = fopen("output_native","wb") ;
    int sep = -1 ;
    fputs(name, fp) ;
    fwrite(&sep,sizeof(int),      1  , fp);
    fwrite(&usa,sizeof(uint16_t), 1  , fp);
    fwrite(&sa, sizeof(int16_t ), len, fp);
    fwrite(&ua, sizeof(uint32_t), len, fp);
    fwrite(&a,  sizeof(int32_t ), len, fp);
#ifdef __BIGINT__
    fwrite(&la, sizeof(int64_t ), len, fp);
#endif
    fwrite(&fw, sizeof(float   ), len, fp);
    fwrite(&dw, sizeof(double  ), len, fp); 
    fclose(fp) ;
  }

  /* byte swap */
  uint16_t usa_bs = bswap_uint16( usa ) ;
  bswap_int16_array ( sa, len) ;
  bswap_uint32_array( ua, len) ;
  bswap_int32_array ( a,  len) ;
  bswap_float_array ( fw, len) ;
  bswap_double_array( dw, len) ;
#ifdef __BIGINT__
  bswap_int64_array ( la, len) ;
#endif

  {
    FILE *fp = fopen("output_swap","wb") ;
    int sep = -1 ;
    fputs(name, fp) ;
    fwrite(&sep,sizeof(int),      1  , fp);
    fwrite(&usa_bs,sizeof(uint16_t), 1,   fp);
    fwrite(&sa,    sizeof(int16_t ), len, fp);
    fwrite(&ua,    sizeof(uint32_t), len, fp);
    fwrite(&a,     sizeof(int32_t ), len, fp);
#ifdef __BIGINT__
    fwrite(&la,    sizeof(int64_t ), len, fp);
#endif
    fwrite(&fw,    sizeof(float   ), len, fp);
    fwrite(&dw,    sizeof(double  ), len, fp); 
    fclose(fp) ;
  }

  return EXIT_SUCCESS ;
}

// Byte swap unsigned 16-bit int
uint16_t bswap_uint16( uint16_t val ) 
{
  return ( val << 8 ) | ( val >> 8 ) ;
}
// Byte swap 16-bit int
int16_t bswap_int16( int16_t val ) 
{
  return ( val << 8 ) | ( (val >> 8)&0xFF ) ;
}
// Byte swap unsigned 32-bit int
uint32_t bswap_uint32( uint32_t val ) 
{
  return ( val << 24 )
    |( (val&0x0000FF00) << 8 )
    |( (val&0x00FF0000) >> 8 )
    |( val >> 24 ) ;
}
// Byte swap 32-bit int
int32_t bswap_int32( int32_t val ) 
{
  return ( val << 24 )
    |( (val&0x0000FF00) << 8 )
    |( ((val&0x00FF0000) >> 8)&0x0000FF00 )
    |( (val >> 24)&0x000000FF ) ;
}
// Byte swap 32-bit FP
void bswap_float( float *v )
{
  char si[4], so[4] ;
  memcpy(si,v,4) ;
  so[0] = si[3] ;
  so[1] = si[2] ;
  so[2] = si[1] ;
  so[3] = si[0] ;
  memcpy(v,so,4) ;
}
// Byte swap 64-bit FP
void bswap_double( double *v )
{
  char si[8], so[8] ;
  memcpy(si,v,8) ;
  so[0] = si[7] ;
  so[1] = si[6] ;
  so[2] = si[5] ;
  so[3] = si[4] ;
  so[4] = si[3] ;
  so[5] = si[2] ;
  so[6] = si[1] ;
  so[7] = si[0] ;
  memcpy(v,so,8) ;
}
// Byte swap unsigned 16-bit int array
void bswap_uint16_array(uint16_t *restrict v, const int len)
{
  for ( int i=0; i<len; ++i ) {
    uint16_t w = v[i] ;
    v[i] = ( w << 8 ) | ( w >> 8 ) ;
  }
}
// Byte swap 16-bit int array
void bswap_int16_array(int16_t *restrict v, const int len)
{
  for ( int i=0; i<len; ++i ) {
    int16_t w = v[i] ;
    v[i] =  ( w << 8 ) | ( ( w >> 8) & 0xFF ) ;
  }
}
// Byte swap unsigned 32-bit int array
void bswap_uint32_array(uint32_t *restrict v, const int len)
{
  for ( int i=0; i<len; ++i ) {
    uint32_t w = v[i] ;
    v[i] = ( w << 24 )
              |( (w & 0x0000FF00) << 8 )
              |( (w & 0x00FF0000) >> 8 )
              |( w >> 24 ) ;
  }
}
// Byte swap 32-bit int array
void bswap_int32_array(int32_t *restrict v, const int len)
{
  for ( int i=0; i<len; ++i ) {
    int32_t w = v[i] ;
    v[i] = ( w << 24 )
              |( (w & 0x0000FF00) << 8 )
              |( ((w & 0x00FF0000) >> 8) & 0x0000FF00 )
              |( (w >> 24) & 0x000000FF ) ;
  }
}
// Byte swap 32-bit FP array
void bswap_float_array( float *restrict v, const int len )
{
  char si[4], so[4] ;
  for ( int i=0; i<len; ++i ) {
    memcpy(si,v+i,4) ;
    so[0] = si[3] ;
    so[1] = si[2] ;
    so[2] = si[1] ;
    so[3] = si[0] ;
    memcpy(v+i,so,4) ;
  }
}
// Byte swap 64-bit FP array
void bswap_double_array( double *restrict v, const int len )
{
  char si[8], so[8] ;
  for ( int i=0; i<len; ++i ) {
    memcpy(si,v+i,8) ;
    so[0] = si[7] ;
    so[1] = si[6] ;
    so[2] = si[5] ;
    so[3] = si[4] ;
    so[4] = si[3] ;
    so[5] = si[2] ;
    so[6] = si[1] ;
    so[7] = si[0] ;
    memcpy(v+i,so,8) ;
  }
}
#ifdef __BIGINT__
// Byte swap 64-bit int
int64_t bswap_int64( int64_t val ) 
{
  return ( val << 56 )
    |( (val&0x000000000000FF00) << 40 )
    |( (val&0x0000000000FF0000) << 24 )
    |( (val&0x00000000FF000000) << 8  )
    |( ((val&0x000000FF00000000) >> 8 )&0x00000000FF000000  )
    |( ((val&0x0000FF0000000000) >> 24)&0x0000000000FF0000  )
    |( ((val&0x00FF000000000000) >> 40)&0x000000000000FF00  )
    |( (val >> 56)&0x00000000000000FF ) ;
}
// Byte swap 64-bit int array
void bswap_int64_array(int64_t *restrict v, const int len)
{
  for ( int i=0; i<len; ++i ) {
    int64_t w = v[i] ;
    v[i] = ( w << 56 )
              |( (w & 0x000000000000FF00) << 40 )
              |( (w & 0x0000000000FF0000) << 24 )
              |( (w & 0x00000000FF000000) << 8  )
              |( ((w & 0x000000FF00000000) >> 8 )&0x00000000FF000000 )
              |( ((w & 0x0000FF0000000000) >> 24)&0x0000000000FF0000 )
              |( ((w & 0x00FF000000000000) >> 40)&0x000000000000FF00 )
              |( (w >> 56)&0x00000000000000FF ) ;
  }
}
#endif
