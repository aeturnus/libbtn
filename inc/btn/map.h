#ifndef __BTN_MAP_H__
#define __BTN_MAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This provides for the map interface
 */
typedef struct _map_ops
{
    bool    (* insert)  (void * map, const void * key, const void * val);
    bool    (* find)     (void * map, const void * key, void * val);
    void *  (* findp)     (void * map, const void * key);
    bool    (* erase)   (void * map, const void * key);
    size_t  (* size)    (void * map);
    bool    (* empty)   (void * map);
} map_ops;

/**
 * Inserts a value for a given key
 * @param[in]   map     Pointer to the struct implementing map
 * @param[in]   key     Pointer to the key
 * @param[in]   val     Pointer to the value
 * @return  True if successful. Fails if key is already in map
 */
#define map_insert(_map, _key, _val) (_map)->ops->map.insert(_map, _key, _val)

/**
 * Finds a value for a given key, writing it back to val
 * @param[in]   map     Pointer to the struct implementing map
 * @param[in]   key     Pointer to the key
 * @param[in]   val     Pointer to where to write the value to
 * @return  True if successful. Fails if key is already in map
 */
#define map_find(_map, _key, _val) (_map)->ops->map.find(_map, _key, _val)

/**
 * Finds a value for a given key, providing the pointer to the value
 * @param[in]   map     Pointer to the struct implementing map
 * @param[in]   key     Pointer to the key
 * @return  Pointer to the value on success, NULL if failure
 */
#define map_findp(_map, _key) (_map)->ops->map.findp(_map, _key)

/**
 * Erase a certain key
 * @return  True if successful. Fails if key isn't present
 */
#define map_erase(_map, _key) (_map)->ops->map.erase(_map, _key)

/**
 * @return  How many key-value pairs are in the map
 */
#define map_size(_map) (_map)->ops->map.size(_map)

/**
 * @return  True if the map is empty
 */
#define map_empty(map) (map)->ops->map.empty(map)

#ifdef __cplusplus
}
#endif

#endif//__BTN_MAP_H__
