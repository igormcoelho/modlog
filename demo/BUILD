
package(
    default_visibility = ["//visibility:public"],
)

cc_binary(
    name = "demo1",
    srcs = ["demo1.cpp"],
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
    copts = ["-DNDEBUG"],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo4",
    srcs = ["demo4.cpp"],
    copts = ["-DNDEBUG"],
    deps = ["//include:modlog"],
)

cc_binary(
    name = "demo5",
    srcs = ["demo5.cpp"],
    copts = ["-g", "-std=c++23"],
    linkopts = ["-lstdc++exp "],
    deps = ["//include:modlog"],
)