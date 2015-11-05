
# enable/disable cmake debug messages related to this pile
set (TRANSLATE_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (translateInit
          ref_cnt_use_mode)

    # default name
    if (NOT TRANSLATE_INIT_NAME)
        set(TRANSLATE_INIT_NAME "translate")
    endif ()

    # compose the list of headers and sources
    set(TRANSLATE_HEADERS
        "translate.h")
    set(TRANSLATE_SOURCES
        "translate.cc")

    pileSetSources(
        "${TRANSLATE_INIT_NAME}"
        "${TRANSLATE_HEADERS}"
        "${TRANSLATE_SOURCES}")

    pileSetCommon(
        "${TRANSLATE_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
