/*
<<<<<<< HEAD
 *  include/asm-s390/current.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> refs/remotes/origin/master
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/current.h"
 */

#ifndef _S390_CURRENT_H
#define _S390_CURRENT_H

<<<<<<< HEAD
#ifdef __KERNEL__
=======
>>>>>>> refs/remotes/origin/master
#include <asm/lowcore.h>

struct task_struct;

#define current ((struct task_struct *const)S390_lowcore.current_task)

<<<<<<< HEAD
#endif

=======
>>>>>>> refs/remotes/origin/master
#endif /* !(_S390_CURRENT_H) */
