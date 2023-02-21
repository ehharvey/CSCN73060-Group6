load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "NetworkServer",
    srcs = ["NetworkServer.cpp"],
    hdrs = ["NetworkServer.h"],
    visibility = ["//Server/Main:__pkg__"],
    deps = [
        "//Server/NetworkServer/Connection",
    ]
)
