#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([naev],
        [0.8.0-beta.3],
        [https://github.com/naev/naev/issues],
        [naev],
        [https://naev.org/])
AC_CONFIG_SRCDIR([src/naev.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build])

AM_GNU_GETTEXT([external])
# Currently steam packages 0.19.7 which is the limiting factor here
AM_GNU_GETTEXT_VERSION([0.19.7])
GETTEXT_PACKAGE=${PACKAGE_NAME}
AC_SUBST(GETTEXT_PACKAGE)

IT_PROG_INTLTOOL

AM_INIT_AUTOMAKE([-Wall -Wno-portability foreign subdir-objects])

# M4 defines __unix__ to be the empty string on UNIX-like systems.  This
# causes AC_DEFINE to fail if it is used to define __unix__.  This seems to
# fix the problem.
ifdef([__unix__], define([__unix__], [[__unix__]]))

#
# Utility functions
#

# If A='$B' and B='hello', then "expand_variable A" will cause A='hello'
expand_variable()
{
  while true; do
    eval local value=\$$1
    eval "$1=\"$value\""
    echo "$value" | grep -qE '(^|[^\\])\$' || break
  done
}

# Temporarily changes the value of a variable, until it is restored later.
# $1: name of variable
# $2: temporary value
define([VAR_SAVE], [
  OLD_$1="$$1"
  $1=$2
])

# Restores the saved variable.
# $1: name of variable
define([VAR_RESTORE], [
  $1="$OLD_$1"
])

#
# Checks for variables
#

# Fix missing prefix
AS_IF([test "$prefix" = "NONE"], [
  prefix="$ac_default_prefix"
])
AS_IF([test "$exec_prefix" = "NONE"], [
  exec_prefix="$prefix"
])

#
# Checks for programs.
#
AC_PROG_CC
AC_PROG_OBJC
AC_PROG_LN_S
AC_PROG_RANLIB
AM_PROG_AR
AX_PROG_CC_FOR_BUILD
PKG_PROG_PKG_CONFIG

AC_CHECK_PROG([CUT], [cut], [cut])
AS_IF([test -z "$CUT"],[
  AC_ERROR([cut not found])
])

AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen])
AC_CHECK_PROG([LDOC], [ldoc], [ldoc])
AC_CHECK_PROG([GIT], [git], [git])
AC_CHECK_PROG([ITSTOOL], [itstool], [itstool])

#
# Checks for build environment
#
AC_MSG_CHECKING([whether we are building utils])
AS_IF([test -d "$srcdir/utils"], [
  have_utils=yes
], [
  have_utils=no
])
AC_MSG_RESULT([$have_utils])

AC_MSG_CHECKING([whether we have data])
AS_IF([test -d "$srcdir/dat"], [
  have_data=yes
], [
  have_data=no
])
AC_MSG_RESULT([$have_data])

AC_MSG_CHECKING([whether we have docs])
AS_IF([test -d "$srcdir/docs"], [
  have_docs=yes
], [
  have_docs=no
])
AC_MSG_RESULT([$have_docs])

#
# Platform checks
#
AC_CANONICAL_HOST

AC_MSG_CHECKING([[if we need a __unix__ define]])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([
  #ifndef __unix__
  # if defined __APPLE__ /* add other known platforms here */
  #   error should have __unix__ but don't
  # endif
  #endif
])], [AC_MSG_RESULT([no])],
[
  AC_MSG_RESULT([yes])
  AC_DEFINE([__unix__], 1,
[Define to 1 if running on a UNIX-like system where __unix__ is missing])
])

#
# Checks for command-line arguments
#

# --enable-debug
AC_MSG_CHECKING([whether to enable debug mode])
AC_ARG_ENABLE([debug],
  AC_HELP_STRING([--enable-debug=@<:@no|yes|paranoid@:>@],
     [enable debug mode (default: yes)]), [
    case "$enableval" in
      no)
        enable_debug=no
        ;;
      paranoid)
        enable_debug=paranoid
        ;;
      *)
        enable_debug=yes
        ;;
    esac
  ], [
    enable_debug=yes
  ])
AC_MSG_RESULT([$enable_debug])

AS_IF([test "x$enable_debug" != "xno"], [
  AS_IF([test "x$ac_cv_prog_cc_g" = "xyes"], [CFLAGS="$CFLAGS -W -Wall -Wextra -Wunused -Wshadow -Wpointer-arith -Wmissing-prototypes -Winline -Wcast-align -Wmissing-declarations -Wredundant-decls -Wno-long-long -Wcast-align -Wformat -Wformat-security"], [])
  AX_CHECK_COMPILE_FLAG([-Wformat-truncation=0 -Werror],[CFLAGS="$CFLAGS -Wformat-truncation=0"],[])
  case "$host" in
    *-linux-gnu)
      AC_CHECK_LIB([bfd], [bfd_init], [
      AC_CHECK_HEADERS([bfd.h], [
      have_bfd=yes
      ], [
      AC_WARN([debug mode is enabled but bfd.h is missing; disabling])
      have_bfd=no
      ])
      ], [
      AC_WARN([debug mode is enabled but libbfd is missing; disabling])
      have_bfd=no
      ])
      ;;
  esac
])

# --with-openal
AC_MSG_CHECKING([whether to use OpenAL])
AC_ARG_WITH([openal],
  AC_HELP_STRING([--with-openal],
     [use the OpenAL sound backend (default: yes)]), [
    AS_IF([test "x$withval" = "xyes"], [
      with_openal=yes
    ], [
      with_openal=no
    ])
  ], [
    with_openal=yes
  ])
AC_MSG_RESULT([$with_openal])

# --with-sdlmixer
AC_MSG_CHECKING([whether to use SDL_mixer])
AC_ARG_WITH([sdlmixer],
  AC_HELP_STRING([--with-sdlmixer],
     [use the SDL_mixer sound backend (default: yes)]), [
    AS_IF([test "x$withval" = "xyes"], [
      with_sdlmixer=yes
    ], [
      with_sdlmixer=no
    ])
  ], [
    with_sdlmixer=yes
  ])
AC_MSG_RESULT([$with_sdlmixer])

# --enable-lua
AC_MSG_CHECKING([which lua to use])
AC_ARG_ENABLE([lua],
  AC_HELP_STRING([--enable-lua=@<:@internal|shared|luajit@:>@],
     [lua to use (default: luajit)]), [
    case "$enableval" in
      luajit)
        used_lua=luajit
        ;;
      shared)
        used_lua=shared
        ;;
      *)
        used_lua=internal
        ;;
    esac
  ], [
    used_lua=luajit
  ])
AC_MSG_RESULT([$used_lua])

#
# Checks for libraries.
#

# "Newish" GCC
AC_CHECK_HEADERS([stdalign.h])

# SDL
AM_PATH_SDL2(2.0.0, [], [
  AC_MSG_ERROR([SDL 2 not found])
])

# libxml2
PKG_CHECK_MODULES([XML], [libxml-2.0])

# FreeType
PKG_CHECK_MODULES([FREETYPE], [freetype2])

# vorbis
PKG_CHECK_MODULES([VORBIS], [vorbis])

# vorbisfile
PKG_CHECK_MODULES([VORBISFILE], [vorbisfile])

# libpng
PKG_CHECK_MODULES([PNG], [libpng])

# lua
case "$used_lua" in
  luajit)
    PKG_CHECK_MODULES([LUA], [luajit], [
      have_lua=yes
      LUA_CFLAGS="$LUA_CFLAGS -DHAVE_LUAJIT"
    ], [
      AC_WARN([Lua is set to LuaJIT but can not find library; using internal Lua])
      have_lua=no
    ])
    ;;
  shared)
    have_lua=no
    m4_foreach([pkgname], [lua-5.1, lua5.1, lua51, lua], [
        AS_IF([test "x$have_lua" = "xno"], [
            PKG_CHECK_MODULES([LUA], [pkgname], [have_lua=yes], [have_lua=no])
        ])
    ])
    AS_IF([test "x$have_lua" = "xno"], [AC_WARN([Lua is set to shared but can not find library; using internal Lua])])
    ;;
  *)
    have_lua=no
    ;;
esac
AS_IF([test "x$have_lua" = "xno"], [
  LUA_CFLAGS="-I\"\$(top_srcdir)/lib/lua\""
  LUA_LIBS="\$(top_builddir)/lib/lua/liblua.a"
  have_lua=yes
  used_lua=internal
])

# sqrt is required for system csparse to work
AC_SEARCH_LIBS([sqrt], [m])

# csparse
AC_CHECK_LIB([csparse], [cs_spalloc], [have_csparse_lib=yes], [have_csparse_lib=no])
AC_CHECK_LIB([cxsparse], [cs_di_spalloc], [have_cxsparse_lib=yes], [have_cxsparse_lib=no])

AS_IF([test "x$have_csparse_lib" = "xyes" -a "x$have_cxsparse_lib" = "xyes"], [
    AC_CHECK_HEADERS([cs.h], [have_cs_h=yes], [have_cs_h=no])
    AC_CHECK_HEADERS([suitesparse/cs.h], [have_suitesparse_cs_h=yes], [have_suitesparse_cs_h=no])

    AS_IF([test $have_cs_h = yes -o $have_suitesparse_cs_h = yes], [
        used_csparse=shared
        CSPARSE_LIBS="-lcsparse -lcxsparse"
    ], [
        used_csparse=internal
    ])
], [
   used_csparse=internal
])

AS_IF([test "x$used_csparse" = "xinternal"], [
    CSPARSE_CFLAGS="-I\"\$(top_srcdir)/lib/csparse\""
    CSPARSE_LIBS="\$(top_builddir)/lib/csparse/libcsparse.a"
])

# OpenAL
AS_IF([test "x$with_openal" = "xyes"], [
  PKG_CHECK_MODULES([OPENAL], [openal], [have_openal=yes], [have_openal=no])
], [have_openal=no])

# SDL_mixer
AS_IF([test "x$with_sdlmixer" = "xyes"], [
  PKG_CHECK_MODULES([SDLMIXER], [SDL2_mixer], [have_sdlmixer=yes], [have_sdlmixer=no])
], [have_sdlmixer=no])

# SDL_image
VAR_SAVE([LIBS])
AC_SEARCH_LIBS([IMG_Load], [SDL2_image], [
  SDLIMAGE_CFLAGS=
  SDLIMAGE_LIBS="$LIBS"
  have_mkspr=yes
], [
  SDLIMAGE_CFLAGS=
  SDLIMAGE_LIBS=
  have_mkspr=no
], [])
VAR_RESTORE([LIBS])

# OpenGL
AM_PATH_OPENGL([], [
  AC_MSG_ERROR([OpenGL not found])
])

#
# Compiler flags
#
case "$host" in
  *-linux*)
    SDL_CFLAGS="$SDL_CFLAGS -D_GNU_SOURCE=1"
    ;;
esac

# global
GLOBAL_CFLAGS="-include \"\$(top_builddir)/src/config.h\" -include \"\$(top_builddir)/src/attributes.h\" -I\"\$(top_srcdir)/src\" -DPKGDATADIR=\\\"\$(pkgdatadir)\\\""
GLOBAL_LIBS=

AS_IF([test "x$enable_debug" != "xno"], [
  GLOBAL_CFLAGS="$GLOBAL_CFLAGS -DLUA_USE_APICHECK"
])

case "$host" in
  *-linux*)
    GLOBAL_LIBS="$NAEV_LIBS -lm" # feels so wrong
    ;;
  *-mingw*)
    GLOBAL_LIBS="$NAEV_LIBS -lwsock32"
    ;;
esac

AC_SUBST([GLOBAL_CFLAGS])
AC_SUBST([GLOBAL_LIBS])

# naev
WIDGET_LIBS="\$(top_builddir)/src/tk/widget/libwidget.a"

NAEV_CFLAGS="$GLOBAL_CFLAGS"
NAEV_LIBS="$GLOBAL_LIBS $WIDGET_LIBS"

# glad requires libdl on non-Windows platforms
case "$host" in
  *-mingw*)
    ;;
  *)
    NAEV_LIBS="$NAEV_LIBS -ldl"
    ;;
esac

AS_IF([test "x$enable_debug" != "xno"], [
  AC_DEFINE([DEBUGGING], 1, [Define to 1 to enable debug code])
  AC_DEFINE([DEBUG], 1, [Define to 1 to enable debug code])
  AS_IF([test "x$have_bfd" != "xno"], [
     case "$host" in
       *-linux-gnu)
         AC_DEFINE([HAS_BFD], 1, [Define to 1 to enable bfd backtrace code])
         NAEV_LIBS="$NAEV_LIBS -lbfd -liberty -rdynamic -ldl"
         ;;
     esac
  ])
])
AS_IF([test "x$enable_debug" = "xparanoid"], [
  AC_DEFINE([DEBUG_PARANOID], 1, [Define to 1 to enable paranoid debug code])
])

NAEV_CFLAGS="$NAEV_CFLAGS $CSPARSE_CFLAGS $SDL_CFLAGS $XML_CFLAGS \
    $FREETYPE_CFLAGS $LUA_CFLAGS $VORBIS_CFLAGS $VORBISFILE_CFLAGS \
    $PNG_CFLAGS $OPENGL_CFLAGS"

NAEV_LIBS="$NAEV_LIBS $CSPARSE_LIBS $SDL_LIBS $XML_LIBS \
    $FREETYPE_LIBS $LUA_LIBS $VORBIS_LIBS $VORBISFILE_LIBS \
    $PNG_LIBS $OPENGL_LIBS"

AS_IF([test "x$have_openal" = "xyes"], [
  NAEV_CFLAGS="$NAEV_CFLAGS $OPENAL_CFLAGS"
  NAEV_LIBS="$NAEV_LIBS $OPENAL_LIBS"
])

AS_IF([test "x$have_sdlmixer" = "xyes"], [
  NAEV_CFLAGS="$NAEV_CFLAGS $SDLMIXER_CFLAGS"
  NAEV_LIBS="$NAEV_LIBS $SDLMIXER_LIBS"
])

case "$host" in
  *-mingw*)
    NAEV_LIBS="$NAEV_LIBS -mwindows"
    ;;
  *-darwin*)
    NAEV_LIBS="$NAEV_LIBS -framework Foundation"
    ;;
esac

# This is a list of in-tree static libs that naev depends on.  It is used by
# automake to detect when the binary needs recompiling after a library has been
# changed.
NAEV_DEPENDENCIES="$WIDGET_LIBS"
AS_IF([test "x$used_lua" = "xinternal"], [
   NAEV_DEPENDENCIES="$NAEV_DEPENDENCIES $LUA_LIBS"
])
AS_IF([test "x$used_csparse" = "xinternal"], [
   NAEV_DEPENDENCIES="$NAEV_DEPENDENCIES $CSPARSE_LIBS"
])

AC_SUBST([NAEV_CFLAGS])
AC_SUBST([NAEV_LIBS])
AC_SUBST([NAEV_DEPENDENCIES])

# liblua
LIBLUA_CFLAGS="$GLOBAL_CFLAGS"

case "$host" in
  *-mingw*)
    LIBLUA_CFLAGS="$LIBLUA_CFLAGS -DLUA_ANSI"
    ;;
  *)
    LIBLUA_CFLAGS="$LIBLUA_CFLAGS -DLUA_USE_POSIX"
    ;;
esac

AC_SUBST([LIBLUA_CFLAGS])


MKSPR_CFLAGS="$GLOBAL_CFLAGS $SDL_CFLAGS $SDLIMAGE_CFLAGS $PNG_CFLAGS -DNOLOGPRINTFCONSOLE"
MKSPR_LIBS="$GLOBAL_LIBS $SDL_LIBS $SDLIMAGE_LIBS $PNG_LIBS"
AC_SUBST([MKSPR_CFLAGS])
AC_SUBST([MKSPR_LIBS])

#
# Checks for headers
#
AC_CHECK_HEADERS([fenv.h])

#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

#
# Checks for functions
#
AC_CHECK_FUNCS([setlocale])

VAR_SAVE([LIBS], ["-lm"])
AC_CHECK_FUNCS([feenableexcept])
VAR_RESTORE([LIBS])

AC_CHECK_FUNCS([clock_gettime])
# if it is not available directly, check it in librt (glibc does this).
AS_IF([test $ac_cv_func_clock_gettime = no], [
   AC_CHECK_LIB(rt, clock_gettime,
         [LIBS="-lrt $LIBS"])
], [])

VAR_SAVE([CFLAGS], ["$VORBISFILE_CFLAGS"])
VAR_SAVE([LIBS],   ["$VORBISFILE_LIBS"])
AC_CHECK_FUNCS([ov_read_filter])
VAR_RESTORE([CFLAGS])
VAR_RESTORE([LIBS])

#
# Misc checks
#
AC_MSG_CHECKING([for the current Git commit identifier])
AS_IF([test -n "$GIT" && test -d "$srcdir/.git"], [
  GIT_COMMIT=$(cd "$srcdir" && "$GIT" rev-parse --short HEAD)
  AC_MSG_RESULT([$GIT_COMMIT])
], [
  GIT_COMMIT=
  AC_MSG_RESULT([unknown])
])

#
# Defines in config.h
#

# Cut out the numeric fields from our SemVer <major>.<minor>.<rev>[-pre_release][+build] for OS versioning.
VMAJOR=$(echo "$VERSION" | cut -d. -f1)
VMINOR=$(echo "$VERSION" | cut -d. -f2)
VREV=$(echo "$VERSION" | cut -d. -f3 | sed "s/[[+-]].*//")

AC_DEFINE_UNQUOTED([VMAJOR], [$VMAJOR], [Define to the major version component])
AC_DEFINE_UNQUOTED([VMINOR], [$VMINOR], [Define to the minor version component])
AC_DEFINE_UNQUOTED([VREV], [$VREV], [Define to the revision version component])

AC_SUBST([VMAJOR])
AC_SUBST([VMINOR])
AC_SUBST([VREV])

AC_DEFINE_UNQUOTED([HOST], ["$host"], [Host triple])

# USE_OPENAL
AS_IF([test "x$have_openal" = "xyes"], [
  AC_DEFINE([USE_OPENAL], 1, [Define to 1 if using OpenAL])
])

# USE_SDLMIX
AS_IF([test "x$have_sdlmixer" = "xyes"], [
  AC_DEFINE([USE_SDLMIX], 1, [Define to 1 if using SDL_mixer])
])

# Operating system
case "$host" in
  *-linux*)
    AC_DEFINE([LINUX], 1, [Define to 1 if running on Linux])
    ;;
  *-freebsd*)
    AC_DEFINE([FREEBSD], 1, [Define to 1 if running on FreeBSD])
    ;;
  *-mingw*)
    AC_CHECK_TOOL(WINDRES, windres)
    AC_DEFINE([WIN32], 1, [Define to 1 if running on Windows])
    ;;
  *-darwin*)
    AC_DEFINE([MACOS], 1, [Define to 1 if running on macOS])
    have_macos=yes
    ;;
esac

# GIT_COMMIT
AS_IF([test -n "$GIT_COMMIT"], [
  AC_DEFINE_UNQUOTED([GIT_COMMIT], ["$GIT_COMMIT"],
     [Define to the current Git commit identifier])
  AC_SUBST([GIT_COMMIT])
])

#
# Automake conditionals
#

AM_CONDITIONAL([HAVE_WINDRES], [test -n "$WINDRES"])
AM_CONDITIONAL([HAVE_MACOS], [test "x$have_macos" = "xyes"])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_CONDITIONAL([HAVE_LDOC], [test -n "$LDOC"])
AM_CONDITIONAL([HAVE_UTILS], [test "x$have_utils" = "xyes"])
AM_CONDITIONAL([HAVE_DATA], [test "x$have_data" = "xyes"])
AM_CONDITIONAL([HAVE_DOCS], [test "x$have_docs" = "xyes"])
AM_CONDITIONAL([HAVE_MKSPR], [test "x$have_mkspr" = "xyes"])
AM_CONDITIONAL([LUA_INTERNAL], [test "x$used_lua" = "xinternal"])
AM_CONDITIONAL([CSPARSE_INTERNAL], [test "x$used_csparse" = "xinternal"])
AM_CONDITIONAL([HAVE_NLS], [test "x$USE_NLS" = "xyes" && test -d "$srcdir/po"])
AM_CONDITIONAL([HAVE_ITSTOOL], [test -n "$ITSTOOL"])
# AC_CHECK_FILE breaks cross-compiling, so we check directly
AM_CONDITIONAL([HAVE_SHADERS_C_GEN], [test -f "$srcdir/src/shaders_c_gen.c"])
#AC_CHECK_FILE([$srcdir/src/shaders_c_gen.c],
#   AM_CONDITIONAL([HAVE_SHADERS_C_GEN],true),
#   AM_CONDITIONAL([HAVE_SHADERS_C_GEN],false))

#
# Output
#
SHAVE_INIT([build], [enable])
AC_CONFIG_FILES([Makefile
      po/Makefile.in
      lib/Makefile
      src/Makefile
      src/tk/Makefile
      src/tk/widget/Makefile
      build/shave])
AS_IF([test "x$have_utils" = "xyes"], [
  AC_CONFIG_FILES([utils/Makefile
     utils/mkspr/Makefile])
])
AS_IF([test "x$have_docs" = "xyes"], [
  AC_CONFIG_FILES([docs/Makefile])
])
AS_IF([test "x$used_lua" = "xinternal"], [
  AC_CONFIG_FILES([lib/lua/Makefile])
])
AS_IF([test "x$used_csparse" = "xinternal"], [
  AC_CONFIG_FILES([lib/csparse/Makefile])
])

AC_OUTPUT

echo
AC_MSG_NOTICE([prefix:       $prefix])
echo
AC_MSG_NOTICE([sound backends:])
AC_MSG_NOTICE([  OpenAL:     $have_openal])
AC_MSG_NOTICE([  SDL_mixer:  $have_sdlmixer])
echo
AC_MSG_NOTICE([lib:])
AC_MSG_NOTICE([  csparse:    $used_csparse])
AC_MSG_NOTICE([  lua:        $used_lua])
echo
AC_MSG_NOTICE([utils:        $have_utils])
AC_MSG_NOTICE([  mkspr:      $have_mkspr])
echo
AC_MSG_NOTICE([debug mode:   $enable_debug])
AS_IF([test "x$enable_debug" != "xno"], [
   case "$host" in
     *-linux*)
       AC_MSG_NOTICE([  bfd:       $have_bfd])
       ;;
   esac
])
