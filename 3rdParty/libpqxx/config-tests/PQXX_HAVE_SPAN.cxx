// Feature check for 'PQXX_HAVE_SPAN'.
// Generated by generate_cxx_checks.py.
#include <version>
#if !defined(__cpp_lib_span)
#  error "No PQXX_HAVE_SPAN: __cpp_lib_span is not set."
#endif
#if !__cpp_lib_span
#  error "No PQXX_HAVE_SPAN: __cpp_lib_span is false."
#endif

int main() {}
