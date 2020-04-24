cmake_minimum_required(VERSION 3.16)
project(QuickSort)

set(CMAKE_CXX_STANDARD 14)


function(assign INPUT index value)
    list(REMOVE_AT INPUT ${index})
    list(INSERT INPUT ${index} ${value})
    set(INPUT ${INPUT} PARENT_SCOPE)
endfunction()


function(_quick_sort INPUT low high)
    if(${low} GREATER_EQUAL ${high})
        return()
    endif()
    set(l ${low})
    set(h ${high})
    list(GET INPUT ${low} pivot)
    while(${low} LESS ${high})

        list(GET INPUT ${high} h_v)
        while((${low} LESS ${high}) AND (${h_v} GREATER ${pivot}))
            math(EXPR high "${high}-1")
            list(GET INPUT ${high} h_v)
        endwhile()
        list(GET INPUT ${high} h_v)
        assign("${INPUT}" ${low} ${h_v})

        list(GET INPUT ${low} l_v)
        while((${low} LESS ${high}) AND (${l_v} LESS ${pivot}))
            math(EXPR low "${low}+1")
            list(GET INPUT ${low} l_v)
        endwhile()
        list(GET INPUT ${low} l_v)
        assign("${INPUT}" ${high} ${l_v})
    endwhile()
    assign("${INPUT}" ${low} ${pivot})
    math(EXPR new_h "${low}-1")
    math(EXPR new_l "${low}+1")
    _quick_sort("${INPUT}" ${l} ${new_h})
    _quick_sort("${INPUT}" ${new_l} ${h})
    set(INPUT "${INPUT}" PARENT_SCOPE)

endfunction()

function(quick_sort INPUT)
    list(LENGTH INPUT len)
    math(EXPR len "${len}-1")
    _quick_sort("${INPUT}" 0 ${len})
    set(INPUT "${INPUT}" PARENT_SCOPE)
endfunction()



set(INPUT 3 4 8 1 5 7 9 6 2 0)
message(INPUT= "${INPUT}")
quick_sort("${INPUT}")
message(INPUT= "${INPUT}")






