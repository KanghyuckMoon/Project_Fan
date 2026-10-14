# 2026/09/02 버전
# ============================================================
# AVR GCC Toolchain File 
#
# 역할:
#   CMake가 Ubuntu용 GCC 대신 AVR 전용 GCC를 사용하도록 설정한다.
#
# 사용 방법:
#   -DCMAKE_TOOLCHAIN_FILE=${workspaceFolder}/tools/avr-toolchain.cmake
#
# AVR Toolchain 기본 경로:
#   /opt/microchip/avr-gcc
#
# 다른 경로를 사용할 경우 Configure Args에서 덮어쓸 수 있다.
#   -DAVR_TOOLCHAIN_DIR=/다른/avr-gcc/경로
# ============================================================


# ============================================================
# 1. 크로스 컴파일 대상 설정
# ============================================================

# AVR에는 Linux, Windows와 같은 운영체제가 없으므로 Generic으로 설정한다.
set(CMAKE_SYSTEM_NAME Generic)

# CMake에 대상 프로세서가 AVR임을 알려준다.
set(CMAKE_SYSTEM_PROCESSOR AVR)

# CMake가 컴파일러를 검사할 때 실행 가능한 프로그램을 만들지 않고
# 정적 라이브러리까지만 생성하도록 제한한다.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


# ============================================================
# 2. AVR Toolchain 설치 경로
# ============================================================

# AVR_TOOLCHAIN_DIR이 Configure Args에서 전달되지 않은 경우
# /opt/microchip/avr-gcc를 기본 경로로 사용한다.
#
# CACHE PATH로 선언했으므로 Configure Args의 -D 값이 우선 적용된다.
set(AVR_TOOLCHAIN_DIR "/opt/microchip/avr-gcc" CACHE PATH "AVR GCC Toolchain directory")

# avr-gcc가 실제로 존재하는지 확인한다.
# 경로가 잘못되었으면 CMake Configure 단계에서 즉시 오류를 발생시킨다.
if(NOT EXISTS "${AVR_TOOLCHAIN_DIR}/bin/avr-gcc")
    message(FATAL_ERROR
        "AVR GCC를 찾을 수 없습니다.\n"
        "현재 AVR_TOOLCHAIN_DIR: ${AVR_TOOLCHAIN_DIR}\n"
        "예: -DAVR_TOOLCHAIN_DIR=/opt/microchip/avr-gcc"
    )
endif()

# AVR 프로그램들의 공통 경로 접두사
#
# 최종 실행 파일 예:
#   ${TOOLCHAIN_PREFIX}gcc     → avr-gcc
#   ${TOOLCHAIN_PREFIX}g++     → avr-g++
#   ${TOOLCHAIN_PREFIX}objcopy → avr-objcopy
#   ${TOOLCHAIN_PREFIX}size    → avr-size
set(TOOLCHAIN_PREFIX "${AVR_TOOLCHAIN_DIR}/bin/avr-")


# ============================================================
# 3. AVR 컴파일러 설정
# ============================================================

# C 소스 파일을 컴파일하는 avr-gcc
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc" CACHE FILEPATH "AVR C compiler")

# C++ 소스 파일을 컴파일하는 avr-g++
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++" CACHE FILEPATH "AVR C++ compiler")

# ASM 소스 파일도 avr-gcc를 통해 컴파일한다.
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc" CACHE FILEPATH "AVR ASM compiler")


# ============================================================
# 4. AVR Binutils 설정
# ============================================================

# 정적 라이브러리(.a)를 생성하는 프로그램
set(CMAKE_AR "${TOOLCHAIN_PREFIX}ar" CACHE FILEPATH "AVR archiver")

# 정적 라이브러리의 심볼 인덱스를 생성하는 프로그램
set(CMAKE_RANLIB "${TOOLCHAIN_PREFIX}ranlib" CACHE FILEPATH "AVR ranlib")

# ELF 파일을 HEX, EEPROM 등의 형식으로 변환하는 프로그램
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}objcopy" CACHE FILEPATH "AVR objcopy")

# ELF 파일의 섹션 및 어셈블리 코드를 확인하는 프로그램
set(CMAKE_OBJDUMP "${TOOLCHAIN_PREFIX}objdump" CACHE FILEPATH "AVR objdump")

# Flash, SRAM 등의 프로그램 크기를 확인하는 프로그램
set(CMAKE_SIZE_UTIL "${TOOLCHAIN_PREFIX}size" CACHE FILEPATH "AVR size")


# ============================================================
# 5. C/C++ 언어 표준
# ============================================================

# C 언어 표준을 C11로 설정한다.
set(CMAKE_C_STANDARD 11)

# C11을 사용할 수 없을 때 이전 표준으로 자동 변경하지 못하게 한다.
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ 언어 표준을 C++11로 설정한다.
set(CMAKE_CXX_STANDARD 11)

# C++11을 사용할 수 없을 때 이전 표준으로 자동 변경하지 못하게 한다.
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ============================================================
# 6. 라이브러리 및 헤더 검색 경로
# ============================================================

# CMake가 AVR용 라이브러리와 헤더를 검색할 기준 경로
set(CMAKE_FIND_ROOT_PATH "${AVR_TOOLCHAIN_DIR}")

# cmake, make 같은 실행 프로그램은 Ubuntu 시스템에서 검색한다.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 라이브러리는 AVR Toolchain 경로에서만 검색한다.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

# 헤더 파일은 AVR Toolchain 경로에서만 검색한다.
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# CMake 패키지는 AVR Toolchain 경로에서만 검색한다.
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)