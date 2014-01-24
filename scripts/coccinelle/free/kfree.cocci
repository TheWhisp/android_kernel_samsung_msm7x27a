/// Find a use after free.
//# Values of variables may imply that some
//# execution paths are not possible, resulting in false positives.
//# Another source of false positives are macros such as
//# SCTP_DBG_OBJCNT_DEC that do not actually evaluate their argument
///
// Confidence: Moderate
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

virtual org
virtual report

@free@
expression E;
position p1;
@@

kfree@p1(E)

@print expression@
<<<<<<< HEAD
constant char *c;
=======
constant char [] c;
>>>>>>> refs/remotes/origin/cm-10.0
expression free.E,E2;
type T;
position p;
identifier f;
@@

(
 f(...,c,...,(T)E@p,...)
|
 E@p == E2
|
 E@p != E2
|
<<<<<<< HEAD
=======
 E2 == E@p
|
 E2 != E@p
|
>>>>>>> refs/remotes/origin/cm-10.0
 !E@p
|
 E@p || ...
)

@sz@
expression free.E;
position p;
@@

 sizeof(<+...E@p...+>)

@loop exists@
expression E;
identifier l;
position ok;
@@

while (1) { ...
  kfree@ok(E)
  ... when != break;
      when != goto l;
      when forall
}

@r exists@
expression free.E, subE<=free.E, E2;
expression E1;
iterator iter;
statement S;
position free.p1!=loop.ok,p2!={print.p,sz.p};
@@

kfree@p1(E,...)
...
(
 iter(...,subE,...) S // no use
|
 list_remove_head(E1,subE,...)
|
 subE = E2
|
 subE++
|
 ++subE
|
 --subE
|
 subE--
|
 &subE
|
 BUG(...)
|
 BUG_ON(...)
|
 return_VALUE(...)
|
 return_ACPI_STATUS(...)
|
 E@p2 // bad use
)

@script:python depends on org@
p1 << free.p1;
p2 << r.p2;
@@

cocci.print_main("kfree",p1)
cocci.print_secs("ref",p2)

@script:python depends on report@
p1 << free.p1;
p2 << r.p2;
@@

<<<<<<< HEAD
msg = "reference preceded by free on line %s" % (p1[0].line)
=======
msg = "ERROR: reference preceded by free on line %s" % (p1[0].line)
>>>>>>> refs/remotes/origin/cm-10.0
coccilib.report.print_report(p2[0],msg)
