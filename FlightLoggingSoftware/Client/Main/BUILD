load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "Main",
    srcs = ["Main.cpp"],
    deps = [
        "//Client/CsvReader",
        "//Client/NetworkClient",
        "//Client/ResponseReceiver",
        "//DataProtocol",
    ],
)
