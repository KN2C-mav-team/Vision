FILE(REMOVE_RECURSE
  "include_window_automoc.cpp"
  "window_automoc.cpp"
  "CMakeFiles/window_pkg_generate_messages_py"
  "devel/lib/python2.7/dist-packages/window_pkg/msg/_WinMsg.py"
  "devel/lib/python2.7/dist-packages/window_pkg/msg/__init__.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/window_pkg_generate_messages_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
