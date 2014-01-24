#
# Makefile for ACPICA Core interpreter
#

ccflags-y			:= -Os
ccflags-$(CONFIG_ACPI_DEBUG)	+= -DACPI_DEBUG_OUTPUT

# use acpi.o to put all files here into acpi.o modparam namespace
obj-y	+= acpi.o

<<<<<<< HEAD
acpi-y := dsfield.o   dsmthdat.o  dsopcode.o  dswexec.o  dswscope.o \
	 dsmethod.o  dsobject.o  dsutils.o   dswload.o  dswstate.o \
	 dsinit.o dsargs.o dscontrol.o dswload2.o

acpi-y += evevent.o  evregion.o  evsci.o    evxfevnt.o \
	 evmisc.o   evrgnini.o  evxface.o  evxfregn.o \
	 evgpe.o    evgpeblk.o evgpeinit.o  evgpeutil.o evxfgpe.o evglock.o

acpi-y += exconfig.o  exfield.o  exnames.o   exoparg6.o  exresolv.o  exstorob.o\
	 exconvrt.o  exfldio.o  exoparg1.o  exprep.o    exresop.o   exsystem.o\
	 excreate.o  exmisc.o   exoparg2.o  exregion.o  exstore.o   exutils.o \
	 exdump.o    exmutex.o  exoparg3.o  exresnte.o  exstoren.o  exdebug.o

acpi-y += hwacpi.o  hwgpe.o  hwregs.o  hwsleep.o hwxface.o hwvalid.o hwpci.o

acpi-$(ACPI_FUTURE_USAGE) += hwtimer.o

acpi-y += nsaccess.o  nsload.o    nssearch.o  nsxfeval.o \
	 nsalloc.o   nseval.o    nsnames.o   nsutils.o   nsxfname.o \
	 nsdump.o    nsinit.o    nsobject.o  nswalk.o    nsxfobj.o  \
	 nsparse.o   nspredef.o  nsrepair.o  nsrepair2.o

acpi-$(ACPI_FUTURE_USAGE) += nsdumpdv.o

acpi-y += psargs.o    psparse.o  psloop.o pstree.o   pswalk.o  \
	 psopcode.o  psscope.o  psutils.o  psxface.o

acpi-y += rsaddr.o rscreate.o rsinfo.o rsio.o rslist.o rsmisc.o rsxface.o \
	 rscalc.o  rsirq.o  rsmemory.o  rsutils.o

acpi-$(ACPI_FUTURE_USAGE) += rsdump.o

acpi-y += tbxface.o tbinstal.o tbutils.o tbfind.o tbfadt.o tbxfroot.o

acpi-y += utalloc.o utdebug.o uteval.o utinit.o utmisc.o utxface.o \
		utcopy.o utdelete.o utglobal.o utmath.o utobject.o \
		utstate.o utmutex.o utobject.o utresrc.o utlock.o utids.o \
		utosi.o utxferror.o utdecode.o
=======
acpi-y :=		\
	dsargs.o	\
	dscontrol.o	\
	dsfield.o	\
	dsinit.o	\
	dsmethod.o	\
	dsmthdat.o	\
	dsobject.o	\
	dsopcode.o	\
	dsutils.o	\
	dswexec.o	\
	dswload.o	\
	dswload2.o	\
	dswscope.o	\
	dswstate.o

acpi-y +=		\
	evevent.o	\
	evgpe.o		\
	evgpeblk.o	\
	evgpeinit.o	\
	evgpeutil.o	\
	evglock.o	\
	evmisc.o	\
	evregion.o	\
	evrgnini.o	\
	evsci.o		\
	evxface.o	\
	evxfevnt.o	\
	evxfgpe.o	\
	evxfregn.o

acpi-y +=		\
	exconfig.o	\
	exconvrt.o	\
	excreate.o	\
	exdebug.o	\
	exdump.o	\
	exfield.o	\
	exfldio.o	\
	exmutex.o	\
	exnames.o	\
	exoparg1.o	\
	exoparg2.o	\
	exoparg3.o	\
	exoparg6.o	\
	exprep.o	\
	exmisc.o	\
	exregion.o	\
	exresnte.o	\
	exresolv.o	\
	exresop.o	\
	exstore.o	\
	exstoren.o	\
	exstorob.o	\
	exsystem.o	\
	exutils.o

acpi-y +=		\
	hwacpi.o	\
	hwesleep.o	\
	hwgpe.o		\
	hwpci.o		\
	hwregs.o	\
	hwsleep.o	\
	hwvalid.o	\
	hwxface.o	\
	hwxfsleep.o

acpi-$(ACPI_FUTURE_USAGE) += hwtimer.o

acpi-y +=		\
	nsaccess.o	\
	nsalloc.o	\
	nsdump.o	\
	nseval.o	\
	nsinit.o	\
	nsload.o	\
	nsnames.o	\
	nsobject.o	\
	nsparse.o	\
	nspredef.o	\
	nsrepair.o	\
	nsrepair2.o	\
	nssearch.o	\
	nsutils.o	\
	nswalk.o	\
	nsxfeval.o	\
	nsxfname.o	\
	nsxfobj.o

acpi-$(ACPI_FUTURE_USAGE) += nsdumpdv.o

acpi-y +=		\
	psargs.o	\
	psloop.o	\
	psopcode.o	\
	psparse.o	\
	psscope.o	\
	pstree.o	\
	psutils.o	\
	pswalk.o	\
	psxface.o

acpi-y +=		\
	rsaddr.o	\
	rscalc.o	\
	rscreate.o	\
	rsinfo.o	\
	rsio.o		\
	rsirq.o		\
	rslist.o	\
	rsmemory.o	\
	rsmisc.o	\
	rsserial.o	\
	rsutils.o	\
	rsxface.o

acpi-$(ACPI_FUTURE_USAGE) += rsdump.o

acpi-y +=		\
	tbfadt.o	\
	tbfind.o	\
	tbinstal.o	\
	tbutils.o	\
	tbxface.o	\
	tbxfroot.o

acpi-y +=		\
	utaddress.o	\
	utalloc.o	\
	utcopy.o	\
	utdebug.o	\
	utdecode.o	\
	utdelete.o	\
	uteval.o	\
	utglobal.o	\
	utids.o		\
	utinit.o	\
	utlock.o	\
	utmath.o	\
	utmisc.o	\
	utmutex.o	\
	utobject.o	\
	utosi.o		\
	utresrc.o	\
	utstate.o	\
	utxface.o	\
	utxferror.o	\
	utxfmutex.o
>>>>>>> refs/remotes/origin/cm-10.0
