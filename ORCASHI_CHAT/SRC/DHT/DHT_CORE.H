#ifndef DHT_CORE_H
#define DHT_CORE_H

#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * DHT CALLBACK TYPES
 * ============================================================================ */

typedef void dht_callback_t(void *closure, int event,
                           const unsigned char *info_hash,
                           const void *data, size_t data_len);

#define DHT_EVENT_NONE 0
#define DHT_EVENT_VALUES 1
#define DHT_EVENT_VALUES6 2
#define DHT_EVENT_SEARCH_DONE 3
#define DHT_EVENT_SEARCH_DONE6 4

/* ============================================================================
 * DHT CORE FUNCTIONS
 * ============================================================================ */

/* Initialize DHT with socket and node ID */
int dht_init(int s, int s6, const unsigned char *id, const unsigned char *v);

/* Cleanup DHT */
int dht_uninit(void);

/* Periodic processing - call this regularly */
int dht_periodic(const void *buf, size_t buflen,
                 const struct sockaddr *from, int fromlen, time_t *tosleep,
                 dht_callback_t *callback, void *closure);

/* ============================================================================
 * DHT SEARCH / ANNOUNCE
 * ============================================================================ */

/* Search for peers with given info_hash */
int dht_search(const unsigned char *id, int port, int af,
               dht_callback_t *callback, void *closure);

/* Insert a node into the routing table */
int dht_insert_node(const unsigned char *id, struct sockaddr *sa, int salen);

/* Ping a node */
int dht_ping_node(const struct sockaddr *sa, int salen);

/* ============================================================================
 * DHT QUERY FUNCTIONS
 * ============================================================================ */

/* Get node statistics */
int dht_nodes(int af, int *good_return, int *dubious_return,
              int *cached_return, int *incoming_return);

/* Dump routing table to file */
void dht_dump_tables(FILE *f);

/* Get list of known nodes */
int dht_get_nodes(struct sockaddr_in *sin, int *num,
                  struct sockaddr_in6 *sin6, int *num6);

/* ============================================================================
 * DHT DEBUG
 * ============================================================================ */

void dht_debug_enable(const char *filename);
void dht_debug_disable(void);
extern FILE *dht_debug;

#ifdef __cplusplus
}
#endif

#endif /* DHT_CORE_H */
