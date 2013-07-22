# CHECK Player
FIND_LIBRARY(with_player
    NAMES playerc
    PATHS /usr/local/lib)

IF(NOT with_player)
    MESSAGE ("*** Player libraries not found. Are needed to PlayerServer component")
endif()
IF(with_player)
    MESSAGE ("*** Player OK")
endif()


include_directories(/usr/local/lib)
link_directories(/usr/local/include)
