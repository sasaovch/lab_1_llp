file(REMOVE_RECURSE
  "MyProgram"
  "MyProgram.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/MyProgram.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
