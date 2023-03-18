load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "Main",
    srcs = ["Main.cpp"],
    deps = [
        "//Client/NetworkClient",
        "//Client/CsvReader",
        "//DataProtocol",
        "@boost//:asio"
    ],
)
