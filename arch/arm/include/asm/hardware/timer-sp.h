<<<<<<< HEAD
<<<<<<< HEAD
void sp804_clocksource_init(void __iomem *, const char *);
=======
void __sp804_clocksource_and_sched_clock_init(void __iomem *,
					      const char *, int);

static inline void sp804_clocksource_init(void __iomem *base, const char *name)
{
	__sp804_clocksource_and_sched_clock_init(base, name, 0);
=======
struct clk;

void __sp804_clocksource_and_sched_clock_init(void __iomem *,
					      const char *, struct clk *, int);
void __sp804_clockevents_init(void __iomem *, unsigned int,
			      struct clk *, const char *);

static inline void sp804_clocksource_init(void __iomem *base, const char *name)
{
	__sp804_clocksource_and_sched_clock_init(base, name, NULL, 0);
>>>>>>> refs/remotes/origin/master
}

static inline void sp804_clocksource_and_sched_clock_init(void __iomem *base,
							  const char *name)
{
<<<<<<< HEAD
	__sp804_clocksource_and_sched_clock_init(base, name, 1);
}

>>>>>>> refs/remotes/origin/cm-10.0
void sp804_clockevents_init(void __iomem *, unsigned int, const char *);
=======
	__sp804_clocksource_and_sched_clock_init(base, name, NULL, 1);
}

static inline void sp804_clockevents_init(void __iomem *base, unsigned int irq, const char *name)
{
	__sp804_clockevents_init(base, irq, NULL, name);

}
>>>>>>> refs/remotes/origin/master
