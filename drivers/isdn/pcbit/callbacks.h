/*
 * Callbacks prototypes for FSM
 *
 * Copyright (C) 1996 Universidade de Lisboa
<<<<<<< HEAD
 * 
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of 
=======
 *
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of
>>>>>>> refs/remotes/origin/cm-10.0
 * the GNU General Public License, incorporated herein by reference.
 */

#ifndef CALLBACKS_H
#define CALLBACKS_H


<<<<<<< HEAD
extern void cb_out_1(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		     struct callb_data *data);

extern void cb_out_2(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		     struct callb_data *data);

extern void cb_in_1(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		    struct callb_data *data);
extern void cb_in_2(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		    struct callb_data *data);
extern void cb_in_3(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		    struct callb_data *data);

extern void cb_disc_1(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		      struct callb_data *data);
extern void cb_disc_2(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		      struct callb_data *data);
extern void cb_disc_3(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		      struct callb_data *data);

extern void cb_notdone(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		       struct callb_data *data);

extern void cb_selp_1(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		      struct callb_data *data);
extern void cb_open(struct pcbit_dev * dev, struct pcbit_chan* chan, 
		    struct callb_data *data);

#endif


=======
extern void cb_out_1(struct pcbit_dev *dev, struct pcbit_chan *chan,
		     struct callb_data *data);

extern void cb_out_2(struct pcbit_dev *dev, struct pcbit_chan *chan,
		     struct callb_data *data);

extern void cb_in_1(struct pcbit_dev *dev, struct pcbit_chan *chan,
		    struct callb_data *data);
extern void cb_in_2(struct pcbit_dev *dev, struct pcbit_chan *chan,
		    struct callb_data *data);
extern void cb_in_3(struct pcbit_dev *dev, struct pcbit_chan *chan,
		    struct callb_data *data);

extern void cb_disc_1(struct pcbit_dev *dev, struct pcbit_chan *chan,
		      struct callb_data *data);
extern void cb_disc_2(struct pcbit_dev *dev, struct pcbit_chan *chan,
		      struct callb_data *data);
extern void cb_disc_3(struct pcbit_dev *dev, struct pcbit_chan *chan,
		      struct callb_data *data);

extern void cb_notdone(struct pcbit_dev *dev, struct pcbit_chan *chan,
		       struct callb_data *data);

extern void cb_selp_1(struct pcbit_dev *dev, struct pcbit_chan *chan,
		      struct callb_data *data);
extern void cb_open(struct pcbit_dev *dev, struct pcbit_chan *chan,
		    struct callb_data *data);

#endif
>>>>>>> refs/remotes/origin/cm-10.0
