cmake_minimum_required(VERSION 3.15)
project(InvaderC LANGUAGES C CXX)

# 표준 지정
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# 소스/헤더 자동 수집
file(GLOB SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
file(GLOB HDR_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/include/*.h
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

# include 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# MSVC 전용 디버그 정보 옵션 유지
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,\
      $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,\
      $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()