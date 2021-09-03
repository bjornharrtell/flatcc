# Note: some files under source control may be tested with binary comparison.
# Under git such files are protected with the `.gitattributes` file.
# Incorrect line endings may lead to failed tests.
if (FLATCC_TEST)
add_subdirectory(flatgeobuf_test)
endif()
