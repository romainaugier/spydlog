function(set_target_options target_name)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(${SANITIZE})
            target_compile_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>)
            target_link_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>)

            target_compile_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=undefined>)
            target_link_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=undefined>)
        endif()

        set(COMPILE_OPTIONS -D_FORTIFY_SOURCES=2 -pipe -Wall -pedantic-errors $<$<CONFIG:Release,RelWithDebInfo>:-O3 -ftree-vectorizer-verbose=2> -mveclibabi=svml -mavx2 -mfma)

        target_compile_options(${target_name} PRIVATE ${COMPILE_OPTIONS})
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(${SANITIZE})
            target_compile_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>)
            target_link_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>)

            target_compile_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=undefined>)
            target_link_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=undefined>)
        endif()

        set(COMPILE_OPTIONS -D_FORTIFY_SOURCES=2 -pipe -Wall -pedantic-errors $<$<CONFIG:Release,RelWithDebInfo>:-O3 -ftree-vectorizer-verbose=2> -mveclibabi=svml -mavx2 -mfma)

        target_compile_options(${target_name} PRIVATE ${COMPILE_OPTIONS})
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        message(FATAL "Intel compiler is not supported")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        include(find_avx)

        if(${SANITIZE})
            target_compile_options(${target_name} PRIVATE $<$<CONFIG:Debug,RelWithDebInfo>:/fsanitize=address>)
        endif()

        set(COMPILE_OPTIONS /W4 /utf-8 ${AVX_FLAGS} $<$<CONFIG:Release,RelWithDebInfo>:/O2 /GF /Ot /Oy /GT /GL /Oi /Zi /Gm- /Zc:inline>)

        target_compile_options(${target_name} PRIVATE ${COMPILE_OPTIONS})
    endif()

    # Provides the macro definition DEBUG_BUILD
    target_compile_definitions(${target_name} PRIVATE $<$<CONFIG:Debug>:DEBUG_BUILD>)
endfunction()
