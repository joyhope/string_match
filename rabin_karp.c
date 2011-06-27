#include<stdio.h>
#include<string.h>
#define PRIME_BASE 59999
#define MOD 60000

/*
  Checks if char* q starts with char* p
 */
int strstarts(const char *p, const char *q)
{
  while(*p && *p == *q){
    ++p;
    ++q;
  }
  // if *p is null return true else return diff
  return *p ? *p - *q: 0;
}

int hash(const char *str, int len)
{
  int i,h;
  for(i = h = 0; i < len; ++i){
    h = h * PRIME_BASE + str[i];
    if(h >= MOD) h %= MOD;
  }
  return h;
}

int rehash(int hash, char remove, char add, int maxpow)
{
  int h = (hash * PRIME_BASE) % MOD;
  h = (h + add) % MOD;
  h = h - remove * maxpow;
  return h % MOD;
  //((hash * PRIME_BASE) - (remove * maxpow) + add;
}

int mod_pow(int base, int pow, int mod)
{
  int ret = 1;
  while(pow){
    if((pow % 2) == 1){
      ret = ret * base;
      if(ret >= MOD) ret %= MOD;
    }
    base = base * base;
    if(base >= MOD) base %= MOD;
    pow /= 2;
  }
  return ret;
}

/*
  Implemented using rolling hash function.
 */
int rabin_karp(const char *needle, const char *haystack)
{
  // Rolling hash function for needle is calculated.
  int len = strlen(needle);
  int maxpow = mod_pow(PRIME_BASE, len, MOD);
  int hash_needle = hash(needle, len);
  int roll_hash = hash(haystack, len);

  const char *win_start = haystack;
  const char *win_end = haystack + len;
  for(;hash_needle != roll_hash; ++win_end, ++win_start){
    if(*win_end == '\0')
      return -1;
    roll_hash = rehash(roll_hash, *win_start, *win_end, maxpow);
  }
  return strstarts(needle, win_start);
}

int main(int argc, char **argv)
{
#if DEBUG
  {
    char needle[100];
    char haystack[1000];
    while(scanf("%s %s", needle, haystack) != -1){
      int index = rabin_karp(needle, haystack);
      if(index >= 0)
        printf("Found %s in %s at %d\n", needle, haystack, index);
      else
        printf("Failed to find %s in %s\n", needle, haystack);
    }
  }
#elif
  if(argc < 3){
    printf("Usage : <command> <pattern> <string>\n");
    return 0;
  }
  int index = rabin_karp(argv[1], argv[2]);
  if(index >= 0)
    printf("Found %s in %s at %d\n", argv[1], argv[2], index);
  else
    printf("Failed to find %s in %s\n", argv[1], argv[2]);
#endif
  return 0;
}