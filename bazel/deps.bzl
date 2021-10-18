load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def ons_deps():
    http_archive(
        name = "org_apache_rocketmq",
        sha256 = "d8c06bd2f8b2387eec1369da7de4c4d4ca4a5a56a5891dd9023343b14ec7a732",
        urls = [
            "https://github.com/apache/rocketmq-client-cpp/archive/refs/tags/v5.0-alpha2.tar.gz",
        ],
        strip_prefix = "rocketmq-client-cpp-5.0-alpha2",
    )

    http_archive(
        name = "rules_pkg",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
            "https://github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
        ],
        sha256 = "a89e203d3cf264e564fcb96b6e06dd70bc0557356eb48400ce4b5d97c2c3720d",
    )