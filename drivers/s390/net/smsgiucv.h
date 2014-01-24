/*
 * IUCV special message driver
 *
<<<<<<< HEAD
 * Copyright (C) 2003 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
 * Copyright IBM Corp. 2003
>>>>>>> refs/remotes/origin/master
 * Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#define SMSGIUCV_DRV_NAME     "SMSGIUCV"

int  smsg_register_callback(const char *,
			    void (*)(const char *, char *));
void smsg_unregister_callback(const char *,
			      void (*)(const char *, char *));

