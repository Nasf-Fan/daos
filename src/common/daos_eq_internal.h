/*
 * SPECIAL LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of Contract No. B599860,
 * and the terms of the LGPL License.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */
/*
 * LGPL HEADER START
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * (C) Copyright 2012, 2013 Intel Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 or (at your discretion) any later version.
 * (LGPL) version 2.1 accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * LGPL HEADER END
 */
/*
 * common/daos_eq_internal.h
 *
 * Author: Liang Zhen  <liang.zhen@intel.com>
 */

#ifndef EVENT_INTERNAL_H
#define EVENT_INTERNAL_H

#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <pthread.h>
#include <daos_common.h>
#include <daos_ev.h>
#include <daos_list.h>
#include <daos_hash.h>

struct daos_event_private {
	daos_handle_t		evx_eqh;
	daos_list_t		evx_link;
	struct daos_hlink	evx_eq_hlink;
	/** children list */
	daos_list_t		evx_child;
	unsigned int		evx_nchild;
	unsigned int		evx_nchild_if;
	unsigned int		evx_nchild_comp;

	struct daos_event_ops	evx_ops;
	daos_ev_status_t	evx_status;
	struct daos_event_private *evx_parent;
	void			*evx_arg;
};

static inline struct daos_event_private *
daos_ev2evx(struct daos_event *ev)
{
	return (struct daos_event_private *)&ev->ev_private;
}

static inline struct daos_event *
daos_evx2ev(struct daos_event_private *evx)
{
	return container_of(evx, struct daos_event, ev_private);
}

struct daos_eq_private {
	/* link chain in the global hash list */
	struct daos_hlink	eqx_hlink;
	pthread_mutex_t		eqx_lock;
	pthread_cond_t		eqx_cond;
	int			eqx_lock_init:1,
				eqx_cond_init:1,
				eqx_finalizing:1,
				eqx_refcount;

	/* All of its events are linked here */
	struct daos_hhash	*eqx_events_hash;
};

static inline struct daos_eq_private *
daos_eq2eqx(struct daos_eq *eq)
{
	return (struct daos_eq_private *)&eq->eq_private;
}

static inline struct daos_eq *
daos_eqx2eq(struct daos_eq_private *eqx)
{
	return container_of(eqx, struct daos_eq, eq_private);
}

int daos_event_launch(struct daos_event *ev);
void daos_event_complete(struct daos_event *ev);

struct daos_data {
	struct daos_hhash	*daos_hhash;
	int			daos_info_fd;
	int			daos_pid;
	int			daos_info_init:1;
	int			daos_lock_init:1;
	pthread_mutex_t		daos_lock;
	daos_list_t		*daos_fd_hash;
};

extern struct daos_data		daos_data;
#endif /* __EVENT_INTERNAL_H__ */