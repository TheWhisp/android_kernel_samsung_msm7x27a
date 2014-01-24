# Makefile for AppArmor Linux Security Module
#
obj-$(CONFIG_SECURITY_APPARMOR) += apparmor.o

apparmor-y := apparmorfs.o audit.o capability.o context.o ipc.o lib.o match.o \
              path.o domain.o policy.o policy_unpack.o procattr.o lsm.o \
              resource.o sid.o file.o
<<<<<<< HEAD
=======
apparmor-$(CONFIG_SECURITY_APPARMOR_HASH) += crypto.o
>>>>>>> refs/remotes/origin/master

clean-files := capability_names.h rlim_names.h


# Build a lower case string table of capability names
# Transforms lines from
#    #define CAP_DAC_OVERRIDE     1
# to
#    [1] = "dac_override",
quiet_cmd_make-caps = GEN     $@
<<<<<<< HEAD
<<<<<<< HEAD
cmd_make-caps = echo "static const char *capability_names[] = {" > $@ ;\
=======
cmd_make-caps = echo "static const char *const capability_names[] = {" > $@ ;\
>>>>>>> refs/remotes/origin/cm-10.0
	sed $< >>$@ -r -n -e '/CAP_FS_MASK/d' \
	-e 's/^\#define[ \t]+CAP_([A-Z0-9_]+)[ \t]+([0-9]+)/[\2] = "\L\1",/p';\
	echo "};" >> $@
=======
cmd_make-caps = echo "static const char *const capability_names[] = {" > $@ ;\
	sed $< >>$@ -r -n -e '/CAP_FS_MASK/d' \
	-e 's/^\#define[ \t]+CAP_([A-Z0-9_]+)[ \t]+([0-9]+)/[\2] = "\L\1",/p';\
	echo "};" >> $@ ;\
	echo -n '\#define AA_FS_CAPS_MASK "' >> $@ ;\
	sed $< -r -n -e '/CAP_FS_MASK/d' \
	    -e 's/^\#define[ \t]+CAP_([A-Z0-9_]+)[ \t]+([0-9]+)/\L\1/p' | \
	     tr '\n' ' ' | sed -e 's/ $$/"\n/' >> $@
>>>>>>> refs/remotes/origin/master


# Build a lower case string table of rlimit names.
# Transforms lines from
#    #define RLIMIT_STACK		3	/* max stack size */
# to
#    [RLIMIT_STACK] = "stack",
#
# and build a second integer table (with the second sed cmd), that maps
<<<<<<< HEAD
<<<<<<< HEAD
# RLIMIT defines to the order defined in asm-generic/resource.h  Thi is
=======
# RLIMIT defines to the order defined in asm-generic/resource.h  This is
>>>>>>> refs/remotes/origin/cm-10.0
=======
# RLIMIT defines to the order defined in asm-generic/resource.h  This is
>>>>>>> refs/remotes/origin/master
# required by policy load to map policy ordering of RLIMITs to internal
# ordering for architectures that redefine an RLIMIT.
# Transforms lines from
#    #define RLIMIT_STACK		3	/* max stack size */
# to
# RLIMIT_STACK, 
<<<<<<< HEAD
<<<<<<< HEAD
quiet_cmd_make-rlim = GEN     $@
cmd_make-rlim = echo "static const char *rlim_names[] = {" > $@ ;\
	sed $< >> $@ -r -n \
	    -e 's/^\# ?define[ \t]+(RLIMIT_([A-Z0-9_]+)).*/[\1] = "\L\2",/p';\
	echo "};" >> $@ ;\
	echo "static const int rlim_map[] = {" >> $@ ;\
	sed -r -n "s/^\# ?define[ \t]+(RLIMIT_[A-Z0-9_]+).*/\1,/p" $< >> $@ ;\
	echo "};" >> $@

$(obj)/capability.o : $(obj)/capability_names.h
$(obj)/resource.o : $(obj)/rlim_names.h
$(obj)/capability_names.h : $(srctree)/include/linux/capability.h
	$(call cmd,make-caps)
$(obj)/rlim_names.h : $(srctree)/include/asm-generic/resource.h
=======
=======
>>>>>>> refs/remotes/origin/master
#
# and build the securityfs entries for the mapping.
# Transforms lines from
#    #define RLIMIT_FSIZE        1   /* Maximum filesize */
#    #define RLIMIT_STACK		3	/* max stack size */
# to
# #define AA_FS_RLIMIT_MASK "fsize stack"
quiet_cmd_make-rlim = GEN     $@
cmd_make-rlim = echo "static const char *const rlim_names[RLIM_NLIMITS] = {" \
	> $@ ;\
	sed $< >> $@ -r -n \
	    -e 's/^\# ?define[ \t]+(RLIMIT_([A-Z0-9_]+)).*/[\1] = "\L\2",/p';\
	echo "};" >> $@ ;\
	echo "static const int rlim_map[RLIM_NLIMITS] = {" >> $@ ;\
	sed -r -n "s/^\# ?define[ \t]+(RLIMIT_[A-Z0-9_]+).*/\1,/p" $< >> $@ ;\
	echo "};" >> $@ ; \
	echo -n '\#define AA_FS_RLIMIT_MASK "' >> $@ ;\
	sed -r -n 's/^\# ?define[ \t]+RLIMIT_([A-Z0-9_]+).*/\L\1/p' $< | \
	    tr '\n' ' ' | sed -e 's/ $$/"\n/' >> $@

$(obj)/capability.o : $(obj)/capability_names.h
$(obj)/resource.o : $(obj)/rlim_names.h
<<<<<<< HEAD
$(obj)/capability_names.h : $(srctree)/include/linux/capability.h \
			    $(src)/Makefile
	$(call cmd,make-caps)
$(obj)/rlim_names.h : $(srctree)/include/asm-generic/resource.h \
		      $(src)/Makefile
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(obj)/capability_names.h : $(srctree)/include/uapi/linux/capability.h \
			    $(src)/Makefile
	$(call cmd,make-caps)
$(obj)/rlim_names.h : $(srctree)/include/uapi/asm-generic/resource.h \
		      $(src)/Makefile
>>>>>>> refs/remotes/origin/master
	$(call cmd,make-rlim)
