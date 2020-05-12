/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#include <windows.h>

#include "cli.h"

static void (*interrupt_handler)(void) = NULL;

static BOOL interrupt_proxy(DWORD signal)
{
	GIT_UNUSED(signal);
	interrupt_handler();
	return TRUE;
}

int cli_sighandler_set_interrupt(void (*handler)(void))
{
	if ((interrupt_handler = handler) != NULL)
		SetConsoleCtrlHandler(interrupt_proxy, FALSE);
	else
		SetConsoleCtrlHandler(NULL, FALSE);
}
