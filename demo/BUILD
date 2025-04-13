
package(
    default_visibility = ["//visibility:public"],
)

cc_binary(
    name = "demo1",
    srcs = ["demo1.cpp"],
    copts = ["-DNDEBUG", "-std=c++20"],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo1_windows",
    srcs = ["demo1.cpp"],
    copts = ["/std:c++latest"],
    deps = ["//include:modlog"],
)

# only with 'import std' support!
#cc_binary(
#    name = "demo2",
#    srcs = ["demo2.cpp"],
#    deps = ["//include:modlog"],
#)

cc_binary(
    name = "demo3",
    srcs = ["demo3.cpp"],
    copts = ["-DNDEBUG", "-std=c++20"],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo4",
    srcs = ["demo4.cpp"],
    copts = ["-DNDEBUG", "-std=c++20"],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo5_stacktrace",
    srcs = ["demo5.cpp"],
    copts = ["-g", "-std=c++23", "-DMODLOG_STACKTRACE=1"],
    linkopts = ["-lstdc++exp "],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo5",
    srcs = ["demo5.cpp"],
    copts = ["-g", "-std=c++23", "-DMODLOG_STACKTRACE=0"],
    deps = ["//include:modlog"],
)