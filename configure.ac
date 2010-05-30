#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Mine], [1.0], [mine@gnu-log.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
	 
# Checks for header files.
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UINT32_T

# Checks for library functions.

case $host in
  x86_64*)
      CC="$CC -m32"
      CXX="$CXX -m32"
      AC_MSG_CHECKING([whether $CC works])
      AC_LINK_IFELSE(AC_LANG_PROGRAM(),AC_MSG_RESULT([yes]),
                     [AC_MSG_RESULT([no])
                      AC_MSG_ERROR([Cannot build a 32-bit program, you need to install 32-bit development libraries.])])
      LD="${LD:-ld} -m elf_i386"
      AS="${AS:-as} --32"
      host_cpu="i386"
      notice_platform="32-bit "
      AC_SUBST(TARGETFLAGS,"-m32")
      ;;
esac
CFLAGS="-Werror -std=gnu99"
CXXFLAGS="-Werror -std=gnu++0x"

AC_MSG_CHECKING(whether to enable debugging)
debug_default="yes"

AC_ARG_ENABLE(debug, [  --enable-debug=[no/yes] turn on debugging
                       [default=$debug_default]],, enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
         CXXFLAGS="$CXXFLAGS -g -DMINE_DEBUG"
         CFLAGS="$CFLAGS -g -DMINE_DEBUG"
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi


AC_CONFIG_FILES([Makefile src/Makefile src/loader/Makefile src/machoread/Makefile src/libs/Makefile src/libs/mine/Makefile src/libs/System/Makefile src/libs/gcc_s/Makefile])
AC_OUTPUT
