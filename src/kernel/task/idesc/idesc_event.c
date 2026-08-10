/**
 * @file
 *
 * @date Oct 24, 2013
 * @author: Anton Bondarev
 * @author: Anton Kozlov
 */

#include <errno.h>
#include <fcntl.h>

#include <kernel/sched.h>
#include <kernel/sched/waitq.h>
#include <kernel/task/resource/idesc.h>
#include <kernel/task/resource/idesc_event.h>
#include <lib/libds/dlist.h>
#include <mem/misc/pool.h>

int idesc_wait_prepare(struct idesc *idesc, struct idesc_wait_link *wl) {
	waitq_wait_prepare(&idesc->idesc_waitq, &wl->link);

	if (idesc->idesc_dev_waitq) {
		waitq_wait_prepare(idesc->idesc_dev_waitq, &wl->link);
	}

	if (idesc->idesc_flags & O_NONBLOCK) {
		return -EAGAIN;
	}

	return 0;
}

void idesc_wait_cleanup(struct idesc *idesc, struct idesc_wait_link *wl) {
	waitq_wait_cleanup(&idesc->idesc_waitq, &wl->link);

	if (idesc->idesc_dev_waitq) {
		waitq_wait_cleanup(idesc->idesc_dev_waitq, &wl->link);
	}
}

int idesc_notify(struct idesc *idesc, int mask) {
	//TODO MASK
	if (idesc->idesc_dev_waitq) {
		waitq_wakeup(idesc->idesc_dev_waitq, 0);
	}
	else {
		waitq_wakeup(&idesc->idesc_waitq, 0);
	}

	return 0;
}
