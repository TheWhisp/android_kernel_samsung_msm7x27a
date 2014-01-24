extern void	pstore_set_kmsg_bytes(int);
<<<<<<< HEAD
extern void	pstore_get_records(void);
extern int	pstore_mkfile(enum pstore_type_id, char *psname, u64 id,
			      char *data, size_t size,
			      struct timespec time, int (*erase)(u64));
=======
extern void	pstore_get_records(int);
extern int	pstore_mkfile(enum pstore_type_id, char *psname, u64 id,
			      char *data, size_t size,
			      struct timespec time, struct pstore_info *psi);
>>>>>>> refs/remotes/origin/cm-10.0
extern int	pstore_is_mounted(void);
