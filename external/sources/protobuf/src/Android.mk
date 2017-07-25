LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  

LOCAL_MODULE := protobuf_static
LOCAL_MODULE_FILENAME := libprotobuf
   
LOCAL_SRC_FILES := 	google/protobuf/io/coded_stream.cc \
					google/protobuf/stubs/common.cc \
					google/protobuf/descriptor.cc \
					google/protobuf/descriptor.pb.cc \
					google/protobuf/descriptor_database.cc \
					google/protobuf/dynamic_message.cc \
					google/protobuf/extension_set.cc \
					google/protobuf/extension_set_heavy.cc \
					google/protobuf/generated_message_reflection.cc \
					google/protobuf/generated_message_util.cc \
					google/protobuf/io/gzip_stream.cc \
					google/protobuf/compiler/importer.cc \
					google/protobuf/message.cc \
					google/protobuf/message_lite.cc \
					google/protobuf/stubs/once.cc \
					google/protobuf/compiler/parser.cc \
					google/protobuf/io/printer.cc \
					google/protobuf/reflection_ops.cc \
					google/protobuf/repeated_field.cc \
					google/protobuf/service.cc \
					google/protobuf/stubs/structurally_valid.cc \
					google/protobuf/stubs/strutil.cc \
					google/protobuf/stubs/substitute.cc \
					google/protobuf/text_format.cc \
					google/protobuf/io/tokenizer.cc \
					google/protobuf/io/strtod.cc \
					google/protobuf/unknown_field_set.cc \
					google/protobuf/wire_format.cc \
					google/protobuf/wire_format_lite.cc \
					google/protobuf/io/zero_copy_stream.cc \
					google/protobuf/io/zero_copy_stream_impl.cc \
					google/protobuf/io/zero_copy_stream_impl_lite.cc \
					google/protobuf/stubs/stringprintf.cc    

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_LDLIBS := -llog -pthread -lz
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)