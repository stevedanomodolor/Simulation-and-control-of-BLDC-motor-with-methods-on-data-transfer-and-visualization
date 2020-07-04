/*
 *
 * @file arrow_test.c
 * @brief Implementation of arrow_offerer
 *
 * This file is the implementation of the arrow_test. It creats data and converts it to the arrow format and displays the same data.
 * connection and open the datachannel.
 * Copyright Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a robot
 * Tutor : Herman Bruyninckx
 */
 // 
 // Licensed under the Apache License, Version 2.0 (the "License");
 // you may not use this file except in compliance with the License.
 // You may obtain a copy of the License at
 //
 // http://www.apache.org/licenses/LICENSE-2.0
 //
 // Unless required by applicable law or agreed to in writing, software
 // distributed under the License is distributed on an "AS IS" BASIS,
 // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 // See the License for the specific language governing permissions and
 // limitations under the License.
 //

#include <arrow-glib/arrow-glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// typedef struct {
//   float x;
//   float y;
//   float z;
// } position_t;

void create_arrow_buffer( gconstpointer * data_raw, gsize *data_size )
{
  GError *error = NULL;
  // First we create the array for each of the members of the position structure object
  GArrowArray * x_array;
  GArrowArray * y_array;
  GArrowArray * z_array;

  GArrowFloatArrayBuilder *x_builder;
  GArrowFloatArrayBuilder *y_builder;
  GArrowFloatArrayBuilder *z_builder;


  x_builder = garrow_float_array_builder_new();
  y_builder = garrow_float_array_builder_new();
  z_builder = garrow_float_array_builder_new();

  // The number of rows per record batch/ array would be 500
  // defining array length which must be equal for all array

  gint64 len = 100;
  // not Null element in the array- GArrowBuffer *data = NULL
  // no Null element -> gint64 n_nulls = 0;

  // creating the data to input into the array
  // GArrowBuffer * x_buffer;
  // GArrowBuffer * y_buffer;
  // GArrowBuffer * z_buffer;

  gfloat data_x[len];
  gfloat data_y[len];
  gfloat data_z[len];
  gboolean data_valid_x[len];
  gboolean data_valid_y[len];
  gboolean data_valid_z[len];

  //generating the values
  for ( int i = 0; i < len; i++){
    data_x[i] = i;
    data_y[i] = i;
    data_z[i] = i;
    data_valid_x[i]  = true;
    data_valid_y[i] = true;
    data_valid_z[i] = true;

    // g_print("%d ", data_x[i]);
    // g_print("%d ", data_y[i]);
    // g_print("%d ", data_z[i]);

  }

  gboolean success = TRUE;
  printf("Done");

  if(success) {
    success = garrow_float_array_builder_append_values(x_builder,data_x,len,data_valid_x,len,&error);
  }
  if(success) {
    success = garrow_float_array_builder_append_values(y_builder,data_y,len,data_valid_y,len,&error);
  }
  if(success) {
    success = garrow_float_array_builder_append_values(z_builder,data_z,len,data_valid_z,len,&error);
  }

  if (!success) {
    g_print("failed to append: %s\n", error->message);
    g_error_free(error);
    g_object_unref(x_builder);
    g_object_unref(y_builder);
    g_object_unref(z_builder);
    return;
    }

  x_array = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(x_builder), &error);
  y_array = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(y_builder), &error);
  z_array = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(z_builder), &error);
  if(!x_array || !y_array || !z_array) {
    g_print("failed to finish: %s\n", error->message);
    g_error_free(error);
    g_object_unref(x_builder);
    g_object_unref(y_builder);
    g_object_unref(z_builder);

  }

  // derefereing builder
  g_object_unref(x_builder);
  g_object_unref(y_builder);
  g_object_unref(z_builder);
  // gint64 size_of_buffer = len * sizeof(data_x[0]);
  //
  // // Creating the buffer
  //
  // x_buffer = garrow_buffer_new(data_x, size_of_buffer);
  // y_buffer = garrow_buffer_new(data_y, size_of_buffer);
  // z_buffer = garrow_buffer_new(data_z, size_of_buffer);
  // gint64 buffer_size_find_x = garrow_buffer_get_size (x_buffer);
  // gint64 buffer_size_find_y = garrow_buffer_get_size (y_buffer);
  // gint64 buffer_size_find_z = garrow_buffer_get_size (z_buffer);
  // printf("%ld for buffer and %ld for second buffer\n ",len * sizeof(guint8),buffer_size_find_x);
  // printf("%ld for buffer and %ld for second buffer\n ",len * sizeof(guint8),buffer_size_find_y);
  // printf("%ld for buffer and %ld for second buffer\n ",len * sizeof(guint8),buffer_size_find_z);
  // creating arrow float aray
  // x_array = garrow_float_array_new(len, x_buffer, NULL, 0);
  // y_array = garrow_float_array_new(len, y_buffer, NULL, 0);
  // z_array = garrow_float_array_new(len, z_buffer, NULL, 0);
  // g_object_unref(x_buffer);
  // g_object_unref(y_buffer);
  // g_object_unref(z_buffer);
  // g_print("%s\n" ,garrow_array_to_string (GARROW_ARRAY(x_array),&error));
  // g_print("%s\n" ,garrow_array_to_string (GARROW_ARRAY(y_array),&error));
  // g_print("%s\n" ,garrow_array_to_string (GARROW_ARRAY(z_array),&error));

  // printf("%ld for buffer and %ld for second buffer\n ",sizeof(x_array),buffer_size_find_x);
  // printf("%ld for buffer and %ld for second buffer\n ",sizeof(y_array),buffer_size_find_y);
  // printf("%ld for buffer and %ld for second buffer\n ",sizeof(z_array),buffer_size_find_z);

  //g_print("%s", garrow_array_to_string (GARROW_ARRAY(x_array),NULL));
  // const gfloat * to_print = garrow_float_array_get_values (x_array,&len);
  // const gfloat * to_printy = garrow_float_array_get_values (y_array,&len);
  // const gfloat * to_printz = garrow_float_array_get_values (z_array,&len);
  // g_print("%g\n", *to_print);
  // g_print("%g\n", *to_printy);
  // g_print("%g\n", *to_printz);
  // declaring fields




  GArrowField *field_x, *field_y, *field_z;
  // all the array are the same type
  // finding the data type
  //GArrowDataType * data_type_x = garrow_array_get_value_data_type(x_array);
  // GArrowDataType * data_type_y = garrow_array_get_value_data_type(x_array);
  // GArrowDataType * data_type_z = garrow_array_get_value_data_type(x_array);

  field_x = garrow_field_new ("x_column", GARROW_DATA_TYPE(garrow_float_data_type_new()));
  field_y = garrow_field_new ("y_column", GARROW_DATA_TYPE(garrow_float_data_type_new()));
  field_z = garrow_field_new ("z_column", GARROW_DATA_TYPE(garrow_float_data_type_new()));
  GList *fields = NULL; // declaring the fields
  fields = g_list_append(fields, field_x);
  fields = g_list_append(fields, field_y);
  fields = g_list_append(fields, field_z);
   // defining and declaring the schema

  GArrowSchema *schema;
  schema = garrow_schema_new (fields);
  // derefereing field list
  g_list_free(fields);

  // creating the record_batch
  // creating list of the column data

  GList *columns =NULL;
  columns = g_list_append(columns, x_array);
  columns = g_list_append(columns, y_array);
  columns = g_list_append(columns, z_array);




  GArrowRecordBatch *record_batch =  garrow_record_batch_new (schema, len, columns, &error);
  // derefereing columns list
  g_list_free(columns);

  // checking for error while building
  if(!record_batch) {
    printf("Error building record_batch");
    g_print("error: %s\n", error->message);
    g_object_unref(x_array); // unrefernce the builder
    g_object_unref(y_array);
    g_object_unref(z_array);
    g_object_unref(field_x);
    g_object_unref(field_y);
    g_object_unref(field_z);
    g_object_unref(schema);
    return;
    // In real program, use this as an exception hanling to go out of the thread // TODO: confirm
  }

 // sending the data to a buffer

   // buffer initial size
   printf("Arrived\n");

   gint64 initial_size = 2000;
   GArrowResizableBuffer *buffer = garrow_resizable_buffer_new(initial_size, &error);
   if (!buffer) {
     printf("Error building resizable buffer\n");
     g_print("error: %s\n", error->message);
     g_object_unref(x_array); // unrefernce the builder
     g_object_unref(y_array);
     g_object_unref(z_array);
     g_object_unref(field_x);
     g_object_unref(field_y);
     g_object_unref(field_z);
     g_object_unref(schema);
     g_object_unref(record_batch);
     return;
   }


   // creating the stream
   GArrowBufferOutputStream *output = garrow_buffer_output_stream_new(buffer);

   //putting the record inside the steam
   GArrowRecordBatchStreamWriter *writer =
   garrow_record_batch_stream_writer_new(GARROW_OUTPUT_STREAM(output),
                                       schema,
                                       &error);
   if (!writer) {
     printf("Error building stream writter\n");
     g_print("error: %s\n", error->message);
     g_object_unref(x_array); // unrefernce the builder
     g_object_unref(y_array);
     g_object_unref(z_array);
     g_object_unref(field_x);
     g_object_unref(field_y);
     g_object_unref(field_z);
     g_object_unref(schema);
     g_object_unref(record_batch);
     g_object_unref(output);
     g_object_unref(buffer);
     return;

   }

   if (!garrow_record_batch_writer_write_record_batch(
      GARROW_RECORD_BATCH_WRITER(writer),
      record_batch,
      &error)) {
        printf("Error building stream writter\n");
        g_print("error: %s\n", error->message);
        g_object_unref(x_array);
        g_object_unref(y_array);
        g_object_unref(z_array);
        g_object_unref(field_x);
        g_object_unref(field_y);
        g_object_unref(field_z);
        g_object_unref(schema);
        g_object_unref(record_batch);
        g_object_unref(writer);
        g_object_unref(output);
        g_object_unref(buffer);
        return;

   }

   if (!garrow_record_batch_writer_close(
      GARROW_RECORD_BATCH_WRITER(writer),
      &error)) {
        printf("Error closing stream writter\n");
        g_print("error: %s\n", error->message);
        g_object_unref(x_array);
        g_object_unref(y_array);
        g_object_unref(z_array);
        g_object_unref(field_x);
        g_object_unref(field_y);
        g_object_unref(field_z);
        g_object_unref(schema);
        g_object_unref(record_batch);
        g_object_unref(writer);
        g_object_unref(output);
        g_object_unref(buffer);
        return;
   }

   GBytes *data = garrow_buffer_get_data(GARROW_BUFFER(buffer));

   *data_raw = g_bytes_get_data(data, data_size);


   g_bytes_unref(data);
   g_object_unref(x_array);
   g_object_unref(y_array);
   g_object_unref(z_array);
   g_object_unref(field_x);
   g_object_unref(field_y);
   g_object_unref(field_z);
   g_object_unref(schema);
   g_object_unref(record_batch);
   g_object_unref(writer);
   g_object_unref(output);
   g_object_unref(buffer);



}

static void print_array(GArrowArray *array)
{
  GArrowType value_type;
  gint64 i, n;

  value_type = garrow_array_get_value_type(array);

  g_print("[");
  n = garrow_array_get_length(array);

#define ARRAY_CASE(type, Type, TYPE, format)                            \
  case GARROW_TYPE_ ## TYPE:                                            \
    {                                                                   \
      GArrow ## Type ## Array *real_array;                              \
      real_array = GARROW_ ## TYPE ## _ARRAY(array);                    \
      for (i = 0; i < n; i++) {                                         \
        if (i > 0) {                                                    \
          g_print(", ");                                                \
        }                                                               \
        g_print(format,                                                 \
                garrow_ ## type ## _array_get_value(real_array, i));    \
      }                                                                 \
    }                                                                   \
    break

  switch (value_type) {
    ARRAY_CASE(uint8,  UInt8,  UINT8,  "%hhu");
    ARRAY_CASE(uint16, UInt16, UINT16, "%" G_GUINT16_FORMAT);
    ARRAY_CASE(uint32, UInt32, UINT32, "%" G_GUINT32_FORMAT);
    ARRAY_CASE(uint64, UInt64, UINT64, "%" G_GUINT64_FORMAT);
    ARRAY_CASE( int8,   Int8,   INT8,  "%hhd");
    ARRAY_CASE( int16,  Int16,  INT16, "%" G_GINT16_FORMAT);
    ARRAY_CASE( int32,  Int32,  INT32, "%" G_GINT32_FORMAT);
    ARRAY_CASE( int64,  Int64,  INT64, "%" G_GINT64_FORMAT);
    ARRAY_CASE( float,  Float,  FLOAT, "%g");
    ARRAY_CASE(double, Double, DOUBLE, "%g");
  default:
    break;
  }
#undef ARRAY_CASE

  g_print("]\n");
}

static void print_record_batch(GArrowRecordBatch *record_batch)
{
  guint nth_column, n_columns;

  n_columns = garrow_record_batch_get_n_columns(record_batch);
  for (nth_column = 0; nth_column < n_columns; nth_column++) {
    GArrowArray *array;

    g_print("columns[%u](%s): ",
            nth_column,
            garrow_record_batch_get_column_name(record_batch, nth_column));
    array = garrow_record_batch_get_column_data(record_batch, nth_column);
    print_array(array);
    g_object_unref(array);
  }
}

void display_arrow_buffer(gconstpointer *data_raw,gsize *data_size)
{



  GBytes * data = g_bytes_new(*data_raw, *data_size);
  // Passing the data into the buffer
  GArrowBuffer *buffer = garrow_buffer_new_bytes (data);

  // input stream
  GArrowBufferInputStream * input = garrow_buffer_input_stream_new (buffer);

  GError *error = NULL;


    {
      GArrowRecordBatchReader *reader;
      GArrowRecordBatchStreamReader *stream_reader;

      stream_reader =
        garrow_record_batch_stream_reader_new(GARROW_INPUT_STREAM(input),
                                              &error);
      if (!stream_reader) {
        g_print("failed to open stream reader: %s\n", error->message);
        g_error_free(error);
        g_object_unref(input);
        return;
      }

      reader = GARROW_RECORD_BATCH_READER(stream_reader);
      while (TRUE) {
        GArrowRecordBatch *record_batch;

        record_batch = garrow_record_batch_reader_read_next(reader, &error);
        if (error) {
          g_print("failed to read the next record batch: %s\n", error->message);
          g_error_free(error);
          g_object_unref(reader);
          g_object_unref(input);
          return;
        }

        if (!record_batch) {
          break;
        }

        print_record_batch(record_batch);
        g_object_unref(record_batch);
      }

      g_object_unref(reader);
    }

    g_object_unref(input);
    g_object_unref(buffer);
    g_bytes_unref(data);
}


int main(void)
{
  gconstpointer data;
  gsize data_size;
  create_arrow_buffer(&data, &data_size);
  display_arrow_buffer(&data, &data_size);

  printf("Finish process\n");
  return 0;
}
