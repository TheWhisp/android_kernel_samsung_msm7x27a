#!/bin/sh
#
# A depmod wrapper used by the toplevel Makefile

<<<<<<< HEAD
if test $# -ne 2; then
	echo "Usage: $0 /sbin/depmod <kernelrelease>" >&2
=======
if test $# -ne 3; then
	echo "Usage: $0 /sbin/depmod <kernelrelease> <symbolprefix>" >&2
>>>>>>> refs/remotes/origin/master
	exit 1
fi
DEPMOD=$1
KERNELRELEASE=$2
<<<<<<< HEAD

<<<<<<< HEAD
if ! "$DEPMOD" -V 2>/dev/null | grep -q module-init-tools; then
	echo "Warning: you may need to install module-init-tools" >&2
	echo "See http://www.codemonkey.org.uk/docs/post-halloween-2.6.txt" >&2
	sleep 1
fi

=======
>>>>>>> refs/remotes/origin/cm-10.0
if ! test -r System.map -a -x "$DEPMOD"; then
	exit 0
fi
=======
SYMBOL_PREFIX=$3

if ! test -r System.map -a -x "$DEPMOD"; then
	exit 0
fi

# older versions of depmod don't support -P <symbol-prefix>
# support was added in module-init-tools 3.13
if test -n "$SYMBOL_PREFIX"; then
	release=$("$DEPMOD" --version)
	package=$(echo "$release" | cut -d' ' -f 1)
	if test "$package" = "module-init-tools"; then
		version=$(echo "$release" | cut -d' ' -f 2)
		later=$(printf '%s\n' "$version" "3.13" | sort -V | tail -n 1)
		if test "$later" != "$version"; then
			# module-init-tools < 3.13, drop the symbol prefix
			SYMBOL_PREFIX=""
		fi
	fi
	if test -n "$SYMBOL_PREFIX"; then
		SYMBOL_PREFIX="-P $SYMBOL_PREFIX"
	fi
fi

>>>>>>> refs/remotes/origin/master
# older versions of depmod require the version string to start with three
# numbers, so we cheat with a symlink here
depmod_hack_needed=true
tmp_dir=$(mktemp -d ${TMPDIR:-/tmp}/depmod.XXXXXX)
mkdir -p "$tmp_dir/lib/modules/$KERNELRELEASE"
if "$DEPMOD" -b "$tmp_dir" $KERNELRELEASE 2>/dev/null; then
	if test -e "$tmp_dir/lib/modules/$KERNELRELEASE/modules.dep" -o \
		-e "$tmp_dir/lib/modules/$KERNELRELEASE/modules.dep.bin"; then
		depmod_hack_needed=false
	fi
fi
rm -rf "$tmp_dir"
if $depmod_hack_needed; then
	symlink="$INSTALL_MOD_PATH/lib/modules/99.98.$KERNELRELEASE"
	ln -s "$KERNELRELEASE" "$symlink"
	KERNELRELEASE=99.98.$KERNELRELEASE
fi

set -- -ae -F System.map
if test -n "$INSTALL_MOD_PATH"; then
	set -- "$@" -b "$INSTALL_MOD_PATH"
fi
<<<<<<< HEAD
"$DEPMOD" "$@" "$KERNELRELEASE"
=======
"$DEPMOD" "$@" "$KERNELRELEASE" $SYMBOL_PREFIX
>>>>>>> refs/remotes/origin/master
ret=$?

if $depmod_hack_needed; then
	rm -f "$symlink"
fi

exit $ret
