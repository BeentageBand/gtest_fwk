$(_flavor_)_PROJ_INC+=$($(_flavor_)_GTEST_PATH)/googlemock/include
$(_flavor_)_PROJ_INC+=$($(_flavor_)_GTEST_PATH)/googletest/include

$(_flavor_)_clean+=$($(_flavor_)_GTEST_PATH)/googletest/make/*.[oa]
$(_flavor_)_clean+=$($(_flavor_)_GTEST_PATH)/googlemock/make/*.[oa]

$(_feat_)_lib=\
	gmock \
	gmock_main \

define $(_flavor_)_GTEST_MAKE
$($(_flavor_)_LIB_DIR)/lib$(1).a : $($(_flavor_)_GTEST_PATH)/googlemock/make/$(1).a $($(_flavor_)_LIB_DIR)
	-cp -Pf $$< $$@;

$($(_flavor_)_GTEST_PATH)/googlemock/make/$(1).a : $($(_flavor_)_GTEST_PATH)/googlemock/make/Makefile $($(_flavor_)_GTEST_PATH)/googletest/make/Makefile
	$(MAKE) all -C $($(_flavor_)_GTEST_PATH)/googletest/make 
	$(MAKE) all -C $($(_flavor_)_GTEST_PATH)/googlemock/make 

endef

ifndef $(_flavor_)_GTEST_OVERRIDE

$(foreach _tg_,$($(_feat_)_lib),\
	$(eval \
		$(call Verbose,$(call $(_flavor_)_GTEST_MAKE,$(_tg_)))\
	)\
)

$(_flavor_)_GTEST_OVERRIDE=1
endif
