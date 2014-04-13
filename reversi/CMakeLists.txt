FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES("../engine")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

SET(SRC
	ReversiGameState.cc
	ReversiGameWindow.cc
	ReversiRules.cc
	ReversiWidget.cc
	SimpleAI.cc
)

SET(FORMS
	ReversiWindow.ui
)

SET(HEADERS
	ReversiWidget.h
	ReversiGameWindow.h
)


QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})
QT4_WRAP_UI(FORMS_UIC ${FORMS})

ADD_EXECUTABLE(ui_player ui_main.cc ${SRC} ${HEADERS_MOC} ${FORMS_UIC})
TARGET_LINK_LIBRARIES(ui_player ${QT_LIBRARIES} engine)

ADD_EXECUTABLE(bot_player bot_main.cc ${SRC} ${HEADERS_MOC} ${FORMS_UIC})
TARGET_LINK_LIBRARIES(bot_player ${QT_LIBRARIES} engine)

ADD_EXECUTABLE(engine_cli engine_main.cc ${SRC} ${HEADERS_MOC} ${FORMS_UIC})
TARGET_LINK_LIBRARIES(engine_cli ${QT_LIBRARIES} engine)
