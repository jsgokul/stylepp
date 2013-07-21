/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

int
isword (char c)
{
  return isalpha (c) || c == '\'' || c == '-';
}

int
isseparator (char *c)
{
  return isspace (c[0]) || ((c[0] == '.' || c[0] == ',' || c[0] == ';') && isspace (c[1]));
}


#define HASH_SIZE 1000000
char _word[1000];
char *
word (char *c)
{
  int i;
  for (i = 0; isword (c[i]); i++)
    _word[i] = c[i];
  _word[i] = '\0';
  return _word;
}

HASH_TYPE code_words[HASH_SIZE];
unsigned int
hash (char *s)
{
  int collect = 0;
  while (*s)
    {
      collect = 11 * collect + *s;
      s++;
    }
  return collect;
}
static int
add_word (char *word, HASH_TYPE value)
{
  int h = hash (word) % HASH_SIZE;
  if (code_words[h])
    return 0;
  code_words[h] = value;
  return 1;
}

static HASH_TYPE
get_word (char *word)
{
  int h = hash (word) % HASH_SIZE;
  return code_words[h];
}
