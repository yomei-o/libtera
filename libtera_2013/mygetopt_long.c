/******* BEGIN LICENCE BLOCK *******

    libtera 
    Copyright (C) 2012 Yomei

    This program is free software; you can redistribute it and/or modify it under the terms of the BSD License.

******* END LICENCE BLOCK *******/

#include <stdlib.h>
#include <string.h>

#include "mygetopt_long.h"


int	myopterr = 1;		/* if error message should be printed */
int	myoptind = 1;		/* index into parent argv vector */
int	myoptopt = '?';		/* character checked for validity */
char    *myoptarg = NULL;		/* argument associated with option */
int	myoptreset = 0;		/* reset getopt */

#define PRINT_ERROR	((myopterr) && (*options != ':'))

#define FLAG_PERMUTE	0x01	/* permute non-options to the end of argv */
#define FLAG_ALLARGS	0x02	/* treat non-options as args to option "-1" */
#define FLAG_LONGONLY	0x04	/* operate as getopt_long_only */

/* return values */
#define	BADCH		(int)'?'
#define	BADARG		((*options == ':') ? (int)':' : (int)'?')
#define	INORDER 	(int)1

#define	EMSG		""

static int getopt_internal(int, char * const *, const char *,
			   const struct myoption *, int *, int);
static int parse_long_options(char * const *, const char *,
			      const struct myoption *, int *, int);
static int gcd(int, int);
static void permute_args(int, int, int, char * const *);

static char *place = EMSG; /* option letter processing */

/* XXX: set optreset to 1 rather than these two */
static int nonopt_start = -1; /* first non option argument (for permute) */
static int nonopt_end = -1;   /* first option after non options (for permute) */

/* Error messages */
static const char recargchar[] = "option requires an argument -- %c";
static const char recargstring[] = "option requires an argument -- %s";
static const char ambig[] = "ambiguous option -- %.*s";
static const char noarg[] = "option doesn't take an argument -- %.*s";
static const char illoptchar[] = "unknown option -- %c";
static const char illoptstring[] = "unknown option -- %s";

/*
 * Compute the greatest common divisor of a and b.
 */
static int
gcd(int a, int b)
{
	int c;

	c = a % b;
	while (c != 0) {
		a = b;
		b = c;
		c = a % b;
	}

	return (b);
}

/*
 * Exchange the block from nonopt_start to nonopt_end with the block
 * from nonopt_end to opt_end (keeping the same order of arguments
 * in each block).
 */
static void
permute_args(int panonopt_start, int panonopt_end, int opt_end,
	char * const *nargv)
{
	int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
	char *swap;

	/*
	 * compute lengths of blocks and number and size of cycles
	 */
	nnonopts = panonopt_end - panonopt_start;
	nopts = opt_end - panonopt_end;
	ncycle = gcd(nnonopts, nopts);
	cyclelen = (opt_end - panonopt_start) / ncycle;

	for (i = 0; i < ncycle; i++) {
		cstart = panonopt_end+i;
		pos = cstart;
		for (j = 0; j < cyclelen; j++) {
			if (pos >= panonopt_end)
				pos -= nnonopts;
			else
				pos += nopts;
			swap = nargv[pos];
			/* LINTED const cast */
			((char **) nargv)[pos] = nargv[cstart];
			/* LINTED const cast */
			((char **)nargv)[cstart] = swap;
		}
	}
}

/*
 * parse_long_options --
 *	Parse long options in argc/argv argument vector.
 * Returns -1 if short_too is set and the option does not match long_options.
 */
static int
parse_long_options(char * const *nargv, const char *options,
	const struct myoption *long_options, int *idx, int short_too)
{
	char *current_argv, *has_equal;
	size_t current_argv_len;
	int i, match;

	current_argv = place;
	match = -1;

	myoptind++;

	if ((has_equal = strchr(current_argv, '=')) != NULL) {
		/* argument found (--option=arg) */
		current_argv_len = has_equal - current_argv;
		has_equal++;
	} else
		current_argv_len = strlen(current_argv);

	for (i = 0; long_options[i].name; i++) {
		/* find matching long option */
		if (strncmp(current_argv, long_options[i].name,
		    current_argv_len))
			continue;

		if (strlen(long_options[i].name) == current_argv_len) {
			/* exact match */
			match = i;
			break;
		}
		/*
		 * If this is a known short option, don't allow
		 * a partial match of a single character.
		 */
		if (short_too && current_argv_len == 1)
			continue;

		if (match == -1)	/* partial match */
			match = i;
		else {
			/* ambiguous abbreviation */
#if 0 /* yomei */
			if (PRINT_ERROR)
				warnx(ambig, (int)current_argv_len,
				     current_argv);
#endif
			myoptopt = 0;
			return (BADCH);
		}
	}
	if (match != -1) {		/* option found */
		if (long_options[match].has_arg == no_argument
		    && has_equal) {
#if 0 /* yomei */
			if (PRINT_ERROR)
				warnx(noarg, (int)current_argv_len,
				     current_argv);
#endif
			/*
			 * XXX: GNU sets optopt to val regardless of flag
			 */
			if (long_options[match].flag == NULL)
				myoptopt = long_options[match].val;
			else
				myoptopt = 0;
			return (BADARG);
		}
		if (long_options[match].has_arg == required_argument ||
		    long_options[match].has_arg == optional_argument) {
			if (has_equal)
				myoptarg = has_equal;
			else if (long_options[match].has_arg ==
			    required_argument) {
				/*
				 * optional argument doesn't use next nargv
				 */
				myoptarg = nargv[myoptind++];
			}
		}
		if ((long_options[match].has_arg == required_argument)
		    && (myoptarg == NULL)) {
			/*
			 * Missing argument; leading ':' indicates no error
			 * should be generated.
			 */
#if 0 /* yomei */
			if (PRINT_ERROR)
				warnx(recargstring,
				    current_argv);
#endif
			/*
			 * XXX: GNU sets optopt to val regardless of flag
			 */
			if (long_options[match].flag == NULL)
				myoptopt = long_options[match].val;
			else
				myoptopt = 0;
			--myoptind;
			return (BADARG);
		}
	} else {			/* unknown option */
		if (short_too) {
			--myoptind;
			return (-1);
		}
#if 0 /* yomei */
		if (PRINT_ERROR)
			warnx(illoptstring, current_argv);
#endif
		myoptopt = 0;
		return (BADCH);
	}
	if (idx)
		*idx = match;
	if (long_options[match].flag) {
		*long_options[match].flag = long_options[match].val;
		return (0);
	} else
		return (long_options[match].val);
}

/*
 * getopt_internal --
 *	Parse argc/argv argument vector.  Called by user level routines.
 */
static int
getopt_internal(int nargc, char * const *nargv, const char *options,
	const struct myoption *long_options, int *idx, int flags)
{
	char *oli;				/* option letter list index */
	int optchar, short_too;
	static int posixly_correct = -1;

	if (options == NULL)
		return (-1);

	/*
	 * Disable GNU extensions if POSIXLY_CORRECT is set or options
	 * string begins with a '+'.
	 */
	if (posixly_correct == -1)
		posixly_correct = (getenv("POSIXLY_CORRECT") != NULL);
	if (posixly_correct || *options == '+')
		flags &= ~FLAG_PERMUTE;
	else if (*options == '-')
		flags |= FLAG_ALLARGS;
	if (*options == '+' || *options == '-')
		options++;

	/*
	 * XXX Some GNU programs (like cvs) set optind to 0 instead of
	 * XXX using optreset.  Work around this braindamage.
	 */
	if (myoptind == 0)
		myoptind = myoptreset = 1;

	myoptarg = NULL;
	if (myoptreset)
		nonopt_start = nonopt_end = -1;
start:
	if (myoptreset || !*place) {		/* update scanning pointer */
		myoptreset = 0;
		if (myoptind >= nargc) {          /* end of argument vector */
			place = EMSG;
			if (nonopt_end != -1) {
				/* do permutation, if we have to */
				permute_args(nonopt_start, nonopt_end,
				    myoptind, nargv);
				myoptind -= nonopt_end - nonopt_start;
			}
			else if (nonopt_start != -1) {
				/*
				 * If we skipped non-options, set optind
				 * to the first of them.
				 */
				myoptind = nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return (-1);
		}
		if (*(place = nargv[myoptind]) != '-' ||
		    (place[1] == '\0' && strchr(options, '-') == NULL)) {
			place = EMSG;		/* found non-option */
			if (flags & FLAG_ALLARGS) {
				/*
				 * GNU extension:
				 * return non-option as argument to option 1
				 */
				myoptarg = nargv[myoptind++];
				return (INORDER);
			}
			if (!(flags & FLAG_PERMUTE)) {
				/*
				 * If no permutation wanted, stop parsing
				 * at first non-option.
				 */
				return (-1);
			}
			/* do permutation */
			if (nonopt_start == -1)
				nonopt_start = myoptind;
			else if (nonopt_end != -1) {
				permute_args(nonopt_start, nonopt_end,
				    myoptind, nargv);
				nonopt_start = myoptind -
				    (nonopt_end - nonopt_start);
				nonopt_end = -1;
			}
			myoptind++;
			/* process next argument */
			goto start;
		}
		if (nonopt_start != -1 && nonopt_end == -1)
			nonopt_end = myoptind;

		/*
		 * Check for "--" or "--foo" with no long options
		 * but if place is simply "-" leave it unmolested.
		 */
		if (place[1] != '\0' && *++place == '-' &&
		    (place[1] == '\0' || long_options == NULL)) {
			myoptind++;
			place = EMSG;
			/*
			 * We found an option (--), so if we skipped
			 * non-options, we have to permute.
			 */
			if (nonopt_end != -1) {
				permute_args(nonopt_start, nonopt_end,
				    myoptind, nargv);
				myoptind -= nonopt_end - nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return (-1);
		}
	}

	/*
	 * Check long options if:
	 *  1) we were passed some
	 *  2) the arg is not just "-"
	 *  3) either the arg starts with -- we are getopt_long_only()
	 */
	if (long_options != NULL && place != nargv[myoptind] &&
	    (*place == '-' || (flags & FLAG_LONGONLY))) {
		short_too = 0;
		if (*place == '-')
			place++;		/* --foo long option */
		else if (*place != ':' && strchr(options, *place) != NULL)
			short_too = 1;		/* could be short option too */

		optchar = parse_long_options(nargv, options, long_options,
		    idx, short_too);
		if (optchar != -1) {
			place = EMSG;
			return (optchar);
		}
	}

	if ((optchar = (int)*place++) == (int)':' ||
	    (oli = strchr(options, optchar)) == NULL) {
		/*
		 * If the user didn't specify '-' as an option,
		 * assume it means -1 as POSIX specifies.
		 */
		if (optchar == (int)'-')
			return (-1);
		/* option letter unknown or ':' */
		if (!*place)
			++myoptind;
#if 0 /* yomei */
		if (PRINT_ERROR)
			warnx(illoptchar, optchar);
#endif
		myoptopt = optchar;
		return (BADCH);
	}
	if (long_options != NULL && optchar == 'W' && oli[1] == ';') {
		/* -W long-option */
		if (*place)			/* no space */
			/* NOTHING */;
		else if (++myoptind >= nargc) {	/* no arg */
			place = EMSG;
#if 0 /* yomei */
			if (PRINT_ERROR)
				warnx(recargchar, optchar);
#endif
			myoptopt = optchar;
			return (BADARG);
		} else				/* white space */
			place = nargv[myoptind];
		optchar = parse_long_options(nargv, options, long_options,
		    idx, 0);
		place = EMSG;
		return (optchar);
	}
	if (*++oli != ':') {			/* doesn't take argument */
		if (!*place)
			++myoptind;
	} else {				/* takes (optional) argument */
		myoptarg = NULL;
		if (*place)			/* no white space */
			myoptarg = place;
		/* XXX: disable test for :: if PC? (GNU doesn't) */
		else if (oli[1] != ':') {	/* arg not optional */
			if (++myoptind >= nargc) {	/* no arg */
				place = EMSG;
#if 0 /* yomri */
				if (PRINT_ERROR)
					warnx(recargchar, optchar);
#endif
				myoptopt = optchar;
				return (BADARG);
			} else
				myoptarg = nargv[myoptind];
		}
		place = EMSG;
		++myoptind;
	}
	/* dump back option letter */
	return (optchar);
}

/*
 * getopt --
 *	Parse argc/argv argument vector.
 *
 * [eventually this will replace the BSD getopt]
 */
int
mygetopt(int nargc, char * const *nargv, const char *options)
{

	/*
	 * We dont' pass FLAG_PERMUTE to getopt_internal() since
	 * the BSD getopt(3) (unlike GNU) has never done this.
	 *
	 * Furthermore, since many privileged programs call getopt()
	 * before dropping privileges it makes sense to keep things
	 * as simple (and bug-free) as possible.
	 */
	return (getopt_internal(nargc, nargv, options, NULL, NULL, 0));
}



/*
 * getopt_long --
 *	Parse argc/argv argument vector.
 */
int
mygetopt_long(int nargc,char * const * nargv,const char * options,const struct myoption* long_options,int* idx)
{

	return (getopt_internal(nargc, nargv, options, long_options, idx,
	    FLAG_PERMUTE));
}

/*
 * getopt_long_only --
 *	Parse argc/argv argument vector.
 */
int
mygetopt_long_only(int nargc,char * const * nargv,const char*options,const struct myoption* long_options,int* idx)
{

	return (getopt_internal(nargc, nargv, options, long_options, idx,
	    FLAG_PERMUTE|FLAG_LONGONLY));
}

