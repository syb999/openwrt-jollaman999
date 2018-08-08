/*
 **************************************************************************
 * Copyright (c) 2014, 2017, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

/*
 * nss_ipv4_reasm.c
 *	NSS IPv4 Reassembly APIs
 */
#include "nss_tx_rx_common.h"
#include "nss_stats.h"
#include "nss_ipv4_reasm_stats.h"

/*
 * nss_ipv4_reasm_msg_handler()
 *	Handle NSS -> HLOS messages for IPv4 reasm
 */
static void nss_ipv4_reasm_msg_handler(struct nss_ctx_instance *nss_ctx, struct nss_cmn_msg *ncm, __attribute__((unused))void *app_data)
{
	struct nss_ipv4_reasm_msg *nim = (struct nss_ipv4_reasm_msg *)ncm;

	BUG_ON(ncm->interface != NSS_IPV4_REASM_INTERFACE);

	/*
	 * Handle deprecated messages.  Eventually these messages should be removed.
	 */
	switch (nim->cm.type) {
	case NSS_IPV4_REASM_STATS_SYNC_MSG:
		/*
		* Update driver statistics on node sync.
		*/
		nss_ipv4_reasm_stats_sync(nss_ctx, &nim->msg.stats_sync);
		break;
	default:
		nss_warning("IPv4 reasm received an unknown message type");
	}
}

/*
 * nss_ipv4_reasm_get_context()
 * 	get NSS context instance for ipv4 reassembly
 */
struct nss_ctx_instance *nss_ipv4_reasm_get_context(void)
{
	return &nss_top_main.nss[nss_top_main.ipv4_reasm_handler_id];
}
EXPORT_SYMBOL(nss_ipv4_reasm_get_context);

/*
 * nss_ipv4_reasm_register_handler()
 *	Register our handler to receive messages for this interface
 */
void nss_ipv4_reasm_register_handler(void)
{
	struct nss_ctx_instance *nss_ctx = nss_ipv4_reasm_get_context();

	if (nss_core_register_handler(nss_ctx, NSS_IPV4_REASM_INTERFACE, nss_ipv4_reasm_msg_handler, NULL) != NSS_CORE_STATUS_SUCCESS) {
		nss_warning("IPv4 reasm handler failed to register");
	}

	nss_ipv4_reasm_stats_dentry_create();
}
