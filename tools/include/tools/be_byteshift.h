#ifndef _TOOLS_BE_BYTESHIFT_H
#define _TOOLS_BE_BYTESHIFT_H

<<<<<<< HEAD
#include <linux/types.h>

static inline __u16 __get_unaligned_be16(const __u8 *p)
=======
#include <stdint.h>

static inline uint16_t __get_unaligned_be16(const uint8_t *p)
>>>>>>> refs/remotes/origin/master
{
	return p[0] << 8 | p[1];
}

<<<<<<< HEAD
static inline __u32 __get_unaligned_be32(const __u8 *p)
=======
static inline uint32_t __get_unaligned_be32(const uint8_t *p)
>>>>>>> refs/remotes/origin/master
{
	return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

<<<<<<< HEAD
static inline __u64 __get_unaligned_be64(const __u8 *p)
{
	return (__u64)__get_unaligned_be32(p) << 32 |
	       __get_unaligned_be32(p + 4);
}

static inline void __put_unaligned_be16(__u16 val, __u8 *p)
=======
static inline uint64_t __get_unaligned_be64(const uint8_t *p)
{
	return (uint64_t)__get_unaligned_be32(p) << 32 |
	       __get_unaligned_be32(p + 4);
}

static inline void __put_unaligned_be16(uint16_t val, uint8_t *p)
>>>>>>> refs/remotes/origin/master
{
	*p++ = val >> 8;
	*p++ = val;
}

<<<<<<< HEAD
static inline void __put_unaligned_be32(__u32 val, __u8 *p)
=======
static inline void __put_unaligned_be32(uint32_t val, uint8_t *p)
>>>>>>> refs/remotes/origin/master
{
	__put_unaligned_be16(val >> 16, p);
	__put_unaligned_be16(val, p + 2);
}

<<<<<<< HEAD
static inline void __put_unaligned_be64(__u64 val, __u8 *p)
=======
static inline void __put_unaligned_be64(uint64_t val, uint8_t *p)
>>>>>>> refs/remotes/origin/master
{
	__put_unaligned_be32(val >> 32, p);
	__put_unaligned_be32(val, p + 4);
}

<<<<<<< HEAD
static inline __u16 get_unaligned_be16(const void *p)
{
	return __get_unaligned_be16((const __u8 *)p);
}

static inline __u32 get_unaligned_be32(const void *p)
{
	return __get_unaligned_be32((const __u8 *)p);
}

static inline __u64 get_unaligned_be64(const void *p)
{
	return __get_unaligned_be64((const __u8 *)p);
}

static inline void put_unaligned_be16(__u16 val, void *p)
=======
static inline uint16_t get_unaligned_be16(const void *p)
{
	return __get_unaligned_be16((const uint8_t *)p);
}

static inline uint32_t get_unaligned_be32(const void *p)
{
	return __get_unaligned_be32((const uint8_t *)p);
}

static inline uint64_t get_unaligned_be64(const void *p)
{
	return __get_unaligned_be64((const uint8_t *)p);
}

static inline void put_unaligned_be16(uint16_t val, void *p)
>>>>>>> refs/remotes/origin/master
{
	__put_unaligned_be16(val, p);
}

<<<<<<< HEAD
static inline void put_unaligned_be32(__u32 val, void *p)
=======
static inline void put_unaligned_be32(uint32_t val, void *p)
>>>>>>> refs/remotes/origin/master
{
	__put_unaligned_be32(val, p);
}

<<<<<<< HEAD
static inline void put_unaligned_be64(__u64 val, void *p)
=======
static inline void put_unaligned_be64(uint64_t val, void *p)
>>>>>>> refs/remotes/origin/master
{
	__put_unaligned_be64(val, p);
}

#endif /* _TOOLS_BE_BYTESHIFT_H */
