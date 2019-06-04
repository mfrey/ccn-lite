/**
 * @f ccnl-cs.h
 * @b CCN lite - Content store implementation
 *
 * Copyright (C) 2018 HAW Hamburg
 * Copyright (C) 2018 MSA Safety 
 * Copyright (C) 2018, 2019 Safety IO
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef CCNL_CS
#define CCNL_CS

#include "ccnl-prefix.h"
#include "ccnl-content.h" // FIXME

#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Provides status codes for content store operations
 * @{
 */
typedef enum {
    CS_OPERATION_UNSUCCESSFUL = 0,   /**< operation was not successfull */
    CS_OPERATION_WAS_SUCCESSFUL = 1, /**< operation was successfull */
    CS_OPTIONS_ARE_NULL = -1,        /**< \ref ccnl_cs_ops_t are NULL */
    CS_NAME_IS_INVALID = -2,         /**< name is invalid or NULL */
    CS_CONTENT_IS_INVALID = -3,      /**< content is invalid or NULL */

    CS_NAME_COULD_NOT_BE_FOUND = -4, /**< a name could not be found */
    CS_PACKET_IS_INVALID = -5,       /**< packet is invalid or NULL */
    
    CS_DO_NOT_USE = INT_MAX          /**< set the enum to a fixed width, do not use! */
} ccnl_cs_status_t;
/** @} */

/**
 * @brief TODO
 * @{
 */
typedef enum {
    CS_MATCH = 0,                  /**< Two prefixes have to match */
    CS_MATCH_EXACT = 1,            /**< Two */
    CS_MATCH_LONGEST = 2,          /**< */
    CS_MATCH_DO_NOT_USE = INT_MAX  /**< set the enum to a fixed width, do not use! */
} ccnl_cs_match_t;
/** @} */

typedef struct {
    unsigned char **components; /**< the components of the prefix without '\0' at the end */
    size_t count;               /**< the number of components */
    size_t *length;             /**< the actual length of the name components */
} ccnl_cs_component_t;

/**
 * @brief An abstract representation of an ICN name 
 */
//typedef struct {
//    uint8_t *name;         /**< the name itself */
//    size_t length;         /**< the length of the name (formerly known as complen) */
//    ccnl_cs_component_t component; /**< */
//} ccnl_cs_name_t;

typedef struct {
    // TODO: ccnl_content_flags flags;
    uint8_t flags;
    uint32_t last_used;          /**< indicates when the stored content was last used */
#ifdef CCNL_RIOT
    evtimer_msg_event_t timeout; /**< event timer message which is triggered when a timeout in the content store occurs */
#endif
} ccnl_cs_content_extra_t;

/**
 * @brief An abstract representation of ICN content
 */
//typedef struct {
//    uint8_t *content;          /**< A byte representation of the content */
//    uint32_t length;           /**< The size of the content */
//    uint32_t served;           /**< denotes how often the content has been served */
//    struct ccnl_pkt_s *packet; /**< a byte representation of received content (the actual packet) */
//    ccnl_cs_content_extra_t options;
//} ccnl_cs_content_t;

/**
 * Type definition for the function pointer to the add function
 */
typedef int (*ccnl_cs_op_add_t)(const ccnl_cs_name_t *name, const ccnl_cs_content_t *content);

/**
 * Type definition for the function pointer to the lookup function
 */
typedef int (*ccnl_cs_op_lookup_t)(const ccnl_cs_name_t *name, ccnl_cs_content_t **content);

/**
 * Type definition for the function pointer to the remove function
 */
typedef int (*ccnl_cs_op_remove_t)(const ccnl_cs_name_t *name);

/**
 * Type definition for the function pointer to the remove function
 */
typedef int (*ccnl_cs_op_clear_t)(void);

/**
 * Type definition for the function pointer to the print function
 */
typedef int (*ccnl_cs_op_print_t)(void);

/**
 * Type definition for the function pointer to the print function
 */
typedef int (*ccnl_cs_op_age_t)(void);

typedef int (*ccnl_cs_op_exists_t)(const ccnl_cs_name_t *name, ccnl_cs_match_t mode);

/**
 * Type definition for the function pointer to the function which removes the 
 * oldest entry in concrete content store implementation
 */
typedef int (*ccnl_cs_op_remove_oldest_t)(void);

/**
 * Type definition for the function pointer to the match interest function
 */
typedef int (*ccnl_cs_op_match_interest_t)(const struct ccnl_pkt_s *packet, ccnl_cs_content_t *content);

/**
 * @brief Holds function pointers to concrete implementations of a content store
 */
typedef struct {
    ccnl_cs_op_add_t add;                           /**< Function pointer to the add function */
    ccnl_cs_op_lookup_t lookup;                     /**< Function pointer to the lookup function */
    ccnl_cs_op_remove_t remove;                     /**< Function pointer to the remove function */
    ccnl_cs_op_clear_t clear;                       /**< Function pointer to the clear function */
    ccnl_cs_op_print_t print;                       /**< Function pointer to the print function */
    ccnl_cs_op_age_t age;                           /**< Function pointer to the ageing function */
    ccnl_cs_op_exists_t exists;                     /**< Function pointer to the exists function */
    ccnl_cs_op_remove_oldest_t remove_oldest_entry; /**< Function pointer to the remove oldest entry function */
    ccnl_cs_op_match_interest_t match_interest;     /**< Function pointer to the match interest function */
} ccnl_cs_ops_t;

/**
 * @brief Sets the given function pointers to \p ops
 *
 * @param[out] ops The data structure to initialize 
 * @param[in] add_fun A function pointer to the add function
 * @param[in] lookup_fun A function pointer to the lookup function
 * @param[in] remove_fun A function pointer to the remove function
 */
void
ccnl_cs_init(ccnl_cs_ops_t *ops,
             ccnl_cs_op_add_t add_fun,
             ccnl_cs_op_lookup_t lookup_fun,
             ccnl_cs_op_remove_t remove_fun,
             ccnl_cs_op_clear_t clear_fun,
             ccnl_cs_op_print_t print_fun,
             ccnl_cs_op_age_t age_fun,
             ccnl_cs_op_exists_t exists_fun,
             ccnl_cs_op_remove_oldest_t oldest_fun,
             ccnl_cs_op_match_interest_t match_interest_fun
             );

/**
 * @brief Adds an item to the content store
 *
 * @param[in] ops Data structure which holds the function pointer to the add function
 * @param[in] name The name of the content
 * @param[in] content The content to add
 *
 * @return 0 The content was added successfully to the content store
 * @return -1 An invalid \ref ccnl_cs_op_t struct was passed to the function (e.g. \p ops is NULL)
 * @return -2 An invalid \ref ccnl_cs_name_t struct was passed to the function (e.g. \p name is NULL)
 * @return -3 An invalid \ref ccnl_cs_content_t struct was passed to the function (e.g. \p content is NULL)
 */
ccnl_cs_status_t
ccnl_cs_add(ccnl_cs_ops_t *ops,
            const ccnl_cs_name_t *name,
            const ccnl_cs_content_t *content);

/**
 * @brief Searches the content store for the specified item
 *
 * @param[in] ops Data structure which holds the function pointer to the lookup function
 * @param[in] name The name of the content which is about to searched in the content store
 * @param[out] content If the lookup was successfull, the variable contains the result
 *
 * @return 0 The content could not be found in the content store
 * @return 1 The content was found in the content store
 * @return -1 An invalid \ref ccnl_cs_op_t struct was passed to the function (e.g. \p ops is NULL)
 * @return -2 An invalid \ref ccnl_cs_name_t struct was passed to the function (e.g. \p name is NULL)
 * @return -3 An invalid \ref ccnl_cs_content_t struct was passed to the function (e.g. \p content is NULL)
 */
ccnl_cs_status_t
ccnl_cs_lookup(ccnl_cs_ops_t *ops,
               const ccnl_cs_name_t *name,
               ccnl_cs_content_t **content);

/**
 * @brief Removes an item from the content store 
 *
 * @param[in] ops Data structure which holds the function pointer to the remove function
 * @param[in] name The name of the content to be removed
 *
 * @return 0 The content was removed successfully from the content store
 * @return -1 An invalid \ref ccnl_cs_op_t struct was passed to the function (e.g. \p ops is NULL)
 * @return -2 An invalid \ref ccnl_cs_name_t struct was passed to the function (e.g. \p name is NULL)
 */
ccnl_cs_status_t
ccnl_cs_remove(ccnl_cs_ops_t *ops,
               const ccnl_cs_name_t *name);

ccnl_cs_status_t
ccnl_cs_exists(ccnl_cs_ops_t *ops,
               const ccnl_cs_name_t *name,
               ccnl_cs_match_t mode);

/**
 * @brief Removes all elements from the content store
 * 
 * @param[in] ops Data structure which holds the function pointer to the clear function
 * 
 * @return 0 All content was removed successfully from the content store
 * @return -1 An invalid \ref ccnl_cs_op_t struct was passed to the function (e.g. \p ops is NULL)
 */
ccnl_cs_status_t
ccnl_cs_clear(ccnl_cs_ops_t *ops);


/**
 * @brief Allows to set the maxmium size of the content store
 * 
 * @param[in] size The new size of the content store
 */
void ccnl_cs_set_cs_capacity(size_t size);

/**
 * @brief Returns the maximum size of the content store
 *
 * @return The maximum size of the content store
 */
size_t ccnl_cs_get_cs_capacity(void);

/**
 * @brief Returns the current size of the content store
 *
 * @return The current size of the content store
 */
size_t ccnl_cs_get_cs_current_size(void);

/**
 * @brief Checks if an Interest packet matches an content object in the content store
 *
 * @param[in] ops Data structure which holds the function pointer to the clear function
 * @param[in] packet  The Interest packet (and thus the prefix, to lookup)
 * @param[out] content If successfull, the pointer points to the content found in the content store
 *
 * @return 0 Content which matches the Interest was found in the content store
 * @return -1 An invalid \ref ccnl_cs_op_t struct was passed to the function (e.g. \p ops is NULL)
 */
ccnl_cs_status_t
ccnl_cs_match_interest(ccnl_cs_ops_t *ops, const struct ccnl_pkt_s *packet, ccnl_cs_content_t *content);


ccnl_cs_status_t
ccnl_cs_remove_oldest_entry(ccnl_cs_ops_t *ops);

ccnl_cs_status_t
ccnl_cs_print(ccnl_cs_ops_t *ops);

ccnl_cs_status_t
ccnl_cs_age(ccnl_cs_ops_t *ops);

#endif //CCNL_CS
