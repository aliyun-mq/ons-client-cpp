load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def ons_deps():
    http_archive(
        name = "org_apache_rocketmq",
        sha256 = "eb79fe3da550770091ee7c0f90b9dc024a98572d981a045ba0e89ca8a644605e",
        urls = [
            "https://github.com/apache/rocketmq-client-cpp/archive/refs/tags/v5.0-alpha3.tar.gz",
        ],
        strip_prefix = "rocketmq-client-cpp-5.0-alpha3",
    )

    http_archive(
        name = "rules_pkg",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
            "https://github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
        ],
        sha256 = "a89e203d3cf264e564fcb96b6e06dd70bc0557356eb48400ce4b5d97c2c3720d",
    )