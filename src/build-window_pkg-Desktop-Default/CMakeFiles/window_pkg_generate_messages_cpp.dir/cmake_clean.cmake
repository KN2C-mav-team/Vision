FILE(REMOVE_RECURSE
  "include_window_automoc.cpp"
  "window_automoc.cpp"
  "CMakeFiles/window_pkg_generate_messages_cpp"
  "devel/include/window_pkg/WinMsg.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/window_pkg_generate_messages_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
