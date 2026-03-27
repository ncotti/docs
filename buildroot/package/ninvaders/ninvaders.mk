NINVADERS_SITE 			:= https://sourceforge.net/projects/ninvaders/files/ninvaders/0.1.1
NINVADERS_VERSION 		:= 0.1.1
NINVADERS_SOURCE		:= ninvaders-$(NINVADERS_VERSION).tar.gz
NINVADERS_SITE_METHOD 	:= wget

NINVADERS_LICENSE		:= GPL-2.0-only
NINVADERS_LICENSE_FILES := gpl.txt

NINVADERS_DEPENDENCIES := ncurses


define NINVADERS_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) $(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -fcommon" -C $(@D)
endef

NINVADERS_INSTALL_TARGET = YES
define NINVADERS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/nInvaders $(TARGET_DIR)/usr/bin/ninvaders
endef

$(eval $(generic-package))