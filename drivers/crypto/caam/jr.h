/*
 * CAAM public-level include definitions for the JobR backend
 *
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 */

#ifndef JR_H
#define JR_H

/* Prototypes for backend-level services exposed to APIs */
<<<<<<< HEAD
int caam_jr_register(struct device *ctrldev, struct device **rdev);
int caam_jr_deregister(struct device *rdev);
=======
struct device *caam_jr_alloc(void);
void caam_jr_free(struct device *rdev);
>>>>>>> refs/remotes/origin/master
int caam_jr_enqueue(struct device *dev, u32 *desc,
		    void (*cbk)(struct device *dev, u32 *desc, u32 status,
				void *areq),
		    void *areq);

<<<<<<< HEAD
extern int caam_jr_probe(struct platform_device *pdev, struct device_node *np,
			 int ring);
extern int caam_jr_shutdown(struct device *dev);
=======
>>>>>>> refs/remotes/origin/master
#endif /* JR_H */
