
# Scan all files recursively
file(GLOB headerFiles "*.h")
file(GLOB sourceFiles "*.cpp")

# Define the project
add_executable(SinhalaOombi WIN32 "${headerFiles};${sourceFiles};Res.rc")

# Required to PlaySound()
target_link_libraries(SinhalaOombi Winmm)