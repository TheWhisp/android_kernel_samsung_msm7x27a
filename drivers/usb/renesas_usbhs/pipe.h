/*
 * Renesas USB driver
 *
 * Copyright (C) 2011 Renesas Solutions Corp.
 * Kuninori Morimoto <kuninori.morimoto.gx@renesas.com>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef RENESAS_USB_PIPE_H
#define RENESAS_USB_PIPE_H

<<<<<<< HEAD
#include "./common.h"
<<<<<<< HEAD
=======
#include "./fifo.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "common.h"
#include "fifo.h"
>>>>>>> refs/remotes/origin/master

/*
 *	struct
 */
struct usbhs_pipe {
	u32 pipe_type;	/* USB_ENDPOINT_XFER_xxx */

	struct usbhs_priv *priv;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	struct usbhs_fifo *fifo;
	struct list_head list;

	int maxp;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	u32 flags;
#define USBHS_PIPE_FLAGS_IS_USED		(1 << 0)
#define USBHS_PIPE_FLAGS_IS_DIR_IN		(1 << 1)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define USBHS_PIPE_FLAGS_IS_DIR_HOST		(1 << 2)

	struct usbhs_pkt_handle *handler;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define USBHS_PIPE_FLAGS_IS_DIR_HOST		(1 << 2)

	struct usbhs_pkt_handle *handler;
>>>>>>> refs/remotes/origin/master

	void *mod_private;
};

struct usbhs_pipe_info {
	struct usbhs_pipe *pipe;
	int size;	/* array size of "pipe" */
	int bufnmb_last;	/* FIXME : driver needs good allocator */
<<<<<<< HEAD
<<<<<<< HEAD
=======

	int (*dma_map_ctrl)(struct usbhs_pkt *pkt, int map);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	int (*dma_map_ctrl)(struct usbhs_pkt *pkt, int map);
>>>>>>> refs/remotes/origin/master
};

/*
 * pipe list
 */
#define __usbhs_for_each_pipe(start, pos, info, i)	\
<<<<<<< HEAD
	for (i = start, pos = (info)->pipe;		\
	     i < (info)->size;				\
	     i++, pos = (info)->pipe + i)
=======
	for ((i) = start;						\
	     ((i) < (info)->size) && ((pos) = (info)->pipe + (i));	\
	     (i)++)
>>>>>>> refs/remotes/origin/master

#define usbhs_for_each_pipe(pos, priv, i)			\
	__usbhs_for_each_pipe(1, pos, &((priv)->pipe_info), i)

#define usbhs_for_each_pipe_with_dcp(pos, priv, i)		\
	__usbhs_for_each_pipe(0, pos, &((priv)->pipe_info), i)

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * pipe module probe / remove
 */
int usbhs_pipe_probe(struct usbhs_priv *priv);
void usbhs_pipe_remove(struct usbhs_priv *priv);

/*
 * cfifo
 */
int usbhs_fifo_write(struct usbhs_pipe *pipe, u8 *buf, int len);
int usbhs_fifo_read(struct usbhs_pipe *pipe, u8 *buf, int len);
int usbhs_fifo_prepare_write(struct usbhs_pipe *pipe);
int usbhs_fifo_prepare_read(struct usbhs_pipe *pipe);

void usbhs_fifo_enable(struct usbhs_pipe *pipe);
void usbhs_fifo_disable(struct usbhs_pipe *pipe);
void usbhs_fifo_stall(struct usbhs_pipe *pipe);

void usbhs_fifo_send_terminator(struct usbhs_pipe *pipe);


/*
 * usb request
 */
void usbhs_usbreq_get_val(struct usbhs_priv *priv, struct usb_ctrlrequest *req);
void usbhs_usbreq_set_val(struct usbhs_priv *priv, struct usb_ctrlrequest *req);
=======
 * data
 */
#define usbhs_priv_to_pipeinfo(pr)	(&(pr)->pipe_info)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * data
 */
#define usbhs_priv_to_pipeinfo(pr)	(&(pr)->pipe_info)
>>>>>>> refs/remotes/origin/master

/*
 * pipe control
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct usbhs_pipe
*usbhs_pipe_malloc(struct usbhs_priv *priv,
		   const struct usb_endpoint_descriptor *desc);

int usbhs_pipe_is_dir_in(struct usbhs_pipe *pipe);
void usbhs_pipe_init(struct usbhs_priv *priv);
int usbhs_pipe_get_maxpacket(struct usbhs_pipe *pipe);
void usbhs_pipe_clear_sequence(struct usbhs_pipe *pipe);

#define usbhs_pipe_number(p)	(int)((p) - (p)->priv->pipe_info.pipe)
=======
=======
>>>>>>> refs/remotes/origin/master
char *usbhs_pipe_name(struct usbhs_pipe *pipe);
struct usbhs_pipe
*usbhs_pipe_malloc(struct usbhs_priv *priv, int endpoint_type, int dir_in);
int usbhs_pipe_probe(struct usbhs_priv *priv);
void usbhs_pipe_remove(struct usbhs_priv *priv);
int usbhs_pipe_is_dir_in(struct usbhs_pipe *pipe);
int usbhs_pipe_is_dir_host(struct usbhs_pipe *pipe);
void usbhs_pipe_init(struct usbhs_priv *priv,
		     int (*dma_map_ctrl)(struct usbhs_pkt *pkt, int map));
int usbhs_pipe_get_maxpacket(struct usbhs_pipe *pipe);
void usbhs_pipe_clear(struct usbhs_pipe *pipe);
int usbhs_pipe_is_accessible(struct usbhs_pipe *pipe);
void usbhs_pipe_enable(struct usbhs_pipe *pipe);
void usbhs_pipe_disable(struct usbhs_pipe *pipe);
void usbhs_pipe_stall(struct usbhs_pipe *pipe);
int usbhs_pipe_is_stall(struct usbhs_pipe *pipe);
<<<<<<< HEAD
=======
void usbhs_pipe_set_trans_count_if_bulk(struct usbhs_pipe *pipe, int len);
>>>>>>> refs/remotes/origin/master
void usbhs_pipe_select_fifo(struct usbhs_pipe *pipe, struct usbhs_fifo *fifo);
void usbhs_pipe_config_update(struct usbhs_pipe *pipe, u16 devsel,
			      u16 epnum, u16 maxp);

#define usbhs_pipe_sequence_data0(pipe)	usbhs_pipe_data_sequence(pipe, 0)
#define usbhs_pipe_sequence_data1(pipe)	usbhs_pipe_data_sequence(pipe, 1)
void usbhs_pipe_data_sequence(struct usbhs_pipe *pipe, int data);

#define usbhs_pipe_to_priv(p)	((p)->priv)
#define usbhs_pipe_number(p)	(int)((p) - (p)->priv->pipe_info.pipe)
#define usbhs_pipe_is_dcp(p)	((p)->priv->pipe_info.pipe == (p))
#define usbhs_pipe_to_fifo(p)	((p)->fifo)
#define usbhs_pipe_is_busy(p)	usbhs_pipe_to_fifo(p)

#define usbhs_pipe_type(p)		((p)->pipe_type)
#define usbhs_pipe_type_is(p, t)	((p)->pipe_type == t)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * dcp control
 */
struct usbhs_pipe *usbhs_dcp_malloc(struct usbhs_priv *priv);
void usbhs_dcp_control_transfer_done(struct usbhs_pipe *pipe);
<<<<<<< HEAD
<<<<<<< HEAD
=======
void usbhs_dcp_dir_for_host(struct usbhs_pipe *pipe, int dir_out);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void usbhs_dcp_dir_for_host(struct usbhs_pipe *pipe, int dir_out);
>>>>>>> refs/remotes/origin/master

#endif /* RENESAS_USB_PIPE_H */
