CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

SET(PROJECT_NAME mass-spring-damper)
PROJECT(${PROJECT_NAME} C)

FILE(GLOB SRC *.c *.h)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})
