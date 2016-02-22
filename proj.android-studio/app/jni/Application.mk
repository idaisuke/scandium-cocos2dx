APP_STL := gnustl_static
APP_ABI := armeabi-v7a
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic

# for SQLCipher
APP_CPPFLAGS += -DSQLITE_HAS_CODEC -DSQLITE_TEMP_STORE=2

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
