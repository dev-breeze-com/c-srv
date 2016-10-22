SET(SRC
    apihandler.cpp
    main.cpp
)

QT4_ADD_RESOURCES(QRC RESOURCES.QRC)

ADD_EXECUTABLE(chatserver ${SRC} ${QRC})
TARGET_LINK_LIBRARIES(chatserver QHttpEngine)

INSTALL(TARGETS chatserver
    RUNTIME DESTINATION "${EXAMPLE_DIR}"
)
