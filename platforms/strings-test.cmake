cmake_minimum_required(VERSION 3.22)
include("${CMAKE_CURRENT_LIST_DIR}/strings.cmake")
if(DEFINED INPUT)
    kobalt_generate_strings("${INPUT}" "${OUTPUT}" ${SOURCE_FILE})
    return()
endif()
if(NOT DEFINED TEST_TMP_ROOT)
    message(FATAL_ERROR "TEST_TMP_ROOT is required")
endif()
set(scratch "${TEST_TMP_ROOT}/strings-build")
file(MAKE_DIRECTORY "${scratch}")
file(WRITE "${scratch}/valid.strings" [=[/* Unsorted, with punctuation and UTF-8. */
"z.key" = "Quotes: \"yes\"; path: C:\\books\nnext\tcolumn\rreturn";
"a.key" = "Élan; 1.5× @key@ ${key}";
]=])
file(WRITE "${scratch}/used.cpp" [=[strings::get(
    "a.key"); strings::key("z.key");]=])
kobalt_generate_strings("${scratch}/valid.strings" "${scratch}/generated.inc" "${scratch}/used.cpp")
file(READ "${scratch}/generated.inc" actual)
set(expected [=[// Generated from assets/valid.strings. Do not edit.
    {"a.key", "Élan; 1.5× @key@ ${key}"},
    {"z.key", "Quotes: \"yes\"; path: C:\\books\nnext\tcolumn\rreturn"},
]=])
if(NOT actual STREQUAL expected)
    message(FATAL_ERROR "UI strings changed order, UTF-8 or escaping:\n${actual}")
endif()
foreach(invalid IN ITEMS duplicate escape syntax empty)
    if(invalid STREQUAL "duplicate")
        set(contents "\"a\" = \"one\";\n\"a\" = \"two\";\n")
    elseif(invalid STREQUAL "escape")
        set(contents [=["a" = "bad\q";]=])
    elseif(invalid STREQUAL "syntax")
        set(contents [=["a" = "no semicolon"]=])
    else()
        set(contents "/* empty */\n")
    endif()
    file(WRITE "${scratch}/${invalid}.strings" "${contents}")
    execute_process(COMMAND "${CMAKE_COMMAND}" "-DINPUT=${scratch}/${invalid}.strings"
        "-DOUTPUT=${scratch}/invalid.inc" -P "${CMAKE_CURRENT_LIST_FILE}"
        RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(result EQUAL 0)
        message(FATAL_ERROR "Accepted ${invalid} UI strings")
    endif()
endforeach()
file(WRITE "${scratch}/missing.cpp" [=[strings::get("missing.key");]=])
execute_process(COMMAND "${CMAKE_COMMAND}" "-DINPUT=${scratch}/valid.strings"
    "-DOUTPUT=${scratch}/invalid.inc" "-DSOURCE_FILE=${scratch}/missing.cpp" -P "${CMAKE_CURRENT_LIST_FILE}"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT error MATCHES "Missing UI string missing.key")
    message(FATAL_ERROR "Missing UI key did not fail generation: ${output}${error}")
endif()
# Prove that an ordinary build reconfigures after editing the canonical source.
file(WRITE "${scratch}/CMakeLists.txt" "cmake_minimum_required(VERSION 3.22)\nproject(Strings NONE)\ninclude(\"${CMAKE_CURRENT_LIST_DIR}/strings.cmake\")\nset_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS \"\${CMAKE_CURRENT_SOURCE_DIR}/valid.strings\")\nkobalt_generate_strings(\"\${CMAKE_CURRENT_SOURCE_DIR}/valid.strings\" \"\${CMAKE_CURRENT_BINARY_DIR}/strings.inc\")\n")
execute_process(COMMAND "${CMAKE_COMMAND}" -S "${scratch}" -B "${scratch}/build" RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Strings fixture configuration failed: ${output}${error}")
endif()
file(APPEND "${scratch}/valid.strings" "\"new.key\" = \"New copy\";\n")
execute_process(COMMAND "${CMAKE_COMMAND}" --build "${scratch}/build" RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Strings fixture rebuild failed: ${output}${error}")
endif()
file(READ "${scratch}/build/strings.inc" rebuilt)
if(NOT rebuilt MATCHES "new.key")
    message(FATAL_ERROR "Editing canonical strings did not regenerate the compiled table")
endif()
# Locale parity: every translation must keep the same keys and %n slots.
file(WRITE "${scratch}/en-pair.strings" [=[
"shared.one" = "One";
"shared.two" = "Two %1 books";
]=])
file(WRITE "${scratch}/ru-pair.strings" [=[
"shared.one" = "Один";
"shared.two" = "Две %1 книги";
]=])
file(WRITE "${scratch}/parity-ok.cmake"
    "include(\"${CMAKE_CURRENT_LIST_DIR}/strings.cmake\")\n"
    "kobalt_check_string_parity(\"${scratch}/en-pair.strings\" \"${scratch}/ru-pair.strings\")\n")
execute_process(COMMAND "${CMAKE_COMMAND}" -P "${scratch}/parity-ok.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Matching locale pair was rejected: ${output}${error}")
endif()
file(WRITE "${scratch}/ru-missing.strings" "\"shared.one\" = \"Один\";\n")
file(WRITE "${scratch}/parity-missing.cmake"
    "include(\"${CMAKE_CURRENT_LIST_DIR}/strings.cmake\")\n"
    "kobalt_check_string_parity(\"${scratch}/en-pair.strings\" \"${scratch}/ru-missing.strings\")\n")
execute_process(COMMAND "${CMAKE_COMMAND}" -P "${scratch}/parity-missing.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT error MATCHES "UI string key mismatch")
    message(FATAL_ERROR "Locale key mismatch was not rejected: ${output}${error}")
endif()
file(WRITE "${scratch}/ru-slots.strings"
    "\"shared.one\" = \"Один\";\n\"shared.two\" = \"Две книги\";\n")
file(WRITE "${scratch}/parity-slots.cmake"
    "include(\"${CMAKE_CURRENT_LIST_DIR}/strings.cmake\")\n"
    "kobalt_check_string_parity(\"${scratch}/en-pair.strings\" \"${scratch}/ru-slots.strings\")\n")
execute_process(COMMAND "${CMAKE_COMMAND}" -P "${scratch}/parity-slots.cmake"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(result EQUAL 0 OR NOT error MATCHES "UI string argument mismatch")
    message(FATAL_ERROR "Locale slot mismatch was not rejected: ${output}${error}")
endif()
file(REMOVE_RECURSE "${scratch}")
message(STATUS "UI strings generation and rebuild checks passed")
