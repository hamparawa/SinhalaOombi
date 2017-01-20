
# Scan all files recursively
file(GLOB_RECURSE headerFiles "*.h")
file(GLOB_RECURSE sourceFiles "*.cpp")

# Define the project
add_executable(OombiBasic "${headerFiles};${sourceFiles}")