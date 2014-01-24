/*
 * PCBIT-D interface with isdn4linux
 *
 * Copyright (C) 1996 Universidade de Lisboa
<<<<<<< HEAD
<<<<<<< HEAD
 * 
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of 
=======
=======
>>>>>>> refs/remotes/origin/master
 *
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * the GNU General Public License, incorporated herein by reference.
 */

/*
 *	Fixes:
 *
 *	Nuno Grilo	<l38486@alfa.ist.utl.pt>
 *      fixed msn_list NULL pointer dereference.
<<<<<<< HEAD
<<<<<<< HEAD
 *		
=======
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>


#include <linux/kernel.h>

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/skbuff.h>

#include <linux/isdnif.h>
#include <asm/string.h>
#include <asm/io.h>
#include <linux/ioport.h>

#include "pcbit.h"
#include "edss1.h"
#include "layer2.h"
#include "capi.h"


extern ushort last_ref_num;

<<<<<<< HEAD
<<<<<<< HEAD
static int pcbit_ioctl(isdn_ctrl* ctl);

static char* pcbit_devname[MAX_PCBIT_CARDS] = {
=======
static int pcbit_ioctl(isdn_ctrl *ctl);

static char *pcbit_devname[MAX_PCBIT_CARDS] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int pcbit_ioctl(isdn_ctrl *ctl);

static char *pcbit_devname[MAX_PCBIT_CARDS] = {
>>>>>>> refs/remotes/origin/master
	"pcbit0",
	"pcbit1",
	"pcbit2",
	"pcbit3"
};

/*
 * prototypes
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int pcbit_command(isdn_ctrl* ctl);
static int pcbit_stat(u_char __user * buf, int len, int, int);
static int pcbit_xmit(int driver, int chan, int ack, struct sk_buff *skb);
static int pcbit_writecmd(const u_char __user *, int, int, int);

static int set_protocol_running(struct pcbit_dev * dev);
=======
=======
>>>>>>> refs/remotes/origin/master
static int pcbit_command(isdn_ctrl *ctl);
static int pcbit_stat(u_char __user *buf, int len, int, int);
static int pcbit_xmit(int driver, int chan, int ack, struct sk_buff *skb);
static int pcbit_writecmd(const u_char __user *, int, int, int);

static int set_protocol_running(struct pcbit_dev *dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static void pcbit_clear_msn(struct pcbit_dev *dev);
static void pcbit_set_msn(struct pcbit_dev *dev, char *list);
static int pcbit_check_msn(struct pcbit_dev *dev, char *msn);


int pcbit_init_dev(int board, int mem_base, int irq)
{
	struct pcbit_dev *dev;
	isdn_if *dev_if;

<<<<<<< HEAD
<<<<<<< HEAD
	if ((dev=kzalloc(sizeof(struct pcbit_dev), GFP_KERNEL)) == NULL)
=======
	if ((dev = kzalloc(sizeof(struct pcbit_dev), GFP_KERNEL)) == NULL)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((dev = kzalloc(sizeof(struct pcbit_dev), GFP_KERNEL)) == NULL)
>>>>>>> refs/remotes/origin/master
	{
		printk("pcbit_init: couldn't malloc pcbit_dev struct\n");
		return -ENOMEM;
	}

	dev_pcbit[board] = dev;
	init_waitqueue_head(&dev->set_running_wq);
	spin_lock_init(&dev->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	if (mem_base >= 0xA0000 && mem_base <= 0xFFFFF ) {
		dev->ph_mem = mem_base;
		if (!request_mem_region(dev->ph_mem, 4096, "PCBIT mem")) {
			printk(KERN_WARNING
				"PCBIT: memory region %lx-%lx already in use\n",
				dev->ph_mem, dev->ph_mem + 4096);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (mem_base >= 0xA0000 && mem_base <= 0xFFFFF) {
		dev->ph_mem = mem_base;
		if (!request_mem_region(dev->ph_mem, 4096, "PCBIT mem")) {
			printk(KERN_WARNING
			       "PCBIT: memory region %lx-%lx already in use\n",
			       dev->ph_mem, dev->ph_mem + 4096);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			kfree(dev);
			dev_pcbit[board] = NULL;
			return -EACCES;
		}
		dev->sh_mem = ioremap(dev->ph_mem, 4096);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	else 
=======
	else
>>>>>>> refs/remotes/origin/cm-10.0
=======
	else
>>>>>>> refs/remotes/origin/master
	{
		printk("memory address invalid");
		kfree(dev);
		dev_pcbit[board] = NULL;
		return -EACCES;
	}

	dev->b1 = kzalloc(sizeof(struct pcbit_chan), GFP_KERNEL);
	if (!dev->b1) {
		printk("pcbit_init: couldn't malloc pcbit_chan struct\n");
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
		return -ENOMEM;
	}
<<<<<<< HEAD
<<<<<<< HEAD
    
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	dev->b2 = kzalloc(sizeof(struct pcbit_chan), GFP_KERNEL);
	if (!dev->b2) {
		printk("pcbit_init: couldn't malloc pcbit_chan struct\n");
		kfree(dev->b1);
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
		return -ENOMEM;
	}

	dev->b2->id = 1;

	INIT_WORK(&dev->qdelivery, pcbit_deliver);

	/*
	 *  interrupts
	 */

<<<<<<< HEAD
<<<<<<< HEAD
	if (request_irq(irq, &pcbit_irq_handler, 0, pcbit_devname[board], dev) != 0) 
=======
	if (request_irq(irq, &pcbit_irq_handler, 0, pcbit_devname[board], dev) != 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (request_irq(irq, &pcbit_irq_handler, 0, pcbit_devname[board], dev) != 0)
>>>>>>> refs/remotes/origin/master
	{
		kfree(dev->b1);
		kfree(dev->b2);
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
		dev_pcbit[board] = NULL;
		return -EIO;
	}

	dev->irq = irq;

	/* next frame to be received */
	dev->rcv_seq = 0;
	dev->send_seq = 0;
	dev->unack_seq = 0;

	dev->hl_hdrlen = 16;

	dev_if = kmalloc(sizeof(isdn_if), GFP_KERNEL);

	if (!dev_if) {
		free_irq(irq, dev);
		kfree(dev->b1);
		kfree(dev->b2);
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
		dev_pcbit[board] = NULL;
		return -EIO;
	}

	dev->dev_if = dev_if;

	dev_if->owner = THIS_MODULE;

	dev_if->channels = 2;
<<<<<<< HEAD
<<<<<<< HEAD
	
	dev_if->features = (ISDN_FEATURE_P_EURO  | ISDN_FEATURE_L3_TRANS | 
			    ISDN_FEATURE_L2_HDLC | ISDN_FEATURE_L2_TRANS );
=======

	dev_if->features = (ISDN_FEATURE_P_EURO  | ISDN_FEATURE_L3_TRANS |
			    ISDN_FEATURE_L2_HDLC | ISDN_FEATURE_L2_TRANS);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	dev_if->features = (ISDN_FEATURE_P_EURO  | ISDN_FEATURE_L3_TRANS |
			    ISDN_FEATURE_L2_HDLC | ISDN_FEATURE_L2_TRANS);
>>>>>>> refs/remotes/origin/master

	dev_if->writebuf_skb = pcbit_xmit;
	dev_if->hl_hdrlen = 16;

	dev_if->maxbufsize = MAXBUFSIZE;
	dev_if->command  = pcbit_command;
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	dev_if->writecmd = pcbit_writecmd;
	dev_if->readstat = pcbit_stat;


	strcpy(dev_if->id, pcbit_devname[board]);

	if (!register_isdn(dev_if)) {
		free_irq(irq, dev);
		kfree(dev->b1);
		kfree(dev->b2);
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
		dev_pcbit[board] = NULL;
		return -EIO;
	}

	dev->id = dev_if->channels;


	dev->l2_state = L2_DOWN;
	dev->free = 511;

	/*
	 * set_protocol_running(dev);
	 */

	return 0;
}

#ifdef MODULE
void pcbit_terminate(int board)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcbit_dev * dev;
=======
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/master

	dev = dev_pcbit[board];

	if (dev) {
<<<<<<< HEAD
<<<<<<< HEAD
	     /* unregister_isdn(dev->dev_if); */
=======
		/* unregister_isdn(dev->dev_if); */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/* unregister_isdn(dev->dev_if); */
>>>>>>> refs/remotes/origin/master
		free_irq(dev->irq, dev);
		pcbit_clear_msn(dev);
		kfree(dev->dev_if);
		if (dev->b1->fsm_timer.function)
			del_timer(&dev->b1->fsm_timer);
		if (dev->b2->fsm_timer.function)
			del_timer(&dev->b2->fsm_timer);
		kfree(dev->b1);
		kfree(dev->b2);
		iounmap(dev->sh_mem);
		release_mem_region(dev->ph_mem, 4096);
		kfree(dev);
	}
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
static int pcbit_command(isdn_ctrl* ctl)
{
	struct pcbit_dev  *dev;
=======
static int pcbit_command(isdn_ctrl *ctl)
{
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int pcbit_command(isdn_ctrl *ctl)
{
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/master
	struct pcbit_chan *chan;
	struct callb_data info;

	dev = finddev(ctl->driver);

	if (!dev)
	{
		printk("pcbit_command: unknown device\n");
		return -1;
	}

	chan = (ctl->arg & 0x0F) ? dev->b2 : dev->b1;


<<<<<<< HEAD
<<<<<<< HEAD
	switch(ctl->command) {
=======
	switch (ctl->command) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	switch (ctl->command) {
>>>>>>> refs/remotes/origin/master
	case ISDN_CMD_IOCTL:
		return pcbit_ioctl(ctl);
		break;
	case ISDN_CMD_DIAL:
		info.type = EV_USR_SETUP_REQ;
		info.data.setup.CalledPN = (char *) &ctl->parm.setup.phone;
		pcbit_fsm_event(dev, chan, EV_USR_SETUP_REQ, &info);
		break;
	case ISDN_CMD_ACCEPTD:
		pcbit_fsm_event(dev, chan, EV_USR_SETUP_RESP, NULL);
		break;
	case ISDN_CMD_ACCEPTB:
		printk("ISDN_CMD_ACCEPTB - not really needed\n");
		break;
	case ISDN_CMD_HANGUP:
		pcbit_fsm_event(dev, chan, EV_USR_RELEASE_REQ, NULL);
		break;
	case ISDN_CMD_SETL2:
		chan->proto = (ctl->arg >> 8);
		break;
	case ISDN_CMD_CLREAZ:
		pcbit_clear_msn(dev);
		break;
	case ISDN_CMD_SETEAZ:
		pcbit_set_msn(dev, ctl->parm.num);
		break;
	case ISDN_CMD_SETL3:
		if ((ctl->arg >> 8) != ISDN_PROTO_L3_TRANS)
			printk(KERN_DEBUG "L3 protocol unknown\n");
		break;
	default:
		printk(KERN_DEBUG "pcbit_command: unknown command\n");
		break;
	};

	return 0;
}

/*
 * Another Hack :-(
 * on some conditions the board stops sending TDATA_CONFs
 * let's see if we can turn around the problem
 */

#ifdef BLOCK_TIMER
static void pcbit_block_timer(unsigned long data)
{
	struct pcbit_chan *chan;
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcbit_dev * dev;
	isdn_ctrl ictl;

	chan = (struct pcbit_chan *) data;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pcbit_dev *dev;
	isdn_ctrl ictl;

	chan = (struct pcbit_chan *)data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dev = chan2dev(chan);

	if (dev == NULL) {
		printk(KERN_DEBUG "pcbit: chan2dev failed\n");
		return;
	}

	del_timer(&chan->block_timer);
	chan->block_timer.function = NULL;

#ifdef DEBUG
	printk(KERN_DEBUG "pcbit_block_timer\n");
<<<<<<< HEAD
<<<<<<< HEAD
#endif	
=======
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif
>>>>>>> refs/remotes/origin/master
	chan->queued = 0;
	ictl.driver = dev->id;
	ictl.command = ISDN_STAT_BSENT;
	ictl.arg = chan->id;
<<<<<<< HEAD
<<<<<<< HEAD
	dev->dev_if->statcallb(&ictl);     
=======
	dev->dev_if->statcallb(&ictl);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->dev_if->statcallb(&ictl);
>>>>>>> refs/remotes/origin/master
}
#endif

static int pcbit_xmit(int driver, int chnum, int ack, struct sk_buff *skb)
{
	ushort hdrlen;
	int refnum, len;
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcbit_chan * chan;
=======
	struct pcbit_chan *chan;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pcbit_chan *chan;
>>>>>>> refs/remotes/origin/master
	struct pcbit_dev *dev;

	dev = finddev(driver);
	if (dev == NULL)
	{
		printk("finddev returned NULL");
		return -1;
	}

	chan = chnum ? dev->b2 : dev->b1;


	if (chan->fsm_state != ST_ACTIVE)
		return -1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (chan->queued >= MAX_QUEUED )
	{
#ifdef DEBUG_QUEUE
		printk(KERN_DEBUG 
=======
=======
>>>>>>> refs/remotes/origin/master
	if (chan->queued >= MAX_QUEUED)
	{
#ifdef DEBUG_QUEUE
		printk(KERN_DEBUG
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		       "pcbit: %d packets already in queue - write fails\n",
		       chan->queued);
#endif
		/*
		 * packet stays on the head of the device queue
		 * since dev_start_xmit will fail
		 * see net/core/dev.c
		 */
#ifdef BLOCK_TIMER
		if (chan->block_timer.function == NULL) {
			init_timer(&chan->block_timer);
			chan->block_timer.function =  &pcbit_block_timer;
			chan->block_timer.data = (long) chan;
			chan->block_timer.expires = jiffies + 1 * HZ;
			add_timer(&chan->block_timer);
		}
<<<<<<< HEAD
<<<<<<< HEAD
#endif		
		return 0;	                 
=======
#endif
		return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif
		return 0;
>>>>>>> refs/remotes/origin/master
	}


	chan->queued++;
<<<<<<< HEAD
<<<<<<< HEAD
	
        len = skb->len;
=======

	len = skb->len;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	len = skb->len;
>>>>>>> refs/remotes/origin/master

	hdrlen = capi_tdata_req(chan, skb);

	refnum = last_ref_num++ & 0x7fffU;
	chan->s_refnum = refnum;

	pcbit_l2_write(dev, MSG_TDATA_REQ, refnum, skb, hdrlen);

	return len;
}

static int pcbit_writecmd(const u_char __user *buf, int len, int driver, int channel)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcbit_dev * dev;
	int i, j;
	const u_char * loadbuf;
	u_char * ptr = NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pcbit_dev *dev;
	int i, j;
	const u_char *loadbuf;
	u_char *ptr = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u_char *cbuf;

	int errstat;

	dev = finddev(driver);

	if (!dev)
	{
		printk("pcbit_writecmd: couldn't find device");
		return -ENODEV;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	switch(dev->l2_state) {
=======
	switch (dev->l2_state) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	switch (dev->l2_state) {
>>>>>>> refs/remotes/origin/master
	case L2_LWMODE:
		/* check (size <= rdp_size); write buf into board */
		if (len < 0 || len > BANK4 + 1 || len > 1024)
		{
			printk("pcbit_writecmd: invalid length %d\n", len);
			return -EINVAL;
		}

		cbuf = memdup_user(buf, len);
		if (IS_ERR(cbuf))
			return PTR_ERR(cbuf);

		memcpy_toio(dev->sh_mem, cbuf, len);
		kfree(cbuf);
		return len;
	case L2_FWMODE:
		/* this is the hard part */
		/* dumb board */
		/* get it into kernel space */
<<<<<<< HEAD
<<<<<<< HEAD
		if ((ptr = kmalloc(len, GFP_KERNEL))==NULL)
=======
		if ((ptr = kmalloc(len, GFP_KERNEL)) == NULL)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if ((ptr = kmalloc(len, GFP_KERNEL)) == NULL)
>>>>>>> refs/remotes/origin/master
			return -ENOMEM;
		if (copy_from_user(ptr, buf, len)) {
			kfree(ptr);
			return -EFAULT;
		}
		loadbuf = ptr;
<<<<<<< HEAD
<<<<<<< HEAD
    
		errstat = 0;

		for (i=0; i < len; i++)
		{
			for(j=0; j < LOAD_RETRY; j++)
=======
=======
>>>>>>> refs/remotes/origin/master

		errstat = 0;

		for (i = 0; i < len; i++)
		{
			for (j = 0; j < LOAD_RETRY; j++)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				if (!(readb(dev->sh_mem + dev->loadptr)))
					break;

			if (j == LOAD_RETRY)
			{
				errstat = -ETIME;
				printk("TIMEOUT i=%d\n", i);
				break;
			}
			writeb(loadbuf[i], dev->sh_mem + dev->loadptr + 1);
			writeb(0x01, dev->sh_mem + dev->loadptr);

			dev->loadptr += 2;
			if (dev->loadptr > LOAD_ZONE_END)
				dev->loadptr = LOAD_ZONE_START;
		}
		kfree(ptr);

		return errstat ? errstat : len;
	default:
		return -EBUSY;
	}
}

/*
 *  demultiplexing of messages
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
void pcbit_l3_receive(struct pcbit_dev * dev, ulong msg, 
			     struct sk_buff * skb,
			     ushort hdr_len, ushort refnum)
=======
void pcbit_l3_receive(struct pcbit_dev *dev, ulong msg,
		      struct sk_buff *skb,
		      ushort hdr_len, ushort refnum)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void pcbit_l3_receive(struct pcbit_dev *dev, ulong msg,
		      struct sk_buff *skb,
		      ushort hdr_len, ushort refnum)
>>>>>>> refs/remotes/origin/master
{
	struct pcbit_chan *chan;
	struct sk_buff *skb2;
	unsigned short len;
	struct callb_data cbdata;
	int complete, err;
	isdn_ctrl ictl;

<<<<<<< HEAD
<<<<<<< HEAD
	switch(msg) {

	case MSG_TDATA_IND:
		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING 
=======
=======
>>>>>>> refs/remotes/origin/master
	switch (msg) {

	case MSG_TDATA_IND:
		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}
		chan->r_refnum = skb->data[7];
		skb_pull(skb, 8);

		dev->dev_if->rcvcallb_skb(dev->id, chan->id, skb);

<<<<<<< HEAD
<<<<<<< HEAD
		if (capi_tdata_resp(chan, &skb2) > 0) 
			pcbit_l2_write(dev, MSG_TDATA_RESP, refnum, 
				       skb2, skb2->len);
		return;
		break;  
	case MSG_TDATA_CONF:
		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING 
=======
=======
>>>>>>> refs/remotes/origin/master
		if (capi_tdata_resp(chan, &skb2) > 0)
			pcbit_l2_write(dev, MSG_TDATA_RESP, refnum,
				       skb2, skb2->len);
		return;
		break;
	case MSG_TDATA_CONF:
		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

#ifdef DEBUG
<<<<<<< HEAD
<<<<<<< HEAD
		if ( (*((ushort *) (skb->data + 2) )) != 0) {
                        printk(KERN_DEBUG "TDATA_CONF error\n");
		}
#endif
#ifdef BLOCK_TIMER
                if (chan->queued == MAX_QUEUED) {
                        del_timer(&chan->block_timer);
			chan->block_timer.function = NULL;
		}
                
#endif		
=======
=======
>>>>>>> refs/remotes/origin/master
		if ((*((ushort *)(skb->data + 2))) != 0) {
			printk(KERN_DEBUG "TDATA_CONF error\n");
		}
#endif
#ifdef BLOCK_TIMER
		if (chan->queued == MAX_QUEUED) {
			del_timer(&chan->block_timer);
			chan->block_timer.function = NULL;
		}

#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		chan->queued--;

		ictl.driver = dev->id;
		ictl.command = ISDN_STAT_BSENT;
		ictl.arg = chan->id;
		dev->dev_if->statcallb(&ictl);
		break;

	case MSG_CONN_IND:
		/*
		 *  channel: 1st not used will do
<<<<<<< HEAD
<<<<<<< HEAD
		 *           if both are used we're in trouble 
=======
		 *           if both are used we're in trouble
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 *           if both are used we're in trouble
>>>>>>> refs/remotes/origin/master
		 */

		if (!dev->b1->fsm_state)
			chan = dev->b1;
		else if (!dev->b2->fsm_state)
			chan = dev->b2;
		else {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO 
			       "Incoming connection: no channels available");

			if ((len = capi_disc_req(*(ushort*)(skb->data), &skb2, CAUSE_NOCHAN)) > 0)
				pcbit_l2_write(dev, MSG_DISC_REQ, refnum, skb2, len);
			break;  
=======
=======
>>>>>>> refs/remotes/origin/master
			printk(KERN_INFO
			       "Incoming connection: no channels available");

			if ((len = capi_disc_req(*(ushort *)(skb->data), &skb2, CAUSE_NOCHAN)) > 0)
				pcbit_l2_write(dev, MSG_DISC_REQ, refnum, skb2, len);
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

		cbdata.data.setup.CalledPN = NULL;
		cbdata.data.setup.CallingPN = NULL;

		capi_decode_conn_ind(chan, skb, &cbdata);
		cbdata.type = EV_NET_SETUP;

		pcbit_fsm_event(dev, chan, EV_NET_SETUP, NULL);

<<<<<<< HEAD
<<<<<<< HEAD
		if (pcbit_check_msn(dev, cbdata.data.setup.CallingPN)) 
=======
		if (pcbit_check_msn(dev, cbdata.data.setup.CallingPN))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (pcbit_check_msn(dev, cbdata.data.setup.CallingPN))
>>>>>>> refs/remotes/origin/master
			pcbit_fsm_event(dev, chan, EV_USR_PROCED_REQ, &cbdata);
		else
			pcbit_fsm_event(dev, chan, EV_USR_RELEASE_REQ, NULL);

		kfree(cbdata.data.setup.CalledPN);
		kfree(cbdata.data.setup.CallingPN);
		break;
<<<<<<< HEAD
<<<<<<< HEAD
    
	case MSG_CONN_CONF:
		/* 
=======

	case MSG_CONN_CONF:
		/*
>>>>>>> refs/remotes/origin/cm-10.0
=======

	case MSG_CONN_CONF:
		/*
>>>>>>> refs/remotes/origin/master
		 * We should be able to find the channel by the message
		 * reference number. The current version of the firmware
		 * doesn't sent the ref number correctly.
		 */
#ifdef DEBUG
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_DEBUG "refnum=%04x b1=%04x b2=%04x\n", refnum, 
		       dev->b1->s_refnum, 
=======
		printk(KERN_DEBUG "refnum=%04x b1=%04x b2=%04x\n", refnum,
		       dev->b1->s_refnum,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		printk(KERN_DEBUG "refnum=%04x b1=%04x b2=%04x\n", refnum,
		       dev->b1->s_refnum,
>>>>>>> refs/remotes/origin/master
		       dev->b2->s_refnum);
#endif
		/* We just try to find a channel in the right state */

		if (dev->b1->fsm_state == ST_CALL_INIT)
			chan = dev->b1;
<<<<<<< HEAD
<<<<<<< HEAD
		else { 		   
			if (dev->b2->s_refnum == ST_CALL_INIT)
				chan = dev->b2;
			else {			
=======
=======
>>>>>>> refs/remotes/origin/master
		else {
			if (dev->b2->s_refnum == ST_CALL_INIT)
				chan = dev->b2;
			else {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				chan = NULL;
				printk(KERN_WARNING "Connection Confirm - no channel in Call Init state\n");
				break;
			}
		}
		if (capi_decode_conn_conf(chan, skb, &complete)) {
			printk(KERN_DEBUG "conn_conf indicates error\n");
			pcbit_fsm_event(dev, chan, EV_ERROR, NULL);
		}
		else
			if (complete)
				pcbit_fsm_event(dev, chan, EV_NET_CALL_PROC, NULL);
			else
				pcbit_fsm_event(dev, chan, EV_NET_SETUP_ACK, NULL);
<<<<<<< HEAD
<<<<<<< HEAD
		break; 
	case MSG_CONN_ACTV_IND:

		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING 
			       "CAPI header: unknown channel id\n");
			break;
		}
		
		if (capi_decode_conn_actv_ind(chan, skb)) {
			printk("error in capi_decode_conn_actv_ind\n");
		     /* pcbit_fsm_event(dev, chan, EV_ERROR, NULL); */
=======
=======
>>>>>>> refs/remotes/origin/master
		break;
	case MSG_CONN_ACTV_IND:

		if (!(chan = capi_channel(dev, skb))) {
			printk(KERN_WARNING
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (capi_decode_conn_actv_ind(chan, skb)) {
			printk("error in capi_decode_conn_actv_ind\n");
			/* pcbit_fsm_event(dev, chan, EV_ERROR, NULL); */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
		}
		chan->r_refnum = refnum;
		pcbit_fsm_event(dev, chan, EV_NET_CONN, NULL);
		break;
	case MSG_CONN_ACTV_CONF:

		if (!(chan = capi_channel(dev, skb))) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING 
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (capi_decode_conn_actv_conf(chan, skb) == 0)
			pcbit_fsm_event(dev, chan, EV_NET_CONN_ACK, NULL);
<<<<<<< HEAD
<<<<<<< HEAD
		
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
		else
			printk(KERN_DEBUG "decode_conn_actv_conf failed\n");
		break;

	case  MSG_SELP_CONF:

		if (!(chan = capi_channel(dev, skb))) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING 
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (!(err = capi_decode_sel_proto_conf(chan, skb)))
			pcbit_fsm_event(dev, chan, EV_NET_SELP_RESP, NULL);
		else {
			/* Error */
			printk("error %d - capi_decode_sel_proto_conf\n", err);
		}
		break;
	case MSG_ACT_TRANSP_CONF:
		if (!(chan = capi_channel(dev, skb))) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING 
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (!capi_decode_actv_trans_conf(chan, skb))
			pcbit_fsm_event(dev, chan, EV_NET_ACTV_RESP, NULL);
		break;

	case MSG_DISC_IND:

		if (!(chan = capi_channel(dev, skb))) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING 
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (!capi_decode_disc_ind(chan, skb))
			pcbit_fsm_event(dev, chan, EV_NET_DISC, NULL);
		else
			printk(KERN_WARNING "capi_decode_disc_ind - error\n");
		break;
	case MSG_DISC_CONF:
		if (!(chan = capi_channel(dev, skb))) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING 
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_WARNING
>>>>>>> refs/remotes/origin/master
			       "CAPI header: unknown channel id\n");
			break;
		}

		if (!capi_decode_disc_ind(chan, skb))
			pcbit_fsm_event(dev, chan, EV_NET_RELEASE, NULL);
		else
			printk(KERN_WARNING "capi_decode_disc_conf - error\n");
		break;
	case MSG_INFO_IND:
#ifdef DEBUG
		printk(KERN_DEBUG "received Info Indication - discarded\n");
#endif
		break;
#ifdef DEBUG
	case MSG_DEBUG_188:
		capi_decode_debug_188(skb->data, skb->len);
		break;

	default:
		printk(KERN_DEBUG "pcbit_l3_receive: unknown message %08lx\n",
		       msg);
		break;
#endif
	}

	kfree_skb(skb);

}

/*
 *   Single statbuf
 *   should be a statbuf per device
 */

static char statbuf[STATBUF_LEN];
static int stat_st = 0;
static int stat_end = 0;

static int pcbit_stat(u_char __user *buf, int len, int driver, int channel)
{
	int stat_count;
	stat_count = stat_end - stat_st;

	if (stat_count < 0)
		stat_count = STATBUF_LEN - stat_st + stat_end;

	/* FIXME: should we sleep and wait for more cookies ? */
<<<<<<< HEAD
<<<<<<< HEAD
	if (len > stat_count)            
=======
	if (len > stat_count)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (len > stat_count)
>>>>>>> refs/remotes/origin/master
		len = stat_count;

	if (stat_st < stat_end)
	{
		if (copy_to_user(buf, statbuf + stat_st, len))
			return -EFAULT;
<<<<<<< HEAD
<<<<<<< HEAD
		stat_st += len;	   
=======
		stat_st += len;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		stat_st += len;
>>>>>>> refs/remotes/origin/master
	}
	else
	{
		if (len > STATBUF_LEN - stat_st)
		{
			if (copy_to_user(buf, statbuf + stat_st,
<<<<<<< HEAD
<<<<<<< HEAD
				       STATBUF_LEN - stat_st))
				return -EFAULT;
			if (copy_to_user(buf, statbuf,
				       len - (STATBUF_LEN - stat_st)))
=======
=======
>>>>>>> refs/remotes/origin/master
					 STATBUF_LEN - stat_st))
				return -EFAULT;
			if (copy_to_user(buf, statbuf,
					 len - (STATBUF_LEN - stat_st)))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				return -EFAULT;

			stat_st = len - (STATBUF_LEN - stat_st);
		}
		else
		{
			if (copy_to_user(buf, statbuf + stat_st, len))
				return -EFAULT;

			stat_st += len;
<<<<<<< HEAD
<<<<<<< HEAD
			
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
			if (stat_st == STATBUF_LEN)
				stat_st = 0;
		}
	}

	if (stat_st == stat_end)
		stat_st = stat_end = 0;

	return len;
}

static void pcbit_logstat(struct pcbit_dev *dev, char *str)
{
	int i;
	isdn_ctrl ictl;

<<<<<<< HEAD
<<<<<<< HEAD
	for (i=stat_end; i<strlen(str); i++)
	{
		statbuf[i]=str[i];
=======
	for (i = stat_end; i < strlen(str); i++)
	{
		statbuf[i] = str[i];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = stat_end; i < strlen(str); i++)
	{
		statbuf[i] = str[i];
>>>>>>> refs/remotes/origin/master
		stat_end = (stat_end + 1) % STATBUF_LEN;
		if (stat_end == stat_st)
			stat_st = (stat_st + 1) % STATBUF_LEN;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ictl.command=ISDN_STAT_STAVAIL;
	ictl.driver=dev->id;
	ictl.arg=strlen(str);
	dev->dev_if->statcallb(&ictl);
}
	
void pcbit_state_change(struct pcbit_dev * dev, struct pcbit_chan * chan, 
			unsigned short i, unsigned short ev, unsigned short f)
{
	char buf[256];
  
	sprintf(buf, "change on device: %d channel:%d\n%s -> %s -> %s\n",
		dev->id, chan->id, 
=======
=======
>>>>>>> refs/remotes/origin/master
	ictl.command = ISDN_STAT_STAVAIL;
	ictl.driver = dev->id;
	ictl.arg = strlen(str);
	dev->dev_if->statcallb(&ictl);
}

void pcbit_state_change(struct pcbit_dev *dev, struct pcbit_chan *chan,
			unsigned short i, unsigned short ev, unsigned short f)
{
	char buf[256];

	sprintf(buf, "change on device: %d channel:%d\n%s -> %s -> %s\n",
		dev->id, chan->id,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		isdn_state_table[i], strisdnevent(ev), isdn_state_table[f]
		);

#ifdef DEBUG
	printk("%s", buf);
#endif

	pcbit_logstat(dev, buf);
}

static void set_running_timeout(unsigned long ptr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcbit_dev * dev;
=======
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pcbit_dev *dev;
>>>>>>> refs/remotes/origin/master

#ifdef DEBUG
	printk(KERN_DEBUG "set_running_timeout\n");
#endif
	dev = (struct pcbit_dev *) ptr;

	wake_up_interruptible(&dev->set_running_wq);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int set_protocol_running(struct pcbit_dev * dev)
=======
static int set_protocol_running(struct pcbit_dev *dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int set_protocol_running(struct pcbit_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	isdn_ctrl ctl;

	init_timer(&dev->set_running_timer);

	dev->set_running_timer.function = &set_running_timeout;
	dev->set_running_timer.data = (ulong) dev;
	dev->set_running_timer.expires = jiffies + SET_RUN_TIMEOUT;

	/* kick it */

	dev->l2_state = L2_STARTING;

<<<<<<< HEAD
<<<<<<< HEAD
	writeb((0x80U | ((dev->rcv_seq & 0x07) << 3) | (dev->send_seq & 0x07)), 
=======
	writeb((0x80U | ((dev->rcv_seq & 0x07) << 3) | (dev->send_seq & 0x07)),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	writeb((0x80U | ((dev->rcv_seq & 0x07) << 3) | (dev->send_seq & 0x07)),
>>>>>>> refs/remotes/origin/master
	       dev->sh_mem + BANK4);

	add_timer(&dev->set_running_timer);

	interruptible_sleep_on(&dev->set_running_wq);

	del_timer(&dev->set_running_timer);

	if (dev->l2_state == L2_RUNNING)
	{
		printk(KERN_DEBUG "pcbit: running\n");

		dev->unack_seq = dev->send_seq;

		dev->writeptr = dev->sh_mem;
		dev->readptr = dev->sh_mem + BANK2;
<<<<<<< HEAD
<<<<<<< HEAD
    
		/* tell the good news to the upper layer */  
=======

		/* tell the good news to the upper layer */
>>>>>>> refs/remotes/origin/cm-10.0
=======

		/* tell the good news to the upper layer */
>>>>>>> refs/remotes/origin/master
		ctl.driver = dev->id;
		ctl.command = ISDN_STAT_RUN;

		dev->dev_if->statcallb(&ctl);
	}
	else
	{
		printk(KERN_DEBUG "pcbit: initialization failed\n");
		printk(KERN_DEBUG "pcbit: firmware not loaded\n");

		dev->l2_state = L2_DOWN;

#ifdef DEBUG
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_DEBUG "Bank3 = %02x\n", 
=======
		printk(KERN_DEBUG "Bank3 = %02x\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		printk(KERN_DEBUG "Bank3 = %02x\n",
>>>>>>> refs/remotes/origin/master
		       readb(dev->sh_mem + BANK3));
#endif
		writeb(0x40, dev->sh_mem + BANK4);

		/* warn the upper layer */
		ctl.driver = dev->id;
		ctl.command = ISDN_STAT_STOP;

		dev->dev_if->statcallb(&ctl);

		return -EL2HLT;	/* Level 2 halted */
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int pcbit_ioctl(isdn_ctrl* ctl)
{
	struct pcbit_dev * dev;
	struct pcbit_ioctl *cmd;

	dev = finddev(ctl->driver);
  
=======
=======
>>>>>>> refs/remotes/origin/master
static int pcbit_ioctl(isdn_ctrl *ctl)
{
	struct pcbit_dev *dev;
	struct pcbit_ioctl *cmd;

	dev = finddev(ctl->driver);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!dev)
	{
		printk(KERN_DEBUG "pcbit_ioctl: unknown device\n");
		return -ENODEV;
	}

	cmd = (struct pcbit_ioctl *) ctl->parm.num;

<<<<<<< HEAD
<<<<<<< HEAD
	switch(ctl->arg) {
=======
	switch (ctl->arg) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	switch (ctl->arg) {
>>>>>>> refs/remotes/origin/master
	case PCBIT_IOCTL_GETSTAT:
		cmd->info.l2_status = dev->l2_state;
		break;

	case PCBIT_IOCTL_STRLOAD:
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;

		dev->unack_seq = dev->send_seq = dev->rcv_seq = 0;

		dev->writeptr = dev->sh_mem;
		dev->readptr = dev->sh_mem + BANK2;
<<<<<<< HEAD
<<<<<<< HEAD
    
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
		dev->l2_state = L2_LOADING;
		break;

	case PCBIT_IOCTL_LWMODE:
		if (dev->l2_state != L2_LOADING)
			return -EINVAL;

		dev->l2_state = L2_LWMODE;
		break;

	case PCBIT_IOCTL_FWMODE:
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;
		dev->loadptr = LOAD_ZONE_START;
		dev->l2_state = L2_FWMODE;

<<<<<<< HEAD
<<<<<<< HEAD
		break; 
=======
		break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		break;
>>>>>>> refs/remotes/origin/master
	case PCBIT_IOCTL_ENDLOAD:
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;
		dev->l2_state = L2_DOWN;
<<<<<<< HEAD
<<<<<<< HEAD
		break; 

	case PCBIT_IOCTL_SETBYTE: 
=======
		break;

	case PCBIT_IOCTL_SETBYTE:
>>>>>>> refs/remotes/origin/cm-10.0
=======
		break;

	case PCBIT_IOCTL_SETBYTE:
>>>>>>> refs/remotes/origin/master
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;

		/* check addr */
		if (cmd->info.rdp_byte.addr > BANK4)
			return -EFAULT;
<<<<<<< HEAD
<<<<<<< HEAD
		
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
		writeb(cmd->info.rdp_byte.value, dev->sh_mem + cmd->info.rdp_byte.addr);
		break;
	case PCBIT_IOCTL_GETBYTE:
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;

		/* check addr */

		if (cmd->info.rdp_byte.addr > BANK4)
		{
			printk("getbyte: invalid addr %04x\n", cmd->info.rdp_byte.addr);
			return -EFAULT;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		
		cmd->info.rdp_byte.value = readb(dev->sh_mem + cmd->info.rdp_byte.addr); 
		break;
	case PCBIT_IOCTL_RUNNING: 
=======
=======
>>>>>>> refs/remotes/origin/master

		cmd->info.rdp_byte.value = readb(dev->sh_mem + cmd->info.rdp_byte.addr);
		break;
	case PCBIT_IOCTL_RUNNING:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (dev->l2_state == L2_RUNNING)
			return -EBUSY;
		return set_protocol_running(dev);
		break;
	case PCBIT_IOCTL_WATCH188:
		if (dev->l2_state != L2_LOADING)
			return -EINVAL;
		pcbit_l2_write(dev, MSG_WATCH188, 0x0001, NULL, 0);
		break;
	case PCBIT_IOCTL_PING188:
		if (dev->l2_state != L2_LOADING)
			return -EINVAL;
		pcbit_l2_write(dev, MSG_PING188_REQ, 0x0001, NULL, 0);
		break;
	case PCBIT_IOCTL_APION:
		if (dev->l2_state != L2_LOADING)
			return -EINVAL;
		pcbit_l2_write(dev, MSG_API_ON, 0x0001, NULL, 0);
		break;
	case PCBIT_IOCTL_STOP:
		dev->l2_state = L2_DOWN;
		writeb(0x40, dev->sh_mem + BANK4);
		dev->rcv_seq = 0;
		dev->send_seq = 0;
		dev->unack_seq = 0;
		break;
	default:
		printk("error: unknown ioctl\n");
		break;
	};
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* 
 *        MSN list handling
 *
 *        if null reject all calls
 *        if first entry has null MSN accept all calls 
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 *        MSN list handling
 *
 *        if null reject all calls
 *        if first entry has null MSN accept all calls
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */

static void pcbit_clear_msn(struct pcbit_dev *dev)
{
	struct msn_entry *ptr, *back;

<<<<<<< HEAD
<<<<<<< HEAD
	for (ptr=dev->msn_list; ptr; )
=======
	for (ptr = dev->msn_list; ptr;)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (ptr = dev->msn_list; ptr;)
>>>>>>> refs/remotes/origin/master
	{
		back = ptr->next;
		kfree(ptr);
		ptr = back;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	dev->msn_list = NULL; 
=======
	dev->msn_list = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev->msn_list = NULL;
>>>>>>> refs/remotes/origin/master
}

static void pcbit_set_msn(struct pcbit_dev *dev, char *list)
{
	struct msn_entry *ptr;
	struct msn_entry *back = NULL;
	char *cp, *sp;
	int len;

	if (strlen(list) == 0) {
		ptr = kmalloc(sizeof(struct msn_entry), GFP_ATOMIC);
		if (!ptr) {
			printk(KERN_WARNING "kmalloc failed\n");
			return;
		}

		ptr->msn = NULL;

		ptr->next = dev->msn_list;
		dev->msn_list = ptr;

		return;
	}

	if (dev->msn_list)
<<<<<<< HEAD
<<<<<<< HEAD
		for (back=dev->msn_list; back->next; back=back->next);
	
	sp = list;

	do {
		cp=strchr(sp, ',');
=======
=======
>>>>>>> refs/remotes/origin/master
		for (back = dev->msn_list; back->next; back = back->next);

	sp = list;

	do {
		cp = strchr(sp, ',');
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (cp)
			len = cp - sp;
		else
			len = strlen(sp);

		ptr = kmalloc(sizeof(struct msn_entry), GFP_ATOMIC);

		if (!ptr) {
			printk(KERN_WARNING "kmalloc failed\n");
			return;
		}
		ptr->next = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
		ptr->msn = kmalloc(len, GFP_ATOMIC);
		if (!ptr->msn) {
			printk(KERN_WARNING "kmalloc failed\n");
			kfree(ptr);
			return;
		}

		memcpy(ptr->msn, sp, len - 1);
		ptr->msn[len] = 0;

#ifdef DEBUG
		printk(KERN_DEBUG "msn: %s\n", ptr->msn);
#endif
		if (dev->msn_list == NULL)
			dev->msn_list = ptr;
		else
			back->next = ptr;
		back = ptr;
		sp += len;
<<<<<<< HEAD
<<<<<<< HEAD
	} while(cp);
=======
	} while (cp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} while (cp);
>>>>>>> refs/remotes/origin/master
}

/*
 *  check if we do signal or reject an incoming call
 */
static int pcbit_check_msn(struct pcbit_dev *dev, char *msn)
{
	struct msn_entry *ptr;
<<<<<<< HEAD
<<<<<<< HEAD
	
	for (ptr=dev->msn_list; ptr; ptr=ptr->next) {

		if (ptr->msn == NULL) 
			return 1;
		
=======
=======
>>>>>>> refs/remotes/origin/master

	for (ptr = dev->msn_list; ptr; ptr = ptr->next) {

		if (ptr->msn == NULL)
			return 1;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (strcmp(ptr->msn, msn) == 0)
			return 1;
	}

	return 0;
}
