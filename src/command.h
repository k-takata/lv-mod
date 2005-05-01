/*
 * command.h
 *
 * All rights reserved. Copyright (C) 1996 by NARITA Tomio
 * $Id: command.h,v 1.3 2003/11/13 03:08:19 nrt Exp $
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <file.h>

public byte *editor_program;

public boolean_t initcmd_mode;
public char *initcmd_str;
public int initcmd_curp;

#define IsNumber( c )		( (c) >= '0' && (c) <= '9' )

public boolean_t CommandInit();
public void Command( file_t *file, byte **optional );

#endif /* __COMMAND_H__ */
