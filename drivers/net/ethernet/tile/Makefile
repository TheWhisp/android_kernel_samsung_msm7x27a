#
# Makefile for the TILE on-chip networking support.
#

obj-$(CONFIG_TILE_NET) += tile_net.o
ifdef CONFIG_TILEGX
<<<<<<< HEAD
tile_net-objs := tilegx.o mpipe.o iorpc_mpipe.o dma_queue.o
else
tile_net-objs := tilepro.o
=======
tile_net-y := tilegx.o
else
tile_net-y := tilepro.o
>>>>>>> refs/remotes/origin/master
endif
