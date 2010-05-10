#/bin/sh
XNU_VERSION=1228
XNU_NAME="xnu"
XNU_PACKAGE_EXT=".tar.gz"
XNU_DIRECTORY="xnu"

LIBC_VERSION=583
LIBC_NAME="Libc"
LIBC_PACKAGE_EXT=".tar.gz"
LIBC_TAR_OPTIONS="-xzf"
LIBC_DIRECTORY="libc"

ARCH_VERSION=258
ARCH_NAME="architecture"
ARCH_PACKAGE_EXT=".tar.gz"



function get_package {
NAME=$1
VERSION=$2
PACKAGE_EXT=$3
PACKAGE_NAME="$NAME-$VERSION"
DIRECTORY=$NAME

FILE=$PACKAGE_NAME$PACKAGE_EXT
URL="http://www.opensource.apple.com/tarballs/$NAME/$PACKAGE_NAME$PACKAGE_EXT"
TAR_OPTIONS="-xzf"

#if [ ! -d $DIRECTORY ]; then
	rm -R $DIRECTORY

	echo $FILE
	if [ ! -f $FILE ]; then
		wget $URL -O $FILE
	fi

	if [ ! -f $FILE ]; then
		echo "$FILE not found"
		exit -1
	fi

	tar $TAR_OPTIONS $FILE

	if [ ! -d $PACKAGE_NAME ]; then
		echo "directory $PACKAGE_NAME not found"
		exit -1
	fi

	cp -R $PACKAGE_NAME $DIRECTORY
	#mv $PACKAGE_NAME $DIRECTORY
	find $DIRECTORY -type l | xargs rm -f
	find $DIRECTORY -type f | grep -v .*[.]h | xargs rm -f
	if [ -d $PACKAGE_NAME ]; then
		find $PACKAGE_NAME -type l | xargs rm -f
		find $PACKAGE_NAME -type f | grep -v .*[.]h | xargs rm -f
	fi
	if [ -f $PACKAGE_NAME.patch ]; then
		patch -p0 < $PACKAGE_NAME.patch
	fi

#fi
}

get_package $XNU_NAME $XNU_VERSION $XNU_PACKAGE_EXT
get_package $LIBC_NAME $LIBC_VERSION $LIBC_PACKAGE_EXT
get_package $ARCH_NAME $ARCH_VERSION $ARCH_PACKAGE_EXT

rm xnu/osfmk/sys/types.h
rm xnu/EXTERNAL_HEADERS/stdint.h
rm xnu/osfmk/mach/message.h
touch xnu/osfmk/mach/message.h
rm architecture/byte_order.h
touch architecture/byte_order.h
rm xnu/osfmk/mach/i386/_types.h
touch xnu/osfmk/mach/i386/_types.h
