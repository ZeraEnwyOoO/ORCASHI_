#include "dht_bootstrap.h"
#include "dht_core.h"
#include "dht_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define BOOTSTRAP_NODES 6

BootstrapNode bootstrap_nodes[BOOTSTRAP_NODES] = {
    {"router.bittorrent.com", 6881, ""},
    {"dht.transmissionbt.com", 6881, ""},
    {"router.utorrent.com", 6881, ""},
    {"dht.aelitis.com", 6881, ""},
    {"bootstrap.jami.net", 6881, ""},
    {"dht.libtorrent.org", 6881, ""}
};

static const char *fallback_ips[BOOTSTRAP_NODES] = {
    "83.236.216.176",
    "162.159.192.33",
    "104.244.79.180",
    "5.45.84.215",
    "51.222.100.206",
    "144.217.249.33"
};

/* ============================================================================
 * RESOLVE HELPER
 * ============================================================================ */

static int resolve_host(const char *host, char *ipbuf, size_t bufsize) {
    struct addrinfo hints, *res, *rp;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(host, NULL, &hints, &res);
    if (status != 0) return -1;

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        struct sockaddr_in *addr = (struct sockaddr_in*)rp->ai_addr;
        void *sin_addr = &addr->sin_addr;
        if (inet_ntop(AF_INET, sin_addr, ipbuf, bufsize) != NULL) {
            freeaddrinfo(res);
            return 0;
        }
    }
    freeaddrinfo(res);
    return -1;
}

/* ============================================================================
 * BOOTSTRAP FUNCTIONS
 * ============================================================================ */

void dht_bootstrap_init(void) {
    printf("[BOOTSTRAP] Loading %d bootstrap nodes...\n", BOOTSTRAP_NODES);
    
    for (int i = 0; i < BOOTSTRAP_NODES; i++) {
        if (resolve_host(bootstrap_nodes[i].host, bootstrap_nodes[i].ip, 
                        sizeof(bootstrap_nodes[i].ip)) == 0) {
            printf("[BOOTSTRAP] %s -> %s\n", bootstrap_nodes[i].host, bootstrap_nodes[i].ip);
        } else {
            printf("[BOOTSTRAP] DNS failed for %s, using fallback\n", bootstrap_nodes[i].host);
            strcpy(bootstrap_nodes[i].ip, fallback_ips[i]);
            printf("[BOOTSTRAP] %s -> %s (fallback)\n", bootstrap_nodes[i].host, bootstrap_nodes[i].ip);
        }
    }
}

void dht_bootstrap_connect(int dht_socket) {
    (void)dht_socket;
    
    usleep(100000);
    
    printf("[BOOTSTRAP] Connecting DHT to bootstrap nodes...\n");
    
    for (int i = 0; i < BOOTSTRAP_NODES; i++) {
        if (strlen(bootstrap_nodes[i].ip) > 0) {
            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(bootstrap_nodes[i].port);
            inet_pton(AF_INET, bootstrap_nodes[i].ip, &addr.sin_addr);
            
            printf("[BOOTSTRAP] Inserting node %s:%d\n", 
                   bootstrap_nodes[i].ip, bootstrap_nodes[i].port);
            
            /* Generate a dummy ID from IP + port */
            unsigned char dummy_id[20];
            char seed[64];
            snprintf(seed, sizeof(seed), "%s:%d:%ld", 
                     bootstrap_nodes[i].ip, bootstrap_nodes[i].port, 
                     (long)time(NULL));
            dht_hash(dummy_id, 20, seed, strlen(seed), NULL, 0, NULL, 0);
            
            int ret = dht_insert_node(dummy_id, (struct sockaddr*)&addr, sizeof(addr));
            if (ret < 0) {
                printf("[BOOTSTRAP] Failed to insert node %s:%d\n", 
                       bootstrap_nodes[i].ip, bootstrap_nodes[i].port);
            }
        }
    }
}

int dht_bootstrap_get_node(int index, BootstrapNode *node) {
    if (index < 0 || index >= BOOTSTRAP_NODES || !node) return -1;
    *node = bootstrap_nodes[index];
    return 0;
}

int dht_bootstrap_count(void) {
    return BOOTSTRAP_NODES;
}
