/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
 */
#include "SDL_config.h"

/* This is the CD-audio control API for Simple DirectMedia Layer */

#include "SDL_cdrom.h"

#if !defined(__MACOS__)
#define CLIP_FRAMES	10	/* Some CD-ROMs won't go all the way */
#endif

static int SDL_cdinitted = 0;

int SDL_numcds;

int SDL_CDROMInit(void)
{

	return(0);
}

/* Check to see if the CD-ROM subsystem has been initialized */
static int CheckInit(int check_cdrom, SDL_CD **cdrom)
{

	return(1);
}

int SDL_CDNumDrives(void)
{

	return(1);
}

const char *SDL_CDName(int drive)
{

	return("");

}

SDL_CD *SDL_CDOpen(int drive)
{
	return 0;
}

CDstatus SDL_CDStatus(SDL_CD *cdrom)
{
	return 0;
}

int SDL_CDPlayTracks(SDL_CD *cdrom,
		int strack, int sframe, int ntracks, int nframes)
{
	return 0;
}

int SDL_CDPlay(SDL_CD *cdrom, int sframe, int length)
{
	return 0;
}

int SDL_CDPause(SDL_CD *cdrom)
{

	return(0);
}

int SDL_CDResume(SDL_CD *cdrom)
{
	return 0;
}

int SDL_CDStop(SDL_CD *cdrom)
{
	return 0;
}

int SDL_CDEject(SDL_CD *cdrom)
{

	return(0);
}

void SDL_CDClose(SDL_CD *cdrom)
{

}

void SDL_CDROMQuit(void)
{

}
