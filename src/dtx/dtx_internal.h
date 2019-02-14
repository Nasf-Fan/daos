/**
 * (C) Copyright 2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */
/**
 * dtx: dtx internal.h
 *
 */
#ifndef __DTX_INTERNAL_H__
#define __DTX_INTERNAL_H__

#include <uuid/uuid.h>
#include <daos/rpc.h>
#include <daos/btree.h>

/*
 * RPC operation codes
 *
 * These are for daos_rpc::dr_opc and DAOS_RPC_OPCODE(opc, ...) rather than
 * crt_req_create(..., opc, ...). See src/include/daos/rpc.h.
 */
#define DAOS_DTX_VERSION	1

/* LIST of internal RPCS in form of:
 * OPCODE, flags, FMT, handler, corpc_hdlr,
 */
#define DTX_PROTO_SRV_RPC_LIST						\
	X(DTX_COMMIT, 0, &CQF_dtx, dtx_handler, NULL),			\
	X(DTX_ABORT, 0, &CQF_dtx, dtx_handler, NULL),			\
	X(DTX_CHECK, 0, &CQF_dtx, dtx_handler, NULL)

#define X(a, b, c, d, e) a

enum dtx_operation {
	DTX_PROTO_SRV_RPC_LIST,
};

#undef X

/* DTX RPC input fields */
#define DAOS_ISEQ_DTX							\
	((uuid_t)		(di_po_uuid)		CRT_VAR)	\
	((uuid_t)		(di_co_uuid)		CRT_VAR)	\
	((struct daos_tx_id)	(di_dtx_array)		CRT_ARRAY)

/* DTX RPC output fields */
#define DAOS_OSEQ_DTX							\
	((int32_t)		(do_status)		CRT_VAR)

CRT_RPC_DECLARE(dtx, DAOS_ISEQ_DTX, DAOS_OSEQ_DTX);

extern struct crt_proto_format dtx_proto_fmt;
extern btr_ops_t dbtree_dtx_cf_ops;

int dtx_check(uuid_t po_uuid, uuid_t co_uuid,
	      struct daos_tx_entry *dte, struct pl_obj_layout *layout);

#endif /* __DTX_INTERNAL_H__ */
