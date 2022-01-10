add_executable(${PROJECT_NAME}
  cli.c
  gemini.c
  flag.c
  log.c
  ssl.c
  ui.c
  viv.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_C_STANDARD_REQUIRED ON
  CMAKE_C_STANDARD          99
  CMAKE_C_EXTENSIONS        OFF
)

target_compile_options(${PROJECT_NAME} PUBLIC
  -std=c99
  # -ansi
  -Wall
  -Wextra
  # -Werror=pedantic
  -Wno-unused-function
  # -Wpedantic
  # -pedantic
  # -pedantic-errors
  -march=native
  # -03
  -D_XOPEN_SOURCE=500
  -D_POSIX_C_SOURCE=200112L
  -g
  -fsanitize=address
)
target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
  OpenSSL::SSL
  OpenSSL::Crypto
)

set(CURSES_NEED_NCURSES ON)
find_package(Curses REQUIRED)
if (CURSES_FOUND AND CURSES_HAVE_NCURSES_H)
  target_include_directories(${PROJECT_NAME} PRIVATE
    ncurses
    menu
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ncurses
    menu
  )
endif ()
