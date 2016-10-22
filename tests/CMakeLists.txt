FIND_PACKAGE(Qt4 4.8.2 REQUIRED QtTest)

SET(CMAKE_AUTOMOC ON)

ADD_SUBDIRECTORY(common)

SET(TESTS
    TestQFilesystemHandler
    TestQHttpHandler
    TestQHttpParser
    TestQHttpServer
    TestQHttpSocket
    TestQIByteArray
    TestQIODeviceCopier
    TestQLocalFile
    TestQObjectHandler
)

FOREACH(TEST ${TESTS})
    ADD_EXECUTABLE(${TEST} ${TEST}.cpp)
    QT4_USE_MODULES(${TEST} Test)
    TARGET_LINK_LIBRARIES(${TEST} QHttpEngine common)
    ADD_TEST(NAME ${TEST} COMMAND ${TEST})
ENDFOREACH()

# On Windows, the library's DLL must exist in the same directory as the test
# executables which link against it - create a custom command to copy it
IF(WIN32 AND NOT BUILD_STATIC)
    ADD_CUSTOM_TARGET(QHttpEngine-copy ALL
        "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:QHttpEngine>" "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS QHttpEngine
    )
ENDIF()
