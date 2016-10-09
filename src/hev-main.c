/*
 ============================================================================
 Name        : hev-main.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2014 everyone.
 Description : Main
 ============================================================================
 */

#include <stdio.h>
#include <signal.h>

#include "hev-main.h"
#include "hev-socks5-tproxy.h"

static void
show_help (const char *app)
{
	fprintf (stderr, "%s LOCAL_ADDR LOCAL_PORT SOCKS5_ADDR SOCKS5_PORT\n", app);
}

static bool
signal_handler (void *data)
{
	HevEventLoop *loop = data;
	hev_event_loop_quit (loop);
	return false;
}

int
main (int argc, char *argv[])
{
	HevEventLoop *loop = NULL;
	HevEventSource *source = NULL;
	HevSocks5TProxy *tproxy = NULL;

	if (5 > argc) {
		show_help (argv[0]);
		exit (1);
	}

	loop = hev_event_loop_new ();

	signal (SIGPIPE, SIG_IGN);

	source = hev_event_source_signal_new (SIGINT);
	hev_event_source_set_priority (source, 3);
	hev_event_source_set_callback (source, signal_handler, loop, NULL);
	hev_event_loop_add_source (loop, source);
	hev_event_source_unref (source);

	tproxy = hev_socks5_tproxy_new (loop, argv[1], atoi (argv[2]), argv[3], atoi (argv[4]));
	if (tproxy) {
		hev_event_loop_run (loop);
		hev_socks5_tproxy_unref (tproxy);
	}

	hev_event_loop_unref (loop);

	return 0;
}
