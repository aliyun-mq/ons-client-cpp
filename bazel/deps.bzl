load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def ons_deps():
    http_archive(
        name = "org_apache_rocketmq",
        sha256 = "9e137ee5568f3acde1bf8b4b5395da3e98bfb035fccdfbd0d1c36889f238b924",
        urls = [
            "https://github.com/apache/rocketmq-client-cpp/archive/46444c2d8056ff9117a98ff8e792008af243494b.zip",
        ],
        strip_prefix = "rocketmq-client-cpp-46444c2d8056ff9117a98ff8e792008af243494b",
    )

    http_archive(
        name = "rules_pkg",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
            "https://github.com/bazelbuild/rules_pkg/releases/download/0.5.1/rules_pkg-0.5.1.tar.gz",
        ],
        sha256 = "a89e203d3cf264e564fcb96b6e06dd70bc0557356eb48400ce4b5d97c2c3720d",
    )