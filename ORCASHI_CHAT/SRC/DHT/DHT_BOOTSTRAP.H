#ifndef DHT_BOOTSTRAP_H
#define DHT_BOOTSTRAP_H

#include <stddef.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * BOOTSTRAP NODES
 * ============================================================================ */

typedef struct {
    char host[64];
    int port;
    char ip[64];
} BootstrapNode;

extern BootstrapNode bootstrap_nodes[];

/* ============================================================================
 * BOOTSTRAP FUNCTIONS
 * ============================================================================ */

/* Initialize bootstrap (resolve DNS) */
void dht_bootstrap_init(void);

/* Connect to bootstrap nodes */
void dht_bootstrap_connect(int dht_socket);

/* Get bootstrap node by index */
int dht_bootstrap_get_node(int index, BootstrapNode *node);

/* Get number of bootstrap nodes */
int dht_bootstrap_count(void);

#ifdef __cplusplus
}
#endif

#endif /* DHT_BOOTSTRAP_H */
