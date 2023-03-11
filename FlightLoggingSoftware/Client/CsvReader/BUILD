load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "CsvReader",
    srcs = ["CsvReader.cpp"],
    hdrs = ["CsvReader.h", "rapidcsv.h" ],
    visibility = ["//Client/Main:__pkg__"],
    deps = ["//DataProtocol"],
)
