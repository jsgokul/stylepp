/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/* A checker for http link validity. Prints warning when it detect unreachable http address.  */
#include "common.h"
int
main (int argc, char **argv)
{
  test_indent_off (argv[1]);
  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0;
  char _buffer[100000], *buffer = _buffer + 1;
  char buffer2[100000];
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  _buffer[0] = ' ';
  while (fgets (buffer, 100000, stdin))
    {
      for (i = 0, j = 0; buffer[i]; )
	{
	  if (!cmp (buffer + i, "http://"))
	    {
	      // List of allowed characters is from http://en.wikipedia.org/wiki/Uniform_resource_locator.
	      int size = strspn (buffer + i, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!*'();:@&=+$,/?%#[]");
	      char command[1000];
	      strcpy (command, "curl -m 1 -f ");
	      char *command_arg = command + strlen (command);
	      for (k = 0; k < size; k++)
		{
		  command_arg[2 * k] = '\\';
		  command_arg[2 * k + 1] = buffer[i + k];
		}
	      command_arg[2 * size] = '\0';
	      strcat (command, " > /dev/null 2> /dev/null");
	      if (system (command))
		{
		  char link[1000];
		  memcpy (link, buffer + i, size);
		  fprintf (stderr, "%s: Invalid link: %s\n", argv[0], link);
		}
	    }

	  if (incomment)
	    {
	      if (!cmp (buffer + i, "*/"))
		incomment = 0;
	    }
	  else if (incomment2)
	    {
	      if (buffer[i] != '\\' && buffer[i + 1] == '\n')
		incomment2 = 0;
	    }
	  else if (insquote)
	    {
	      if (buffer[i] == '\\')
		buffer2[j++] = buffer[i++];
	      else if (buffer[i] == '\'')
		insquote = 0;
	    }
	  else if (indquote)
	    {
	      if (buffer[i] == '\\')
		buffer2[j++] = buffer[i++];
	      else if (buffer[i] == '"')
		indquote = 0;
	    }
	  else
	    {
	      if (!cmp (buffer + i, "/*"))
		incomment = 1;
	      if (!cmp (buffer + i, "//") || buffer[i] == '#')
		incomment2 = 1;

	      if (buffer[i] == '\'')
		insquote = 1;
	      if (buffer[i] == '"')
		indquote = 1;
	    }
	  buffer2[j++] = buffer[i++];
	skip:;
	}
      memcpy (ip, buffer, i); ip += i;
      memcpy (op, buffer2, j); op += j;
    }
  return write_if_common (input, ip, output, op);
}