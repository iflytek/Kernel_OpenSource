/*
 * Copyright (C) 2020 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef SPRD_ACIIONS_QUEUE_H
#define SPRD_ACIIONS_QUEUE_H

typedef void (*do_actions) (int action, void *data);

void *sprd_create_action_queue(char *name, do_actions do_function, void *data);
void sprd_destroy_action_queue(void *action_queue);
int sprd_add_action(void *actions_queue, int action);
#endif

