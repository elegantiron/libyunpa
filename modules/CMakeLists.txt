target_sources(
    ${PROJECT_NAME}
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
    libyunpa.cppm
    libyunpa/Core.cppm
    libyunpa/Events.cppm
    libyunpa/Helpers.cppm
    libyunpa/Keyboard.cppm
    libyunpa/Menu.cppm
    libyunpa/Scene.cppm
    libyunpa/Time.cppm
)