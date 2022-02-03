/* http://www.martinbroadhurst.com/dynamic-array-in-c.html */

#include <viv/dynamic_array.h>

#include <stdlib.h>
#include <string.h>

#define START_SIZE 4 /* Initial buffer size if not specified */

VIV_DYNAMIC_ARRAY_dynamic_array *VIV_DYNAMIC_ARRAY_create(unsigned int size) {
  VIV_DYNAMIC_ARRAY_dynamic_array *array = malloc(
    sizeof(VIV_DYNAMIC_ARRAY_dynamic_array)
  );

  if (array != NULL) {
    if (size) {
      array->buffer = malloc(size * sizeof(void *));
      if (array->buffer) {
        array->size = size;
      } else {
        free(array);
        array = NULL;
      }
    } else {
      array->buffer = NULL;
      array->size = 0;
    }

    array->count = 0;
  }

  return array;
}

void VIV_DYNAMIC_ARRAY_empty(VIV_DYNAMIC_ARRAY_dynamic_array *array) {
  array->count = 0;
}

void VIV_DYNAMIC_ARRAY_delete(VIV_DYNAMIC_ARRAY_dynamic_array *array) {
  if (array) {
    free(array->buffer);
    free(array);
  }
}

void VIV_DYNAMIC_ARRAY_add_tail(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  void *data
) {
  if (array->count == array->size) {
    /* No more space */
    if (array->buffer != NULL) {
      void **buffer = realloc(
        array->buffer,
        array->size * 2 * sizeof(void *)
      );
      array->buffer = buffer;
      array->size *= 2;
    } else {
      array->buffer = malloc(START_SIZE * sizeof(void *));
      array->size = START_SIZE;
    }
  }
  if (array->buffer != NULL) {
    array->buffer[array->count] = data;
    array->count++;
  }
}

void VIV_DYNAMIC_ARRAY_add_head(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  void *data
) {
  if (array->count == array->size) {
    /* No more space */
    if (array->buffer != NULL) {
      void **temp = malloc(array->size * 2 * sizeof(void *));
      if (temp) {
        /* Copy the elements one space to the right */
        memcpy(temp + 1, array->buffer, array->count * sizeof(void *));
        free(array->buffer);
        array->buffer = temp;
        array->size *= 2;
      }
    } else {
      array->buffer = malloc(START_SIZE * sizeof(void *));
      if (array->buffer) {
        array->size = START_SIZE;
      }
    }
  } else {
    /* Move the elements one space to the right */
    memmove(
      array->buffer + 1,
      array->buffer,
      array->count * sizeof(void *)
    );
  }
  if (array->buffer != NULL) {
    array->buffer[0] = data;
    array->count++;
  }
}

void *VIV_DYNAMIC_ARRAY_remove_tail(VIV_DYNAMIC_ARRAY_dynamic_array *array) {
  void *data = NULL;

  if (array->count > 0) {
    data = array->buffer[array->count - 1];
    array->count--;
  }

  return data;
}

void *VIV_DYNAMIC_ARRAY_remove_head(VIV_DYNAMIC_ARRAY_dynamic_array *array) {
  void *data = NULL;

  if (array->count > 0) {
    data = array->buffer[0];

    /* Move the elements one space to the left */
    memmove(
      array->buffer,
      array->buffer + 1,
      (array->count - 1) * sizeof(void *)
    );

    array->count--;
  }

  return data;
}

void VIV_DYNAMIC_ARRAY_insert(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  unsigned int pos,
  void *data
) {
  if (pos == 0) {
    VIV_DYNAMIC_ARRAY_add_head(array, data);
  } else if (pos == array->count) {
    VIV_DYNAMIC_ARRAY_add_tail(array, data);
  } else if (pos < array->count) {
    /* unsigned int i; */

    if (array->count == array->size) {
      /* Reallocate the buffer and copy, leaving a space */
      void **temp = malloc(array->size * 2 * sizeof(void *));
      if (temp) {
        memcpy(temp, array->buffer, pos * sizeof(void *));
        memcpy(
          temp + pos + 1,
          array->buffer + pos,
          (array->count - pos) * sizeof(void *)
        );
        free(array->buffer);
        array->buffer = temp;
        array->size *= 2;
      }
    } else {
      /* Move the elements after to the right */
      memmove(
        array->buffer + pos + 1, array->buffer + pos,
        (array->count - pos) *sizeof(void *)
        );
    }

    array->buffer[pos] = data;
    array->count++;
  }
}

void *VIV_DYNAMIC_ARRAY_remove(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  unsigned int pos
) {
  void *data;

  if (array->count < pos + 1) {
    data = NULL;
  } else if (pos == 0) {
    data = VIV_DYNAMIC_ARRAY_remove_head(array);
  } else if (pos == array->count - 1) {
    data = VIV_DYNAMIC_ARRAY_remove_tail(array);
  } else {
    /* unsigned int i; */

    data = array->buffer[pos];

    /* Move the following elements left */
    memmove(
      array->buffer + pos, array->buffer + pos + 1,
      (array->count - pos - 1) * sizeof(void *)
    );

    array->count--;
  }

  return data;
}

void *VIV_DYNAMIC_ARRAY_get(
  const VIV_DYNAMIC_ARRAY_dynamic_array *array,
  unsigned int pos
) {
  void *data = NULL;

  if (pos < array->count) {
    data = array->buffer[pos];
  }

  return data;
}

void *VIV_DYNAMIC_ARRAY_set(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  unsigned int pos,
  void *data
) {
  void *temp = NULL;

  if (pos == array->count) {
    VIV_DYNAMIC_ARRAY_add_tail(array, data);
  } else if (pos < array->count) {
    temp = array->buffer[pos];
    array->buffer[pos] = data;
  }

  return temp;
}

void VIV_DYNAMIC_ARRAY_set_size(
  VIV_DYNAMIC_ARRAY_dynamic_array *array,
  unsigned int size
) {
  array->buffer = realloc(array->buffer, size);

  if (array->buffer) {
    array->size = size;

    if (array->size < array->count) {
      array->count = array->size;
    }
  } else {
    array->size = 0;
    array->count = 0;
  }
}

void VIV_DYNAMIC_ARRAY_for_each(
  const VIV_DYNAMIC_ARRAY_dynamic_array *array,
  VIV_DYNAMIC_ARRAY_for_function function
) {
  unsigned int i;

  for (i = 0; i < array->count; ++i) {
    function(array->buffer[i]);
  }
}

unsigned int VIV_DYNAMIC_ARRAY_get_count(
  const VIV_DYNAMIC_ARRAY_dynamic_array *array
) {
  return array->count;
}
