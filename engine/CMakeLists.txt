FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)

INCLUDE(${QT_USE_FILE})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -pedantic ")

SET(SRC
	Engine.cc
	ExternalPlayer.cc
	GameRules.cc
	GameState.cc
	Player.cc
	PlayerListener.cc
	StdoutPlayer.cc
	WebsocketServer.cc
	Logger.cc
)

SET(HEADERS
	Engine.h
	Player.h
	ExternalPlayer.h
	WebsocketServer.h
)

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})

ADD_LIBRARY(engine SHARED ${SRC} ${HEADERS_MOC})
TARGET_LINK_LIBRARIES(engine ${QT_LIBRARIES})
