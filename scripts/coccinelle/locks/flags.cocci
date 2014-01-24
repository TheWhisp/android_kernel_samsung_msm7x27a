/// Find nested lock+irqsave functions that use the same flags variables
///
// Confidence: High
<<<<<<< HEAD
<<<<<<< HEAD
// Copyright: (C) 2010 Nicolas Palix, DIKU.  GPLv2.
// Copyright: (C) 2010 Julia Lawall, DIKU.  GPLv2.
// Copyright: (C) 2010 Gilles Muller, INRIA/LiP6.  GPLv2.
=======
// Copyright: (C) 2010-2012 Nicolas Palix.  GPLv2.
// Copyright: (C) 2010-2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2010-2012 Gilles Muller, INRIA/LiP6.  GPLv2.
>>>>>>> refs/remotes/origin/cm-10.0
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: -no_includes -include_headers
=======
// Copyright: (C) 2010-2012 Nicolas Palix.  GPLv2.
// Copyright: (C) 2010-2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2010-2012 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: --no-includes --include-headers
>>>>>>> refs/remotes/origin/master

virtual context
virtual org
virtual report

<<<<<<< HEAD
<<<<<<< HEAD
@r@
=======
@r exists@
>>>>>>> refs/remotes/origin/cm-10.0
=======
@r exists@
>>>>>>> refs/remotes/origin/master
expression lock1,lock2,flags;
position p1,p2;
@@

(
spin_lock_irqsave@p1(lock1,flags)
|
read_lock_irqsave@p1(lock1,flags)
|
write_lock_irqsave@p1(lock1,flags)
)
... when != flags
(
spin_lock_irqsave(lock1,flags)
|
read_lock_irqsave(lock1,flags)
|
write_lock_irqsave(lock1,flags)
|
spin_lock_irqsave@p2(lock2,flags)
|
read_lock_irqsave@p2(lock2,flags)
|
write_lock_irqsave@p2(lock2,flags)
)

<<<<<<< HEAD
<<<<<<< HEAD
@d@
=======
@d exists@
>>>>>>> refs/remotes/origin/cm-10.0
=======
@d exists@
>>>>>>> refs/remotes/origin/master
expression f <= r.flags;
expression lock1,lock2,flags;
position r.p1, r.p2;
@@

(
*spin_lock_irqsave@p1(lock1,flags)
|
*read_lock_irqsave@p1(lock1,flags)
|
*write_lock_irqsave@p1(lock1,flags)
)
... when != f
(
*spin_lock_irqsave@p2(lock2,flags)
|
*read_lock_irqsave@p2(lock2,flags)
|
*write_lock_irqsave@p2(lock2,flags)
)

// ----------------------------------------------------------------------

@script:python depends on d && org@
p1 << r.p1;
p2 << r.p2;
@@

cocci.print_main("original lock",p1)
cocci.print_secs("nested lock+irqsave that reuses flags",p2)

@script:python depends on d && report@
p1 << r.p1;
p2 << r.p2;
@@

<<<<<<< HEAD
<<<<<<< HEAD
msg="ERROR: nested lock+irqsave that reuses flags from %s." % (p1[0].line)
=======
msg="ERROR: nested lock+irqsave that reuses flags from line %s." % (p1[0].line)
>>>>>>> refs/remotes/origin/cm-10.0
=======
msg="ERROR: nested lock+irqsave that reuses flags from line %s." % (p1[0].line)
>>>>>>> refs/remotes/origin/master
coccilib.report.print_report(p2[0], msg)
