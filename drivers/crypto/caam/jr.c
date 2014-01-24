/*
 * CAAM/SEC 4.x transport/backend driver
 * JobR backend functionality
 *
<<<<<<< HEAD
 * Copyright 2008-2011 Freescale Semiconductor, Inc.
 */

=======
 * Copyright 2008-2012 Freescale Semiconductor, Inc.
 */

#include <linux/of_irq.h>
#include <linux/of_address.h>

>>>>>>> refs/remotes/origin/master
#include "compat.h"
#include "regs.h"
#include "jr.h"
#include "desc.h"
#include "intern.h"

<<<<<<< HEAD
=======
struct jr_driver_data {
	/* List of Physical JobR's with the Driver */
	struct list_head	jr_list;
	spinlock_t		jr_alloc_lock;	/* jr_list lock */
} ____cacheline_aligned;

static struct jr_driver_data driver_data;

static int caam_reset_hw_jr(struct device *dev)
{
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	unsigned int timeout = 100000;

	/*
	 * mask interrupts since we are going to poll
	 * for reset completion status
	 */
	setbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);

	/* initiate flush (required prior to reset) */
	wr_reg32(&jrp->rregs->jrcommand, JRCR_RESET);
	while (((rd_reg32(&jrp->rregs->jrintstatus) & JRINT_ERR_HALT_MASK) ==
		JRINT_ERR_HALT_INPROGRESS) && --timeout)
		cpu_relax();

	if ((rd_reg32(&jrp->rregs->jrintstatus) & JRINT_ERR_HALT_MASK) !=
	    JRINT_ERR_HALT_COMPLETE || timeout == 0) {
		dev_err(dev, "failed to flush job ring %d\n", jrp->ridx);
		return -EIO;
	}

	/* initiate reset */
	timeout = 100000;
	wr_reg32(&jrp->rregs->jrcommand, JRCR_RESET);
	while ((rd_reg32(&jrp->rregs->jrcommand) & JRCR_RESET) && --timeout)
		cpu_relax();

	if (timeout == 0) {
		dev_err(dev, "failed to reset job ring %d\n", jrp->ridx);
		return -EIO;
	}

	/* unmask interrupts */
	clrbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);

	return 0;
}

/*
 * Shutdown JobR independent of platform property code
 */
int caam_jr_shutdown(struct device *dev)
{
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	dma_addr_t inpbusaddr, outbusaddr;
	int ret;

	ret = caam_reset_hw_jr(dev);

	tasklet_kill(&jrp->irqtask);

	/* Release interrupt */
	free_irq(jrp->irq, dev);

	/* Free rings */
	inpbusaddr = rd_reg64(&jrp->rregs->inpring_base);
	outbusaddr = rd_reg64(&jrp->rregs->outring_base);
	dma_free_coherent(dev, sizeof(dma_addr_t) * JOBR_DEPTH,
			  jrp->inpring, inpbusaddr);
	dma_free_coherent(dev, sizeof(struct jr_outentry) * JOBR_DEPTH,
			  jrp->outring, outbusaddr);
	kfree(jrp->entinfo);

	return ret;
}

static int caam_jr_remove(struct platform_device *pdev)
{
	int ret;
	struct device *jrdev;
	struct caam_drv_private_jr *jrpriv;

	jrdev = &pdev->dev;
	jrpriv = dev_get_drvdata(jrdev);

	/*
	 * Return EBUSY if job ring already allocated.
	 */
	if (atomic_read(&jrpriv->tfm_count)) {
		dev_err(jrdev, "Device is busy\n");
		return -EBUSY;
	}

	/* Remove the node from Physical JobR list maintained by driver */
	spin_lock(&driver_data.jr_alloc_lock);
	list_del(&jrpriv->list_node);
	spin_unlock(&driver_data.jr_alloc_lock);

	/* Release ring */
	ret = caam_jr_shutdown(jrdev);
	if (ret)
		dev_err(jrdev, "Failed to shut down job ring\n");
	irq_dispose_mapping(jrpriv->irq);

	return ret;
}

>>>>>>> refs/remotes/origin/master
/* Main per-ring interrupt handler */
static irqreturn_t caam_jr_interrupt(int irq, void *st_dev)
{
	struct device *dev = st_dev;
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	u32 irqstate;

	/*
	 * Check the output ring for ready responses, kick
	 * tasklet if jobs done.
	 */
	irqstate = rd_reg32(&jrp->rregs->jrintstatus);
	if (!irqstate)
		return IRQ_NONE;

	/*
	 * If JobR error, we got more development work to do
	 * Flag a bug now, but we really need to shut down and
	 * restart the queue (and fix code).
	 */
	if (irqstate & JRINT_JR_ERROR) {
		dev_err(dev, "job ring error: irqstate: %08x\n", irqstate);
		BUG();
	}

	/* mask valid interrupts */
	setbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);

	/* Have valid interrupt at this point, just ACK and trigger */
	wr_reg32(&jrp->rregs->jrintstatus, irqstate);

	preempt_disable();
<<<<<<< HEAD
	tasklet_schedule(&jrp->irqtask[smp_processor_id()]);
=======
	tasklet_schedule(&jrp->irqtask);
>>>>>>> refs/remotes/origin/master
	preempt_enable();

	return IRQ_HANDLED;
}

/* Deferred service handler, run as interrupt-fired tasklet */
static void caam_jr_dequeue(unsigned long devarg)
{
	int hw_idx, sw_idx, i, head, tail;
	struct device *dev = (struct device *)devarg;
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	void (*usercall)(struct device *dev, u32 *desc, u32 status, void *arg);
	u32 *userdesc, userstatus;
	void *userarg;
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&jrp->outlock, flags);

	head = ACCESS_ONCE(jrp->head);
	sw_idx = tail = jrp->tail;

	while (CIRC_CNT(head, tail, JOBR_DEPTH) >= 1 &&
	       rd_reg32(&jrp->rregs->outring_used)) {

		hw_idx = jrp->out_ring_read_index;
=======

	while (rd_reg32(&jrp->rregs->outring_used)) {

		head = ACCESS_ONCE(jrp->head);

		spin_lock(&jrp->outlock);

		sw_idx = tail = jrp->tail;
		hw_idx = jrp->out_ring_read_index;

>>>>>>> refs/remotes/origin/master
		for (i = 0; CIRC_CNT(head, tail + i, JOBR_DEPTH) >= 1; i++) {
			sw_idx = (tail + i) & (JOBR_DEPTH - 1);

			smp_read_barrier_depends();

			if (jrp->outring[hw_idx].desc ==
			    jrp->entinfo[sw_idx].desc_addr_dma)
				break; /* found */
		}
		/* we should never fail to find a matching descriptor */
		BUG_ON(CIRC_CNT(head, tail + i, JOBR_DEPTH) <= 0);

		/* Unmap just-run descriptor so we can post-process */
		dma_unmap_single(dev, jrp->outring[hw_idx].desc,
				 jrp->entinfo[sw_idx].desc_size,
				 DMA_TO_DEVICE);

		/* mark completed, avoid matching on a recycled desc addr */
		jrp->entinfo[sw_idx].desc_addr_dma = 0;

		/* Stash callback params for use outside of lock */
		usercall = jrp->entinfo[sw_idx].callbk;
		userarg = jrp->entinfo[sw_idx].cbkarg;
		userdesc = jrp->entinfo[sw_idx].desc_addr_virt;
		userstatus = jrp->outring[hw_idx].jrstatus;

<<<<<<< HEAD
		smp_mb();
=======
		/* set done */
		wr_reg32(&jrp->rregs->outring_rmvd, 1);
>>>>>>> refs/remotes/origin/master

		jrp->out_ring_read_index = (jrp->out_ring_read_index + 1) &
					   (JOBR_DEPTH - 1);

		/*
		 * if this job completed out-of-order, do not increment
		 * the tail.  Otherwise, increment tail by 1 plus the
		 * number of subsequent jobs already completed out-of-order
		 */
		if (sw_idx == tail) {
			do {
				tail = (tail + 1) & (JOBR_DEPTH - 1);
				smp_read_barrier_depends();
			} while (CIRC_CNT(head, tail, JOBR_DEPTH) >= 1 &&
				 jrp->entinfo[tail].desc_addr_dma == 0);

			jrp->tail = tail;
		}

<<<<<<< HEAD
		/* set done */
		wr_reg32(&jrp->rregs->outring_rmvd, 1);

		spin_unlock_irqrestore(&jrp->outlock, flags);

		/* Finally, execute user's callback */
		usercall(dev, userdesc, userstatus, userarg);

		spin_lock_irqsave(&jrp->outlock, flags);

		head = ACCESS_ONCE(jrp->head);
		sw_idx = tail = jrp->tail;
	}

	spin_unlock_irqrestore(&jrp->outlock, flags);

=======
		spin_unlock(&jrp->outlock);

		/* Finally, execute user's callback */
		usercall(dev, userdesc, userstatus, userarg);
	}

>>>>>>> refs/remotes/origin/master
	/* reenable / unmask IRQs */
	clrbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);
}

/**
<<<<<<< HEAD
 * caam_jr_register() - Alloc a ring for someone to use as needed. Returns
 * an ordinal of the rings allocated, else returns -ENODEV if no rings
 * are available.
 * @ctrldev: points to the controller level dev (parent) that
 *           owns rings available for use.
 * @dev:     points to where a pointer to the newly allocated queue's
 *           dev can be written to if successful.
 **/
int caam_jr_register(struct device *ctrldev, struct device **rdev)
{
	struct caam_drv_private *ctrlpriv = dev_get_drvdata(ctrldev);
	struct caam_drv_private_jr *jrpriv = NULL;
	unsigned long flags;
	int ring;

	/* Lock, if free ring - assign, unlock */
	spin_lock_irqsave(&ctrlpriv->jr_alloc_lock, flags);
	for (ring = 0; ring < ctrlpriv->total_jobrs; ring++) {
		jrpriv = dev_get_drvdata(ctrlpriv->jrdev[ring]);
		if (jrpriv->assign == JOBR_UNASSIGNED) {
			jrpriv->assign = JOBR_ASSIGNED;
			*rdev = ctrlpriv->jrdev[ring];
			spin_unlock_irqrestore(&ctrlpriv->jr_alloc_lock, flags);
			return ring;
		}
	}

	/* If assigned, write dev where caller needs it */
	spin_unlock_irqrestore(&ctrlpriv->jr_alloc_lock, flags);
	*rdev = NULL;

	return -ENODEV;
}
EXPORT_SYMBOL(caam_jr_register);

/**
 * caam_jr_deregister() - Deregister an API and release the queue.
 * Returns 0 if OK, -EBUSY if queue still contains pending entries
 * or unprocessed results at the time of the call
 * @dev     - points to the dev that identifies the queue to
 *            be released.
 **/
int caam_jr_deregister(struct device *rdev)
{
	struct caam_drv_private_jr *jrpriv = dev_get_drvdata(rdev);
	struct caam_drv_private *ctrlpriv;
	unsigned long flags;

	/* Get the owning controller's private space */
	ctrlpriv = dev_get_drvdata(jrpriv->parentdev);

	/*
	 * Make sure ring empty before release
	 */
	if (rd_reg32(&jrpriv->rregs->outring_used) ||
	    (rd_reg32(&jrpriv->rregs->inpring_avail) != JOBR_DEPTH))
		return -EBUSY;

	/* Release ring */
	spin_lock_irqsave(&ctrlpriv->jr_alloc_lock, flags);
	jrpriv->assign = JOBR_UNASSIGNED;
	spin_unlock_irqrestore(&ctrlpriv->jr_alloc_lock, flags);

	return 0;
}
EXPORT_SYMBOL(caam_jr_deregister);
=======
 * caam_jr_alloc() - Alloc a job ring for someone to use as needed.
 *
 * returns :  pointer to the newly allocated physical
 *	      JobR dev can be written to if successful.
 **/
struct device *caam_jr_alloc(void)
{
	struct caam_drv_private_jr *jrpriv, *min_jrpriv = NULL;
	struct device *dev = NULL;
	int min_tfm_cnt	= INT_MAX;
	int tfm_cnt;

	spin_lock(&driver_data.jr_alloc_lock);

	if (list_empty(&driver_data.jr_list)) {
		spin_unlock(&driver_data.jr_alloc_lock);
		return ERR_PTR(-ENODEV);
	}

	list_for_each_entry(jrpriv, &driver_data.jr_list, list_node) {
		tfm_cnt = atomic_read(&jrpriv->tfm_count);
		if (tfm_cnt < min_tfm_cnt) {
			min_tfm_cnt = tfm_cnt;
			min_jrpriv = jrpriv;
		}
		if (!min_tfm_cnt)
			break;
	}

	if (min_jrpriv) {
		atomic_inc(&min_jrpriv->tfm_count);
		dev = min_jrpriv->dev;
	}
	spin_unlock(&driver_data.jr_alloc_lock);

	return dev;
}
EXPORT_SYMBOL(caam_jr_alloc);

/**
 * caam_jr_free() - Free the Job Ring
 * @rdev     - points to the dev that identifies the Job ring to
 *             be released.
 **/
void caam_jr_free(struct device *rdev)
{
	struct caam_drv_private_jr *jrpriv = dev_get_drvdata(rdev);

	atomic_dec(&jrpriv->tfm_count);
}
EXPORT_SYMBOL(caam_jr_free);
>>>>>>> refs/remotes/origin/master

/**
 * caam_jr_enqueue() - Enqueue a job descriptor head. Returns 0 if OK,
 * -EBUSY if the queue is full, -EIO if it cannot map the caller's
 * descriptor.
 * @dev:  device of the job ring to be used. This device should have
 *        been assigned prior by caam_jr_register().
 * @desc: points to a job descriptor that execute our request. All
 *        descriptors (and all referenced data) must be in a DMAable
 *        region, and all data references must be physical addresses
 *        accessible to CAAM (i.e. within a PAMU window granted
 *        to it).
 * @cbk:  pointer to a callback function to be invoked upon completion
 *        of this request. This has the form:
 *        callback(struct device *dev, u32 *desc, u32 stat, void *arg)
 *        where:
 *        @dev:    contains the job ring device that processed this
 *                 response.
 *        @desc:   descriptor that initiated the request, same as
 *                 "desc" being argued to caam_jr_enqueue().
 *        @status: untranslated status received from CAAM. See the
 *                 reference manual for a detailed description of
 *                 error meaning, or see the JRSTA definitions in the
 *                 register header file
 *        @areq:   optional pointer to an argument passed with the
 *                 original request
 * @areq: optional pointer to a user argument for use at callback
 *        time.
 **/
int caam_jr_enqueue(struct device *dev, u32 *desc,
		    void (*cbk)(struct device *dev, u32 *desc,
				u32 status, void *areq),
		    void *areq)
{
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	struct caam_jrentry_info *head_entry;
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> refs/remotes/origin/master
	int head, tail, desc_size;
	dma_addr_t desc_dma;

	desc_size = (*desc & HDR_JD_LENGTH_MASK) * sizeof(u32);
	desc_dma = dma_map_single(dev, desc, desc_size, DMA_TO_DEVICE);
	if (dma_mapping_error(dev, desc_dma)) {
		dev_err(dev, "caam_jr_enqueue(): can't map jobdesc\n");
		return -EIO;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&jrp->inplock, flags);
=======
	spin_lock_bh(&jrp->inplock);
>>>>>>> refs/remotes/origin/master

	head = jrp->head;
	tail = ACCESS_ONCE(jrp->tail);

	if (!rd_reg32(&jrp->rregs->inpring_avail) ||
	    CIRC_SPACE(head, tail, JOBR_DEPTH) <= 0) {
<<<<<<< HEAD
		spin_unlock_irqrestore(&jrp->inplock, flags);
=======
		spin_unlock_bh(&jrp->inplock);
>>>>>>> refs/remotes/origin/master
		dma_unmap_single(dev, desc_dma, desc_size, DMA_TO_DEVICE);
		return -EBUSY;
	}

	head_entry = &jrp->entinfo[head];
	head_entry->desc_addr_virt = desc;
	head_entry->desc_size = desc_size;
	head_entry->callbk = (void *)cbk;
	head_entry->cbkarg = areq;
	head_entry->desc_addr_dma = desc_dma;

	jrp->inpring[jrp->inp_ring_write_index] = desc_dma;

	smp_wmb();

	jrp->inp_ring_write_index = (jrp->inp_ring_write_index + 1) &
				    (JOBR_DEPTH - 1);
	jrp->head = (head + 1) & (JOBR_DEPTH - 1);

<<<<<<< HEAD
	wmb();

	wr_reg32(&jrp->rregs->inpring_jobadd, 1);

	spin_unlock_irqrestore(&jrp->inplock, flags);
=======
	wr_reg32(&jrp->rregs->inpring_jobadd, 1);

	spin_unlock_bh(&jrp->inplock);
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL(caam_jr_enqueue);

<<<<<<< HEAD
static int caam_reset_hw_jr(struct device *dev)
{
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	unsigned int timeout = 100000;

	/*
	 * mask interrupts since we are going to poll
	 * for reset completion status
	 */
	setbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);

	/* initiate flush (required prior to reset) */
	wr_reg32(&jrp->rregs->jrcommand, JRCR_RESET);
	while (((rd_reg32(&jrp->rregs->jrintstatus) & JRINT_ERR_HALT_MASK) ==
		JRINT_ERR_HALT_INPROGRESS) && --timeout)
		cpu_relax();

	if ((rd_reg32(&jrp->rregs->jrintstatus) & JRINT_ERR_HALT_MASK) !=
	    JRINT_ERR_HALT_COMPLETE || timeout == 0) {
		dev_err(dev, "failed to flush job ring %d\n", jrp->ridx);
		return -EIO;
	}

	/* initiate reset */
	timeout = 100000;
	wr_reg32(&jrp->rregs->jrcommand, JRCR_RESET);
	while ((rd_reg32(&jrp->rregs->jrcommand) & JRCR_RESET) && --timeout)
		cpu_relax();

	if (timeout == 0) {
		dev_err(dev, "failed to reset job ring %d\n", jrp->ridx);
		return -EIO;
	}

	/* unmask interrupts */
	clrbits32(&jrp->rregs->rconfig_lo, JRCFG_IMSK);

	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Init JobR independent of platform property detection
 */
static int caam_jr_init(struct device *dev)
{
	struct caam_drv_private_jr *jrp;
	dma_addr_t inpbusaddr, outbusaddr;
	int i, error;

	jrp = dev_get_drvdata(dev);

<<<<<<< HEAD
	/* Connect job ring interrupt handler. */
	for_each_possible_cpu(i)
		tasklet_init(&jrp->irqtask[i], caam_jr_dequeue,
			     (unsigned long)dev);

	error = request_irq(jrp->irq, caam_jr_interrupt, IRQF_SHARED,
			    "caam-jobr", dev);
=======
	tasklet_init(&jrp->irqtask, caam_jr_dequeue, (unsigned long)dev);

	/* Connect job ring interrupt handler. */
	error = request_irq(jrp->irq, caam_jr_interrupt, IRQF_SHARED,
			    dev_name(dev), dev);
>>>>>>> refs/remotes/origin/master
	if (error) {
		dev_err(dev, "can't connect JobR %d interrupt (%d)\n",
			jrp->ridx, jrp->irq);
		irq_dispose_mapping(jrp->irq);
		jrp->irq = 0;
		return -EINVAL;
	}

	error = caam_reset_hw_jr(dev);
	if (error)
		return error;

<<<<<<< HEAD
	jrp->inpring = kzalloc(sizeof(dma_addr_t) * JOBR_DEPTH,
			       GFP_KERNEL | GFP_DMA);
	jrp->outring = kzalloc(sizeof(struct jr_outentry) *
			       JOBR_DEPTH, GFP_KERNEL | GFP_DMA);
=======
	jrp->inpring = dma_alloc_coherent(dev, sizeof(dma_addr_t) * JOBR_DEPTH,
					  &inpbusaddr, GFP_KERNEL);

	jrp->outring = dma_alloc_coherent(dev, sizeof(struct jr_outentry) *
					  JOBR_DEPTH, &outbusaddr, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master

	jrp->entinfo = kzalloc(sizeof(struct caam_jrentry_info) * JOBR_DEPTH,
			       GFP_KERNEL);

	if ((jrp->inpring == NULL) || (jrp->outring == NULL) ||
	    (jrp->entinfo == NULL)) {
		dev_err(dev, "can't allocate job rings for %d\n",
			jrp->ridx);
		return -ENOMEM;
	}

	for (i = 0; i < JOBR_DEPTH; i++)
		jrp->entinfo[i].desc_addr_dma = !0;

	/* Setup rings */
<<<<<<< HEAD
	inpbusaddr = dma_map_single(dev, jrp->inpring,
				    sizeof(u32 *) * JOBR_DEPTH,
				    DMA_BIDIRECTIONAL);
	if (dma_mapping_error(dev, inpbusaddr)) {
		dev_err(dev, "caam_jr_init(): can't map input ring\n");
		kfree(jrp->inpring);
		kfree(jrp->outring);
		kfree(jrp->entinfo);
		return -EIO;
	}

	outbusaddr = dma_map_single(dev, jrp->outring,
				    sizeof(struct jr_outentry) * JOBR_DEPTH,
				    DMA_BIDIRECTIONAL);
	if (dma_mapping_error(dev, outbusaddr)) {
		dev_err(dev, "caam_jr_init(): can't map output ring\n");
			dma_unmap_single(dev, inpbusaddr,
					 sizeof(u32 *) * JOBR_DEPTH,
					 DMA_BIDIRECTIONAL);
		kfree(jrp->inpring);
		kfree(jrp->outring);
		kfree(jrp->entinfo);
		return -EIO;
	}

=======
>>>>>>> refs/remotes/origin/master
	jrp->inp_ring_write_index = 0;
	jrp->out_ring_read_index = 0;
	jrp->head = 0;
	jrp->tail = 0;

	wr_reg64(&jrp->rregs->inpring_base, inpbusaddr);
	wr_reg64(&jrp->rregs->outring_base, outbusaddr);
	wr_reg32(&jrp->rregs->inpring_size, JOBR_DEPTH);
	wr_reg32(&jrp->rregs->outring_size, JOBR_DEPTH);

	jrp->ringsize = JOBR_DEPTH;

	spin_lock_init(&jrp->inplock);
	spin_lock_init(&jrp->outlock);

	/* Select interrupt coalescing parameters */
	setbits32(&jrp->rregs->rconfig_lo, JOBR_INTC |
		  (JOBR_INTC_COUNT_THLD << JRCFG_ICDCT_SHIFT) |
		  (JOBR_INTC_TIME_THLD << JRCFG_ICTT_SHIFT));

<<<<<<< HEAD
	jrp->assign = JOBR_UNASSIGNED;
	return 0;
}

/*
 * Shutdown JobR independent of platform property code
 */
int caam_jr_shutdown(struct device *dev)
{
	struct caam_drv_private_jr *jrp = dev_get_drvdata(dev);
	dma_addr_t inpbusaddr, outbusaddr;
	int ret, i;

	ret = caam_reset_hw_jr(dev);

	for_each_possible_cpu(i)
		tasklet_kill(&jrp->irqtask[i]);

	/* Release interrupt */
	free_irq(jrp->irq, dev);

	/* Free rings */
	inpbusaddr = rd_reg64(&jrp->rregs->inpring_base);
	outbusaddr = rd_reg64(&jrp->rregs->outring_base);
	dma_unmap_single(dev, outbusaddr,
			 sizeof(struct jr_outentry) * JOBR_DEPTH,
			 DMA_BIDIRECTIONAL);
	dma_unmap_single(dev, inpbusaddr, sizeof(u32 *) * JOBR_DEPTH,
			 DMA_BIDIRECTIONAL);
	kfree(jrp->outring);
	kfree(jrp->inpring);
	kfree(jrp->entinfo);

	return ret;
}

/*
 * Probe routine for each detected JobR subsystem. It assumes that
 * property detection was picked up externally.
 */
int caam_jr_probe(struct platform_device *pdev, struct device_node *np,
		  int ring)
{
	struct device *ctrldev, *jrdev;
	struct platform_device *jr_pdev;
	struct caam_drv_private *ctrlpriv;
	struct caam_drv_private_jr *jrpriv;
	u32 *jroffset;
	int error;

	ctrldev = &pdev->dev;
	ctrlpriv = dev_get_drvdata(ctrldev);

	jrpriv = kmalloc(sizeof(struct caam_drv_private_jr),
			 GFP_KERNEL);
	if (jrpriv == NULL) {
		dev_err(ctrldev, "can't alloc private mem for job ring %d\n",
			ring);
		return -ENOMEM;
	}
	jrpriv->parentdev = ctrldev; /* point back to parent */
	jrpriv->ridx = ring; /* save ring identity relative to detection */

	/*
	 * Derive a pointer to the detected JobRs regs
	 * Driver has already iomapped the entire space, we just
	 * need to add in the offset to this JobR. Don't know if I
	 * like this long-term, but it'll run
	 */
	jroffset = (u32 *)of_get_property(np, "reg", NULL);
	jrpriv->rregs = (struct caam_job_ring __iomem *)((void *)ctrlpriv->ctrl
							 + *jroffset);

	/* Build a local dev for each detected queue */
	jr_pdev = of_platform_device_create(np, NULL, ctrldev);
	if (jr_pdev == NULL) {
		kfree(jrpriv);
		return -EINVAL;
	}
	jrdev = &jr_pdev->dev;
	dev_set_drvdata(jrdev, jrpriv);
	ctrlpriv->jrdev[ring] = jrdev;

	/* Identify the interrupt */
	jrpriv->irq = of_irq_to_resource(np, 0, NULL);
=======
	return 0;
}


/*
 * Probe routine for each detected JobR subsystem.
 */
static int caam_jr_probe(struct platform_device *pdev)
{
	struct device *jrdev;
	struct device_node *nprop;
	struct caam_job_ring __iomem *ctrl;
	struct caam_drv_private_jr *jrpriv;
	static int total_jobrs;
	int error;

	jrdev = &pdev->dev;
	jrpriv = kmalloc(sizeof(struct caam_drv_private_jr),
			 GFP_KERNEL);
	if (!jrpriv)
		return -ENOMEM;

	dev_set_drvdata(jrdev, jrpriv);

	/* save ring identity relative to detection */
	jrpriv->ridx = total_jobrs++;

	nprop = pdev->dev.of_node;
	/* Get configuration properties from device tree */
	/* First, get register page */
	ctrl = of_iomap(nprop, 0);
	if (!ctrl) {
		dev_err(jrdev, "of_iomap() failed\n");
		return -ENOMEM;
	}

	jrpriv->rregs = (struct caam_job_ring __force *)ctrl;

	if (sizeof(dma_addr_t) == sizeof(u64))
		if (of_device_is_compatible(nprop, "fsl,sec-v5.0-job-ring"))
			dma_set_mask(jrdev, DMA_BIT_MASK(40));
		else
			dma_set_mask(jrdev, DMA_BIT_MASK(36));
	else
		dma_set_mask(jrdev, DMA_BIT_MASK(32));

	/* Identify the interrupt */
	jrpriv->irq = irq_of_parse_and_map(nprop, 0);
>>>>>>> refs/remotes/origin/master

	/* Now do the platform independent part */
	error = caam_jr_init(jrdev); /* now turn on hardware */
	if (error) {
		kfree(jrpriv);
		return error;
	}

<<<<<<< HEAD
	return error;
}
=======
	jrpriv->dev = jrdev;
	spin_lock(&driver_data.jr_alloc_lock);
	list_add_tail(&jrpriv->list_node, &driver_data.jr_list);
	spin_unlock(&driver_data.jr_alloc_lock);

	atomic_set(&jrpriv->tfm_count, 0);

	return 0;
}

static struct of_device_id caam_jr_match[] = {
	{
		.compatible = "fsl,sec-v4.0-job-ring",
	},
	{
		.compatible = "fsl,sec4.0-job-ring",
	},
	{},
};
MODULE_DEVICE_TABLE(of, caam_jr_match);

static struct platform_driver caam_jr_driver = {
	.driver = {
		.name = "caam_jr",
		.owner = THIS_MODULE,
		.of_match_table = caam_jr_match,
	},
	.probe       = caam_jr_probe,
	.remove      = caam_jr_remove,
};

static int __init jr_driver_init(void)
{
	spin_lock_init(&driver_data.jr_alloc_lock);
	INIT_LIST_HEAD(&driver_data.jr_list);
	return platform_driver_register(&caam_jr_driver);
}

static void __exit jr_driver_exit(void)
{
	platform_driver_unregister(&caam_jr_driver);
}

module_init(jr_driver_init);
module_exit(jr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FSL CAAM JR request backend");
MODULE_AUTHOR("Freescale Semiconductor - NMG/STC");
>>>>>>> refs/remotes/origin/master
