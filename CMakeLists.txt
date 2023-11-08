cmake_minimum_required(VERSION 3.20)
project(iDriver100)

set(PROJECT_COMPILE_DEFINES linux_630)
set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PROJECT_SOURCES_TO_REMOVE ${PROJECT_SOURCE_DIR}/src/_*.cpp)
list(REMOVE_ITEM PROJECT_SOURCES ${PROJECT_SOURCES_TO_REMOVE})

add_subdirectory(estd)
set(PROJECT_LINK_LIBRARIES estd)
set(PROJECT_COMPILE_OPTIONS -nostdinc)

# -----------------------------------------------------------------------------
# For Windows
# -----------------------------------------------------------------------------
if(WIN32)
    file(GLOB_RECURSE PROJECT_SOURCES_TO_REMOVE ${PROJECT_SOURCE_DIR}/src/*linux*.cpp)
    list(REMOVE_ITEM PROJECT_SOURCES ${PROJECT_SOURCES_TO_REMOVE})

    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} /GS-)

    set(PROJECT_LINK_LIBRARIES ${PROJECT_LINK_LIBRARIES} ntoskrnl)
    set(PROJECT_LINK_OPTIONS ${PROJECT_LINK_OPTIONS} /NXCOMPAT /NODEFAULTLIB /SUBSYSTEM:NATIVE /DRIVER)
    set(PROJECT_LINK_OPTIONS ${PROJECT_LINK_OPTIONS} /DYNAMICBASE /MANIFEST:NO /OPT:REF,ICF /ENTRY:DriverEntry)
    set(PROJECT_LINK_OPTIONS ${PROJECT_LINK_OPTIONS} /MERGE:.edata=.rdata /MERGE:.rustc=.data /INTEGRITYCHECK)

    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".sys" )
endif()

# -----------------------------------------------------------------------------
# For Linux
# -----------------------------------------------------------------------------
if(LINUX)
    file(GLOB_RECURSE PROJECT_SOURCES_TO_REMOVE ${PROJECT_SOURCE_DIR}/src/*windows*.cpp)
    list(REMOVE_ITEM PROJECT_SOURCES ${PROJECT_SOURCES_TO_REMOVE})

    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -O2 -m64 -mcmodel=kernel)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fmacro-prefix-map=./=)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -funsigned-char -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=none -m64)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -falign-loops=1 -mno-80387 -mno-fp-ret-in-387) # -mpreferred-stack-boundary=3 -falign-jumps=1
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -mskip-rax-setup)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -mtune=generic -mno-red-zone -mcmodel=kernel)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fno-asynchronous-unwind-tables -mindirect-branch-cs-prefix) # -mindirect-branch=thunk-extern -mindirect-branch-register
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -mfunction-return=thunk-extern -fno-jump-tables -mharden-sls=all -fpatchable-function-entry=16,16 -fno-delete-null-pointer-checks)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -Wframe-larger-than=1024 -fstack-protector-strong) # -fno-allow-store-data-races
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fno-omit-frame-pointer -fno-optimize-sibling-calls -ftrivial-auto-var-init=zero -fno-stack-clash-protection)
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fzero-call-used-regs=used-gpr -pg -mfentry -falign-functions=16 -Wdeclaration-after-statement) #  -mrecord-mcount
    set(PROJECT_COMPILE_OPTIONS ${PROJECT_COMPILE_OPTIONS} -fsanitize=bounds -fsanitize=shift -fsanitize=bool -fsanitize=enum)

    add_library(${PROJECT_NAME} OBJECT ${PROJECT_SOURCES})
    add_custom_command(OUTPUT ${PROJECT_NAME}.ko
        COMMAND ld -r -m elf_x86_64 -z noexecstack --build-id=sha1
            -T ${PROJECT_SOURCE_DIR}/src/module.lds $<TARGET_OBJECTS:${PROJECT_NAME}> -o ${PROJECT_NAME}.ko
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS $<TARGET_OBJECTS:${PROJECT_NAME}> VERBATIM
        COMMAND_EXPAND_LISTS)
    add_custom_target(${PROJECT_NAME}-module ALL DEPENDS ${PROJECT_NAME}.ko)
endif()

# -----------------------------------------------------------------------------
# iDriver
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINES})
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_OPTIONS})
# if(WIN32)
    target_link_options(${PROJECT_NAME} PUBLIC ${PROJECT_LINK_OPTIONS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LINK_LIBRARIES})
# endif()
