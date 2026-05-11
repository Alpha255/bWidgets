file(GLOB_RECURSE SVG_FILES "${ICONS_SVG_DIR}/*.svg")
list(SORT SVG_FILES)

set(CONTENT "")
foreach(SVG_FILE ${SVG_FILES})
    get_filename_component(ICON_NAME "${SVG_FILE}" NAME_WE)
    string(TOUPPER "${ICON_NAME}" ICON_NAME_UPPER)
    string(APPEND CONTENT "DEF_ICON(${ICON_NAME_UPPER})\n")
endforeach()

file(WRITE "${OUTPUT_HEADER}" "${CONTENT}")
