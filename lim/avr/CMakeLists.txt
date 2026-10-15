cmake_minimum_required(VERSION 3.13)

# ============================================================
# 프로젝트 설정
# ============================================================

project(atmega128a
    LANGUAGES ASM C CXX
)

# 실제 칩은 ATmega128A지만 컴파일 Target은 atmega128
# 인터럽트 벡터 테이블 생성에 이상이 있으므로 atmega128로 설정함
set(MCU atmega128)

# MCU 동작 주파수
# ATmega128A가 외부 16MHz 크리스털을 사용한다고 가정
set(MCU_CLOCK 16000000UL)

# 생성할 실행 파일
set(EXECUTABLE ${PROJECT_NAME}.elf)

# VS Code IntelliSense용 compile_commands.json 생성
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C/C++ 표준
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 소스 파일 수집
# ============================================================

# 프로젝트 루트의 C/C++ 소스
file(GLOB ROOT_SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# src 폴더 아래의 모든 C/C++/ASM 소스
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.S"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.s"
)

# ============================================================
# ELF 실행 파일 생성
# ============================================================

add_executable(${EXECUTABLE}
    ${ROOT_SOURCE_FILES}
    ${SOURCE_FILES}
)

# ============================================================
# 컴파일 타임 매크로
# ============================================================

target_compile_definitions(${EXECUTABLE} PRIVATE
    F_CPU=${MCU_CLOCK}
)

# ============================================================
# Header Include 디렉토리
# 사용하지 않는 경로는 삭제하거나 주석 처리 가능
# ============================================================

target_include_directories(${EXECUTABLE} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ap"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bsp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/driver"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/common/hw/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/project"
)

# ============================================================
# 컴파일러 옵션
# ============================================================

target_compile_options(${EXECUTABLE} PRIVATE
    # ATmega128A 선택
    -mmcu=${MCU}

    # 사용하지 않는 함수 및 데이터 제거 준비
    -ffunction-sections
    -fdata-sections

    # Link Time Optimization
    -flto
    -fno-fat-lto-objects

    # 경고 출력
    -Wall
    -Wextra

    # 크기 최적화
    -Os

    # 디버깅 정보
    -g3
)

# ============================================================
# 링커 옵션
# ============================================================

target_link_options(${EXECUTABLE} PRIVATE
    # ATmega128A용 링크
    -mmcu=${MCU}

    # Link Time Optimization
    -flto
    -fuse-linker-plugin

    # 사용하지 않는 함수 및 데이터 제거
    -Wl,--gc-sections

    # MAP 파일 생성
    "-Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map,--cref"

    # Flash, SRAM, EEPROM 사용량 출력
    -Wl,--print-memory-usage
)

# ============================================================
# 라이브러리
# ============================================================

# AVR 수학 라이브러리
target_link_libraries(${EXECUTABLE} PRIVATE
    m
)

# ============================================================
# ELF → HEX 파일 생성
# ============================================================

add_custom_command(
    TARGET ${EXECUTABLE}
    POST_BUILD

    COMMAND ${CMAKE_OBJCOPY}
        -O ihex
        -R .eeprom
        $<TARGET_FILE:${EXECUTABLE}>
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex"

    COMMENT "Generating ATmega128A HEX file"
    VERBATIM
)

# ============================================================
# EEPROM 파일 생성
# ============================================================

add_custom_command(
    TARGET ${EXECUTABLE}
    POST_BUILD

    COMMAND ${CMAKE_OBJCOPY}
        -O ihex
        -j .eeprom
        --set-section-flags=.eeprom=alloc,load
        --no-change-warnings
        --change-section-lma
        .eeprom=0
        $<TARGET_FILE:${EXECUTABLE}>
        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.eep"

    COMMENT "Generating ATmega128A EEPROM file"
    VERBATIM
)

# ============================================================
# 펌웨어 크기 출력
# ============================================================

add_custom_command(
    TARGET ${EXECUTABLE}
    POST_BUILD

    COMMAND ${CMAKE_SIZE_UTIL}
        -B
        $<TARGET_FILE:${EXECUTABLE}>

    COMMENT "ATmega128A Firmware Size:"
    VERBATIM
)