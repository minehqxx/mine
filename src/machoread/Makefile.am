bin_PROGRAMS = machoread
machoread_SOURCES = main.cc reader_macho.cc
machoread_LDADD = @top_builddir@/src/libs/mine/libmine.la
machoread_CPPFLAGS = -I@top_srcdir@/include
machoread_CPPFLAGS += -I@top_srcdir@/include/apsl/xnu/EXTERNAL_HEADERS 
machoread_CPPFLAGS += -I@top_srcdir@/include/apsl/xnu/osfmk 
machoread_CPPFLAGS += -I@top_srcdir@/include/apsl/xnu/osfmk/mach 
machoread_CPPFLAGS += -I@top_srcdir@/include/apsl/xnu/osfmk/mach-o
machoread_CPPFLAGS += -I@top_srcdir@/include/apsl

