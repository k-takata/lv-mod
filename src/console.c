/*
 * console.c
 *
 * All rights reserved. Copyright (C) 1996 by NARITA Tomio.
 * $Id: console.c,v 1.8 2004/01/05 07:27:46 nrt Exp $
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef MSDOS
#include <dos.h>
#endif /* MSDOS */

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif /* _WIN32 */

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif /* HAVE_TERMIOS_H */

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif /* HAVE_SYS_IOCTL_H */
#endif /* UNIX */

#ifdef TERMINFO
#include <termio.h>
#include <curses.h>
#include <term.h>
#endif /* TERMINFO */

#include <import.h>
#include <ascii.h>
#include <attr.h>
#include <begin.h>
#include <console.h>

#define ANSI_ATTR_LENGTH	8

#if defined( MSDOS ) || defined( _WIN32 )
private char tbuf[ 16 ];

private char *clear_screen		= "\x1b[2J";
private char *clr_eol			= "\x1b[K";
private char *delete_line		= "\x1b[M";
private char *insert_line		= "\x1b[L";
private char *enter_standout_mode	= "\x1b[7m";
private char *exit_standout_mode	= "\x1b[m";
private char *enter_underline_mode	= "\x1b[4m";
private char *exit_underline_mode	= "\x1b[m";
private char *enter_bold_mode		= "\x1b[1m";
private char *exit_attribute_mode	= "\x1b[m";
private char *cursor_visible		= NULL;
private char *cursor_invisible		= NULL;
private char *enter_ca_mode		= NULL;
private char *exit_ca_mode		= NULL;
private char *keypad_local		= NULL;
private char *keypad_xmit		= NULL;

private void tputs( char *cp, int affcnt, int (*outc)(byte) )
{
  while( *cp )
    outc( *cp++ );
}

private int (*putfunc)(byte) = ConsolePrint;
#endif /* MSDOS,_WIN32 */

#ifdef _WIN32
#ifndef FOREGROUND_WHITE
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#endif
#ifndef BACKGROUND_WHITE
#define BACKGROUND_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#endif
#ifndef FOREGROUND_BLACK
#define FOREGROUND_BLACK FOREGROUND_INTENSITY
#endif
#ifndef BACKGROUND_BLACK
#define BACKGROUND_BLACK BACKGROUND_INTENSITY
#endif
#ifndef FOREGROUND_MASK
#define FOREGROUND_MASK (FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#endif
#ifndef BACKGROUND_MASK
#define BACKGROUND_MASK (BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY)
#endif

private WORD console_attr = 0;
private HANDLE stdout_handle = NULL;
private HANDLE console_handle = NULL;
private DWORD initial_mode;
private DWORD new_mode;
#endif /* _WIN32 */

#ifdef UNIX

#ifdef HAVE_TERMIOS_H
private struct termios ttyOld, ttyNew;
#else
private struct sgttyb ttyOld, ttyNew;
#endif /* HAVE_TERMIOS_H */

#ifdef putchar
private int putfunc( int ch )
{
  return putchar( ch );
}
#else
private int (*putfunc)(int) = putchar;
#endif

#endif /* UNIX */

#ifdef TERMCAP
private char entry[ 1024 ];
private char func[ 1024 ];

extern char *tgetstr(), *tgoto();
extern int tgetent(), tgetflag(), tgetnum(), tputs();

private char *cursor_address		= NULL;
private char *clear_screen		= NULL;
private char *clr_eol			= NULL;
private char *insert_line		= NULL;
private char *delete_line		= NULL;
private char *enter_standout_mode	= NULL;
private char *exit_standout_mode	= NULL;
private char *enter_underline_mode	= NULL;
private char *exit_underline_mode	= NULL;
private char *enter_bold_mode		= NULL;
private char *exit_attribute_mode	= NULL;
private char *cursor_visible		= NULL;
private char *cursor_invisible		= NULL;
private char *enter_ca_mode		= NULL;
private char *exit_ca_mode		= NULL;
private char *keypad_local		= NULL;
private char *keypad_xmit		= NULL;
#endif /* TERMCAP */

#ifdef _WIN32
private WORD GetWin32Attribute( byte attr )
{
  WORD attr_new = FOREGROUND_WHITE;
  if( ATTR_STANDOUT & attr ){
    attr_new =
      ((attr_new & FOREGROUND_MASK) << 4) |
      ((attr_new & BACKGROUND_MASK) >> 4);
  } else if( ATTR_COLOR & attr ){
    if( ATTR_REVERSE & attr ){
      if( ATTR_COLOR_B & attr ){
	attr_new = (attr_new & BACKGROUND_MASK)
	  | FOREGROUND_INTENSITY;
	if((ATTR_COLOR & attr) & 1) attr_new |= BACKGROUND_RED;
	if((ATTR_COLOR & attr) & 2) attr_new |= BACKGROUND_GREEN;
	if((ATTR_COLOR & attr) & 4) attr_new |= BACKGROUND_BLUE;
      } else {
	attr_new = (attr_new & BACKGROUND_MASK)
	  | FOREGROUND_INTENSITY | FOREGROUND_WHITE;
	if((ATTR_COLOR & attr) & 1) attr_new |= BACKGROUND_RED;
	if((ATTR_COLOR & attr) & 2) attr_new |= BACKGROUND_GREEN;
	if((ATTR_COLOR & attr) & 4) attr_new |= BACKGROUND_BLUE;
      }
    } else {
      attr_new = (attr_new & BACKGROUND_MASK);
      if((ATTR_COLOR & attr) & 1) attr_new |= FOREGROUND_RED;
      if((ATTR_COLOR & attr) & 2) attr_new |= FOREGROUND_GREEN;
      if((ATTR_COLOR & attr) & 4) attr_new |= FOREGROUND_BLUE;
    }
  } else if( ATTR_REVERSE & attr ){
    attr_new =
      ((attr_new & FOREGROUND_MASK) << 4) |
      ((attr_new & BACKGROUND_MASK) >> 4);
  }
  if( ATTR_BLINK & attr ){
    attr_new |= FOREGROUND_INTENSITY;
  }
  if( ATTR_UNDERLINE & attr ){
    attr_new |= FOREGROUND_INTENSITY;
  }
  if( ATTR_HILIGHT & attr ){
    attr_new |= FOREGROUND_INTENSITY;
  }
  return attr_new;
}
#endif /* _WIN32 */

public void ConsoleInit()
{
  allow_interrupt	= FALSE;
  kb_interrupted	= FALSE;
  window_changed	= FALSE;
}

public void ConsoleResetAnsiSequence()
{
  ansi_standout		= "7";
  ansi_reverse		= "7";
  ansi_blink		= "5";
  ansi_underline	= "4";
  ansi_hilight		= "1";
}

#ifdef MSDOS
private void InterruptIgnoreHandler( int arg )
{
  signal( SIGINT, InterruptIgnoreHandler );
}
#endif /* MSDOS */

private RETSIGTYPE InterruptHandler( int arg )
{
  kb_interrupted = TRUE;

#ifndef HAVE_SIGACTION
  signal( SIGINT, InterruptHandler );
#endif /* HAVE_SIGACTION */
}

public void ConsoleEnableInterrupt()
{
#ifdef MSDOS
  allow_interrupt = TRUE;
  signal( SIGINT, InterruptHandler );
#endif /* MSDOS */

#ifdef UNIX
  signal( SIGTSTP, SIG_IGN );
#ifdef HAVE_TERMIOS_H
  ttyNew.c_lflag |= ISIG;
  tcsetattr( 0, TCSADRAIN, &ttyNew );
#else /* HAVE_TERMIOS_H */
  ttyNew.sg_flags &= ~RAW;
  ioctl( 0, TIOCSETN, &ttyNew );
#endif /* HAVE_TERMIOS_H */
#endif /* UNIX */
}

public void ConsoleDisableInterrupt()
{
#ifdef MSDOS
  allow_interrupt = FALSE;
  signal( SIGINT, InterruptIgnoreHandler );
#endif /* MSDOS */

#ifdef UNIX
#ifdef HAVE_TERMIOS_H
  ttyNew.c_lflag &= ~ISIG;
  tcsetattr( 0, TCSADRAIN, &ttyNew );
#else /* HAVE_TERMIOS_H */
  ttyNew.sg_flags |= RAW;
  ioctl( 0, TIOCSETN, &ttyNew );
#endif /* HAVE_TERMIOS_H */
  signal( SIGTSTP, SIG_DFL );
#endif /* UNIX */
}

public void ConsoleGetWindowSize()
{
#ifdef UNIX
  struct winsize winSize;

  ioctl( 0, TIOCGWINSZ, &winSize );
  WIDTH = winSize.ws_col;
  HEIGHT = winSize.ws_row;
  if( 0 >= WIDTH || 0 >= HEIGHT ){
#ifdef HAVE_TGETNUM
    WIDTH = tgetnum( "columns" );
    HEIGHT = tgetnum( "lines" );
#else
    WIDTH = tigetnum( "columns" );
    HEIGHT = tigetnum( "lines" );
#endif /* HAVE_TGETNUM */
    if( 0 >= WIDTH || 0 >= HEIGHT )
      WIDTH = 80, HEIGHT = 24;
  }
#endif /* UNIX */

#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SMALL_RECT rect;
  int width, height;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  height = csbi.srWindow.Bottom - csbi.srWindow.Top;
  if( width != WIDTH || height != HEIGHT ){
    window_changed = TRUE;
    WIDTH = width;
    HEIGHT = height;
  }
#endif /* _WIN32 */
}

#ifdef UNIX
private RETSIGTYPE WindowChangeHandler( int arg )
{
  window_changed = TRUE;

  ConsoleGetWindowSize();

#ifndef HAVE_SIGACTION
  signal( SIGWINCH, WindowChangeHandler );
#endif /* HAVE_SIGACTION */
}
#endif /* UNIX */

public void ConsoleTermInit()
{
  /*
   * 1. setup terminal capability
   * 2. retrieve window size
   * 3. initialize terminal status
   */

#ifdef MSDOS
  byte *ptr;

#define ANSI		0
#define FMRCARD		1

  int term = ANSI;

  if( NULL != (ptr = getenv("TERM")) ){
    if( !strcmp( ptr, "fmr4020" ) ){
      term = FMRCARD;
      WIDTH = 40;
      HEIGHT = 19;
    } else if( !strcmp( ptr, "fmr8025" ) ){
      term = FMRCARD;
      WIDTH = 80;
      HEIGHT = 24;
    }
  }

  switch( term ){
  case FMRCARD:
    delete_line      = "\x1bR";
    insert_line      = "\x1b" "E";
    cursor_visible   = "\x1b[v";
    cursor_invisible = "\x1b[1v";
    break;
  default:
    WIDTH  = 80;
    HEIGHT = 24;
  }

  cur_left		= "\x1bK";
  cur_right		= "\x1bM";
  cur_up		= "\x1bH";
  cur_down		= "\x1bP";
  cur_ppage		= "\x1bI";
  cur_npage		= "\x1bQ";
#endif /* MSDOS */

#ifdef _WIN32
  no_scroll		= FALSE;

  console_handle = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
  ConsoleGetWindowSize();
#endif /* _WIN32 */

#ifdef TERMCAP
  byte *term, *ptr;
#endif
#ifdef TERMINFO
  byte *term;
  int state;
#endif

#ifdef UNIX
  int fd = open("/dev/tty", O_RDONLY);
  dup2(fd, 0);
  close(fd);
#endif

#ifdef TERMCAP
  if( NULL == (term = getenv( "TERM" )) )
    fprintf( stderr, "lv: environment variable TERM is required\n" );
  if( 0 >= tgetent( entry, term ) )
    fprintf( stderr, "lv: %s not found in termcap\n", term );

  ConsoleGetWindowSize();

  ptr = func;

  cursor_address	= tgetstr( "cm", &ptr );
  clear_screen		= tgetstr( "cl", &ptr );
  clr_eol		= tgetstr( "ce", &ptr );
  insert_line		= tgetstr( "al", &ptr );
  delete_line		= tgetstr( "dl", &ptr );
  enter_standout_mode	= tgetstr( "so", &ptr );
  exit_standout_mode	= tgetstr( "se", &ptr );
  enter_underline_mode	= tgetstr( "us", &ptr );
  exit_underline_mode	= tgetstr( "ue", &ptr );
  enter_bold_mode	= tgetstr( "md", &ptr );
  exit_attribute_mode	= tgetstr( "me", &ptr );
  cursor_visible	= tgetstr( "ve", &ptr );
  cursor_invisible	= tgetstr( "vi", &ptr );
  enter_ca_mode		= tgetstr( "ti", &ptr );
  exit_ca_mode		= tgetstr( "te", &ptr );

  keypad_local		= tgetstr( "ke", &ptr );
  keypad_xmit		= tgetstr( "ks", &ptr );

  cur_left		= tgetstr( "kl", &ptr );
  cur_right		= tgetstr( "kr", &ptr );
  cur_up		= tgetstr( "ku", &ptr );
  cur_down		= tgetstr( "kd", &ptr );
  cur_ppage		= tgetstr( "kP", &ptr );
  cur_npage		= tgetstr( "kN", &ptr );

  if( NULL == cursor_address || NULL == clear_screen || NULL == clr_eol )
    fprintf( stderr, "lv: termcap cm, cl, ce are required\n" );

  if( NULL == insert_line || NULL == delete_line )
    no_scroll = TRUE;
  else
    no_scroll = FALSE;
#endif /* TERMCAP */

#ifdef TERMINFO

  if( NULL == (term = getenv( "TERM" )) )
    fprintf( stderr, "lv: environment variable TERM is required\n" );

  setupterm( term, 1, &state );
  if( 1 != state )
    fprintf( stderr, "lv: cannot initialize terminal\n" );

  ConsoleGetWindowSize();

  cur_left		= key_left;
  cur_right		= key_right;
  cur_up		= key_up;
  cur_down		= key_down;
  cur_ppage		= key_ppage;
  cur_npage		= key_npage;

  if( NULL == cursor_address || NULL == clear_screen || NULL == clr_eol )
    fprintf( stderr, "lv: terminfo cursor_address, clr_eol are required\n" );

  if( NULL == insert_line || NULL == delete_line )
    no_scroll = TRUE;
  else
    no_scroll = FALSE;
#endif /* TERMINFO */

  if( enter_ca_mode )
    tputs( enter_ca_mode, 1, putfunc );
  if( keypad_xmit )
    tputs( keypad_xmit, 1, putfunc );
}

public void ConsoleSetUp()
{
#ifdef MSDOS
  signal( SIGINT, InterruptIgnoreHandler );
#endif /* MSDOS */

#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SMALL_RECT rect;
  COORD coord;
  DWORD written;

  GetConsoleScreenBufferInfo( console_handle, &csbi );
  console_attr = csbi.wAttributes;
  rect = csbi.srWindow;
  coord = GetLargestConsoleWindowSize( console_handle );
  SetConsoleScreenBufferSize( console_handle, coord );
  GetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), &initial_mode );
  new_mode = initial_mode &
    ~( ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT );
  SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), new_mode );
  SetConsoleActiveScreenBuffer( console_handle );
#endif /* _WIN32 */

#ifdef HAVE_SIGACTION
  struct sigaction sa;

  sa.sa_handler = WindowChangeHandler;
  sigemptyset( &sa.sa_mask );
  sa.sa_flags = 0;           /* No SA_RESTART means interrupt syscalls.  */
  sigaction( SIGWINCH, &sa, NULL );

  sa.sa_handler = InterruptHandler;
  sigemptyset( &sa.sa_mask );
  sa.sa_flags = 0;           /* No SA_RESTART means interrupt syscalls.  */
  sigaction( SIGINT, &sa, NULL );
#else
# ifdef SIGWINCH
  signal( SIGWINCH, WindowChangeHandler );
# endif
  signal( SIGINT, InterruptHandler );
#endif /* HAVE_SIGACTION */

#ifdef UNIX
#ifdef HAVE_TERMIOS_H
  tcgetattr( 0, &ttyOld );
  ttyNew = ttyOld;
  ttyNew.c_iflag &= ~ISTRIP;
  ttyNew.c_iflag &= ~INLCR;
  ttyNew.c_iflag &= ~ICRNL;
  ttyNew.c_iflag &= ~IGNCR;
  ttyNew.c_lflag &= ~ISIG;
  ttyNew.c_lflag &= ~ICANON;
  ttyNew.c_lflag &= ~ECHO;
  ttyNew.c_lflag &= ~IEXTEN;
#ifdef VDISCRD /* IBM AIX */
#define VDISCARD VDISCRD
#endif
  ttyNew.c_cc[ VDISCARD ] = -1;
  ttyNew.c_cc[ VMIN ] = 1;
  ttyNew.c_cc[ VTIME ] = 0;
  tcsetattr( 0, TCSADRAIN, &ttyNew );
#else
  ioctl( 0, TIOCGETP, &ttyOld );
  ttyNew = ttyOld;
  ttyNew.sg_flags &= ~ECHO;
  ttyNew.sg_flags |= RAW;
  ttyNew.sg_flags |= CRMOD;
  ioctl( 0, TIOCSETN, &ttyNew );
#endif /* HAVE_TERMIOS_H */
#endif /* UNIX */
}

public void ConsoleSetDown()
{
#ifdef _WIN32
  CloseHandle( console_handle );
  SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), initial_mode );
#endif /* _WIN32 */

#ifdef UNIX
#ifdef HAVE_TERMIOS_H
  tcsetattr( 0, TCSADRAIN, &ttyOld );
#else
  ioctl( 0, TIOCSETN, &ttyOld );
#endif /* HAVE_TERMIOS_H */
#endif /* UNIX */

  if( keypad_local )
    tputs( keypad_local, 1, putfunc );
  if( exit_ca_mode )
    tputs( exit_ca_mode, 1, putfunc );
  else {
    ConsoleSetCur( 0, HEIGHT - 1 );
    ConsolePrint( CR );
    ConsolePrint( LF );
  }
}

public void ConsoleShellEscape()
{
#ifdef UNIX
#ifdef HAVE_TERMIOS_H
  tcsetattr( 0, TCSADRAIN, &ttyOld );
#else
  ioctl( 0, TIOCSETN, &ttyOld );
#endif /* HAVE_TERMIOS_H */
#endif /* UNIX */

#ifdef _WIN32
  SetConsoleActiveScreenBuffer( GetStdHandle( STD_OUTPUT_HANDLE ) );
  SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), initial_mode );
#else /* _WIN32 */
  if( keypad_local )
    tputs( keypad_local, 1, putfunc );
  if( exit_ca_mode )
    tputs( exit_ca_mode, 1, putfunc );
  else
    ConsoleSetCur( 0, HEIGHT - 1 );
#endif /* _WIN32 */

  ConsoleFlush();
}

public void ConsoleReturnToProgram()
{
#ifdef _WIN32
  SetConsoleActiveScreenBuffer( console_handle );
  SetConsoleMode( GetStdHandle( STD_INPUT_HANDLE ), new_mode );
#else /* _WIN32 */
  if( keypad_xmit )
    tputs( keypad_xmit, 1, putfunc );
  if( enter_ca_mode )
    tputs( enter_ca_mode, 1, putfunc );
#endif /* _WIN32 */

#ifdef UNIX
#ifdef HAVE_TERMIOS_H
  tcsetattr( 0, TCSADRAIN, &ttyNew );
#else
  ioctl( 0, TIOCSETN, &ttyNew );
#endif /* HAVE_TERMIOS_H */
#endif /* UNIX */
}

public void ConsoleSuspend()
{
#if !defined( MSDOS ) && !defined( _WIN32 ) /* if NOT defind */
  kill(0, SIGSTOP);	/*to pgrp*/
#endif
}

public int ConsoleGetChar()
{
#ifdef MSDOS
  return getch();
#endif /* MSDOS */

#ifdef _WIN32
  static byte u8buf[ 4 ];
  static int inindex = 0;
  static int outindex = 0;
  WCHAR buf[ 2 ];
  int ch, cnt;

  if( outindex < inindex )
    return u8buf[ outindex++ ];

  cnt = 0;
  buf[ cnt++ ] = ch = _getwch();

  ConsoleGetWindowSize();

  if( 0xe0 == ch || 0x00 == ch ){
    /* Extended keys or function keys */
    ch = _getwch();
    switch( ch ){
      case 0x49: /* PageUp */
	return STX; /* C-b */
      case 0x51: /* PageDown */
	return ACK; /* C-f */
      case 0x53: /* Del */
	return DEL;
      case 0x47: /* Home */
      case 0x4f: /* End */
      case 0x52: /* Ins */
	return 0; /* ignore */
      default:
	if( 0x3b <= ch && ch <= 0x44 )  /* F1..F10 */
	  return 0; /* ignore */
	return ch;
    }
  }
  if( IS_HIGH_SURROGATE( ch ) )
    buf[ cnt++ ] = _getwch();

#ifdef USE_UNICODE_IO
  inindex = WideCharToMultiByte( CP_UTF8, 0, buf, cnt, u8buf,
      sizeof( u8buf ), NULL, NULL );
#else /* USE_UNICODE_IO */
  inindex = WideCharToMultiByte( CP_ACP, 0, buf, cnt, u8buf,
      sizeof( u8buf ), NULL, NULL );
#endif /* USE_UNICODE_IO */
  if( 0 == inindex )
    return 0;
  outindex = 0;
  return u8buf[ outindex++ ];
#endif /* _WIN32 */

#ifdef UNIX
  byte buf;

  if( 0 > read( 0, &buf, 1 ) )
    return EOF;
  else
    return (int)buf;
#endif /* UNIX */
}

#ifdef MSDOS
union REGS regs;
#endif /* MSDOS */

public int ConsolePrint( byte c )
{
#ifdef MSDOS
  /*
   * fast console output, but remember that you cannot use lv
   * through remote terminals, for example AUX (RS232C).
   * I changed this code for FreeDOS. Because function code No.6
   * with Int 21h on FreeDOS (Alpha 5) doesn't seem to work correctly.
   */
  regs.h.al = c;
  return int86( 0x29, &regs, &regs );
/*
  return (int)bdos( 0x06, 0xff != c ? c : 0, 0 );
*/
#endif /* MSDOS */

#ifdef _WIN32
#ifdef USE_UNICODE_IO
  static byte buf[ 5 ];
  static int index = 0;
  static int len = 0;

  if( 0 == len ){
    index = 0;
    if( (c & 0x80) == 0x00 )
      len = 1;
    else if( (c & 0xe0) == 0xc0 )
      len = 2;
    else if( (c & 0xf0) == 0xe0 )
      len = 3;
    else if( (c & 0xf8) == 0xf0 )
      len = 4;
    else
      len = 1;
  }
  buf[ index++ ] = c;
  if( index == len ){
    buf[ index ] = '\0';
    ConsolePrints( buf );
    len = 0;
  }
  return c;
#else /* USE_UNICODE_IO */
  DWORD written;
  WriteFile( console_handle, &c, 1, &written, NULL );
#endif /* USE_UNICODE_IO */
#endif /* _WIN32 */

#ifdef UNIX
  return putchar( c );
#endif /* UNIX */
}

public void ConsolePrints( byte *str )
{
#if defined( _WIN32 ) && defined( USE_UNICODE_IO )
  int size, ret;
  LPWSTR buf;
  DWORD written;

  size = MultiByteToWideChar( CP_UTF8, 0, str, -1, NULL, 0 );
  if( 0 == size )
    return;
  buf = ( LPWSTR )Malloc( size * sizeof( WCHAR ) );
  if( NULL == buf )
    return;
  ret = MultiByteToWideChar( CP_UTF8, 0, str, -1, buf, size );
  if( ret > 1 )
    WriteConsoleW( console_handle, buf, ret - 1, &written, NULL );
  free( buf );
#else /* _WIN32,USE_UNICODE_IO */
  while( *str )
    ConsolePrint( *str++ );
#endif /* _WIN32,USE_UNICODE_IO */
}

public void ConsolePrintsStr( str_t *str, int length )
{
  int i;
  byte attr, lastAttr;

  attr = lastAttr = ATTR_NULL;
  for( i = 0 ; i < length ; i++ ){
    attr = ( 0xff00 & str[ i ] ) >> 8;
    if( lastAttr != attr )
      ConsoleSetAttribute( attr );
    lastAttr = attr;
    ConsolePrint( 0xff & str[ i ] );
  }
  if( 0 != attr )
    ConsoleSetAttribute( 0 );
}

public void ConsoleFlush()
{
#ifdef UNIX
  fflush( stdout );
#endif /* UNIX */
}

public void ConsoleSetCur( int x, int y )
{
#ifdef MSDOS
  sprintf( tbuf, "\x1b[%d;%dH", y + 1, x + 1 );
  ConsolePrints( tbuf );
#endif /* MSDOS */

#ifdef _WIN32
  COORD coord;
  if( x != -1 ) coord.X = x;
  if( y != -1 ) coord.Y = y;
  SetConsoleCursorPosition( console_handle, coord );
#endif /* _WIN32 */

#ifdef TERMCAP
  tputs( tgoto( cursor_address, x, y ), 1, putfunc );
#endif /* TERMCAP */

#ifdef TERMINFO
  tputs( tparm( cursor_address, y, x ), 1, putfunc );
#endif /* TERMINFO */
}

public void ConsoleOnCur()
{
#ifdef _WIN32
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo( console_handle, &cci );
  cci.bVisible = TRUE;
  SetConsoleCursorInfo( console_handle, &cci );
#else /* _WIN32 */
  if( cursor_visible )
    tputs( cursor_visible, 1, putfunc );
#endif /* _WIN32 */
}

public void ConsoleOffCur()
{
#ifdef _WIN32
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo( console_handle, &cci );
  cci.bVisible = FALSE;
  SetConsoleCursorInfo( console_handle, &cci );
#else /* _WIN32 */
  if( cursor_invisible )
    tputs( cursor_invisible, 1, putfunc );
#endif /* _WIN32 */
}

public void ConsoleClearScreen()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD coord = {0, 0};
  DWORD size, written;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  size = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter( console_handle, ' ', size, coord, &written );
  FillConsoleOutputAttribute( console_handle,
    csbi.wAttributes, size, coord, &written );
  SetConsoleCursorPosition( console_handle, coord );
#else /* _WIN32 */
  tputs( clear_screen, 1, putfunc );
#endif /* _WIN32 */
}

public void ConsoleClearRight()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD coord;
  DWORD size, written;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  coord = csbi.dwCursorPosition;
  size = csbi.dwSize.X - coord.X;
  FillConsoleOutputCharacter( console_handle, ' ', size, coord, &written );
  FillConsoleOutputAttribute( console_handle,
    csbi.wAttributes, size, coord, &written );
  SetConsoleCursorPosition( console_handle, csbi.dwCursorPosition );
#else /* _WIN32 */
  tputs( clr_eol, 1, putfunc );
#endif /* _WIN32 */
}

public void ConsoleGoAhead()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD coord;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  csbi.dwCursorPosition.X = 0;
  SetConsoleCursorPosition( console_handle, csbi.dwCursorPosition );
#else /* _WIN32 */
  ConsolePrint( 0x0d );
#endif /* _WIN32 */
}

public void ConsoleScrollUp()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SMALL_RECT rect;
  CHAR_INFO ci;
  COORD coord;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  rect = csbi.srWindow;
  rect.Top = 1;
  coord.X = 0;
  coord.Y = 0;
  ci.Char.AsciiChar = ' ';
  ci.Attributes = csbi.wAttributes;
  ScrollConsoleScreenBuffer( console_handle, &rect, &csbi.srWindow,
      coord, &ci );
  SetConsoleCursorPosition( console_handle, csbi.dwCursorPosition );
#else /* _WIN32 */
  if( delete_line )
    tputs( delete_line, 1, putfunc );
#endif /* _WIN32 */
}

public void ConsoleScrollDown()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SMALL_RECT rect;
  CHAR_INFO ci;
  COORD coord;
  GetConsoleScreenBufferInfo( console_handle, &csbi );
  rect = csbi.srWindow;
  coord.X = 0;
  coord.Y = 1;
  ci.Char.AsciiChar = ' ';
  ci.Attributes = csbi.wAttributes;
  ScrollConsoleScreenBuffer( console_handle, &rect, &csbi.srWindow,
      coord, &ci );
  SetConsoleCursorPosition( console_handle, csbi.dwCursorPosition );
#else /* _WIN32 */
  if( insert_line )
    tputs( insert_line, 1, putfunc );
#endif /* _WIN32 */
}

private byte prevAttr = 0;

public void ConsoleSetAttribute( byte attr )
{
#ifdef _WIN32
  SetConsoleTextAttribute( console_handle, GetWin32Attribute( attr ) );
  prevAttr = attr;
#else /* _WIN32 */
#ifndef MSDOS /* IF NOT DEFINED */
  if( TRUE == allow_ansi_esc ){
#endif /* MSDOS */
    ConsolePrints( "\x1b[0" );
    if( 0 != attr ){
      if( ATTR_STANDOUT & attr ){
	ConsolePrint( ';' );
	ConsolePrints( ansi_standout );
      } else if( ATTR_COLOR & attr ){
	if( ATTR_REVERSE & attr ){
	  if( ATTR_COLOR_B & attr ){
	    ConsolePrints( ";30;4" );
	    ConsolePrint( ( ATTR_COLOR & attr ) + '0' );
	  } else {
	    ConsolePrints( ";37;4" );
	    ConsolePrint( ( ATTR_COLOR & attr ) + '0' );
	  }
	} else {
	  ConsolePrints( ";3" );
	  ConsolePrint( ( ATTR_COLOR & attr ) + '0' );
	}
      } else if( ATTR_REVERSE & attr ){
	ConsolePrint( ';' );
	ConsolePrints( ansi_reverse );
      }
      if( ATTR_BLINK & attr ){
	ConsolePrint( ';' );
	ConsolePrints( ansi_blink );
      }
      if( ATTR_UNDERLINE & attr ){
	ConsolePrint( ';' );
	ConsolePrints( ansi_underline );
      }
      if( ATTR_HILIGHT & attr ){
	ConsolePrint( ';' );
	ConsolePrints( ansi_hilight );
      }
    }
    ConsolePrint( 'm' );
#ifndef MSDOS /* IF NOT DEFINED */
  } else {
    /*
     * non ansi sequence
     */
    if( ( ATTR_HILIGHT & prevAttr ) && 0 == ( ATTR_HILIGHT & attr ) )
      if( exit_attribute_mode )
	tputs( exit_attribute_mode, 1, putfunc );
    if( ( ATTR_UNDERLINE & prevAttr ) && 0 == ( ATTR_UNDERLINE & attr ) )
      if( exit_underline_mode )
	tputs( exit_underline_mode, 1, putfunc );
    if( ( ATTR_STANDOUT & prevAttr ) && 0 == ( ATTR_STANDOUT & attr ) )
      if( exit_standout_mode )
	tputs( exit_standout_mode, 1, putfunc );

    if( ATTR_HILIGHT & attr )
      if( enter_bold_mode )
	tputs( enter_bold_mode, 1, putfunc );
    if( ATTR_UNDERLINE & attr )
      if( enter_underline_mode )
	tputs( enter_underline_mode, 1, putfunc );
    if( ATTR_STANDOUT & attr )
      if( enter_standout_mode )
	tputs( enter_standout_mode, 1, putfunc );
  }
  prevAttr = attr;
#endif /* MSDOS */
#endif /* _WIN32 */
}
