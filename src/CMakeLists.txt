
include_directories(${PROJECT_NAME} PUBLIC common)


set(SOURCES 
    main.c
    connectivity/Socket.c)

add_executable(${PROJECT_NAME} ${SOURCES})