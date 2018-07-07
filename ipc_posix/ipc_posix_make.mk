define $(_flavor_)_$(_feat_)_MAKE
$(_flavor_)_$(_feat_)_inc=ipc_posix.h
$(_flavor_)_$(_feat_)_lib_objs=ipc_posix
$(_flavor_)_$(_feat_)_lib=ipc_posix
endef

-include $(PROJ_MAK_DIR)/epilog.mk
