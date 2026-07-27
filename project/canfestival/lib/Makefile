
PKG_NAME    := canfestival
PKG_VER     := caa42ec6df0bd9e937afc258f0ee3dac803e0e06
PKG_SOURCES := https://github.com/embox/canfestival/archive/$(PKG_VER).zip
PKG_MD5     := e3792d09c09a7dedd67c505eb32fba87

CF_SOURCE_DIR = $(PKG_SOURCE_DIR)
CF_BUILD_DIR = $(PKG_SOURCE_DIR)/build

INSTALL_INC_DIR := $(PKG_INSTALL_DIR)/include

$(CONFIGURE) :
	$(MKDIR) $(CF_BUILD_DIR) && cd $(CF_BUILD_DIR) && ( \
		cmake $(CF_SOURCE_DIR) \
			-DCMAKE_BUILD_TYPE:STRING=None \
			-DCMAKE_SYSTEM_NAME:STRING=Generic \
			-DCMAKE_C_COMPILER:PATH=$(EMBOX_GCC) \
			-DCMAKE_C_FLAGS="-DCF_MAX_NB_CAN_PORTS=2" \
			-DCF_TARGET=embox \
			-DCF_CAN_DRIVER=embox \
	)
	touch $@

$(BUILD) :
	cd $(CF_BUILD_DIR) && ( \
		$(MAKE) MAKEFLAGS='$(EMBOX_IMPORTED_MAKEFLAGS)'; \
	)
	touch $@

$(INSTALL) :
	$(MKDIR) $(INSTALL_INC_DIR)
	$(CP) $(CF_BUILD_DIR)/src/libcanfestival.a $(PKG_INSTALL_DIR)
	$(CP) $(CF_BUILD_DIR)/drivers/libcanfestival_embox.a $(PKG_INSTALL_DIR)
	$(CP) $(CF_BUILD_DIR)/drivers/libcanfestival_can_embox.a $(PKG_INSTALL_DIR)
	$(CP) -r $(CF_BUILD_DIR)/include/* $(INSTALL_INC_DIR)/
	$(CP) -r $(CF_SOURCE_DIR)/include/* $(INSTALL_INC_DIR)/
	$(CP) -r $(CF_SOURCE_DIR)/include/embox/* $(INSTALL_INC_DIR)/
	touch $@
