cmake_minimum_required(VERSION 3.15)
project(Bubble_Sort_By_CMake)

set(CMAKE_CXX_STANDARD 14)

function(swap INPUT first second)
    if(${first} GREATER ${second})
        set(temp ${first})
        set(first ${second})
        set(second ${temp})
        unset(temp)
    endif()
    list(GET INPUT ${first} FIRST)
    list(GET INPUT ${second} SECOND)
    list(REMOVE_AT INPUT ${first})
    list(INSERT INPUT ${first} ${SECOND})
    list(REMOVE_AT INPUT ${second})
    list(INSERT INPUT ${second} ${FIRST})

    set(INPUT ${INPUT} PARENT_SCOPE) # Return the value
endfunction()


function(bubble_sort INPUT)
    # Check Length
    list(LENGTH INPUT len)
    if(${len} LESS 2)
        return()
    endif()
    math(EXPR len "${len}-2")
    foreach(i RANGE ${len})
        list(GET INPUT ${i} v_x)
        math(EXPR limit "${len}-${i}")
        foreach(j RANGE ${limit})
            list(GET INPUT ${j} v_j)
            math(EXPR j_1 "${j}+1")
            list(GET INPUT ${j_1} v_j_1)
            if(${v_j} GREATER ${v_j_1})
                swap("${INPUT}" ${j} ${j_1})
            endif()
        endforeach()
    endforeach()
    set(INPUT ${INPUT} PARENT_SCOPE)
endfunction()


set(INPUT 3 4 8 1 5 7 9 6 2 0)
message(INPUT= "${INPUT}")
bubble_sort("${INPUT}")
message(OUTPUT= "${INPUT}")


