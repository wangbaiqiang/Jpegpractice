# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


# Copyright 2014 http://Bither.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)


#jpeg-turbo
include $(CLEAR_VARS)
LOCAL_MODULE    := libjpegbither
LOCAL_SRC_FILES := libjpegbither.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := bitherjni
LOCAL_SRC_FILES := imagerar.c
#用到了上面的库
LOCAL_SHARED_LIBRARIES := libjpegbither 
LOCAL_LDLIBS := -ljnigraphics -llog  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/main/jin \
                    $(LOCAL_PATH)/src/main/jin/jpeg \
                    $(LOCAL_PATH)/src/main/jin/jpeg/android
include $(BUILD_SHARED_LIBRARY)