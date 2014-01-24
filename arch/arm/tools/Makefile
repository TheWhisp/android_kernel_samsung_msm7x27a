#
# linux/arch/arm/tools/Makefile
#
# Copyright (C) 2001 Russell King
#

include/generated/mach-types.h: $(src)/gen-mach-types $(src)/mach-types
<<<<<<< HEAD
	@echo '  Generating $@'
=======
	@$(kecho) '  Generating $@'
>>>>>>> refs/remotes/origin/master
	@mkdir -p $(dir $@)
	$(Q)$(AWK) -f $^ > $@ || { rm -f $@; /bin/false; }
