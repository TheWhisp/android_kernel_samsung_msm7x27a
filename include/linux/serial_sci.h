#ifndef __LINUX_SERIAL_SCI_H
#define __LINUX_SERIAL_SCI_H

#include <linux/serial_core.h>
#include <linux/sh_dma.h>

/*
<<<<<<< HEAD
 * Generic header for SuperH SCI(F) (used by sh/sh64/h8300 and related parts)
 */

<<<<<<< HEAD
=======
#define SCIx_NOT_SUPPORTED	(-1)

>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Generic header for SuperH (H)SCI(F) (used by sh/sh64 and related parts)
 */

#define SCIx_NOT_SUPPORTED	(-1)

>>>>>>> refs/remotes/origin/master
enum {
	SCBRR_ALGO_1,		/* ((clk + 16 * bps) / (16 * bps) - 1) */
	SCBRR_ALGO_2,		/* ((clk + 16 * bps) / (32 * bps) - 1) */
	SCBRR_ALGO_3,		/* (((clk * 2) + 16 * bps) / (16 * bps) - 1) */
	SCBRR_ALGO_4,		/* (((clk * 2) + 16 * bps) / (32 * bps) - 1) */
	SCBRR_ALGO_5,		/* (((clk * 1000 / 32) / bps) - 1) */
<<<<<<< HEAD
=======
	SCBRR_ALGO_6,		/* HSCIF variable sample rate algorithm */
>>>>>>> refs/remotes/origin/master
};

#define SCSCR_TIE	(1 << 7)
#define SCSCR_RIE	(1 << 6)
#define SCSCR_TE	(1 << 5)
#define SCSCR_RE	(1 << 4)
#define SCSCR_REIE	(1 << 3)	/* not supported by all parts */
#define SCSCR_TOIE	(1 << 2)	/* not supported by all parts */
#define SCSCR_CKE1	(1 << 1)
#define SCSCR_CKE0	(1 << 0)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* SCxSR SCI */
#define SCI_TDRE  0x80
#define SCI_RDRF  0x40
#define SCI_ORER  0x20
#define SCI_FER   0x10
#define SCI_PER   0x08
#define SCI_TEND  0x04

#define SCI_DEFAULT_ERROR_MASK (SCI_PER | SCI_FER)

<<<<<<< HEAD
/* SCxSR SCIF */
=======
/* SCxSR SCIF, HSCIF */
>>>>>>> refs/remotes/origin/master
#define SCIF_ER    0x0080
#define SCIF_TEND  0x0040
#define SCIF_TDFE  0x0020
#define SCIF_BRK   0x0010
#define SCIF_FER   0x0008
#define SCIF_PER   0x0004
#define SCIF_RDF   0x0002
#define SCIF_DR    0x0001

#define SCIF_DEFAULT_ERROR_MASK (SCIF_PER | SCIF_FER | SCIF_ER | SCIF_BRK)

/* SCSPTR, optional */
#define SCSPTR_RTSIO	(1 << 7)
#define SCSPTR_CTSIO	(1 << 5)
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define SCSPTR_SPB2IO	(1 << 1)
#define SCSPTR_SPB2DT	(1 << 0)

/* HSSRR HSCIF */
#define HSCIF_SRE	0x8000

>>>>>>> refs/remotes/origin/master
/* Offsets into the sci_port->irqs array */
enum {
	SCIx_ERI_IRQ,
	SCIx_RXI_IRQ,
	SCIx_TXI_IRQ,
	SCIx_BRI_IRQ,
	SCIx_NR_IRQS,
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

	SCIx_MUX_IRQ = SCIx_NR_IRQS,	/* special case */
};

/* Offsets into the sci_port->gpios array */
enum {
	SCIx_SCK,
	SCIx_RXD,
	SCIx_TXD,
	SCIx_CTS,
	SCIx_RTS,

	SCIx_NR_FNS,
};

enum {
	SCIx_PROBE_REGTYPE,

	SCIx_SCI_REGTYPE,
	SCIx_IRDA_REGTYPE,
	SCIx_SCIFA_REGTYPE,
	SCIx_SCIFB_REGTYPE,
	SCIx_SH2_SCIF_FIFODATA_REGTYPE,
	SCIx_SH3_SCIF_REGTYPE,
	SCIx_SH4_SCIF_REGTYPE,
	SCIx_SH4_SCIF_NO_SCSPTR_REGTYPE,
	SCIx_SH4_SCIF_FIFODATA_REGTYPE,
	SCIx_SH7705_SCIF_REGTYPE,
<<<<<<< HEAD

	SCIx_NR_REGTYPES,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	SCIx_HSCIF_REGTYPE,

	SCIx_NR_REGTYPES,
>>>>>>> refs/remotes/origin/master
};

#define SCIx_IRQ_MUXED(irq)		\
{					\
	[SCIx_ERI_IRQ]	= (irq),	\
	[SCIx_RXI_IRQ]	= (irq),	\
	[SCIx_TXI_IRQ]	= (irq),	\
	[SCIx_BRI_IRQ]	= (irq),	\
}

<<<<<<< HEAD
<<<<<<< HEAD
struct device;

=======
=======
>>>>>>> refs/remotes/origin/master
#define SCIx_IRQ_IS_MUXED(port)			\
	((port)->cfg->irqs[SCIx_ERI_IRQ] ==	\
	 (port)->cfg->irqs[SCIx_RXI_IRQ]) ||	\
	((port)->cfg->irqs[SCIx_ERI_IRQ] &&	\
	 !(port)->cfg->irqs[SCIx_RXI_IRQ])
/*
 * SCI register subset common for all port types.
 * Not all registers will exist on all parts.
 */
enum {
	SCSMR, SCBRR, SCSCR, SCxSR,
	SCFCR, SCFDR, SCxTDR, SCxRDR,
	SCLSR, SCTFDR, SCRFDR, SCSPTR,
<<<<<<< HEAD
=======
	HSSRR,
>>>>>>> refs/remotes/origin/master

	SCIx_NR_REGS,
};

struct device;

struct plat_sci_port_ops {
	void (*init_pins)(struct uart_port *, unsigned int cflag);
};

/*
 * Port-specific capabilities
 */
#define SCIx_HAVE_RTSCTS	(1 << 0)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Platform device specific platform_data struct
 */
struct plat_sci_port {
	unsigned long	mapbase;		/* resource base */
	unsigned int	irqs[SCIx_NR_IRQS];	/* ERI, RXI, TXI, BRI */
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int	type;			/* SCI / SCIF / IRDA */
	upf_t		flags;			/* UPF_* flags */
=======
	unsigned int	gpios[SCIx_NR_FNS];	/* SCK, RXD, TXD, CTS, RTS */
	unsigned int	type;			/* SCI / SCIF / IRDA */
	upf_t		flags;			/* UPF_* flags */
	unsigned long	capabilities;		/* Port features/capabilities */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int	gpios[SCIx_NR_FNS];	/* SCK, RXD, TXD, CTS, RTS */
	unsigned int	type;			/* SCI / SCIF / IRDA / HSCIF */
	upf_t		flags;			/* UPF_* flags */
	unsigned long	capabilities;		/* Port features/capabilities */
>>>>>>> refs/remotes/origin/master

	unsigned int	scbrr_algo_id;		/* SCBRR calculation algo */
	unsigned int	scscr;			/* SCSCR initialization */

<<<<<<< HEAD
<<<<<<< HEAD
	struct device	*dma_dev;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Platform overrides if necessary, defaults otherwise.
	 */
	int		overrun_bit;
	unsigned int	error_mask;

	int		port_reg;
	unsigned char	regshift;
	unsigned char	regtype;

	struct plat_sci_port_ops	*ops;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	unsigned int	dma_slave_tx;
	unsigned int	dma_slave_rx;
};

#endif /* __LINUX_SERIAL_SCI_H */
