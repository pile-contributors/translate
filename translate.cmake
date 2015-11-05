
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
        "translang.h"
        "translate.h")
    set(TRANSLATE_SOURCES
        "translang.cc"
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

# Creates translation files. Should be run in internationalization directory.
#
# A custom target is created - translations_update - that scans source files
# for strings to translate. A .ts file is created for each language if it does
# not exist already and corresponding .qm file is compiled and installed.
#
# Each language is represented by a directory (name not important but should
# be meaningful) that consists of the .ts file, an icon.png file and
# a metadata.ini file.
#
macro (internationalizationDirectory)

    find_package(Qt5 REQUIRED COMPONENTS LinguistTools)

    # prevent source files from being deleted on make clean
    set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

    # get the list of subdirectories
    unset(lang_dirs)
    pileSubDirList(lang_dirs "${CMAKE_CURRENT_SOURCE_DIR}")

    # iterate and collect translations
    unset(lang_ts_files)
    foreach(lang_dir ${lang_dirs})
        unset(contents)
        unset(language_label)
        unset(language_icon)
        unset(language_translate)
        unset(language_additional)
        unset(content_parts)
        unset(content_part_1)
        unset(content_part_2)
        unset (lang_qm_file)
        unset(lang_meta_file)

        set (lang_meta_file "${CMAKE_CURRENT_SOURCE_DIR}/${lang_dir}/metadata.ini")
        file(READ "${lang_meta_file}" contents)
        string(REGEX REPLACE ";" "\\\\;" contents "${contents}")
        string(REGEX REPLACE "\n" ";" contents "${contents}")


        foreach(content_line_iter ${contents})
            string(REGEX REPLACE ";" "\\\\;" content_line "${content_line_iter}")
            unset(content_parts)
            string(REGEX REPLACE "=" ";" content_parts "${content_line}")
            unset(content_part_1)
            unset(content_part_2)
            unset(content_parts_count)
            list(LENGTH content_parts content_parts_count)
            list(GET content_parts 0 content_part_1)
            if (content_parts_count GREATER "1")
                list(GET content_parts 1 content_part_2)
            endif()
            if (content_part_1 MATCHES "^label$")
                set(language_label ${content_part_2} )
            elseif (content_part_1 MATCHES "^icon$")
                set(language_icon ${content_part_2} )
            elseif (content_part_1 MATCHES "^translate$")
                set(language_translate ${content_part_2} )
            elseif (content_part_1 MATCHES "^additional$")
                string(REGEX REPLACE "\\\\;" ";" language_additional "${content_part_2}")
            endif()
        endforeach()

        unset(install_destin)
        if(APPLE AND UNIX) ## OSX
            set(install_destin "${PROJECT_NAME}.app/Contents/Resources/translations/${lang_dir}")
        else()
            set(install_destin "share/locale/${PROJECT_NAME_UNIX}/${lang_dir}")
        endif()

        install(
            FILES
                "${lang_meta_file}"
            DESTINATION ${install_destin})

        if (language_translate)
            unset(local_ts_file)
            set(local_ts_file "${CMAKE_CURRENT_SOURCE_DIR}/${lang_dir}/${language_translate}.ts")
            list(APPEND lang_ts_files "${local_ts_file}")
            unset(lang_qm_file)
            set(lang_qm_file "${CMAKE_CURRENT_BINARY_DIR}/${language_translate}.qm")

            install(
                FILES
                    "${lang_qm_file}"
                DESTINATION ${install_destin})
        endif()
        if (language_icon)
            install(
                FILES
                    "${CMAKE_CURRENT_SOURCE_DIR}/${lang_dir}/${language_icon}"
                DESTINATION ${install_destin})
        endif()
        foreach(additional ${language_additional})
            install(
                FILES
                    "${CMAKE_CURRENT_SOURCE_DIR}/${lang_dir}/${additional}"
                DESTINATION ${install_destin})
        endforeach()
    endforeach()

    unset(existing_lang_ts_files)
    file(GLOB_RECURSE existing_lang_ts_files "*.ts")
    list(APPEND lang_ts_files ${existing_lang_ts_files})
    list(REMOVE_DUPLICATES lang_ts_files)

    qt5_create_translation(
        lang_qm_files
        ${PROJECT_SOURCE_DIR}
        ${lang_ts_files})

    # qt5_add_translation(TRANSLATION_QM ${lang_qm_files})
    # message(STATUS "---->> TRANSLATION_QM = ${TRANSLATION_QM}")

    add_custom_target(
        translations_update ALL
        DEPENDS ${lang_qm_files})


endmacro ()
