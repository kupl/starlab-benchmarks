/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/

#ifndef __APPLE__
#error This file is for Mac OS X only. You probably should not compile it.
#endif

// Please note that this file is just some Mac-specific bits. Most of the
// Mac OS X code is shared with other Unix platforms in sys_unix.c ...

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

/*
==============
Sys_Dialog

Display an OS X dialog box
==============
*/
dialogResult_t Sys_Dialog( dialogType_t type, const char *message, const char *title )
{
	dialogResult_t result = DR_OK;
	NSAlert *alert = [NSAlert new];

	[alert setMessageText: [NSString stringWithUTF8String: title]];
	[alert setInformativeText: [NSString stringWithUTF8String: message]];

	if( type == DT_ERROR )
		[alert setAlertStyle: NSCriticalAlertStyle];
	else
		[alert setAlertStyle: NSWarningAlertStyle];

	switch( type )
	{
		default:
			[alert runModal];
			result = DR_OK;
			break;

		case DT_YES_NO:
			[alert addButtonWithTitle: @"Yes"];
			[alert addButtonWithTitle: @"No"];
			switch( [alert runModal] )
			{
				default:
				case NSAlertFirstButtonReturn: result = DR_YES; break;
				case NSAlertSecondButtonReturn: result = DR_NO; break;
			}
			break;

		case DT_OK_CANCEL:
			[alert addButtonWithTitle: @"OK"];
			[alert addButtonWithTitle: @"Cancel"];

			switch( [alert runModal] )
			{
				default:
				case NSAlertFirstButtonReturn: result = DR_OK; break;
				case NSAlertSecondButtonReturn: result = DR_CANCEL; break;
			}
			break;
	}

	[alert release];

	return result;
}

/*
=================
Sys_StripAppBundle

Discovers if passed dir is suffixed with the directory structure of a Mac OS X
.app bundle. If it is, the .app directory structure is stripped off the end and
the result is returned. If not, dir is returned untouched.
=================
*/
char *Sys_StripAppBundle( char *dir )
{
	static char cwd[MAX_OSPATH];

	Q_strncpyz(cwd, dir, sizeof(cwd));
	if(strcmp(Sys_Basename(cwd), "MacOS"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	if(strcmp(Sys_Basename(cwd), "Contents"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	if(!strstr(Sys_Basename(cwd), ".app"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	return cwd;
}
