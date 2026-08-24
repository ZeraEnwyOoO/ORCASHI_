#ifndef DHT_UTILS_H
#define DHT_UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * HASH FUNCTIONS
 * ============================================================================ */

/* Generate DHT hash (20 bytes) from multiple data sources */
void dht_hash(void *hash_return, int hash_size,
              const void *v1, int len1,
              const void *v2, int len2,
              const void *v3, int len3);

/* Compare two DHT IDs */
int dht_compare_ids(const unsigned char *id1, const unsigned char *id2);

/* Convert ID to string */
void dht_id_to_string(const unsigned char *id, char *buf, int buflen);

/* Convert string to ID */
int dht_string_to_id(const char *str, unsigned char *id);

/* ============================================================================
 * RANDOM FUNCTIONS
 * ============================================================================ */

/* Generate random bytes */
int dht_random_bytes(void *buf, size_t size);

/* ============================================================================
 * BLACKLIST
 * ============================================================================ */

/* Check if address is blacklisted */
int dht_blacklisted(const struct sockaddr *sa, int salen);

/* Add address to blacklist */
void dht_blacklist_add(const struct sockaddr *sa, int salen);

/* Clear blacklist */
void dht_blacklist_clear(void);

/* Check blacklist */
int dht_blacklist_check(const struct sockaddr *sa, int salen);

/* Get blacklist count */
int dht_blacklist_get_count(void);

/* Get blacklist entry */
int dht_blacklist_get_entry(int index, struct sockaddr_storage *addr, int *addrlen);

/* ============================================================================
 * SEND HELPER
 * ============================================================================ */

/* Send UDP packet with error checking */
int dht_sendto(int sockfd, const void *buf, int len, int flags,
               const struct sockaddr *to, int tolen);

#ifdef __cplusplus
}
#endif

#endif /* DHT_UTILS_H */
