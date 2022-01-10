/* http://www.martinbroadhurst.com/dynamic-array-in-c.html */

#ifndef VIV_DYNAMIC_ARRAY_H
#define VIV_DYNAMIC_ARRAY_H

#pragma once

typedef struct {
  void **buffer;
  unsigned int size;
  unsigned int count;
} VIV_DYNAMIC_ARRAY_dynamic_array;

typedef void (*VIV_DYNAMIC_ARRAY_for_function)(void *);

VIV_DYNAMIC_ARRAY_dynamic_array *VIV_DYNAMIC_ARRAY_create(unsigned int);
void VIV_DYNAMIC_ARRAY_empty(VIV_DYNAMIC_ARRAY_dynamic_array *);
void VIV_DYNAMIC_ARRAY_delete(VIV_DYNAMIC_ARRAY_dynamic_array *);
void VIV_DYNAMIC_ARRAY_add_tail(VIV_DYNAMIC_ARRAY_dynamic_array *, void *);
void VIV_DYNAMIC_ARRAY_add_head(VIV_DYNAMIC_ARRAY_dynamic_array *, void *);
void *VIV_DYNAMIC_ARRAY_remove_tail(VIV_DYNAMIC_ARRAY_dynamic_array *);
void *VIV_DYNAMIC_ARRAY_remove_head(VIV_DYNAMIC_ARRAY_dynamic_array *);
void VIV_DYNAMIC_ARRAY_insert(VIV_DYNAMIC_ARRAY_dynamic_array *, unsigned int, void *);
void *VIV_DYNAMIC_ARRAY_remove(VIV_DYNAMIC_ARRAY_dynamic_array *, unsigned int);
void *VIV_DYNAMIC_ARRAY_get(const VIV_DYNAMIC_ARRAY_dynamic_array *, unsigned int);
void *VIV_DYNAMIC_ARRAY_set(VIV_DYNAMIC_ARRAY_dynamic_array *, unsigned int, void *);
void VIV_DYNAMIC_ARRAY_for_each(
  const VIV_DYNAMIC_ARRAY_dynamic_array *,
  VIV_DYNAMIC_ARRAY_for_function
);
unsigned int VIV_DYNAMIC_ARRAY_get_count(const VIV_DYNAMIC_ARRAY_dynamic_array *);
void VIV_DYNAMIC_ARRAY_set_size(VIV_DYNAMIC_ARRAY_dynamic_array *, unsigned int);

#endif /* VIV_DYNAMIC_ARRAY_H */
