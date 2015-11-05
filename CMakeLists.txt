if (NOT TRANSLATE_BUILD_MODE)
    set (TRANSLATE_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (translate)
translateInit(${TRANSLATE_BUILD_MODE})
