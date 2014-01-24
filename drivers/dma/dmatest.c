/*
 * DMA Engine test module
 *
 * Copyright (C) 2007 Atmel Corporation
<<<<<<< HEAD
=======
 * Copyright (C) 2013 Intel Corporation
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/dmaengine.h>
=======
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/freezer.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/freezer.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/wait.h>

static unsigned int test_buf_size = 16384;
<<<<<<< HEAD
module_param(test_buf_size, uint, S_IRUGO);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel), S_IRUGO);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[20];
module_param_string(device, test_device, sizeof(test_device), S_IRUGO);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO);
=======
module_param(test_buf_size, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(test_buf_size, "Size of the memcpy test buffer");

static char test_channel[20];
module_param_string(channel, test_channel, sizeof(test_channel),
		S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(channel, "Bus ID of the channel to test (default: any)");

static char test_device[20];
module_param_string(device, test_device, sizeof(test_device),
		S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(device, "Bus ID of the DMA Engine to test (default: any)");

static unsigned int threads_per_chan = 1;
module_param(threads_per_chan, uint, S_IRUGO | S_IWUSR);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(threads_per_chan,
		"Number of threads to start per channel (default: 1)");

static unsigned int max_channels;
<<<<<<< HEAD
module_param(max_channels, uint, S_IRUGO);
=======
module_param(max_channels, uint, S_IRUGO | S_IWUSR);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(max_channels,
		"Maximum number of channels to use (default: all)");

static unsigned int iterations;
<<<<<<< HEAD
module_param(iterations, uint, S_IRUGO);
=======
module_param(iterations, uint, S_IRUGO | S_IWUSR);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(iterations,
		"Iterations before stopping test (default: infinite)");

static unsigned int xor_sources = 3;
<<<<<<< HEAD
module_param(xor_sources, uint, S_IRUGO);
=======
module_param(xor_sources, uint, S_IRUGO | S_IWUSR);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(xor_sources,
		"Number of xor source buffers (default: 3)");

static unsigned int pq_sources = 3;
<<<<<<< HEAD
module_param(pq_sources, uint, S_IRUGO);
=======
module_param(pq_sources, uint, S_IRUGO | S_IWUSR);
>>>>>>> refs/remotes/origin/master
MODULE_PARM_DESC(pq_sources,
		"Number of p+q source buffers (default: 3)");

static int timeout = 3000;
<<<<<<< HEAD
module_param(timeout, uint, S_IRUGO);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

=======
module_param(timeout, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(timeout, "Transfer Timeout in msec (default: 3000), "
		 "Pass -1 for infinite timeout");

static bool noverify;
module_param(noverify, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(noverify, "Disable random data setup and verification");

static bool verbose;
module_param(verbose, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(verbose, "Enable \"success\" result messages (default: off)");

/**
 * struct dmatest_params - test parameters.
 * @buf_size:		size of the memcpy test buffer
 * @channel:		bus ID of the channel to test
 * @device:		bus ID of the DMA Engine to test
 * @threads_per_chan:	number of threads to start per channel
 * @max_channels:	maximum number of channels to use
 * @iterations:		iterations before stopping test
 * @xor_sources:	number of xor source buffers
 * @pq_sources:		number of p+q source buffers
 * @timeout:		transfer timeout in msec, -1 for infinite timeout
 */
struct dmatest_params {
	unsigned int	buf_size;
	char		channel[20];
	char		device[20];
	unsigned int	threads_per_chan;
	unsigned int	max_channels;
	unsigned int	iterations;
	unsigned int	xor_sources;
	unsigned int	pq_sources;
	int		timeout;
	bool		noverify;
};

/**
 * struct dmatest_info - test information.
 * @params:		test parameters
 * @lock:		access protection to the fields of this structure
 */
static struct dmatest_info {
	/* Test parameters */
	struct dmatest_params	params;

	/* Internal state */
	struct list_head	channels;
	unsigned int		nr_channels;
	struct mutex		lock;
	bool			did_init;
} test_info = {
	.channels = LIST_HEAD_INIT(test_info.channels),
	.lock = __MUTEX_INITIALIZER(test_info.lock),
};

static int dmatest_run_set(const char *val, const struct kernel_param *kp);
static int dmatest_run_get(char *val, const struct kernel_param *kp);
static struct kernel_param_ops run_ops = {
	.set = dmatest_run_set,
	.get = dmatest_run_get,
};
static bool dmatest_run;
module_param_cb(run, &run_ops, &dmatest_run, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(run, "Run the test (default: false)");

/* Maximum amount of mismatched bytes in buffer to print */
#define MAX_ERROR_COUNT		32

>>>>>>> refs/remotes/origin/master
/*
 * Initialization patterns. All bytes in the source buffer has bit 7
 * set, all bytes in the destination buffer has bit 7 cleared.
 *
 * Bit 6 is set for all bytes which are to be copied by the DMA
 * engine. Bit 5 is set for all bytes which are to be overwritten by
 * the DMA engine.
 *
 * The remaining bits are the inverse of a counter which increments by
 * one for each byte address.
 */
#define PATTERN_SRC		0x80
#define PATTERN_DST		0x00
#define PATTERN_COPY		0x40
#define PATTERN_OVERWRITE	0x20
#define PATTERN_COUNT_MASK	0x1f

struct dmatest_thread {
	struct list_head	node;
<<<<<<< HEAD
=======
	struct dmatest_info	*info;
>>>>>>> refs/remotes/origin/master
	struct task_struct	*task;
	struct dma_chan		*chan;
	u8			**srcs;
	u8			**dsts;
	enum dma_transaction_type type;
<<<<<<< HEAD
=======
	bool			done;
>>>>>>> refs/remotes/origin/master
};

struct dmatest_chan {
	struct list_head	node;
	struct dma_chan		*chan;
	struct list_head	threads;
};

<<<<<<< HEAD
/*
 * These are protected by dma_list_mutex since they're only used by
 * the DMA filter function callback
 */
static LIST_HEAD(dmatest_channels);
static unsigned int nr_channels;

static bool dmatest_match_channel(struct dma_chan *chan)
{
	if (test_channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), test_channel) == 0;
}

static bool dmatest_match_device(struct dma_device *device)
{
	if (test_device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), test_device) == 0;
=======
static DECLARE_WAIT_QUEUE_HEAD(thread_wait);
static bool wait;

static bool is_threaded_test_run(struct dmatest_info *info)
{
	struct dmatest_chan *dtc;

	list_for_each_entry(dtc, &info->channels, node) {
		struct dmatest_thread *thread;

		list_for_each_entry(thread, &dtc->threads, node) {
			if (!thread->done)
				return true;
		}
	}

	return false;
}

static int dmatest_wait_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;

	if (params->iterations)
		wait_event(thread_wait, !is_threaded_test_run(info));
	wait = true;
	return param_get_bool(val, kp);
}

static struct kernel_param_ops wait_ops = {
	.get = dmatest_wait_get,
	.set = param_set_bool,
};
module_param_cb(wait, &wait_ops, &wait, S_IRUGO);
MODULE_PARM_DESC(wait, "Wait for tests to complete (default: false)");

static bool dmatest_match_channel(struct dmatest_params *params,
		struct dma_chan *chan)
{
	if (params->channel[0] == '\0')
		return true;
	return strcmp(dma_chan_name(chan), params->channel) == 0;
}

static bool dmatest_match_device(struct dmatest_params *params,
		struct dma_device *device)
{
	if (params->device[0] == '\0')
		return true;
	return strcmp(dev_name(device->dev), params->device) == 0;
>>>>>>> refs/remotes/origin/master
}

static unsigned long dmatest_random(void)
{
	unsigned long buf;

<<<<<<< HEAD
	get_random_bytes(&buf, sizeof(buf));
	return buf;
}

static void dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len)
=======
	prandom_bytes(&buf, sizeof(buf));
	return buf;
}

static void dmatest_init_srcs(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size)
>>>>>>> refs/remotes/origin/master
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_SRC | PATTERN_COPY
				| (~i & PATTERN_COUNT_MASK);
<<<<<<< HEAD
		for ( ; i < test_buf_size; i++)
=======
		for ( ; i < buf_size; i++)
>>>>>>> refs/remotes/origin/master
			buf[i] = PATTERN_SRC | (~i & PATTERN_COUNT_MASK);
		buf++;
	}
}

<<<<<<< HEAD
static void dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len)
=======
static void dmatest_init_dsts(u8 **bufs, unsigned int start, unsigned int len,
		unsigned int buf_size)
>>>>>>> refs/remotes/origin/master
{
	unsigned int i;
	u8 *buf;

	for (; (buf = *bufs); bufs++) {
		for (i = 0; i < start; i++)
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
		for ( ; i < start + len; i++)
			buf[i] = PATTERN_DST | PATTERN_OVERWRITE
				| (~i & PATTERN_COUNT_MASK);
<<<<<<< HEAD
		for ( ; i < test_buf_size; i++)
=======
		for ( ; i < buf_size; i++)
>>>>>>> refs/remotes/origin/master
			buf[i] = PATTERN_DST | (~i & PATTERN_COUNT_MASK);
	}
}

static void dmatest_mismatch(u8 actual, u8 pattern, unsigned int index,
		unsigned int counter, bool is_srcbuf)
{
	u8		diff = actual ^ pattern;
	u8		expected = pattern | (~counter & PATTERN_COUNT_MASK);
	const char	*thread_name = current->comm;

	if (is_srcbuf)
<<<<<<< HEAD
		pr_warning("%s: srcbuf[0x%x] overwritten!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warning("%s: dstbuf[0x%x] not copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warning("%s: dstbuf[0x%x] was copied!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
	else
		pr_warning("%s: dstbuf[0x%x] mismatch!"
				" Expected %02x, got %02x\n",
				thread_name, index, expected, actual);
=======
		pr_warn("%s: srcbuf[0x%x] overwritten! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if ((pattern & PATTERN_COPY)
			&& (diff & (PATTERN_COPY | PATTERN_OVERWRITE)))
		pr_warn("%s: dstbuf[0x%x] not copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else if (diff & PATTERN_SRC)
		pr_warn("%s: dstbuf[0x%x] was copied! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
	else
		pr_warn("%s: dstbuf[0x%x] mismatch! Expected %02x, got %02x\n",
			thread_name, index, expected, actual);
>>>>>>> refs/remotes/origin/master
}

static unsigned int dmatest_verify(u8 **bufs, unsigned int start,
		unsigned int end, unsigned int counter, u8 pattern,
		bool is_srcbuf)
{
	unsigned int i;
	unsigned int error_count = 0;
	u8 actual;
	u8 expected;
	u8 *buf;
	unsigned int counter_orig = counter;

	for (; (buf = *bufs); bufs++) {
		counter = counter_orig;
		for (i = start; i < end; i++) {
			actual = buf[i];
			expected = pattern | (~counter & PATTERN_COUNT_MASK);
			if (actual != expected) {
<<<<<<< HEAD
				if (error_count < 32)
=======
				if (error_count < MAX_ERROR_COUNT)
>>>>>>> refs/remotes/origin/master
					dmatest_mismatch(actual, pattern, i,
							 counter, is_srcbuf);
				error_count++;
			}
			counter++;
		}
	}

<<<<<<< HEAD
	if (error_count > 32)
		pr_warning("%s: %u errors suppressed\n",
			current->comm, error_count - 32);
=======
	if (error_count > MAX_ERROR_COUNT)
		pr_warn("%s: %u errors suppressed\n",
			current->comm, error_count - MAX_ERROR_COUNT);
>>>>>>> refs/remotes/origin/master

	return error_count;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void dmatest_callback(void *completion)
{
	complete(completion);
=======
=======
>>>>>>> refs/remotes/origin/master
/* poor man's completion - we want to use wait_event_freezable() on it */
struct dmatest_done {
	bool			done;
	wait_queue_head_t	*wait;
};

static void dmatest_callback(void *arg)
{
	struct dmatest_done *done = arg;

	done->done = true;
	wake_up_all(done->wait);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static unsigned int min_odd(unsigned int x, unsigned int y)
{
	unsigned int val = min(x, y);

	return val % 2 ? val : val - 1;
}

static void result(const char *err, unsigned int n, unsigned int src_off,
		   unsigned int dst_off, unsigned int len, unsigned long data)
{
	pr_info("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)",
		current->comm, n, err, src_off, dst_off, len, data);
}

static void dbg_result(const char *err, unsigned int n, unsigned int src_off,
		       unsigned int dst_off, unsigned int len,
		       unsigned long data)
{
	pr_debug("%s: result #%u: '%s' with src_off=0x%x dst_off=0x%x len=0x%x (%lu)",
		   current->comm, n, err, src_off, dst_off, len, data);
}

#define verbose_result(err, n, src_off, dst_off, len, data) ({ \
	if (verbose) \
		result(err, n, src_off, dst_off, len, data); \
	else \
		dbg_result(err, n, src_off, dst_off, len, data); \
})

static unsigned long long dmatest_persec(s64 runtime, unsigned int val)
{
	unsigned long long per_sec = 1000000;

	if (runtime <= 0)
		return 0;

	/* drop precision until runtime is 32-bits */
	while (runtime > UINT_MAX) {
		runtime >>= 1;
		per_sec <<= 1;
	}

	per_sec *= val;
	do_div(per_sec, runtime);
	return per_sec;
}

static unsigned long long dmatest_KBs(s64 runtime, unsigned long long len)
{
	return dmatest_persec(runtime, len >> 10);
>>>>>>> refs/remotes/origin/master
}

/*
 * This function repeatedly tests DMA transfers of various lengths and
 * offsets for a given operation type until it is told to exit by
 * kthread_stop(). There may be multiple threads running this function
 * in parallel for a single channel, and there may be multiple channels
 * being tested in parallel.
 *
 * Before each test, the source and destination buffer is initialized
 * with a known pattern. This pattern is different depending on
 * whether it's in an area which is supposed to be copied or
 * overwritten, and different in the source and destination buffers.
 * So if the DMA engine doesn't copy exactly what we tell it to copy,
 * we'll notice.
 */
static int dmatest_func(void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct dmatest_thread	*thread = data;
=======
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK(done_wait);
	struct dmatest_thread	*thread = data;
	struct dmatest_done	done = { .wait = &done_wait };
>>>>>>> refs/remotes/origin/cm-10.0
	struct dma_chan		*chan;
	const char		*thread_name;
=======
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK(done_wait);
	struct dmatest_thread	*thread = data;
	struct dmatest_done	done = { .wait = &done_wait };
	struct dmatest_info	*info;
	struct dmatest_params	*params;
	struct dma_chan		*chan;
	struct dma_device	*dev;
>>>>>>> refs/remotes/origin/master
	unsigned int		src_off, dst_off, len;
	unsigned int		error_count;
	unsigned int		failed_tests = 0;
	unsigned int		total_tests = 0;
	dma_cookie_t		cookie;
	enum dma_status		status;
	enum dma_ctrl_flags 	flags;
<<<<<<< HEAD
	u8			pq_coefs[pq_sources + 1];
=======
	u8			*pq_coefs = NULL;
>>>>>>> refs/remotes/origin/master
	int			ret;
	int			src_cnt;
	int			dst_cnt;
	int			i;
<<<<<<< HEAD

	thread_name = current->comm;
<<<<<<< HEAD
=======
	set_freezable();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ktime_t			ktime;
	s64			runtime = 0;
	unsigned long long	total_len = 0;

	set_freezable();
>>>>>>> refs/remotes/origin/master

	ret = -ENOMEM;

	smp_rmb();
<<<<<<< HEAD
	chan = thread->chan;
	if (thread->type == DMA_MEMCPY)
		src_cnt = dst_cnt = 1;
	else if (thread->type == DMA_XOR) {
		src_cnt = xor_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 1;
	} else if (thread->type == DMA_PQ) {
		src_cnt = pq_sources | 1; /* force odd to ensure dst = src */
		dst_cnt = 2;
		for (i = 0; i < src_cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_srcs;
=======
	info = thread->info;
	params = &info->params;
	chan = thread->chan;
	dev = chan->device;
	if (thread->type == DMA_MEMCPY)
		src_cnt = dst_cnt = 1;
	else if (thread->type == DMA_XOR) {
		/* force odd to ensure dst = src */
		src_cnt = min_odd(params->xor_sources | 1, dev->max_xor);
		dst_cnt = 1;
	} else if (thread->type == DMA_PQ) {
		/* force odd to ensure dst = src */
		src_cnt = min_odd(params->pq_sources | 1, dma_maxpq(dev, 0));
		dst_cnt = 2;

		pq_coefs = kmalloc(params->pq_sources+1, GFP_KERNEL);
		if (!pq_coefs)
			goto err_thread_type;

		for (i = 0; i < src_cnt; i++)
			pq_coefs[i] = 1;
	} else
		goto err_thread_type;
>>>>>>> refs/remotes/origin/master

	thread->srcs = kcalloc(src_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->srcs)
		goto err_srcs;
	for (i = 0; i < src_cnt; i++) {
<<<<<<< HEAD
		thread->srcs[i] = kmalloc(test_buf_size, GFP_KERNEL);
=======
		thread->srcs[i] = kmalloc(params->buf_size, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!thread->srcs[i])
			goto err_srcbuf;
	}
	thread->srcs[i] = NULL;

	thread->dsts = kcalloc(dst_cnt+1, sizeof(u8 *), GFP_KERNEL);
	if (!thread->dsts)
		goto err_dsts;
	for (i = 0; i < dst_cnt; i++) {
<<<<<<< HEAD
		thread->dsts[i] = kmalloc(test_buf_size, GFP_KERNEL);
=======
		thread->dsts[i] = kmalloc(params->buf_size, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!thread->dsts[i])
			goto err_dstbuf;
	}
	thread->dsts[i] = NULL;

	set_user_nice(current, 10);

	/*
<<<<<<< HEAD
	 * src buffers are freed by the DMAEngine code with dma_unmap_single()
	 * dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT
	      | DMA_COMPL_SKIP_DEST_UNMAP | DMA_COMPL_SRC_UNMAP_SINGLE;

	while (!kthread_should_stop()
	       && !(iterations && total_tests >= iterations)) {
		struct dma_device *dev = chan->device;
		struct dma_async_tx_descriptor *tx = NULL;
		dma_addr_t dma_srcs[src_cnt];
		dma_addr_t dma_dsts[dst_cnt];
<<<<<<< HEAD
		struct completion cmp;
		unsigned long tmo = msecs_to_jiffies(timeout);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * src and dst buffers are freed by ourselves below
	 */
	flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;

	ktime = ktime_get();
	while (!kthread_should_stop()
	       && !(params->iterations && total_tests >= params->iterations)) {
		struct dma_async_tx_descriptor *tx = NULL;
		struct dmaengine_unmap_data *um;
		dma_addr_t srcs[src_cnt];
		dma_addr_t *dsts;
>>>>>>> refs/remotes/origin/master
		u8 align = 0;

		total_tests++;

		/* honor alignment restrictions */
		if (thread->type == DMA_MEMCPY)
			align = dev->copy_align;
		else if (thread->type == DMA_XOR)
			align = dev->xor_align;
		else if (thread->type == DMA_PQ)
			align = dev->pq_align;

<<<<<<< HEAD
		if (1 << align > test_buf_size) {
			pr_err("%u-byte buffer too small for %d-byte alignment\n",
			       test_buf_size, 1 << align);
			break;
		}

		len = dmatest_random() % test_buf_size + 1;
		len = (len >> align) << align;
		if (!len)
			len = 1 << align;
		src_off = dmatest_random() % (test_buf_size - len + 1);
		dst_off = dmatest_random() % (test_buf_size - len + 1);

		src_off = (src_off >> align) << align;
		dst_off = (dst_off >> align) << align;

		dmatest_init_srcs(thread->srcs, src_off, len);
		dmatest_init_dsts(thread->dsts, dst_off, len);

		for (i = 0; i < src_cnt; i++) {
			u8 *buf = thread->srcs[i] + src_off;

			dma_srcs[i] = dma_map_single(dev->dev, buf, len,
						     DMA_TO_DEVICE);
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		for (i = 0; i < dst_cnt; i++) {
			dma_dsts[i] = dma_map_single(dev->dev, thread->dsts[i],
						     test_buf_size,
						     DMA_BIDIRECTIONAL);
		}


		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
							 dma_dsts[0] + dst_off,
							 dma_srcs[0], len,
							 flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dma_dsts[0] + dst_off,
						      dma_srcs, src_cnt,
=======
		if (1 << align > params->buf_size) {
			pr_err("%u-byte buffer too small for %d-byte alignment\n",
			       params->buf_size, 1 << align);
			break;
		}

		if (params->noverify) {
			len = params->buf_size;
			src_off = 0;
			dst_off = 0;
		} else {
			len = dmatest_random() % params->buf_size + 1;
			len = (len >> align) << align;
			if (!len)
				len = 1 << align;
			src_off = dmatest_random() % (params->buf_size - len + 1);
			dst_off = dmatest_random() % (params->buf_size - len + 1);

			src_off = (src_off >> align) << align;
			dst_off = (dst_off >> align) << align;

			dmatest_init_srcs(thread->srcs, src_off, len,
					  params->buf_size);
			dmatest_init_dsts(thread->dsts, dst_off, len,
					  params->buf_size);
		}

		len = (len >> align) << align;
		if (!len)
			len = 1 << align;
		total_len += len;

		um = dmaengine_get_unmap_data(dev->dev, src_cnt+dst_cnt,
					      GFP_KERNEL);
		if (!um) {
			failed_tests++;
			result("unmap data NULL", total_tests,
			       src_off, dst_off, len, ret);
			continue;
		}

		um->len = params->buf_size;
		for (i = 0; i < src_cnt; i++) {
			void *buf = thread->srcs[i];
			struct page *pg = virt_to_page(buf);
			unsigned pg_off = (unsigned long) buf & ~PAGE_MASK;

			um->addr[i] = dma_map_page(dev->dev, pg, pg_off,
						   um->len, DMA_TO_DEVICE);
			srcs[i] = um->addr[i] + src_off;
			ret = dma_mapping_error(dev->dev, um->addr[i]);
			if (ret) {
				dmaengine_unmap_put(um);
				result("src mapping error", total_tests,
				       src_off, dst_off, len, ret);
				failed_tests++;
				continue;
			}
			um->to_cnt++;
		}
		/* map with DMA_BIDIRECTIONAL to force writeback/invalidate */
		dsts = &um->addr[src_cnt];
		for (i = 0; i < dst_cnt; i++) {
			void *buf = thread->dsts[i];
			struct page *pg = virt_to_page(buf);
			unsigned pg_off = (unsigned long) buf & ~PAGE_MASK;

			dsts[i] = dma_map_page(dev->dev, pg, pg_off, um->len,
					       DMA_BIDIRECTIONAL);
			ret = dma_mapping_error(dev->dev, dsts[i]);
			if (ret) {
				dmaengine_unmap_put(um);
				result("dst mapping error", total_tests,
				       src_off, dst_off, len, ret);
				failed_tests++;
				continue;
			}
			um->bidi_cnt++;
		}

		if (thread->type == DMA_MEMCPY)
			tx = dev->device_prep_dma_memcpy(chan,
							 dsts[0] + dst_off,
							 srcs[0], len, flags);
		else if (thread->type == DMA_XOR)
			tx = dev->device_prep_dma_xor(chan,
						      dsts[0] + dst_off,
						      srcs, src_cnt,
>>>>>>> refs/remotes/origin/master
						      len, flags);
		else if (thread->type == DMA_PQ) {
			dma_addr_t dma_pq[dst_cnt];

			for (i = 0; i < dst_cnt; i++)
<<<<<<< HEAD
				dma_pq[i] = dma_dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, dma_srcs,
=======
				dma_pq[i] = dsts[i] + dst_off;
			tx = dev->device_prep_dma_pq(chan, dma_pq, srcs,
>>>>>>> refs/remotes/origin/master
						     src_cnt, pq_coefs,
						     len, flags);
		}

		if (!tx) {
<<<<<<< HEAD
			for (i = 0; i < src_cnt; i++)
				dma_unmap_single(dev->dev, dma_srcs[i], len,
						 DMA_TO_DEVICE);
			for (i = 0; i < dst_cnt; i++)
				dma_unmap_single(dev->dev, dma_dsts[i],
						 test_buf_size,
						 DMA_BIDIRECTIONAL);
			pr_warning("%s: #%u: prep error with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1,
					src_off, dst_off, len);
=======
			dmaengine_unmap_put(um);
			result("prep error", total_tests, src_off,
			       dst_off, len, ret);
>>>>>>> refs/remotes/origin/master
			msleep(100);
			failed_tests++;
			continue;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		init_completion(&cmp);
		tx->callback = dmatest_callback;
		tx->callback_param = &cmp;
=======
		done.done = false;
		tx->callback = dmatest_callback;
		tx->callback_param = &done;
>>>>>>> refs/remotes/origin/cm-10.0
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			pr_warning("%s: #%u: submit error %d with src_off=0x%x "
					"dst_off=0x%x len=0x%x\n",
					thread_name, total_tests - 1, cookie,
					src_off, dst_off, len);
=======
		done.done = false;
		tx->callback = dmatest_callback;
		tx->callback_param = &done;
		cookie = tx->tx_submit(tx);

		if (dma_submit_error(cookie)) {
			dmaengine_unmap_put(um);
			result("submit error", total_tests, src_off,
			       dst_off, len, ret);
>>>>>>> refs/remotes/origin/master
			msleep(100);
			failed_tests++;
			continue;
		}
		dma_async_issue_pending(chan);

<<<<<<< HEAD
<<<<<<< HEAD
		tmo = wait_for_completion_timeout(&cmp, tmo);
		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);

		if (tmo == 0) {
=======
		wait_event_freezable_timeout(done_wait, done.done,
					     msecs_to_jiffies(timeout));
=======
		wait_event_freezable_timeout(done_wait, done.done,
					     msecs_to_jiffies(params->timeout));
>>>>>>> refs/remotes/origin/master

		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);

		if (!done.done) {
			/*
			 * We're leaving the timed out dma operation with
			 * dangling pointer to done_wait.  To make this
			 * correct, we'll need to allocate wait_done for
			 * each test iteration and perform "who's gonna
			 * free it this time?" dancing.  For now, just
			 * leave it dangling.
			 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			pr_warning("%s: #%u: test timed out\n",
				   thread_name, total_tests - 1);
			failed_tests++;
			continue;
		} else if (status != DMA_SUCCESS) {
			pr_warning("%s: #%u: got completion callback,"
				   " but status is \'%s\'\n",
				   thread_name, total_tests - 1,
				   status == DMA_ERROR ? "error" : "in progress");
=======
			dmaengine_unmap_put(um);
			result("test timed out", total_tests, src_off, dst_off,
			       len, 0);
			failed_tests++;
			continue;
		} else if (status != DMA_COMPLETE) {
			dmaengine_unmap_put(um);
			result(status == DMA_ERROR ?
			       "completion error status" :
			       "completion busy status", total_tests, src_off,
			       dst_off, len, ret);
>>>>>>> refs/remotes/origin/master
			failed_tests++;
			continue;
		}

<<<<<<< HEAD
		/* Unmap by myself (see DMA_COMPL_SKIP_DEST_UNMAP above) */
		for (i = 0; i < dst_cnt; i++)
			dma_unmap_single(dev->dev, dma_dsts[i], test_buf_size,
					 DMA_BIDIRECTIONAL);

		error_count = 0;

		pr_debug("%s: verifying source buffer...\n", thread_name);
		error_count += dmatest_verify(thread->srcs, 0, src_off,
=======
		dmaengine_unmap_put(um);

		if (params->noverify) {
			verbose_result("test passed", total_tests, src_off,
				       dst_off, len, 0);
			continue;
		}

		pr_debug("%s: verifying source buffer...\n", current->comm);
		error_count = dmatest_verify(thread->srcs, 0, src_off,
>>>>>>> refs/remotes/origin/master
				0, PATTERN_SRC, true);
		error_count += dmatest_verify(thread->srcs, src_off,
				src_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, true);
		error_count += dmatest_verify(thread->srcs, src_off + len,
<<<<<<< HEAD
				test_buf_size, src_off + len,
				PATTERN_SRC, true);

		pr_debug("%s: verifying dest buffer...\n",
				thread->task->comm);
=======
				params->buf_size, src_off + len,
				PATTERN_SRC, true);

		pr_debug("%s: verifying dest buffer...\n", current->comm);
>>>>>>> refs/remotes/origin/master
		error_count += dmatest_verify(thread->dsts, 0, dst_off,
				0, PATTERN_DST, false);
		error_count += dmatest_verify(thread->dsts, dst_off,
				dst_off + len, src_off,
				PATTERN_SRC | PATTERN_COPY, false);
		error_count += dmatest_verify(thread->dsts, dst_off + len,
<<<<<<< HEAD
				test_buf_size, dst_off + len,
				PATTERN_DST, false);

		if (error_count) {
			pr_warning("%s: #%u: %u errors with "
				"src_off=0x%x dst_off=0x%x len=0x%x\n",
				thread_name, total_tests - 1, error_count,
				src_off, dst_off, len);
			failed_tests++;
		} else {
			pr_debug("%s: #%u: No errors with "
				"src_off=0x%x dst_off=0x%x len=0x%x\n",
				thread_name, total_tests - 1,
				src_off, dst_off, len);
		}
	}
=======
				params->buf_size, dst_off + len,
				PATTERN_DST, false);

		if (error_count) {
			result("data error", total_tests, src_off, dst_off,
			       len, error_count);
			failed_tests++;
		} else {
			verbose_result("test passed", total_tests, src_off,
				       dst_off, len, 0);
		}
	}
	runtime = ktime_us_delta(ktime_get(), ktime);
>>>>>>> refs/remotes/origin/master

	ret = 0;
	for (i = 0; thread->dsts[i]; i++)
		kfree(thread->dsts[i]);
err_dstbuf:
	kfree(thread->dsts);
err_dsts:
	for (i = 0; thread->srcs[i]; i++)
		kfree(thread->srcs[i]);
err_srcbuf:
	kfree(thread->srcs);
err_srcs:
<<<<<<< HEAD
	pr_notice("%s: terminating after %u tests, %u failures (status %d)\n",
			thread_name, total_tests, failed_tests, ret);

<<<<<<< HEAD
=======
	/* terminate all transfers on specified channels */
	chan->device->device_control(chan, DMA_TERMINATE_ALL, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	if (iterations > 0)
		while (!kthread_should_stop()) {
			DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wait_dmatest_exit);
			interruptible_sleep_on(&wait_dmatest_exit);
		}
=======
	kfree(pq_coefs);
err_thread_type:
	pr_info("%s: summary %u tests, %u failures %llu iops %llu KB/s (%d)\n",
		current->comm, total_tests, failed_tests,
		dmatest_persec(runtime, total_tests),
		dmatest_KBs(runtime, total_len), ret);

	/* terminate all transfers on specified channels */
	if (ret)
		dmaengine_terminate_all(chan);

	thread->done = true;
	wake_up(&thread_wait);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static void dmatest_cleanup_channel(struct dmatest_chan *dtc)
{
	struct dmatest_thread	*thread;
	struct dmatest_thread	*_thread;
	int			ret;

	list_for_each_entry_safe(thread, _thread, &dtc->threads, node) {
		ret = kthread_stop(thread->task);
<<<<<<< HEAD
		pr_debug("dmatest: thread %s exited with status %d\n",
				thread->task->comm, ret);
		list_del(&thread->node);
		kfree(thread);
	}
<<<<<<< HEAD
=======

	/* terminate all transfers on specified channels */
	dtc->chan->device->device_control(dtc->chan, DMA_TERMINATE_ALL, 0);

>>>>>>> refs/remotes/origin/cm-10.0
	kfree(dtc);
}

static int dmatest_add_threads(struct dmatest_chan *dtc, enum dma_transaction_type type)
{
=======
		pr_debug("thread %s exited with status %d\n",
			 thread->task->comm, ret);
		list_del(&thread->node);
		put_task_struct(thread->task);
		kfree(thread);
	}

	/* terminate all transfers on specified channels */
	dmaengine_terminate_all(dtc->chan);

	kfree(dtc);
}

static int dmatest_add_threads(struct dmatest_info *info,
		struct dmatest_chan *dtc, enum dma_transaction_type type)
{
	struct dmatest_params *params = &info->params;
>>>>>>> refs/remotes/origin/master
	struct dmatest_thread *thread;
	struct dma_chan *chan = dtc->chan;
	char *op;
	unsigned int i;

	if (type == DMA_MEMCPY)
		op = "copy";
	else if (type == DMA_XOR)
		op = "xor";
	else if (type == DMA_PQ)
		op = "pq";
	else
		return -EINVAL;

<<<<<<< HEAD
	for (i = 0; i < threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct dmatest_thread), GFP_KERNEL);
		if (!thread) {
			pr_warning("dmatest: No memory for %s-%s%u\n",
				   dma_chan_name(chan), op, i);

			break;
		}
		thread->chan = dtc->chan;
		thread->type = type;
		smp_wmb();
		thread->task = kthread_run(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warning("dmatest: Failed to run thread %s-%s%u\n",
					dma_chan_name(chan), op, i);
=======
	for (i = 0; i < params->threads_per_chan; i++) {
		thread = kzalloc(sizeof(struct dmatest_thread), GFP_KERNEL);
		if (!thread) {
			pr_warn("No memory for %s-%s%u\n",
				dma_chan_name(chan), op, i);
			break;
		}
		thread->info = info;
		thread->chan = dtc->chan;
		thread->type = type;
		smp_wmb();
		thread->task = kthread_create(dmatest_func, thread, "%s-%s%u",
				dma_chan_name(chan), op, i);
		if (IS_ERR(thread->task)) {
			pr_warn("Failed to create thread %s-%s%u\n",
				dma_chan_name(chan), op, i);
>>>>>>> refs/remotes/origin/master
			kfree(thread);
			break;
		}

		/* srcbuf and dstbuf are allocated by the thread itself */
<<<<<<< HEAD

		list_add_tail(&thread->node, &dtc->threads);
=======
		get_task_struct(thread->task);
		list_add_tail(&thread->node, &dtc->threads);
		wake_up_process(thread->task);
>>>>>>> refs/remotes/origin/master
	}

	return i;
}

<<<<<<< HEAD
static int dmatest_add_channel(struct dma_chan *chan)
=======
static int dmatest_add_channel(struct dmatest_info *info,
		struct dma_chan *chan)
>>>>>>> refs/remotes/origin/master
{
	struct dmatest_chan	*dtc;
	struct dma_device	*dma_dev = chan->device;
	unsigned int		thread_count = 0;
	int cnt;

	dtc = kmalloc(sizeof(struct dmatest_chan), GFP_KERNEL);
	if (!dtc) {
<<<<<<< HEAD
		pr_warning("dmatest: No memory for %s\n", dma_chan_name(chan));
=======
		pr_warn("No memory for %s\n", dma_chan_name(chan));
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}

	dtc->chan = chan;
	INIT_LIST_HEAD(&dtc->threads);

	if (dma_has_cap(DMA_MEMCPY, dma_dev->cap_mask)) {
<<<<<<< HEAD
		cnt = dmatest_add_threads(dtc, DMA_MEMCPY);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(dtc, DMA_PQ);
<<<<<<< HEAD
		thread_count += cnt > 0 ?: 0;
=======
		thread_count += cnt > 0 ? cnt : 0;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	pr_info("dmatest: Started %u threads using %s\n",
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &dmatest_channels);
	nr_channels++;
=======
		cnt = dmatest_add_threads(info, dtc, DMA_MEMCPY);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_XOR, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_XOR);
		thread_count += cnt > 0 ? cnt : 0;
	}
	if (dma_has_cap(DMA_PQ, dma_dev->cap_mask)) {
		cnt = dmatest_add_threads(info, dtc, DMA_PQ);
		thread_count += cnt > 0 ? cnt : 0;
	}

	pr_info("Started %u threads using %s\n",
		thread_count, dma_chan_name(chan));

	list_add_tail(&dtc->node, &info->channels);
	info->nr_channels++;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static bool filter(struct dma_chan *chan, void *param)
{
<<<<<<< HEAD
	if (!dmatest_match_channel(chan) || !dmatest_match_device(chan->device))
=======
	struct dmatest_params *params = param;

	if (!dmatest_match_channel(params, chan) ||
	    !dmatest_match_device(params, chan->device))
>>>>>>> refs/remotes/origin/master
		return false;
	else
		return true;
}

<<<<<<< HEAD
static int __init dmatest_init(void)
{
	dma_cap_mask_t mask;
	struct dma_chan *chan;
	int err = 0;

	dma_cap_zero(mask);
	dma_cap_set(DMA_MEMCPY, mask);
	for (;;) {
		chan = dma_request_channel(mask, filter, NULL);
		if (chan) {
			err = dmatest_add_channel(chan);
			if (err) {
=======
static void request_channels(struct dmatest_info *info,
			     enum dma_transaction_type type)
{
	dma_cap_mask_t mask;

	dma_cap_zero(mask);
	dma_cap_set(type, mask);
	for (;;) {
		struct dmatest_params *params = &info->params;
		struct dma_chan *chan;

		chan = dma_request_channel(mask, filter, params);
		if (chan) {
			if (dmatest_add_channel(info, chan)) {
>>>>>>> refs/remotes/origin/master
				dma_release_channel(chan);
				break; /* add_channel failed, punt */
			}
		} else
			break; /* no more channels available */
<<<<<<< HEAD
		if (max_channels && nr_channels >= max_channels)
			break; /* we have all we need */
	}

	return err;
}
/* when compiled-in wait for drivers to load first */
late_initcall(dmatest_init);

static void __exit dmatest_exit(void)
=======
		if (params->max_channels &&
		    info->nr_channels >= params->max_channels)
			break; /* we have all we need */
	}
}

static void run_threaded_test(struct dmatest_info *info)
{
	struct dmatest_params *params = &info->params;

	/* Copy test parameters */
	params->buf_size = test_buf_size;
	strlcpy(params->channel, strim(test_channel), sizeof(params->channel));
	strlcpy(params->device, strim(test_device), sizeof(params->device));
	params->threads_per_chan = threads_per_chan;
	params->max_channels = max_channels;
	params->iterations = iterations;
	params->xor_sources = xor_sources;
	params->pq_sources = pq_sources;
	params->timeout = timeout;
	params->noverify = noverify;

	request_channels(info, DMA_MEMCPY);
	request_channels(info, DMA_XOR);
	request_channels(info, DMA_PQ);
}

static void stop_threaded_test(struct dmatest_info *info)
>>>>>>> refs/remotes/origin/master
{
	struct dmatest_chan *dtc, *_dtc;
	struct dma_chan *chan;

<<<<<<< HEAD
	list_for_each_entry_safe(dtc, _dtc, &dmatest_channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		dmatest_cleanup_channel(dtc);
		pr_debug("dmatest: dropped channel %s\n",
			 dma_chan_name(chan));
		dma_release_channel(chan);
	}
=======
	list_for_each_entry_safe(dtc, _dtc, &info->channels, node) {
		list_del(&dtc->node);
		chan = dtc->chan;
		dmatest_cleanup_channel(dtc);
		pr_debug("dropped channel %s\n", dma_chan_name(chan));
		dma_release_channel(chan);
	}

	info->nr_channels = 0;
}

static void restart_threaded_test(struct dmatest_info *info, bool run)
{
	/* we might be called early to set run=, defer running until all
	 * parameters have been evaluated
	 */
	if (!info->did_init)
		return;

	/* Stop any running test first */
	stop_threaded_test(info);

	/* Run test with new parameters */
	run_threaded_test(info);
}

static int dmatest_run_get(char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	if (is_threaded_test_run(info)) {
		dmatest_run = true;
	} else {
		stop_threaded_test(info);
		dmatest_run = false;
	}
	mutex_unlock(&info->lock);

	return param_get_bool(val, kp);
}

static int dmatest_run_set(const char *val, const struct kernel_param *kp)
{
	struct dmatest_info *info = &test_info;
	int ret;

	mutex_lock(&info->lock);
	ret = param_set_bool(val, kp);
	if (ret) {
		mutex_unlock(&info->lock);
		return ret;
	}

	if (is_threaded_test_run(info))
		ret = -EBUSY;
	else if (dmatest_run)
		restart_threaded_test(info, dmatest_run);

	mutex_unlock(&info->lock);

	return ret;
}

static int __init dmatest_init(void)
{
	struct dmatest_info *info = &test_info;
	struct dmatest_params *params = &info->params;

	if (dmatest_run) {
		mutex_lock(&info->lock);
		run_threaded_test(info);
		mutex_unlock(&info->lock);
	}

	if (params->iterations && wait)
		wait_event(thread_wait, !is_threaded_test_run(info));

	/* module parameters are stable, inittime tests are started,
	 * let userspace take over 'run' control
	 */
	info->did_init = true;

	return 0;
}
/* when compiled-in wait for drivers to load first */
late_initcall(dmatest_init);

static void __exit dmatest_exit(void)
{
	struct dmatest_info *info = &test_info;

	mutex_lock(&info->lock);
	stop_threaded_test(info);
	mutex_unlock(&info->lock);
>>>>>>> refs/remotes/origin/master
}
module_exit(dmatest_exit);

MODULE_AUTHOR("Haavard Skinnemoen (Atmel)");
MODULE_LICENSE("GPL v2");
