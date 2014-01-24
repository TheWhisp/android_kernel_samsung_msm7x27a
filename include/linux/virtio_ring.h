#ifndef _LINUX_VIRTIO_RING_H
#define _LINUX_VIRTIO_RING_H
<<<<<<< HEAD
/* An interface for efficient virtio implementation, currently for use by KVM
 * and lguest, but hopefully others soon.  Do NOT change this since it will
 * break existing servers and clients.
 *
 * This header is BSD licensed so anyone can use the definitions to implement
 * compatible drivers/servers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of IBM nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL IBM OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Copyright Rusty Russell IBM Corporation 2007. */
#include <linux/types.h>

/* This marks a buffer as continuing via the next field. */
#define VRING_DESC_F_NEXT	1
/* This marks a buffer as write-only (otherwise read-only). */
#define VRING_DESC_F_WRITE	2
/* This means the buffer contains a list of buffer descriptors. */
#define VRING_DESC_F_INDIRECT	4

/* The Host uses this in used->flags to advise the Guest: don't kick me when
 * you add a buffer.  It's unreliable, so it's simply an optimization.  Guest
 * will still kick if it's out of buffers. */
#define VRING_USED_F_NO_NOTIFY	1
/* The Guest uses this in avail->flags to advise the Host: don't interrupt me
 * when you consume a buffer.  It's unreliable, so it's simply an
 * optimization.  */
#define VRING_AVAIL_F_NO_INTERRUPT	1

/* We support indirect buffer descriptors */
#define VIRTIO_RING_F_INDIRECT_DESC	28

/* The Guest publishes the used index for which it expects an interrupt
 * at the end of the avail ring. Host should ignore the avail->flags field. */
/* The Host publishes the avail index for which it expects a kick
 * at the end of the used ring. Guest should ignore the used->flags field. */
#define VIRTIO_RING_F_EVENT_IDX		29

/* Virtio ring descriptors: 16 bytes.  These can chain together via "next". */
struct vring_desc {
	/* Address (guest-physical). */
	__u64 addr;
	/* Length. */
	__u32 len;
	/* The flags as indicated above. */
	__u16 flags;
	/* We chain unused descriptors via this, too */
	__u16 next;
};

struct vring_avail {
	__u16 flags;
	__u16 idx;
	__u16 ring[];
};

/* u32 is used here for ids for padding reasons. */
struct vring_used_elem {
	/* Index of start of used descriptor chain. */
	__u32 id;
	/* Total length of the descriptor chain which was used (written to) */
	__u32 len;
};

struct vring_used {
	__u16 flags;
	__u16 idx;
	struct vring_used_elem ring[];
};

struct vring {
	unsigned int num;

	struct vring_desc *desc;

	struct vring_avail *avail;

	struct vring_used *used;
};

/* The standard layout for the ring is a continuous chunk of memory which looks
 * like this.  We assume num is a power of 2.
 *
 * struct vring
 * {
 *	// The actual descriptors (16 bytes each)
 *	struct vring_desc desc[num];
 *
 *	// A ring of available descriptor heads with free-running index.
 *	__u16 avail_flags;
 *	__u16 avail_idx;
 *	__u16 available[num];
 *	__u16 used_event_idx;
 *
 *	// Padding to the next align boundary.
 *	char pad[];
 *
 *	// A ring of used descriptor heads with free-running index.
 *	__u16 used_flags;
 *	__u16 used_idx;
 *	struct vring_used_elem used[num];
 *	__u16 avail_event_idx;
 * };
 */
/* We publish the used event index at the end of the available ring, and vice
 * versa. They are at the end for backwards compatibility. */
#define vring_used_event(vr) ((vr)->avail->ring[(vr)->num])
#define vring_avail_event(vr) (*(__u16 *)&(vr)->used->ring[(vr)->num])

static inline void vring_init(struct vring *vr, unsigned int num, void *p,
			      unsigned long align)
{
	vr->num = num;
	vr->desc = p;
	vr->avail = p + num*sizeof(struct vring_desc);
<<<<<<< HEAD
	vr->used = (void *)(((unsigned long)&vr->avail->ring[num] + align-1)
			    & ~(align - 1));
=======
	vr->used = (void *)(((unsigned long)&vr->avail->ring[num] + sizeof(__u16)
		+ align-1) & ~(align - 1));
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline unsigned vring_size(unsigned int num, unsigned long align)
{
<<<<<<< HEAD
	return ((sizeof(struct vring_desc) * num + sizeof(__u16) * (2 + num)
=======
	return ((sizeof(struct vring_desc) * num + sizeof(__u16) * (3 + num)
>>>>>>> refs/remotes/origin/cm-10.0
		 + align - 1) & ~(align - 1))
		+ sizeof(__u16) * 3 + sizeof(struct vring_used_elem) * num;
}

/* The following is used with USED_EVENT_IDX and AVAIL_EVENT_IDX */
/* Assuming a given event_idx value from the other size, if
 * we have just incremented index from old to new_idx,
 * should we trigger an event? */
static inline int vring_need_event(__u16 event_idx, __u16 new_idx, __u16 old)
{
	/* Note: Xen has similar logic for notification hold-off
	 * in include/xen/interface/io/ring.h with req_event and req_prod
	 * corresponding to event_idx + 1 and new_idx respectively.
	 * Note also that req_event and req_prod in Xen start at 1,
	 * event indexes in virtio start at 0. */
	return (__u16)(new_idx - event_idx - 1) < (__u16)(new_idx - old);
}

#ifdef __KERNEL__
#include <linux/irqreturn.h>
struct virtio_device;
struct virtqueue;

struct virtqueue *vring_new_virtqueue(unsigned int num,
				      unsigned int vring_align,
				      struct virtio_device *vdev,
<<<<<<< HEAD
=======
				      bool weak_barriers,
>>>>>>> refs/remotes/origin/cm-10.0
				      void *pages,
				      void (*notify)(struct virtqueue *vq),
=======

#include <asm/barrier.h>
#include <linux/irqreturn.h>
#include <uapi/linux/virtio_ring.h>

/*
 * Barriers in virtio are tricky.  Non-SMP virtio guests can't assume
 * they're not on an SMP host system, so they need to assume real
 * barriers.  Non-SMP virtio hosts could skip the barriers, but does
 * anyone care?
 *
 * For virtio_pci on SMP, we don't need to order with respect to MMIO
 * accesses through relaxed memory I/O windows, so smp_mb() et al are
 * sufficient.
 *
 * For using virtio to talk to real devices (eg. other heterogeneous
 * CPUs) we do need real barriers.  In theory, we could be using both
 * kinds of virtio, so it's a runtime decision, and the branch is
 * actually quite cheap.
 */

#ifdef CONFIG_SMP
static inline void virtio_mb(bool weak_barriers)
{
	if (weak_barriers)
		smp_mb();
	else
		mb();
}

static inline void virtio_rmb(bool weak_barriers)
{
	if (weak_barriers)
		smp_rmb();
	else
		rmb();
}

static inline void virtio_wmb(bool weak_barriers)
{
	if (weak_barriers)
		smp_wmb();
	else
		wmb();
}
#else
static inline void virtio_mb(bool weak_barriers)
{
	mb();
}

static inline void virtio_rmb(bool weak_barriers)
{
	rmb();
}

static inline void virtio_wmb(bool weak_barriers)
{
	wmb();
}
#endif

struct virtio_device;
struct virtqueue;

struct virtqueue *vring_new_virtqueue(unsigned int index,
				      unsigned int num,
				      unsigned int vring_align,
				      struct virtio_device *vdev,
				      bool weak_barriers,
				      void *pages,
				      bool (*notify)(struct virtqueue *vq),
>>>>>>> refs/remotes/origin/master
				      void (*callback)(struct virtqueue *vq),
				      const char *name);
void vring_del_virtqueue(struct virtqueue *vq);
/* Filter out transport-specific feature bits. */
void vring_transport_features(struct virtio_device *vdev);

irqreturn_t vring_interrupt(int irq, void *_vq);
<<<<<<< HEAD
#endif /* __KERNEL__ */
=======
>>>>>>> refs/remotes/origin/master
#endif /* _LINUX_VIRTIO_RING_H */
