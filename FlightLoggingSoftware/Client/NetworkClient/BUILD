load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "NetworkClient",
    srcs = ["NetworkClient.cpp"],
    hdrs = ["NetworkClient.h"],
    visibility = ["//Client/Main:__pkg__"],
    deps = ["//DataProtocol", "@boost//:asio"],
)
