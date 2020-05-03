/*
 * Copyright (c), Edward Thomson <ethomson@edwardthomson.com>
 * All rights reserved.
 *
 * This file is part of adopt, distributed under the MIT license.
 * For full terms and conditions, see the included LICENSE file.
 *
 * THIS FILE IS AUTOMATICALLY GENERATED; DO NOT EDIT.
 *
 * This file was produced by using the `rename.pl` script included with
 * adopt.  The command-line specified was:
 *
 * ./rename.pl cli_opt --filename=opt --include=cli.h --inline=GIT_INLINE --header-guard=CLI_opt_h__ --without-usage --lowercase-status
 */

#ifndef CLI_opt_h__
#define CLI_opt_h__

#include <stdio.h>
#include <stdint.h>

/**
 * The type of argument to be parsed.
 */
typedef enum {
	CLI_OPT_NONE = 0,

	/**
	 * An argument that, when specified, sets a given value to true.
	 * This is useful for arguments like "--debug".  The `value` pointer
	 * in the returned option will be set to `1` when this is set.
	 */
	CLI_OPT_BOOL,

	/**
	 * An argument that, when specified, sets the given `value_ptr`
	 * to the given `value`.
	 */
	CLI_OPT_SWITCH,

	/** An argument that has a value ("-nvalue" or "--name value") */
	CLI_OPT_VALUE,

	/** An argument that has an optional value ("-n" or "-n foo") */
	CLI_OPT_VALUE_OPTIONAL,

	/** The literal arguments follow specifier, bare "--" */
	CLI_OPT_LITERAL,

	/** A single "free" argument ("path") */
	CLI_OPT_ARG,

	/** Unmatched arguments, a collection of "free" arguments ("paths...") */
	CLI_OPT_ARGS,
} cli_opt_type_t;

/**
 * Usage information for an argument, to be displayed to the end-user.
 * This is only for display, the parser ignores this usage information.
 */
typedef enum {
	/** This argument is required. */
	CLI_OPT_USAGE_REQUIRED = (1u << 0),

	/** This argument should not be displayed in usage. */
	CLI_OPT_USAGE_HIDDEN = (1u << 1),

	/** This is a multiple choice argument, combined with the previous arg. */
	CLI_OPT_USAGE_CHOICE = (1u << 2),

	/** In usage, show the long format instead of the abbreviated format. */
	CLI_OPT_USAGE_SHOW_LONG = (1u << 3),
} cli_opt_usage_t;

/** Specification for an available option. */
typedef struct cli_opt_spec {
	/** Type of option expected. */
	cli_opt_type_t type;

	/** Name of the long option. */
	const char *name;

	/** The alias is the short (one-character) option alias. */
	const char alias;

	/**
	 * If this spec is of type `CLI_OPT_BOOL`, this is a pointer to
	 * an `int` that will be set to `1` if the option is specified.
	 *
	 * If this spec is of type `CLI_OPT_SWITCH`, this is a pointer to
	 * an `int` that will be set to the opt's `value` (below) when
	 * this option is specified.
	 *
	 * If this spec is of type `CLI_OPT_VALUE` or `CLI_OPT_VALUE_OPTIONAL`,
	 * this is a pointer to a `char *`, that will be set to the value
	 * specified on the command line.
	 */
	void *value;

	/**
	 * If this spec is of type `CLI_OPT_SWITCH`, this is the value to
	 * set in the option's `value_ptr` pointer when it is specified.
	 * This is ignored for other opt types.
	 */
	int switch_value;

	/**
	 * The name of the value, provided when creating usage information.
	 * This is required only for the functions that display usage
	 * information and only when a spec is of type `CLI_OPT_VALUE`.
	 */
	const char *value_name;

	/**
	 * Short description of the option, used when creating usage
	 * information.  This is only used when creating usage information.
	 */
	const char *help;

	/**
	 * Optional `cli_opt_usage_t`, used when creating usage information.
	 */
	cli_opt_usage_t usage;
} cli_opt_spec;

/** Return value for `cli_opt_parser_next`. */
typedef enum {
	/** Parsing is complete; there are no more arguments. */
	CLI_OPT_STATUS_DONE = 0,

	/**
	 * This argument was parsed correctly; the `opt` structure is
	 * populated and the value pointer has been set.
	 */
	CLI_OPT_STATUS_OK = 1,

	/**
	 * The argument could not be parsed correctly, it does not match
	 * any of the specifications provided.
	 */
	CLI_OPT_STATUS_UNKNOWN_OPTION = 2,

	/**
	 * The argument matched a spec of type `CLI_OPT_VALUE`, but no value
	 * was provided.
	 */
	CLI_OPT_STATUS_MISSING_VALUE = 3,
} cli_opt_status_t;

/** An option provided on the command-line. */
typedef struct cli_opt {
	/** The status of parsing the most recent argument. */
	cli_opt_status_t status;

	/**
	 * The specification that was provided on the command-line, or
	 * `NULL` if the argument did not match an `cli_opt_spec`.
	 */
	const cli_opt_spec *spec;

	/**
	 * The argument as it was specified on the command-line, including
	 * dashes, eg, `-f` or `--foo`.
	 */
	char *arg;

	/**
	 * If the spec is of type `CLI_OPT_VALUE` or `CLI_OPT_VALUE_OPTIONAL`,
	 * this is the value provided to the argument.
	 */
	char *value;
} cli_opt;

/* The internal parser state.  Callers should not modify this structure. */
typedef struct cli_opt_parser {
	const cli_opt_spec *specs;
	char **args;
	size_t args_len;

	size_t idx;
	size_t arg_idx;
	int in_literal : 1,
	in_short : 1;
} cli_opt_parser;

/**
 * Initializes a parser that parses the given arguments according to the
 * given specifications.
 *
 * @param parser The `cli_opt_parser` that will be initialized
 * @param specs A NULL-terminated array of `cli_opt_spec`s that can be parsed
 * @param args The arguments that will be parsed
 * @param args_len The length of arguments to be parsed
 */
void cli_opt_parser_init(
	cli_opt_parser *parser,
	const cli_opt_spec specs[],
	char **args,
	size_t args_len);

/**
 * Parses the next command-line argument and places the information about
 * the argument into the given `opt` data.
 *
 * @param opt The `cli_opt` information parsed from the argument
 * @param parser An `cli_opt_parser` that has been initialized with
 *        `cli_opt_parser_init`
 * @return true if the caller should continue iterating, or 0 if there are
 *         no arguments left to process.
 */
cli_opt_status_t cli_opt_parser_next(
	cli_opt *opt,
	cli_opt_parser *parser);

/**
 * Parses all the command-line arguments and updates all the options using
 * the pointers provided.  Parsing stops on any invalid argument and
 * information about the failure will be provided in the opt argument.
 *
 * @param opt The The `cli_opt` information that failed parsing
 * @param specs A NULL-terminated array of `cli_opt_spec`s that can be parsed
 * @param args The arguments that will be parsed
 * @param args_len The length of arguments to be parsed
 */
cli_opt_status_t cli_opt_parse(
    cli_opt *opt,
    const cli_opt_spec specs[],
    char **args,
    size_t args_len);

/**
 * Prints the status after parsing the most recent argument.  This is
 * useful for printing an error message when an unknown argument was
 * specified, or when an argument was specified without a value.
 *
 * @param file The file to print information to
 * @param opt The option that failed to parse
 * @return 0 on success, -1 on failure
 */
int cli_opt_status_fprint(
	FILE *file,
	const cli_opt *opt);

#endif /* CLI_opt_h__ */
