set (EXECUTABLE_NAME SlotMachineSDL)

add_subdirectory(CAppSettings)
add_subdirectory(CSDLContext)
add_subdirectory(FontManager)
add_subdirectory(TextureManager)

include_directories({PROJECT_SOURCE_DIR}/ICommand)
include_directories({PROJECT_SOURCE_DIR}/Serializer)
include_directories({PROJECT_SOURCE_DIR}/CustomTypes)

add_executable(${EXECUTABLE_NAME} main.cpp Game.hpp Game.cpp SlotMachine.hpp SlotMachine.cpp SlotSymbol.hpp SlotSymbol.cpp)

target_link_libraries(${EXECUTABLE_NAME} CAppSettings
                                         CSDLContext 
                                         FontManager 
                                         TextureManager)

##Vendor
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(../vendor/SDL ../sdl_build)
target_link_libraries(${EXECUTABLE_NAME} SDL2)

add_subdirectory(../vendor/SDL_image ../sdl_image_build)
target_link_libraries(${EXECUTABLE_NAME} SDL2_image)

add_subdirectory(../vendor/SDL_ttf ../sdl_ttf_build)
target_link_libraries(${EXECUTABLE_NAME} SDL2_ttf)

set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)

add_subdirectory(../vendor/SDL_mixer ../sdl_mixer_build)
target_link_libraries(${EXECUTABLE_NAME} SDL2_mixer)