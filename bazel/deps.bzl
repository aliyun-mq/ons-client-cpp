load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def ons_deps():
    http_archive(
        name = "org_apache_rocketmq",
        sha256 = "52ad1c024bbc251df99f05dc8d67edb1eb44f9fc565c803d2698facd874ba888",
        urls = [
            "https://github.com/apache/rocketmq-client-cpp/archive/refs/tags/v5.0-rc1.tar.gz",
        ],
        strip_prefix = "rocketmq-client-cpp-5.0-rc1",
    )

    http_archive(
        name = "rules_pkg",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
            "https://github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
        ],
        sha256 = "a89e203d3cf264e564fcb96b6e06dd70bc0557356eb48400ce4b5d97c2c3720d",
    )