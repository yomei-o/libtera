/******* BEGIN LICENCE BLOCK *******

    Sony Digital Photo Frame Application
    Copyright (C) 2008 Sony Corporation.

    This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License,version 2, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.

    Linking Sony Digital Photo Frame Application statically or dynamically with other modules is making a combined work based on Sony Digital Photo Frame Application. Thus, the terms and conditions of the GNU General Public License cover the whole combination.

    In addition, as a special exception, the copyright holders of Sony Digital Photo Frame Application give you permission to combine Sony Digital Photo Frame Application with free software programs or libraries that are released under the GNU LGPL and with code included in the standard release of Sony OS Abstraction Library (or modified versions of such code, with unchanged license). You may copy and distribute such a system following the terms of the GNU GPL for Sony Digital Photo Frame Application and the licenses of the other code concerned{, provided that you include the source code of that other code when and as the GNU GPL requires distribution of source code}.

    Note that people who make modified versions of Sony Digital Photo Frame Application are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU General Public License gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.

******* END LICENCE BLOCK *******/


 
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mygetopt.h"

#define BAD_OPTION '\0'

char *myoptarg = 0;
int myoptind = 0;
static char *nextchar;
int myopterr = 1;
int myoptopt = BAD_OPTION;

static enum
{
  REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER
} ordering;



static int my_strlen (const char *str)
{
  int n = 0;
  while (*str++)
    n++;
  return n;
}

static char * my_index (const char* str,int chr)
{
  while (*str)
    {
      if (*str == chr)
	return (char *) str;
      str++;
    }
  return 0;
}


static int first_nonopt;
static int last_nonopt;


static void exchange (char **argv)
{
  char *temp, **first, **last;

  /* Reverse all the elements [first_nonopt, optind) */
  first = &argv[first_nonopt];
  last  = &argv[myoptind-1];
  while (first < last) {
    temp = *first; *first = *last; *last = temp; first++; last--;
  }
  /* Put back the options in order */
  first = &argv[first_nonopt];
  first_nonopt += (myoptind - last_nonopt);
  last  = &argv[first_nonopt - 1];
  while (first < last) {
    temp = *first; *first = *last; *last = temp; first++; last--;
  }

  /* Put back the non options in order */
  first = &argv[first_nonopt];
  last_nonopt = myoptind;
  last  = &argv[last_nonopt-1];
  while (first < last) {
    temp = *first; *first = *last; *last = temp; first++; last--;
  }
}

int _getopt_internal (int argc,char *const *argv,const char * optstring,
					  const struct myoption * longopts,int* longind,int long_only)
{
  int option_index;

  myoptarg = 0;

  /* Initialize the internal data when the first call is made.
     Start processing options with ARGV-element 1 (since ARGV-element 0
     is the program name); the sequence of previously skipped
     non-option ARGV-elements is empty.  */

  if (myoptind == 0)
    {
      first_nonopt = last_nonopt = myoptind = 1;

      nextchar = NULL;

      /* Determine how to handle the ordering of options and nonoptions.  */

      if (optstring[0] == '-')
	{
	  ordering = RETURN_IN_ORDER;
	  ++optstring;
	}
      else if (optstring[0] == '+')
	{
	  ordering = REQUIRE_ORDER;
	  ++optstring;
	}
      else if (getenv ("POSIXLY_CORRECT") != NULL)
	ordering = REQUIRE_ORDER;
      else
	ordering = PERMUTE;
    }

  if (nextchar == NULL || *nextchar == '\0')
    {
      if (ordering == PERMUTE)
	{
	  /* If we have just processed some options following some non-options,
	     exchange them so that the options come first.  */

	  if (first_nonopt != last_nonopt && last_nonopt != myoptind)
	    exchange ((char **) argv);
	  else if (last_nonopt != myoptind)
	    first_nonopt = myoptind;

	  /* Now skip any additional non-options
	     and extend the range of non-options previously skipped.  */

	  while (myoptind < argc
		 && (argv[myoptind][0] != '-' || argv[myoptind][1] == '\0')
#ifdef GETOPT_COMPAT
		 && (longopts == NULL
		     || argv[myoptind][0] != '+' || argv[myoptind][1] == '\0')
#endif				/* GETOPT_COMPAT */
		 )
	    myoptind++;
	  last_nonopt = myoptind;
	}

      /* Special ARGV-element `--' means premature end of options.
	 Skip it like a null option,
	 then exchange with previous non-options as if it were an option,
	 then skip everything else like a non-option.  */

      if (myoptind != argc && !strcmp (argv[myoptind], "--"))
	{
	  myoptind++;

	  if (first_nonopt != last_nonopt && last_nonopt != myoptind)
	    exchange ((char **) argv);
	  else if (first_nonopt == last_nonopt)
	    first_nonopt = myoptind;
	  last_nonopt = argc;

	  myoptind = argc;
	}

      /* If we have done all the ARGV-elements, stop the scan
	 and back over any non-options that we skipped and permuted.  */

      if (myoptind == argc)
	{
	  /* Set the next-arg-index to point at the non-options
	     that we previously skipped, so the caller will digest them.  */
	  if (first_nonopt != last_nonopt)
	    myoptind = first_nonopt;
	  return EOF;
	}

      /* If we have come to a non-option and did not permute it,
	 either stop the scan or describe it to the caller and pass it by.  */

      if ((argv[myoptind][0] != '-' || argv[myoptind][1] == '\0')
#ifdef GETOPT_COMPAT
	  && (longopts == NULL
	      || argv[myoptind][0] != '+' || argv[myoptind][1] == '\0')
#endif				/* GETOPT_COMPAT */
	  )
	{
	  if (ordering == REQUIRE_ORDER)
	    return EOF;
	  myoptarg = argv[myoptind++];
	  return 1;
	}

      /* We have found another option-ARGV-element.
	 Start decoding its characters.  */

      nextchar = (argv[myoptind] + 1
		  + (longopts != NULL && argv[myoptind][1] == '-'));
    }

  if (longopts != NULL
      && ((argv[myoptind][0] == '-'
	   && (argv[myoptind][1] == '-' || long_only))
#ifdef GETOPT_COMPAT
	  || argv[myoptind][0] == '+'
#endif				/* GETOPT_COMPAT */
	  ))
    {
      const struct myoption *p;
      char *s = nextchar;
      int exact = 0;
      int ambig = 0;
      const struct myoption *pfound = NULL;
      int indfound = 0;

      while (*s && *s != '=')
	s++;

      /* Test all options for either exact match or abbreviated matches.  */
      for (p = longopts, option_index = 0; p->name;
	   p++, option_index++)
	if (!strncmp (p->name, nextchar, s - nextchar))
	  {
	    if (s - nextchar == my_strlen (p->name))
	      {
		/* Exact match found.  */
		pfound = p;
		indfound = option_index;
		exact = 1;
		break;
	      }
	    else if (pfound == NULL)
	      {
		/* First nonexact match found.  */
		pfound = p;
		indfound = option_index;
	      }
	    else
	      /* Second nonexact match found.  */
	      ambig = 1;
	  }

      if (ambig && !exact)
	{
	  if (myopterr)
	    fprintf (stderr, "%s: option `%s' is ambiguous\n",
		     argv[0], argv[myoptind]);
	  nextchar += my_strlen (nextchar);
	  myoptind++;
	  return BAD_OPTION;
	}

      if (pfound != NULL)
	{
	  option_index = indfound;
	  myoptind++;
	  if (*s)
	    {
	      /* Don't test has_arg with >, because some C compilers don't
		 allow it to be used on enums.  */
	      if (pfound->has_arg)
		myoptarg = s + 1;
	      else
		{
		  if (myopterr)
		    {
		      if (argv[myoptind - 1][1] == '-')
			/* --option */
			fprintf (stderr,
				 "%s: option `--%s' doesn't allow an argument\n",
				 argv[0], pfound->name);
		      else
			/* +option or -option */
			fprintf (stderr,
			     "%s: option `%c%s' doesn't allow an argument\n",
			     argv[0], argv[myoptind - 1][0], pfound->name);
		    }
		  nextchar += my_strlen (nextchar);
		  return BAD_OPTION;
		}
	    }
	  else if (pfound->has_arg == 1)
	    {
	      if (myoptind < argc)
		myoptarg = argv[myoptind++];
	      else
		{
		  if (myopterr)
		    fprintf (stderr, "%s: option `%s' requires an argument\n",
			     argv[0], argv[myoptind - 1]);
		  nextchar += my_strlen (nextchar);
		  return optstring[0] == ':' ? ':' : BAD_OPTION;
		}
	    }
	  nextchar += my_strlen (nextchar);
	  if (longind != NULL)
	    *longind = option_index;
	  if (pfound->flag)
	    {
	      *(pfound->flag) = pfound->val;
	      return 0;
	    }
	  return pfound->val;
	}
      /* Can't find it as a long option.  If this is not getopt_long_only,
	 or the option starts with '--' or is not a valid short
	 option, then it's an error.
	 Otherwise interpret it as a short option.  */
      if (!long_only || argv[myoptind][1] == '-'
#ifdef GETOPT_COMPAT
	  || argv[myoptind][0] == '+'
#endif				/* GETOPT_COMPAT */
	  || my_index (optstring, *nextchar) == NULL)
	{
	  if (myopterr)
	    {
	      if (argv[myoptind][1] == '-')
		/* --option */
		fprintf (stderr, "%s: unrecognized option `--%s'\n",
			 argv[0], nextchar);
	      else
		/* +option or -option */
		fprintf (stderr, "%s: unrecognized option `%c%s'\n",
			 argv[0], argv[myoptind][0], nextchar);
	    }
	  nextchar = (char *) "";
	  myoptind++;
	  return BAD_OPTION;
	}
    }

  /* Look at and handle the next option-character.  */

  {
    char c = *nextchar++;
    char *temp = my_index (optstring, c);

    /* Increment `optind' when we start to process its last character.  */
    if (*nextchar == '\0')
      ++myoptind;

    if (temp == NULL || c == ':')
      {
	if (myopterr)
	  {
#if 0
	    if (c < 040 || c >= 0177)
	      fprintf (stderr, "%s: unrecognized option, character code 0%o\n",
		       argv[0], c);
	    else
	      fprintf (stderr, "%s: unrecognized option `-%c'\n", argv[0], c);
#else
	    /* 1003.2 specifies the format of this message.  */
	    fprintf (stderr, "%s: illegal option -- %c\n", argv[0], c);
#endif
	  }
	myoptopt = c;
	return BAD_OPTION;
      }
    if (temp[1] == ':')
      {
	if (temp[2] == ':')
	  {
	    /* This is an option that accepts an argument optionally.  */
	    if (*nextchar != '\0')
	      {
		myoptarg = nextchar;
		myoptind++;
	      }
	    else
	      myoptarg = 0;
	    nextchar = NULL;
	  }
	else
	  {
	    /* This is an option that requires an argument.  */
	    if (*nextchar != '\0')
	      {
		myoptarg = nextchar;
		/* If we end this ARGV-element by taking the rest as an arg,
		   we must advance to the next element now.  */
		myoptind++;
	      }
	    else if (myoptind == argc)
	      {
		if (myopterr)
		  {
#if 0
		    fprintf (stderr, "%s: option `-%c' requires an argument\n",
			     argv[0], c);
#else
		    /* 1003.2 specifies the format of this message.  */
		    fprintf (stderr, "%s: option requires an argument -- %c\n",
			     argv[0], c);
#endif
		  }
		myoptopt = c;
		if (optstring[0] == ':')
		  c = ':';
		else
		  c = BAD_OPTION;
	      }
	    else
	      /* We already incremented `optind' once;
		 increment it again when taking next ARGV-elt as argument.  */
	      myoptarg = argv[myoptind++];
	    nextchar = NULL;
	  }
      }
    return c;
  }
}




int
mygetopt (int argc,char *const *argv,const char *optstring)
{
  return _getopt_internal (argc, argv, optstring,
			   (const struct myoption *) 0,
			   (int *) 0,
			   0);
}





#if 0


int main (int argc,char* argv[])
{
  int c;
  int digit_optind = 0;

  while (1)
    {
      int this_option_optind = myoptind ? myoptind : 1;

      c = mygetopt (argc, argv, "abc:d:0123456789");
      if (c == EOF)
	break;

      switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  if (digit_optind != 0 && digit_optind != this_option_optind)
	    printf ("digits occur in two different argv-elements.\n");
	  digit_optind = this_option_optind;
	  printf ("option %c\n", c);
	  break;

	case 'a':
	  printf ("option a\n");
	  break;

	case 'b':
	  printf ("option b\n");
	  break;

	case 'c':
	  printf ("option c with value `%s'\n", myoptarg);
	  break;

	case BAD_OPTION:
	  break;

	default:
	  printf ("?? getopt returned character code 0%o ??\n", c);
	}
    }

  if (myoptind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (myoptind < argc)
	printf ("%s ", argv[myoptind++]);
      printf ("\n");
    }

  exit (0);
}

#endif /* TEST */





